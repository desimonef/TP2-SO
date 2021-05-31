
#ifndef STDLIB_H
#define STDLIB_H

int getchar();
int putchar(char c);
int scanf(char * command, ...);
void printf(char * format, ...);
char * hexaToString(int num, char * str);
char * decimalToString(int num, char * str);
char * reverseString(char * str, int len);
int strlen(char * string);
int strcmp(const char * str1, const char * str2);
void strcpy(const char *str1, char *str2);
int myAtoi(const char *num, int *dest);
void reverse(char str[], int length);
char *itoa(int num, char *str, int base);

#endif
