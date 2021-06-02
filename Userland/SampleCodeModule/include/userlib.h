#ifndef USERLIB_H
#define USERLIB_H

#include <stdint.h>

int scan(int fd, char * buffer, uint32_t length);
int print(int fd, char * buffer, uint32_t length);
int dumpRegs(uint64_t * buffer); 
int dumpMem(char * buffer, uint64_t address, uint64_t bytes);
void accessClock(int id, char * buff);
void clearScreen();
void UDcaller();

#endif