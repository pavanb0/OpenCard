#include "Arduino.h"
#include "card_throw_state.h"
#include <hardware_drivers/ldr/ldr.h>
#include <hardware_drivers/servo/servo.h>
#include <config/hardware.h>
#include <hardware_drivers/motor/motor.h>
#include <hardware_drivers/buzzer/buzzer.h>

volatile card_throw_sates cardThrowState = STATE_IDLE;

/**
 * step 1
 * Check if stack is not empty (later implementation)
 *
 * step 2
 * push servo to positive 180 wait 80ms
 *
 * step 3
 * check ldr detects something in gantry then ldr will read low start store that time count
 * if ldr dont detect clear within 3 second stop motor and return status code of 500
 * if ldr shows gantry clear less than 3 second
 * return status code 200
 *
 */
void throwCard(void *pvArgs)
{
    for (;;)
    {
        long motorRunDuration = 0;

        switch (cardThrowState)

        {
        case STATE_IDLE:
            /* code */
            vTaskDelay(100 / portTICK_PERIOD_MS); // current architecture well only use this approch later we will implemetn xqueuesend/revice
            break;

        case STATE_RUNNING:
            if (!isStackEmpty())
            {
                moveServo(180);
                vTaskDelay(500 / portTICK_PERIOD_MS);
                motorRunDuration = millis(); // motor starting time
                for (u_int16_t i = 0; i < MOTOR_PWM_MAX; i += MOTOR_PWM_SOFT_STEP_COUNT)
                { // motor will ramp up in 17 steps slow to full speed in 17 * 30 ms
                    setMotorDuty(i);
                    vTaskDelay(30 / portTICK_PERIOD_MS);
                } // after half second motor will start at full power
                while ((millis() - motorRunDuration) < 3000)
                {
                    if (isGantryClear())
                        break;
                    vTaskDelay(25 / portTICK_PERIOD_MS);
                }
                if ((millis() - motorRunDuration) > 3000)
                { // TODO we will replace this actual time diff between two ldr high low events
                    cardThrowState = STATE_CARD_JAMMED;
                }
                else
                {
                    cardThrowState = STATE_CARD_SUCCESS;
                }
            }
            else
            {
                cardThrowState = STATE_CARD_EMPTY;
            }

        case STATE_CARD_JAMMED:
            stopMotor();
            buzzerTone(2000, 2000);
            break;
            
        default:
            break;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
