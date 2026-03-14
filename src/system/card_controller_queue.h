#ifndef CARD_CONTROLLER_QUEUE_H
#define CARD_CONTROLLER_QUEUE_H
#include "Arduino.h"

typedef struct {
    bool isSendTask;
    int playerCount;
    int cardsPerPlayer;
} CardControllerRequest;

typedef struct{
    
    int currentPlayer;
    int currentCard;
    int totalPlayers;
    int cardsPerPlayer;

    bool deckEmpty;
    bool jamDetected;
    bool finished;

} CardControllerResponse;

extern QueueHandle_t cardControllerRequest;
extern QueueHandle_t cardControllerResponse;

void initCardControllerQueue();

#endif