#include "stm32f10x.h"
#include "usart.h"
#include "CLI.h"

void CLI_Transmit(uint8_t *pData, uint16_t Size)
{
	delay();
	for(uint16_t i = 0; i < Size; i++)
	{
		sendbyte( *(pData + i) );
	}
	sendbyte('\r');
	sendbyte('\n');

}

void CLI_Receive(uint8_t *pData, uint16_t Size)
{
	while(getbyte() != '\r')
	{
			echoCharacter();
	}
}

void CLI_Prompt()
{
	uint8_t command_prompt[] = "Enter Command:";
	CLI_Transmit(command_prompt, (sizeof(command_prompt) / sizeof(uint8_t)));

}