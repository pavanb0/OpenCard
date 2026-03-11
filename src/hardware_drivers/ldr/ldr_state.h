#ifndef LDR_STATE_H
#define LDR_STATE_H

#include <Arduino.h>

typedef struct {
    volatile bool gantryClear;
    volatile bool stackClear;
} ldr_state;

extern volatile ldr_state ldrState;
extern volatile long gantryLdrFallDetect; // detects fall which means card in it record the time
extern volatile long gantryLdrFallRemovedDetected; // fall is removed from the ldr its high again record both times

#endif