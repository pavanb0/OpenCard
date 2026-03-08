#include "Arduino.h"
#include <config/pinmap.h>
#include "ldr_state.h"
#include <hardware/buzzer/buzzer.h>

volatile ldr_state ldrState = {false, false};

void IRAM_ATTR gantryISR()
{
    if(!digitalRead(GANTRY_LDR)){
        ldrState.gantryClear = true;
    }else{
        ldrState.gantryClear = false;
    }

}

void ldrInit()
{
    pinMode(GANTRY_LDR, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(GANTRY_LDR), gantryISR, CHANGE);
}


unsigned long readDelay = 500;
unsigned long lastRead = 0;

void ldrTask()
{
    static bool lastState = true;

    if(lastState != ldrState.gantryClear){

        lastState = ldrState.gantryClear;

        Serial.println(ldrState.gantryClear);

        if(ldrState.gantryClear){
            buzzerTone(1000,500);
        }
    }
}

void testLdr()
{

    if (millis() - lastRead > readDelay)
    {
        Serial.println(digitalRead(GANTRY_LDR));
        Serial.println("\n");
        Serial.println("LDR STATE");
        Serial.println("\n");
        lastRead = millis();
    }
}