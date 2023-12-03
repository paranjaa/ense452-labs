#include "util.h"


void serial_init(void)
{
	
	//enabling clocks on Port A, Port C, AFIO, USART2 and Timer2
	RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; 
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	
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
	
	//set up PA5 (the onboard LED) as an output so it can get blinked 
	GPIOA->CRL |=  GPIO_CRL_MODE5_0 |  GPIO_CRL_MODE5_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF5_0 &~ GPIO_CRL_CNF5_1;
	
	
	//setting up USART, baud rate 115200
	USART2->BRR = (8<<0) | (19<<4);
	
	//Set the RecieveEnable, Transmit Enable and USARTEnable bits in USART2
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	
	//clear the 12th bit in CR1 so it's set to "Start bit, 8 Data bits, n Stop bit"
	USART2 -> CR1 &= ~0x1000; 
	
	
	//set the USART Interrupt Enable, so it generates an interrupt when a character is recieved?
	//USART2->CR1 |= USART_CR1_RXNEIE;
	
	
	//setting the prescaler value to one less than the clock speed (wrongly assumed it was 24 MHZ before)
	TIM2->PSC = 0x8C9F;
	//setting the auto reset value to 10ms (or 100hz), might want to double check that, since it wasn't for 
	TIM2->ARR = 100;
	// Enabling Timer2
	TIM3->CR2 |= TIM_CR1_ARPE | TIM_CR1_CEN; 
	
	
//Onboard Button -> PC/13
	//need to setup interrupts on PC 13
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	//Turn on the clocks for AFIO and PortC ( RCC-> APB2ENR)
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	
	//Select Port C pins as the source for EXTI 0 events (AFIO->EXTICR)
	AFIO->EXTICR[3] |= 0x20;
	
	
	//Unmask PC13 as the interrupt source (EXTI->IMR)
	EXTI->IMR |= 0x2000;
	
	//select the falling edge of PC13 events as the trigger (EXTI->FTSR)
	EXTI->FTSR |= 0x2000;
	
	//Unmask EXTI as an interrupt source in the NVIC (NVIC->ISER[0])
	NVIC_EnableIRQ (EXTI15_10_IRQn);
	
	USART2->CR1 |= USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USART2_IRQn);
}