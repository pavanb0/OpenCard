#include <ESP32Servo.h>
#include <config/pinmap.h>


#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#define SERVO_PIN SG90_SERVO

void servoInit() {
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, SERVO_PIN);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50;    
    pwm_config.cmpr_a = 0;
    pwm_config.cmpr_b = 0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
}

void moveServo(int angle) {
    float duty = (angle * 2.0 / 180.0) + 0.5;
    mcpwm_set_duty_in_us(
        MCPWM_UNIT_0,
        MCPWM_TIMER_0,
        MCPWM_OPR_A,
        duty * 1000
    );
}
