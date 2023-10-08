
//ENSE 452
//Alok Paranjape
// October 4th:
//Lab 2: Checking if the github is working again


#include "stm32f10x.h"
#include "usart.h"
#include "CLI.h"
#include <string.h>

//Connected on COM 5

void startupCheck(void)
{	
	//show the program is running by toggling the onboard LED
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL |=  GPIO_CRL_MODE5_0 |  GPIO_CRL_MODE5_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF5_0 &~ GPIO_CRL_CNF5_1;
	GPIOA->ODR |= GPIO_ODR_ODR5;
	delay();
	GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;

}


int main() {
	
	startupCheck();

	
	serial_open();
	
	//phase 1
	//uint8_t testChar = 'a';
	
	uint16_t testSize = 4;
	uint8_t testCharArray[testSize];
	
	
	//testCharArray[0] = 't';
	//testCharArray[1] = 'e';
	//testCharArray[2] = 's';
	//testCharArray[3] = 't';
	
	uint8_t* testPointer = &testCharArray[0];
	while(1)
	{
		
		//CLI_Receive(testCharArray, testSize);
		CLI_Help();
		delay();
		
		
	}
	
	//return 0;
}

		
		//sendbyte(*testPointer);
		//sendbyte(*(testPointer+1));
		//sendbyte(*(testPointer+2));
		//sendbyte(*(testPointer+3));
		

		//sendData();
		//sendbyte(testChar);
		//recieveData();
		
		//testChar = getbyte();
		
		//if(testChar == 'y')
		//{
		//	GPIOA->ODR |= GPIO_ODR_ODR5;
		//}
		
		//if(testChar == 'n')
		//{
		//		GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;
		//}
		//echoCharacter();
		//CLI_Transmit(testPointer, testSize);
