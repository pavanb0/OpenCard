
#include "Arduino.h"
#include "../src/config/pinmap.h"
#include <Stepper.h>

const int stepsPerReoulution = 2048; 
Stepper turretStepper(stepsPerReoulution,STEPPER_L1,STEPPER_L2,STEPPER_L3,STEPPER_L4);
void stepperInit(){
    turretStepper.setSpeed(5);
}

void turretHoming(){ 
    // TODO implement it here in future first dcmotor need to spin
}

