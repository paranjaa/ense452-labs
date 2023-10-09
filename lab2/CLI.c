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
	uint16_t currentSize = 0;
	while(currentChar != '\r' && currentSize < Size)
	{
		
		currentChar = getbyte();
		if(currentChar > 32 && currentChar < 127)
		{
				sendbyte(currentChar);
				*(pData+currentSize) = currentChar;
				currentSize++;
		}
		if(currentChar == 127)
		{
			sendbyte(currentChar);
			*(pData+currentSize) = ' ';
			currentSize--;
		}
		
	}
	
	if(currentSize < Size)
	{
		for(int i = currentSize; i < Size; i++)
		{
			*(pData+i) = ' ';
		}
	
	}
	uint8_t result1[] = "\n \rYou Typed: ";
	CLI_Transmit(result1, (sizeof(result1) / sizeof(uint8_t)));
	
	//CLI_Transmit(pData, Size);
	


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
	uint8_t help5[] = "	help - prints list of commands (as you can see)";
	CLI_Transmit(help5, (sizeof(help5) / sizeof(uint8_t)));
	uint8_t help6[] = "	quit - ends program";
	CLI_Transmit(help6, (sizeof(help6) / sizeof(uint8_t)));
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


void CLI_LEDON(void)
{
	uint8_t on_msg[] = "Turning LED ON";
	CLI_Transmit(on_msg, (sizeof(on_msg) / sizeof(uint8_t)));
	GPIOA->ODR |= GPIO_ODR_ODR5;
}

void CLI_LEDOFF(void)
{
	uint8_t off_msg[] = "Turning LED OFF";
	CLI_Transmit(off_msg, (sizeof(off_msg) / sizeof(uint8_t)));
	GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;
}

void CLI_Input(uint8_t *pData, uint16_t Size)
{
	if(*(pData) == 'h' && *(pData+1) == 'e' && *(pData+2) == 'l' && *(pData+3) == 'p' )
	{
		CLI_Help();
		return;
	}
	
	if(*(pData) == 'l' && *(pData+1) == 'e' && *(pData+2) == 'd' && *(pData+3) == 'o' && *(pData+4) == 'n' )
	{
		CLI_LEDON();
		return;
	}
	
	if(*(pData) == 'l' && *(pData+1) == 'e' && *(pData+2) == 'd' && *(pData+3) == 'o' && *(pData+4) == 'f' && *(pData+5) == 'f')
	{
		CLI_LEDOFF();
		return;
	}
	else
	{
		uint8_t error_msg[] = "Error - Not a registered command";
		CLI_Transmit(error_msg, (sizeof(error_msg) / sizeof(uint8_t)));
	}
	
	
}