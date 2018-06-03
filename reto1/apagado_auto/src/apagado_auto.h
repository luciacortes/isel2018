#include "fsm.h"

void on (fsm_t *this);
void off (fsm_t *this);

int timeout (fsm_t *this);
int sensor (fsm_t *this);
void inter(void *ignore);
