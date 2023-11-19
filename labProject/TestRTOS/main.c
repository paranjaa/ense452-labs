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
#define mainCLIPSELL_TASK_PRIORITY (tskIDLE_PRIORITY + 3)

static void vBlinkTask( void * parameters);

static void vPaperClipDisplayTask(void * parameters);

static void vPaperClipSellTask(void * parameters);

QueueHandle_t xFreq_Queue;
QueueHandle_t xCLI_Queue;

QueueHandle_t xClip_Queue;
QueueHandle_t xSell_Queue;

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
	
	
	//startupCheck(); 
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
	
	

	
	GPIOA->ODR |= (1u<<5);

	
	xClip_Queue = xQueueCreate(1, sizeof(uint32_t));	
	
	xSell_Queue = xQueueCreate(1, sizeof(uint32_t));	
	
	//xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE, NULL, mainBLINKY_TASK_PRIORITY, NULL);
	
	xTaskCreate(vPaperClipDisplayTask, "ClipDisplay", configMINIMAL_STACK_SIZE, NULL, mainCLIPDISPLAY_TASK_PRIORITY, NULL);
	
	xTaskCreate(vPaperClipSellTask, "SellClips", configMINIMAL_STACK_SIZE, NULL, mainCLIPDISPLAY_TASK_PRIORITY, NULL);
	
	

	
	
	//print out a title message
	uint8_t title_msg[] = "ENSE 452 Lab Project \n\r";
	CLI_Transmit(title_msg, (sizeof(title_msg) / sizeof(uint8_t)));
	
	


	
	/* Start the scheduler. */
	vTaskStartScheduler();
	
	while(1)
	{
	
	}
	
	
	return 0;
}


static void vBlinkTask( void * parameters)
{
	for (;;)
	{
	
		GPIOA->ODR |= (1u<<5);	
		//USART2->DR = 0x60;
		vTaskDelay(1000);
		
		GPIOA->ODR &= ~(1u<<5);
		
		vTaskDelay(1500);
		
		
	}
	
	
}


static void vPaperClipDisplayTask( void * parameters)
{
	uint8_t paperclips = 0;
	uint8_t money = 0;
	
	uint8_t newClips = 0;
	uint8_t sellClips = 0;
	uint8_t clipCharSize = 8;
	uint8_t clipCharArray[clipCharSize]; 
	for (;;)
	{
		//paperclips = 0;
		
		uint8_t top_ANSI[] = "\x1b[2;0H";
		CLI_Transmit(top_ANSI, (sizeof(top_ANSI) / sizeof(uint8_t)));
		
		//BaseType_t status = xQueueReceive(xClip_Queue, &paperclips, 100); 	
		
		if( uxQueueMessagesWaiting( xClip_Queue ) != 0 )		
		{			

			//replace the new clips value with it
			xQueueReceive(xClip_Queue, &newClips, NULL);
			paperclips = paperclips + newClips;

	
		}
		

		
		
		if( uxQueueMessagesWaiting( xSell_Queue ) != 0 )		
		{			

			//replace the clips value with it
			xQueueReceive(xSell_Queue, &sellClips, NULL);
			
			if( (paperclips - sellClips) > 0 )
			{
				paperclips = paperclips - sellClips;
				sellClips = 0;
				money = money + 1;
			
			}
		}
		
		uint8_t clip_msg[] = "Paperclips: ";
		CLI_Transmit(clip_msg, (sizeof(clip_msg) / sizeof(uint8_t)));
		
		sprintf(clipCharArray, "%d", paperclips);
		CLI_Transmit(clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
		
		uint8_t money_msg[] = "Money: ";
		CLI_Transmit(money_msg, (sizeof(money_msg) / sizeof(uint8_t)));
		
			
		sprintf(clipCharArray, "%d", money);
		CLI_Transmit(clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('$');
		sendbyte('\n');
		sendbyte('\r');
	

			
		vTaskDelay(500);
		}
}

	



static void vPaperClipSellTask( void * parameters)
{
	int sellRate = 0;
	int sellFactor = 5;
	
	for (;;)
	{
		sellRate = sellRate + 1;
		
		if(sellRate > sellFactor)
		{
			int newClips = 1;
			xQueueSendToFrontFromISR( xSell_Queue, &newClips, NULL);			
			sellRate = 0;
		
		}
		
		vTaskDelay(1000);
	}
	
	
}


		//position the cursor so it's at the start of the screen (at the top left)
	//uint8_t top_ANSI[] = "\x1b[0;0H";
	//CLI_Transmit(top_ANSI, (sizeof(top_ANSI) / sizeof(uint8_t)));
	//also print a status for the number of paperclips
	//uint8_t status_msg[] = "\r\nPaperclips: 0";
	//CLI_Transmit(status_msg, (sizeof(status_msg) / sizeof(uint8_t)));
	
	//make a scrollable window with the top at line 3
	//think leaving it blank makes it go to the base of the current view?
	//uint8_t mid_ANSI[] = "\x1b[3;r";
	//CLI_Transmit(mid_ANSI, (sizeof(mid_ANSI) / sizeof(uint8_t)));

