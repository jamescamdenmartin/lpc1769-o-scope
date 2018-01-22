/*
 * globals.h
 *
 *  Created on: Apr 9, 2017
 *      Author: JCM
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#define oscopeDataLength 65
#define adcVREF 3.14

//Pin Control Registers
#define PINSEL0 (*(volatile unsigned int*)0x4002C000)
#define PINSEL1 (*(volatile unsigned int*)0x4002C004)
#define PINSEL3 (*(volatile unsigned int*)0x4002C00C)
#define PINSEL4 (*(volatile unsigned int*)0x4002C010)
#define FIO1DIR (*(volatile unsigned int*)0x2009C020)

#define FIO0PIN (*(volatile unsigned int*)0x2009C014)
#define FIO0SET (*(volatile unsigned int*)0x2009C018)
#define FIO0SET (*(volatile unsigned int*)0x2009C018)
#define FIO0CLR (*(volatile unsigned int*)0x2009C01C)
#define FIO0DIR (*(volatile unsigned int*)0x2009C000)

#define PINMODE4 (*(volatile unsigned int*)0x4002C050)

//PLL0 Registers
#define PLL0CON (*(volatile unsigned int*)0x400FC080)
#define PLL0CFG (*(volatile unsigned int*)0x400FC084)
#define PLL0STAT (*(volatile unsigned int*)0x400FC088)
#define PLL0FEED (*(volatile unsigned int*)0x400FC08C)

//System Control Registers
#define SCS (*(volatile unsigned int*) 0x400FC1A0)
#define CLKSRC (*(volatile unsigned int*)0x400FC10C)

//Clock Control Registers
#define CCLKCFG (*(volatile unsigned int*)0x400FC104) //CPU Clock Divider
#define CLKOUTCFG (*(volatile unsigned int*)0x400FC1C8) //CLKOUT Divider
#define PCLKSEL0  (*(volatile unsigned int*)0x400FC1A8)

//Power Control Registers
#define PCONP (*(volatile unsigned int*)0x400FC0C4)

//UART Control Registers
#define U0LCR (*(volatile unsigned int*)0x4000C00C)
#define U0DLL (*(volatile unsigned int*)0x4000C000)
#define U0DLM (*(volatile unsigned int*)0x4000C004)
#define U0FCR (*(volatile unsigned int*)0x4000C008)
#define U0THR (*(volatile unsigned int*)0x4000C000)
#define U0RBR (*(volatile unsigned int*)0x4000C000)
#define U0LSR (*(volatile unsigned int*)0x4000C014)

//ADC Control Registers
#define AD0CR (*(volatile unsigned int*)0x40034000)
#define AD0GDR (*(volatile unsigned int*)0x40034004)
#define AD0DR0 (*(volatile unsigned int*)0x40034010)

//PWM Control Registers
#define PWM1TCR (*(volatile unsigned int*)0x40018004)
#define PWM1PCR (*(volatile unsigned int*)0x4001804C)
#define PWM1MR0 (*(volatile unsigned int*)0x40018018)
#define PWM1MR1 (*(volatile unsigned int*)0x4001801C)
#define PWM1LER (*(volatile unsigned int*)0x40018050)
#define PWM1TC (*(volatile unsigned int*)0x40018008)
#define PWM1PR (*(volatile unsigned int*)0x4001800C)
#define PWM1PC (*(volatile unsigned int*)0x40018010)

//SYSTICK Timer Control Registers
#define STCTRL (*(volatile unsigned int*)0xE000E010)
#define STRELOAD (*(volatile unsigned int*)0xE000E014)
#define STCURR (*(volatile unsigned int*)0xE000E018)

//SPI Registers
#define S0SPCCR (*(volatile unsigned int*)0x4002000C)
#define S0SPDR (*(volatile unsigned int*)0x40020008)
#define S0SPSR (*(volatile unsigned int*)0x40020004)
#define S0SPCR (*(volatile unsigned int*)0x40020000)

#endif /* GLOBALS_H_ */
