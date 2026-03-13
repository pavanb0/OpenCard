#include "card_throw_queue.h"

QueueHandle_t throwCommandQueue;
QueueHandle_t throwResultQueue;

void initThrowQueues() {
    throwCommandQueue = xQueueCreate(5,sizeof(ThrowCommand));
    throwResultQueue = xQueueCreate(5,sizeof(ThrowResult));
};
