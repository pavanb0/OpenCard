// #include "Arduino.h"
// #include <hardware_drivers/ldr/ldr.h>
// #include <modules/card_throw.h>
// #include <modules/card_throw_state.h>
// #include <hardware_drivers/stepper/stepper.h>

// static int player = 4;
// static int cards_per_player = 5;

// void startGame(int players, int cards_per_player)
// {
//     player = players;
//     cards_per_player = cards_per_player;
// };

// void gameControllerTask(void *pvArguments)
// {
//     for (;;)
//     {
//         if (player != 0 && cards_per_player != 0 && getCardThrowState() == STATE_IDLE)
//         {
//             if (getCardThrowState() == STATE_CARD_JAMMED)
//             {
//                 break;
//             }

//             for (uint16_t circleCount = 0; circleCount < cards_per_player; circleCount++)
//             {
//                 if (getCardThrowState() == STATE_CARD_JAMMED)
//                 {
//                     break;
//                 }
//                 Serial.println("outer Circle ");
//                 for (u_int16_t playerRotate = 0; playerRotate < player; playerRotate++)
//                 {
//                     setCardThrowState(STATE_RUNNING);
//                     vTaskDelay(3300 / portTICK_PERIOD_MS);
//                     if (getCardThrowState() == STATE_CARD_SUCCESS)
//                     {

//                         stepperMove(360 / player);
//                         vTaskDelay((200 * (360 / player)) / portTICK_PERIOD_MS);
//                     }
//                     else if (getCardThrowState() == STATE_CARD_JAMMED)
//                     {
//                         Serial.println("Card Jammed");
//                         break;
//                     }
//                 }

//             }

//         }

//         vTaskDelay(300 / portTICK_PERIOD_MS);
//     }
// };

// #include "Arduino.h"
// #include <hardware_drivers/ldr/ldr.h>
// #include <modules/card_throw.h>
// #include <modules/card_throw_state.h>
// #include <hardware_drivers/stepper/stepper.h>

// static int player = 2;
// static int cards_per_player = 3;

// void startGame(int players, int cardsPerPlayer)
// {
//    // Serial.println("startGame() called");
//     // Serial.print("Players: ");
//     // Serial.println(players);
//     // Serial.print("Cards per player: ");
//     // Serial.println(cardsPerPlayer);

//     player = players;
//     cards_per_player = cardsPerPlayer;
// };
// bool isFirstTime = true;
// void gameControllerTask(void *pvArguments)
// {
//     Serial.println("Game Controller Task Started");

//     for (;;)
//     {

// // ------------------------

//         // Serial.println("Controller loop tick");

//         // if (player != 0 && cards_per_player != 0 && getCardThrowState() == STATE_IDLE)
//         // {
//         //     Serial.println("Game condition valid. Starting dealing cycle");

//         //     if (getCardThrowState() == STATE_CARD_JAMMED)
//         //     {
//         //         Serial.println("Card jam detected before start. Exiting controller.");
//         //         break;
//         //     }

//         //     for (uint16_t circleCount = 0; circleCount < cards_per_player; circleCount++)
//         //     {
//         //         // Serial.print("Outer circle iteration: ");
//         //         Serial.println(circleCount);

//         //         if (getCardThrowState() == STATE_CARD_JAMMED)
//         //         {
//         //             // Serial.println("Jam detected. Breaking outer loop.");
//         //             break;
//         //         }

//         //         // Serial.println("outer Circle ");

//         //         for (u_int16_t playerRotate = 0; playerRotate < player; playerRotate++)
//         //         {
//         //             // Serial.print("Dealing to player index: ");
//         //             Serial.println(playerRotate);

//         //             setCardThrowState(STATE_RUNNING);
//         //             Serial.println("State set to STATE_RUNNING");

//         //             vTaskDelay(5500 / portTICK_PERIOD_MS); // servo(1000) + ramp(~510) + gantry wait(up to 3000) = ~4500ms, 5500 gives margin

//         //             Serial.print("Card throw state after delay: ");
//         //             Serial.println(getCardThrowState());

//         //             if (getCardThrowState() == STATE_CARD_SUCCESS)
//         //             {
//         //                 Serial.println("Card throw SUCCESS");

//         //                 Serial.print("Stepper rotating degrees: ");
//         //                 Serial.println(360 / player);

//         //                 stepperMove(360 / player);

//         //                 vTaskDelay((200 * (360 / player)) / portTICK_PERIOD_MS);
//         //             }
//         //             else if (getCardThrowState() == STATE_CARD_JAMMED)
//         //             {
//         //                 Serial.println("Card Jammed");
//         //                 break;
//         //             }
//         //         }
//         //     }
//         // }
//         // vTaskDelay(300 / portTICK_PERIOD_MS);

//         // ------------------------

//         vTaskDelay(300 / portTICK_PERIOD_MS);
//     }
// };

#include "Arduino.h"
#include <hardware_drivers/ldr/ldr.h>
#include <modules/card_throw.h>
#include <modules/card_throw_state.h>
#include <hardware_drivers/stepper/stepper.h>
#include <modules/card_throw_queue.h>

static int player = 2;
static int cards_per_player = 3;
volatile bool isDeckEmpty = false;

void startGame(int players, int cardsPerPlayer)
{
    player = players;
    cards_per_player = cardsPerPlayer;
    isDeckEmpty = false;

};


void gameControllerTask(void *pvArguments)
{
    Serial.println("Game Controller Task Started");
    int angle = 360 / player;
    for (;;)
    {
        Serial.println("Game Controller Task loop");

        for (int rounds = 0; rounds < cards_per_player && !isDeckEmpty; rounds++)
        {
            for (int p = 0; p < player && !isDeckEmpty; p++)
            {
                ThrowCommand command = CMD_CARD_THROW;
                xQueueSend(throwCommandQueue, &command, portMAX_DELAY);
                ThrowResult result;
                if (xQueueReceive(throwResultQueue, &result, portMAX_DELAY))
                {
                    Serial.println(uxQueueMessagesWaiting(throwResultQueue));

                    switch (result)
                    {
                    case RESULT_SUCCESS:
                        Serial.println("one card throwed succesfully");
                        stepperMove(angle);
                        break;
                    case RESULT_EMPTY:
                        /* code */
                        isDeckEmpty = true;
                        Serial.println("Stack is empty");

                        break;
                    case RESULT_JAM:
                        /* code */
                        while (true)
                        {
                            vTaskDelay(1000 / portTICK_PERIOD_MS);
                            Serial.println("controller HALTED Card jammed");
                        } // TODO later will add in menue to handle this
                        break;

                    default:
                        break;
                    }
                }
            }
        }

        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
};