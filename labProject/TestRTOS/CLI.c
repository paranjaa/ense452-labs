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



//extern uint8_t recieved_char;
//extern uint8_t new_recieved;
extern QueueHandle_t xWire_Queue;
extern QueueHandle_t xClipper_msg_Queue;
extern QueueHandle_t xPrice_Queue;

//make an array for storing the characters the user sends 
//5 should be enough
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
	if (strcmp(inputArray, "more") == 0)
	{
		uint8_t msg1[] = "\n\r - trying to increase the price\n\r";
		uint8_t price_send = 1;
		xQueueSendToBack( xPrice_Queue, &price_send, NULL);
		CLI_Transmit(msg1, (sizeof(msg1) / sizeof(uint8_t)));
		return;
	}
	if (strcmp(inputArray, "less") == 0)
	{
		uint8_t msg2[] = " - trying to decrease the price\n\r";
		uint8_t price_send = 0;
		xQueueSendToBack( xPrice_Queue, &price_send, NULL);
		CLI_Transmit(msg2, (sizeof(msg2) / sizeof(uint8_t)));
		return;
	}
	if (strcmp(inputArray, "wire") == 0)
	{
		uint8_t msg3[] = " -trying to purchase wire \n\r";
		CLI_Transmit(msg3, (sizeof(msg3) / sizeof(uint8_t)));
		uint8_t wire_send = 20;
		xQueueSendToBack( xWire_Queue, &wire_send, NULL);
		return;
	}
	
	if (strcmp(inputArray, "auto") == 0)
	{
		uint8_t msg3[] = " - trying to purchase autoclipper \n\r";
		CLI_Transmit(msg3, (sizeof(msg3) / sizeof(uint8_t)));
		uint8_t clipper_send = 1;
		xQueueSendToBack(xClipper_msg_Queue, &clipper_send, NULL);
		return;
	}
	else
	{		
			uint8_t error_msg[] = "\n\r- error, unrecognized command\n\r";
			CLI_Transmit(error_msg, (sizeof(error_msg) / sizeof(uint8_t)));

	}

}



