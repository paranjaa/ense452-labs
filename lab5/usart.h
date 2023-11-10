/*
ENSE 452 Lab 5
Alok Paranjape
200246370
November 9th
No changes from lab 4
*/

#include <stdint.h>
#include "stm32f10x.h"


/** toggles the led for a second w/ delay so it easier to see if it's actually running 
@pre must have already called serial open, specifically needs
the GPIOA clock on and GPIOA set for being an output
*/
void startupCheck(void);
	

/** adds in a ~1 second delay, only used for the startup check */
void delay2(void);
/** Configure and enable the device. */
void serial_open(void);

/**
Undo whatever serial_open() did, setting the peripheral back to
its reset configuration. */
void serial_close(void);

/**
Send an 8-bit byte to the serial port, using the configured
bit-rate, # of bits, etc.
The notes say something about returning a 0 on success/ non zero on failure
but wasn't sure how to do that, it just returns 0 or gets stuck I guess
@param b the 8-bit quantity to be sent.
@pre must have already called serial_open()
*/
int sendbyte(uint8_t b);

/**
Gets an 8-bit character from the serial port, and returns it.
@pre must have already called serial_open()
*/
uint8_t getbyte(void);




