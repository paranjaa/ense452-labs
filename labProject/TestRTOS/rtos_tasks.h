#include "queue.h"
#include "task.h"


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



void createQueues(void);

void createTasks(void);

static void vPaperClipDisplayTask(void * parameters);

static void vPaperClipSellTask(void * parameters);

static void vAutoClippersTask(void * parameters);

static void vCLI_Task(void * parameters);