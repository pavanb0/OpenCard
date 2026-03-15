// #include "Arduino.h"
// #include <hardware_drivers/buzzer/buzzer.h>

// /**
//  * @param taskParam
//  * // use to give arguments to function
//  *
//  */
// #define BUZZER_PWM_CHANNEL 4

// void buzzerTask(void *taskParams)
// {
//     int melody[] = {
//         // NOTE_E5, NOTE_E5, 0, NOTE_E5,
//         // 0, NOTE_C5, NOTE_E5, 0,
//         // NOTE_G5, 0, 0, 0,
//         // NOTE_G4, 0, 0, 0,

//         //         NOTE_E5, NOTE_D5, NOTE_F4, NOTE_G4,
//         //   NOTE_C5, NOTE_B4, NOTE_D4, NOTE_E4,
//         //   NOTE_F4, 0, NOTE_G4, 0,
//         //   NOTE_C5, 0

//         NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5,
//         NOTE_E5, NOTE_G5, NOTE_C6, 0,

//         NOTE_C5, NOTE_DS5, NOTE_G5, NOTE_AS5, // quick rising "bie bie biepp"
//         NOTE_DS6, 0,                          // peak + short rest
//         NOTE_AS5, NOTE_G5, NOTE_DS5,          // slight descend/confirm
//         NOTE_C5

//         // NOTE_F4, NOTE_GS4, NOTE_CS5, NOTE_F5, NOTE_GS5,
//         //   NOTE_CS6, 0,
//         //   NOTE_AS5, NOTE_F5

//     };

//     int noteDurations[] = {
//         // 8, 8, 8, 8,
//         // 8, 8, 8, 8,
//         // 8, 4, 4, 4,
//         // 8, 4, 4, 4,
//         // 8,8,8,8, 8,8,8,8, 4,4,4,4, 2,4
//         8, 8, 8, 8, 8, 8, 8, 2,

//         12, 12, 12, 12, // very short staccato notes
//         8, 8,           // peak
//         10, 10, 10,     // descend
//         4

//         // 16, 16, 12, 12, 10, 8, 6, 10, 6

//     };

//     ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

//     for (int thisNote = 0; thisNote < sizeof(noteDurations) / sizeof(noteDurations[0]); thisNote++)
//     {
//         int noteDuration = 1000 / noteDurations[thisNote];
//         // ledcWriteTone(BUZZER_PWM_CHANNEL, melody[thisNote]);

//         ledcWriteTone(BUZZER_PWM_CHANNEL, melody[thisNote]);
//         ledcWrite(BUZZER_PWM_CHANNEL, 255);

//         vTaskDelay((noteDuration * 1.3) / portTICK_PERIOD_MS);
//         // noTone(BUZZER);
//         ledcWriteTone(BUZZER_PWM_CHANNEL, 0);
//     }

//     while (true)
//     {
//         buzzerUpdate();
//         vTaskDelay(10 / portTICK_PERIOD_MS);
//     }

//     // vTaskDelete(NULL);
// }


// #include "Arduino.h"
// #include <hardware_drivers/buzzer/buzzer.h>

// #define BUZZER_PWM_CHANNEL 4

// void buzzerTask(void *taskParams)
// {
//     int melody[] = {
//         NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5,
//         NOTE_E5, NOTE_G5, NOTE_C6, 0,

//         NOTE_C5, NOTE_DS5, NOTE_G5, NOTE_AS5,
//         NOTE_DS6, 0,
//         NOTE_AS5, NOTE_G5, NOTE_DS5,
//         NOTE_C5
//     };

//     int noteDurations[] = {
//         8, 8, 8, 8, 8, 8, 8, 2,

//         12, 12, 12, 12,
//         8, 8,
//         10, 10, 10,
//         4
//     };

//     ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

//     for (int thisNote = 0; thisNote < (int)(sizeof(noteDurations) / sizeof(noteDurations[0])); thisNote++)
//     {
//         int noteDuration = 1000 / noteDurations[thisNote];

//         if (melody[thisNote] == 0)
//         {
//             // Rest: just silence for the note duration
//             ledcWrite(BUZZER_PWM_CHANNEL, 0);
//             ledcWriteTone(BUZZER_PWM_CHANNEL, 0);
//             vTaskDelay(noteDuration / portTICK_PERIOD_MS);
//         }
//         else
//         {
//             // Play the note using buzzerTone so state is properly tracked
//             buzzerTone(melody[thisNote], noteDuration);

//             // Wait for note to finish (buzzerUpdate will silence it)
//             vTaskDelay((int)(noteDuration * 1.3f) / portTICK_PERIOD_MS);

//             // Explicitly silence between notes
//             ledcWrite(BUZZER_PWM_CHANNEL, 0);
//             ledcWriteTone(BUZZER_PWM_CHANNEL, 0);
//         }

//         // Small gap between notes
//         vTaskDelay(5 / portTICK_PERIOD_MS);
//     }

//     while (true)
//     {
//         buzzerUpdate();
//         vTaskDelay(10 / portTICK_PERIOD_MS);
//     }
// }


#include "Arduino.h"
#include <hardware_drivers/buzzer/buzzer.h>

#define BUZZER_PWM_CHANNEL 4

void buzzerTask(void *taskParams)
{
    int melody[] = {
        NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5,
        NOTE_E5, NOTE_G5, NOTE_C6, 0,

        NOTE_C5, NOTE_DS5, NOTE_G5, NOTE_AS5,
        NOTE_DS6, 0,
        NOTE_AS5, NOTE_G5, NOTE_DS5,
        NOTE_C5
    };

    int noteDurations[] = {
        8, 8, 8, 8, 8, 8, 8, 2,

        12, 12, 12, 12,
        8, 8,
        10, 10, 10,
        4
    };

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    for (int thisNote = 0; thisNote < (int)(sizeof(noteDurations) / sizeof(noteDurations[0])); thisNote++)
    {
        int noteDuration = 1000 / noteDurations[thisNote];

        if (melody[thisNote] == 0)
        {
            // Rest: duty=0 to silence, do NOT call ledcWriteTone(0)
            ledcWrite(BUZZER_PWM_CHANNEL, 0);
            vTaskDelay(noteDuration / portTICK_PERIOD_MS);
        }
        else
        {
            buzzerTone(melody[thisNote], noteDuration);
            vTaskDelay((int)(noteDuration * 1.3f) / portTICK_PERIOD_MS);
            ledcWrite(BUZZER_PWM_CHANNEL, 0); // silence between notes, duty only
        }

        // Small gap between notes
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }

    while (true)
    {
        buzzerUpdate();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}