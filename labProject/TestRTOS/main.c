/*
ENSE 452 Lab Project
Alok Paranjape
200246370
December 3rd

Implements the first part of the first 
phase of the incremental game [Universal Paperclips]
(https://www.decisionproblem.com/paperclips/index2.html)


Seperated out my initialization and task creation into
different files so main isn't so huge

Had trouble with downloading the packages for FreeRTOS,
so, like Lab 5, started from the TestRTOS sample code 

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
	//get the board ready to run the game
	//remade from serial_open() from the labs, which was being unpredictable w/ 
	serial_init();
	
	//print the initial UI, and set up the scrollable window below it
	intro_message();
	
	//create the assorted queues needed to pass information between tasks
	createQueues();

	//create the assorted tasks needed, for displaying information, calculations
	//making/selling clips, etc
	createTasks();
	


	//start the task scheduler so all tasks start running 
	vTaskStartScheduler();
	
	//make an empty while loop for the tasks to all run in
	//it shouldn't ever reach that return 0 at the end
	while(1)
	{
	
	}
	
	
	return 0;
}

