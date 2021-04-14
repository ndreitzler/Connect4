/*Nick Dreitzler
 * Purpose: To run a game of connect 4 using and Arduino mega on custom hardware.
 * 
 */
#include <Keypad.h>

#include "Connect4.h"
#include "Game.hpp"
#include "Solver.hpp"
#include "Sorter.hpp"
#include "CrankShaft.hpp"
#include "Dropper.hpp"
#include "SorterMotor.hpp"
#include "Corkscrew.hpp"

byte rowPins[ROWS] = {ROW0, ROW1, ROW2, ROW3}; //row pins for the keypad
byte colPins[COLS] = {COL0, COL1, COL2, COL3}; //column pins for the keypad

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);//Setup keypad

void setup(){
  Serial.begin(9600);

  pinMode(CS_STEP_PIN, OUTPUT);
  pinMode(CS_DIR_PIN, OUTPUT);
  pinMode(CS_EN_PIN, OUTPUT);
  pinMode(DROP_STEP_PIN, OUTPUT);
  pinMode(DROP_DIR_PIN, OUTPUT);
  pinMode(DROP_EN_PIN, OUTPUT);
  pinMode(SORTER_STEP_PIN, OUTPUT);
  pinMode(SORTER_DIR_PIN, OUTPUT);
  pinMode(SORTER_EN_PIN, OUTPUT);
  pinMode(CORK_PURPLE_STEP_PIN, OUTPUT);
  pinMode(CORK_PURPLE_DIR_PIN, OUTPUT);
  pinMode(CORK_PURPLE_EN_PIN, OUTPUT);
  pinMode(CORK_ORANGE_STEP_PIN, OUTPUT);
  pinMode(CORK_ORANGE_DIR_PIN, OUTPUT);
  pinMode(CORK_ORANGE_EN_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(CS_BUTTON_PIN, INPUT); 
  pinMode(DROP_TRIGGER_PIN, INPUT);
  pinMode(SORTER_BUTTON, INPUT);

  Serial.println("done setup");
}
  
void loop(){
  static bool first = true;
  char customKey = customKeypad.getKey();

  if(customKey >= ' ' || first)//Do first so that objects are initalized
  {
    Serial.println(customKey);
    processUserInput(customKey);
  }
    
  first = false;
  delay(50);
}

void processUserInput(char keyPress)
{
  static Game MasterGame;
  static Sorter sorter;
  static CrankShaft crankShaft(CS_STEP_PIN, CS_DIR_PIN, CS_EN_PIN, CS_UDELAY, CS_BUTTON_PIN);
  static Dropper dropper(DROP_STEP_PIN, DROP_DIR_PIN, DROP_EN_PIN, DROP_UDELAY, DROP_TRIGGER_PIN);
  static SorterMotor sorterMotor(SORTER_STEP_PIN, SORTER_DIR_PIN, SORTER_EN_PIN, SORTER_UDELAY, SORTER_BUTTON);
  static Corkscrew purpleCork(CORK_PURPLE_STEP_PIN, CORK_PURPLE_DIR_PIN, CORK_PURPLE_EN_PIN, CORK_PURPLE_UDELAY);
  static Corkscrew orangeCork(CORK_ORANGE_STEP_PIN, CORK_ORANGE_DIR_PIN, CORK_ORANGE_EN_PIN, CORK_ORANGE_UDELAY);
  static bool isGameOver = false;

  //Serial.println(keyPress);
  //purpleCork.moveUpOne();
  //purpleCork.dropToBottom();
//  if(isGameOver)
//    dropper.moveDispenser(keyPress - '1');
//   isGameOver = true;
  //Process Input

   if(isGameOver) //The Game is over don't process new game move
   {
     Serial.println("Game is over, hit D to reset");
     beep();
   } else {
    switch (keyPress)
    {
    case 'D':
      resetAll(MasterGame, sorter, crankShaft, dropper, sorterMotor, purpleCork, orangeCork);
      isGameOver = false;
      break;
    case 'A':
      for(int i = 0; i < 10; ++i)
        purpleCork.moveUpOne();
      break;
    case 'B':
      purpleCork.moveUpOne();
      break;
    case '*'
      for(int i = 0; i < 10; ++i)
        orangeCork.moveUpOne();
      break;
    case '0':
      purpleCork.moveUpOne();
      break;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
      isGameOver = playGame(MasterGame, dropper, keyPress - '1', purpleCork, orangeCork);
      break;
    default:
      beep();
      break;
  }
}

bool playGame(Game &MasterGame, Dropper &dropper, byte keyPress, Corkscrew &purpleCork, Corkscrew &orangeCork)
{
  Solver solver;
  int AImove;
  bool isGameOver = false;

  if(MasterGame.canPlay(keyPress)) // a vaild move was made
  {
    Serial.println("playing");
    dropToken(dropper, keyPress, orangeCork); //Drop human token
    MasterGame.makeHumanMove(keyPress); //Record human move
    //MasterGame.printGame();
    if(MasterGame.checkWin(true)) //The human has won the game
    {
      isGameOver = true;
      Serial.println("Human wins");
    } else { //The game is not over and the AI must make a move
      AImove = solver.decideAIMove(MasterGame); //Get AI move
      delay(3000);
      dropToken(dropper, AImove, purpleCork); //Drop AI's token
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
void resetAll(Game &MasterGame, Sorter &sorter, CrankShaft &crankShaft, Dropper &dropper, SorterMotor &sorterMotor, Corkscrew &purpleCork, Corkscrew &orangeCork)
{
    purpleCork.dropToBottom();
    orangeCork.dropToBottom();
    sorter.sortPieces(MasterGame, crankShaft, sorterMotor, purpleCork, orangeCork);
    MasterGame.fullReset();
    
}

void beep(void)
{
    tone(BUZZER, 1000);
    delay(100);
    noTone(BUZZER);
}
