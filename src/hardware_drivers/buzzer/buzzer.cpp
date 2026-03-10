#include "Arduino.h"
#include "../src/config/pinmap.h"
#include "buzzer.h"
#include "hardware_drivers/display/display_state.h"
#define BUZZER_PWM_CHANNEL 4
#define BUZZER_PWM_FREQ 2000
#define BUZZER_PWM_RES 8
#define BUZZER_DUTY 128 * 3

static uint8_t buzzerPin;
static uint8_t pwmChannel = 0;

static unsigned long offTime = 0;
static bool active = false;


void buzzerInit(uint8_t pin)
{
    ledcSetup(BUZZER_PWM_CHANNEL, BUZZER_PWM_FREQ, BUZZER_PWM_RES);
    ledcAttachPin(pin, BUZZER_PWM_CHANNEL);
    ledcWrite(BUZZER_PWM_CHANNEL, 0);
}

void buzzerTone(uint16_t freq, uint16_t durationMs)
{
    ledcWriteTone(BUZZER_PWM_CHANNEL, freq);
    ledcWrite(BUZZER_PWM_CHANNEL, BUZZER_DUTY);
    active = true;
    offTime = millis() + durationMs;
}

void buzzerUpdate()
{
  
    if (active && millis() >= offTime)
    {
        ledcWrite(BUZZER_PWM_CHANNEL, 0);
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
