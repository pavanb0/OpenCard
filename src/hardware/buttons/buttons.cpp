#include <Arduino.h>
#include "../src/config/pinmap.h"
#include "buttons.h"
#include <hardware/buzzer/buzzer.h>

void buttonInit()
{
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
}

void button_print()
{

  if (digitalRead(BTN_UP) == LOW)
  {
    Serial.println("UP pressed");
    buzzerClick();
    
  }

  if (digitalRead(BTN_SELECT) == LOW)
  {
    Serial.println("SELECT pressed");
    buzzerClick();
  }

  if (digitalRead(BTN_DOWN) == LOW)
  {
    Serial.println("DOWN pressed");
    buzzerClick();
  }

  // delay(500);
}