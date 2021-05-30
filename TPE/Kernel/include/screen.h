<<<<<<< HEAD
#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

void initScreen();
void setSize(int newWidth, int newHeight); 
void setCursor(int pos);
int getCursor();
void printString(char *string, int pos, char colour, char background);
void checkPosition();

=======
#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

void initScreen();
void setSize(int newWidth, int newHeight); 
void setCursor(int pos);
int getCursor();
void printString(char *string, int pos, char colour, char background);
void checkPosition();

>>>>>>> 664ba25eeda664f8e265c2f59e0b0abdb58c13f8
#endif