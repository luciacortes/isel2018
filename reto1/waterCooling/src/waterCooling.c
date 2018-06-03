#include "fsm.h"
#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "waterCooling.h"

#define	REBOUND_TICK 130/portTICK_RATE_MS
volatile portTickType xLastISRTick0=0;

int temp_sensor;
int temp_1;
int temp_2;
int temp_3;


enum fsm_state {
REPOSO,
TUBERIA1,
TUBERIA2,
TUBERIA3
};

static fsm_trans_t control[]={
{REPOSO,limite1,TUBERIA1,act_t1},
{TUBERIA1,ideal,REPOSO,desactivado},
{TUBERIA1,limite2,TUBERIA2,act_t2},
{TUBERIA2,limite1,TUBERIA1,act_t1},
{TUBERIA2,limite3,TUBERIA3,act_t3},
{TUBERIA3,limite2,TUBERIA2,act_t2},
{-1,NULL,-1,NULL},
};

#define PERIOD_TICK 100/portTICK_RATE_MS

void waterCooling(void *ignore){
  fsm_t* fsm=fsm_new(control);
  desactivado(fsm);
  portTickType xLastWakeTime;

  while(true){
    xLastWakeTime = xTaskGetTickCount();
    fsm_fire(fsm);
    vTaskDelayUntil(&xLastWakeTime,PERIOD_TICK);
  }
}

int limite1 (fsm_t *this){
if(temp_sensor>=temp_1 && temp_sensor<temp_2){
  return 1;
}
  return 0;
}

int limite2 (fsm_t *this){
if(temp_sensor>=temp_2 && temp_sensor<temp_3){
  return 1;
}
  return 0;
}

int limite3 (fsm_t *this){
if(temp_sensor>=temp_3){
  return 1;
}
  return 0;
}

int ideal (fsm_t *this){
if(temp_sensor<temp_1){
  return 1;
}
  return 0;
}

void act_t1 (fsm_t *this){
  //Activacion de una tuberia, se mantienen las otras 2 desactivadas
}

void act_t2 (fsm_t *this){
  //Activacion de dos tuberias, se mantiene 1 desactivada
}

void act_t3 (fsm_t *this){
  //Activacion de las 3 tuberias
}

void desactivado(fsm_t *this){
  //Apagadas todas
}
