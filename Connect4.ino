#include <Keypad.h>

#define ONE_SEC 1000

#define ROWS 4 //Rows of keypad
#define COLS 4 //Columns of keypad

#define WIDTH 7   //Width of the gameboard
#define HEIGHT 6  //Hight of the gameboard

//For grid and mask grid[0] is the first column, grid[1] is the second, etc
//The first bit of each byte represents the first row, etc
byte grid[WIDTH]; //If the corresponding mask bit is set and the grid bit is 1, the player has a piece at that position 
byte mask[WIDTH]; //Each bit marks if a piece is in that position

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //row pins for the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //column pins for the keypad

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);//Setup keypad

void setup(){
  Serial.begin(9600);
  int i = 6;
  int start = 0x01;

  resetGrid();
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
  printGrid();
//  checkWin();
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey >= '1' && customKey <= '7')
  {
    processUserMove( customKey);
    printGrid();
    checkWin();
  }
  if (customKey == 'D')
  {
    resetGrid();
    printGrid();
  }

  delay(50);
}

void processUserMove(char keyPress)
{
  mask[keyPress - '1'] = (mask[keyPress - '1'] << 1) + 1;
  grid[keyPress - '1'] = (grid[keyPress - '1'] << 1) + 1;
}

/*Check for 4 in a row*/
void checkWin()
{
  int r, c;
  int rowStart, colStart;
  int count;
  byte m;

  // horizontal
  for(r = 0; r < HEIGHT; ++r)
  {
    count = 0;
    for(c = 0; c < WIDTH; ++c)
    {
      if(grid[c] & (1 << r))
        ++count;
      else
        count = 0;

      if(count == 4)
        Serial.println("WIN");
    }
  }

  //diagonal 1 /
  // bottom-left to top-right - upper diagonals
  for(rowStart = 0; rowStart < WIDTH - 4; ++rowStart)
  {
    count = 0;
    for(r = rowStart, c = 0; r < WIDTH && c < HEIGHT; ++r, ++c)
    {
      if(grid[c] & (1 << r))
        ++count;
      else
        count = 0;  

      if(count == 4)
        Serial.println("WIN");
    }
  }
  // bottom-left to top-right - lower diagonals
  for(colStart = 1; colStart <= WIDTH - 4; ++colStart)
  {
    count = 0;
    for(r = 0, c = colStart; r < HEIGHT && c < WIDTH; ++r, ++c)
    {
      if(grid[c] & (1 << r))
        ++count;
      else
        count = 0;  
    
      if(count == 4)
        Serial.println("WIN");
    }
  }

  //diagonal 2 \
  // bottom-right to top-left - upper diagonals
  for(rowStart = 0; rowStart < WIDTH - 4; ++rowStart)
  {
    count = 0;
    for(r = rowStart, c = WIDTH - 1; r < HEIGHT && c >= 0; ++r, --c)
    {
      
      if(grid[c] & (1 << r))
        ++count;
      else
        count = 0;  

      if(count == 4)
        Serial.println("WIN");
    }
  }
  // bottom-right to top-left - lower diagonals
  for(colStart = HEIGHT - 1; colStart >= 3; --colStart)
  {
    count = 0;
    for(r = 0, c = colStart; r < WIDTH && c >= 0; ++r, --c)
    {
      if(grid[c] & (1 << r))
        ++count;
      else
        count = 0;  

//      Serial.print(c);
//      Serial.print(" ");
//      Serial.print(r);
//      Serial.print(" ");
//      Serial.print(count);
//      Serial.print("\n");
        
      if(count == 4)
        Serial.println("WIN");
    }
  }
 
  //vertical
  for(c = 0; c < WIDTH; ++c)
  {
    m = grid[c] & (grid[c] >> 1);
    if(m & (m >> 2)) 
      Serial.println("WIN");
  }

}

/*Set every value in both grid and mask to 0*/
void resetGrid()
{
  int i;
  for(i = 0; i < WIDTH; ++i)
  {
    grid[i] = 0;
    mask[i] = 0;
  }
}

/*Print grid to serial out
  X for player piece
  O for AI piece*/
void printGrid()
{
    int i, j;

    for(i = HEIGHT - 1; i >= 0; --i)
    {
      for(j = 0; j < WIDTH; ++j)
      {
        if(mask[j] & (1 << i))
        {
          if(grid[j] & (1 << i))
            Serial.print("X ");
          else
            Serial.print("O ");
        }
        else
        {
          Serial.print("- ");
        }
      }
      Serial.print("\n");
    }
    Serial.print("\n");
       
}
