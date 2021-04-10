/* Nick Dreitzler
 * Purpose: class to contol the movements of corkscrews used to move tokens from sorter to the dropper
 * Must give params to the constructor
 * @Params for constructor: 
 *    Stepper Motor Params:
 *      stepPin: step pin connected to A4988
 *      dirPin: direction pin connected to the A4988
 *      enPin:  enable pin connected to the A4988
 *      microStep: wired value for microstepping on A4988
 *      stepDelay: step delay, the delay between each toggle of the step pin when rotating stepper motor
 */
#ifndef CORKSCREW_H
#define CORKSCREW_H

#include "Connect4.h"
#include "StepperMotor.hpp"

#define MOVE_DISTANCE 20 //Steps to move tokens up or down once on the corkscrew

class Corkscrew : private StepperMotor{
public:
    Corkscrew( byte stepPin, byte dirPin, byte enPin, byte microStep, int stepDelay) 
        : StepperMotor{ stepPin, dirPin, enPin, microStep, stepDelay}
        {distanceFromBottom = 0;}

    //Move tokens up one slot in the corkscrew
    void moveUpOne(void)
    {
        ++distanceFromBottom;
        moveMotor(MOVE_DISTANCE, CLOCK_WISE);
    }

    //Lower tokens to the bottom of the corkscrew
    void dropToBottom(void)
    {
        for(int i = 0; i < distanceFromBottom; ++i)
        {
            moveMotor(MOVE_DISTANCE, COUNTER_WISE);
        }
        distanceFromBottom = 0;
    }

private:
    byte distanceFromBottom;
}; 


#endif