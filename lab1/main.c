//ENSE 452
//Alok Paranjape
//September 14th
//Lab 1: Blinking the LED on the nucleo board to blink
// and also setting up the github to share it
//got soem of the code for this from 352 (the delay function) 
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
		
		//do the same things to what's in PA0 as well to set the LED that way?
		//prep light 0 (in PA0/A0)
		GPIOA->CRL |=  GPIO_CRL_MODE0_0 |  GPIO_CRL_MODE0_1;
		GPIOA->CRL &= ~GPIO_CRL_CNF0_0 &~ GPIO_CRL_CNF0_1;
		

	
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
