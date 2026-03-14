// #include "Arduino.h"
// #include "card_throw_state.h"
// #include <hardware_drivers/ldr/ldr.h>
// #include <hardware_drivers/servo/servo.h>
// #include <config/hardware.h>
// #include <hardware_drivers/motor/motor.h>
// #include <hardware_drivers/buzzer/buzzer.h>

// volatile card_throw_sates cardThrowState = STATE_IDLE;
// // volatile card_throw_sates cardThrowState = STATE_RUNNING;

// /**
//  * step 1
//  * Check if stack is not empty (later implementation)
//  *
//  * step 2
//  * push servo to positive 180 wait 80ms
//  *
//  * step 3
//  * check ldr detects something in gantry then ldr will read low start store that time count
//  * if ldr dont detect clear within 3 second stop motor and return status code of 500
//  * if ldr shows gantry clear less than 3 second
//  * return status code 200
//  *
//  */
// void throwCardTask(void *pvArgs)
// {
//     for (;;)
//     {
//         long motorRunDuration = 0;

//         switch (cardThrowState)

//         {
//         case STATE_IDLE:
//             /* code */
//             vTaskDelay(100 / portTICK_PERIOD_MS); // current architecture well only use this approch later we will implemetn xqueuesend/revice
//             break;

//         case STATE_RUNNING:
//             // moveServo(180);
//             // Serial.println("gave command to move cl 360");
//             // vTaskDelay(500 / portTICK_PERIOD_MS);
//             // moveServo(0);
//             // Serial.println("gave command to move ccw 0");
//             // vTaskDelay(500 / portTICK_PERIOD_MS);

//             if (!isStackEmpty())
//             {
//                 moveServo(180);
//                 vTaskDelay(500 / portTICK_PERIOD_MS);
//                 motorRunDuration = millis(); // motor starting time
//                 for (u_int16_t i = 0; i < MOTOR_PWM_MAX; i += MOTOR_PWM_SOFT_STEP_COUNT)
//                 { // motor will ramp up in 17 steps slow to full speed in 17 * 30 ms
//                     setMotorDuty(i);
//                     vTaskDelay(30 / portTICK_PERIOD_MS);
//                 } // after half second motor will start at full power
//                 while ((millis() - motorRunDuration) < 3000)
//                 {
//                     if (isGantryClear())
//                         break;
//                     vTaskDelay(25 / portTICK_PERIOD_MS);
//                 }
//                 if ((millis() - motorRunDuration) > 3000)
//                 { // TODO we will replace this actual time diff between two ldr high low events
//                     cardThrowState = STATE_CARD_JAMMED;
//                 }
//                 else
//                 {
//                     cardThrowState = STATE_CARD_SUCCESS;
//                     moveServo(180);
//                     vTaskDelay(500 / portTICK_PERIOD_MS);
//                 }
//             }
//             else
//             {
//                 cardThrowState = STATE_CARD_EMPTY;
//             }
//             break;

//         case STATE_CARD_JAMMED:
//             stopMotor();
//             buzzerTone(2000, 2000);
//             break;

//         default:
//             break;
//         }
//         vTaskDelay(100 / portTICK_PERIOD_MS);
//     }
// }

// void setCardThrowState(card_throw_sates state)
// {
//     cardThrowState = state;
// }

// card_throw_sates getCardThrowState()
// {
//     return cardThrowState;
// }

// #include "Arduino.h"
// #include "card_throw_state.h"
// #include <hardware_drivers/ldr/ldr.h>
// #include <hardware_drivers/servo/servo.h>
// #include <config/hardware.h>
// #include <hardware_drivers/motor/motor.h>
// #include <hardware_drivers/buzzer/buzzer.h>

// volatile card_throw_sates cardThrowState = STATE_IDLE;
// // volatile card_throw_sates cardThrowState = STATE_RUNNING;

// /**
//  * step 1
//  * Check if stack is not empty (later implementation)
//  *
//  * step 2
//  * push servo to positive 180 wait 80ms
//  *
//  * step 3
//  * check ldr detects something in gantry then ldr will read low start store that time count
//  * if ldr dont detect clear within 3 second stop motor and return status code of 500
//  * if ldr shows gantry clear less than 3 second
//  * return status code 200
//  *
//  */
// void throwCardTask(void *pvArgs)
// {
//     Serial.println("Card Throw Task Started");

//     for (;;)
//     {
//         long motorRunDuration = 0;

//         // Serial.print("Current cardThrowState: ");

//         switch (cardThrowState)

//         {
//         case STATE_IDLE:
//             Serial.println("STATE_IDLE");
//             /* code */
//             //  moveServo(180);
//             // Serial.println("gave command to move cl 360");
//             // vTaskDelay(500 / portTICK_PERIOD_MS);
//             moveServo(0);
//             // Serial.println("gave command to move ccw 0");
//             // vTaskDelay(500 / portTICK_PERIOD_MS);
//             vTaskDelay(100 / portTICK_PERIOD_MS); // current architecture well only use this approch later we will implemetn xqueuesend/revice
//             break;

//         case STATE_RUNNING:

//             Serial.println("STATE_RUNNING entered");

//             // moveServo(180);
//             // Serial.println("gave command to move cl 360");
//             // vTaskDelay(500 / portTICK_PERIOD_MS);
//             // moveServo(0);
//             // Serial.println("gave command to move ccw 0");
//             // vTaskDelay(1500 / portTICK_PERIOD_MS);
//             // setMotorDuty(255);
//             // vTaskDelay(1500 / portTICK_PERIOD_MS);
//             // setMotorDuty(0);
//             // vTaskDelay(1500 / portTICK_PERIOD_MS);
//             // vTaskDelay(4000/portTICK_PERIOD_MS);
//             // cardThrowState = STATE_CARD_SUCCESS;
//             // -----------------------------------------

//             if (!isStackEmpty())
//             {
//                 Serial.println("Stack detected (not empty)");

//                 moveServo(180);
//                 Serial.println("Servo moved to 180");

//                 vTaskDelay(1000 / portTICK_PERIOD_MS);

//                 motorRunDuration = millis(); // motor starting time

//                 Serial.println("Motor ramp start");

//                 for (u_int16_t i = 0; i < MOTOR_PWM_MAX; i += MOTOR_PWM_SOFT_STEP_COUNT)
//                 { // motor will ramp up in 17 steps slow to full speed in 17 * 30 ms
//                     Serial.print("Motor PWM duty: ");
//                     Serial.println(i);

//                     setMotorDuty(i);
//                     vTaskDelay(30 / portTICK_PERIOD_MS);
//                 } // after half second motor will start at full power

//                 Serial.println("Motor at full power");

//                 while ((millis() - motorRunDuration) < 3000)
//                 {
//                     Serial.print("Motor run time: ");
//                     Serial.println(millis() - motorRunDuration);

//                     if (isGantryClear())
//                     {
//                         Serial.println("Gantry clear detected");
//                         break;
//                     }

//                     vTaskDelay(25 / portTICK_PERIOD_MS);
//                 }
//                 Serial.print("is gantry Clear");
//                 Serial.println(isGantryClear());
//                 if ((millis() - motorRunDuration) > 3000 && !isGantryClear())
//                 { // TODO we will replace this actual time diff between two ldr high low events

//                     Serial.println("Card JAMMED detected");

//                     cardThrowState = STATE_CARD_JAMMED;
//                 }
//                 else
//                 {
//                     Serial.println("Card SUCCESS");

//                     cardThrowState = STATE_CARD_SUCCESS;

//                     stopMotor(); // stop the throw motor before resetting servo
//                     Serial.println("Motor stopped after success");

//                     moveServo(0);
//                     Serial.println("Servo reset");

//                     vTaskDelay(1000 / portTICK_PERIOD_MS);
//                 }
//             }
//             else
//             {
//                 Serial.println("Stack EMPTY");

//                 cardThrowState = STATE_CARD_EMPTY;
//             }

//             // -----------------------------------------

//             break;

//         case STATE_CARD_JAMMED:

//             Serial.println("STATE_CARD_JAMMED triggered");

//             stopMotor();
//             // Serial.println("Motor stopped");

//             buzzerTone(2000, 2000);
//             // Serial.println("Buzzer activated");

//             cardThrowState = STATE_IDLE; // reset so we don't loop the jam handler forever
//             break;

//         case STATE_CARD_SUCCESS:
//             Serial.println("Success state reached");

//         break;

//         default:
//             Serial.println("Unknown state reached");
//             break;
//         }

//         vTaskDelay(100 / portTICK_PERIOD_MS);
//     }
// }

// void setCardThrowState(card_throw_sates state)
// {
//     Serial.print("setCardThrowState() -> ");
//     Serial.println(state);

//     cardThrowState = state;
// }

// card_throw_sates getCardThrowState()
// {
//     return cardThrowState;
// }

#include "Arduino.h"
#include "card_throw_state.h"
#include <hardware_drivers/ldr/ldr.h>
#include <hardware_drivers/servo/servo.h>
#include <config/hardware.h>
#include <hardware_drivers/motor/motor.h>
#include <hardware_drivers/buzzer/buzzer.h>
#include "card_throw_queue.h"

// volatile card_throw_sates cardThrowState = STATE_IDLE;
// volatile card_throw_sates cardThrowState = STATE_RUNNING;

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
void throwCardTask(void *pvArgs)
{
    Serial.println("Card Throw Task Started");

    for (;;)
    {
        Serial.println("Card Throw Task loop");

        ThrowCommand throwCmd;
        if (xQueueReceive(throwCommandQueue, &throwCmd, portMAX_DELAY) == pdTRUE)
        {
            switch (throwCmd)
            {
            case CMD_CARD_THROW:
            {
                moveServo(0);
                vTaskDelay(800 / portTICK_PERIOD_MS);
                ThrowResult throwRes = RESULT_SUCCESS;
                for (u_int16_t i = 0; i < MOTOR_PWM_MAX; i += MOTOR_PWM_SOFT_STEP_COUNT)
                { // motor will ramp up in 17 steps slow to full speed in 17 * 30 ms
                    Serial.print("Motor PWM duty: ");
                    Serial.println(i);
                    Serial.println(isGantryClear() ? "GANTRY CLEAR" : "GANTRY BLOCKED");
                    setMotorDuty(i);
                    vTaskDelay(30 / portTICK_PERIOD_MS);
                }
                setMotorDuty(255);
                vTaskDelay(1500/portTICK_RATE_MS);
                stopMotor();
                moveServo(180);
                vTaskDelay(800/portTICK_RATE_MS);

                // vTaskDelay(5000 / portTICK_PERIOD_MS);
                // moveServo(0); // this throw card forward 0 deg 180 pulls it back
                // vTaskDelay(5000 / portTICK_PERIOD_MS);
                // Serial.println("in 5 sec servo moving 180 degree");
                // vTaskDelay(2000 / portTICK_PERIOD_MS);

                xQueueSend(throwResultQueue, &throwRes, portMAX_DELAY);
                break;
            }

            case CMD_CARD_HALT:
            {
                Serial.println("stopping Execution");
                break;
            }

            default:
            {
                break;
            }
            }
        }

        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}
