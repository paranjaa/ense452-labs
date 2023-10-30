/*
ENSE 452
Alok Paranjape
200246370
October 22nd
main.c just loops calls the functions for running the CLI, like prompting the user and reading input
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
	
	startupCheck();
	
	
	//setting the max size of the array as 8 (as shown with the max length on ledquery
	//, didn't need 32
	uint16_t testSize = 8;
	uint8_t testCharArray[testSize];
	

	//clear the screen first
	uint8_t clear_ANSI[] = "\x1b[2J";
	CLI_Transmit(clear_ANSI, (sizeof(clear_ANSI) / sizeof(uint8_t)));
	
	//position the cursor so it's at the start of the screen
	uint8_t top_ANSI[] = "\x1b[0;0H";
	CLI_Transmit(top_ANSI, (sizeof(top_ANSI) / sizeof(uint8_t)));
	
	uint8_t title_msg[] = "ENSE452 Lab 4";
	CLI_Transmit(title_msg, (sizeof(title_msg) / sizeof(uint8_t)));
	uint8_t status_msg[] = "\r\nLED Status: OFF";
	CLI_Transmit(status_msg, (sizeof(status_msg) / sizeof(uint8_t)));
	
	//make a scrollable window with the top lines down? sort of like the example
	uint8_t mid_ANSI[] = "\x1b[3;r";
	CLI_Transmit(mid_ANSI, (sizeof(mid_ANSI) / sizeof(uint8_t)));
	
	

	
	
	
	//used to leave the while loop, when it's zero
	uint8_t quit_zero = 1;
	
	
	
	
	//print the initial message
	CLI_Prompt();
	
	while(quit_zero == 1)
	{
		
		
		//when it gets a new character (i.e, from the flag)
		if(new_recieved == 1)
		{
			//sendbyte(recieved_char);
			//new_recieved = 0;
			CLI_Receive(testCharArray,testSize);
			
			quit_zero = CLI_Quit(testCharArray, testSize);
			if(quit_zero == 0)
			{
				//if they do, leave the loop
				break;
			}	
			CLI_Input(testCharArray, testSize);
			CLI_Clean(testCharArray, testSize);
			CLI_Prompt();

		
			
		}
		

	}	
	//also just going to turn off the LED so it doesn't stay on while nothing's happening
	//startupCheck();
	GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;
	return 0;
}

			/*
			//CLI_Transmit(testCharArray,testSize);

			//return it to the terminal
			//sendbyte(recieved_char);
			//then set the flag back to 0, so it checks again
			//new_recieved = 0;
			
			CLI_Receive(testCharArray,testSize);
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
			//then call the prompt again so they can enter the next command
			CLI_Prompt();*/