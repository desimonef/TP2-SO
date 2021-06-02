#include "lib.h"
#include "sysCallDispatcher.h"

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
	hold(5);
}