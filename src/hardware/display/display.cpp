#include "Arduino.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../src/config/pinmap.h"
#include <hardware/buzzer/buzzer.h>
#define SCREEN_ADDRESS 0x3C
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void displayInit()
{
    Wire.begin(); // ESP32 default I2C pins are fine unless you changed

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
    {
        Serial.println("OLED failed");
        while (1);
        buzzerClick();
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("ESP32 Ready");
    display.display();
}

void clearDisplay(){
    display.clearDisplay();
    display.display();
}

