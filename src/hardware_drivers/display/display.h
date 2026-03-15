#ifndef DISPLAY_H
#define DISPLAY_H
#include <Adafruit_SSD1306.h>

void displayInit();
void clearDisplay();
void writeDisplay(const char* text);
void displayTask(void *pvTaskData);
void drawLoading();
extern Adafruit_SSD1306 display;

#endif