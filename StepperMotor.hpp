/* Nick Dreitzler
 * Purpose: Class to control the movement of a steper moter using a A4988 motor driver
 */

#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

class StepperMotor{
protected: 
    static const byte stepsPerRevolution = 200;   
    byte stepPin;
    byte dirPin;
    byte microStep;
    int stepDelay;

public:
    StepperMotor(byte sp, byte dp, byte ms, int sd)
    {
        stepPin = sp;
        dirPin = dp;
        microStep = ms;
        stepDelay = sd;
        digitalWrite(stepPin, LOW);
        digitalWrite(dirPin, LOW);
    }

    void setStepDelay(int sd)
    {
        stepDelay = sd;
    }

    //Move the attachted motor numSteps steps.
    //If clockwise is true the motor will step in the clockwise direction
    void moveMotor(int numSteps, bool clockwise)
    {
        if(clockwise)
        {
            digitalWrite(dirPin, HIGH);
        } else {
            digitalWrite(dirPin, LOW);
        }
        for(int i = 0; i < numSteps*2; ++i)//A4988 causes the motor to step after a pulse, it takes 2 toggles for 1 pulse
        {
            digitalWrite(stepPin, !digitalRead(stepPin));
            delay(stepDelay);
        }
    }
  
};

#endif
