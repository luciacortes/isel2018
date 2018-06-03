#include "fsm.h"
#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "apagado_auto.h"

#define	REBOUND_TICK 130/portTICK_RATE_MS
volatile portTickType xLastISRTick0=0;
#define	TIMEOUT 2000/portTICK_RATE_MS
volatile portTickType xLastISRTick1=0;

enum fsm_state {
LIGHT_ON,
LIGHT_OFF,
};

static fsm_trans_t luz[]={
{LIGHT_OFF,sensor,LIGHT_ON,on},
{LIGHT_ON,timeout,LIGHT_OFF,off},
{-1,NULL,-1,NULL},
};

#define PERIOD_TICK 100/portTICK_RATE_MS

void inter(void *ignore){
  fsm_t* fsm=fsm_new(luz);
  off(fsm);
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  while(true){
    fsm_fire(fsm);
    vTaskDelayUntil(&xLastWakeTime,PERIOD_TICK);
  }
}

int sensor (fsm_t *this){
  portTickType now = xTaskGetTickCount();
  if(!GPIO_INPUT_GET(0) || GPIO_INPUT_GET(15) ){
    if(now > xLastISRTick0){
      xLastISRTick0 = now + REBOUND_TICK;
      return 0;
    }

      return 1;
  }
  return 0;
}

int timeout (fsm_t *this){

  portTickType now = xTaskGetTickCount();

    if(now > xLastISRTick1){
      xLastISRTick1 = now + TIMEOUT;
      return 1;
    }
    if(now < xLastISRTick1){
        return 0;
    }
  return 0;
}

void on (fsm_t *this){
  GPIO_OUTPUT_SET(2,0);
}

void off (fsm_t *this){
  GPIO_OUTPUT_SET(2,1);
}
