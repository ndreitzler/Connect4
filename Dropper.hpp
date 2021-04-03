/* Nick Dreitzler
 * Purpose: class to contol the movements of droppers release of the game pieces into the gameboard
 */
#ifndef DROPPER_H
#define DROPPER_H

#include "Connect4.h"
#include "StepperMotor.hpp"

#define COLUMN_DIFF 257
#define BACK_COMP 8

class Dropper : private StepperMotor{
public:
    Dropper(byte sp, byte dp, byte ms, int sd)
        : StepperMotor{ sp, dp, ms, sd}
        {     
            currentLoc = 0;
            currentDirection = 0;
            for(int i = 0; i < 7; ++i )
            {
                columnLocs[i] = COLUMN_DIFF*i;
            }
        }

  //Move dispencer to new location
  void moveDispenser(byte newLoc)
  {
    int i;
    int diff = columnLocs[currentLoc] - columnLocs[newLoc];
    int backlashComp = 0;
    
    if(diff > 0) //new column is to the left of the current column
    {
      if(currentDirection == 1) //If changing directions compensate for backlash in threaded rod
      {
        backlashComp = BACK_COMP;
        //Serial.println("right to left");
      }
      //move motor
      moveMotor(diff*microStep + backlashComp, true);
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
      moveMotor(diff*microStep + backlashComp, false);
      currentDirection = 1;
    }

    currentLoc = newLoc;
  }
    
private:
  //static const int COLUMN_DIFF = 257;//Number of steps between each column    
  byte columnLocs[7];
  byte currentDirection;
  byte currentLoc;

  

//   //Drop token into gameboard
//   void dropToken(bool isPurple) //If isPurple is true drop a purple token, otherwise drop a orange token 
//   {
//       //Drop Token
//   }

};


#endif