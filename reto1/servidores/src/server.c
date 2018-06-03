#include "fsm.h"
#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "server.h"

#define	REBOUND_TICK 130/portTICK_RATE_MS
volatile portTickType xLastISRTick0=0;

int medida_sensor;
int carga_max;
int carga_min;
int contador_par;
int maximo_par;
int f;

enum fsm_state {
APAGADO,
ENCENDIDO,
MIDE_CARGA,
IMPAR,
PAR
};

static fsm_trans_t control[]={
{MIDE_CARGA,medir,MIDE_CARGA,medida},
{MIDE_CARGA,carga_maxima,ENCENDIDO,NULL},
{MIDE_CARGA,sin_carga,APAGADO,NULL},
{ENCENDIDO,seleccion,PAR,enciende_par},
{ENCENDIDO,!seleccion,IMPAR,enciende_impar},
{PAR,1,MIDE_CARGA,cambiar_sel},
{IMPAR,1,MIDE_CARGA,cambiar_sel},
{APAGADO,seleccion,PAR,apaga_par},
{APAGADO,!seleccion,IMPAR,apaga_impar},
{-1,NULL,-1,NULL},
};

#define PERIOD_TICK 100/portTICK_RATE_MS

void servidor(void *ignore){
  fsm_t* fsm=fsm_new(control);
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  while(true){
    fsm_fire(fsm);
    vTaskDelayUntil(&xLastWakeTime,PERIOD_TICK);
  }
}

int medir (fsm_t *this){
  if(medida_sensor<=carga_max && medida_sensor>=carga_min){
    return 1;
  }
  return 0;
}

int carga_maxima (fsm_t *this){
  if(medida_sensor>=carga_max){
    return 1;
  }
  return 0;
}

int sin_carga (fsm_t *this){
  if(medida_sensor<=carga_min){
    return 1;
  }
  return 0;
}

int seleccion (fsm_t *this){
  return !f;
}


void medida (fsm_t *this){
  //act valor medida_sensor
}

void enciende_par (fsm_t *this){
  //enciende server par
  //aumenta contador server par
}

void enciende_impar (fsm_t *this){
  //enciende server impar
}

void apaga_par (fsm_t *this){
  //apaga server par
  //aumenta contador server par
}

void cambiar_sel (fsm_t *this){
  if(contador_par==maximo_par){
    f=1;
  }
  f=0;
}
