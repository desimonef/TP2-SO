#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char * intToHexa (uint64_t num, char * str, int bytes);
void reverse(char * str, int length);
int strlen(char * string);
void sleep(int secs);

char * cpuVendor(char *result);
uint64_t getDateTime(int register);
int readKey();
uint64_t getRegister(int index);
int asmGetByte(int pos);


#endif