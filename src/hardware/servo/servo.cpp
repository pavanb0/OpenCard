#include <ESP32Servo.h>
#include <config/pinmap.h>

Servo pushServo;
void servoInit(){
    pushServo.attach(SG90_SERVO);
}

void moveServo(int angle){
    pushServo.write(angle);
}