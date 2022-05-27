// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "procLib.h"
#include "sysCalls.h"

int createProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd)
{
    return syscall(CREATE_P, (uint64_t)entryPoint, argc, (uint64_t)argv, (int)fg, (uint64_t)fd, 0);
}

int killProc(uint64_t pid)
{
    return syscall(KILL_P, pid, 0, 0, 0, 0, 0);
}

/*
int blockProc(uint64_t pid)
{
    return syscall(BLOCK_P, pid, 0, 0, 0, 0, 0);
}

int unblockProc(uint64_t pid)
{
    return syscall(UNBLOCK_P, pid, 0, 0, 0, 0, 0);
}*/

int getPID()
{
    return syscall(CURRENT_P, 0, 0, 0, 0, 0, 0);
}

/*void niceProc(uint64_t pid, int priority)
{
    syscall(NICE, pid, priority, 0, 0, 0, 0);
}*/

void yieldProc()
{
    syscall(YIELD, 0, 0, 0, 0, 0, 0);
}

/*
void wait(uint64_t pid)
{
    syscall(WAIT, pid, 0, 0, 0, 0, 0);
}*/