// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "sysCalls.h"
#include <stdint.h>
#include "memLib.h"

void * malloc(int size)
{
    return (void *) syscall(MALLOC, size, 0, 0, 0, 0, 0);
}

void freeMem(void * addr)
{
    syscall(FREE, (uint64_t) addr, 0, 0, 0, 0, 0);
}