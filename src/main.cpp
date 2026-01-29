
#include <Arduino.h>
#include "../src/hardware/buttons/buttons.h"
#include "hardware/buzzer/buzzer.h"
#include "hardware/buttons/buttons.h"
#include "../src/config/pinmap.h"
#include "hardware/display/display.h"

void setup()
{
    Serial.begin(115200);
    buzzerInit(BUZZER);
    buttonInit();
    displayInit();
}

void loop()
{
    buzzerUpdate();
    button_print();
    // Serial.println(millis());
}