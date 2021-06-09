#include <time.h>
#include <keyboard.h>
#include <screen.h>
#include <stdint.h>
#include <stddef.h>
#include <naiveConsole.h>
#include <sysCallDispatcher.h>
#include <lib.h>

typedef uint64_t (*PSysCall) (uint64_t, uint64_t, uint64_t);


static PSysCall sysCalls[] = {&sysRead, &sysWrite, &sysGetRegs, &sysGetMem, &sysGetDateTime, &sysClearScreen, &sysGetDateTime};

uint64_t sysCallDispatcher (uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx){
    PSysCall sysCall = sysCalls[rdi];
    if (sysCall != 0)
        return sysCall(rsi, rdx, rcx);
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

    /*
    int inserted = 0;
    while(count > 0) {
        if(*castbuff == '\0' || *castbuff==-1)
            break;
        else
            ncPrintChar(*castbuff);
        castbuff++;
        inserted++;
        count--;
    }
    */
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


