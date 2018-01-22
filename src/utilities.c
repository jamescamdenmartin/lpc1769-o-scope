#include "globals.h"
#include "utilities.h"

//Setup SysTickTimer for provided cpufreq in MHz
void setupSysTickTimer(int cpufreq){
    cpu_cycles_to_us=cpufreq;

    //Set the clock source to the CPUCLK
    STCTRL|=(1 << 2);
}

extern inline void delay_us(int delay);
extern inline void delay_ms(int delay);
extern inline void delay_clock_cycles(int cycles);
