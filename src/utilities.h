#ifndef UTILITIES_H_
#define UTILITIES_H_

void setupSysTickTimer(int CPUFREQ);
static volatile int cpu_cycles_to_us=26;

//Raw delay of a certain number of clock cycles. Compensates for how many cycles the setup instructions take.
static inline void delay_clock_cycles(int cycles){

}

//
//For delays greater than 3, uses exactly 26 cycles per delay when inlined, if CPU is running at 26MHz
static inline void delay_us(int pdelay){
	if(pdelay==0) //don't let 0 delay cause us to get stuck in the count down while loop
		return;
	volatile int countervalue=(cpu_cycles_to_us*pdelay);

    STRELOAD=countervalue & 0xFFFFFF;
    STCURR=1; //Clears the counter and loads from STRELOAD

    //Enable SYSTICK Counter
    STCTRL|=1;

    int test=STCURR & 0xFFFFFF;

    while(((STCTRL>>16) & 1)==0){} //Do nothing until SYSTICK finishes counting down

    //Disable SYSTICK Counter
    STCTRL&=~1;
}

//Supports delay up to about 500ms to be safe (technically 625ms)
static inline void delay_ms(int pdelay){
    delay_us(pdelay*1000);
}

#endif /* UTILITIES_H_ */
