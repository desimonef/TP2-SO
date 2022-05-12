#ifndef _PROCESSES_H
#define _PROCESSES_H

#include "stdint.h"
#include "sysCalls.h"

#define NULL (void *) 0
#define FG 1
#define BG 0

int createProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd);
int killProc(uint64_t pid);
int blockProc(uint64_t pid);
int unblockProc(uint64_t pid);
void niceProc(uint64_t pid, int priority);
int getPID();
void yieldProc();
//void wait(uint64_t pid);

#endif