#include <Keypad.h>
#include "Game.hpp"
#include "Solver.hpp"
#include "MotorControl.hpp"
#include "Sorter.hpp"

#define ONE_SEC 1000
#define buzzer 53

// Connections to A4988
#define DROP_DIR_PIN 11  // Direction
#define DROP_STEP_PIN 12 // Step
#define REL_DIR_PIN 23   // Direction
#define REL_STEP_PIN 22  // Step

#define ROWS 4 //Rows of keypad
#define COLS 4 //Columns of keypad

byte rowPins[ROWS] = {9, 8, 7, 6}; //row pins for the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //column pins for the keypad

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);//Setup keypad

void setup(){
  Serial.begin(9600);

  pinMode(buzzer, OUTPUT);

}
  
void loop(){
  char customKey = customKeypad.getKey();

  processUserInput(customKey);

  delay(50);
}

void processUserInput(char keyPress)
{
  static Game MasterGame;
  static MotorControl Motors;
  static Sorter sorter;
  bool isGameOver = false;
  
  if(isGameOver)
  {
    Serial.println("Game is over, hit D to reset");
  }
  else if (keyPress >= '1' && keyPress <= '7') //Vaild move
  {
    isGameOver = playGame(MasterGame, keyPress - '1', Motors);
  }
  else if (keyPress == 'D') //Reset game
  {
    resetGame(MasterGame, Motors, sorter);
  }
}

bool playGame(Game &MasterGame, byte keyPress, MotorControl &Motors)
{
  Solver solver;
  int AImove;
  bool isGameOver = false;

  if(MasterGame.canPlay(keyPress)) // a vaild move was made
  {
    Motors.dropToken(keyPress);
    MasterGame.makeHumanMove(keyPress);
    //MasterGame.printGame();
    if(MasterGame.checkWin(true))
    {
      isGameOver = true;
      Serial.println("Human wins");
    } else {
      AImove = solver.decideAIMove(MasterGame);
      Motors.dropToken(AImove);
      MasterGame.makeAIMove(AImove);
      if(MasterGame.checkWin(false))
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

void resetAll(Game &MasterGame, MotorControl &Motors, Sorter &sorter)
{
    sorter.sortPieces(MasterGame, Motors);
    MasterGame.fullReset();
}

void beep(void)
{
    tone(buzzer, 1000);
    delay(10);
    noTone(buzzer);
}
