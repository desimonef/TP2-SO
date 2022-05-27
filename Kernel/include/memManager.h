#ifndef MEMMANAGER_H
#define MEMMANAGER_H

#include "naiveConsole.h"

//#define NULL ((void *) 0)
#define EMPTY 0
#define PARTIAL 1
#define FULL 2

#define BEGIN_MEM (unsigned)0x600000
#define END_MEM (unsigned)0xFFFFFF
#define MEM_START (unsigned)0xF00000
#define MIN_SIZE 32
#define TOTALMEM 1048576
#define LIMIT 0xF00000

void initMM(void);
void * malloc(int size);
void free(void * addr);
void memDump(void );

#endif