//ENSE 452 Lab Project
// Built from TESTRTOS again

#include <stdio.h>
//#include "stm32F103RB.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "CLI.h"
#include "queue.h"
#include "task.h"

#define mainBLINKY_TASK_PRIORITY 		(tskIDLE_PRIORITY + 1 )
#define mainCLIPDISPLAY_TASK_PRIORITY (tskIDLE_PRIORITY + 2)

static void vBlinkTask( void * parameters);

static void vPaperClipDisplayTask(void * parameters);


QueueHandle_t xCLI_Queue;
QueueHandle_t xFreq_Queue;

QueueHandle_t xClip_Queue;

int main(void)
{
	//serial_open();
	RCC->APB2ENR |= (1u<<2) | (1u<<4) ;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //enable USART2 clock
	GPIOA->CRL &= ~(1u<<22) &~ (1u<<23) &~(1u<<10) &~ (1u<<11);
	GPIOA->CRL |=  (1u<<20) |  (1u<<21) | (3<<8) | (2<<10);
	GPIOA->CRL &= ~(1u<<22) &~ (1u<<23) &~(4<11);
	
	AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;
	USART2->BRR = (8<<0) | (19<<4);  //hopefully baud 115200
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	USART2->CR2 |= USART_CR2_CLKEN;
	
	
	
	//Onboard Button -> PC/13
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; 
	
	
	
	
	
	


	/*
	GPIOA->ODR |= GPIO_ODR_ODR5;

	while( (GPIOC->IDR & GPIO_IDR_IDR13) != 0 ) 
	{
	
	}

	GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;
	*/


	
	
	while(1)
	{
	
	}
	
	
	//xFreq_Queue = xQueueCreate(1, sizeof(uint32_t));	
	
	//xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE, NULL, mainBLINKY_TASK_PRIORITY, NULL);
	
	//xTaskCreate(vPaperClipDisplayTask, "ClipDisplay", configMINIMAL_STACK_SIZE, NULL, mainBLINKY_TASK_PRIORITY, NULL);
	
	
	//for some reason, it doesn't like to print N? wait, maybe it's just that index?
	//uint8_t title_msg[] = "ENSE 452 Lab Project \r\n";
	//CLI_Transmit(title_msg, (sizeof(title_msg) / sizeof(uint8_t)));
		

	
	/* Start the scheduler. */
	//vTaskStartScheduler();
	
	
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

static void vPaperClipDisplayTask( void * parameters)
{
	int paperclips = 0;
	int clipCharSize = 8;
	uint8_t clipCharArray[clipCharSize]; 
	for (;;)
	{
		vTaskDelay(1000);
		paperclips++;
		sprintf(clipCharArray, "%d", paperclips);
		
		/*
		for(unsigned int i = 0; i < clipCharSize; i++)
		{
			if(clipCharArray[i] == '¥')
			{
				
			}
		}*/
		CLI_Transmit(clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\r');
		sendbyte('\n');
		vTaskDelay(1000);
		
		
		
		
	}
	
	
}
