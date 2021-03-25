#include <Keypad.h>

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
   
  int i = 6;
  int start = 0x01;

  //resetGrid();
//  testPrint();
//
//  grid[i--] = start;
//  grid[i--] = start << 1;
//  grid[i--] = start << 2;
//  grid[i--] = start << 3;
//  mask[1] = 0xff;
//  mask[2] = 0xff;
//  mask[3] = 0xff;
//  mask[4] = 0xff;
//  mask[5] = 0xff;
//  mask[6] = 0xff;
//  mask[7] = 0xff;
//  mask[0] = 0xff;
   // printGrid();
//  checkWin();
}
  
void loop(){
  char customKey = customKeypad.getKey();

  processUserInput(customKey);

  delay(50);
}

void processUserInput(char keyPress)
{
  bool vaildMove;
  
  if (keyPress >= '1' && keyPress <= '7') //Vaild move
  {
    playGame( keyPress - '1');
  }
  if (keyPress == 'D') //Reset game
  {
    fullReset();
  }
}
