#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

char getAscii(int scancode);
int codeMatchesAscii(int scancode);
int isArrow(int scancode);
void screenMove(int scancode);
void shiftPressing(int scancode);
void checkIndex();
int keyboardRead(char *ret, int count);
void store(char c);
void keyboard_handler();
void resetKeyboardFlags();

#endif
