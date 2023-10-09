#include "usart.h"
#include "stm32f10x.h"


//delay for about a second, holdover from ENEL 351 code
void delay(void)
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
	
	//Set the baud rate to to 115200
	//calculated out this first value for it, 19.5 with Mantissa: 1011 and Frac: 0.5
	//USART2->BRR = 0x138;
	//But it was probably wrong,
	//and also Trevor showed a much less confusing way to set those bits
	USART2 -> BRR = (8 << 0) | (19 << 4);
	

}

//still unimplemented
void serial_close(void)
{

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

