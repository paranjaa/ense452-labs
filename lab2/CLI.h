/*
ENSE 452
Alok Paranjape
200246370
October 8th
CLI.h has the function header comments for phase 2 and 3
Tried to keep it similar the the ones in the assignment
*/

#include "stm32f10x.h"
#include "usart.h"

//added for phase 2
/**
Uses sendByte in a loop a given array of values along the serial port
so they get printed out in the terminal
used for showing the current input, and responses to it
and the interface in the command line interface
@param *pData, points to an array storing values to print out
@param Size, the size of the array, so it doesn't go out of bounds
@pre must have already called serial_open()
*/
void CLI_Transmit(uint8_t *pData, uint16_t Size);

/**
Uses getByte to read in input from the user via the serial port
so that it can get the commands to use
@param *pData, points to an array for where to put the recieved values
@param Size, the size of the array, so it doesn't go out of bounds
@pre must have already called serial_open()
*/
void CLI_Receive(uint8_t *pData, uint16_t Size);

//added for phase 3

/**
Calls CLI_Transmit to print out a message to
prompt the user to input a command
@pre must have already called serial_open()
*/
void CLI_Prompt(void);

/**
Calls CLI_Transmit to print out a message to
list out the commands and their functions
@pre must have already called serial_open()
*/
void CLI_Help(void);

/**
Checks the state of the register controlling the
onboard LED, then calls CLI_Transmit to print a
message depending on the state
@pre must have already called serial_open()
@pre the output changes if GPIOA_ODR5 is 1 or not, 
so if either CLI_LEDON or CLI_LEDOFF are called beforehand
*/
void CLI_Query(void);

/**
Checks the pointer and the first few away from it to see
what command it is and what function to run. Uses a series AND statements for each one.
Isn't very typo friendly, any early spaces or random caps still cause an error
@param *pData, points to an array for where the user's input has been stored
@param Size, the size of the array, so it doesn't go out of bounds 
(it isn't really used, but kept it there cause it'll probably 
be needed later on for actual checking)
@pre must have already called serial_open()
*/
void CLI_Input(uint8_t *pData, uint16_t Size);

/**
 CLI_Transmit to print a message out and turns on the LED
@pre must have already called serial_open()
*/
void CLI_LEDON(void);

/**
 CLI_Transmit to print a message out and turns off the LED
@pre must have already called serial_open()
*/
void CLI_LEDOFF(void);
/**
Like CLI_Query, checks the stored input, but specifically for "quit"
so it can exit the main loop. Returns a 0 if it reads quit
or 1 otherwise
@param *pData, points to an array for where the user's input has been stored
@param Size, the size of that array, still really used here.
@pre must have already called serial_open()
*/
uint8_t CLI_Quit(uint8_t *pData, uint16_t Size);

/**
Loops through the array via the given pointer and replaces the characters with ' '
So it can get reused later on and there aren't leftovers from the previous loop
@param *pData, points to an array for where the user's input has been stored
@param Size, the size of that array, this time it actually does go right to the end
@pre must have already called serial_open()
*/
void CLI_Clean(uint8_t *pData, uint16_t Size);
