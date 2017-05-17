/* Display Functions and interface for PCD8544 matrix LCD */
#include "globals.h"
#include "display.h"

char screenbuffer[screenbufferbytes] = {0};

//FIO0SET - 0x2009 C018
//FIO0CLR - 0x2009 C01C

//P0.0 connected to active low display reset
//P0.6 connected to display Data/Command pins
#define DisplayResetPin 0
#define DisplayDCPin 6

void setupDisplaySPI(){
	//Set to outputs
	FIO0DIR|= 1<<DisplayResetPin;
	FIO0DIR|= 1<<DisplayDCPin;

	//Reset the display
	FIO0CLR = 1<<DisplayResetPin;
	FIO0SET = 1<<DisplayResetPin;

}

//Sets a pixel to black
static inline void drawPixel(x,y){
	//int byte = x+y*screenwidth / 8;
	//char bitindex = (x+y*screenwidth % 8);

	screenbuffer[x+y*screenwidth / 8] |= 1 << (x+y*screenwidth / 8);
}

//Sets a pixel to white
static inline void clearPixel(x,y){
	//int byte = x+y*screenwidth / 8;
	//char bitindex = (x+y*screenwidth % 8);

	screenbuffer[x+y*screenwidth / 8] &= ~(1 << (x+y*screenwidth / 8));
}

void clearBuffer(){
	for(int i=0;i<screenbufferbytes;i++)
		screenbuffer[i]=0;
}
