#define WIDTH 7   //Width of the gameboard
#define HEIGHT 6  //Hight of the gameboard

//For grid and mask, grid[0] is the first column, grid[1] is the second, etc
//The first bit of each byte represents the first row, etc
byte grid[WIDTH]; //If the corresponding mask bit is set and the grid bit is 1, the player has a piece at that position 
byte mask[WIDTH]; //Each bit marks if a piece is in that position

class Game{
private:
  byte grid[WIDTH];
  byte mask[WIDTH];

public:
  Game()
  {
    resetGrid();
  }
  Game(byte *g, byte *m)
  {
    for(i = 0; i < WIDTH; ++i)
    {
      grid[i] = g[i];
      mask[i] = m[i];
    }
  }

  bool canPlay(byte col)
  {
    return !(mask[col] & (1 << (HEIGHT - 1)));
  }
  
}

bool processUserMove(byte keyPress)
{
  if(canPlay(keyPress)) //Verify that column is not full, (already has 6 tokens)
  { 
    makeHumanMove(grid, mask, keyPress);
    printGrid();
    checkWin(grid);
    return 1; //A vaild move was made
  }
  Serial.print("Column #");
  Serial.print(keyPress);
  Serial.print(" is full\n");
  tone(buzzer, 1000);
  delay(10);
  noTone(buzzer);
  return 0; //A vaild move was not made
}



void makeHumanMove(byte *tGrid, byte *tMask, int keyPress)
{
  tGrid[keyPress] ^= tMask[keyPress];
  tMask[keyPress] = (tMask[keyPress] << 1) + 1;
  tGrid[keyPress] ^= tMask[keyPress];
}

/*Check for 4 in a row*/
bool checkWin(byte *tGrid)
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
      if(tGrid[c] & (1 << r))
        ++count;
      else
        count = 0;

      if(count == 4)
      {
        Serial.println("WIN");
        return 1;
      } 
    }
  }

  //diagonal 1 /
  // bottom-left to top-right - upper diagonals
  for(rowStart = 0; rowStart < WIDTH - 4; ++rowStart)
  {
    count = 0;
    for(r = rowStart, c = 0; r < WIDTH && c < HEIGHT; ++r, ++c)
    {
      if(tGrid[c] & (1 << r))
        ++count;
      else
        count = 0;  

      if(count == 4)
      {
        Serial.println("WIN");
        return 1;
      }
    }
  }
  // bottom-left to top-right - lower diagonals
  for(colStart = 1; colStart <= WIDTH - 4; ++colStart)
  {
    count = 0;
    for(r = 0, c = colStart; r < HEIGHT && c < WIDTH; ++r, ++c)
    {
      if(tGrid[c] & (1 << r))
        ++count;
      else
        count = 0;  
    
      if(count == 4)
      {
        Serial.println("WIN");
        return 1;
      }
    }
  }

  //diagonal 2 \
  // bottom-right to top-left - upper diagonals
  for(rowStart = 0; rowStart < WIDTH - 4; ++rowStart)
  {
    count = 0;
    for(r = rowStart, c = WIDTH - 1; r < HEIGHT && c >= 0; ++r, --c)
    {
      
      if(tGrid[c] & (1 << r))
        ++count;
      else
        count = 0;  

      if(count == 4)
      {
        Serial.println("WIN");
        return 1;
      }
    }
  }
  // bottom-right to top-left - lower diagonals
  for(colStart = HEIGHT - 1; colStart >= 3; --colStart)
  {
    count = 0;
    for(r = 0, c = colStart; r < WIDTH && c >= 0; ++r, --c)
    {
      if(tGrid[c] & (1 << r))
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
      {
        Serial.println("WIN");
        return 1;
      }
    }
  }
 
  //vertical
  for(c = 0; c < WIDTH; ++c)
  {
    m = tGrid[c] & (tGrid[c] >> 1);
    if(m & (m >> 2))
    {
      Serial.println("WIN");
      return 1;
    }
  }
  return 0;
}

/*Set every value in both grid and mask to 0*/
void resetGrid(void)
{
  int i;
  for(i = 0; i < WIDTH; ++i)
  {
    grid[i] = 0;
    mask[i] = 0;
  }
}

/*Print grid to serial out
  X for player pieces
  O for AI pieces*/
void printGrid(void)
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

void decideAIMove(void)
{
  byte aiGrid[WIDTH];
  byte tMask[WIDTH];
  byte huGrid[WIDTH];
  byte tempGrid, tempMask;
  int i;

  for(i = 0; i < WIDTH; ++i)
  {
    aiGrid[i] = grid[i] ^ tMask[i];
    huGrid[i] = grid[i];
    tMask[i] = mask[i];
  }

  //Check if human player can win in 1 move, if so play that column
  for(i = 0; i < WIDTH; ++i)
  {
    if(canPlay(i))//Only check columns that are not full
    {
      tempGrid = huGrid[i];
      tempMask = tMask[i];
      makeHumanMove(huGrid, tMask, i);
      if(checkWin(grid))
      {
        makeAIMove(mask, i);
        return;
      }
      huGrid[i] = tempGrid;
      tMask[i] = tempMask;
    }
  }

  for(i = 0; i < WIDTH; ++i)
  {
    if(canPlay(i))
    {
      makeAIMove(mask, i);
      break;
    }
  }

  printGrid();
}

void makeAIMove(byte *tMask, int col)
{
  tMask[col] = (tMask[col] << 1) + 1;
  Serial.println(tMask[col]);
}

//byte *convertToAI(byte *tGrid)
//{
//  int i;
//  for(i = 0; i < WIDTH; ++i)
//    tGrid[i] ^= mask[i];
//  return tGrid;
//}

//void testPrint()
//{
//  makeAIMove(mask, 0);
//  printGrid();
//}
