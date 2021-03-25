Game MasterGame;

void playGame(byte keyPress)
{
  if(MasterGame.makeHumanMove(keyPress)) // a vaild move was made
  {
    MasterGame.printGame();
    MasterGame.checkWin();
    decideAIMove();
    
    MasterGame.checkWin();
  } else {  //A vaild move was not made, column was full
    Serial.print("Column #");
    Serial.print(keyPress);
    Serial.print(" is full\n");
    tone(buzzer, 1000);
    delay(10);
    noTone(buzzer);
  }
}

void decideAIMove(void)
{
  Game AIGame(MasterGame);
  int move;
  int i;

  move = AIGame.findWinningColumn();

  if(move > -1)
  {
    //Serial.println(move);
    MasterGame.makeAIMove(move);
    MasterGame.printGame();
  }


  // //Check if human player can win in 1 move, if so play that column
  // for(i = 0; i < WIDTH; ++i)
  // {
  //   if(canPlay(i))//Only check columns that are not full
  //   {
  //     tempGrid = huGrid[i];
  //     tempMask = mask[i];
  //     makeHumanMove(huGrid, mask, i);
  //     if(checkWin(grid))
  //     {
  //       makeAIMove(mask, i);
  //       return;
  //     }
  //     huGrid[i] = tempGrid;
  //     mask[i] = tempMask;
  //   }
  // }

  // for(i = 0; i < WIDTH; ++i)
  // {
  //   if(canPlay(i))
  //   {
  //     makeAIMove(mask, i);
  //     break;
  //   }
  // }

  //printGame();
}


void fullReset(void)
{
  MasterGame.resetGame();
  MasterGame.printGame();
}

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
//  printGame();
//}
