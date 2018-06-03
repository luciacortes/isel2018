#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "fsm.h"

#define t_antir  130/portTICK_RATE_MS //tiempo de antirrebotes
#define contador  1000/portTICK_RATE_MS //tiempo máximo entre pulsaciones
volatile int a = 0;
volatile int b = 0;
volatile int c = 0;
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
//GPIO 0: ARMAR/DESARMAR ALARMA
//GPIO 2: LED (SE ENCIENDE SI ALARMA ACTIVADA Y PRESENCIA)
//GPIO 15: PRESENCIA/NO PRESENCIA

/*
* Array de estados.
*/
enum fsm_state {
  ALARMA_PRE_ON_1,
  ALARMA_PRE_ON_2,
  ALARMA_PRE_ON_3,
  ALARMA_ON,
  ALARMA_OFF,
  ALARMA_PRE_OFF_1,
  ALARMA_PRE_OFF_2,
  ALARMA_PRE_OFF_3,
  ALARMA_SONANDO

};

/*
* Aquí encendemos la alarma y aplicamos el antirrebotes.
* También guardamos el momento en el que se ha pulsado.
*/
int boton_alarma (fsm_t *this) {
  int flag=0;
    if(!GPIO_INPUT_GET(0)){

    portTickType ahora = xTaskGetTickCount();
    if (ahora < a){
      flag = 0;
      a = ahora + t_antir;
      b = ahora + contador;
      return flag;
    }
    a = ahora + t_antir;
    b = ahora + contador;
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
    if(GPIO_INPUT_GET(15)){

    portTickType ahora = xTaskGetTickCount();
    if (ahora < c){
      flag = 0;
      c= ahora + t_antir;
      return flag;
    }
    c = ahora + t_antir;
    flag = 1;
    return flag;
  }
    return flag;
}
/*
* Enciende la alarma
*/
void activar_alarma (fsm_t *this) {
  GPIO_OUTPUT_SET(2, 1);
}
/*
* Apaga la alarma
*/
void desactivar_alarma (fsm_t *this) {
  GPIO_OUTPUT_SET(2, 1);
}

/*
* Comprueba que si se ha pasado el tiempo de espera entre pulsaciones.
*/
int cuenta (fsm_t* this){
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
* Enciende el led (detector)
*/
void sonar_alarma (fsm_t *this) {
    GPIO_OUTPUT_SET(2, 0);
}

/*
* Array de transiciones
*/
static fsm_trans_t interruptor[] = {
  { ALARMA_OFF, boton_alarma, ALARMA_PRE_ON_1,  desactivar_alarma },
  { ALARMA_PRE_ON_1, boton_alarma, ALARMA_PRE_ON_2,  desactivar_alarma },
  { ALARMA_PRE_ON_2, boton_alarma, ALARMA_PRE_ON_3,  desactivar_alarma },
  { ALARMA_PRE_ON_3, boton_alarma, ALARMA_ON,  activar_alarma },

  { ALARMA_PRE_ON_1, cuenta, ALARMA_OFF,  desactivar_alarma },
  { ALARMA_PRE_ON_2, cuenta, ALARMA_OFF,  desactivar_alarma },
  { ALARMA_PRE_ON_3, cuenta, ALARMA_OFF,  desactivar_alarma },

  { ALARMA_ON, detector, ALARMA_SONANDO, sonar_alarma },
  { ALARMA_SONANDO, boton_alarma, ALARMA_ON, activar_alarma },

  { ALARMA_ON, boton_alarma, ALARMA_PRE_OFF_1, activar_alarma },
  { ALARMA_PRE_OFF_1, boton_alarma, ALARMA_PRE_OFF_2, activar_alarma },
  { ALARMA_PRE_OFF_2, boton_alarma, ALARMA_PRE_OFF_3, activar_alarma },
  { ALARMA_PRE_OFF_3, boton_alarma, ALARMA_OFF, desactivar_alarma },

  { ALARMA_PRE_OFF_1, cuenta, ALARMA_ON,  activar_alarma },
  { ALARMA_PRE_OFF_2, cuenta, ALARMA_ON,  activar_alarma },
  { ALARMA_PRE_OFF_3, cuenta, ALARMA_ON,  activar_alarma },

  {-1, NULL, -1, NULL },
};



#define PERIOD_TICK 100/portTICK_RATE_MS

void inter(void* ignore)  {
  fsm_t* fsm = fsm_new(interruptor);
  desactivar_alarma(fsm);
  portTickType xLastWakeTime;
  while (true) {
    xLastWakeTime = xTaskGetTickCount();
    fsm_fire(fsm);
    vTaskDelayUntil(&xLastWakeTime, PERIOD_TICK);
  }
}

void user_init(void)
{
    PIN_FUNC_SELECT(GPIO_PIN_REG_15, FUNC_GPIO15);
    xTaskCreate(&inter, "startup", 2048, NULL, 1, NULL);


}
