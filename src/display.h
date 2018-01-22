#ifndef __DISPLAY_H_INCLUDED
#define __DISPLAY_H_INCLUDED

#define screenwidth 84
#define screenheight 48

#define screenbufferbytes 504

void drawBlackScreen();
void setupDisplaySPI();
void clearDisplayBuffer();
void writeOutToDisplay();
void sendScreenCommand(char);
void setLCDCursor(char, char);
void sendSPIByte(char);
void drawTestPattern();
void drawTestLinesPattern();

void clearSampleBuffer();

void drawWaveformScreen(int* samples, int length, int usPerDiv, int mVPerDiv, int verticalOffset, int samplingScopeOrSingleShot);
void drawWaveformTickMarks();
void drawWaveformUnits(int usPerDiv, int mVPerDiv, int verticalOffset);
void drawCharacter(int xoffset,int yoffset, char text);

#endif /* __DISPLAY_H_INCLUDED */
