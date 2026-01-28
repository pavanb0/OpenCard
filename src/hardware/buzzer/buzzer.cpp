#include "Arduino.h"
#include "../src/config/pinmap.h"
#include "buzzer.h"

static uint8_t buzzerPin;
static uint8_t pwmChannel = 0;

static unsigned long offTime = 0;
static bool active = false;

void buzzerInit(uint8_t pin){
    // pinMode(BUZZER, OUTPUT);
    buzzerPin = pin;
    ledcSetup(pwmChannel, 2000, 8);   
    ledcAttachPin(buzzerPin, pwmChannel);
    ledcWrite(pwmChannel, 0); 
}


void buzzerTone(uint16_t freq, uint16_t durationMs) {
    ledcWriteTone(pwmChannel, freq);
    ledcWrite(pwmChannel, 255);       // 100% duty
    active = true;
    offTime = millis() + durationMs;
}

void buzzerUpdate() {
    if (active && millis() > offTime) {
        ledcWrite(pwmChannel, 0);
        active = false;
    }
}


void buzzerClick() {
    buzzerTone(3000, 1000);
}

void buzzerSuccess() {
    buzzerTone(1800, 120);
}

void buzzerError() {
    buzzerTone(400, 350);
}

void buzzerStartup() {
    buzzerTone(1500, 100);
}