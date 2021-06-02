#ifndef STDLIB_H
#define STDLIB_H

int getChar();
int putChar(char c);
int scanf(char * command, ...);
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
char * intToHex(long long num, char *str, int bytes);
int abs(int num);

#endif