#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>

int getchar();
int putchar(char c);
void clearBuff();
int scanf(char * command, ...);
void readLine();
void printf(char * command, ...);
int strlen(char * string);
int strcmp(const char * str1, const char * str2);
void strcpy(const char * str1, char * str2);
void strcpyWithSeparator(const char * str1, char * str2, char separator);
void putInBuff(char c);
void reverse(char str[], int length);
int strToInt(char *str, int *size);
char* itoa(int num, char* str, int base);
int isValidHexaAddress(char * buff);
int hexToInt(char* num);
char * intToHex(uint64_t num, char * str, int bytes);
void sleep(int secs);

#endif