#include <cstdint>
#ifndef MOTOR_H
void motorInit();
void setMotorDuty(uint8_t pwm);
void stopMotor();
void motorTask(void *taskParameter);

#endif