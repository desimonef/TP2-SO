#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

int codeMatchesAscii(int scancode);
int getScanCode();
int isLetter(char c);
uint8_t getMatchingAscii(int scancode);
void onKeyPressed(char chr); 
int keyboardRead(char *ret, int count);
void keyboard_handler();
void isShiftPressed(int scancode);
int isArrowKey(int scancode) ;

#endif
