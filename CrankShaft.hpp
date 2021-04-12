/* Nick Dreitzler
 * Purpose: class to contol the movements of crankshaft release of the game pieces into the sorter
 * Micro Step 16, wired value for microstepping on A4988
 * Must give params to the constructor
 * @Params for constructor: 
 *    Stepper Motor Params:
 *      stepPin: step pin connected to A4988
 *      dirPin: direction pin connected to the A4988
 *      enPin:  enable pin connected to the A4988
 *      stepDelay: step delay, the delay between each toggle of the step pin when rotating stepper motor
 */

#ifndef CRANK_SHAFT_H
#define CRANK_SHAFT_H

#include "Connect4.h"
#include "StepperMotor.hpp"

class CrankShaft : private StepperMotor{
private: 
byte currentStage;

public:
    CrankShaft( byte stepPin, byte dirPin, byte enPin, int stepDelay) 
        : StepperMotor{ stepPin, dirPin, enPin, stepDelay}
        { currentStage = 0; }


  void advanceRelease(void) //advance the stage of the crankshaft, by advance I mean close the last column and open the next
  {
    turnMotorOn();
    // Serial.print("cs is ");
    // Serial.print(currentStage);
    // Serial.print("\n");
    if(currentStage == 0 || currentStage == WIDTH + 1) 
    { 
        // Serial.println("cs is 0 or 9");
        moveMotor(stepsPerRevolution*CS_MICRO_STEP/2/(WIDTH + 1), false);//Make a half advancment, either open the first column, or close the last
    } else {
        // Serial.println("cs is 1 to 8");
        moveMotor(stepsPerRevolution*CS_MICRO_STEP/(WIDTH + 1), false); //Make a full advancment
    }
    currentStage = (currentStage + 1) % (WIDTH+2);//% 9 for a normal sized gameboard
    turnMotorOff();
  }
};

#endif
