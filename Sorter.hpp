/* Stephen Frost and Nick Dreitzler
 * Purpose: To sort the pieces that are currently in the gameboard and return them to the feed columns
 *
 */

#ifndef SORTER_H
#define SORTER_H

#include "Connect4.h"
#include "Game.hpp"
#include "CrankShaft.hpp"
#include "SorterMotor.hpp"
#include "Corkscrew.hpp"

class Sorter 
{
 public:
  void sortPieces(Game &board, CrankShaft &crankShaft, SorterMotor &sorterMotor, Corkscrew &purpleCS, Corkscrew &orangeCS)
  {

    // Var initializations
    unsigned char BIT_SEL = 0x01;     // Start at lowest order bit
    int i;                        // Column character select
    unsigned char Current_MASK_Value, Current_POS_Value;
    
    crankShaft.advanceRelease(); //Open first column
    delay(ONE_SEC);

    for (i = 6; i >= 0; --i) {
      byte grid = board.getGridCol(i);
      byte mask = board.getMaskCol(i);
      Current_MASK_Value  = mask & BIT_SEL;  // Is there a piece
      Current_POS_Value = grid & BIT_SEL;  // Direction to move sorter
      while (Current_MASK_Value) {
        sorterMotor.moveSorterFlap((bool)Current_POS_Value);
        if(Current_POS_Value)
        {
          orangeCS.moveUpOne();
        } else {
          purpleCS.moveUpOne();
        }
        BIT_SEL = BIT_SEL << 1;           //Shift the bit being selected left
        Current_MASK_Value  =  mask & BIT_SEL; // Is there a piece
        Current_POS_Value = grid & BIT_SEL;// Direction to move sorter
      } 
      crankShaft.advanceRelease(); //Open next column
      delay(ONE_SEC);
      BIT_SEL = 0x01;
    }
    crankShaft.advanceRelease(); // Close last column
  }

};

#endif
