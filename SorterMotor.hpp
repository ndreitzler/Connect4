/* Nick Dreitzler
 * Purpose: Class to control sorter flap used to sort pieces
 * Micro Step 2, wired value for microstepping on A4988
 * Must give params to the constructor
 * @Params for constructor: 
 *    Stepper Motor Params:
 *      stepPin: step pin connected to A4988
 *      dirPin: direction pin connected to the A4988
 *      enPin:  enable pin connected to the A4988
 *      stepDelay: step delay, the delay between each toggle of the step pin when rotating stepper motor
 *    SorterMotor Params:
 *      sorterButton: pin connected to button on sorter motor, used to calibrate sorter location
 */

#ifndef SORTER_MOTOR_H
#define SORTER_MOTOR_H

#include "Connect4.h"
#include "StepperMotor.hpp"

#define INIT_STEP 1  //Number of steps to make before checking if the button has been pressed 
#define BUTTON_TO_CENTER 14 //Nmbmer of steps from the button to the center of the sorter
#define SORT_MOVE 16  //Number of steps the sorter must make in order to sort pieces
#define ORANGE_ADJUSTMENT (-1) //Adjustment made to the number of steps needed to sort orange pieces


class SorterMotor : public StepperMotor{
public:
    SorterMotor(byte stepPin, byte dirPin, byte enPin, int stepDelay, byte sorterButton) 
        : StepperMotor{ stepPin, dirPin, enPin, stepDelay}
        {
          //Serial.println("test");
          this->sorterButton = sorterButton;
          initSorter();
        }

    void turnSorterMotorOn(void)
    {
      turnMotorOn();
    }

    void turnSorterMotorOff(void)
    {
      turnMotorOff();
    }

    //Sort tokens
    void moveSorterFlap(bool isPurple) 
    {
        
        if(isPurple)
        {
            moveMotor(SORT_MOVE, CLOCK_WISE); //Sort token into purple
            delay(ONE_SEC);
            moveMotor(SORT_MOVE, COUNTER_WISE); //Return to center
        } else {
            moveMotor(SORT_MOVE + ORANGE_ADJUSTMENT, COUNTER_WISE); //Sort token into orange
            delay(ONE_SEC);
            moveMotor(SORT_MOVE + ORANGE_ADJUSTMENT, CLOCK_WISE); //Return to center
        }
        delay(ONE_SEC);
    }

    void initSorter(void)
    {
      Serial.println("init");
      turnMotorOn();
      while(digitalRead(sorterButton) == LOW) //Move motor counterclockwise until button is pressed
      {
        moveMotor(INIT_STEP, COUNTER_WISE);
      }
      delay(1000);
      moveMotor(BUTTON_TO_CENTER, CLOCK_WISE); //Move motor clockwise to center of sorter
      turnMotorOff();
    }

private:
  byte sorterButton;
    //static const int sorterMove = 20;

};


#endif
