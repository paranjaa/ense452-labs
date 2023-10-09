#include "stm32f10x.h"
#include "usart.h"

//added for phase 2
void CLI_Transmit(uint8_t *pData, uint16_t Size);
void CLI_Receive(uint8_t *pData, uint16_t Size);
void CLI_Prompt(void);
void CLI_Help(void);
void CLI_Query(void);
void CLI_Input(uint8_t *pData, uint16_t Size);
void CLI_LEDON(void);
void CLI_LEDOFF(void);
