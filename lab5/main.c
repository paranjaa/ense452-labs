
//Starting again with the sample code, since I couldn't remake it in my lab 5



#include <stdio.h>
//#include "stm32F103RB.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "CLI.h"
#include "queue.h"

#define mainBLINKY_TASK_PRIORITY 		(tskIDLE_PRIORITY + 1 )
static void vBlinkTask( void * parameters);
QueueHandle_t Freq_Queue;

//making this again for the CLI_Task
#define mainCLI_TASK_PRIORITY 			(tskIDLE_PRIORITY +1)
static void vCLI_Task (void* parameters);
QueueHandle_t CLI_Queue;


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
	Freq_Queue = xQueueCreate(1, sizeof(uint8_t));	
	//and one for the CLI, same deal
	CLI_Queue = xQueueCreate(1, sizeof(uint8_t));
	//
	/* Start the scheduler. */
	vTaskStartScheduler();
	
	//Just keep looking after that
	while(1)
	{
		
	}
	//return 0;
}


static void vBlinkTask( void * parameters)
{
	//adding in a starting frequency here
	int frequency = 1000;
	for (;;)
	{
		//if there's a new value in the queue for it to deal with
		if( uxQueueMessagesWaiting( Freq_Queue ) != 0 )		
		{			
			//replace the frequency value with it
			xQueueReceive(Freq_Queue, &frequency, 250);
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

//making a new task for CLI, most of the code from lab 4's main
static void vCLI_Task(void * parameters)
{
	
	//for some reason, it doesn't like to print N? wait, maybe it's just that index?
	uint8_t title_msg[] = "ENSE 452 Lab 5 CLI";
	CLI_Transmit(title_msg, (sizeof(title_msg) / sizeof(uint8_t)));
	sendbyte('\r');
	sendbyte('\n');
	
	uint8_t recieved_char;
	for (;;)
	{
		//check if there's something saved in the queue from the user
		BaseType_t status = xQueueReceive(CLI_Queue, &recieved_char, 100);
		
		//if there is, then print it out
		if(status == pdTRUE)
		{
			sendbyte(recieved_char);
		
		}
		
		
		
		
	}
}


