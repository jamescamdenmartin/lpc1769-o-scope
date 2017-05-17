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

//data functions
int* getOscopeData();
int sampleADC(int channel);

//UART functions
void writeUART0(char totransmit);
void printLineSerial(char *string);
char readUART0();


#endif /* DATA_H_ */
