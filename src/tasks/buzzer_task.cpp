#include "Arduino.h"
#include <hardware_drivers/buzzer/buzzer.h>

/**
 * @param taskParam
 * // use to give arguments to function
 *
 */
#define BUZZER_PWM_CHANNEL 4

void buzzerTask(void *taskParams)
{
    int melody[] = {
        NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3,
        NOTE_G3, 0, NOTE_B3, NOTE_C4};

    int noteDurations[] = {4, 8, 8, 4, 4, 8, 4, 8};

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    for (int thisNote = 0; thisNote < 8; thisNote++)
    {
        int noteDuration = 1000 / noteDurations[thisNote];
        buzzerTone( melody[thisNote], noteDuration);

        vTaskDelay((noteDuration * 1.3) / portTICK_PERIOD_MS);
        // noTone(BUZZER);
        ledcWrite(BUZZER_PWM_CHANNEL, 0);

    }

    vTaskDelete(NULL);
}