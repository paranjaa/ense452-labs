#include <stdio.h>
//#include "stm32F103RB.h"
#include "FreeRTOS.h"
#include "task.h"

#define mainBLINKY_TASK_PRIORITY 		(tskIDLE_PRIORITY + 1 )

static void vBlinkTask( void * parameters);

int main(void)
{
	RCC->APB2ENR |= (1u<<2) | (1u<<4) ;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //enable USART2 clock
	GPIOA->CRL &= ~(1u<<22) &~ (1u<<23) &~(1u<<10) &~ (1u<<11);
	GPIOA->CRL |=  (1u<<20) |  (1u<<21) | (3<<8) | (2<<10);
	//GPIOA->CRL &= ~(1u<<22) &~ (1u<<23) &~(4<11);
	
	//AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;
	USART2->BRR = (8<<0) | (19<<4);  //hopefully baud 115200
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	USART2->CR2 |= USART_CR2_CLKEN;
	
	xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE, NULL, mainBLINKY_TASK_PRIORITY, NULL);
	
	/* Start the scheduler. */
	vTaskStartScheduler();
	
	
	return 0;
}


static void vBlinkTask( void * parameters)
{
	for (;;)
	{
	
		GPIOA->ODR |= (1u<<5);	
		USART2->DR = 0x60;
		vTaskDelay(1000);
		
		GPIOA->ODR &= ~(1u<<5);
		
		vTaskDelay(1500);
		
		
	}
	
	
}
