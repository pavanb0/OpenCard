#ifndef BUZZER_H
#define BUZZER_H
#include "../src/config/pinmap.h"

void  buzzerInit(uint8_t pin);
void buzzerTone(uint16_t freq, uint16_t durationMs);

void buzzerClick();
void buzzerSuccess();
void buzzerError();
void buzzerStartup();

void buzzerUpdate();

#endif