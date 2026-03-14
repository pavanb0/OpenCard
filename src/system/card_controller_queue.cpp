#include "card_controller_queue.h"

QueueHandle_t cardControllerRequest;
QueueHandle_t cardControllerResponse;

void initCardControllerQueue() {
    cardControllerRequest = xQueueCreate(5,sizeof(CardControllerRequest));
    cardControllerResponse = xQueueCreate(5,sizeof(CardControllerResponse));
};

