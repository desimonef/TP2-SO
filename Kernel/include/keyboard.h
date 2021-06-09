#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

int codeMatchesAscii(int scancode);
int getScanCode();
int isLetter(char c);
char getAscii(int scancode);
void onKeyPressed(char chr); 
int keyboardRead(char *ret, int count);
void keyboard_handler();
void shiftPressing(int scancode);
int arrows(int scancode);

#endif
