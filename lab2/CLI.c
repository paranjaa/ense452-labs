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
	
	
	
	
	/*
	sendbyte(*pData);
	delay();
	
	sendbyte(*(pData+1));
	delay();
	
	sendbyte(*(pData+2));
	delay();

	sendbyte(*(pData+3));
	delay();



	sendbyte('\n');
	delay();
	
	sendbyte('\r');
	delay();
	*/

}

void CLI_Receive(uint8_t *pData, uint16_t Size)
{
	uint8_t testCharArray[Size];
	uint8_t testChar;
	pData = &testCharArray[0];
	
	for(int i; i < Size; i++)
	{
		testChar = getbyte();
		testCharArray[i] = testChar;
	}

}