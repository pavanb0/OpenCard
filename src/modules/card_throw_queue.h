#ifndef THROW_QUEUE_H
#define THROW_QUEUE_H
#include "Arduino.h"

typedef enum{
    CMD_CARD_THROW,
    CMD_CARD_HALT
} ThrowCommand;

typedef enum{
    RESULT_SUCCESS,
    RESULT_JAM,
    RESULT_EMPTY
}ThrowResult;

extern QueueHandle_t throwCommandQueue;
extern QueueHandle_t throwResultQueue;

void initThrowQueues();

#endif