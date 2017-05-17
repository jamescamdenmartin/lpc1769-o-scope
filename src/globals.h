/*
 * globals.h
 *
 *  Created on: Apr 9, 2017
 *      Author: JCM
 */

#ifndef FINALPROJECT_H_
#define FINALPROJECT_H_


//Pin Control Registers
#define PINSEL0 (*(volatile unsigned int*)0x4002C000)
#define PINSEL1 (*(volatile unsigned int*)0x4002C004)
#define PINSEL3 (*(volatile unsigned int*)0x4002C00C)
#define FIO1DIR (*(volatile unsigned int*)0x2009C020)

#define FIO0PIN (*(volatile unsigned int*)0x2009C014)
#define FIO0SET (*(volatile unsigned int*)0x2009C018)
#define FIO0SET (*(volatile unsigned int*)0x2009C018)
#define FIO0CLR (*(volatile unsigned int*)0x2009C01C)
#define FIO0DIR (*(volatile unsigned int*)0x2009C000)

//PLL0 Registers
#define PLL0CON (*(volatile unsigned int*)0x400FC080)
#define PLL0CFG (*(volatile unsigned int*)0x400FC084)
#define PLL0STAT (*(volatile unsigned int*)0x400FC088)
#define PLL0FEED (*(volatile unsigned int*)0x400FC08C)

//System Control Registers
#define SCS (*(volatile unsigned int*) 0x400FC1A0)
#define CLKSRC (*(volatile unsigned int*)0x400FC10C)

//Clock Divider Registers
#define CCLKCFG (*(volatile unsigned int*)0x400FC104) //CPU Clock Divider
#define CLKOUTCFG (*(volatile unsigned int*)0x400FC1C8) //CLKOUT Divider

//Power Control Registers
#define PCONP (*(volatile unsigned int*)0x400FC0C4)

//Clock Control Registers
#define PCLKSEL0  (*(volatile unsigned int*)0x400FC1A8)

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

#endif /* FINALPROJECT_H_ */
