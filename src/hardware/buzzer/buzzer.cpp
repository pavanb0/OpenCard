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