
#include <Arduino.h>
#include "../src/hardware/buttons/buttons.h"
#include "hardware/buzzer/buzzer.h"
#include "hardware/buttons/buttons.h"
#include "../src/config/pinmap.h"
#include "hardware/display/display.h"


int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


void setup()
{
    Serial.begin(115200);
    buzzerInit(BUZZER);
    buttonInit();
    displayInit();

    for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER);
  }
}

// void loop()
// {
//     buzzerUpdate();
//     button_print();
//     // Serial.println(millis());
// }

void loop()
{
    // digitalWrite(BUZZER, HIGH);
    // delay(200);
    // digitalWrite(BUZZER, LOW);
    // delay(1000);
}
