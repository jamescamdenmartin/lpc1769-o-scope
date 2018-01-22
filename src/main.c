/*
===============================================================================
 Name        : main.c
 Author      : James Martin and Bradley Morrow
 Version     : 1.0
 Copyright   : All Rights Reserved.
 Description : main definition for LPC1769 oscilloscope
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

//#include <cr_section_macros.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "globals.h"
#include "data.h"
#include "utilities.h"
#include "display.h"

volatile int delay_between_samples_us=0;

volatile char trigger_direction=1; //0 - rising 1 - falling

int mVPerDiv=250;
int uSPerDiv=5;
int verticalOffset=0;
int samplingScopeOrSingleShot=0; //0 draws multiple captures to the screen like a sampling scope, 1 draws a single capture like a real-time scope

int main(void) {
	setupCLK();//sets CPU clk to 26 MHZ
	setupADC();//turn on ADC subsystem
	setupUART0();//turns on the UART subsystem with 9600 baud
    setupSysTickTimer(26); //Configures the delay function timer to match the cpu freq
    setupDisplaySPI(); //Configure SPI peripherial and reset display

   /* //Display test pattern
    while(1){
    	delay_ms(400);
    	clearDisplayBuffer();
    	drawTestLinesPattern();
    	writeOutToDisplay();

		printLineSerial("SDF");
		delay_us(6400);
    }*/

    //Main program loop
	while(1){
		//read in oscope data
		int* data=getOscopeData(delay_between_samples_us, trigger_direction);

		drawWaveformScreen(data,oscopeDataLength,uSPerDiv,mVPerDiv,verticalOffset,samplingScopeOrSingleShot);
    	writeOutToDisplay();

		//print out data over UART for debugging
		printLineSerial("New Data");
		delay_us(100);
		for(int i=0;i<64;i++){
			char stringbuf [33];
			float convertedVal=((float)data[i])/4096*adcVREF;
			sprintf(stringbuf,"%.2f",convertedVal);
			printLineSerial(stringbuf);
			delay_us(6000);
		}
		printLineSerial("\n");

		//read in user settings
		char input;
		while((input=readUART0())){
			switch(input){

				case '-':	mVPerDiv-=50;clearSampleBuffer();break;
				case '=':	mVPerDiv+=50;clearSampleBuffer();break;

				case ']':	uSPerDiv+=5;clearSampleBuffer();delay_between_samples_us=uSPerDiv-5;break;
				case '[':	uSPerDiv-=5;clearSampleBuffer();delay_between_samples_us=uSPerDiv-5;break;
                
                case ';':   samplingScopeOrSingleShot=0;break; //sampling scope mode
                case '\'':  samplingScopeOrSingleShot=1;break; //single shot mode
                
                case '8': verticalOffset=0;break;
                case '0': verticalOffset+=1;break;
                case '9': verticalOffset-=1;break;
				default: 	break;
			}

			//make sure inputs are in valid ranges
			if(mVPerDiv<50)
				mVPerDiv=50;
			if(mVPerDiv>1000)
				mVPerDiv=1000;

			if(uSPerDiv<5){
				uSPerDiv=5;
				delay_between_samples_us=uSPerDiv-5;
			}
			if(uSPerDiv>50){
				uSPerDiv=50;
				delay_between_samples_us=uSPerDiv-5;
			}

            if(verticalOffset>25){
               verticalOffset=25;
            }
            if(verticalOffset<-25){
               verticalOffset=-25;
            }

		}
	}
}
