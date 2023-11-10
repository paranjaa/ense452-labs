/*
ENSE 452
Alok Paranjape
200246370
October 22nd
usart.c has the function definitions for phase 1
the basic sending and getting byte functions
plus a startup and delay function I used for debugging
Added in a bit for timing, but wasn't sure how to test it, especially with how late things got
*/


#include "usart.h"
#include "stm32f10x.h"

//added code here to interact with FreeRTOS and its queue
#include "FreeRTOS.h"
#include "queue.h"


extern QueueHandle_t CLI_Queue;

volatile uint8_t recieved_char;
volatile uint8_t new_recieved;




void USART2_IRQHandler(void)
{
	
	
	
	//read the recieved data from the flag
	//should probably add a check for the RXNE bit
	//recieved_char = USART2 ->DR;
	
	//then set the flag to show it got a new character
	//new_recieved = 1;
	
	//also clearing the flag, suprised it was working without doing this
	//USART2->SR &= ~USART_SR_RXNE;
	
	//starting again since we want to use the queue and not a global variable
	uint8_t recieved_char = USART2->DR;
	xQueueSendToFrontFromISR(CLI_Queue, &recieved_char, NULL);
	new_recieved = 1;
	USART2->SR &= ~USART_SR_RXNE;

}

void startupCheck(void)
{	
	//show the program is running by toggling the onboard LED

	GPIOA->ODR |= GPIO_ODR_ODR5;
	//wait a second, then turn it off
	delay2();
	GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;
}

//delay for about a second, holdover from ENEL 351 code
void delay2(void)
{
	
	unsigned volatile int c, d;
   
   for (c = 1; c <= 3000; c++)
       for (d = 1; d <= 3000; d++)
       {}
}


/** Configure and enable the device. */
void serial_open(void)
{
	//Ensure the the Port A clock is enabled.
	RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN; 
  //Ensure the USART 2 clock is enabled. (It's in APB1)
	RCC->APB1ENR |=  RCC_APB1ENR_USART2EN; 
	
	 	
	//enabling timer 2 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	

	
	//setting the prescaler value to one less than the clock speed (wrongly assumed it was 24 MHZ before)
	TIM2->PSC = 0x8C9F;
	
	//setting the auto reset value to 10ms (or 100hz), might want to double check that, since it wasn't for 
	TIM2->ARR = 100;

	
	// Enabling Timer2
	TIM3->CR2 |= TIM_CR1_ARPE | TIM_CR1_CEN; 
	
  //Configure PA2 for alternate function output Push-pull mode, max speed 50 MHz.
	//MODE 11: Output, 50 Mhz
	//CNF 10: Alternate function push pull mode
	GPIOA->CRL |=  GPIO_CRL_MODE2_0 |  GPIO_CRL_MODE2_1; //output 50Mhz
	GPIOA->CRL |= GPIO_CRL_CNF2_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF2_0;
	
  //Configure PA3 for Input with pull-up / pull-down.
	//CRL 10: Input with pull-up / pull-down, so set bit 1, clear bit 0
	GPIOA->CRL |=  GPIO_CRL_MODE3_1;
	GPIOA->CRL &= ~GPIO_CRL_MODE3_0;
	
	//get the output ready for using the onboard LED
	GPIOA->CRL |=  GPIO_CRL_MODE5_0 |  GPIO_CRL_MODE5_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF5_0 &~ GPIO_CRL_CNF5_1;
	
	

	
	//set the UsartEnable bit for USART2, before setting TE and RE
	USART2->CR1 |= USART_CR1_UE;
  //Enable the USART TransmitEnable and RecieveEnable bits
	USART2->CR1 |= USART_CR1_TE;
	USART2->CR1 |= USART_CR1_RE;
	//set the UsartEnable bit again for USART2, after TE and RE
	//(not actually sure why, I just remember Dave mentioning it and noting it down)
	USART1->CR1 |= USART_CR1_UE;

	//clear the 12th bit in CR1 so it's set to "Start bit, 8 Data bits, n Stop bit"
	USART2 -> CR1 &= ~0x1000; 
	
	//set the USART Interrupt Enable, so it generates an interrupt when a character is recieved
	USART2->CR1 |= USART_CR1_RXNEIE;
	
	
	
	//Set the baud rate to to 115200
	//calculated out this first value for it, 19.5 with Mantissa: 1011 and Frac: 0.5
	//USART2->BRR = 0x138;
	//But it was probably wrong,
	//and also Trevor showed me a much less confusing way to set those bits
	USART2 -> BRR = (8 << 0) | (19 << 4);
	
	
	//enabling the interrupt for USART2
	NVIC_EnableIRQ(USART2_IRQn);
	
	//enabling timer 2 interrupt
	NVIC_EnableIRQ(TIM2_IRQn);
	

}

//just reverses every bit that was set, flipped all the 1 sets (with |) to 0s (with &)
//not sure if this actually puts everything back to how it was on startup, probably need to check defaults
void serial_close(void)
{

	//turn off the port A clock and the usart2 clock
	RCC->APB2ENR &=  ~RCC_APB2ENR_IOPAEN; 
	RCC->APB1ENR &=  ~RCC_APB1ENR_USART2EN; 
	
	
	//stop using PA for alternate function push pull, and set the speed off of 50
	GPIOA->CRL &=  ~GPIO_CRL_MODE2_0;
	GPIOA->CRL &= ~GPIO_CRL_MODE2_1; 
	GPIOA->CRL &= ~GPIO_CRL_CNF2_1;
	GPIOA->CRL |= GPIO_CRL_CNF2_0;
	
	//stop PA3 from being used for input
	GPIOA->CRL &=  ~GPIO_CRL_MODE3_1;
	GPIOA->CRL |= GPIO_CRL_MODE3_0;
	
	//stop PA5 from being ready for an LED
	GPIOA->CRL &=  ~GPIO_CRL_MODE5_0;
	GPIOA->CRL &=  ~GPIO_CRL_MODE5_1;
	GPIOA->CRL |= GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1;
	
	//set the USARTEnable/TransmitEnable/RecieveEnable bits to 0
	USART2->CR1 &= ~USART_CR1_UE;
	USART2->CR1 &= ~USART_CR1_TE;
	USART2->CR1 &= ~USART_CR1_RE;
	

	//clear out the stop bit, which should put it back to default n bits
	USART2 -> CR1 |= ~0x1000; 
	
	//get rid of the baud rate
	USART2 -> BRR = 0x0;
	

}


//based off of code from the ENEL 351 USART lab from last winter
//with some changes, it previously just automatically looped and sent every char from ! to ~
int sendbyte(uint8_t b)
{
	//get the CR1 register, then mask it
	volatile unsigned int USART_TXE_checker = USART2->SR;
	USART_TXE_checker &= USART_SR_TXE;
	//TXE is in bit 7, move it over to position 0 for easy checking
	USART_TXE_checker = USART_TXE_checker >> 7;
	
	//start up the timer
	TIM2->CR1 |= TIM_CR1_CEN;
	//set the initial value to 0
	TIM2->CNT = 0;
	
	//make a value to check the timer, AND it with the flag to just get that bit
	volatile unsigned int timer2_checker = TIM2->SR;
	timer2_checker &= TIM_SR_UIF;
	while(USART_TXE_checker != 1)
	{
		//continue checking the TXE while in the loop
		USART_TXE_checker = USART2->SR;
		USART_TXE_checker &= USART_SR_TXE;
		USART_TXE_checker = USART_TXE_checker >> 7;
		
		//also keep checking the timer while doing this
		timer2_checker = TIM2->SR;
		timer2_checker &= TIM_SR_UIF;
		//if it equals one (since everything else got filtered out)
		//then there's a timeout error
		if(timer2_checker)
		{
			TIM2->SR &= ~TIM_SR_UIF; //clear out the update flag for the next call
			TIM2->CR1 &= ~TIM_CR1_CEN; //stop the timer from running
			TIM2->CNT = 0; //reset the counter
			//return 1, still not really sure how to make it fail though
			return 1;
		}
		
		

	}
	//after exiting the loop, send the bytes by copying them into the DR register
	USART2->DR = b;
	
	//stop the timer and reset the counter if it gets out the regular way as well
	TIM2->CR1 &= ~TIM_CR1_CEN;
	TIM2->CR1 = 0;
	
	
	//then return 0, wasn't sure how to make it fail, it just kept going through or getting stuck
	return 0;
}	

//also based on code from ENEL 351, also with a few changes, less
uint8_t getbyte(void)
{
	//adding in a value to get the time
	
	
	//get the SR register, mask it
	//RXNE is in position 5, move it so it's easy to check
	uint8_t value;
	volatile unsigned int USART_RXNE_checker = USART2->SR;
	USART_RXNE_checker &= USART_SR_RXNE;
	USART_RXNE_checker = USART_RXNE_checker >> 5;
	

	//if it's ready, then copy the value from the register
	if(USART_RXNE_checker == 1)
	{
		value = USART2 ->DR;
	}
	//then return it as the value
	return value;
}

