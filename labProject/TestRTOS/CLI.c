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

extern QueueHandle_t xWire_Queue;
extern QueueHandle_t xClipper_msg_Queue;
extern QueueHandle_t xPrice_Queue;


char inputArray[10];


void CLI_Transmit(uint8_t *pData, uint16_t Size)
{

	for(uint16_t i = 0; i < Size; i++)
	{
		sendbyte( *(pData + i) );
	}

}

void CLI_Receive(uint8_t *pData, uint16_t Size)
{
	uint8_t newChar = *pData;
	uint8_t currentIndex = strlen(inputArray);
	
	sendbyte(newChar);
	
	//if it's a delete
	if(newChar == 8 || newChar == 127)
	{
		if(currentIndex > 0)
		{
			//delete the latest character
			inputArray[currentIndex] = ' ';
			inputArray[currentIndex-1] = '\0';
			
		}
	}
	
	if(newChar == '\r')
	{
		sendbyte('\n');
		//CLI_Transmit(inputArray, (sizeof(inputArray) / sizeof(char)));
		CLI_Decide((uint8_t*)inputArray, (sizeof(inputArray) / sizeof(char))); 
		//erase the input array for next time
		//was using a loop before
		memset(inputArray, 0, sizeof(inputArray));
		
		return;
	}
	
	if(currentIndex < 10)
	{
		inputArray[currentIndex] = newChar;
		inputArray[currentIndex+1] = '\0';
	
	}
	
}


//making a new function for dealing with the LED


void CLI_Decide(uint8_t *pData, uint16_t Size)
{
	if (strcmp(inputArray, "raise") == 0)
	{
		uint8_t msg1[] = "> trying to increase the price\n\r";
		uint8_t price_send = 1;
		xQueueSendToBack( xPrice_Queue, &price_send, NULL);
		CLI_Transmit(msg1, (sizeof(msg1) / sizeof(uint8_t)));
		return;
	}
	if (strcmp(inputArray, "lower") == 0)
	{
		uint8_t msg2[] = "> trying to decrease the price\n\r";
		uint8_t price_send = 0;
		xQueueSendToBack( xPrice_Queue, &price_send, NULL);
		CLI_Transmit(msg2, (sizeof(msg2) / sizeof(uint8_t)));
		return;
	}
	if (strcmp(inputArray, "wire") == 0)
	{
		uint8_t msg3[] = "> trying to purchase wire \n\r";
		CLI_Transmit(msg3, (sizeof(msg3) / sizeof(uint8_t)));
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
	{		
			uint8_t error_msg[] = "> error, unrecognized command\n\r";
			CLI_Transmit(error_msg, (sizeof(error_msg) / sizeof(uint8_t)));

	}

}



