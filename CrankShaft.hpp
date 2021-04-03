/* Nick Dreitzler
 * Purpose: class to contol the movements of crankshaft release of the game pieces into the sorter
 */

#ifndef CRANK_SHAFT_H
#define CRANK_SHAFT_H

#include "Connect4.h"
#include "StepperMotor.hpp"

class CrankShaft : private StepperMotor{
private: 
byte currentStage;

public:
    CrankShaft( byte sp, byte dp, byte ms, int sd) 
        : StepperMotor{ sp, dp, ms, sd}
        { currentStage = 0;}

  void advanceRelease(void)
  {
    if(currentStage == 0 || currentStage == WIDTH + 1)
    { 
        moveMotor(stepsPerRevolution*microStep/2/(WIDTH + 1), true);
    } else {
        moveMotor(stepsPerRevolution*microStep/(WIDTH + 1), true); 
    }
    currentStage = (currentStage + 1) % (WIDTH+2);//% 9 for a normal sized gameboard
  }

};

#endif