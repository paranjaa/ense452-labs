/*
ENSE 452
Alok Paranjape
200246370
October 8th
usart.c has the function definitions for phase 1
the basic sending and getting byte functions
plus a startup and delay function I used for debugging
*/


#include "usart.h"
#include "stm32f10x.h"


void USART2_IRQHandler(void)
{
	//uint8_t value;
	//volatile unsigned int USART_RXNE_checker = USART2->SR;
	//USART_RXNE_checker &= USART_SR_RXNE;
	//USART_RXNE_checker = USART_RXNE_checker >> 5;
	
	//if it's ready, then copy the value from the register
	//if(USART_RXNE_checker == 1)
	//{
  //		value = USART2 ->DR;
	//}
	//then return it as the value
	uint8_t value;
	value = USART2 ->DR;
	sendbyte(value);

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

	
	// Configure clocks and IO pins for TIM3 CH1
	//TIM3->CR1 |= TIM_CR1_CEN; // Enable Timer3
	//IM3->EGR |= TIM_EGR_UG; // Reinitialize the counter
	//TIM3->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // PWM mode 1
	//TIM3->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC1FE; // Preload Enable, Fast Enable
	//TIM3->CCER |= TIM_CCER_CC1E; // Enable CH1
	//TIM3->PSC = 0x095F; // Divide 24 MHz by 2400 (PSC+1), PSC_CLK= 10000 Hz, 1 count = 0.1 ms
	//ARR defines the period
	//TIM3->ARR = 100; // 100 counts = 10 ms or 100 Hz 
	//CCR defines the pulse width
	//TIM3->CCR1 = 50; // 50 counts = 5 ms = 50% duty cycle
	//TIM3->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN; // Enable Timer3
	
	 	
	//enabling timer 2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	
	
	
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
	
	
	NVIC_EnableIRQ(USART2_IRQn);
	

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
	
	while(USART_TXE_checker != 1)
	{
		//continue checking the TXE while in the loop
		USART_TXE_checker = USART2->SR;
		USART_TXE_checker &= USART_SR_TXE;
		USART_TXE_checker = USART_TXE_checker >> 7;

	}
	//after exiting the loop, send the bytes by copying them into the DR register
	USART2->DR = b;
	//then return 0, wasn't sure how to make it fail, it just kept going through or getting stuck
	return 0;
}	

//also based on code from ENEL 351, also with a few changes, less
uint8_t getbyte(void)
{
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

