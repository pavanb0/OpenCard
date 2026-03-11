#ifndef CARD_THROW_STATE_H
#define CARD_THROW_STATE_H

#include <Arduino.h>
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_CARD_JAMMED,
    STATE_CARD_EMPTY,
    STATE_CARD_SUCCESS
} card_throw_sates;



extern volatile card_throw_sates cardThrowState;

void setCardThrowState(card_throw_sates state)
{
    cardThrowState = state;
}

card_throw_sates getCardThrowState()
{
    return cardThrowState;
}
#endif