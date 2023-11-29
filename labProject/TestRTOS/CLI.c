/*
ENSE 452 Lab 5
Alok Paranjape
200246370
November 9th
Mostly the same code as lab 4
Added a function for RTOS, couldn't get the rest of it to work
*/

#include "stm32f10x.h"
#include "usart.h"
#include "CLI.h"
#include "FreeRTOS.h"
#include "queue.h"

#include <string.h>

//QueueHandle_t CLI_Queue;
//extern QueueHandle_t xFreq_Queue;



extern uint8_t recieved_char;
extern uint8_t new_recieved;


void CLI_Transmit(uint8_t *pData, uint16_t Size)
{

	for(uint16_t i = 0; i < Size; i++)
	{
		sendbyte( *(pData + i) );
	}

}


//making a new function for dealing with the LED


//Receive data via USART
/*
void CLI_RTOS(uint8_t *input, uint16_t size) 
{
	int frequency;
	if(input[0] == 'a')
	{
		//if it gets a, then send a capital A to confirm it got here
		sendbyte('A');
		//then set the frequency to 250 and put it in the queue
		frequency = 250;
		xQueueSendToFront(xFreq_Queue, &frequency, NULL);
		return;
	}
	
	
	else if(input[0] == 'b')
	{
		//if it's B, print B to make sure it got here, then set the frequency to 1000
		sendbyte('B');
		//it doesn't actually set it to 1000, it's something else
		frequency = 1000;
		xQueueSendToFront(xFreq_Queue, &frequency, NULL);
		return;
	}
	else if(input[0] == 'c')
	{
		//same deal with c
		sendbyte('C');
		frequency = 3000;
		xQueueSendToFront(xFreq_Queue, &frequency, NULL);
		return;
	}
	else
	{
		//if it's anything other than a b or c, then 
		sendbyte('!');
		//set the frequency to 100
		frequency = 100;
		//and put it on the queue, for some reason this one does work
		//not sure why though
		xQueueSendToFront(xFreq_Queue, &frequency, NULL);
		return;
	}
	



	

	

} 
*/




/*
void CLI_Receive(uint8_t *pData, uint16_t Size)
{
	//reads in an initial input from the port before actually going
	uint8_t currentChar = ' ';
	//this is the working size, the current number of characters in the user's input is
	//starting at the beggining of the array
	uint16_t currentSize = 0;
	//while the user hasn't hit enter (or reached the size,
	//which they shouldn't, the commands are real short)
	while(currentChar != '\r' && currentSize < Size)
	{
		//added in a check for if the interrupt happens, like with what was in main before
		if(new_recieved == 1)
		{
			//return it to the terminal
			//then set the flag back to 0, so it checks again
		
			
			//read a character again
			currentChar = recieved_char;
			new_recieved = 0;
			sendbyte(currentChar);
			
			//if it's a regular ASCII letter (or symbol?)
			if(currentChar > 32 && currentChar < 127)
			{
					//print it out so the user can see it
					//sendbyte(currentChar);
					//and change the current value in 
					*(pData+currentSize) = currentChar;
					//increment the current size so it's ready to go for the next character
					currentSize++;
			}
			//if they hit the delete button (ASCII 127)
			//(Also added a check so there isn't trying to reach a negative index)
			if(currentChar == 127 && currentSize > 0)
			{
				//then send that byte as well 
				//which removes the previous character in the terminal
				//sendbyte(currentChar);
				//replace the current saved character with a space
				*(pData+currentSize) = ' ';
				//decrement the currentSize, so the next time
				//the (recently deleted) spot gets modified
				currentSize--;
			}
		
	

			
		}

	
	}
	
	//then, for the rest of the array past the currentSize
	//replace everything with a space so it's all initialized
	//but doesn't show up when printed
	if(currentSize < Size)
	{
		for(int i = currentSize; i < Size; i++)
		{
			*(pData+i) = ' ';
		}
	
	}
	
	
	


}


void CLI_Prompt(void)
{
	//makes an array for this string
	//and then prints it out using the size
	//probably a nicer way to do this
	uint8_t prompt_msg[] = "\r\n\nEnter Command:\r\n";
	CLI_Transmit(prompt_msg, (sizeof(prompt_msg) / sizeof(uint8_t)));

}


void CLI_Help(void)
{
	//same as prompt, just a for a bunch of different messages this time
	//not sure if this is the best way to format it, 
	//especially if later commands have arguements
	uint8_t help_msg1[] = "\r\n> List of commands:";
	CLI_Transmit(help_msg1, (sizeof(help_msg1) / sizeof(uint8_t)));
	uint8_t help_msg2[] = "\r\n	 ledon - turns LED on";
	CLI_Transmit(help_msg2, (sizeof(help_msg2) / sizeof(uint8_t)));
	uint8_t help_msg3[] = "\r\n	 ledoff - turns LED off";
	CLI_Transmit(help_msg3, (sizeof(help_msg3) / sizeof(uint8_t)));
	uint8_t help_msg4[] = "\r\n	 ledquery - prints LED state";
	CLI_Transmit(help_msg4, (sizeof(help_msg4) / sizeof(uint8_t)));
	uint8_t help_msg5[] = "\r\n	 help - prints list of commands (as you can see)";
	CLI_Transmit(help_msg5, (sizeof(help_msg5) / sizeof(uint8_t)));
	uint8_t help_msg6[] = "\r\n	 quit - ends program";
	CLI_Transmit(help_msg6, (sizeof(help_msg6) / sizeof(uint8_t)));
}





void CLI_Query(void)
{
	//get the GPIO ODR Line, which should have if the LED is on
	volatile unsigned int GPIO_ODR_checker = (GPIOA->ODR);
	//mask out the other output bits
	GPIO_ODR_checker &= GPIO_ODR_ODR5;
	//then move the one unmasked bit to position 0 so it's easy to check
	GPIO_ODR_checker = GPIO_ODR_checker >> 5;

	//depending on the result
	//print out a different message to the terminal
	if(GPIO_ODR_checker == 1)
	{
		uint8_t query_on_msg[] = "\r\n> Onboard LED is currently ON";
		CLI_Transmit(query_on_msg, (sizeof(query_on_msg) / sizeof(uint8_t)));
	}
	else
	{
		uint8_t query_off_msg[] = "\r\n> Onboard LED is currently OFF";
		CLI_Transmit(query_off_msg, (sizeof(query_off_msg) / sizeof(uint8_t)));
	
	}

}



void CLI_LEDON(void)
{
	//same printing with CLI_Transmit as usual
	uint8_t on_msg[] = "\r\n> Turning LED ON";
	CLI_Transmit(on_msg, (sizeof(on_msg) / sizeof(uint8_t)));
	//turn the LED on like the startup checker function
	GPIOA->ODR |= GPIO_ODR_ODR5;
	
	//added some ANSI code here
	
	uint8_t save_cursor_ANSI[] = "\x1b[s";
	CLI_Transmit(save_cursor_ANSI, (sizeof(save_cursor_ANSI) / sizeof(uint8_t)));
	
	//position the cursor so it's one line down from the start of the screen (starts at 1?)
	uint8_t top_ANSI[] = "\x1b[2;0H";
	CLI_Transmit(top_ANSI, (sizeof(top_ANSI) / sizeof(uint8_t)));
	
	//erase that line (so the last F in "LED Status: OFF" doesn't stick around
	//tried using "\x1b[K", but it kept just printing J for some reason
	uint8_t ANSI_clear[] = "\x1b[K";
	CLI_Transmit(ANSI_clear, (sizeof(ANSI_clear) / sizeof(uint8_t)));
	
	//write on the line to make a new status
	uint8_t status_msg[] = "LED Status: ON";
	CLI_Transmit(status_msg, (sizeof(status_msg) / sizeof(uint8_t)));
	
	//remake the scrollable window again, same deal with 3 lines down
	uint8_t mid_ANSI[] = "\x1b[3;r";
	CLI_Transmit(mid_ANSI, (sizeof(mid_ANSI) / sizeof(uint8_t)));
		
	//put the cursor back to where it was before
	uint8_t return_cursor_ANSI[] = "\x1b[u";
	CLI_Transmit(return_cursor_ANSI, (sizeof(return_cursor_ANSI) / sizeof(uint8_t)));

}


void CLI_LEDOFF(void)
{
	uint8_t off_msg[] = "\r\n> Turning LED OFF";
	CLI_Transmit(off_msg, (sizeof(off_msg) / sizeof(uint8_t)));
	//turn the LED off, also like the startup checker function
	GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;
	
	//also added ANSI code here, mostly the same but the message update is different

	uint8_t save_cursor_ANSI[] = "\x1b[s";
	CLI_Transmit(save_cursor_ANSI, (sizeof(save_cursor_ANSI) / sizeof(uint8_t)));
	
	//position the cursor so it's one line down from the start of the screen
	uint8_t top_ANSI[] = "\x1b[2;0H";
	CLI_Transmit(top_ANSI, (sizeof(top_ANSI) / sizeof(uint8_t)));
	
	
	
	uint8_t ANSI_clear[] = "\x1b[K";
	CLI_Transmit(ANSI_clear, (sizeof(ANSI_clear) / sizeof(uint8_t)));
	
	uint8_t status_msg[] = "LED Status: OFF";
	CLI_Transmit(status_msg, (sizeof(status_msg) / sizeof(uint8_t)));
	
	uint8_t mid_ANSI[] = "\x1b[3;r";
	CLI_Transmit(mid_ANSI, (sizeof(mid_ANSI) / sizeof(uint8_t)));
		
		
	uint8_t return_cursor_ANSI[] = "\x1b[u";
	CLI_Transmit(return_cursor_ANSI, (sizeof(return_cursor_ANSI) / sizeof(uint8_t)));

	

}


void CLI_Input(uint8_t *pData, uint16_t Size){
	//look at the first set of value at (and from) the pointer
	//based on how the array (and size) are made, 
	//shouldn't get anywhere near the bounds, current commands are like 7 letters
	//definitely a nicer way to do this, at least for handling whitespaces + removing caps 
	if(*(pData) == 'h'
	&& *(pData+1) == 'e' 
	&& *(pData+2) == 'l'
	&& *(pData+3) == 'p' )
	{
		CLI_Help();
		//put a return after each one, don't want it to try looking for any others (or the error function)
		//after getting a match. Will probably need to split this up once we start adding arguements
		return;
	}
	
	if(*(pData) == 'l'
	&& *(pData+1) == 'e'
	&& *(pData+2) == 'd'
	&& *(pData+3) == 'o'
	&& *(pData+4) == 'n' )
	{
		CLI_LEDON();
		return;
	}
	
	if(*(pData) == 'l'
	&& *(pData+1) == 'e'
	&& *(pData+2) == 'd'
	&& *(pData+3) == 'o'
	&& *(pData+4) == 'f'
	&& *(pData+5) == 'f')
	{
		CLI_LEDOFF();
		return;
	}
	
	if(*(pData) == 'l' 
		&& *(pData+1) == 'e'
		&& *(pData+2) == 'd'
		&& *(pData+3) == 'q'
		&& *(pData+4) == 'u'
		&& *(pData+5) == 'e'
		&& *(pData+6) == 'r'
		&& *(pData+7) == 'y'
	)
	{
		CLI_Query();
		return;
	}
	//if it isn't any of the commands listed and it gets here
	//then print out an error message for the user to see
	else
	{
		uint8_t error_msg[] = "\r\n> Error - Not a registered command";
		CLI_Transmit(error_msg, (sizeof(error_msg) / sizeof(uint8_t)));
	}
	
	
}


uint8_t CLI_Quit(uint8_t *pData, uint16_t Size)
{
	//like CLI_Input, just make a bunch of checks on the
	//pointer's value and the ones near it
	if(*(pData) == 'q'
	&& *(pData+1) == 'u'
	&& *(pData+2) == 'i'
	&& *(pData+3) == 't' )
	{
		//if it does match, print a message to show it
		//and return 0 so the loop gets exited
		uint8_t quit_msg[] = "\r\n> Ending the program";
		CLI_Transmit(quit_msg, (sizeof(quit_msg) / sizeof(uint8_t)));
		return 0;
	}
	else
	{
		//or return 1 if it isn't a match, so it can go and look for commands
		return 1;
	}

}


void CLI_Clean(uint8_t *pData, uint16_t Size)
{
	//actually use the size for once
	//go from the beginning to the end of the array

	for(int i = 0; i < Size; i++)
	{
		//and white out each of the values
		//that way it's cleaned up for the next loop
		*(pData + i) = ' ';
	}

}*/