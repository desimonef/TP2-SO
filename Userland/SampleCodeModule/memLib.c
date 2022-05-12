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