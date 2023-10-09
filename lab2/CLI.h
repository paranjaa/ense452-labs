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
@param *pData, points to an array for where to put the recieved valyes
@param Size, the size of the array, so it doesn't go out of bounds
@pre must have already called serial_open()
*/
void CLI_Receive(uint8_t *pData, uint16_t Size);

//added for phase 3
void CLI_Prompt(void);
void CLI_Help(void);
void CLI_Query(void);
void CLI_Input(uint8_t *pData, uint16_t Size);
void CLI_LEDON(void);
void CLI_LEDOFF(void);
uint8_t CLI_Quit(uint8_t *pData, uint16_t Size);
void CLI_Clean(uint8_t *pData, uint16_t Size);
