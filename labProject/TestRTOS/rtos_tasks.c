#include <stdio.h>
//#include "stm32F103RB.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "CLI.h"
#include "queue.h"
#include "task.h"
#include "util.h"
#include "rtos_tasks.h"



 QueueHandle_t xCLI_Queue;
 QueueHandle_t xClip_Queue;
 QueueHandle_t xSell_Queue;
 QueueHandle_t xWire_Queue;
 QueueHandle_t xClipper_Queue;
 QueueHandle_t xClipper_msg_Queue;
 QueueHandle_t xPrice_Queue;
 QueueHandle_t xSell_Rate_Queue;




void createQueues(void)
{
	xCLI_Queue = xQueueCreate(1, sizeof(uint8_t));
	xClip_Queue = xQueueCreate(8, sizeof(uint8_t));	
	xSell_Queue = xQueueCreate(8, sizeof(uint8_t));	
	xWire_Queue = xQueueCreate(1, sizeof(uint8_t));
	xClipper_msg_Queue = xQueueCreate(1, sizeof(uint8_t));
	xClipper_Queue = xQueueCreate(1, sizeof(uint8_t));
	xPrice_Queue = xQueueCreate(1, sizeof(uint8_t));
	xSell_Rate_Queue = xQueueCreate(4, sizeof(uint8_t));
	


}

void createTasks(void)
{
	xTaskCreate(vCLI_Task, "CLI", configMINIMAL_STACK_SIZE, NULL, mainCLI_TASK_PRIORITY, NULL);
	xTaskCreate(vPaperClipDisplayTask, "ClipDisplay", configMINIMAL_STACK_SIZE, NULL, mainCLIPDISPLAY_TASK_PRIORITY, NULL);
	xTaskCreate(vPaperClipSellTask, "SellClips", configMINIMAL_STACK_SIZE, NULL, mainCLIPSELL_TASK_PRIORITY, NULL);
	xTaskCreate(vAutoClippersTask, "AutoClippers", configMINIMAL_STACK_SIZE, NULL, mainAutoClipper_TASK_PRIORITY, NULL);
}






static void vPaperClipDisplayTask( void * parameters)
{
	uint8_t paperclips = 0;
	uint8_t money = 0;
	uint8_t wire = 20;
	uint8_t autoclippers = 0;
	uint8_t price = 3;
	
	uint8_t newClips = 0;
	uint8_t sellClips = 0;
	uint8_t newWire = 0;
	uint8_t newClippers = 0;
	uint8_t newPrice = 0;
	
	uint8_t clipCharSize = 16;
	char clipCharArray[clipCharSize]; 
	for (;;)
	{
		
		uint8_t save_cursor_ANSI[] = "\x1b[s";
		CLI_Transmit(save_cursor_ANSI, (sizeof(save_cursor_ANSI) / sizeof(uint8_t)));
				
		uint8_t top_ANSI[] = "\x1b[2;0H";
		CLI_Transmit(top_ANSI, (sizeof(top_ANSI) / sizeof(uint8_t)));
		
		//when it gets updates about new clips being made
		if( uxQueueMessagesWaiting( xClip_Queue ) != 0 )		
		{			

			//replace the new clips value with it
			xQueueReceive(xClip_Queue, &newClips, NULL);
			
			//if there's enough wire in storage
			if( (wire - newClips) >= 0)
			{
				//increment the number of paperclips
				paperclips = paperclips + newClips;
				//decrement the amount of wire
				wire = wire - newClips;
			}
	
		}
		
		
		if( uxQueueMessagesWaiting( xSell_Queue ) != 0 )		
		{			

			//replace the clips clips value with it
			xQueueReceive(xSell_Queue, &sellClips, NULL);
			
			if( (paperclips - sellClips) >= 0 )
			{
				paperclips = paperclips - sellClips;
				sellClips = 0;
				money = money + price;
			
			}
		}
		
		if( uxQueueMessagesWaiting( xWire_Queue ) != 0 )		
		{			
			
			//get the wire value from the queue
			xQueueReceive(xWire_Queue, &newWire, NULL);
			//if there's enough funds for it
			if( (money - 10) >= 0) 
			{
				wire = wire + 10;
				money = money - 14;
				
			
			}
		}
		
		//if there are messages waiting for new autoclippers
		if( uxQueueMessagesWaiting( xClipper_msg_Queue ) != 0 )		
		{			
			
			//get the value from the queue
			xQueueReceive(xClipper_msg_Queue, &newClippers, NULL);
			
			
			//if there's enough funding for an autoclipper
			if( (money - 40) >= 0)
			{
				money = money - 40;
				//send that to the autoclipper task so it can start production
				newClippers = 1;
				xQueueSendToBack(xClipper_Queue, &newClippers, NULL);
				autoclippers = autoclippers+1;
			}
	
		}
		
		
		

		
		
		
		
		if(uxQueueMessagesWaiting(xPrice_Queue) != 0)
		{
			xQueueReceive(xPrice_Queue, &newPrice, NULL);
			
			//if they put in to lower it with 0, try to lower the price
			if(newPrice == 0)
			{
				//if the price can go down (not less than 1), decrement it
				if(price > 1)
				{
					price = price - 1;
					//tell the sell function to update its rate of sale to fit the new price
					uint8_t update_sale_rate = 0;
					xQueueSendToBack(xSell_Rate_Queue, &update_sale_rate, NULL);
					
				}
			}
			//
			if(newPrice == 1)
			{
				//if the price can go up (to 10 at most), increment it
				if(price < 11)
				{
					price = price + 1;
					//tell the sale function to update its rate of sale
					uint8_t update_sale_rate = 1;
					xQueueSendToBack(xSell_Rate_Queue, &update_sale_rate, NULL);
		
				}
			
			}
			

			
			
		}
		
		//reprint the label for the number of paperclips
		uint8_t clip_msg[] = "Paperclips: ";
		CLI_Transmit(clip_msg, (sizeof(clip_msg) / sizeof(uint8_t)));
		
		//get the number of paperclips with sprintf
		sprintf(clipCharArray, "%d", paperclips);
		//then clean up the array made from it
		for(uint8_t i = 0; i < clipCharSize; i++)
		{
			if( (clipCharArray[i] < '0') || (clipCharArray[i] > '9'))
			{
				clipCharArray[i] = ' ';
			
			}
		
		}
		
		//print the number of paperclips, then a new line
		CLI_Transmit((uint8_t*)clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
		
		//print the label for the amount of money
		uint8_t money_msg[] = "Money: ";
		CLI_Transmit(money_msg, (sizeof(money_msg) / sizeof(uint8_t)));
		
		//get the amount of money
		sprintf(clipCharArray, "%d", money);
		
		//print it, then a new line
		CLI_Transmit((uint8_t*)clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
		
		//print the label for the amount of wire
		uint8_t wire_msg[] = "Wire (14$): ";
		CLI_Transmit(wire_msg, (sizeof(wire_msg) / sizeof(uint8_t)));
		sprintf(clipCharArray, "%d", wire);
		CLI_Transmit((uint8_t*)clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
		
		uint8_t clipper_msg[] = "Autoclippers (40$): ";
		CLI_Transmit(clipper_msg, (sizeof(clipper_msg) / sizeof(uint8_t)));
		
		sprintf(clipCharArray, "%d", autoclippers);
		CLI_Transmit((uint8_t*)clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
		
		
		uint8_t price_msg[] = "Price($): ";
		CLI_Transmit(price_msg, (sizeof(price_msg) / sizeof(uint8_t)));
		
		
		sprintf(clipCharArray, "%d", price);
		CLI_Transmit((uint8_t*)clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
		
	
		
		uint8_t mid_ANSI[] = "\x1b[8;r";
		CLI_Transmit(mid_ANSI, (sizeof(mid_ANSI) / sizeof(uint8_t)));
	
		uint8_t return_cursor_ANSI[] = "\x1b[u";
		CLI_Transmit(return_cursor_ANSI, (sizeof(return_cursor_ANSI) / sizeof(uint8_t)));
			
		vTaskDelay(100);
		}
}

	



static void vPaperClipSellTask( void * parameters)
{
	//these need to be changeable to reflect increasing demand
	uint8_t sellRate = 0;
	uint8_t sellFactor = 3;
	uint8_t new_factor = 0;
	
	for (;;)
	{
	
		if( uxQueueMessagesWaiting( xSell_Rate_Queue ) != 0 )	
		{
			xQueueReceive(xSell_Rate_Queue, &new_factor, NULL);
			GPIOA->ODR |= GPIO_ODR_ODR5;

			
			if(new_factor == 1)
			{
				sellFactor = sellFactor + 1;
				sellRate = 0;

				
			}
			if(new_factor == 0)
			{
				sellFactor = sellFactor - 1;
				sellRate = 0;
			
			}
		
		}
		else
		{
			//increment the sell rate
			sellRate = sellRate + 1;
			if(sellRate > sellFactor)
			{
				uint8_t sellClips = 1;
				xQueueSendToBack( xSell_Queue, &sellClips, NULL);			
				sellRate = 0;
			
			}
			
			//try again a half second later
		
		}
		
		
		vTaskDelay(500);
	}
	
	
}


		
static void vCLI_Task(void * parameters)
{
	
	uint8_t newChar;
	
	for (;;)
	{
		if( uxQueueMessagesWaiting( xCLI_Queue ) != 0 )		
		{			
			//replace the new clips value with it
			xQueueReceive(xCLI_Queue, &newChar, NULL);
			
			//then do the reception on it
			//sendbyte(newChar);
			CLI_Receive(&newChar, 1);
		}

		
		
		
	}
	
}

static void vAutoClippersTask(void * parameters)
{
	uint8_t auto_clippers = 0;
	uint8_t new_clippers = 0;
	uint8_t new_clips = 0;
	for(;;)
	{
		if( uxQueueMessagesWaiting( xClipper_Queue ) != 0 )		
		{			
			
			//get the value from the queue
			xQueueReceive(xClipper_Queue, &new_clippers, NULL);
			auto_clippers = auto_clippers + 1;
			new_clippers = 0;
			//update the amount the task produces each time
			new_clips = auto_clippers*1;
	
		}
		
		if(new_clips > 0)
		{
			xQueueSendToBack( xClip_Queue, &new_clips, NULL);
			
		}
		
		
		
		vTaskDelay(1000);
	}
	

}





