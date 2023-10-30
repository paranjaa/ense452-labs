/*
ENSE 452 Lab 4
Alok Paranjape
200246370
October 29th
Carryover from 351, has a delay function and some examples of startup code
Realized I didn't have a comment here before
*/

 #include "stm32f10x.h"
 #include "util.h"
 

//**************************** Utility ************************************************************
// delay = 1800 is approximately 1 ms @ SYSCLK = 24 MHz (ymmv)

void delay(uint32_t delay)
{
  	 while (delay--)
	{
		}
}

//**************************** Clock Configuration ************************************************************
void clockInit(void)
{
  
//* enable HSI and wait for it to be ready

		RCC->CR |= RCC_CR_HSION;
    while (((RCC->CR) & (RCC_CR_HSION | RCC_CR_HSIRDY)) == 0);
			
//* enable HSE with Bypass and wait for it to be ready

		RCC->CR |= RCC_CR_HSEON | RCC_CR_HSEBYP;
    while (((RCC->CR) & (RCC_CR_HSEON | RCC_CR_HSEBYP | RCC_CR_HSERDY)) == 0);
				
    
////SET HSI as SYSCLK and wait for it to be recognized

//		RCC->CFGR = RCC_CFGR_SW_HSI;
//    while (((RCC->CFGR) & (RCC_CFGR_SW_HSI | RCC_CFGR_SWS_HSI)) != 0);
			
//SET HSE as SYSCLK and wait for it to be recognized

		RCC->CFGR = RCC_CFGR_SW_HSE;
    while (((RCC->CFGR) & (RCC_CFGR_SW_HSE | RCC_CFGR_SWS_HSE)) == 0);

//****************To Use PLL as SYSCLK	
// Disable PLL. 
		
		RCC->CR &= ~RCC_CR_PLLON;
			
//Change PLL source and set the PLL multiplier
			
//These are the SYSCLK values when using the PLL with HSI/2 as the input. The max value is 64 MHz
		//RCC->CFGR = 0x00000000;// 8MHz
		//RCC->CFGR = 0x00040000;// 12 MHz
		//RCC->CFGR = 0x00080000;// 16 MHz
		//RCC->CFGR = 0x000c0000;// 20 MHz			
		//RCC->CFGR = 0x00100000;// 24 MHz
		//RCC->CFGR = 0x00140000;//	28 MHz
		//RCC->CFGR = 0x00180000;// 32 MHz
		//RCC->CFGR = 0x001C0000;// 36 MHz			
		//RCC->CFGR = 0x00200000;// 40 MHz
		//RCC->CFGR = 0x00240000;//	44 MHz
		//RCC->CFGR = 0x00280000;// 48 MHz
		//RCC->CFGR = 0x002C0000;// 52 MHz
		//RCC->CFGR = 0x003C0000;// 64 MHz			

//These are the SYSCLK values when using the PLL with HSE/Bypass as the input. The max value is 72 MHz
		//RCC->CFGR = 0x00010000;// 16 MHz
		RCC->CFGR = 0x00050000;// 24 MHz 
		//RCC->CFGR = 0x00090000;// 32 MHz
		//RCC->CFGR = 0x000d0000;// 40 MHz			
		//RCC->CFGR = 0x00110000;// 48 MHz
		//RCC->CFGR = 0x00150000;//	56 MHz
		//RCC->CFGR = 0x00190000;// 64 MHz
		//RCC->CFGR = 0x001d0000;// 72 MHz		


//ENABLE PLL and wait for it to be ready

    RCC->CR |= RCC_CR_PLLON;
    while (((RCC->CR) & (RCC_CR_PLLON | RCC_CR_PLLRDY)) == 0);
   
// Set PLL as SYSCLK and wait for it to be ready
			
    RCC->CFGR |= RCC_CFGR_SW_PLL;// 0x00000002;
    while (((RCC->CFGR) & (RCC_CFGR_SW_PLL | RCC_CFGR_SWS_PLL)) == 0);
			


// Enable clock visibity on PA8 and select desired source (By default, the output will be the PLL frequency divided by 2)
			
		//RCC->CFGR |= RCC_CFGR_MCO_HSI;
		//RCC->CFGR |= RCC_CFGR_MCO_HSE;
		//RCC->CFGR |= RCC_CFGR_MCO_SYSCLK;
//		RCC->CFGR |= RCC_CFGR_MCO_PLL;

	
//Enable Port A and AFIO clocks
			
 //   RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;

// Configure PA8 as AFIO ( Write 1011b into the configuration and mode bits )
//    GPIOA->CRH |= GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8_1 | GPIO_CRH_MODE8_0 ;
 //   GPIOA->CRH &= ~GPIO_CRH_CNF8_0 ;

}

//**************************** I/O ************************************************************

void led_IO_init (void)
{
    //Enable Port A and Port C clocks
    
    RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN ;
	
		
	//****************NUCLEO-64*******************************************************************
//    //Set the config and mode bits for Port A bit 5 so it will be a push-pull output (up to 50 MHz)
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL &= ~GPIO_CRL_CNF5;
	//****************NUCLEO-64*******************************************************************
	
}	
	
void sw2led(void)
{
	
  	// Read the USER button and control the GREEN LED pattern according to its state
  
	
	if ((GPIOC->IDR & GPIO_IDR_IDR13) == 0)
		
  {
     GPIOA->BSRR = NUC_GREEN_ON;
  }
  else
  {
     GPIOA->BSRR = NUC_GREEN_OFF;
  }
}
	
	
void led_flash(void)
{			
	    while(1){
			GPIOA->BSRR = NUC_GREEN_ON;
			delay(1800);
	    GPIOA->BSRR = NUC_GREEN_OFF;
			delay(1800);
			}
}

