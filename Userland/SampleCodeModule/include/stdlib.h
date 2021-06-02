#ifndef STDLIB_H
#define STDLIB_H
#include "stdint.h"

int getchar();
int putchar(char c);
int scanf(char * format, ...);
void readLine();
int strToInt(char *str, int* size);
void printf(char * format, ...);
int strlen(char * string);
int strcmp(const char * str1, const char * str2);
void strcpy(const char * str1, char * str2);
void strcpyWithSeparator(const char * str1, char * str2, char separator);
void putInBuff(char c);
int myAtoi(const char * num, int * dest);
void reverse(char str[], int length);
char* itoa(int num, char* str, int base);
int hexToInt(char* num);
char *intToHex(uint64_t num, char *str, int bytes);
int isValidHexaAddress(char * buff);

#endif