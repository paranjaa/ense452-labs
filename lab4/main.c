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
	
	//startupCheck();
	
	
	//setting the max size of the array as 8 (as shown with the max length on ledquery
	//, didn't need 32
	uint16_t testSize = 8;
	uint8_t testCharArray[testSize];
	

	//esc = \x1b
	//ESC[<t>;<b>r
		
	uint8_t scroll_ascii[] = "\x1b[5;25r";
	CLI_Transmit(scroll_ascii, (sizeof(scroll_ascii) / sizeof(uint8_t)));
	
	
	uint8_t clear_ascii[] = "	\x1b[2J";
	CLI_Transmit(clear_ascii, (sizeof(clear_ascii) / sizeof(uint8_t)));
	
	uint8_t top_cursor[] = "	\x1b[2J";
	CLI_Transmit(top_cursor, (sizeof(top_cursor) / sizeof(uint8_t)));

	
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