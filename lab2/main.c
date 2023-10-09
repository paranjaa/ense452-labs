
//ENSE 452
//Alok Paranjape
// October 4th:
//Lab 2: Checking if the github is working again


#include "stm32f10x.h"
#include "usart.h"
#include "CLI.h"

//Connected on COM 5

void startupCheck(void)
{	
	//show the program is running by toggling the onboard LED
	GPIOA->ODR |= GPIO_ODR_ODR5;
	delay();
	GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;

}


int main() {
	
	serial_open();
	startupCheck();
	
	//phase 1
	//uint8_t testChar = 'a';
	
	uint16_t testSize = 32;
	uint8_t testCharArray[testSize];
	
	
	//testCharArray[0] = 't';
	//testCharArray[1] = 'e';
	//testCharArray[2] = 's';
	//testCharArray[3] = 't';

	
	//uint8_t* testPointer = &testCharArray[0];
	//while(1)
	//{
	//CLI_LEDON();
	uint8_t quit1 = 1;
	while(1)
	{
		CLI_Prompt();
		CLI_Receive(testCharArray,testSize);
		CLI_Transmit(testCharArray, testSize);
		quit1 = CLI_Quit(testCharArray, testSize);
		if(quit1 == 0)
		{
			uint8_t quit_msg[] = "Ending the program";
			CLI_Transmit(quit_msg, (sizeof(quit_msg) / sizeof(uint8_t)));
			break;
		}	
		CLI_Input(testCharArray, testSize);
	
	}
	
	//}
	
	return 0;
}

		
		//sendbyte(*testPointer);
		//sendbyte(*(testPointer+1));
		//sendbyte(*(testPointer+2));
		//sendbyte(*(testPointer+3));
		

		//sendData();
		//sendbyte(testChar);
		//recieveData();
		
		//testChar = getbyte();
		
		//if(testChar == 'y')
		//{
		//	GPIOA->ODR |= GPIO_ODR_ODR5;
		//}
		
		//if(testChar == 'n')
		//{
		//		GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;
		//}
		//echoCharacter();
		//CLI_Transmit(testPointer, testSize);
		
		//CLI_Receive(testCharArray, testSize);
		//CLI_Help();
		//GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;
		//CLI_Query();
		//delay();
		//GPIOA->ODR |= GPIO_ODR_ODR5;
		//CLI_Query();
		//delay();