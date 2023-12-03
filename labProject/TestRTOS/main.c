/*
ENSE 452 Lab Project
Alok Paranjape
200246370
December 3rd

*/


#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "CLI.h"
#include "queue.h"
#include "task.h"
#include "util.h"
#include "rtos_tasks.h"




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






