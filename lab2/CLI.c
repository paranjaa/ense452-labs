#include "stm32f10x.h"
#include "usart.h"
#include "CLI.h"

void CLI_Transmit(uint8_t *pData, uint16_t Size)
{
	for(uint16_t i = 0; i < Size; i++)
	{
		sendbyte( *(pData + i) );
	}
	sendbyte('\r');
	sendbyte('\n');

}

void CLI_Receive(uint8_t *pData, uint16_t Size)
{
	uint8_t currentChar = getbyte();
	while(currentChar != '\r')
	{
		
		currentChar = getbyte();
		if(currentChar > 32 && currentChar < 127)
		{
				sendbyte(currentChar);
		}
		
		
	}

}

void CLI_Prompt(void)
{
	uint8_t command_prompt[] = "Enter Command:";
	CLI_Transmit(command_prompt, (sizeof(command_prompt) / sizeof(uint8_t)));

}

void CLI_Help(void)
{
	uint8_t help1[] = "List of commands:";
	CLI_Transmit(help1, (sizeof(help1) / sizeof(uint8_t)));
	uint8_t help2[] = "	ledon - turns LED on";
	CLI_Transmit(help2, (sizeof(help2) / sizeof(uint8_t)));
	uint8_t help3[] = "	ledof - turns LED off";
	CLI_Transmit(help3, (sizeof(help3) / sizeof(uint8_t)));
	uint8_t help4[] = "	ledquery - prints LED state";
	CLI_Transmit(help4, (sizeof(help4) / sizeof(uint8_t)));
}

void CLI_Query(void)
{
	//get the GPIO ODR Line, which should have if the LED is on
	volatile unsigned int GPIO_ODR_checker = (GPIOA->ODR);
	GPIO_ODR_checker &= GPIO_ODR_ODR5;
	GPIO_ODR_checker = GPIO_ODR_checker >> 5;

	if(GPIO_ODR_checker == 1)
	{
		uint8_t query_on[] = "Onboard LED is currently ON";
		CLI_Transmit(query_on, (sizeof(query_on) / sizeof(uint8_t)));
	}
	else
	{
		uint8_t query_off[] = "Onboard LED is currently OFF";
		CLI_Transmit(query_off, (sizeof(query_off) / sizeof(uint8_t)));
	
	}
	

}