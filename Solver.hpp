#ifndef SOLVER_H
#define SOLVER_H

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

  int decideAIMove(Game MasterGame)
  {
    //Game AIGame(MasterGame);
    int columnValues[] = {15, 30, 50, 40, 50, 30, 15};
    // byte bestScore = -Game::WIDTH*Game::HEIGHT/2 - 1;//Always worse that worst score
    int bestScore;
    byte bestIndex = 0;
    byte i;

    for(i = 0; i < Game::WIDTH; ++i)
    {
      if(!MasterGame.canPlay(i))
        columnValues[i] -= 1000;//can't play column
      if(columnValues[i] >= 0 && (MasterGame.isWinningColumn(i, false) || MasterGame.isWinningColumn(i, true)))
      {
        //Serial.println("test");
        return i;
      }
    }

    for(i = 0; i < Game::WIDTH; ++i)
    {
      if(columnValues[i] >= 0)
      {
        Game AIGame(MasterGame);
        AIGame.makeAIMove(i);
        if(AIGame.findWinningColumn(true))
        {
          columnValues[i] -= 200; //human can win if AI plays here
        }
        else if(AIGame.findWinningColumn(false))
        {
          columnValues[i] += 200; //AI will have a chance to win
        }
      }
    }

    for(i = 0; i < Game::WIDTH; ++i)
    {
      columnValues[i] -= MasterGame.getGridCol(i)*2;
    }

    Serial.print("{ ");
    for(i = 0; i < Game::WIDTH - 1; ++i)
    {
      Serial.print(columnValues[i]);
      Serial.print(", ");
    }
    Serial.print(columnValues[i]);
    Serial.print("}\n");
      
    bestScore = columnValues[0];
    for(i = 1; i < Game::WIDTH; ++i)
    {
      if(columnValues[i] > bestScore)
      {
        bestScore = columnValues[i];
        bestIndex = i;
      }
    }

    return bestIndex;


    //Serial.println(negamax(MasterGame, -22, 22));

    // for(i = 0; i < Game::WIDTH; ++i)
    // {
    //   Serial.println("test");
    //   if(AIGame.canPlay(columnOrder[i]))
    //     AIGame.makeAIMove(columnOrder[i]);
      
    //   //if(negamax(AIGame, -Game::WIDTH*Game::HEIGHT/2, Game::WIDTH*Game::HEIGHT/2) )

    //   currentScore = -negamax(AIGame, -1, 1);//Negative because negamax finds the best score for the current player,
    //                                               //which is the human becasue the ai just played
    //   if(currentScore > bestScore)
    //   {
    //     bestScore = currentScore;
    //     bestIndex = i;
    //   }
    //   Serial.println(i);
    // }

    // return columnOrder[i];



    //printGame();
  }

  // /**
  //  * Reccursively score connect 4 position using negamax variant of alpha-beta algorithm.
  //  * @param: alpha < beta, a score window within which we are evaluating the position.
  //  *
  //  * @return the exact score, an upper or lower bound score depending of the case:
  //  * - if actual score of position <= alpha then actual score <= return value <= alpha
  //  * - if actual score of position >= beta then beta <= return value <= actual score
  //  * - if alpha <= actual score <= beta then return value = actual score
  //  */
  // int negamax(const Game &G, int alpha, int beta) {
  //   int i;
  //   if(G.getNumMoves() == Game::WIDTH*Game::HEIGHT) // check for draw game
  //     return 0; 

  //   for(i = 0; i < Game::WIDTH; i++) // check if current player can win next move
  //     if(G.canPlay(i) && G.isWinningColumn(i)) 
  //       return (Game::WIDTH*Game::HEIGHT+1 - G.getNumMoves())/2;

  //   int max = (Game::WIDTH*Game::HEIGHT-1 - G.getNumMoves())/2;	// upper bound of our score as we cannot win immediately
  //   if(beta > max) 
  //   {
  //     beta = max;                     // there is no need to keep beta above our max possible score.
  //     if(alpha >= beta) // prune the exploration if the [alpha;beta] window is empty.
  //       return beta;  
  //   }

  //   for(i = 0; i < Game::WIDTH; i++) // compute the score of all possible next move and keep the best one
  //     if(G.canPlay(columnOrder[i])) 
  //     {
  //       Game G2(G);
  //       G2.makeSolverMove(columnOrder[i]);               // It's opponent turn in P2 position after current player plays x column.
  //       int score = -negamax(G2, -beta, -alpha); // explore opponent's score within [-beta;-alpha] windows:
  //                                           // no need to have good precision for score better than beta (opponent's score worse than -beta)
  //                                           // no need to check for score worse than alpha (opponent's score worse better than -alpha)

  //       if(score >= beta) // prune the exploration if we find a possible move better than what we were looking for.
  //         return score;  
  //       if(score > alpha) // reduce the [alpha;beta] window for next exploration, as we only 
  //         alpha = score;    // need to search for a position that is better than the best so far.
                                          
  //     }

  //   return alpha;
  // }

  //byte solve(Game )

};


#endif


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
