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
#define mainCLI_TASK_PRIORITY (tskIDLE_PRIORITY+4)

static void vBlinkTask( void * parameters);

static void vPaperClipDisplayTask(void * parameters);

static void vPaperClipSellTask(void * parameters);

static void vCLI_Task(void * parameters);




QueueHandle_t xClip_Queue;
QueueHandle_t xSell_Queue;

QueueHandle_t xCLI_Queue;



int main(void)
{
	
		

	
	
	serial_open();
	
	/*
	RCC->APB2ENR |= (1u<<2) | (1u<<4) ;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //enable USART2 clock
	GPIOA->CRL &= ~(1u<<22) &~ (1u<<23) &~(1u<<10) &~ (1u<<11);
	GPIOA->CRL |=  (1u<<20) |  (1u<<21) | (3<<8) | (2<<10);
	GPIOA->CRL &= ~(1u<<22) &~ (1u<<23) &~(4<11);
	
	AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;
	USART2->BRR = (8<<0) | (19<<4);  //hopefully baud 115200
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	USART2->CR2 |= USART_CR2_CLKEN;
	
	//GPIOA->ODR |= (1u<<5);
	
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
	
			GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2 | GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
		GPIOA->CRL |= (GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2 | GPIO_CRL_CNF3_1);
	
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
	
	//set the USART Interrupt Enable, so it generates an interrupt when a character is recieved
	USART2->CR1 |= USART_CR1_RXNEIE;
	
	
	
	//Set the baud rate to to 115200
	//calculated out this first value for it, 19.5 with Mantissa: 1011 and Frac: 0.5
	//USART2->BRR = 0x138;
	//But it was probably wrong,
	//and also Trevor showed me a much less confusing way to set those bits
	USART2 -> BRR = (8 << 0) | (19 << 4);
	//Unmask USART2 as an interrupt source in the NVIC
	NVIC_EnableIRQ(USART2_IRQn);
	*/
	

	//startupCheck();


	

	
	//xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE, NULL, mainBLINKY_TASK_PRIORITY, NULL);
	
	//xTaskCreate(vPaperClipDisplayTask, "ClipDisplay", configMINIMAL_STACK_SIZE, NULL, mainCLIPDISPLAY_TASK_PRIORITY, NULL);
	
	//xTaskCreate(vPaperClipSellTask, "SellClips", configMINIMAL_STACK_SIZE, NULL, mainCLIPDISPLAY_TASK_PRIORITY, NULL);
	
	xTaskCreate(vCLI_Task, "CLI",configMINIMAL_STACK_SIZE, NULL, mainCLI_TASK_PRIORITY, NULL);
	
	
	//xClip_Queue = xQueueCreate(16, sizeof(uint8_t));	
	
	//xSell_Queue = xQueueCreate(16, sizeof(uint8_t));	
	
	xCLI_Queue = xQueueCreate(1, sizeof(uint8_t));
	
	//
	
	//uint8_t ANSI_clear[] = "\x1b[2J";
	//CLI_Transmit(ANSI_clear, (sizeof(ANSI_clear) / sizeof(uint8_t)));
	
	
	//print out a title message
	uint8_t title_msg[] = "ENSE 452 Lab Project \n\r";
	CLI_Transmit(title_msg, (sizeof(title_msg) / sizeof(uint8_t)));
	
	//skip printing for the working parts
	//sendbyte('\n');
	//sendbyte('\n');
	
	
	//later UI stuff
	//uint8_t UI_msg1[] = "Wire: Unimplemented\n\r";
	//CLI_Transmit(UI_msg1, (sizeof(UI_msg1) / sizeof(uint8_t)));

	
	//these are currently static, they're just the rates in the sell task
	//uint8_t UI_msg2[] = "Sell Rate: 25%\n\r";
	//CLI_Transmit(UI_msg2, (sizeof(UI_msg2) / sizeof(uint8_t)));
	
	//uint8_t UI_msg3[] = "Price: 1.00 $ \n \r";
	//CLI_Transmit(UI_msg3, (sizeof(UI_msg3) / sizeof(uint8_t)));
	
	
	
	
	
	


	
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
				
		uint8_t top_ANSI[] = "\x1b[2;0H";
		CLI_Transmit(top_ANSI, (sizeof(top_ANSI) / sizeof(uint8_t)));
		
		//when it gets updates about 
		if( uxQueueMessagesWaiting( xClip_Queue ) != 0 )		
		{			

			//replace the new clips value with it
			xQueueReceive(xClip_Queue, &newClips, NULL);
			paperclips = paperclips + newClips;

	
		}
		

		
		//should probably split updating for sold clips off into a different task
		if( uxQueueMessagesWaiting( xSell_Queue ) != 0 )		
		{			

			//replace the clips clips value with it
			xQueueReceive(xSell_Queue, &sellClips, NULL);
			
			if( (paperclips - sellClips) >= 0 )
			{
				paperclips = paperclips - sellClips;
				sellClips = 0;
				money = money + 1;
			
			}
		}
		
		uint8_t clip_msg[] = "Paperclips: ";
		CLI_Transmit(clip_msg, (sizeof(clip_msg) / sizeof(uint8_t)));
		
		sprintf(clipCharArray, "%d", paperclips);
		for(uint8_t i = 0; i < clipCharSize; i++)
		{
			if( (clipCharArray[i] < '0') || (clipCharArray[i] > '9'))
			{
				clipCharArray[i] = ' ';
			
			}
		
		}
		
		CLI_Transmit(clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
		
		uint8_t money_msg[] = "Money: ";
		CLI_Transmit(money_msg, (sizeof(money_msg) / sizeof(uint8_t)));
		
			
		sprintf(clipCharArray, "%d", money);
		
		CLI_Transmit(clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
	

			
		vTaskDelay(100);
		}
}

	



static void vPaperClipSellTask( void * parameters)
{
	//these need to be changeable to reflect increasing demand
	int sellRate = 0;
	int sellFactor = 4;
	
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


		




static void vCLI_Task(void * parameters)
{
	//uint8_t cli_msg_test[] = "setup the CLI task \n\r";
	//CLI_Transmit(cli_msg_test, (sizeof(cli_msg_test) / sizeof(uint8_t)));
	
	uint8_t newChar;
	for (;;)
	{
		if( uxQueueMessagesWaiting( xCLI_Queue ) != 0 )		
		{			
			//replace the new clips value with it
			xQueueReceive(xCLI_Queue, &newChar, NULL);
			sendbyte(newChar);
		}
		
		
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

