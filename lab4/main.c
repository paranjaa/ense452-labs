/*
ENSE 452 Lab 4
Alok Paranjape
200246370
October 29th
Added in some ANSI commands at startup to 
clear the screen, make a startup section, then a scrollable window
*/

#include "stm32f10x.h"
#include "usart.h"
#include "CLI.h"


extern uint8_t recieved_char;
extern uint8_t new_recieved;
//uint8_t recieved_char;

int main() {
	
	//calling the setup function
	serial_open();
	
	//for seeing if it's actually running, should pulse the LED for a second
	//startupCheck();
	
	//setting the max size of the array as 10, didn't need 32
	//think the longest is ledquery, which is 8
	uint16_t testSize = 10;
	uint8_t testCharArray[testSize];
	

	//clear the screen first(was kind of unreliable)
	uint8_t clear_ANSI[] = "\x1b[2J";
	CLI_Transmit(clear_ANSI, (sizeof(clear_ANSI) / sizeof(uint8_t)));
	
	//position the cursor so it's at the start of the screen (at the top left)
	uint8_t top_ANSI[] = "\x1b[0;0H";
	CLI_Transmit(top_ANSI, (sizeof(top_ANSI) / sizeof(uint8_t)));
	
	//print out a title message
	uint8_t title_msg[] = "ENSE 452 Lab 4 CLI";
	CLI_Transmit(title_msg, (sizeof(title_msg) / sizeof(uint8_t)));
	
	//also print a status for the LED, which should be off
	uint8_t status_msg[] = "\r\nLED Status: OFF";
	CLI_Transmit(status_msg, (sizeof(status_msg) / sizeof(uint8_t)));
	
	//make a scrollable window with the top at line 3
	//think leaving it blank makes it go to the base of the current view?
	uint8_t mid_ANSI[] = "\x1b[3;r";
	CLI_Transmit(mid_ANSI, (sizeof(mid_ANSI) / sizeof(uint8_t)));
	
	
	//used to leave the while loop, when it's zero
	uint8_t quit_zero = 1;
	
	
	
	//print the initial message for entering a prompt
	CLI_Prompt();
	
	
	//changed this to also exit the loop instead of a while 1 as well
	//	since it was being weird with not breaking the loop (or, weirder, breaking immediately) while in debug
	while(quit_zero == 1)
	{
		
		
		//when it gets a new character (i.e, from the flag)
		if(new_recieved == 1)
		{
			//used to initially see if it gets recieved, now that's all in CLI_Recieve()
			
			//sendbyte(recieved_char);
			//new_recieved = 0;
			CLI_Receive(testCharArray,testSize);
			
			quit_zero = CLI_Quit(testCharArray, testSize);
			if(quit_zero == 0)
			{
				//if they do, leave the loop
				break;
			}	
			//read the input and decide what to do
			CLI_Input(testCharArray, testSize);
			//empty out the character array for next time
			CLI_Clean(testCharArray, testSize);
			
			//print the prompt again
			CLI_Prompt();

		
			
		}
		

	}	
	//also just going to turn off the LED so it doesn't stay on while nothing's happening
	
	GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;
	return 0;
}
