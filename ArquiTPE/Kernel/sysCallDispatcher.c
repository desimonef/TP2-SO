#include <time.h>
#include <keyboard.h>
#include <stdint.h>
#include <stddef.h>
#include <naiveConsole.h>
#include <sysCallDispatcher.h>
#include <lib.h>

typedef uint64_t (*PSysCall) (uint64_t, uint64_t, uint64_t);


static PSysCall sysCalls[] = {&sysRead, &sysWrite, &sysGetRegs, &sysGetMem, &sysGetDateTime, &sysClearScreen, &sysGetDateTimeBuff};

void sysCallDispatcher (uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx){
    PSysCall sysCall = sysCalls[rdi];
    if (sysCall != 0)
        sysCall(rsi, rdx, rcx);
}

void sysRead (int fd, char * buff, uint16_t amount){
    if(fd != 0) {
        return;
    }
    readFromKeyboard(buff, amount);
}


void sysWrite(int fd, char * buff, uint64_t count){
    /*char color;
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
        */
    for(int i = 0; i < count; i++){
        ncPrintChar(*buff);
        buff++;
    }
}
/*
void sysGetRegs(int fd, char * buff, uint64_t count){
    char * registers[16] = {"RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSI: ", "RDI: ", "RBP: ", "RSP: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: "};
    for (int i = 0; i < 16; i++){
        write(1, registers[i], 5);
        uint64_t reg = getRegister(i);
        char buffer[20];
        char * hexa = intToHexa(reg, buffer, 8);
        write(1, hexa, 10);
        ncNewline();
    }
}
*/

static uint64_t regs[17] = {0};


uint64_t * getRegs(){
    return regs;
}


uint64_t sysGetRegs(uint64_t buffer, uint64_t rdx, uint64_t rcx){
    long long *array = (long long *)buffer;
    for (int i = 0; i < 15; i++) {
        array[i] = regs[i];
    }
    return 0;
}



void sysGetMem(char * buff, uint64_t address, uint64_t amount){
    unsigned char * aux = (unsigned char *) buff;
	for(int i = 0 ; i < amount; i++){
		int position = address + i;
		aux[i] = (unsigned char) asmGetByte(position);
	}
}

uint64_t sysGetDateTime(uint64_t id, uint64_t rdx, uint64_t rcx){
    uint64_t data = getDateTime(id);
    int firstDigit = (data >> 4)*10;
    int secondDigit = data&0x0F;
    uint64_t returnValue = firstDigit + secondDigit;
    ncPrint("Valor (en sys):");
    ncPrintDec(returnValue);
    ncPrint("   ");
    return returnValue;

}

void sysGetDateTimeBuff(uint64_t id, uint64_t buff, uint64_t rcx){
    unsigned char * aux = (unsigned char *) buff;
    uint64_t data = getDateTime(id);
    int firstDigit = (data >> 4)&0x0F;
    int secondDigit = data&0x0F;
    aux[0] = '0' + firstDigit;
    aux[1] = '0' + secondDigit;
    return;
}

void sysClearScreen(char * buff, uint64_t address, uint64_t amount){
    ncClear();
}


