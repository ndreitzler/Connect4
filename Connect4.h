/* Nick Dreitzler
 * Purpose: contains pin declarations and other constants
 * 
 */

#ifndef CONNECT4_H
#define CONNECT4_H

//Game board dimensions
#define HEIGHT 6
#define WIDTH 7

#define ROWS 4 //Rows of keypad
#define COLS 4 //Columns of keypad

//used to set a 1 sec delay, delay(ONE_SEC)
#define ONE_SEC 1000

#define COUNTER_WISE false //counter-clockwise
#define CLOCK_WISE true   //Clockwise

/*Pin Connections*/

//Motor pins
//Motor connected to crank shaft release
#define CS_DIR_PIN 10  
#define CS_STEP_PIN 11 
#define CS_EN_PIN 12
#define CS_MICRO_STEP 16 
#define CS_UDELAY 1000
#define CS_BUTTON_PIN 33

//Motor connected to the dropper
#define DROP_DIR_PIN 7  
#define DROP_STEP_PIN 8 
#define DROP_EN_PIN 9
#define DROP_MICRO_STEP 8
#define DROP_UDELAY 150
#define DROP_TRIGGER_PIN 35

//Motor connected to sorter
#define SORTER_DIR_PIN 4 
#define SORTER_STEP_PIN 5
#define SORTER_EN_PIN 6
#define SORTER_MICRO_STEP 16
#define SORTER_UDELAY 1000
#define SORTER_BUTTON 31

//Motor connected to purple corkscrew
#define CORK_PURPLE_DIR_PIN 3 
#define CORK_PURPLE_STEP_PIN 24
#define CORK_PURPLE_EN_PIN 22
#define CORK_PURPLE_MICRO_STEP 16
#define CORK_PURPLE_UDELAY 200

//Motor connected to orange corkscrew
#define CORK_ORANGE_DIR_PIN  2
#define CORK_ORANGE_STEP_PIN 25
#define CORK_ORANGE_EN_PIN 23
#define CORK_ORANGE_MICRO_STEP 16
#define CORK_ORANGE_UDELAY 200

//Pin for the buzzer
#define BUZZER 48

//Pins for the keypad
#define ROW0 39
#define ROW1 41
#define ROW2 43
#define ROW3 45
#define COL0 47
#define COL1 49
#define COL2 51
#define COL3 53


#endif
