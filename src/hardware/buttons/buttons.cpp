#include <Arduino.h>
#include "../src/config/pinmap.h"
#include "buttons.h"
#include <hardware/buzzer/buzzer.h>
#include <hardware/display/display.h>
#include "hardware/display/display_state.h"

void buttonInit()
{
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_SELECT, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
}

void buttonTask(void *pvButtonParams)
{
    const TickType_t buttonDelay = 30 / portTICK_PERIOD_MS;

    bool upPrev = HIGH;
    bool selectPrev = HIGH;
    bool downPrev = HIGH;

    for (;;)
    {
        bool upNow = digitalRead(BTN_UP);
        bool selectNow = digitalRead(BTN_SELECT);
        bool downNow = digitalRead(BTN_DOWN);

        // edge detection
        /**
         * think like this we initiali set up prev as high
         * and when button pressed upNow becomes low because button pulls it low
         * and in this loop up now and up prev become true and we enter in the block
         * and when we dont leave the button and its still low then then upPrev is low
         * and we dont go inside and as we set it it becomse high again and it resets 
         * so we prevent continues click with this method
         */
        if (upPrev == HIGH && upNow == LOW)
        {
            Serial.println("UpPressed");
            displayState = DISPLAY_TEST_UP;
            buzzerClick();
        }

        if (selectPrev == HIGH && selectNow == LOW)
        {
            Serial.println("SelectPressed");
            displayState = DISPLAY_TEST_SELECT;
            buzzerClick();
        }

        if (downPrev == HIGH && downNow == LOW)
        {
            Serial.println("DownPressed");
            displayState = DISPLAY_TEST_DOWN;

            buzzerClick();
        }

        upPrev = upNow;
        selectPrev = selectNow;
        downPrev = downNow;

        vTaskDelay(buttonDelay);
    }
}

// void buttonTask(void *pvButtonParams)
// {
//     const TickType_t buttonDelay = 300 / portTICK_PERIOD_MS;
//     for (;;)
//     {
//         bool upNow = digitalRead(BTN_UP);
//         bool selectNow = digitalRead(BTN_SELECT);
//         bool downNow = digitalRead(BTN_DOWN);

//         if (upNow == HIGH)
//         {
//           //0.  displayState = DISPLAY_TEST_UP;
//            // buzzerClick();
//            Serial.write("UpPressed \n");
//            vTaskDelay(buttonDelay);
//         }

//         if (selectNow == HIGH)
//         {
//             //  displayState = DISPLAY_TEST_SELECT;
//             //   buzzerClick();
//             Serial.write("SelectPressed \n");
//             vTaskDelay(buttonDelay);
//         }

//         if (downNow == HIGH)
//         {
//             // displayState = DISPLAY_TEST_DOWN;
//             //  buzzerClick();
//             Serial.write("DownPressed \n");
//             vTaskDelay(buttonDelay);
//         }
//     }
// }
