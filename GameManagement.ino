#define WIDTH 7   //Width of the gameboard
#define HEIGHT 6  //Hight of the gameboard

class Game{
  private:
  //For grid and mask, grid[0] is the first column, grid[1] is the second, etc
  //The first bit of each byte represents the first row, etc
  byte grid[WIDTH]; //If the corresponding mask bit is set and the grid bit is 1, the player has a piece at that position 
  byte mask[WIDTH]; //Each bit marks if a piece is in that position

  public:
  Game(void)
  {
    resetGrid();
  }
  Game(const Game &G)
  {
    int i;
    for(i = 0; i < WIDTH; ++i)
    {
      grid[i] = G.grid[i];
      mask[i] = G.mask[i];
    }
  }

  bool canPlay(byte col)
  {
    return !(mask[col] & (1 << (HEIGHT - 1)));
  }
  
  bool makeHumanMove(int col)
  {
    if(canPlay(col))
    {
      grid[col] ^= mask[col];
      mask[col] = (mask[col] << 1) + 1;
      grid[col] ^= mask[col];
      return 1;
    } 
    return  0;
  }
  
  /*Check for 4 in a row*/
  bool checkWin(void)
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
        if(grid[c] & (1 << r))
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
        if(grid[c] & (1 << r))
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
        
        if(grid[c] & (1 << r))
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
        {
          Serial.println("WIN");
          return 1;
        }
      }
    }
   
    //vertical
    for(c = 0; c < WIDTH; ++c)
    {
      m = grid[c] & (grid[c] >> 1);
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
};//end class

Game MasterGame;


bool processUserMove(byte keyPress)
{
  if(MasterGame.makeHumanMove(keyPress))
  {
    MasterGame.printGrid();
    MasterGame.checkWin();
    return 1;
  }
 
  Serial.print("Column #");
  Serial.print(keyPress);
  Serial.print(" is full\n");
  tone(buzzer, 1000);
  delay(10);
  noTone(buzzer);
  return 0; //A vaild move was not made
}

// void decideAIMove(void)
// {
//   byte aiGrid[WIDTH];
//   byte mask[WIDTH];
//   byte huGrid[WIDTH];
//   byte tempGrid, tempMask;
//   int i;

//   for(i = 0; i < WIDTH; ++i)
//   {
//     aiGrid[i] = grid[i] ^ mask[i];
//     huGrid[i] = grid[i];
//     mask[i] = mask[i];
//   }

//   //Check if human player can win in 1 move, if so play that column
//   for(i = 0; i < WIDTH; ++i)
//   {
//     if(canPlay(i))//Only check columns that are not full
//     {
//       tempGrid = huGrid[i];
//       tempMask = mask[i];
//       makeHumanMove(huGrid, mask, i);
//       if(checkWin(grid))
//       {
//         makeAIMove(mask, i);
//         return;
//       }
//       huGrid[i] = tempGrid;
//       mask[i] = tempMask;
//     }
//   }

//   for(i = 0; i < WIDTH; ++i)
//   {
//     if(canPlay(i))
//     {
//       makeAIMove(mask, i);
//       break;
//     }
//   }

//   //printGrid();
// }

// void makeAIMove(byte *mask, int col)
// {
//   mask[col] = (mask[col] << 1) + 1;
//   Serial.println(mask[col]);
// }

//byte *convertToAI(byte *grid)
//{
//  int i;
//  for(i = 0; i < WIDTH; ++i)
//    grid[i] ^= mask[i];
//  return grid;
//}

//void testPrint()
//{
//  makeAIMove(mask, 0);
//  printGrid();
//}
