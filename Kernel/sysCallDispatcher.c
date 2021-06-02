#include <time.h>
#include <keyboard.h>
#include <stdint.h>
#include <stddef.h>
#include <naiveConsole.h>
#include <sysCallDispatcher.h>
#include <lib.h>

typedef uint64_t (*PSysCall) (uint64_t, uint64_t, uint64_t);


static PSysCall sysCalls[] = {&sysRead, &sysWrite, &sysGetRegs, &sysGetMem, &sysGetDateTime, &sysClearScreen};

void sysCallDispatcher (uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx){
    PSysCall sysCall = sysCalls[rax];
    if (sysCall != 0)
        sysCall(rdi, rsi, rdx);
}

void sysRead (int fd, char * buff, uint16_t amount){
    if(fd != 0) {
        return;
    }
    readFromKeyboard(buff, amount);
}

void sysWrite(int fd, char * buff, uint64_t count){
    char color;
    if (fd == 1)
        color = 0x07;
    else if (fd == 2)
        color = 0x04;
    else{
        ncPrint("Error in file descriptor");
        ncNewline();
        return;
    }
    int i;
    for (i = 0; i < count || buff[i] == '\0'; i++)
        ncPrintCharColor(buff[i], color);
}

void sysGetRegs(uint64_t * buff, uint64_t rdx, uint64_t rcx){
    for (int i = 0; i < 16; i++)
        buff[i] = getRegister(i);
}

void sysGetMem(char * buff, uint64_t address, uint64_t amount){
	for(int i = 0 ; i < amount; i++)
		buff[i] = asmGetByte(address + i);
}

void sysGetDateTime(uint64_t id, uint64_t buff, uint64_t rcx){
    unsigned char * aux = (unsigned char *) buff;
    uint64_t data = getDateTime(id);
    int firstDigit = (data >> 4)&0x0F;
    int secondDigit = data&0x0F;
    aux[0] = '0' + firstDigit;
    aux[1] = '0' + secondDigit;
    return;
}

void sysClearScreen(uint64_t rsi, uint64_t rdx, uint64_t rcx){
    ncClear();
}