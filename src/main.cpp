
#include <Arduino.h>
#include "../src/hardware/buttons/buttons.h"
#include "hardware/buzzer/buzzer.h"
#include "hardware/buttons/buttons.h"
#include "../src/config/pinmap.h"
#include "hardware/display/display.h"

TaskHandle_t initTaskHandler = NULL;
TaskHandle_t buzzerTaskHandler = NULL;

void initTask(void *taskParams)
{

  buttonInit();
  displayInit();
  // i want to flag here that task initialization is done
  xTaskNotifyGive(initTaskHandler);
  vTaskDelete(NULL);
};
/**
 * @param taskParam
 * // use to give arguments to function
 *
 */
void buzzerTask(void *taskParams)
{
  int melody[] = {
      NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

  int noteDurations[] = {
      4, 8, 8, 4, 4, 4, 4, 4};
  for (;;)
  {

    for (int thisNote = 0; thisNote < 8; thisNote++)
    {
      if (ulTaskNotifyTake(pdTRUE, 0) > 0)
      {
        noTone(BUZZER);
        vTaskDelete(NULL); // stop forever
      }

      int noteDuration = 1000 / noteDurations[thisNote];
      tone(BUZZER, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      vTaskDelay(pauseBetweenNotes / portTICK_PERIOD_MS);
      noTone(BUZZER);
    }
    // and i want to break the music here when it here
  }
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
      2,
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
