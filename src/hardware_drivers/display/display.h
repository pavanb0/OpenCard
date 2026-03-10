#ifndef DISPLAY_H
#define DISPLAY_H

void displayInit();
void clearDisplay();
void writeDisplay(const char* text);
void displayTask(void *pvTaskData);
void drawLoading();

#endif