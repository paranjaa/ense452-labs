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
	USART1->CR1 |= USART_CR1_TE; //are these the right bits?
	USART1->CR1 |= USART_CR1_RE;
	
	
  //Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz).
	RCC->CFGR = 0x001C0000;// 36 MHz	//is this the peripheral clock? seems like it's based on the system clock
	

}

void serial_close(void)
{
	//undo all that stuff above?
}

int sendbyte(uint8_t b)
{
	return 0;
}	

uint8_t getbyte(void)
{
	return 0;
}
