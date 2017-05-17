#include "data.h"
#include "globals.h"
#include "string.h"

//sets the CPU clock to 26MHz and outputs the clock divided by 2 to pin 1.27
void setupCLK(){
	CLKOUTCFG&=~0xF; //Use CPU clock as CLKOUT source

	//Divide CLKOUT by 2
	CLKOUTCFG&=~(0xF << 4);
	CLKOUTCFG|=((2 - 1) << 4);

	//Enable CLKOUT
	CLKOUTCFG|=(1 << 8);

	//Set the GPIO pin 1.27 to output the CLKOUT signal
	FIO1DIR=(1 << 27);
	PINSEL3|=(1 << 22);
	PINSEL3&=~(1 << 23);

	//Disconnect PLL0 in case it's connected
	PLL0CON&=~1;
	PLL0FEED=0xAA;
	PLL0FEED=0x55;

	//Enable the main oscillator
	SCS|=(1<<5);
	while(!(SCS >> 6)){} //Wait for the main oscillator to be ready

	//Set the sysclk source to the main board 12MHz oscillator
	CLKSRC|=1;

	//PLL0 Clock Divider Config
	PLL0CFG&=~0x3FFF;  //14 bit multiplier
	PLL0CFG|=(13 - 1); //Set multiplier to 13 times
	PLL0CFG&=~(0xFF<<16); //Clear predivider (set to divide by 1)
	PLL0FEED=0xAA;
	PLL0FEED=0x55;

	//Enable PLL0
	PLL0CON|=1;
	PLL0FEED=0xAA;
	PLL0FEED=0x55;
	while(!(PLL0STAT >> 26)){} //Wait for PLL0 to Lock

	//Set CPU Clock Divider, divide by 24
	CCLKCFG&=~0xFF;
	CCLKCFG|=(12-1);

	//Finally set CPU CLK Source to PLL0
	PLL0CON|=(1 << 1);
	PLL0FEED=0xAA;
	PLL0FEED=0x55;
}

//turns on the ADC and sets it's clock to the max 13MHZ
void setupADC(){
    //Power on peripheral
    PCONP |= (1 << 12);

    //Set PCLK to CPUCLK/2 = 13MHz
	PCLKSEL0 |= (1 << 25);
	PCLKSEL0 &= ~(1 << 24);

    //Select P0.23 as ADC Channel AD0.0
    PINSEL1 |= (1 << 14);
    PINSEL1 &= ~(1 << 15);
}

//turns on the UART subsystem with 9600 baud
void setupUART0(){
	//Set the UART0 Peripheral Clock Source to CPUCLK/1
	//Gives a UART0 PCLK of 13Mhz
	PCLKSEL0 &= ~(1 << 7);
	PCLKSEL0 |= (1 << 6);

	//On reset, UART0 is enabled (PCUART0 = 1), and UART2/3 are disabled

	//Enable access to the UART0 Divisor Latches (DLAB)
	U0LCR |= (1<<7);

	//Set UART0 Clock Divider to 26
	//Gives a clock rate of 4Mhz/(16 * 26) = 9615.38Hz, error of +0.16% off 9600 baud rate
	U0DLL &= ~0xFF;
	U0DLM &= ~0xFF;
	U0DLL |= 169;

	//Disable access to the UART0 Divisor Latches (DLAB)
	U0LCR &= ~(1<<7);

	//Set worth length to 8 bit
	U0LCR |= 3;

	//Enable UART0 FIFO
	U0FCR |= 1;

	//Configure P0.2 and P0.3 as UART0 TX and RX
	PINSEL0 |= (1 << 4);
	PINSEL0 &= ~(1 << 5);
	PINSEL0 |= (1 << 6);
	PINSEL0 &= ~(1 << 7);
	//UART receive pins should not have pull-down resistors enabled
	// but at this point all pins default to pullup
}

#define oscopeDataLength 64
int oscopeData[oscopeDataLength];

//reads in the oscope data and passes back a raw array of samples
int* getOscopeData(int delay){
	//read in data
	for(int i=0;i<oscopeDataLength;i++){
		oscopeData[i]=sampleADC(0);

	}
	return oscopeData;
}

//reads in data from ADC channel channel
int sampleADC(int channel){
	AD0CR  = (1<<channel)|(1<<21);
	AD0CR |= (1<<24); //Begins conversion

	while((AD0GDR & (1<<31)) == 0){} //Wait until done

	int result =((AD0GDR >> 4) & ((1<<12)-1)); //12 bit result
	AD0CR &=~(1<<24); //Clear conversion

    return result;
}

void writeUART0(char totransmit){
	U0THR = totransmit;
}

void printLineSerial(char *string){
    for(int i = 0; i < strlen(string); i++){
         writeUART0(string[i]);
    }
    writeUART0('\n');
    writeUART0('\r');
}

char readUART0(){
	if(U0LSR & 1)
		return U0RBR & 0xFF;
	else
		return 0;
}

