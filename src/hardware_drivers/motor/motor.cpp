#include "Arduino.h"
#include <config/pinmap.h>
#include "motor.h"
#include "motor_state.h"
#include <config/hardware.h>


volatile motor_State motorState = MOTOR_IDLE;
static u_int8_t duty = 0;

void motorInit()
{
    ledcSetup(MOTOR_PWM_CHANNEL, MOTOR_PWM_FREQ, MOTOR_PWM_RES);
    ledcAttachPin(CARD_MOTOR, MOTOR_PWM_CHANNEL);
    ledcWrite(MOTOR_PWM_CHANNEL, 0);
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
            if (duty < MOTOR_PWM_MAX)
            {
                ledcWrite(MOTOR_PWM_CHANNEL, duty);
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
                ledcWrite(MOTOR_PWM_CHANNEL, duty);
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
    ledcWrite(MOTOR_PWM_CHANNEL, duty);
}

void stopMotor()
{
    ledcWrite(MOTOR_PWM_CHANNEL, 0);
}