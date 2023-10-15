//ENSE 452
//Alok Paranjape
//October 14th
//Lab 3: Added in code from the interrupt lab from ene351



#include "stm32f10x.h"
#include "usart.h"
#include "CLI.h"
#include "util.h"


//The handler will appear as a function in your main.c file and must use the name SysTick_Handler. 
void SysTick_Handler(void)
{
	//do something
	
	//toggle the led, about every half a second
	GPIOA->ODR ^= GPIO_ODR_ODR5;
}

int main() {
	
	//setup clock and onboard LED
	clockInit();
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL |=  GPIO_CRL_MODE5_0 |  GPIO_CRL_MODE5_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF5_0 &~ GPIO_CRL_CNF5_1;
	
	
	//setup LED in D7
	
	GPIOA->CRH |=  GPIO_CRH_MODE8_0 |  GPIO_CRH_MODE8_1;
	GPIOA->CRH &= ~GPIO_CRH_CNF8_0 &~ GPIO_CRH_CNF8_1;
	
	//use PB0 as an input? it already is by default
	
	//just put this here to make sure it's working
	//GPIOA->ODR |= GPIO_ODR_ODR8; 
	
	//calling the setup function
	//serial_open();
	
	//startupCheck();
	
	
	//code from ENEL 351 Lab #4 from the winter term
	
	//Let’s look at SYSTICK first:
	
				//The SYSTICK system has 4 registers that might need to be considered: CTRL, LOAD, VAL, and CALIB.
				//The order of configuration operations will be:
				//1) Disable SYSTICK by writing 0 into the CTRL register
				SysTick->CTRL = 0x0;
				//2) Clear the SYSTICK counter by writing 0 into the VAL register
				SysTick->VAL = 0x0;
				
				//3) Configure the interval for the interrupt by writing a value into the LOAD register
				
				//The value is the number of counts of the 24 MHZ clock that occur before the interrupt is generated.
				//The value is also automatically reloaded into the counter after each SYSTICK event
				SysTick->LOAD = 0xB71B00;
				
				
				//4) Select the clock source, enable the interrupt, and enable the counter by writing a 7 into the CTRL register
				//SYSTICK will also require a handler or Interrupt Service Routine to respond to the interrupt. 
				
				//0x7 is 0111, which puts 1s the three non reserved bits CTRL has
				SysTick->CTRL = 0x7;
	
		//2) Create an interrupt service routine for SYSTICK which will toggle the state of any available output on your
	//development board when it executes. 
	
	//^ it's up above 
	
	/*
	//setting the max size of the array as 32, totally enough room
	uint16_t testSize = 32;
	uint8_t testCharArray[testSize];
	
	//used to leave the while loop, when it's zero
	uint8_t quit_zero = 1;
	
	while(1)
	{
		//send out the initial prompt to enter a command
		CLI_Prompt();
		//record the user's input (and also echo it back to them so they can see)
		CLI_Receive(testCharArray,testSize);
		//check if they put in "quit"
		quit_zero = CLI_Quit(testCharArray, testSize);
		if(quit_zero == 0)
		{
			//if they do, leave the loop
			break;
		}	
		//after that, read the input the user put in and call the right function for it
		CLI_Input(testCharArray, testSize);
		//then white out the spaces so it's ready for the next run
		CLI_Clean(testCharArray, testSize);
	
	}	
	//also just going to turn off the LED so it doesn't stay on while nothing's happening
	GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;*/
	
	while(1)
	{
	
	}
	return 0;
}

