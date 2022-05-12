#include <semLib.h>
#include "sysCalls.h"

uint64_t semOpen(uint64_t id, uint64_t value)
{
    return syscall(OPEN_SEM, id, value, 0, 0, 0, 0);
}

int semWait(uint64_t id)
{
    return syscall(WAIT_SEM, id, 0, 0, 0, 0, 0);
}

int semPost(uint64_t id)
{
    return syscall(POST_SEM, id, 0, 0, 0, 0, 0);
}

int semClose(uint64_t id)
{
    return syscall(CLOSE_SEM, id, 0, 0, 0, 0, 0);
}