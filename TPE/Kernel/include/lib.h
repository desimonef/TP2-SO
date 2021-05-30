#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
char * toHexa (long long number, int bytes);
char *intToHexa(long long num, char *str, int bytes);
int hexaToInt(char* num);
char* itoa(int num, char* str, int base);
void reverse(char str[], int length);
void printf(char * format, ...);
int strlen(char * string);


char *cpuVendor(char *result);
long getRegister(int index);
void dumpMem(char * buff, int address, int bytes);
int asmGetByte(int pos);

#endif