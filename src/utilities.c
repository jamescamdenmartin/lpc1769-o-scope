#include "utilities.h"

//Calculate the number of CPUCLK pulses in 1 us
void setupSysTickTimer(int cpufreq){
    cpu_cycles_to_us=1/(((float)1000000)/(cpufreq));
}

extern inline void delay_us(int delay);
extern inline void delay_ms(int delay);
