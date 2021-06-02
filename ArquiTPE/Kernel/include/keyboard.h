#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

int codeMatchesAscii(int scancode);
int getScanCode();
int isLetter(char c);
uint8_t getMatchingAscii(int scancode);
void onKeyPressed(char chr);
int readFromKeyboard(char *dest, int amount); 
void keyboard_handler();
void isShiftPressed(int scancode);
void isArrowKey(int scancode) ;

#endif