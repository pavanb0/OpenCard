
#include "Arduino.h"
#include <hardware_drivers/ldr/ldr.h>
#include <modules/card_throw.h>
#include <modules/card_throw_state.h>
#include <hardware_drivers/stepper/stepper.h>
#include <modules/card_throw_queue.h>
#include "card_controller_queue.h"
#include <ui/ui_menue.h>          

volatile bool isDeckEmpty = false;

static void pushProgress(const CardControllerResponse &r)
{
    GameProgress gp;
    gp.currentPlayer  = r.currentPlayer;
    gp.currentCard    = r.currentCard;
    gp.totalPlayers   = r.totalPlayers;
    gp.cardsPerPlayer = r.cardsPerPlayer;
    gp.deckEmpty      = r.deckEmpty;
    gp.jamDetected    = r.jamDetected;
    gp.finished       = r.finished;
    menuSetProgress(gp);
}

void gameControllerTask(void *pvArguments)
{
    Serial.println("Game Controller Task Started");

    for (;;)
    {
        CardControllerRequest req;
        if (xQueueReceive(cardControllerRequest, &req, portMAX_DELAY) != pdTRUE)
            continue;

        if (!req.isSendTask)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
            continue;
        }

        isDeckEmpty = false;
        const int angle = (req.playerCount > 0) ? (360 / req.playerCount) : 0;

        CardControllerResponse resp = {};
        resp.totalPlayers   = req.playerCount;
        resp.cardsPerPlayer = req.cardsPerPlayer;

        for (int round = 0; round < req.cardsPerPlayer && !isDeckEmpty; round++)
        {
            resp.currentCard = round + 1;

            for (int p = 0; p < req.playerCount && !isDeckEmpty; p++)
            {
                resp.currentPlayer  = p + 1;
                resp.deckEmpty      = false;
                resp.jamDetected    = false;
                resp.finished       = false;

                pushProgress(resp);
                xQueueSend(cardControllerResponse, &resp, 0);

                ThrowCommand cmd = CMD_CARD_THROW;
                xQueueSend(throwCommandQueue, &cmd, portMAX_DELAY);

                ThrowResult result;
                if (xQueueReceive(throwResultQueue, &result, portMAX_DELAY) != pdTRUE)
                    continue;

                switch (result)
                {
                case RESULT_SUCCESS:
                    Serial.println("Card thrown OK");
                    stepperMove(angle);
                    break;

                case RESULT_EMPTY:
                    Serial.println("Deck empty");
                    isDeckEmpty        = true;
                    resp.deckEmpty     = true;
                    pushProgress(resp);
                    xQueueSend(cardControllerResponse, &resp, 0);
                    break;

                case RESULT_JAM:
                    Serial.println("Card JAMMED");
                    resp.jamDetected   = true;
                    pushProgress(resp);
                    xQueueSend(cardControllerResponse, &resp, 0);
                    for (;;) vTaskDelay(1000 / portTICK_PERIOD_MS);
                    break;

                default:
                    break;
                }
            }
        }

        resp.finished = true;
        pushProgress(resp);
        xQueueSend(cardControllerResponse, &resp, 0);

        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}

