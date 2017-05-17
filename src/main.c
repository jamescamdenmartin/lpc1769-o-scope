/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "main.h"
#include "data.h"
#include "utilities.h"

#define adcVREF 3.14


int main(void) {
	setupCLK();//sets CPU clk to 26 MHZ
	setupADC();//turn on ADC subsystem
	setupUART0();//turns on the UART subsystem with 9600 baud

    setupSysTickTimer(26*1000000); //Configures the delay function timer to match the cpu freq
    
	while(1){
		//read in oscope data
		int* data=getOscopeData(5);

		//print out data over UART
		printLineSerial("New Data");
		for(int i=0;i<64;i++){
			char stringbuf [33];
			float convertedVal=((float)data[i])/4096*adcVREF;
			sprintf(stringbuf,"%.2f",convertedVal);
			printLineSerial(stringbuf);
			delay_us(100);
		}
		printLineSerial("\n");

		delay_ms(1);
	}
}
