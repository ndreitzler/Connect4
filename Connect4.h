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

#define COUNTER_WISE false //counter-clockwise
#define CLOCK_WISE true   //Clockwise

/*Pin Connections*/

//Motor pins
//Motor connected to crank shaft release
#define CS_DIR_PIN 8  
#define CS_STEP_PIN 9 
#define CS_EN_PIN 10
#define CS_MICRO_STEP 16 
#define CS_US_DELAY 1000

//Motor connected to the dropper
#define DROP_DIR_PIN 5  
#define DROP_STEP_PIN 6 
#define DROP_EN_PIN 7
#define DROP_MICRO_STEP 8
#define DROP_US_DELAY 100
#define TRIGGER_PIN a0

//Motor connected to sorter
#define SORTER_DIR_PIN 2 
#define SORTER_STEP_PIN 3
#define SORTER_EN_PIN 4
#define SORTER_MICRO_STEP 16
#define SORTER_US_DELAY 10000
#define SORTER_BUTTON 22

//Motor connected to purple corkscrew
#define CORK_PURPLE_DIR_PIN 22 
#define CORK_PURPLE_STEP_PIN 23
#define CORK_PURPLE_EN_PIN 24
#define CORK_PURPLE_MICRO_STEP 16
#define CORK_PURPLE_US_DELAY 1000

//Motor connected to orange corkscrew
#define CORK_ORANGE_DIR_PIN  25
#define CORK_ORANGE_STEP_PIN 26
#define CORK_ORANGE_EN_PIN 27
#define CORK_ORANGE_MICRO_STEP 16
#define CORK_ORANGE_US_DELAY 1000

//Pin for the buzzer
#define BUZZER 52



#endif
