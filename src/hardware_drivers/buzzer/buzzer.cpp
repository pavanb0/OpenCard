// #include "Arduino.h"
// #include "../src/config/pinmap.h"
// #include "buzzer.h"
// #include "hardware_drivers/display/display_state.h"
// #define BUZZER_PWM_CHANNEL 4
// #define BUZZER_PWM_FREQ 2000
// #define BUZZER_PWM_RES 8
// #define BUZZER_DUTY 128

// static uint8_t buzzerPin;

// static unsigned long offTime = 0;
// static bool active = false;
// static uint8_t clickStage = 0;
// static unsigned long nextClickTime = 0;

// void buzzerInit(uint8_t pin)
// {
//     ledcSetup(BUZZER_PWM_CHANNEL, BUZZER_PWM_FREQ, BUZZER_PWM_RES);
//     ledcAttachPin(pin, BUZZER_PWM_CHANNEL);
//     ledcWriteTone(BUZZER_PWM_CHANNEL, 0);
// }
// void buzzerDoubleClick()
// {
//     clickStage = 1;
//     buzzerTone(3000, 80);
//     nextClickTime = millis() + 120;
// }

// void buzzerTone(uint16_t freq, uint16_t durationMs)
// {
//     Serial.println("BUZZ ON");
//     Serial.print("Core: ");
//     Serial.println(xPortGetCoreID());
    
//     ledcSetup(BUZZER_PWM_CHANNEL, freq, BUZZER_PWM_RES);
//     ledcWriteTone(BUZZER_PWM_CHANNEL, freq);
//     // ledcWrite(BUZZER_PWM_CHANNEL, BUZZER_DUTY);
//     ledcWrite(BUZZER_PWM_CHANNEL, 255);
//     active = true;
//     offTime = millis() + durationMs;
// }

// void buzzerUpdate()
// {

//     if (active && millis() >= offTime)
//     {
//         Serial.println("BUZZ OFF");
//         ledcWrite(BUZZER_PWM_CHANNEL, 0);
//         ledcWriteTone(BUZZER_PWM_CHANNEL, 0);
//         active = false;
//     }

//     if (clickStage == 1 && millis() >= nextClickTime)
//     {
//         clickStage = 0;
//         buzzerTone(3000, 80);
//     }
// }

// void buzzerClick()
// {
//     buzzerTone(3000, 200);
// }

// void buzzerSuccess()
// {
//     buzzerTone(1800, 120);
// }

// void buzzerError()
// {
//     buzzerTone(400, 350);
// }

// void buzzerStartup()
// {
//     buzzerTone(1500, 100);
// }

// #include "Arduino.h"
// #include "../src/config/pinmap.h"
// #include "buzzer.h"
// #include "hardware_drivers/display/display_state.h"

// #define BUZZER_PWM_CHANNEL 4
// #define BUZZER_PWM_FREQ 2000
// #define BUZZER_PWM_RES 8
// #define BUZZER_DUTY 128

// static uint8_t buzzerPin;

// static volatile unsigned long offTime = 0;
// static volatile bool active = false;
// static volatile uint8_t clickStage = 0;
// static volatile unsigned long nextClickTime = 0;

// static portMUX_TYPE buzzerMux = portMUX_INITIALIZER_UNLOCKED;

// void buzzerInit(uint8_t pin)
// {
//     buzzerPin = pin;
//     ledcSetup(BUZZER_PWM_CHANNEL, BUZZER_PWM_FREQ, BUZZER_PWM_RES);
//     ledcAttachPin(pin, BUZZER_PWM_CHANNEL);
//     ledcWriteTone(BUZZER_PWM_CHANNEL, 0);
// }

// void buzzerTone(uint16_t freq, uint16_t durationMs)
// {
//     Serial.println("BUZZ ON");
//     Serial.print("Core: ");
//     Serial.println(xPortGetCoreID());

//     taskENTER_CRITICAL(&buzzerMux);
//     active = true;
//     offTime = millis() + durationMs;
//     taskEXIT_CRITICAL(&buzzerMux);

//     // ledcSetup(BUZZER_PWM_CHANNEL, freq, BUZZER_PWM_RES);
//     ledcWriteTone(BUZZER_PWM_CHANNEL, freq);
//     ledcWrite(BUZZER_PWM_CHANNEL, 255);
// }

// void buzzerUpdate()
// {
//     taskENTER_CRITICAL(&buzzerMux);
//     bool isActive = active;
//     unsigned long off = offTime;
//     uint8_t stage = clickStage;
//     unsigned long nextClick = nextClickTime;
//     taskEXIT_CRITICAL(&buzzerMux);

//     if (isActive && millis() >= off)
//     {
//         Serial.println("BUZZ OFF");
//         ledcWrite(BUZZER_PWM_CHANNEL, 0);
//         ledcWriteTone(BUZZER_PWM_CHANNEL, 0);
//         taskENTER_CRITICAL(&buzzerMux);
//         active = false;
//         taskEXIT_CRITICAL(&buzzerMux);
//     }

//     if (stage == 1 && millis() >= nextClick)
//     {
//         taskENTER_CRITICAL(&buzzerMux);
//         clickStage = 0;
//         taskEXIT_CRITICAL(&buzzerMux);
//         buzzerTone(3000, 80);
//     }
// }

// void buzzerDoubleClick()
// {
//     taskENTER_CRITICAL(&buzzerMux);
//     clickStage = 1;
//     nextClickTime = millis() + 120;
//     taskEXIT_CRITICAL(&buzzerMux);
//     buzzerTone(3000, 80);
// }

// void buzzerClick()
// {
//     buzzerTone(3000, 200);
// }

// void buzzerSuccess()
// {
//     buzzerTone(1800, 120);
// }

// void buzzerError()
// {
//     buzzerTone(400, 350);
// }

// void buzzerStartup()
// {
//     buzzerTone(1500, 100);
// }


#include "Arduino.h"
#include "../src/config/pinmap.h"
#include "buzzer.h"
#include "hardware_drivers/display/display_state.h"

#define BUZZER_PWM_CHANNEL 4
#define BUZZER_PWM_FREQ 2000
#define BUZZER_PWM_RES 8
#define BUZZER_DUTY 128

static uint8_t buzzerPin;

static volatile unsigned long offTime = 0;
static volatile bool active = false;
static volatile uint8_t clickStage = 0;
static volatile unsigned long nextClickTime = 0;

static portMUX_TYPE buzzerMux = portMUX_INITIALIZER_UNLOCKED;

void buzzerInit(uint8_t pin)
{
    buzzerPin = pin;
    ledcSetup(BUZZER_PWM_CHANNEL, BUZZER_PWM_FREQ, BUZZER_PWM_RES);
    ledcAttachPin(pin, BUZZER_PWM_CHANNEL);
    ledcWrite(BUZZER_PWM_CHANNEL, 0); // duty=0, do NOT call ledcWriteTone(0) - it breaks the channel timer binding
}

void buzzerTone(uint16_t freq, uint16_t durationMs)
{
    Serial.println("BUZZ ON");
    Serial.print("Core: ");
    Serial.println(xPortGetCoreID());

    taskENTER_CRITICAL(&buzzerMux);
    active = true;
    offTime = millis() + durationMs;
    taskEXIT_CRITICAL(&buzzerMux);

    ledcSetup(BUZZER_PWM_CHANNEL, freq, BUZZER_PWM_RES);
    ledcWriteTone(BUZZER_PWM_CHANNEL, freq);
    ledcWrite(BUZZER_PWM_CHANNEL, 255);
}

void buzzerUpdate()
{
    taskENTER_CRITICAL(&buzzerMux);
    bool isActive = active;
    unsigned long off = offTime;
    uint8_t stage = clickStage;
    unsigned long nextClick = nextClickTime;
    taskEXIT_CRITICAL(&buzzerMux);

    if (isActive && millis() >= off)
    {
        Serial.println("BUZZ OFF");
        ledcWrite(BUZZER_PWM_CHANNEL, 0); // duty=0 silences it, do NOT call ledcWriteTone(0)
        taskENTER_CRITICAL(&buzzerMux);
        active = false;
        taskEXIT_CRITICAL(&buzzerMux);
    }

    if (stage == 1 && millis() >= nextClick)
    {
        taskENTER_CRITICAL(&buzzerMux);
        clickStage = 0;
        taskEXIT_CRITICAL(&buzzerMux);
        buzzerTone(3000, 80);
    }
}

void buzzerDoubleClick()
{
    taskENTER_CRITICAL(&buzzerMux);
    clickStage = 1;
    nextClickTime = millis() + 120;
    taskEXIT_CRITICAL(&buzzerMux);
    buzzerTone(3000, 80);
}

void buzzerClick()
{
    buzzerTone(3000, 200);
}

void buzzerSuccess()
{
    buzzerTone(1800, 120);
}

void buzzerError()
{
    buzzerTone(400, 350);
}

void buzzerStartup()
{
    buzzerTone(1500, 100);
}