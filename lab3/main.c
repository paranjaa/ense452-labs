//ENSE 452
//Alok Paranjape
//September 14th
//Lab 3: Testing that this got added to github correctly
//just some blinking LED code like lab 1
#include "stm32f10x.h"


void delay()
{
	
	unsigned volatile int c, d;
   
   for (c = 1; c <= 3000; c++)
       for (d = 1; d <= 3000; d++)
       {}
}

int main() {
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;


		GPIOA->CRL |=  GPIO_CRL_MODE5_0 |  GPIO_CRL_MODE5_1;
		GPIOA->CRL &= ~GPIO_CRL_CNF5_0 &~ GPIO_CRL_CNF5_1;
		
	
		while(1)
		{
			GPIOA->ODR |= GPIO_ODR_ODR5; //turn on the onboard light
			delay();
			GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5; //turn off the onboard light 
			delay();
		}
	
	
    //return 0;
}

