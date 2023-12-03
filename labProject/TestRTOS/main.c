

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

extern QueueHandle_t xCLI_Queue;
extern QueueHandle_t xClip_Queue;
extern QueueHandle_t xSell_Queue;
extern QueueHandle_t xWire_Queue;
extern QueueHandle_t xClipper_Queue;
extern QueueHandle_t xClipper_msg_Queue;
extern QueueHandle_t xPrice_Queue;
extern QueueHandle_t xSell_Rate_Queue;


int main(void)
{
	
	serial_init();
	
	createQueues();

	createTasks();
	
	intro_message();
	

	vTaskStartScheduler();
	
	
	
	
	while(1)
	{
	
	}
	
	
	return 0;
}






