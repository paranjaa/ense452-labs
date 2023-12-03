/*
ENSE 452 Lab Project
Alok Paranjape
200246370
December 3rd

The function definitions for each task (display/calculation, CLI, sales,
autoclippers), most of the project code is in here

*/


#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "CLI.h"
#include "queue.h"
#include "task.h"
#include "util.h"
#include "rtos_tasks.h"

//opted to put these here rather than in the .h, compiler kept complaining

//used to store user input from the USART interrupt
//gets used by the CLI task
 QueueHandle_t xCLI_Queue;
 
 //used to store requests for clips being made
 //from both the button and autoclippers
 //results handled by the display before printing, like the rest of these below
 QueueHandle_t xClip_Queue;
 
 //used to store requests for clips being sold from the selling task
 //checked before displaying and making the change
 QueueHandle_t xSell_Queue;
 
 //used to store requests to purchase additional wire from the CLI command
 QueueHandle_t xWire_Queue;
 
 //used to store requests to the display about buying new autoclippers 
 //from the CLI
 QueueHandle_t xClipper_Queue;
 //used by the display to notify autoclipper task that new ones have been bought
 //also from the CLI
 QueueHandle_t xClipper_msg_Queue;
 //used to store requests to change the price,
// gets checked with display task first
 QueueHandle_t xPrice_Queue;
 
 //used to store requests from the display task 
 //to tell the selling task to update the sell rate
 QueueHandle_t xSell_Rate_Queue;




void createQueues(void)
{
	//create each of the queues, was having issues with the size before
	//so made some of them larger than 1,
	//probably depends on how fast input comes i
	xCLI_Queue = xQueueCreate(1, sizeof(uint8_t));
	
	//clips are being bought and sold more quickly, so need more space
	xClip_Queue = xQueueCreate(8, sizeof(uint8_t));	
	xSell_Queue = xQueueCreate(8, sizeof(uint8_t));	
	xWire_Queue = xQueueCreate(1, sizeof(uint8_t));
	xClipper_msg_Queue = xQueueCreate(1, sizeof(uint8_t));
	xClipper_Queue = xQueueCreate(1, sizeof(uint8_t));
	xPrice_Queue = xQueueCreate(1, sizeof(uint8_t));
	xSell_Rate_Queue = xQueueCreate(1, sizeof(uint8_t));
	


}

void createTasks(void)
{
	//create each one of the tasks with their assigned priorities
	xTaskCreate(vCLI_Task, "CLI", configMINIMAL_STACK_SIZE, NULL, mainCLI_TASK_PRIORITY, NULL);
	xTaskCreate(vPaperClipDisplayTask, "ClipDisplay", configMINIMAL_STACK_SIZE, NULL, mainCLIPDISPLAY_TASK_PRIORITY, NULL);
	xTaskCreate(vPaperClipSellTask, "SellClips", configMINIMAL_STACK_SIZE, NULL, mainCLIPSELL_TASK_PRIORITY, NULL);
	xTaskCreate(vAutoClippersTask, "AutoClippers", configMINIMAL_STACK_SIZE, NULL, mainAutoClipper_TASK_PRIORITY, NULL);
}






static void vPaperClipDisplayTask( void * parameters)
{
	//store the starting values for all stored/printed information
	uint8_t paperclips = 0;
	uint8_t money = 0;
	uint8_t wire = 20;
	uint8_t autoclippers = 0;
	uint8_t price = 3;
	
	//and make variables for reading from its relevant queues to add on to it
	uint8_t newClips = 0;
	uint8_t sellClips = 0;
	uint8_t newWire = 0;
	uint8_t newClippers = 0;
	uint8_t newPrice = 0;
	
	//make an array for storing/displaying each one of those values
	uint8_t clipCharSize = 16;
	char clipCharArray[clipCharSize]; 
	for (;;)
	{
		//save the cursor's current location in the scrollable window
		uint8_t save_cursor_ANSI[] = "\x1b[s";
		CLI_Transmit(save_cursor_ANSI, (sizeof(save_cursor_ANSI) / sizeof(uint8_t)));
				
		//move it to first bar, the paperclips
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
				//increment the number of paperclips with the new value
				paperclips = paperclips + newClips;
				//and decrement the amount of wire
				wire = wire - newClips;
			}
	
		}
		
		//if there's updates about clips being sold
		if( uxQueueMessagesWaiting( xSell_Queue ) != 0 )		
		{			

			//replace the clips value with it
			xQueueReceive(xSell_Queue, &sellClips, NULL);
			//check if there's paperclips in inventory to sell
			if( (paperclips - sellClips) >= 0 )
			{
				//sell the requested amount of paperclips and update the funds
				paperclips = paperclips - sellClips;
				money = money + (price*sellClips);
			
			}
		}
		
		//if theres updates about buying new wire
		if( uxQueueMessagesWaiting( xWire_Queue ) != 0 )		
		{			
			
			//get the wire value from the queue
			xQueueReceive(xWire_Queue, &newWire, NULL);
			//if there's enough funds for it
			if( (money - 14) >= 0) 
			{
				//add onto the wire tally (could probably use a variable for this)
				wire = wire + 10;
				//deduct from available funds
				money = money - 14;
				
			
			}
		}
		
		//if there are messages waiting for new autoclippers
		if( uxQueueMessagesWaiting( xClipper_msg_Queue ) != 0 )		
		{			
			
			//get the value from the queue
			xQueueReceive(xClipper_msg_Queue, &newClippers, NULL);
			
			
			//if there's enough funding to buy an autoclipper
			if( (money - 40) >= 0)
			{
				//subtract it from the total
				money = money - 40;
				//send a value to the autoclipper task so it can start production
				newClippers = 1;
				xQueueSendToBack(xClipper_Queue, &newClippers, NULL);
				//increment the displayed amount of autoclippers as well
				autoclippers = autoclippers+1;
			}
	
		}
		
		
		//if there are requests to change the price
		if(uxQueueMessagesWaiting(xPrice_Queue) != 0)
		{
			//get the value from the queue
			xQueueReceive(xPrice_Queue, &newPrice, NULL);
			
			//if they put in to lower it (with 0), try to lower the price
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
			//same deal for if they asked to increase the price
			if(newPrice == 1)
			{
				//if the price can go up (to 11 at most), increment it
				if(price < 11)
				{
					price = price + 1;
					//tell the sale function to update its rate of sale
					uint8_t update_sale_rate = 1;
					xQueueSendToBack(xSell_Rate_Queue, &update_sale_rate, NULL);
		
				}
			
			}
			
		}
		uint8_t ANSI_clear[] = "\x1b[K";
		CLI_Transmit(ANSI_clear, (sizeof(ANSI_clear) / sizeof(uint8_t)));
		
		//with all that information from each task
		
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
	
		CLI_Transmit(
		(uint8_t*)clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
		
		CLI_Transmit(ANSI_clear, (sizeof(ANSI_clear) / sizeof(uint8_t)));
		//print the label for the amount of money
		uint8_t money_msg[] = "Money: ";
		CLI_Transmit(money_msg, (sizeof(money_msg) / sizeof(uint8_t)));
		
		//get the amount of money
		sprintf(clipCharArray, "%d", money);
		
		//print it, then a new line
		CLI_Transmit(
		(uint8_t*)clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
		
		CLI_Transmit(ANSI_clear, (sizeof(ANSI_clear) / sizeof(uint8_t)));
		//do the same for every other printable label and its value
		uint8_t wire_msg[] = "Wire (14$): ";
		CLI_Transmit(wire_msg, (sizeof(wire_msg) / sizeof(uint8_t)));
		sprintf(clipCharArray, "%d", wire);
		CLI_Transmit(
		(uint8_t*)clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
		
		CLI_Transmit(ANSI_clear, (sizeof(ANSI_clear) / sizeof(uint8_t)));
		uint8_t clipper_msg[] = "Autoclippers (40$): ";
		CLI_Transmit(clipper_msg, (sizeof(clipper_msg) / sizeof(uint8_t)));
		
		sprintf(clipCharArray, "%d", autoclippers);
		CLI_Transmit(
		(uint8_t*)clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
		
		CLI_Transmit(ANSI_clear, (sizeof(ANSI_clear) / sizeof(uint8_t)));
		uint8_t price_msg[] = "Price($): ";
		CLI_Transmit(price_msg, (sizeof(price_msg) / sizeof(uint8_t)));
		
		
		sprintf(clipCharArray, "%d", price);
		CLI_Transmit((
		uint8_t*)clipCharArray, (sizeof(clipCharArray) / sizeof(uint8_t)));
		sendbyte('\n');
		sendbyte('\r');
		
	
		//put the cursor back outside the scroll view
		uint8_t mid_ANSI[] = "\x1b[8;r";
		CLI_Transmit(mid_ANSI, (sizeof(mid_ANSI) / sizeof(uint8_t)));
	
		//then put it where it was saved before
		uint8_t return_cursor_ANSI[] = "\x1b[u";
		CLI_Transmit(
		return_cursor_ANSI, (sizeof(return_cursor_ANSI) / sizeof(uint8_t)));
		
		//then delay updating the display/calculating for 0.1 seconds
		//since it needs to run pretty frequently
		vTaskDelay(100);
		}
}

	



static void vPaperClipSellTask( void * parameters)
{
	//using a sell rate to try selling the clips rapidly
	//but only actually sell them every few times
	//figured this would be better than changing the delay while running
	//probably a nicer way to do it though
	uint8_t sellRate = 0;
	uint8_t sellFactor = 3;
	uint8_t new_factor = 0;
	
	for (;;)
	{
		//if there are requests (verified from the display)
		if( uxQueueMessagesWaiting( xSell_Rate_Queue ) != 0 )	
		{
			//get the value from its queue
			xQueueReceive(xSell_Rate_Queue, &new_factor, NULL);

			//if it's 1, 
			if(new_factor == 1)
			{
				//then increment the sell factor so it takes one run longer to sell
				sellFactor = sellFactor + 1;
				//reset the rate so it starts trying again with the new threshold
				sellRate = 0;

				
			}
			//same deal if they try and lower the price
			if(new_factor == 0)
			{
				sellFactor = sellFactor - 1;
				sellRate = 0;
			
			}
		
		}
		else
		{
			//increment the sell rate to try and sell a clip, 
			sellRate = sellRate + 1;
			if(sellRate > sellFactor)
			{
				//if it's high enough hit the sell factor
				uint8_t sellClips = 1;
				//go tell the display to try selling a clip
				xQueueSendToBack( xSell_Queue, &sellClips, NULL);			
				//then set it back to 0 to start trying again
				sellRate = 0;
			
			}
			

		
		}
		
		//try again a half second later
		//each change to the sell factor should change how quickly it tries by half a second
		//probably enough to be noticiable, but not monotonous
		vTaskDelay(500);
	}
	
	
}


static void vCLI_Task(void * parameters)
{
	
	uint8_t newChar;
	
	for (;;)
	{
		//after getting a message from the usart input
		if( uxQueueMessagesWaiting( xCLI_Queue ) != 0 )		
		{			
			//replace the get the new character value with it
			xQueueReceive(xCLI_Queue, &newChar, NULL);
			//then call cli_recieve to add it to the array and operate on it
			CLI_Receive(&newChar, 1);
		}

		
		
		
	}
	
}

static void vAutoClippersTask(void * parameters)
{
	//start the number clippers at zero, since they need to be bought firsst
	uint8_t auto_clippers = 0;
	uint8_t new_clippers = 0;
	uint8_t new_clips = 0;
	for(;;)
	{
		//if there's a message from the display task confirming one has been bought
		if( uxQueueMessagesWaiting( xClipper_Queue ) != 0 )		
		{			
			
			//get the value from the queue
			xQueueReceive(xClipper_Queue, &new_clippers, NULL);
			//add onto the total amount of clippers
			auto_clippers = auto_clippers + 1;
			new_clippers = 0;
			//update the amount the task produces each time the autoclippers run
			new_clips = auto_clippers*1;
	
		}
		//if there are clips being made
		if(new_clips > 0)
		{
			//send them back to the display to tally
			xQueueSendToBack( xClip_Queue, &new_clips, NULL);
		}
		//only run this every second or so, that way clicking the button isn't 
		//immediately obsolete
		vTaskDelay(1000);
	}
	

}





