/*
ENSE 452 Lab Project
Alok Paranjape
200246370
December 3rd

The function definitions for startup, 
as well as the printing the initial UI and window
*/


#include "util.h"
#include "usart.h"
#include "CLI.h"



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
	
	//Unmask EXTI as an interrupt source in the NVIC. Does the (NVIC->ISER[0]) step like in 351?
	NVIC_EnableIRQ (EXTI15_10_IRQn);
	
	//set it to create interrupts when it recieves on USART2
	USART2->CR1 |= USART_CR1_RXNEIE;
	//enable the interrupt (same deal as above?)
	NVIC_EnableIRQ(USART2_IRQn);
}


void intro_message(void)
{
	
	//put the cursor at the top left corner of the screen
	uint8_t top_ANSI[] = "\x1b[0;0H";
	CLI_Transmit(top_ANSI, (sizeof(top_ANSI) / sizeof(uint8_t)));
	
	//print out a title message
	uint8_t title_msg[] = "\rENSE 452 Lab Project (Universal Paperclips Phase 1a) \n\r";
	CLI_Transmit(title_msg, (sizeof(title_msg) / sizeof(uint8_t)));
	
	//print a bar for the number of paperclips in inventory
	uint8_t clip_msg[] = "Paperclips: \n\r";
	CLI_Transmit(clip_msg, (sizeof(clip_msg) / sizeof(uint8_t)));
	
	//print a bar for the amount of funding available
	uint8_t money_message[] = "Money: \n\r";
	CLI_Transmit(money_message, (sizeof(money_message) / sizeof(uint8_t)));
	
	//print a bar for the amount of wire ready for production
	//it's static, could use a variable if it needs to be rebalanced more
	uint8_t UI_msg1[] = "Wire (12$): \n\r";
	CLI_Transmit(UI_msg1, (sizeof(UI_msg1) / sizeof(uint8_t)));

	//print a bar of the amount of autoclippers currently running
	uint8_t UI_msg2[] = "Autoclippers (40$): \n\r";
	CLI_Transmit(UI_msg2, (sizeof(UI_msg2) / sizeof(uint8_t)));
	
	
	//print a bar for the currently set price
	uint8_t UI_msg3[] = "Price ($): 3 \n\r";
	CLI_Transmit(UI_msg3, (sizeof(UI_msg3) / sizeof(uint8_t)));
	
	//print a line to seperate where the commands are typed below 
	uint8_t UI_divider[] = "------------------------";
	CLI_Transmit(UI_divider, (sizeof(UI_divider) / sizeof(uint8_t)));
	

	
	//make a scrolling window starting below where the line was printed
	uint8_t mid_ANSI[] = "\x1b[8;r";
	CLI_Transmit(mid_ANSI, (sizeof(mid_ANSI) / sizeof(uint8_t)));
	
	
	//put the cursor there
	uint8_t mid_place_ANSI[] = "\x1b[8;0H";
	CLI_Transmit(mid_place_ANSI, (sizeof(mid_place_ANSI) / sizeof(uint8_t)));


}