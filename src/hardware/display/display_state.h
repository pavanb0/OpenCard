#ifndef DISPLAY_STATE_H
#define DISPLAY_STATE_H

#include <Arduino.h>
typedef enum
{
    DISPLAY_HOME_LOADING,
    DISPLAY_MENUE_DEFAULT_ERROR,
    DISPLAY_MENUE,
    DISPLAY_HOME,
    DISPLAY_HOMING
} display_States;

extern volatile display_States displayState;
#endif