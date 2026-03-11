#include "Arduino.h"
#include <hardware_drivers/ldr/ldr.h>
#include <modules/card_throw.h>
#include <modules/card_throw_state.h>
#include <hardware_drivers/stepper/stepper.h>

static int player = 0;
static int cards_per_player = 0;

void startGame(int players, int cards_per_player)
{
    player = players;
    cards_per_player = cards_per_player;
};

void gameControllerTask(void *pvArguments)
{
    for (;;)
    {
        if (player != 0 && cards_per_player != 0 && getCardThrowState() == STATE_IDLE)
        {

            while (cards_per_player > 0)
            {
                for (uint16_t circleCount = 0; circleCount < cards_per_player; circleCount++)
                {
                    for(u_int16_t playerRotate = 0; playerRotate < player; playerRotate++){
                        setCardThrowState(STATE_RUNNING);
                        vTaskDelay(3300 / portTICK_PERIOD_MS);
                        if(getCardThrowState() == STATE_CARD_SUCCESS){
                            // stepperMove();
                        }
                    }
                    
                }
            }
        }

        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
};
