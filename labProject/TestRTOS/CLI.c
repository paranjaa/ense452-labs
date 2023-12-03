/*
ENSE 452 Lab Project
Alok Paranjape
200246370
December 3rd

Function definitions for reading and writing whole
commands via usart, using the related functions in other files

also a function for deciding what to do with that input,
and what queues need to get new information from it

*/
#include "stm32f10x.h"
#include "usart.h"
#include "CLI.h"
#include "FreeRTOS.h"
#include "queue.h"


//need to send values to these queues from the CLI,
//so get those from their file
extern QueueHandle_t xWire_Queue;
extern QueueHandle_t xClipper_msg_Queue;
extern QueueHandle_t xPrice_Queue;


//get a char array to store user input
char inputArray[10];


void CLI_Transmit(uint8_t *pData, uint16_t Size)
{
	//loop through the given array
	for(uint16_t i = 0; i < Size; i++)
	{
		//use send byte to print it one value at a time
		sendbyte( *(pData + i) );
	}

}

//made some changes from previous labs, including using char
//and not having an internal loop, since this is running in a task
void CLI_Receive(uint8_t *pData, uint16_t Size)
{
	//get the currently inputted character 
	//(which should have come from the interrupt
	uint8_t newChar = *pData;
	
	//get the current length of the string
	uint8_t currentIndex = strlen(inputArray);
	
	//print it out to the user, so they can see what they typed
	sendbyte(newChar);
	
	//if it's a back space or delete
	if(newChar == 8 || newChar == 127)
	{
		//if there's still space in the string
		if(currentIndex > 0)
		{
			//delete the latest character
			inputArray[currentIndex] = ' ';
			//and replace what was before it with a null
			//so it's apparent the input ends one space before
			inputArray[currentIndex-1] = '\0';
			
		}
	}
	
	//if it's an enter (which comes in as just carriage return)
	if(newChar == '\r')
	{
		//send back a new line as well
		sendbyte('\n');
		
		//run the function to decide what to do with the input
		CLI_Decide(inputArray, (sizeof(inputArray) / sizeof(char))); 
		//erase the input array for next time
		//was using a loop before, really glad it's chars now
		memset(inputArray, 0, sizeof(inputArray));
		//then leave the function for the next call
		return;
	}
	
	//if there's still room in the string
	if(currentIndex < 10)
	{
		//put the new character in the latest spot
		inputArray[currentIndex] = newChar;
		//then move the null it replaced forward
		inputArray[currentIndex+1] = '\0';
		return;
	}
	
}



void CLI_Decide(char *pData, uint16_t Size)
{
	//using strcmp to check for matching commands
	if (strcmp(inputArray, "raise") == 0)
	{
		//if they typed to raise the price
		//print that it's trying (since here, we don't know the current funds)
		uint8_t msg1[] = "> trying to increase the price\n\r";
		CLI_Transmit(msg1, (sizeof(msg1) / sizeof(uint8_t)));
		//make a 1 to increment the price and put it in the queue for the other task
		uint8_t price_send = 1;
		xQueueSendToBack( xPrice_Queue, &price_send, NULL);
		return;
	}
	if (strcmp(inputArray, "lower") == 0)
	{
		//same deal with lowering the price, and the other commands below
		uint8_t msg2[] = "> trying to decrease the price\n\r";
		CLI_Transmit(msg2, (sizeof(msg2) / sizeof(uint8_t)));
		uint8_t price_send = 0;
		xQueueSendToBack( xPrice_Queue, &price_send, NULL);
		return;
	}
	if (strcmp(inputArray, "wire") == 0)
	{
		uint8_t msg3[] = "> trying to purchase wire \n\r";
		CLI_Transmit(msg3, (sizeof(msg3) / sizeof(uint8_t)));
		//sending back wire to increase the user's supplies, in its queue
		uint8_t wire_send = 20;
		xQueueSendToBack( xWire_Queue, &wire_send, NULL);
		return;
	}
	
	if (strcmp(inputArray, "autoclip") == 0)
	{
		uint8_t msg3[] = "> trying to purchase autoclipper \n\r";
		CLI_Transmit(msg3, (sizeof(msg3) / sizeof(uint8_t)));
		uint8_t clipper_send = 1;
		xQueueSendToBack(xClipper_msg_Queue, &clipper_send, NULL);
		return;
	}
	if(strcmp(inputArray, "help") == 0)
	{
		//if they type help
		//print a bunch of messages explaining each command
		uint8_t help_msg1[] = "> list of commands: \n\r";
		CLI_Transmit(help_msg1, (sizeof(help_msg1) / sizeof(uint8_t)));
		
		uint8_t help_msg2[] = 
		"raise -> increases clip price (up to 11$), decreases sell speed\n\r";
		CLI_Transmit(help_msg2, (sizeof(help_msg2) / sizeof(uint8_t)));
		
		uint8_t help_msg3[] = 
		"lower -> decreases clip price (at least 1$), increases sell speed\n\r";
		CLI_Transmit(help_msg3, (sizeof(help_msg3) / sizeof(uint8_t)));
		
		uint8_t help_msg4[] = 
		"wire -> tries spending (14$) to purchase wire (10 clips worth)\n\r";
		CLI_Transmit(help_msg4, (sizeof(help_msg4) / sizeof(uint8_t)));
		
		uint8_t help_msg5[] = 
		"autoclip -> tries spending (40$) to purchase an auto clip maker\n\r";
		CLI_Transmit(help_msg5, (sizeof(help_msg5) / sizeof(uint8_t)));
		
		uint8_t help_msg6[] = 
		"help -> displays this list of commands right here \n\r";
		CLI_Transmit(help_msg6, (sizeof(help_msg6) / sizeof(uint8_t)));
		
		uint8_t help_msg7[] = 
		"blue button -> (on the board, not a command)  \n\r";
		CLI_Transmit(help_msg7, (sizeof(help_msg7) / sizeof(uint8_t)));
		
		uint8_t help_msg8[] = 
		"	manually makes a single paper clip, using 1 wire\n\r";
		CLI_Transmit(help_msg8, (sizeof(help_msg8) / sizeof(uint8_t)));
		
	
		return;
	
	}
	else
	{		//if it isn't any of those, print for that as well
			uint8_t error_msg[] = "> error, unrecognized command\n\r";
			CLI_Transmit(error_msg, (sizeof(error_msg) / sizeof(uint8_t)));

	}

}



