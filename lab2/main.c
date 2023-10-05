
//ENSE 452
//Alok Paranjape
// October 4th:
//Lab 2: Checking if the github is working again

//ADDED SOME CODE HERE
//include <iostream>

//THE COM PORT is COM5
//int main() {
//	serial_open();
	//  return 0;
//} 
	
#include "stm32f10x.h"
#include "usart.h"

//Connected on COM 5

void delay()
{
	
	unsigned volatile int c, d;
   
   for (c = 1; c <= 3000; c++)
       for (d = 1; d <= 3000; d++)
       {}
}

int main() {
	
	
	serial_open();
	
	
	return 0;
}

