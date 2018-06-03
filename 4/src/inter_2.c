#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "fsm.h"

#define t_antir  130/portTICK_RATE_MS //tiempo de antirrebotes
volatile int a = 0;
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
};
/*
* Aquí pulsamos los botones y aplicamos el antirrebotes.
*/
int button_pressed (fsm_t *this) {
int flag=0;
  if(!GPIO_INPUT_GET(0)||GPIO_INPUT_GET(15)){

  portTickType ahora = xTaskGetTickCount();
  if (ahora < a){
    a = ahora + t_antir;
    return flag;
  }
  a = ahora + t_antir;
  flag = 1;
  return flag;
}
return flag;
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
* Array de transiciones
*/
static fsm_trans_t interruptor[] = {
  { LED_OFF, button_pressed, LED_ON,  led_on },
  { LED_ON, button_pressed, LED_OFF,  led_off },
  {-1, NULL, -1, NULL },
};

#define PERIOD_TICK 100/portTICK_RATE_MS

void inter(void* ignore)  {
  fsm_t* fsm = fsm_new(interruptor);
  led_off(fsm);
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
  PIN_FUNC_SELECT(GPIO_PIN_REG_14, FUNC_GPIO14);
  xTaskCreate(&inter, "startup", 2048, NULL, 1, NULL);

}
