/* Nick Dreitzler
 * Purpose: class to contol the movements of droppers release of the game pieces into the gameboard.
 * Micro Step 8, wired value for microstepping on A4988
 * Must give params to the constructor
 * @Params for constructor: 
 *    Stepper Motor Params:
 *      stepPin: step pin connected to A4988
 *      dirPin: direction pin connected to the A4988
 *      enPin:  enable pin connected to the A4988
 *      microStep: wired value for microstepping on A4988
 *      stepDelay: step delay, the delay between each toggle of the step pin when rotating stepper motor
 *    Dropper Params:
 *      triggerPin: pin connected to normally open button used to calibrate dropper location
 * 
 * @Dependencies
 *      StepperMotor.hpp
 */
#ifndef DROPPER_H
#define DROPPER_H

#include "Connect4.h"
#include "StepperMotor.hpp"

#define COLUMN_DIFF 257 //Number of steps between each column
#define BACK_COMP 8 //Number of steps to compensate for backlash in threaded rod
#define TRIGGER 500 //Value to be compared to the force sensor reading. When the force sensor read a value hight than this, the dropper will be zeroed
#define INIT_VAL 200 //the number of steps need to be over the first column starting from the wall with the motors
#define INIT_UDELAY 200 //To make the motor move slower during init. This helps prevent damage to the game body

class Dropper : private StepperMotor{
public:
    Dropper(byte stepPin, byte dirPin, byte enPin, int stepDelay, byte triggerPin) 
        : StepperMotor{ stepPin, dirPin, enPin, stepDelay}
        {     
            this->triggerPin = triggerPin;
            currentLoc = 6;
            currentDirection = 1;
            for(int i = 0; i < 7; ++i )//set columnLoc values
            {
                columnLocs[i] = COLUMN_DIFF*i;
            }
            initMotor();
        }

  //Move dispencer to new location
  void moveDropper(byte newLoc)
  {
    int i;
    int diff = columnLocs[currentLoc] - columnLocs[newLoc];
    byte backlashComp = 0;

//    Serial.println("enter");
//    Serial.println(diff);
//    Serial.println(currentLoc);
//    Serial.println(columnLocs[currentLoc]);
//    Serial.println(newLoc);
//    Serial.println(columnLocs[newLoc]);
    turnMotorOn();
    if(diff > 0) //new column is to the left of the current column
    {
      if(currentDirection == 1) //If changing directions compensate for backlash in threaded rod
      {
        backlashComp = BACK_COMP;
        //Serial.println("right to left");
      }
      //move motor
      moveMotor(diff*DROP_MICRO_STEP + backlashComp, CLOCK_WISE);
      currentDirection = 0;
    }
    else if (diff < 0) //new column is to the right of the current column
    {
      if(currentDirection == 0) //If changing directions compensate for backlash in threaded rod
      {
        backlashComp = BACK_COMP;
        //Serial.println("left to right");
      }
      diff = -diff;
      //move motor
      moveMotor(diff*DROP_MICRO_STEP + backlashComp, COUNTER_WISE);
      currentDirection = 1;
    }
    turnMotorOff();

    currentLoc = newLoc;
  }

  void initMotor(void)
  {
    turnMotorOn();
    setStepDelay(INIT_UDELAY);
    Serial.println("init");
    while(digitalRead(triggerPin) == LOW)
    {
      moveMotor(DROP_MICRO_STEP, COUNTER_WISE);
    }
    moveMotor(INIT_VAL, CLOCK_WISE);
    setStepDelay(DROP_UDELAY);
    turnMotorOff();
  }

  void printCols(void) 
  {
    for(int i = 0; i < 7; ++i )
    {
      Serial.println(columnLocs[i]);
    }
  }

private:
  //static const int COLUMN_DIFF = 257;//Number of steps between each column    
  int columnLocs[7];
  byte currentDirection;
  byte currentLoc;
  byte triggerPin;


};


#endif
