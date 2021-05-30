#ifndef USERLIB_H
#define USERLIB_H

#include <stdint.h>

int scan(int fd, char * buffer, uint32_t length);
int print(int fd, char * buffer, uint32_t length);
int dumpRegs(char * buffer); 
int dumpMem(char * buffer, uint64_t address, uint64_t bytes);

#endif