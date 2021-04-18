/* Nick Dreitzler
   Purpose: class to contol the movements of corkscrews used to move tokens from sorter to the dropper
   Micro Step 16, wired value for microstepping on A4988
   Must give params to the constructor
   @Params for constructor:
      Stepper Motor Params:
        stepPin: step pin connected to A4988
        dirPin: direction pin connected to the A4988
        enPin:  enable pin connected to the A4988
        stepDelay: step delay, the delay between each toggle of the step pin when rotating stepper motor

   @Dependencies
        StepperMotor.hpp
*/
#ifndef CORKSCREW_H
#define CORKSCREW_H

#include "Connect4.h"
#include "StepperMotor.hpp"

#define JIGGLE_NUM 3
#define JIGGLE_DIV 16
#define NUMBER_OF_JIGGLES 2
#define EXTRA_STEPS 50

class Corkscrew : public StepperMotor {
  public:
    Corkscrew( byte stepPin, byte dirPin, byte enPin, int stepDelay)
      : StepperMotor{ stepPin, dirPin, enPin, stepDelay}
    {
      distanceFromBottom = 0;
    }

    //Move tokens up one slot in the corkscrew
    void moveUpOne(void)
    {
      ++distanceFromBottom;
      turnMotorOn();
      moveMotor((CORK_PURPLE_MICRO_STEP * stepsPerRevolution), COUNTER_WISE);
      turnMotorOff();
    }

    //Move tokens up one slot in the corkscrew
    void loadUpOne(void)
    {
      turnMotorOn();
      moveMotor((CORK_PURPLE_MICRO_STEP * stepsPerRevolution)/4+EXTRA_STEPS, COUNTER_WISE);
      //Jiggle for stuck tokens
      for(int i = 0; i < NUMBER_OF_JIGGLES; ++i)
      {
        moveMotor((CORK_PURPLE_MICRO_STEP * stepsPerRevolution)*JIGGLE_NUM/JIGGLE_DIV, CLOCK_WISE);
        delay(100);
        moveMotor((CORK_PURPLE_MICRO_STEP * stepsPerRevolution)*JIGGLE_NUM/JIGGLE_DIV, COUNTER_WISE);
      }
      moveMotor((CORK_PURPLE_MICRO_STEP * stepsPerRevolution*3)/4-EXTRA_STEPS, COUNTER_WISE);
      turnMotorOff();
    }
    
    //Move tokens up one slot in the corkscrew
    void debugUp(byte num)
    {
      turnMotorOn();
      for (int i = 0; i < num; ++i)
        moveMotor(CORK_PURPLE_MICRO_STEP * stepsPerRevolution, COUNTER_WISE);
      turnMotorOff();
    }

    //Lower tokens to the bottom of the corkscrew
    void dropToBottom(void)
    {
      turnMotorOn();
      //Serial.println(distanceFromBottom);
      for (int i = 0; i < distanceFromBottom; ++i)
      {
        //Serial.println("test");
        moveMotor(CORK_PURPLE_MICRO_STEP * stepsPerRevolution, CLOCK_WISE);
        //moveMotor(50000, CLOCK_WISE);
      }
      turnMotorOff();
      distanceFromBottom = 0;
    }

    void initMotor(void)
    {
      return;
    }

  private:
    byte distanceFromBottom;
};


#endif
