#include "lib.h"
#include "sysCallDispatcher.h"
#include "naiveConsole.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6


void exceptionDispatcher(int exception, uint64_t rsp) {
	char * message;
	if (exception == ZERO_EXCEPTION_ID)
		 message = "Exception: ZERO DIVISION\n";
	else if (exception == INVALID_OPCODE_ID)
		message = "Exception: INVALID OPERATION\n";
	else
		return;
	sysWrite(2, message, strlen(message));
	ncNewline();
	ncPrint("RSP: ");
	char auxHexa[20] ={0};
	intToHexa((uint64_t)(rsp + 3*8), auxHexa, 8);
	sysWrite(2, auxHexa, 10);
	printRegisters();
	sleep(5);
}

void printRegisters(){
    char * registers[16] = {"RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSI: ", "RDI: ", "RBP: ", "RSP: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: "};
    for (int i=0 ;i < 16;i++){
        if(i % 2 == 0){
            ncNewline();
        }
		char hexa[20];
		uint64_t aux = getRegister(i);
        intToHexa(aux,hexa,8);
        ncPrint(registers[i]);
        ncPrint(hexa);
		ncPrint(" ");
    }
    ncNewline();
}