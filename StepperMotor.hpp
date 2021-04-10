/* Nick Dreitzler
 * Purpose: Class to control the movement of a steper moter using a A4988 motor driver
 */

#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

class StepperMotor{
protected: 
    static const byte stepsPerRevolution = 200;   
    byte stepPin;   //Arduino pin connected to A4988 driver step pin 
    byte dirPin;    //Arduino pin connected to A4988 driver dir pin 
    byte enPin;     //Arduino pin connected to A4988 driver en pin 
    byte microStep; //microStep value on A4988 driver 
    int stepDelay;  //Micro seconds between toggle of step pin

public:
    StepperMotor(byte stepPin, byte dirPin, byte enPin, byte microStep, int stepDelay)
    {
        this->stepPin = stepPin;
        this->dirPin = dirPin;
        this->microStep = microStep;
        this->stepDelay = stepDelay;
        digitalWrite(this->stepPin, LOW);
        digitalWrite(this->dirPin, LOW);
    }

    void setStepDelay(int stepDelay)
    {
        this->stepDelay = stepDelay;
    }

    //Move the attachted motor numSteps steps.
    //If clockwise is true the motor will step in the clockwise direction
    void moveMotor(int numSteps, bool clockwise)
    {
        //Serial.println("enter move");
        if(clockwise) // Set direction of motor
        {
            digitalWrite(dirPin, HIGH);
        } else {
            digitalWrite(dirPin, LOW);
        }
        //Serial.println("enter for");
        for(int i = 0; i < numSteps*2; ++i)//A4988 causes the motor to step after a pulse, it takes 2 toggles for 1 pulse
        {
            digitalWrite(stepPin, !digitalRead(stepPin));
            delayMicroseconds(stepDelay);
        }
        //Serial.println("Leave move");
    }
  
};

#endif
