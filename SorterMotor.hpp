/* Nick Dreitzler
 * Purpose: Class to control sorter flap used to sort pieces
 */
#ifndef SORTER_MOTOR_H
#define SORTER_MOTOR_H

#include "Connect4.h"
#include "StepperMotor.hpp"

class SorterMotor : private StepperMotor{
public:
    SorterMotor(byte sp, byte dp, byte ms, int sd)
        : StepperMotor{ sp, dp, ms, sd}
        {}


    //Sort tokens
    void moveSorterFlap(bool isPurple) 
    {
        if(isPurple)
        {
            moveMotor(sorterMove, true); //Sort token into purple
            delay(ONE_SEC);
            moveMotor(sorterMove, false); //Return to center
        } else {
            moveMotor(sorterMove, false); //Sort token into orange
            delay(ONE_SEC);
            moveMotor(sorterMove, true); //Return to center
        }
        delay(ONE_SEC);
    }

private:
    static const int sorterMove = 10;

};


#endif
