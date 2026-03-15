#include <Arduino.h>
#include "../src/config/pinmap.h"
#include "buttons.h"
#include <hardware_drivers/buzzer/buzzer.h>
#include <hardware_drivers/display/display.h>
#include "hardware_drivers/display/display_state.h"
#include "hardware_drivers/motor/motor_state.h"
#include "hardware_drivers/servo/servo.h"
#include "buttons.h"

#define LONG_PRESS_MS 800

struct Btn
{
  uint8_t pin;
  bool lastState;
  uint32_t pressedAt;
  bool longSent;
};

static Btn btn[3];

void buttonInit()
{
  btn[STATE_BTN_UP] = {BTN_UP, HIGH, 0, false};
  btn[STATE_BTN_SELECT] = {BTN_SELECT, HIGH, 0, false};
  btn[STATE_BTN_DOWN] = {BTN_DOWN, HIGH, 0, false};

  for (auto &b : btn)
  {
    pinMode(b.pin, INPUT_PULLUP);
  }
  // pinMode(STATE_BTN_UP, INPUT_PULLUP);
  // pinMode(BTN_SELECT, INPUT_PULLUP);
  // pinMode(BTN_DOWN, INPUT_PULLUP);
}

bool buttonShortPress(ButtonState b)
{
  Btn &bt = btn[b];
  bool cur = digitalRead(bt.pin);

  if (bt.lastState == HIGH && cur == LOW) {
    bt.pressedAt = millis();
    bt.longSent = false;
  }

  if (bt.lastState == LOW && cur == HIGH) {
    if (!bt.longSent && (millis() - bt.pressedAt) < LONG_PRESS_MS) {
      bt.lastState = cur;
      buzzerClick();
      return true;
    }
  }

  bt.lastState = cur;
  return false;

}

bool buttonLongPress(ButtonState b) {
  Btn &bt = btn[b];
  bool cur = digitalRead(bt.pin);

  if (bt.lastState == HIGH && cur == LOW) {
    bt.pressedAt = millis();
    bt.longSent = false;
  }

  if (cur == LOW && !bt.longSent &&
      (millis() - bt.pressedAt) >= LONG_PRESS_MS) {
    bt.longSent = true;
    buzzerDoubleClick();
    return true;
  }

  bt.lastState = cur;
  return false;
}



