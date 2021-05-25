#include <time.h>
#include <keyboard.h>
#include <stdint.h>
#include <stddef.h>
#include <naiveConsole.h>
#include <sysCallDispatcher.h>

typedef uint64_t (*PSysCall)(uint64_t, uint64_t, uint64_t);


//static PSysCall sysCalls[255] = {NULL, &write};

void write(unsigned int fd, const char * buff, size_t count){
    char att;
    if (fd == 1)
        att = 0x07;
    else if (fd == 2)
        att = 0x04;
    else{
        ncPrintAtt("Error in file descriptor", 0x07, 0);
        ncNewline();
        return;
    }
    int i;
    for (i = 0; buff[i] && i < count; i++)
        ncPrintCharAtt(buff[i], att);
    ncNewline();
}

void sysCallDispatcher (uint64_t id, uint64_t arg1, uint64_t arg2, uint64_t agr3){
    switch (id){
    case 1:
        write(arg1, (char *) arg2, agr3);
        break;
    }
    /*PSysCall sysCall = sysCalls[rax];
    if (sysCall != 0)
        return sysCall(rdi, rsi, rdx);
    return 0;
    */
}
