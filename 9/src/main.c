#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "fsm.h"
#include "stdio.h"


#define pausa 5000/portTICK_RATE_MS //Tiempo para apagar el led.
#define t_antir  130/portTICK_RATE_MS //Tiempo de antirrebotes.
volatile int a = 0;
volatile int b = 0;
volatile int c = 0;
volatile int d = 0;
#define GPIO_DOWN 13
#define GPIO_UP 0
#define GPIO_DOWN 15
#define LED 2
#define LED 12
/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

/*
* Array de estados.
*/
enum fsm_state {
  LED_ON,
  LED_OFF,
  ALARMA_ON,
  ALARMA_OFF,
  ALARMA_SONANDO
};

/*enum fsm_state2 {
  ALARMA_ON,
  ALARMA_OFF,
  ALARMA_SONANDO
};*/

/*
* Aquí pulsamos los botones y aplicamos el antirrebotes.
*/
int button_pressed (fsm_t *this) {
  int flag=0;
    if(GPIO_INPUT_GET(15)){
    portTickType ahora = xTaskGetTickCount();
    if (ahora < a){
      a = ahora + t_antir;
      b = ahora + pausa;
      return flag;
    }
    a = ahora + t_antir;
    b = ahora + pausa;
    flag = 1;
    return flag;
  }
  return flag;
}

/*
* Apaga el led cuando pasa el tiempo establecido
*/
int time_out(fsm_t *this){
  int flag_cnt = 0;
  portTickType tiempo = xTaskGetTickCount();
  if (b < tiempo){
    flag_cnt=1;
    return flag_cnt;
  }
    flag_cnt = 0;
    return flag_cnt;
}

/*
* Enciende el led
*/
void led_on (fsm_t *this) {
  GPIO_OUTPUT_SET(2, 0);
}

/*
* Apaga el led
*/
void led_off (fsm_t *this) {
  GPIO_OUTPUT_SET(2, 1);
}

/*
* Aquí encendemos la alarma y aplicamos el antirrebotes.
*/
int boton_alarma (fsm_t *this) {
  int flag=0;
    if(!GPIO_INPUT_GET(0)){
    portTickType ahora = xTaskGetTickCount();
    if (ahora < c){
      flag = 0;
      c = ahora + t_antir;
      return flag;
    }
    c = ahora + t_antir;
    flag = 1;
    return flag;
  }
    return flag;
  }

/*
* Aquí encendemos el detector y aplicamos el antirrebotes.
*/
int detector (fsm_t *this){
int flag=0;
  if(GPIO_INPUT_GET(13)){
  portTickType ahora = xTaskGetTickCount();
  if (ahora < d){
    flag = 0;
    d= ahora + t_antir;
    return flag;
  }
  d = ahora + t_antir;
  flag = 1;
  return flag;
}
  return flag;
}

/*
* Enciende la alarma
*/
void activar_alarma (fsm_t *this) {
  puts("Alarma activada");
  GPIO_OUTPUT_SET(12, 1);
}
/*
* Apaga la alarma
*/
void desactivar_alarma (fsm_t *this) {
  GPIO_OUTPUT_SET(12, 1);
  puts("Alarma desactivada");
}
/*
* Enciende el led (detector)
*/
void sonar_alarma (fsm_t *this) {
  GPIO_OUTPUT_SET(12, 0);
  puts("Alarma sonando");
}



/*
* Array de transiciones interruptor
*/
static fsm_trans_t interruptor[] = {
  { LED_OFF, button_pressed, LED_ON,  led_on },
  { LED_ON, time_out, LED_OFF,  led_off },
  {-1, NULL, -1, NULL },
};

/*
* Array de transiciones alarma
*/
static fsm_trans_t alarma[] = {
  { ALARMA_OFF, boton_alarma, ALARMA_ON,  activar_alarma },
  { ALARMA_ON, boton_alarma, ALARMA_OFF, desactivar_alarma },
  { ALARMA_ON, detector, ALARMA_SONANDO, sonar_alarma },
  { ALARMA_SONANDO, boton_alarma, ALARMA_ON, activar_alarma },
  {-1, NULL, -1, NULL },
};




#define PERIOD_TICK 200/portTICK_RATE_MS
void inter(void* ignore)  {
  fsm_t* fsm1 = fsm_new(interruptor);
  fsm_t* fsm2 = fsm_new(alarma);
  led_off(fsm1);
  activar_alarma(fsm2);
  portTickType xLastWakeTime;
  while (true) {
    xLastWakeTime = xTaskGetTickCount();
    fsm_fire(fsm2);
    fsm_fire(fsm1);
    vTaskDelayUntil(&xLastWakeTime, PERIOD_TICK);
  }
}

void user_init(void)
{
    PIN_FUNC_SELECT(GPIO_PIN_REG_15, FUNC_GPIO15);
    PIN_FUNC_SELECT(GPIO_PIN_REG_13, FUNC_GPIO13);
//    GPIO_Mode_Input_13
    xTaskCreate(&inter, "startup", 2048, NULL, 1, NULL);

}
