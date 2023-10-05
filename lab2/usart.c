#include "usart.h"
#include "stm32f10x.h"

/** Configure and enable the device. */
void serial_open(void)
{
	//Ensure the the Port A clock is enabled.
	RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN; 
  //Ensure the USART 2 clock is enabled.
	RCC->APB1ENR |=  RCC_APB1ENR_USART2EN; 
  //Configure PA2 for alternate function output Push-pull mode, max speed 50 MHz.
	GPIOA->CRL |=  GPIO_CRL_MODE2_0 |  GPIO_CRL_MODE2_1; //output 50Mhz
	GPIOA->CRL |= GPIO_CRL_CNF2_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF2_0;
  //Configure PA3 for Input with pull-up / pull-down.
	//10: Input with pull-up / pull-down, so set bit 1, clear bit 0
	GPIOA->CRL |=  GPIO_CRL_MODE3_1;
	GPIOA->CRL &= ~GPIO_CRL_MODE3_0;

  //Enable the USART Tx and Rx in the USART Control register.
	USART2->CR1 |= USART_CR1_TE; //are these the right bits?
	USART2->CR1 |= USART_CR1_RE;
	
	
  //Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz).
	RCC->CFGR = 0x001C0000;// 36 MHz	//is this the peripheral clock? seems like it's based on the system clock
	
	USART2 -> CR1 &= ~0x1000; //clear the 12th bit so it's set to Start bit, 8 Data bits, n Stop bit
	
	
	//Write BRRR (it's a specific value, 9600)
	//USARTDIV = 36 000 000 / 
	USART2->BRR = 0x9C4;
	
	

}

void serial_close(void)
{
	//undo all that stuff above?
}

int sendbyte(uint8_t b)
{
	//get the CR1 register, then mask it
	volatile unsigned int USART_TXE_checker = USART1->SR;
	USART_TXE_checker &= USART_SR_TXE;
	//TXE is in bit 7, move it over to position 0 for easy checking
	USART_TXE_checker = USART_TXE_checker >> 7;
	
	while(USART_TXE_checker != 1)
	{
		//continue checking the TXE
		USART_TXE_checker = USART1->SR;
		USART_TXE_checker &= USART_SR_TXE;
		USART_TXE_checker = USART_TXE_checker >> 7;

	}
	
	USART1->DR = b;
	return 0;
}	

uint8_t getbyte(void)
{
	//get the SR register, mask it
	//RXNE is in position 5, move it so it's easy to check
	uint8_t value;
	volatile unsigned int USART_RXNE_checker = USART1->SR;
	USART_RXNE_checker &= USART_SR_RXNE;
	USART_RXNE_checker = USART_RXNE_checker >> 5;
	
	//if it's ready, then copy the value from the register
	if(USART_RXNE_checker == 1)
	{
		value = USART1 ->DR;
	}
	return value;
}
