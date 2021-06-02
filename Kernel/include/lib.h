#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int strlen(char * string);

char * cpuVendor(char *result);
uint64_t getDateTime(int register);
int readKey();
uint64_t getRegister(int index);
int asmGetByte(int pos);

#endif