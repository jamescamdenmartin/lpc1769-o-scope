/* Display Functions and interface for PCD8544 matrix LCD */
#include "globals.h"
#include "display.h"
#include "utilities.h"

char screenbuffer[screenbufferbytes] = {0};

//P0.0 connected to active low display reset
//P0.6 connected to display Command/Data pin. Low is Command, High is Data
#define DisplayResetPin 0
#define DisplayDCPin 7
#define DisplaySlaveSelectPin 6
#define SCKPIN 15
#define MOSIPIN 18
#define MISOPIN 17

#define waveformScreenXOffset 14
#define waveformScreenYOffset 4

#define averageOverSets 3

int samplepoints[averageOverSets][oscopeDataLength][2];
int fifoPointer=0;

char font0[5][5]=			{ { 0, 1, 1, 1, 0 },
					  	  	  { 0, 1, 0, 1, 0 },
							  { 0, 1, 0, 1, 0 },
							  { 0, 1, 0, 1, 0 },
							  { 0, 1, 1, 1, 0 } };

char font1[5][5]=	        { { 0, 0, 1, 0, 0 },
					  	  	  { 0, 1, 1, 0, 0 },
							  { 0, 0, 1, 0, 0 },
							  { 0, 0, 1, 0, 0 },
							  { 0, 1, 1, 1, 0 } };

char font2[5][5]=	        { { 0, 0, 1, 0, 0 },
						      { 0, 1, 0, 1, 0 },
							  { 0, 0, 0, 1, 0 },
							  { 0, 0, 1, 0, 0 },
							  { 0, 1, 1, 1, 0 } };

char font3[5][5]=	        { { 0, 1, 1, 0, 0 },
						      { 0, 0, 0, 1, 0 },
							  { 0, 1, 1, 1, 0 },
							  { 0, 0, 0, 1, 0 },
							  { 0, 1, 1, 0, 0 } };

char font4[5][5]=	        { { 0, 1, 0, 1, 0 },
						      { 0, 1, 0, 1, 0 },
							  { 0, 1, 1, 1, 0 },
							  { 0, 0, 0, 1, 0 },
							  { 0, 0, 0, 1, 0 } };

char font5[5][5]=	        { { 0, 1, 1, 1, 0 },
						      { 0, 1, 0, 0, 0 },
							  { 0, 1, 1, 1, 0 },
							  { 0, 0, 0, 1, 0 },
							  { 0, 1, 1, 1, 0 } };

char font6[5][5]=	        { { 0, 1, 1, 1, 0 },
						      { 0, 1, 0, 0, 0 },
							  { 0, 1, 1, 1, 0 },
							  { 0, 1, 0, 1, 0 },
							  { 0, 1, 1, 1, 0 } };

char font7[5][5]=	        { { 0, 1, 1, 1, 0 },
						      { 0, 0, 0, 1, 0 },
							  { 0, 0, 0, 1, 0 },
							  { 0, 0, 1, 0, 0 },
							  { 0, 0, 1, 0, 0 } };

char font8[5][5]=	        { { 0, 1, 1, 1, 0 },
						      { 0, 1, 0, 1, 0 },
							  { 0, 1, 1, 1, 0 },
							  { 0, 1, 0, 1, 0 },
							  { 0, 1, 1, 1, 0 } };

char font9[5][5]=	        { { 0, 1, 1, 1, 0 },
						      { 0, 1, 0, 1, 0 },
							  { 0, 1, 1, 1, 0 },
							  { 0, 0, 0, 1, 0 },
							  { 0, 0, 0, 1, 0 } };

char fontu[5][5]=			{ { 0, 1, 0, 1, 0 },
						      { 0, 1, 0, 1, 0 },
							  { 0, 1, 1, 0, 0 },
							  { 0, 1, 0, 0, 0 },
							  { 0, 1, 0, 0, 0 } };

char fonts[5][5]=	        { { 0, 0, 1, 1, 0 },
						      { 0, 1, 0, 0, 0 },
							  { 0, 0, 1, 0, 0 },
							  { 0, 0, 0, 1, 0 },
							  { 0, 1, 1, 0, 0 } };

char fontm[5][5]=	        { { 0, 0, 0, 0, 0 },
						      { 0, 0, 0, 0, 0 },
							  { 0, 1, 0, 1, 0 },
							  { 1, 0, 1, 0, 1 },
							  { 1, 0, 1, 0, 1 } };

char fontv[5][5]=	        { { 1, 0, 0, 0, 1 },
						      { 1, 0, 0, 0, 1 },
							  { 0, 1, 0, 1, 0 },
							  { 0, 1, 0, 1, 0 },
							  { 0, 0, 1, 0, 0 } };

int everyother=0;

void setupDisplaySPI(){
	//Reset SPI Peripherial
	PCONP&=~(1 << 8);
	PCONP|=(1 << 8);

	//Set PCLK to CPUCLK/1 = 26MHz
	PCLKSEL0 &= ~(3 << 16);
	PCLKSEL0 |= (1 << 16);

	//Set the data rate to PCLK/8 = 3.25mbit/s
	// note that S0SPCCR has to be an even number greater or equal to 8
	S0SPCCR &=~0xFF;
	S0SPCCR |= 0x08;
	//Set to 8 bits per transfer
	S0SPCR &=~(1<<2);
	//Set SPI operates in Master mode
	S0SPCR |=(1<<5);

	//Set SPI Pins to SPI Mode
	/* PINSELx
	 * 1 		5:4 GPIO P0.18 MOSI
	 * 1 		3:2 GPIO P0.17 MISO
	 * 0 		31:30 GPIO P0.15 SCK
	 */
	PINSEL1 |=(3 <<4);
	PINSEL1 |=(3 << 2);
	PINSEL0 |=(3 << 30);

	//Set to outputs
	FIO0DIR|= 1<<DisplayResetPin;
	FIO0DIR|= 1<<DisplayDCPin;
	FIO0DIR|= 1<<DisplaySlaveSelectPin;
	FIO0DIR|= 1<<SCKPIN;
	FIO0DIR|= 1<<MISOPIN;
	FIO0DIR|= 1<<MOSIPIN;

	//Slave Select is active low. Set High->unselected.
	FIO0SET = 1<<DisplaySlaveSelectPin;

	//Reset the display
	FIO0CLR = 1<<DisplayResetPin;
	delay_us(100);
	FIO0SET = 1<<DisplayResetPin;
	delay_us(500);


	sendScreenCommand(0x21);//Extended Command Mode
	sendScreenCommand(0xB9);//Set Contrast
	sendScreenCommand(0x04);//Set Temp coefficient
	sendScreenCommand(0x14);//Set bias mode to 1:48

	sendScreenCommand(0x20);//Power On Screen
	sendScreenCommand(0x0C);//Normal command mode
}

//Sets a pixel to black
static inline void drawPixel(int x,int y){
	x=screenwidth-x-1; //Flip screen to match its unfortunate breadboard location
	if(x<0 || x > screenwidth || y<0 || y> screenheight)
		return;

	screenbuffer[x + (y/8)*screenwidth] |= 1 << (y % 8);
}

//Sets a pixel to white
static inline void clearPixel(int x,int y){
	x=screenwidth-x-1; //Flip screen to match its unfortunate breadboard location
	if(x<0 || x > screenwidth || y<0 || y> screenheight)
		return;

	screenbuffer[x + (y/8)*screenwidth] &= ~(1 << (y % 8));
}

void clearDisplayBuffer(){
	for(int i=0;i<screenbufferbytes;i++)
		screenbuffer[i]=0;
}

//Writes the display frame buffer out to display over SPI
void writeOutToDisplay(){
	setLCDCursor(0,0);
	FIO0SET = 1<<DisplayDCPin;
	FIO0CLR = 1<<DisplaySlaveSelectPin;
	for(int i=0;i<screenbufferbytes;i++)
		sendSPIByte(screenbuffer[i]);

	FIO0SET = 1<<DisplaySlaveSelectPin;

}

void drawBlackScreen(){
	for(int x=0;x<screenwidth;x++)
		for(int y=0;y<screenheight;y++)
			drawPixel(x,y);
}

void drawTestPattern(){
	char everyother=0;
	for(int x=0;x<screenwidth;x+=1){
		for(int y=everyother;y<screenheight;y+=2){
			drawPixel(x,y);
		}
		everyother=!everyother;
	}
}

void drawTestLinesPattern(){
	for(int x=0;x<screenwidth;x+=1){
		for(int y=0;y<screenheight;y+=10){
			drawPixel(x,y);
		}
	}
}

void drawCharacter(int xoffset,int yoffset, char text){
	char (*font)[5];
	switch(text){
		case 0: font=font0;break;
		case 1: font=font1;break;
		case 2: font=font2;break;
		case 3: font=font3;break;
		case 4: font=font4;break;
		case 5: font=font5;break;
		case 6: font=font6;break;
		case 7: font=font7;break;
		case 8: font=font8;break;
		case 9: font=font9;break;
		case 'u': font=fontu;break;
		case 's': font=fonts;break;
		case 'm': font=fontm;break;
		case 'v': font=fontv;break;
		default:
			 font=font0;break;
	}

	for(int x=0;x<5;x+=1){
		for(int y=0;y<5;y+=1){
			if(font[x][y])
				drawPixel(xoffset+x,yoffset+y);
		}
	}
}

void drawWaveformScreen(int* samples, int length, int usPerDiv, int mVPerDiv, int verticalOffset, int samplingScopeOrSingleShot){
	clearDisplayBuffer();
	drawWaveformTickMarks();
	drawWaveformUnits(usPerDiv,mVPerDiv,verticalOffset);

	//Advanced which buffer this set of samples is stored in
	//Average waveform over three sample sets
	fifoPointer++;
	if(fifoPointer>averageOverSets)
		fifoPointer=0;

	//Scale sample voltage to height on screen and store
	for(int i=0;i<length;i++){
		samplepoints[fifoPointer][i][0]=i+waveformScreenXOffset;
		samplepoints[fifoPointer][i][1]=(int)(waveformScreenYOffset+(((((float)samples[i])/4096*adcVREF)*1000)/mVPerDiv)*5);
	}

    if(!samplingScopeOrSingleShot){
        //Draw all sample sets to screen buffer, like a sampling scope
        for(int sampleset=0;sampleset<averageOverSets;sampleset++){
            for(int i=0;i<length;i++){
                drawPixel(samplepoints[sampleset][i][0],samplepoints[sampleset][i][1] + verticalOffset*5);
            }
        }
    }
    else
    { //Draw only the single latest capture to screen buffer, like a single-shot scope
            for(int i=0;i<length;i++){
                drawPixel(samplepoints[fifoPointer][i][0],samplepoints[fifoPointer][i][1]+ verticalOffset*5);
            }        
    }
}

//Drawn Time and Volt div tickmarks, 5 samples per
void drawWaveformTickMarks(){
	//Time Div
	for(int i=waveformScreenXOffset;i<screenwidth-3;i+=5){
		for(int j=0;j<3;j+=1){
			drawPixel(i,j);
		}
	}

	//Volt Div
	for(int i=waveformScreenYOffset;i<screenheight;i+=5){
		for(int j=10;j<13;j+=1){
			drawPixel(j,i);
		}
	}
}

void drawWaveformUnits(int usPerDiv, int mVPerDiv, int verticalOffset){
	int xoffset=2;
    //Draw time div information
	drawCharacter(2,xoffset,(usPerDiv/10) % 10);
	drawCharacter(2,xoffset+5,usPerDiv % 10);
	drawCharacter(3,xoffset+10,'u');
	drawCharacter(2,xoffset+13,'s');

    //Draw volt div information
	drawCharacter(2,xoffset+22,(mVPerDiv/100) % 10);
	drawCharacter(2,xoffset+26,(mVPerDiv/10) % 10);
	drawCharacter(2,xoffset+30,mVPerDiv % 10);
	drawCharacter(2,xoffset+35,'m');
	drawCharacter(2,xoffset+40,'v');
    
    //Draw markers for 0V, 1V, 2V, and 3V so that vertical waveform offset can be read
    drawCharacter(80,verticalOffset*5,0);
	drawCharacter(80,verticalOffset*5+4,'v');
    drawCharacter(80,(1000/mVPerDiv)*5+verticalOffset*5,1);
	drawCharacter(80,(1000/mVPerDiv)*5+verticalOffset*5+4,'v');
    drawCharacter(80,(2000/mVPerDiv)*5+verticalOffset*5,2);
	drawCharacter(80,(2000/mVPerDiv)*5+verticalOffset*5+4,'v');
    drawCharacter(80,(3000/mVPerDiv)*5+verticalOffset*5,3);
	drawCharacter(80,(3000/mVPerDiv)*5+verticalOffset*5+4,'v');
}

void clearSampleBuffer(){
	for(int sampleset=0;sampleset<averageOverSets;sampleset++){
		for(int i=0;i<oscopeDataLength;i++){
			samplepoints[sampleset][i][0]=0;
			samplepoints[sampleset][i][1]=0;
		}
	}
}

//Send Screen Command
void sendScreenCommand(char command){
	FIO0CLR = 1<<DisplayDCPin;
	FIO0CLR = 1<<DisplaySlaveSelectPin;
	sendSPIByte(command);
	FIO0SET = 1<<DisplayDCPin;
	FIO0SET = 1<<DisplaySlaveSelectPin;

}

void setLCDCursor(char x, char y){
	sendScreenCommand(0x80 | x);
	sendScreenCommand(0x40 | y);
}

//Send SPIByte and block until transaction completes
inline void sendSPIByte(char out){
	S0SPDR=out;
	while(!((S0SPSR) & (1 << 7))){}//block until SPIF is set
}
