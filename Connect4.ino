/*Nick Dreitzler
 * Purpose: To run a game of connect 4 using and Arduino mega on custom hardware.
 * 
 */
#include <Keypad.h>

#include "Connect4.h"
#include "Game.hpp"
//#include "MotorControl.hpp"
#include "Solver.hpp"
#include "Sorter.hpp"
#include "CrankShaft.hpp"
#include "Dropper.hpp"
#include "SorterMotor.hpp"
#include "Corkscrew.hpp"

#define ROWS 4 //Rows of keypad
#define COLS 4 //Columns of keypad

byte rowPins[ROWS] = {39, 41, 43, 45}; //row pins for the keypad
byte colPins[COLS] = {47, 49, 51, 53}; //column pins for the keypad

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);//Setup keypad

//CrankShaft crankShaft(CS_STEP_PIN, CS_DIR_PIN, CS_MICRO_STEP, CS_US_DELAY);

void setup(){
  Serial.begin(9600);

  pinMode(CS_STEP_PIN, OUTPUT);
  pinMode(CS_DIR_PIN, OUTPUT);
  pinMode(DROP_STEP_PIN, OUTPUT);
  pinMode(DROP_DIR_PIN, OUTPUT);
  pinMode(SORTER_STEP_PIN, OUTPUT);
  pinMode(SORTER_DIR_PIN, OUTPUT);

  Serial.println("done setup");
}
  
void loop(){
  static bool first = true;
  char customKey = customKeypad.getKey();

//  if(customKey >= '0' && customKey <= '9')
//  Serial.println(customKey);

  if(customKey > ' ' || first)//Do first so that objects are initalized
    processUserInput(customKey);

  first = false;
  delay(50);
}

void processUserInput(char keyPress)
{
  static Game MasterGame;
  //static MotorControl Motors;
  static Sorter sorter;
  static CrankShaft crankShaft(CS_STEP_PIN, CS_DIR_PIN, CS_EN_PIN, CS_MICRO_STEP, CS_US_DELAY);
  static Dropper dropper(DROP_STEP_PIN, DROP_DIR_PIN, DROP_EN_PIN, DROP_MICRO_STEP, DROP_US_DELAY, A0);
  static SorterMotor sorterMotor(SORTER_STEP_PIN, SORTER_DIR_PIN, SORTER_EN_PIN, SORTER_MICRO_STEP, SORTER_US_DELAY, SORTER_BUTTON);
  static Corkscrew purpleCS(CORK_PURPLE_STEP_PIN, CORK_PURPLE_DIR_PIN, CORK_PURPLE_EN_PIN, CORK_PURPLE_MICRO_STEP, CORK_PURPLE_US_DELAY);
  static Corkscrew orangeCS(CORK_ORANGE_STEP_PIN, CORK_ORANGE_DIR_PIN, CORK_ORANGE_EN_PIN, CORK_ORANGE_MICRO_STEP, CORK_ORANGE_US_DELAY);
  static bool isGameOver = false;
  
  //Process Input

  if (keyPress == 'D') //Reset the game, release tokens and sort them, prepare for new game
  {
    resetAll(MasterGame, sorter, crankShaft, dropper, sorterMotor, purpleCS, orangeCS);
    isGameOver = false;
  }
  else if(isGameOver) //The Game is over don't process new game move
  {
    Serial.println("Game is over, hit D to reset");
    beep();
  }
  else if (keyPress >= '1' && keyPress <= '7') //Vaild move, drop token in chosen column
  {
    //Serial.println(keyPress);
    isGameOver = playGame(MasterGame, dropper, keyPress - '1', purpleCS, orangeCS);
  }
}

bool playGame(Game &MasterGame, Dropper &dropper, byte keyPress, Corkscrew &purpleCS, Corkscrew &orangeCS)
{
  Solver solver;
  int AImove;
  bool isGameOver = false;

  if(MasterGame.canPlay(keyPress)) // a vaild move was made
  {
    Serial.println("playing");
    dropToken(dropper, keyPress, orangeCS); //Drop human token
    MasterGame.makeHumanMove(keyPress); //Record human move
    //MasterGame.printGame();
    if(MasterGame.checkWin(true)) //The human has won the game
    {
      isGameOver = true;
      Serial.println("Human wins");
    } else { //The game is not over and the AI must make a move
      AImove = solver.decideAIMove(MasterGame); //Get AI move
      delay(3000);
      dropToken(dropper, AImove, purpleCS); //Drop AI's token
      MasterGame.makeAIMove(AImove); //Record AI move
      if(MasterGame.checkWin(false)) //Check if the AI has won
      {
        isGameOver = true;
        Serial.println("Computer wins");
      }
    }
    MasterGame.printGame();
  } else {  //A vaild move was not made, column was full
    Serial.print("Column #");
    Serial.print(keyPress);
    Serial.print(" is full\n");
    beep();
  }
  return isGameOver;
}

void dropToken(Dropper &dropper, byte keyPress, Corkscrew &color)
{
  dropper.moveDispenser(keyPress);
  color.moveUpOne();
}


//Reset the game. Release and sort pieces, clear game record, and move dropper to center column
void resetAll(Game &MasterGame, Sorter &sorter, CrankShaft &crankShaft, Dropper &dropper, SorterMotor &sorterMotor, Corkscrew &purpleCS, Corkscrew &orangeCS)
{
    purpleCS.dropToBottom();
    orangeCS.dropToBottom();
    sorter.sortPieces(MasterGame, crankShaft, sorterMotor, purpleCS, orangeCS);
    MasterGame.fullReset();
    
}

void beep(void)
{
    tone(BUZZER, 1000);
    delay(10);
    noTone(BUZZER);
}
