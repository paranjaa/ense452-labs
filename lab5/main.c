
//Starting again with the sample code, since I couldn't remake it in my own lab 5 project file

/*
ENSE 452 Lab 5
Alok Paranjape
200246370
November 9th
Project Description:
    Main task (blinks the LED at a certain rate)
    CLI task that updates the terminal and receives characters
		from the USART2 ISR from a Queue and sends to the Main task (via Queue)
		to change the frequency of the Blinky light.
    in the USART2 ISR send the data via Queues from FreeRTOS.
		
		Comments:
		It doesn't really work well at all. Am able to get characters with the ISR and queue, but not with the frequency queue
		Like, I can change the frequency from 250 to 100, but the others are weird
		Can see that it's going between tasks fine with the matching lowercases and capitals, 
		but the frequency is sometimes a totally different number, probably should ask about this
		
*/
#include <stdio.h>
//#include "stm32F103RB.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "CLI.h"
#include "queue.h"

#define mainBLINKY_TASK_PRIORITY 		(tskIDLE_PRIORITY + 1 )

//making this again for the CLI_Task
#define mainCLI_TASK_PRIORITY 			(tskIDLE_PRIORITY +2)

static void vBlinkTask( void * parameters);
static void vCLI_Task (void* parameters);

QueueHandle_t xCLI_Queue;
QueueHandle_t xFreq_Queue;


int main(void)
{
	
	//calling the setup function
	serial_open();
	
	//for seeing if it's actually running, should pulse the LED for a second
	//startupCheck();
	
	
	//replaced with my own serial open commands
	/*
	RCC->APB2ENR |= (1u<<2) | (1u<<4) ;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //enable USART2 clock
	GPIOA->CRL &= ~(1u<<22) &~ (1u<<23) &~(1u<<10) &~ (1u<<11);
	GPIOA->CRL |=  (1u<<20) |  (1u<<21) | (3<<8) | (2<<10);
	//GPIOA->CRL &= ~(1u<<22) &~ (1u<<23) &~(4<11);
	
	//AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;
	USART2->BRR = (8<<0) | (19<<4);  //hopefully baud 115200
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	USART2->CR2 |= USART_CR2_CLKEN;
	*/
	
	
	//uint8_t title_msg[] = "452 Lab 5 CLI";
	//CLI_Transmit(title_msg, (sizeof(title_msg) / sizeof(uint8_t)));
	
	//from TEST RTOS code, makes the LED blink at a certain rate
	xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE, NULL, mainBLINKY_TASK_PRIORITY, NULL);
	
	//making a new task for handling the CLI
	xTaskCreate(vCLI_Task, "CommandLineInterface", configMINIMAL_STACK_SIZE, NULL, mainCLI_TASK_PRIORITY, NULL);
	
	
	//creating a queue for storing the blink frequency, doesn't need to hold much
	xFreq_Queue = xQueueCreate(1, sizeof(uint8_t));	
	//and one for the CLI, same deal
	xCLI_Queue = xQueueCreate(3, sizeof(uint8_t));
	//
	/* Start the scheduler. */
	vTaskStartScheduler();
	
	//Just keep looking after that
	while(1)
	{
		
	}
	//return 0;
}

/**
Modification of the blinky code from RTOS test, with a way to check if there's a new value in the queue
it can detect the change, but it read it right a lot of the time
@param, just some parameters from the sample code, they aren't used, but it got upset without them
@pre must have already called serial_open(), and run the rest of the code up in main for setting up RTOS
*/


static void vBlinkTask( void * parameters)
{
	//adding in a starting frequency here
	int frequency = 250;
	for (;;)
	{
		
		//if there's a new value in the queue for it to deal with
		
		BaseType_t status = xQueueReceive(xFreq_Queue, &frequency, 100); 		
		
		if( uxQueueMessagesWaiting( xFreq_Queue ) != 0 )		
		{			
			//sendbyte('n');
			//replace the frequency value with it
			xQueueReceive(xFreq_Queue, &frequency, 100);
	
		}
		
		if(frequency == 250)
		{
			sendbyte('1');
		}
		
		if(frequency == 1000)
		{
			sendbyte('2');

		}
		
		
		if(frequency == 3000)
		{
			sendbyte('3');

		}
		
		
		if(frequency != 250 && frequency != 1000 && frequency != 3000 )
		{
			sendbyte('?');

		}
		
		if(frequency == 100)
		{
			sendbyte('%');
		
		}
			
			
	


		GPIOA->ODR |= (1u<<5);	
		//USART2->DR = 0x60;
		//vTaskDelay(1000);
		
		//turn on the LED, then wait the specific amount of ticks
		vTaskDelay(pdMS_TO_TICKS(frequency));
		
		GPIOA->ODR &= ~(1u<<5);
		//then turn it off and wait that many ticks again
		vTaskDelay(pdMS_TO_TICKS(frequency));
		//vTaskDelay(1500);
					
		
		

		
		
	}
	
}

/**
//making a new task for CLI (which just prints symbols now)
@param, just some parameters from the sample code, they aren't used, but it got upset without them
@pre must have already called serial_open(), and run the rest of the code up in main for setting up RTOS
*/
static void vCLI_Task(void * parameters)
{
	
	//for some reason, it doesn't like to print N? wait, maybe it's just that index?
	uint8_t title_msg[] = "ENSE 452 Lab 5 CLI \r\n";
	CLI_Transmit(title_msg, (sizeof(title_msg) / sizeof(uint8_t)));
		
	uint8_t msg1[] = "Wasn't able to get this CLI to work \r\n";
	CLI_Transmit(msg1, (sizeof(msg1) / sizeof(uint8_t)));
	
	uint8_t msg2[] = "It's on 250ms by default \r\n";
	CLI_Transmit(msg2, (sizeof(msg2) / sizeof(uint8_t)));
	
	uint8_t msg3[] = "Press a to keep it that way or set it back, it'll print A to confirm, then 1s \r\n";
	CLI_Transmit(msg3, (sizeof(msg3) / sizeof(uint8_t)));
	
	
	uint8_t msg4[] = "Was supposed to switch with b and c (and also return capitals) \r\n";
	CLI_Transmit(msg4, (sizeof(msg4) / sizeof(uint8_t)));
	
	uint8_t msg5[] = "but those just set it to some unknown frequency (and print a ?) \r\n";
	CLI_Transmit(msg5, (sizeof(msg5) / sizeof(uint8_t)));
	
	
	uint8_t msg6[] = "Putting anything else in will put it to 100 ms (and print a %) \r\n";
	CLI_Transmit(msg6, (sizeof(msg6) / sizeof(uint8_t)));
	
	uint8_t charReceived;
	for (;;)
	{
		//check if there's something saved in the queue from the user
		BaseType_t xStatus = xQueueReceive(xCLI_Queue, &charReceived, 100);		
		//if there is, then print it out
		if(xStatus == pdTRUE)
		{
			
			//send the byte back
			sendbyte(charReceived);
			
		
			//wasn't able to get the rest of it to work,
			//so I just made a simplified RTOS handler, which takes one character and prints
			CLI_RTOS(&charReceived, 1);
				
			
			//CLI_Receive(testCharArray, testSize);
			//quit_zero = CLI_Quit(testCharArray, testSize);
			//if(quit_zero == 0)
			//{
				//if they do, leave the loop
			//	break;
			//}	
			
			//when it gets a new character (i.e, from the flag)
	//	if(new_recieved == 1)
	//	{
			//used to initially see if it gets recieved, now that's all in CLI_Recieve()
			
			//sendbyte(recieved_char);
			//new_recieved = 0;
		//	CLI_Receive(testCharArray,testSize);
			
		//	quit_zero = CLI_Quit(testCharArray, testSize);
		//	if(quit_zero == 0)
		//	{
				//if they do, leave the loop
			//	break;
			//}	
			//read the input and decide what to do
			//CLI_Input(testCharArray, testSize);
			//empty out the character array for next time
		//	CLI_Clean(testCharArray, testSize);
			
			//print the prompt again
			//CLI_Prompt();

		
			
		//}
		
		}
		
		
		
		
	}
}


