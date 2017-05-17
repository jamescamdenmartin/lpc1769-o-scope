#ifndef UTILITIES_H_
#define UTILITIES_H_

#define STCTRL (*(volatile unsigned int*)0xE000E010)
#define STRELOAD (*(volatile unsigned int*)0xE000E014)
#define STCURR (*(volatile unsigned int*)0xE000E018)

void setupSysTickTimer(int CPUFREQ);
static volatile int cpu_cycles_to_us=0;

static inline void delay_us(int delay){
    //Load delay value into the counter
    STRELOAD=(cpu_cycles_to_us*delay) & 0xFFFFFF;
    STCURR=1; //Clears the counter and loads from STRELOAD

    //Enable SYSTICK Counter
    STCTRL|=1;

    while(((STCTRL>>16) & 1)==0){} //Do nothing until SYSTICK finishes counting down

    //Disable SYSTICK Counter
    STCTRL&=~1;
}

//Supports delay up to about 500ms to be safe (technically 625ms)
static inline void delay_ms(int delay){
    delay_us(delay*1000);
}

#endif /* UTILITIES_H_ */
