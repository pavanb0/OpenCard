#ifndef LDR_STATE_H
#define LDR_STATE_H

#include <Arduino.h>

typedef struct {
    volatile bool gantryClear;
    volatile bool stackClear;
} ldr_state;

extern volatile ldr_state ldrState;

#endif