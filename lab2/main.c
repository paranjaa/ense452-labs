
//ENSE 452
//Alok Paranjape
// October 4th:
//Lab 2: Checking if the github is working again

//ADDED SOME CODE HERE
//include <iostream>

//THE COM PORT is COM5
//int main() {
//	serial_open();
	//  return 0;
//} 
	
#include "stm32f10x.h"
#include "usart.h"

//Connected on COM 5


int main() {
	
	//check if the program is running with the onboard LED
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL |=  GPIO_CRL_MODE5_0 |  GPIO_CRL_MODE5_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF5_0 &~ GPIO_CRL_CNF5_1;
	GPIOA->ODR |= GPIO_ODR_ODR5;
	delay();
	GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;
	
	
	serial_open();
	
	while(1)
	{
		sendData();
		//recieveData();
	}
	
	//return 0;
}


