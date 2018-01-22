/*
 * data.h
 *
 *  Created on: Apr 6, 2017
 *      Author: mart7066
 */

#ifndef DATA_H_
#define DATA_H_

//setup functions
void setupADC();
void setupCLK();
void setupUART0();
void setupPWM();

//PWM functions
void updatePWMSettings(float period, float dutyCycle);

//data functions
int* getOscopeData(int delay, char triggerriseorfall);
int sampleADC(int channel);
int sampleADCtrig(int channel, char triggerriseorfall);

//UART functions
void writeUART0(char totransmit);
void printLineSerial(char *string);
char readUART0();


#endif /* DATA_H_ */
