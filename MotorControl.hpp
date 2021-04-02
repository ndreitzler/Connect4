#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "Game.hpp"

// Connections to A4988
#define DROP_DIR_PIN 11  // Direction
#define DROP_STEP_PIN 12 // Step
#define REL_DIR_PIN 23   // Direction
#define REL_STEP_PIN 22  // Step
#define SORTER_DIR_PIN 24
#define SORTER_STEP_PIN 25
#define MOVE 10
#define MS_DELAY 5000
#define ONE_SEC 1000

class MotorControl {
public:
  static const byte STEPS_PER_REV = 200; //Steps per 1 revolution of stepper motor
  static const byte DROP_MICRO_STEP = 8; //Microstep for dropper stepper motor
  static const byte REL_MICRO_STEP = 16; //Microstep for release stepper motor
  static const int DROP_DELAY = 60; //Delay between each step of the dropper stepper motor
  static const int REL_MS_DELAY = 500; //Delay between each step of the release stepper motor
  static const byte BACK_COMP = 8; //Number of clicks to compensate for backlash on threaded rod
  static const int COLUMN_DIFF = 257;//Number of steps between each column


  void dropToken(byte col)
  {
    moveDispenser(col);
    //add dropping later
  }
  
  void advanceRelease(void)
  {
    if(currentStage == 0 || currentStage == 8)
    { 
      for(int x = 0; x < STEPS_PER_REV*REL_MICRO_STEP/2/8; x++) {
        digitalWrite(REL_STEP_PIN,HIGH); 
        delayMicroseconds(REL_MS_DELAY); 
        digitalWrite(REL_STEP_PIN,LOW); 
        delayMicroseconds(REL_MS_DELAY); 
      }
    } else {
      for(int x = 0; x < STEPS_PER_REV*REL_MICRO_STEP/8; x++) {
        digitalWrite(REL_STEP_PIN,HIGH); 
        delayMicroseconds(REL_MS_DELAY); 
        digitalWrite(REL_STEP_PIN,LOW); 
        delayMicroseconds(REL_MS_DELAY); 
      }
    }
    currentStage = (currentStage + 1) % 9;
  }

  void moveSorterFlap(bool isPurple) //add real world values
  {
    if(isPurple)
    {
      digitalWrite(SORTER_DIR_PIN, HIGH);
      for(int x = 0; x < MOVE; x++) {
        digitalWrite(SORTER_STEP_PIN,HIGH); 
        delayMicroseconds(MS_DELAY); 
        digitalWrite(SORTER_STEP_PIN,LOW); 
        delayMicroseconds(MS_DELAY); 
      }
      delay(1000);
      digitalWrite(SORTER_DIR_PIN, LOW);
      for(int x = 0; x < MOVE; x++) {
        digitalWrite(SORTER_STEP_PIN,HIGH); 
        delayMicroseconds(MS_DELAY); 
        digitalWrite(SORTER_STEP_PIN,LOW); 
        delayMicroseconds(MS_DELAY); 
      }
      
    } 
    else if(!isPurple)
    {
      digitalWrite(SORTER_DIR_PIN, LOW);
      for(int x = 0; x < MOVE; x++) {
        digitalWrite(SORTER_STEP_PIN,HIGH); 
        delayMicroseconds(MS_DELAY); 
        digitalWrite(SORTER_STEP_PIN,LOW); 
        delayMicroseconds(MS_DELAY); 
      }
      digitalWrite(SORTER_DIR_PIN, HIGH);
      delay(1000);
      for(int x = 0; x < MOVE; x++) {
        digitalWrite(SORTER_STEP_PIN,HIGH); 
        delayMicroseconds(MS_DELAY); 
        digitalWrite(SORTER_STEP_PIN,LOW); 
        delayMicroseconds(MS_DELAY); 
      }
    }
    delay(1000);
    
  }

  void resetGameboard(void)
  {
    //releaseGameboard();
    moveDispenser(4);
  }

  MotorControl(){
    currentLoc = 0;
    currentStage = 0;
    currentDirection = 0;
    for(int i = 0; i < 7; ++i )
    {
      columnLocs[i] = COLUMN_DIFF*i;
    }
  }

private:
  byte currentLoc;
  byte currentStage;
  byte currentDirection;
  byte columnLocs[7] ;

  //Move dispencer to new location
  void moveDispenser(byte newLoc)
  {
    int i;
    int diff = columnLocs[currentLoc] - columnLocs[newLoc];
    int backlashComp = 0;
  //  digitalWrite(EN, LOW);
    
    if(diff > 0) //new column is to the left of the current column
    {
      digitalWrite(DROP_DIR_PIN, HIGH);
      if(currentDirection == 1) //If changing directions compensate for backlash
      {
        backlashComp = BACK_COMP;
        Serial.println("right to left");
      }
      //move motor
      for(i = 0; i < diff*DROP_MICRO_STEP + backlashComp; i++) {
        digitalWrite(DROP_STEP_PIN,HIGH); 
        delayMicroseconds(DROP_DELAY); 
        digitalWrite(DROP_STEP_PIN,LOW); 
        delayMicroseconds(DROP_DELAY); 
      }
      currentDirection = 0;
    }
    else if (diff < 0) //new column is to the right of the current column
    {
      digitalWrite(DROP_DIR_PIN , LOW);
      if(currentDirection == 0) //If changing directions compensate for backlash
      {
        backlashComp = BACK_COMP;
        Serial.println("left to right");
      }
      diff = -diff;
      //move motor
      for(i = 0; i < diff*DROP_MICRO_STEP + backlashComp; i++) {
        digitalWrite(DROP_STEP_PIN,HIGH); 
        delayMicroseconds(DROP_DELAY); 
        digitalWrite(DROP_STEP_PIN,LOW); 
        delayMicroseconds(DROP_DELAY); 
      }
      currentDirection = 1;
    }
  //  digitalWrite(EN, HIGH);
    currentLoc = newLoc;
  }

  //Release tokens from the gameboard
  void releaseGameboard(void)
  {
    //Start with release mechanism resting against first spring
    //It takes 1/8 of a rotation to go from resting against 1 pin to reasting against the next
    //Do half of that (so 1/16) of a rotation to release the first column
    //Then 1/8 of a rotation to go from releasing 1 coulmn to the next
    //Release first column
    for(int x = 0; x < STEPS_PER_REV*REL_MICRO_STEP/2/8; x++) {
        digitalWrite(REL_STEP_PIN,HIGH); 
        delayMicroseconds(REL_MS_DELAY); 
        digitalWrite(REL_STEP_PIN,LOW); 
        delayMicroseconds(REL_MS_DELAY); 
    }
    delay(ONE_SEC);
    //Release columns 2-7
    for(int i = 0; i < 7; i++){
      for(int x = 0; x < STEPS_PER_REV*REL_MICRO_STEP/8; x++) {
        digitalWrite(REL_STEP_PIN,HIGH); 
        delayMicroseconds(REL_MS_DELAY); 
        digitalWrite(REL_STEP_PIN,LOW); 
        delayMicroseconds(REL_MS_DELAY); 
      }
      delay(ONE_SEC);
    }
    //Return to start
    for(int x = 0; x < STEPS_PER_REV*REL_MICRO_STEP/2/8; x++) {
      digitalWrite(REL_STEP_PIN,HIGH); 
      delayMicroseconds(REL_MS_DELAY); 
      digitalWrite(REL_STEP_PIN,LOW); 
      delayMicroseconds(REL_MS_DELAY); 
    }
  }
};

#endif
