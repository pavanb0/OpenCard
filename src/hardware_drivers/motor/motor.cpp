#include "Arduino.h"
#include <config/pinmap.h>
#include "motor.h"
#include "motor_state.h"
#define PWM_CHANNEL 0
#define PWM_FREQ 20000 // 20 kHz (silent motor)
#define PWM_RES 8
#define PWM_MAX 255

volatile motor_State motorState = MOTOR_IDLE;
static u_int8_t duty = 0;

void motorInit()
{
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RES);
    ledcAttachPin(CARD_MOTOR, PWM_CHANNEL);
    ledcWrite(PWM_CHANNEL, 0);
}

void motorTask(void *taskParameter)
{

    const TickType_t stepDelay = 20 / portTICK_PERIOD_MS;
    const int stepCount = 5; // increase for more soft start
    for (;;)
    {
        switch (motorState)
        {
        // case MOTOR_IDLE:

        //  //   stopMotor();
        //     break;

        case MOTOR_SOFT_START:
            if (duty < PWM_MAX)
            {
                ledcWrite(PWM_CHANNEL, duty);
                duty += stepCount;
            }
            else
            {
                motorState = MOTOR_IDLE;
            }
            break;

        case MOTOR_SOFT_STOP:
            if (duty > 0)
            {
                duty -= stepCount;
                ledcWrite(PWM_CHANNEL, duty);
            }
            else
            {
                duty = 0;
                motorState = MOTOR_IDLE;
            }
            break;

        default:
            stopMotor();
            break;
        }
        vTaskDelay(stepDelay);
    }
}

void setMotorDuty(uint8_t duty)
{
    ledcWrite(PWM_CHANNEL, duty);
}

void stopMotor()
{
    ledcWrite(PWM_CHANNEL, 0);
}