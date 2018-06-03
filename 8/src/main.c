#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "fsm.h"

#define t_antir  130/portTICK_RATE_MS //tiempo de antirrebotes
#define t_unseg  1000/portTICK_RATE_MS //tiempo máximo entre pulsaciones
#define max_espera 10000/portTICK_RATE_MS //tiempo máximo espera entre dígitos

#define ETS_GPIO_INTR_ENABLE()  _xt_isr_unmask(1 << ETS_GPIO_INUM)  //ENABLE INTERRUPTS
#define ETS_GPIO_INTR_DISABLE() _xt_isr_mask(1 << ETS_GPIO_INUM)    //DISABLE INTERRUPTS
#define GPIO_UP 0
#define GPIO_DOWN 15
#define LED 2

volatile int a = 0;
volatile int b = 0;
volatile int c = 0;
//volatile int i = 0;
volatile int codigo_complet = 0;  //Boolean 0-> codigo incompleto; 1-> codigo completo
volatile int codigo_correct = 0;  //Boolean 0-> codigo incorrecto; 1-> codigo correcto
volatile int code_index;          //Entero que indica en que dígito estás, (0,1,2)
volatile int code_inserted [3];   //Codigo que estas introduciendo
volatile int flag_digito=0;
const int codigo[3] = {1,1,1};    //Clave de la alarma


//portTick nextTimeout;
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
  ALARMA,
//  ALARMA_OFF,
//  ALARMA_ON
};

/*enum fsm_state_2 {
  ALARMA_OFF,
  ALARMA_ON
};*/

//INTERUPCION
/*void isr_gpio(void* arg) {
//  static portTickType xLastISRTick0 = 0;
//  static portTickType xLastISRTick15 = 0;
  uint32 estado = GPIO_REG_READ(GPIO_STATUS_ADDRESS);          //READ STATUS OF INTERRUPT
  portTickType ahora = xTaskGetTickCount ();
  if((ahora-b)>=t_unseg)
    flag_digito=1;
  for (int j = 0; j<3; j++){
    if (code_index>2||code_inserted[code_index]>10)
      codigo_correct=0;
    else if(code_inserted[j]!=codigo[j])
      codigo_correct=0;
    else
      codigo_correct=1;
  }
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, estado);       //CLEAR THE estado IN THE W1 INTERRUPT REGISTER
}*/

/*
* Aquí encendemos la alarma y aplicamos el antirrebotes.
* También guardamos el momento en el que se ha pulsado.
*/
int boton_alarma (fsm_t* this) {
  int flag=0;
    if(!GPIO_INPUT_GET(0)){
    portTickType ahora = xTaskGetTickCount();
    if (ahora < a){
      flag = 0;
      a = ahora + t_antir;
      b = ahora + t_unseg;
      return flag;
    }
    a = ahora + t_antir;
    b = ahora + t_unseg;
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
* Comprueba que si se ha pasado el tiempo de espera entre pulsaciones.
*/
int cuenta (fsm_t* this){
  int flag_cnt = 0;
  portTickType ahora = xTaskGetTickCount();
  if ((ahora-b) < max_espera){
    flag_cnt=1;
    return flag_cnt;
  }
    flag_cnt = 0;
    return flag_cnt;
}
//Mirar si funciona o no
/*int timeout (fsm_t *this) { //modificar cuenta
  portTick now = xTaskGetTickCount();
  if (nextTimeout > 0)
  return 0;
  if (nextTimeout < now)
  return 1;
  return 0;
}*/

/*int codigo_incorrecto (fsm_t *this){
  for (int i = 0; i<3; i++){
     if(code_inserted[i]!=codigo[i])
     return 1;
  }
  return 0;
}*/

/*void comprobar_codigo(fsm_t* this){
  for (int j = 0; j<3; j++){
     if(code_inserted[j]!=codigo[j])
     codigo_correct=0;
     else
     codigo_correct=1;
  }
}*/

/*
* Enciende la alarma
*/
void activar_alarma (fsm_t *this) {
  GPIO_OUTPUT_SET(2, 0);
}
/*
* Apaga la alarma
*/
void desactivar_alarma (fsm_t *this) {
  GPIO_OUTPUT_SET(2, 1);
}
/*//NUEVO
int codigo_tehaspasado (fsm_t *this) {
  if (code_index>2||code_inserted[code_index]>10)
    return 1;
  return 0;
}*/
void update_code (fsm_t *this){
  portTickType ahora = xTaskGetTickCount();
  if ((ahora-b)<=t_unseg){
    code_inserted[code_index]++;
  }
  /*if (((ahora-b)>t_unseg)&&(i<3)){
    i++;
    code_inserted[i]++;
  }
  if (i==3){
    codigo_complet=1;
  }*/
//  nextTimeout =  xTaskGetTickCount() + t_unseg;
}

/*
* Enciende el led (detector)
*/
void sonar_alarma (fsm_t *this) {
    GPIO_OUTPUT_SET(2, 0);
}


void next_index (fsm_t *this){
  if (code_index<3){
    code_index++;
    //nextTimeout = 0xFFFFFFFFF; //si uso esto el if de arriba no hace falta
  }
}

void reset_flags(fsm_t *this) {
  code_index=0;
  flag_digito=0;
  codigo_complet=0;
  codigo_correct=0;
  for(int j=0; j<3; j++){
    code_inserted[j]=0;
  }
}

int codigo_correcto (fsm_t *this) {
  return codigo_correct;
}

int codigo_incorrecto (fsm_t *this) {
  if(codigo_correct==0)
    return 1;
  return 0;
}

int codigo_completo (fsm_t *this) {
  return codigo_complet;
}

int digito_completo (fsm_t *this) {
  return flag_digito;
}

/*
* Array de transiciones
*/
static fsm_trans_t trans[] = {
  { ALARMA, boton_alarma, ALARMA, update_code },
  { ALARMA, digito_completo, ALARMA, next_index },
  { ALARMA, cuenta, ALARMA, reset_flags },
//  { ALARMA, codigo_tehaspasado, ALARMA, reset_flags },
//  { ALARMA_OFF, mirar_flag, ALARMA_OFF, reset_flags },
  { ALARMA, codigo_correcto, ALARMA, activar_alarma },
  {-1, NULL, -1, NULL },
};
/*static fsm_trans_t en_off[] = {
  { ALARMA, boton_alarma, ALARMA, update_code },
  { ALARMA, cuenta, ALARMA, reset_flags },
  { ALARMA, codigo_tehaspasado, ALARMA, reset_flags },
//  { ALARMA_OFF, mirar_flag, ALARMA_OFF, reset_flags },
  { ALARMA, codigo_completo, ALARMA, comprobar_codigo },
  {-1, NULL, -1, NULL },
};*/
/*static fsm_trans_t para_activar[] = {
 { ALARMA_OFF, codigo_correcto, ALARMA_ON,  activar_alarma },
 { ALARMA_ON, codigo_correcto, ALARMA_OFF, desactivar_alarma },
 { ALARMA_ON, detector, ALARMA_ON, sonar_alarma },
 {-1, NULL, -1, NULL },
};*/



#define PERIOD_TICK 100/portTICK_RATE_MS

void inter(void* ignore)  {
    fsm_t* fsm1 = fsm_new(trans);
    //fsm_t* fsm2 = fsm_new(para_activar);
    reset_flags(fsm1);
  //  desactivar_alarma(fsm2);

//    gpio_intr_handler_register((void*)isr_gpio, NULL);
//    gpio_pin_intr_state_set(0, GPIO_PIN_INTR_NEGEDGE);
//    gpio_pin_intr_state_set(15, GPIO_PIN_INTR_POSEDGE);
//    ETS_GPIO_INTR_ENABLE();
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (true) {
      fsm_fire(fsm1);
    //  fsm_fire(fsm2);
      vTaskDelayUntil(&xLastWakeTime, PERIOD_TICK);
  }
}

void user_init(void)
{
    PIN_FUNC_SELECT(GPIO_PIN_REG_15, FUNC_GPIO15);
    xTaskCreate(&inter, "startup", 2048, NULL, 1, NULL);


}
