
#include <Arduino.h>
#include "../src/hardware/buttons/buttons.h"
#include "hardware/buzzer/buzzer.h"
#include "hardware/buttons/buttons.h"
#include "hardware/motor/motor.h"
#include "../src/config/pinmap.h"
#include "hardware/display/display.h"

TaskHandle_t initTaskHandler = NULL;
TaskHandle_t buzzerTaskHandler = NULL;
TaskHandle_t displayTaskHandler = NULL;
TaskHandle_t buttonTaskHandler = NULL;
TaskHandle_t motorTaskHandler = NULL;

void initTask(void *taskParams)
{
  buttonInit();
  displayInit();
  motorInit();
  xTaskNotifyGive(buzzerTaskHandler);
  vTaskDelete(NULL);
}

void setup()
{
  Serial.begin(115200);
  buzzerInit(BUZZER);

  xTaskCreatePinnedToCore(
      initTask,
      "initTask",
      4096,
      NULL,
      5,
      &initTaskHandler,
      1);

  xTaskCreatePinnedToCore(
      buzzerTask,
      "buzzerTask",
      2048,
      NULL,
      2,
      &buzzerTaskHandler,
      1);

  xTaskCreatePinnedToCore(
      displayTask,
      "displayTask",
      8192,
      NULL,
      1,
      &displayTaskHandler,
      1);

  xTaskCreatePinnedToCore(
      buttonTask,
      "bittonTask",
      2048,
      NULL,
      2,
      &buttonTaskHandler,
      1);

  xTaskCreatePinnedToCore(
      motorTask,
      "motor",
      4096,
      NULL,
      2,
      &motorTaskHandler,
      2);
  // xTaskCreatePinnedToCore(
  //   motorTask
  // )
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
  buzzerUpdate();
}
