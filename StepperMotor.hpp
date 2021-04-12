/* Nick Dreitzler
 * Purpose: Class to control the movement of a steper moter using a A4988 motor driver
 * Must give params to the constructor
 * @Params for constructor: 
 *      stepPin: step pin connected to A4988
 *      dirPin: direction pin connected to the A4988
 *      enPin:  enable pin connected to the A4988
 *      stepDelay: step delay, the delay between each toggle of the step pin when rotating stepper motor
 */

#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

class StepperMotor{
public:
    StepperMotor(byte stepPin, byte dirPin, byte enPin, int stepDelay)
    {
        this->stepPin = stepPin;
        this->dirPin = dirPin;
        //this->microStep = microStep;
        this->stepDelay = stepDelay;
        isOn = false;
        digitalWrite(this->stepPin, LOW); //Init step pin value
        digitalWrite(this->dirPin, LOW); //Init dir pin value
        digitalWrite(this->enPin, HIGH); //Disable motor
    }

    void setStepDelay(int stepDelay)
    {
        this->stepDelay = stepDelay;
    }

protected:
    static const byte stepsPerRevolution = 200;   
    //byte microStep; //microStep value on A4988 driver 
    
    //Move the attachted motor numSteps steps.
    //If clockwise is true the motor will step in the clockwise direction
    inline void moveMotor(int numSteps, bool clockwise)
    {
        //Serial.println("enter move");
        if(isOn)
        {
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
        } else {
            Serial.println("Motor off");
        }
    }

    //enable stepper motor
    inline void turnMotorOn(void)
    {
        isOn = true;
        digitalWrite(enPin, LOW);
    }

    //enable stepper motor
    inline void turnMotorOff(void)
    {
        isOn = false;
        digitalWrite(enPin, HIGH);
    }
  
private:
    bool isOn;
    byte stepPin;   //Arduino pin connected to A4988 driver step pin 
    byte dirPin;    //Arduino pin connected to A4988 driver dir pin 
    byte enPin;     //Arduino pin connected to A4988 driver en pin, hold LOW to enable, hold HIGH to disable
    int stepDelay;  //Micro seconds between toggle of step pin

};

#endif
