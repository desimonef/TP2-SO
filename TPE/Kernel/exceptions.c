#include "lib.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

void exceptionDispatcher(int exception) {
	char * message;
	if (exception == ZERO_EXCEPTION_ID)
		 message = "Exception: ZERO DIVISION\n";
	else if (exception == INVALID_OPCODE_ID)
		message = "Exception: INVALID OPERATION\n";
	else
		return;
	sysWrite(2, message, strlen(message));
<<<<<<< HEAD
}
=======
}
>>>>>>> 664ba25eeda664f8e265c2f59e0b0abdb58c13f8
