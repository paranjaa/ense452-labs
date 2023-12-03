/*
ENSE 452 Lab Project
Alok Paranjape
200246370
December 3rd

Made a new version of serial open for this, moved it out of main
Also put the intro message command here as well,
since it runs before the tasks start

*/
#ifndef UTIL_H
#define UTIL_H

#include "stm32f10x.h"

/**
Gets the board ready to run the rest of the functions and tasks
starts the relevant clocks, sets the inputs, and prepares for the interrupts
*/

void serial_init(void);

/**
Prints out the initial UI the user sees on startup, and sets up the scrolling window 
for command input
@pre need to already have run serial_open() first
*/

void intro_message(void);

#endif