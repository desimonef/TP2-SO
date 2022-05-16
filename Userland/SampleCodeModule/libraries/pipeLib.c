// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pipeLib.h"
#include "sysCalls.h"
#include <stdint.h>

uint64_t pipeOpen(uint64_t id)
{
    return syscall(OPEN_PIPE, id, 0, 0, 0, 0, 0);
}

int pipeRead(uint64_t id)
{
    return syscall(READ_PIPE, id, 0, 0, 0, 0, 0);
}

int pipeWrite(uint64_t id, char *str)
{
    return syscall(WRITE_PIPE, id, (uint64_t)str, 0, 0, 0, 0);
}

int pipeClose(uint64_t id)
{
    return syscall(CLOSE_PIPE, id, 0, 0, 0, 0, 0);
}

int printPipes()
{
    return syscall(PRINT_PIPES, 0, 0, 0, 0, 0, 0);
}