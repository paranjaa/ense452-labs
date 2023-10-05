//ADDED SOME CODE HERE
//include <iostream>
//#include "usart.h"
//THE COM PORT is COM5
//int main() {
//	serial_open();
	//  return 0;
//} 
	
//ENSE 452
//Alok Paranjape
// October 4th:
//Lab 2: Checking if the github is working again
#include "stm32f10x.h"


void delay()
{
	
	unsigned volatile int c, d;
   
   for (c = 1; c <= 3000; c++)
       for (d = 1; d <= 3000; d++)
       {}
}

int main() {
		//RCC->APB2ENR |= 
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;


		//prep onboard light, in PA5?
		GPIOA->CRL |=  GPIO_CRL_MODE5_0 |  GPIO_CRL_MODE5_1;
		GPIOA->CRL &= ~GPIO_CRL_CNF5_0 &~ GPIO_CRL_CNF5_1;
		
	
		//just infinitely blink that LED
		while(1)
		{
			GPIOA->ODR |= GPIO_ODR_ODR5; //turn on the onboard light
			delay();
			GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5; //turn off the onboard light 
			delay();
		}
	
	
    //return 0;
}
	
	