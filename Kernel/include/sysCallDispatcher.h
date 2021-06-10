#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>

uint64_t sysCallDispatcher (uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx);
uint64_t sysRead(uint64_t fd, uint64_t buff, uint64_t amount);
uint64_t sysWrite(uint64_t fd, uint64_t buff, uint64_t count);
uint64_t sysGetRegs(uint64_t buff, uint64_t rdx, uint64_t rcx);
uint64_t sysGetMem(uint64_t buff, uint64_t address, uint64_t amount);
uint64_t sysGetDateTime(uint64_t id, uint64_t buff, uint64_t rcx);
uint64_t sysClearScreen(uint64_t mode, uint64_t address, uint64_t amount);

#endif
