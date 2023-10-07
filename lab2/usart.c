#include "usart.h"
#include "stm32f10x.h"


//delay for about a second
void delay()
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
	
	//ne
	
	USART2->CR1 |= USART_CR1_UE;
  //Enable the USART Tx and Rx in the USART Control register.
	USART2->CR1 |= USART_CR1_TE; //are these the right bits?
	USART2->CR1 |= USART_CR1_RE;
	USART1->CR1 |= USART_CR1_UE;
	//Actually supposed to be 115200
  //Configure USART 2 for 9600 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz).
	
	//RCC->CFGR = 0x001C0000;// 36 MHz	//is this the peripheral clock? seems like it's based on the system clock
	USART2 -> CR1 &= ~0x1000; //clear the 12th bit so it's set to Start bit, 8 Data bits, n Stop bit
	//Write BRRR (for now it's 9600, but should be 115200)
	//USARTDIV = 36 000 000 / 
	//USART2->BRR = 0x9C4;
	
	//calculated a new value for it
	//USART2->BRR = 0x138;
	USART2 -> BRR = (8 << 0) | (19 << 4);
	// 24 Mhz / 16 * USART_DIV = 

}

void serial_close(void)
{
	//undo all that stuff above?
}

int sendbyte(uint8_t b)
{
	//get the CR1 register, then mask it
	volatile unsigned int USART_TXE_checker = USART2->SR;
	USART_TXE_checker &= USART_SR_TXE;
	//TXE is in bit 7, move it over to position 0 for easy checking
	USART_TXE_checker = USART_TXE_checker >> 7;
	
	while(USART_TXE_checker != 1)
	{
		//continue checking the TXE
		USART_TXE_checker = USART2->SR;
		USART_TXE_checker &= USART_SR_TXE;
		USART_TXE_checker = USART_TXE_checker >> 7;

	}
	
	USART2->DR = b;
	return 0;
	
	//maybe add a failure case here? Like, a return 1?
}	

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
	return value;
}

//
void echoCharacter(void)
{
	uint8_t char1 = getbyte();
	if(char1 == '\r')
	{
		sendbyte('\r');
		sendbyte('\n');
	}
	else{
		sendbyte(char1);
	}
	
}



//adapted from ENSE 352 lab, mostly used as a reference

void sendData()
{
	
		//supposed to be ASCII characters beginning with !
		//	and ending with ~(Hex values $21 through $7E)
		char X = 0x21;
		while(X != 0x7F)
		{
			//wait ~1 second, so it doesn't spawm all a once
			//delay();

			//get the CR1 register, then mask it
			volatile unsigned int USART_TXE_checker = USART2->SR;
			USART_TXE_checker &= USART_SR_TXE;
			//TXE is in bit 7, move it over to position 0 for easy checking
			USART_TXE_checker = USART_TXE_checker >> 7;
	
			// !!!
			//might want incorporate recieveData in here so that it doesn't
			//do all the characters before checking for a button press
			while(USART_TXE_checker != 1)
			{
				//continue checking the TXE
				USART_TXE_checker = USART2->SR;
				USART_TXE_checker &= USART_SR_TXE;
				USART_TXE_checker = USART_TXE_checker >> 7;

			}
		
			//when it's ready, send it
			USART2->DR = X;
			//increment the value to send, hopefully it gets to ~ eventually
			X++;


	}
}



void recieveData(void)
    {
		//get the SR register, mask it
		volatile unsigned int USART_RXNE_checker = USART2->SR;
		USART_RXNE_checker &= USART_SR_RXNE;
		//RXNE is in position 5, move it so it's easy to check
		char Y;
		USART_RXNE_checker = USART_RXNE_checker >> 5;
		if(USART_RXNE_checker == 1)
		{
			//if it is ready, then copy the value to Y
			Y = USART2 ->DR;
			//picked y and n as the values for turning the LED on and off
			if(Y == 'y')
			{
				GPIOA->ODR |= GPIO_ODR_ODR5;
			}
			if(Y == 'n')
			{
				GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;
			}
			//if it's neither of those, exit the function
			else
			{
			
			}
		
		}
		//if it isn't ready, exit the function
		else
		{
			return;
		}
	

}



