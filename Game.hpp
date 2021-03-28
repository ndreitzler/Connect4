#ifndef GAME_H
#define GAME_H

  /** 
   * A class storing a Connect 4 position.
   * Functions are relative to the human player
   */
class Game{
public:
    static const byte WIDTH = 7; //Number of columns in the game board
    static const byte HEIGHT = 6;//Number of rows in the game board

  private:
  //For grid and mask, grid[0] is the first column, grid[1] is the second, etc
  //The first bit of each byte represents the first row, etc
  byte grid[WIDTH]; //If the corresponding mask bit is set and the grid bit is 1, the player has a piece at that position 
  byte mask[WIDTH]; //Each bit marks if a piece is in that position
  byte numMoves;

  public:
  Game(void)
  {
    resetGame();
  }
  Game(const Game *G)
  {
    int i;
    for(i = 0; i < WIDTH; ++i)
    {
      grid[i] = G->getGridCol(i);
      mask[i] = G->getMaskCol(i);
    }
    numMoves = G->getNumMoves();
  }

  byte getGridCol(int col) const
  {
    return grid[col];
  }

  byte getMaskCol(int col) const 
  {
    return mask[col];
  }

  byte getNumMoves(void) const 
  {
    return numMoves;
  }

  bool canPlay(byte col) const
  {
    return !(mask[col] & (1 << (HEIGHT - 1)));
  }
  
  void makeHumanMove(int col)
  {
    grid[col] ^= mask[col];
    mask[col] = (mask[col] << 1) + 1;
    grid[col] ^= mask[col];
    ++numMoves;
  }
  
  void makeAIMove(int col)
  {
    mask[col] = (mask[col] << 1) + 1;//Shift mask over by 1 to signify new AI token
    ++numMoves;
    //Serial.println(mask[col], HEX);
  }

  void makeSolverMove(int col)
  {
    int i;
    for(i = 0; i < Game::WIDTH; ++i)
    {
      grid[col] ^= mask[col];
      if(i == col)
        mask[col] = (mask[col] << 1) + 1;
    }
    ++numMoves;
  }
  
  /*Check for 4 in a row*/
  bool checkWin(bool humanPlayer) //humanPlayer = true if human player, = false if AI
  {
    int r, c;
    int rowStart, colStart;
    int count;
    byte tGrid;
    byte m;
  
    // horizontal
    for(r = 0; r < HEIGHT; ++r)
    {
      count = 0;
      for(c = 0; c < WIDTH; ++c)
      {
        if((mask[c] & (1 << r)) && (((grid[c] & (1 << r)) >> r) == humanPlayer))
        {
          ++count;
        }

        else
          count = 0;
  
        if(count == 4)
        {
          //Serial.println("WIN");
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
        if((mask[c] & (1 << r)) && (((grid[c] & (1 << r)) >> r) == humanPlayer))
          ++count;
        else
          count = 0;  
  
        if(count == 4)
        {
          //Serial.println("WIN");
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
        if((mask[c] & (1 << r)) && (((grid[c] & (1 << r)) >> r) == humanPlayer))
          ++count;
        else
          count = 0;  
      
        if(count == 4)
        {
          //Serial.println("WIN");
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
        
        if((mask[c] & (1 << r)) && (((grid[c] & (1 << r)) >> r) == humanPlayer))
          ++count;
        else
          count = 0;  
  
        if(count == 4)
        {
          //Serial.println("WIN");
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
        if((mask[c] & (1 << r)) && (((grid[c] & (1 << r)) >> r) == humanPlayer))
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
          //Serial.println("WIN");
          return 1;
        }
      }
    }

    //vertical
    for(c = 0; c < WIDTH; ++c)
    {
      //Only if checking for AI victory, makes it so grid bits are 1 for the AI
      if(!humanPlayer)
        tGrid =  grid[c] ^ mask[c];
      else
        tGrid = grid[c];

      m = tGrid & (tGrid >> 1);
      if(m & (m >> 2))
      {
        //Serial.println("WIN");
        return 1;
      }
    }
    return 0;
  }

  bool isWinningColumn(int col, bool humanPlayer) const
  {
    Game G(this);

    //Serial.println("Before can play");
    //G.printGame();
    if(G.canPlay(col))
    {
      if(humanPlayer)
        G.makeHumanMove(col);
      else
        G.makeAIMove(col);
      if(G.checkWin(humanPlayer))
      {
        // Serial.print("test ");
        // Serial.print(col);
        // Serial.print("\n");
        return 1;
      }   
    }
    return 0;
  }

  int findWinningColumn(bool humanPlayer)
  {
    int i;

    for(i = 0; i < WIDTH; ++i)
    {
      if(canPlay(i) && isWinningColumn(i, humanPlayer))
        return i;
    }
    return -1;
  }

 
  /*Set every value in both grid and mask to 0*/
  void resetGame(void)
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
  void printGame(void)
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

  void fullReset()
  {
    resetGame();
    printGame();
  }
};//end class

#endif