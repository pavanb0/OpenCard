#include "Arduino.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../src/config/pinmap.h"
#include <hardware/buzzer/buzzer.h>
#include "display_state.h"
#define SCREEN_ADDRESS 0x3C
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
volatile display_States displayState = DISPLAY_HOME_LOADING;

static uint8_t animationFrame = 0;
static uint32_t animationLastUpdate = 0;
const uint32_t animationInterval = 400;

void displayInit()
{
    Wire.begin(); // ESP32 default I2C pins are fine but we need to press boot during boot then reset button

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
    {
        Serial.println("OLED failed");
        buzzerClick();
        while (1)
            ;
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    // display.setTextSize(1);
    // display.setCursor(0, 0);
    // display.println("ESP32 Ready");
    display.display();
}

void displayTask(void *pvTaskData)
{
    const TickType_t frameDelay = 50 / portTICK_PERIOD_MS;
    for (;;)
    {
        switch (displayState)
        {
        case DISPLAY_HOME_LOADING:
            drawLoading();
            break;

        default:
            break;
        }
        vTaskDelay(frameDelay);
    }
}

void writeDisplay(const char *text)
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(text);
    display.display();
}

void clearDisplay()
{
    display.clearDisplay();
    display.display();
}

void drawLoading()
{

    if (millis() - animationLastUpdate < animationInterval)
        return;

    animationLastUpdate = millis();
    animationFrame = (animationFrame + 1) % 2;

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);

    if (animationFrame == 0)
        display.println("Loading.");
    else
        display.println("Loading..");

    display.display();
}