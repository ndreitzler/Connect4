#include <Keypad.h>
#include "Game.hpp"
#include "Solver.hpp"

#define ONE_SEC 1000
#define buzzer 53

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
  
  if (keyPress >= '1' && keyPress <= '7') //Vaild move
  {
    playGame(MasterGame, keyPress - '1');
  }
  if (keyPress == 'D') //Reset game
  {
    MasterGame.fullReset();
  }
}

void playGame(Game &MasterGame, byte keyPress)
{
  Solver solver;
  int AImove;

  if(MasterGame.canPlay(keyPress)) // a vaild move was made
  {
    MasterGame.makeHumanMove(keyPress);
    //MasterGame.printGame();
    if(MasterGame.checkWin(true))
    {
      Serial.println("Human wins");
    }
    AImove = solver.decideAIMove(MasterGame);
    MasterGame.makeAIMove(AImove);
    MasterGame.printGame();
    if(MasterGame.checkWin(false))
    {
     Serial.println("Computer wins");
    }
  } else {  //A vaild move was not made, column was full
    Serial.print("Column #");
    Serial.print(keyPress);
    Serial.print(" is full\n");
    beep();
  }
}

void beep(void)
{
    tone(buzzer, 1000);
    delay(10);
    noTone(buzzer);
}