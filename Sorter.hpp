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

class Sorter 
{
 public:
  void sortPieces(Game &board, CrankShaft &crankShaft, SorterMotor &sorterMotor)
  {

    // Var initializations
    unsigned char BIT_SEL = 0x01;     // Start at lowest order bit
    int i;                        // Column character select
    unsigned char Current_MASK_Value, Current_POS_Value;
    
    crankShaft.advanceRelease(); //Open first column

    for (i = 0; i < 7; i++) {
      byte grid = board.getGridCol(i);
      byte mask = board.getMaskCol(i);
      //printf("Drop Column #%d\n", i+1);         // Replace with what makes column actually drop
      Current_MASK_Value  = mask & BIT_SEL;  // Is there a piece
      Current_POS_Value = grid & BIT_SEL;  // Direction to move sorter
      while (Current_MASK_Value) {
        sorterMotor.moveSorterFlap(Current_POS_Value);
        BIT_SEL = BIT_SEL << 1;           //Shift the bit being selected left
        Current_MASK_Value  =  mask & BIT_SEL; // Is there a piece
        Current_POS_Value = grid & BIT_SEL;// Direction to move sorter
      } 
      crankShaft.advanceRelease(); //Open next column
      BIT_SEL = 0x01;
    }
    crankShaft.advanceRelease(); // Close last column
    //printf("Done Sorting\n");               // Replace with what happens after sorting
  }

};

#endif
