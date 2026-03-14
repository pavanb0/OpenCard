
#include <Arduino.h>
#include "../src/hardware_drivers/buttons/buttons.h"
#include "hardware_drivers/buzzer/buzzer.h"
#include "hardware_drivers/buttons/buttons.h"
#include "hardware_drivers/motor/motor.h"
#include "../src/config/pinmap.h"
#include "hardware_drivers/display/display.h"
#include "hardware_drivers/servo/servo.h"
#include "hardware_drivers/ldr/ldr.h"
#include "tasks/buzzer_task.h"
#include <modules/card_throw.h>
#include <system/card_controller.h>
#include <modules/card_throw_queue.h>
#include "hardware_drivers/stepper/stepper.h"
#include "system/card_controller_queue.h"

TaskHandle_t initTaskHandler = NULL;
TaskHandle_t buzzerTaskHandler = NULL;
TaskHandle_t displayTaskHandler = NULL;
TaskHandle_t buttonTaskHandler = NULL;
TaskHandle_t motorTaskHandler = NULL;
TaskHandle_t cardThrowTaskHandler = NULL;
TaskHandle_t gameControllerTaskHandler = NULL;

void initTask(void *taskParams)
{
  initThrowQueues();
  initCardControllerQueue();
  buttonInit();
  displayInit();
  motorInit();
  servoInit();
  ldrInit();
  stepperInit();
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

  // xTaskCreatePinnedToCore(
  //     buttonTask,
  //     "bittonTask",
  //     2048,
  //     NULL,
  //     2,
  //     &buttonTaskHandler,
  //     1);

  xTaskCreatePinnedToCore(
    throwCardTask,
    "cardThrowTask",
    2048,
    NULL,
    2,
    &cardThrowTaskHandler,
    1
  );
  xTaskCreatePinnedToCore(
    gameControllerTask,
    "gameController",
    2048,
    NULL,
    2,
    &gameControllerTaskHandler,
    1
  );

//   xTaskCreatePinnedToCore(
//       motorTask,
//       "motor",
//       4096,
//       NULL,
//       2,
//       &motorTaskHandler,
//       2);
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
  //ldrTask();
  
}
