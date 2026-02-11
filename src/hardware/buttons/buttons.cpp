#include <Arduino.h>
#include "../src/config/pinmap.h"
#include "buttons.h"
#include <hardware/buzzer/buzzer.h>
#include <hardware/display/display.h>

void buttonInit()
{
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
}

// void button_print()
// {

//   if (digitalRead(BTN_UP) == LOW)
//   {
//     writeDisplay("UP pressed");
//     buzzerClick();
//    delay(500);
//   }

//   if (digitalRead(BTN_SELECT) == LOW)
//   {
//     writeDisplay("SELECT pressed");
//     buzzerClick();
//   }

//   if (digitalRead(BTN_DOWN) == LOW)
//   {
//     writeDisplay("DOWN pressed");
//     buzzerClick();
//   }

//   // delay(500);
// }
static bool upPrev = HIGH;
static bool selectPrev = HIGH;
static bool downPrev = HIGH;

void button_print()
{

    bool upNow = digitalRead(BTN_UP);
    bool selectNow = digitalRead(BTN_SELECT);
    bool downNow = digitalRead(BTN_DOWN);

    if (upPrev == HIGH && upNow == LOW) {
        writeDisplay("UP pressed");
        buzzerClick();
    }

    if (selectPrev == HIGH && selectNow == LOW) {
        writeDisplay("SELECT pressed");
        buzzerClick();
    }

    if (downPrev == HIGH && downNow == LOW) {
        writeDisplay("DOWN pressed");
        buzzerClick();
    }

    upPrev = upNow;
    selectPrev = selectNow;
    downPrev = downNow;
}
