#ifndef MOTOR_STATE_H
#include <Arduino.h>
typedef enum {
    MOTOR_IDLE = 0,
    MOTOR_SOFT_START,
    MOTOR_SOFT_STOP,
    MOTOR_RUNNING
} motor_State;

extern volatile motor_State motorState;
#endif