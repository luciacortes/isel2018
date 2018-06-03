#include "fsm.h"

int medir (fsm_t *this);
int carga_maxima (fsm_t *this);


int sin_carga (fsm_t *this);

int seleccion (fsm_t *this);

void medida (fsm_t *this);

void enciende_par (fsm_t *this);

void enciende_impar (fsm_t *this);

void apaga_par (fsm_t *this);

void cambiar_sel (fsm_t *this);

void servidor(void *ignore);
