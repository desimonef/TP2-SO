// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>
#include <keyboard.h>
#include <screen.h>
#include <stdint.h>
#include <stddef.h>
#include <naiveConsole.h>
#include <sysCallDispatcher.h>
#include <lib.h>

#include "memManager.h"
#include "schedule.h"
#include "semaphores.h"
#include "pipes.h"

typedef uint64_t (*PSysCall) (uint64_t, uint64_t, uint64_t);


static PSysCall sysCalls[] = {&sysRead, &sysWrite, &sysGetRegs, &sysGetMem, &sysGetDateTime, &sysClearScreen};

uint64_t sysCallDispatcher (uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    if(rdi < 6){    
        PSysCall sysCall = sysCalls[rdi];
        if (sysCall != 0)
            return sysCall(rsi, rdx, r10);
        return 0;
    }
    else{
        switch(rdi){
            //------------- MEMORY SYSCALLS -----------------
            case MALLOC:
                return (uint64_t) malloc(rsi);
            case FREE:
                free((void*)rsi);
                return 0;
            case PRINT_MEM:
                memDump();
                return 0;
            
            //------------ PROCESS SYSCALLS -----------------
            case CREATE_P:
                return addProcess((void (*)(int, char **))rsi, (int)rdx, (char **)r10, (int)r8, (int *)r9);
            case KILL_P:
                return killProcess(rsi);
            case BLOCK_P:
                return blockProcess(rsi);
            case UNBLOCK_P:
                return unblockProcess(rsi);
            case PS:
                processDisplay();
                return 0;
            case CURRENT_P:
                return (uint64_t) getCurrPID();
            case NICE:
                setNewCycle(rsi, (int)rdx);
                return 0;
            case YIELD:
                yield();
                return 0;

            
            // ----------- SEMAPHORE SYSCALLS ---------------
            case OPEN_SEM:
                return (uint64_t) semOpen((uint32_t) rsi, (uint32_t) rdx);
            case WAIT_SEM:
                return (uint64_t) semWait((uint32_t) rsi);
            case POST_SEM:
                return (uint64_t) semPost((uint32_t) rsi);
            case CLOSE_SEM:
                return (uint64_t) semClose((uint32_t) rsi);
            case PRINT_SEMS:
                semStatus();
                return 0;

            // ---------- PIPES SYSCALLS --------------------
            case OPEN_PIPE:
                return (uint64_t) pipeOpen((uint32_t) rsi);
            case READ_PIPE:
                return (uint64_t) pipeRead((uint32_t) rsi);
            case WRITE_PIPE:   
                return (uint64_t) pipeWrite((uint32_t) rsi, (char *) rdx);
            case CLOSE_PIPE:
                return (uint64_t) pipeClose((uint32_t) rsi);
            case PRINT_PIPES:
                dumpPipes();
                return 0;
            case TOTAL_P:
                return 0;
            case TICKS_ELAPSED:
                return ticks_elapsed();
            default:
                return -1;
        }
    }
}

uint64_t sysRead(uint64_t fd, uint64_t buff, uint64_t amount){
    char * castbuff = (char *) buff;
    if(fd != 0) {
        return -1;
    }
    return keyboardRead(castbuff, amount);
    
}

uint64_t sysWrite(uint64_t fd, uint64_t buff, uint64_t count){
    char * castbuff = (char *) buff;
    char color;
    if (fd == 1) {
        color = 0x07;
    }
    else if (fd == 2)
        color = 0x04;
    else{
        ncPrint("Error in file descriptor");
        ncNewline();
        return 0;
    }

    int i;
    for (i = 0; i < count && castbuff[i] != '\0'; i++)
        ncPrintCharColor(castbuff[i], color);

    return i;
}


uint64_t sysGetRegs(uint64_t buff, uint64_t rdx, uint64_t rcx){
    uint64_t * castbuff = (uint64_t *)buff;
    for (int i = 0; i < 15; i++) {
        castbuff[i] = getRegister(i);
    }
    return 0;
}



uint64_t sysGetMem(uint64_t buff, uint64_t address, uint64_t amount){
    char * castbuff = (char *) buff;
	for(int i = 0 ; i < amount; i++){
		int position = address + i;
		castbuff[i] = (char) asmGetByte(position);
	}
    return 0;
}

uint64_t sysGetDateTime(uint64_t id, uint64_t buff, uint64_t rcx){
    char * castbuff = (char *) buff;
    uint64_t data = getDateTime(id);
    int firstDigit = (data >> 4)&0x0F;
    int secondDigit = data&0x0F;
    castbuff[0] = '0' + firstDigit;
    castbuff[1] = '0' + secondDigit;
    castbuff[2] = '\0';
    return data;
}

uint64_t sysClearScreen(uint64_t mode, uint64_t address, uint64_t amount){
    ncClear();
    if (mode == 0){
        initScreen();
    }
    else if(mode == 1){
        initDoubleScreen(0);
    }
    else if(mode == 2){
        initDoubleScreen(1);
    }
    return 0;
}


