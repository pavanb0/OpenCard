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


#include "Arduino.h"
#include "card_throw_state.h"
#include <hardware_drivers/ldr/ldr.h>
#include <hardware_drivers/servo/servo.h>
#include <config/hardware.h>
#include <hardware_drivers/motor/motor.h>
#include <hardware_drivers/buzzer/buzzer.h>

volatile card_throw_sates cardThrowState = STATE_IDLE;
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
        long motorRunDuration = 0;

        Serial.print("Current cardThrowState: ");
        Serial.println(cardThrowState);

        switch (cardThrowState)

        {
        case STATE_IDLE:
            Serial.println("STATE_IDLE");
            /* code */
            vTaskDelay(100 / portTICK_PERIOD_MS); // current architecture well only use this approch later we will implemetn xqueuesend/revice
            break;

        case STATE_RUNNING:

            Serial.println("STATE_RUNNING entered");

            // moveServo(180);
            // Serial.println("gave command to move cl 360");
            // vTaskDelay(500 / portTICK_PERIOD_MS);
            // moveServo(0);
            // Serial.println("gave command to move ccw 0");
            // vTaskDelay(500 / portTICK_PERIOD_MS);

            if (!isStackEmpty())
            {
                Serial.println("Stack detected (not empty)");

                moveServo(180);
                Serial.println("Servo moved to 180");

                vTaskDelay(500 / portTICK_PERIOD_MS);

                motorRunDuration = millis(); // motor starting time

                Serial.println("Motor ramp start");

                for (u_int16_t i = 0; i < MOTOR_PWM_MAX; i += MOTOR_PWM_SOFT_STEP_COUNT)
                { // motor will ramp up in 17 steps slow to full speed in 17 * 30 ms
                    Serial.print("Motor PWM duty: ");
                    Serial.println(i);

                    setMotorDuty(i);
                    vTaskDelay(30 / portTICK_PERIOD_MS);
                } // after half second motor will start at full power

                Serial.println("Motor at full power");

                while ((millis() - motorRunDuration) < 3000)
                {
                    Serial.print("Motor run time: ");
                    Serial.println(millis() - motorRunDuration);

                    if (isGantryClear())
                    {
                        Serial.println("Gantry clear detected");
                        break;
                    }

                    vTaskDelay(25 / portTICK_PERIOD_MS);
                }

                if ((millis() - motorRunDuration) > 3000)
                { // TODO we will replace this actual time diff between two ldr high low events

                    Serial.println("Card JAMMED detected");

                    cardThrowState = STATE_CARD_JAMMED;
                }
                else
                {
                    Serial.println("Card SUCCESS");

                    cardThrowState = STATE_CARD_SUCCESS;

                    moveServo(180);
                    Serial.println("Servo reset");

                    vTaskDelay(500 / portTICK_PERIOD_MS);
                }
            }
            else
            {
                Serial.println("Stack EMPTY");

                cardThrowState = STATE_CARD_EMPTY;
            }

        case STATE_CARD_JAMMED:

            Serial.println("STATE_CARD_JAMMED triggered");

            stopMotor();
            Serial.println("Motor stopped");

            buzzerTone(2000, 2000);
            Serial.println("Buzzer activated");

            break;

        default:
            Serial.println("Unknown state reached");
            break;
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void setCardThrowState(card_throw_sates state)
{
    Serial.print("setCardThrowState() -> ");
    Serial.println(state);

    cardThrowState = state;
}

card_throw_sates getCardThrowState()
{
    return cardThrowState;
}



/**
 * some Serial out for debug purpose only 
 * un time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 550
Motor run time: 575
Motor run time: 600
Motor run time: 625
Motor run time: 650
Motor run time: 675
Motor run time: 700
Motor run time: 725
Motor run time: 750
Motor run time: 775
Motor run time: 800
Motor run time: 825
Motor run time: 850
Motor run time: 875
Motor run time: 900
Motor run time: 925
Motor run time: 950
Motor run time: 975
Motor run time: 1000
Motor run time: 1025
Motor run time: 1050
Motor run time: 1075
Motor run time: 1100
Motor run time: 1125
Motor run time: 1150
Motor run time: 1175
Motor run time: 1200
Motor run time: 1225
Motor run time: 1250
Motor run time: 1275
Motor run time: 1300
Motor run time: 1325
Motor run time: 1350
Motor run time: 1375
Motor run time: 1400
Motor run time: 1425
Motor run time: 1450
Motor run time: 1475
Motor run time: 1500
Motor run time: 1525
Motor run time: 1550
Motor run time: 1575
Motor run time: 1600
Motor run time: 1625
Motor run time: 1650
Motor run time: 1675
Motor run time: 1700
Motor run time: 1725
Motor run time: 1750
Motor run time: 1775
Motor run time: 1800
Motor run time: 1825
Motor run time: 1850
Motor run time: 1875
Motor run time: 1900
Motor run time: 1925
Motor run time: 1950
Motor run time: 1975
Motor run time: 2000
Motor run time: 2025
Motor run time: 2050
Motor run time: 2075
Motor run time: 2100
Motor run time: 2125
Motor run time: 2150
Motor run time: 2175
Motor run time: 2200
Motor run time: 2225
Motor run time: 2250
Motor run time: 2275
Motor run time: 2300
Motor run time: 2325
Motor run time: 2350
Motor run time: 2375
Motor run time: 2400
Motor run time: 2425
Motor run time: 2450
Motor run time: 2475
Motor run time: 2500
Motor run time: 2525
Motor run time: 2550
Motor run time: 2575
Motor run time: 2600
Card throw state after delay: 1
Dealing to player index: 1
setCardThrowState() -> 1
State set to STATE_RUNNING
Motor run time: 2625
Motor run time: 2650
Motor run time: 2675
Motor run time: 2700
Motor run time: 2725
Motor run time: 2750
Motor run time: 2775
Motor run time: 2800
Motor run time: 2825
Motor run time: 2850
Motor run time: 2875
Motor run time: 2900
Motor run time: 2925
Motor run time: 2950
Motor run time: 2975
Card SUCCESS
Servo reset
STATE_CARD_JAMMED triggered
Motor stopped
Buzzer activated
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Card throw state after delay: 4
Card throw SUCCESS
Stepper rotating degrees: 90
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
Current cardThrowState: 4
Unknown state reached
 */