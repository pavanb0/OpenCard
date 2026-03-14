
#include "Arduino.h"
#include "../src/config/pinmap.h"
#include <Stepper.h>
 

const int STEPS_PER_REV = 2048;
Stepper turretStepper(STEPS_PER_REV, STEPPER_L1, STEPPER_L3, STEPPER_L2, STEPPER_L4);

void stepperRelease() {
  digitalWrite(STEPPER_L1, LOW);
  digitalWrite(STEPPER_L2, LOW);
  digitalWrite(STEPPER_L3, LOW);
  digitalWrite(STEPPER_L4, LOW);
}

void stepperInit(){
  turretStepper.setSpeed(15);   
    //TODO add homing logic 
}
void stepperMove(int angle){ //give 0-360 angle and move accordingly
    turretStepper.step(( STEPS_PER_REV / 360 ) * angle);
    stepperRelease();
}
void turretHoming(){ 
    // TODO implement it here in future first dcmotor need to spin
}



 
// // Define Variables
 
// // Number of Steps Required
// int StepsRequired;
 
// // Create Instance of Stepper Class
// // Specify Pins used for motor coils
// // The pins used are 8,9,10,11 
// // Connected to ULN2003 Motor Driver In1, In2, In3, In4 
// // Pins entered in sequence 1-3-2-4 for proper step sequencing
 
// Stepper steppermotor(STEPS_PER_REV, 8, 10, 9, 11);
 
// void setup()
// {
// // Nothing  (Stepper Library sets pins as outputs)
// }
 
// void loop()
// {
//   // Slow - 4-step CW sequence to observe lights on driver board
//   steppermotor.setSpeed(1);    
//   StepsRequired  =  4;
//   steppermotor.step(StepsRequired);
//   delay(2000);
 
//    // Rotate CW 1/2 turn slowly
//   StepsRequired  =  STEPS_PER_OUT_REV / 2; 
//   steppermotor.setSpeed(100);   
//   steppermotor.step(StepsRequired);
//   delay(1000);
  
//   // Rotate CCW 1/2 turn quickly
//   StepsRequired  =  - STEPS_PER_OUT_REV / 2;   
//   steppermotor.setSpeed(700);  
//   steppermotor.step(StepsRequired);
//   delay(2000);
 
// }