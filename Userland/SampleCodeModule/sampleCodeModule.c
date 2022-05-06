#include "shell.h"
#include "sysCalls.h"
#include "stdlib.h"

char * v = (char*)0xB8000 + 79 * 2;

int main() {

	screenClear(0);
	printf("Im in preshell");
	char * argv[] = {"Shell"};
	syscall(CREATE_P, (uint64_t)&initShell, 1, (uint64_t)argv, 1, 0, 0);


	printf("Hasta la próxima!");
	return 0xDEADBEEF;
}