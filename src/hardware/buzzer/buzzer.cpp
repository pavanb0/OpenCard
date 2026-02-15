#include "Arduino.h"
#include "../src/config/pinmap.h"
#include "buzzer.h"
#include "hardware/display/display_state.h"

static uint8_t buzzerPin;
static uint8_t pwmChannel = 0;

static unsigned long offTime = 0;
static bool active = false;

/**
 * @param taskParam
 * // use to give arguments to function
 *
 */
void buzzerTask(void *taskParams)
{
    int melody[] = {
        NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3,
        NOTE_G3, 0, NOTE_B3, NOTE_C4};

    int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    for (int thisNote = 0; thisNote < 8; thisNote++)
    {
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(BUZZER, melody[thisNote], noteDuration);

        vTaskDelay((noteDuration * 1.3) / portTICK_PERIOD_MS);
        noTone(BUZZER);
    }
    displayState = DISPLAY_TEST_WELCOME;

    vTaskDelete(NULL);
}

void buzzerInit(uint8_t pin)
{
    // pinMode(BUZZER, OUTPUT);
    // buzzerPin = pin;
    // ledcSetup(pwmChannel, 2000, 8);
    // ledcAttachPin(buzzerPin, pwmChannel);
    // ledcWrite(pwmChannel, 0);

    noTone(pin);
}

void buzzerTone(uint16_t freq, uint16_t durationMs)
{
    // ledcWriteTone(pwmChannel, freq);
    // ledcWrite(pwmChannel, 255);       // 100% duty
    tone(BUZZER, NOTE_A3);
    active = true;
    offTime = millis() + durationMs;
}

void buzzerUpdate()
{
    if (active && millis() > offTime)
    {
        noTone(BUZZER);
        active = false;
    }
}

void buzzerClick()
{
    buzzerTone(3000, 200);
}

void buzzerSuccess()
{
    buzzerTone(1800, 120);
}

void buzzerError()
{
    buzzerTone(400, 350);
}

void buzzerStartup()
{
    buzzerTone(1500, 100);
}

// #include "Arduino.h"
// #include "../src/config/pinmap.h"
// #include "buzzer.h"
// #include "hardware/display/display_state.h"

// /* ---------- LEDC CONFIG ---------- */
// #define BUZZER_CHANNEL 7
// #define BUZZER_RES     8

// static uint8_t buzzerPin;
// static bool active = false;
// static uint32_t offTime = 0;

// /* ---------- INTERNAL HELPERS ---------- */
// static inline void buzzerOff()
// {
//     ledcWrite(BUZZER_CHANNEL, 0);
//     active = false;
// }

// /* ---------- TASK ---------- */
// void buzzerTask(void *taskParams)
// {
//     ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

//     struct {
//         uint16_t freq;
//         uint16_t dur;
//     } melody[] = {
//         {262, 250}, {196, 125}, {196, 125}, {220, 250},
//         {196, 250}, {0,   250}, {247, 250}, {262, 250}
//     };

//     for (uint8_t i = 0; i < 8; i++)
//     {
//         if (melody[i].freq > 0)
//             buzzerTone(melody[i].freq, melody[i].dur);

//         vTaskDelay(pdMS_TO_TICKS(melody[i].dur + 50));
//     }

//     displayState = DISPLAY_TEST_WELCOME;
//     vTaskDelete(NULL);
// }

// /* ---------- INIT ---------- */
// void buzzerInit(uint8_t pin)
// {
//     buzzerPin = pin;

//     ledcSetup(BUZZER_CHANNEL, 2000, BUZZER_RES);
//     ledcAttachPin(buzzerPin, BUZZER_CHANNEL);
//     buzzerOff();
// }

// /* ---------- NON-BLOCKING TONE ---------- */
// void buzzerTone(uint16_t freq, uint16_t durationMs)
// {
//     ledcWriteTone(BUZZER_CHANNEL, freq);
//     ledcWrite(BUZZER_CHANNEL, 255); // 50% duty

//     active = true;
//     offTime = millis() + durationMs;
// }

// /* ---------- UPDATE (CALL IN LOOP OR TASK) ---------- */
// void buzzerUpdate()
// {
//     if (active && millis() >= offTime)
//     {
//         buzzerOff();
//     }
// }

// /* ---------- PRESETS ---------- */
// void buzzerClick()   { buzzerTone(3000, 200); }
// void buzzerSuccess() { buzzerTone(1800, 120); }
// void buzzerError()   { buzzerTone(400,  350); }
// void buzzerStartup() { buzzerTone(1500, 100); }
