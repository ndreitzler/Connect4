/* Nick Dreitzler
 * Purpose: contains pin declarations and other constants
 */

#ifndef CONNECT4_H
#define CONNECT4_H

//Game board dimensions
#define HEIGHT 6
#define WIDTH 7

//used to set a 1 sec delay, delay(ONE_SEC)
#define ONE_SEC 1000

//Motor pins
//Motor connected to crank shaft release
#define CS_DIR_PIN 23  
#define CS_STEP_PIN 22 
#define CS_MICRO_STEP 16 
#define CS_US_DELAY 500

//Motor connected to the dropper
#define DROP_DIR_PIN 11  
#define DROP_STEP_PIN 12 
#define DROP_MICRO_STEP 8
#define DROP_US_DELAY 60

//Motor connected to sorter
#define SORTER_DIR_PIN 24 
#define SORTER_STEP_PIN 25
#define SORTER_MICRO_STEP 8
#define SORTER_US_DELAY 3000

//Pin for the buzzer
#define BUZZER 53





#endif
