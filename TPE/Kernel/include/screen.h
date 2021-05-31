
#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

void initScreen();
void setSize(int newWidth, int newHeight); 
void setCursor(int pos);
int getCursor();
void moveCursor(int move);
void changeScreen(int whichScreen);
void printString(char *string, int pos, char colour, char background);
void printChar(char c, char colour, char background);
void checkPosition();
void initDoubleScreen();

#endif
