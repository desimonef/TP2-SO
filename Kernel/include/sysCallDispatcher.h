#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>

#define MALLOC 6
#define FREE 7
#define CREATE_P 8
#define KILL_P 9
#define BLOCK_P 10
#define UNBLOCK_P 11
#define COUNT_P 12
#define PS 13
#define CURRENT_P 14
#define NICE 15
#define YIELD 16

//  SEMAPHORES SYSCALLS
#define OPEN_SEM 17
#define WAIT_SEM 18
#define POST_SEM 19
#define CLOSE_SEM 20
#define PRINT_SEMS 21

//PIPES
#define OPEN_PIPE 22
#define READ_PIPE 23
#define WRITE_PIPE 24
#define CLOSE_PIPE 25
#define PRINT_PIPES 26

//Me olvide :/
#define TOTAL_P 27
#define TICKS_ELAPSED 28
#define PROCESS_COUNT 29
#define GET_PIPES 30
#define PRINT_MEM 31
#define NEW_WRITE 32
#define NEW_READ 33
#define READ_FROM 34
#define WRITE_TO 35

uint64_t sysCallDispatcher (uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9);
uint64_t sysRead(uint64_t fd, uint64_t buff, uint64_t amount);
uint64_t sysWrite(uint64_t fd, uint64_t buff, uint64_t count);
uint64_t sysGetRegs(uint64_t buff, uint64_t rdx, uint64_t rcx);
uint64_t sysGetMem(uint64_t buff, uint64_t address, uint64_t amount);
uint64_t sysGetDateTime(uint64_t id, uint64_t buff, uint64_t rcx);
uint64_t sysClearScreen(uint64_t mode, uint64_t address, uint64_t amount);

uint64_t newWrite(uint64_t fd, uint64_t buff, uint64_t count);
uint64_t newRead(uint64_t fd, uint64_t buff, uint64_t amount);

#endif
