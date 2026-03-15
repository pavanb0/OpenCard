#ifndef BUTTONS_H
#define BUTTONS_H

enum ButtonState {
  STATE_BTN_UP,
  STATE_BTN_SELECT,
  STATE_BTN_DOWN
};
bool buttonShortPress(ButtonState b);
bool buttonLongPress(ButtonState b);
void buttonInit();
// void button_print();
// void buttonTask(void *pvbuttonParameter);
#endif