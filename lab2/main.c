/*
ENSE 452
Alok Paranjape
200246370
October 8th
main.c calls the setup function and then all the other
ones for input in a sequence. 
Also has a function for toggling the LED that was good to check if
code was actually running
*/


#include "stm32f10x.h"
#include "usart.h"
#include "CLI.h"



int main() {
	
	//calling the setup function
	serial_open();
	
	//startupCheck();
	
	//setting the max size of the array as 32, totally enough room
	uint16_t testSize = 32;
	uint8_t testCharArray[testSize];
	
	//used to leave the while loop, when it's zero
	uint8_t quit_zero = 1;
	
	while(1)
	{
		//send out the initial prompt to enter a command
		CLI_Prompt();
		//record the user's input (and also echo it back to them so they can see)
		CLI_Receive(testCharArray,testSize);
		//check if they put in "quit"
		quit_zero = CLI_Quit(testCharArray, testSize);
		if(quit_zero == 0)
		{
			//if they do, leave the loop
			break;
		}	
		//after that, read the input the user put in and call the right function for it
		CLI_Input(testCharArray, testSize);
		//then white out the spaces so it's ready for the next run
		CLI_Clean(testCharArray, testSize);
	
	}	
	//also just going to turn off the LED so it doesn't stay on while nothing's happening
	GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;
	return 0;
}