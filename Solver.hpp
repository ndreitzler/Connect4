/* Nick Dreitzler
 * Purpose: To determine a move for a connect 4 game
 */
#ifndef SOLVER_H
#define SOLVER_H

#include "Connect4.h"
#include "Game.hpp"

class Solver{
private:

public:
  Solver(void)
  {
    //int i;

    // for(i = 0; i < Game::WIDTH; ++i)
    //   columnValues[i] = Game::WIDTH/2 + (1-2*(i%2))*(i+1)/2;
  }

  /* Returns a column that is a good move for the AI to make
     Each column starts with an inital value, varius aspects of the current game state can modify this score.
     These include, the human can win if the AI plays here, the AI can win by playing this column, etc
   */
  int decideAIMove(Game MasterGame)
  {
    //Game AIGame(MasterGame);
    int columnValues[] = {15, 30, 50, 55, 50, 30, 15};
    // byte bestScore = -Game::WIDTH*Game::HEIGHT/2 - 1;//Always worse that worst score
    int bestScore;
    byte bestIndex = 0;
    byte i;

    randomSeed(analogRead(A0));

    for(i = 0; i < WIDTH; ++i)
    {
      columnValues[i] += random(-5, 5);
    }

    //Check for columns where the AI can win in 1 move. If one exists return that column number
    for(i = 0; i < Game::width; ++i)
    {
      if(!MasterGame.canPlay(i))
      {
        columnValues[i] -= 2000;//can't play column
      }
      else if(MasterGame.isWinningColumn(i, true))
      {
        Serial.print("I can win here: ");
        Serial.print(i);
        Serial.print("\n");
        return i;
      }
      else if(MasterGame.isWinningColumn(i, false))
      {
        columnValues[i] += 1000;
      }
      else
      {
        Game AIGame(MasterGame);
        AIGame.makeAIMove(i);
        if(AIGame.findWinningColumn(true))
        {
          columnValues[i] -= 1000; //human can win if AI plays here
        }
        else if(AIGame.findWinningColumn(false))
        {
          columnValues[i] += 200; //AI will have a chance to win
        }
        columnValues[i] -= MasterGame.getGridCol(i)*2;
      }
    }

    //Print scores
    Serial.print("{ ");
    for(i = 0; i < Game::width - 1; ++i)
    {
      Serial.print(columnValues[i]);
      Serial.print(", ");
    }
    Serial.print(columnValues[i]);
    Serial.print("}\n");
    
    //Find best score
    bestScore = columnValues[0];
    for(i = 1; i < Game::width; ++i)
    {
      if(columnValues[i] > bestScore)
      {
        bestScore = columnValues[i];
        bestIndex = i;
      }
    }

    return bestIndex;
  }


};


#endif
