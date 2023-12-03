/*
ENSE 452 Lab Project
Alok Paranjape
200246370
December 3rd

Defines the constants needed for each task's priority,
the use of each queue (of many)
and the signatures for setting up the queues, and for setting up 
the functions that use them

*/

#include "queue.h"
#include "task.h"


//putting this in order of importance and how quickly they need to run
#define mainCLI_TASK_PRIORITY (tskIDLE_PRIORITY+1)
#define mainCLIPDISPLAY_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define mainCLIPSELL_TASK_PRIORITY (tskIDLE_PRIORITY + 3)
#define mainAutoClipper_TASK_PRIORITY (tskIDLE_PRIORITY + 4)



extern QueueHandle_t xCLI_Queue;
extern QueueHandle_t xClip_Queue;
extern QueueHandle_t xSell_Queue;
extern QueueHandle_t xWire_Queue;
extern QueueHandle_t xClipper_Queue;
extern QueueHandle_t xClipper_msg_Queue;
extern QueueHandle_t xPrice_Queue;
extern QueueHandle_t xSell_Rate_Queue;


/**
Initializes the queues externed above
@pre need to already have run serial_init() first
*/
void createQueues(void);
/**
Sets up the tasks that use all the priorities above
@pre need to run serial_init() and createQueues() first
*/
void createTasks(void);


/**
Manages updating the display and
handling input from the other tasks and interrupts
Does some calculations to see if a request is
possible before updating and printing it
Funnels input and output from almost
every other queue/task could probably be split up a bit more
@pre need run serial_init(), createQueues() and createTasks() first
@param parameters is included as part of the FreeRTOS format for making tasks
*/
static void vPaperClipDisplayTask(void * parameters);

/**
Automatically times when to sell paperclips,
sends them to display to check the sale is possible
Gets information from the sell rate queue, 
to vary how quickly it tries to sell clips over each time it runs
@pre need run serial_init(), createQueues() and createTasks() first
@param parameters is included as part of the FreeRTOS format for making tasks
*/
static void vPaperClipSellTask(void * parameters);


/**
Automatically times when to create clips, once autoclippers are purchased
takes information from the clippers_queue, 
since that needs to be purchased seperately
basically just does what the button does, but automatically
@pre need run serial_init(), createQueues() and createTasks() first
@param parameters is included as part of the FreeRTOS format for making tasks
*/
static void vAutoClippersTask(void * parameters);


/**
Gets input from the usart interrupt getting a character,
takes it out of the queue and calls CLI_Recieve on it to store/read it
@pre need run serial_init(), createQueues() and createTasks() first
@param parameters is included as part of the FreeRTOS format for making tasks
*/
static void vCLI_Task(void * parameters);