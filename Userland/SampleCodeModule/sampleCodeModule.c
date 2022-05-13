// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "shell.h"
#include "sysCalls.h"
#include "stdlib.h"

char * v = (char*)0xB8000 + 79 * 2;

int main() {

	char * argv[] = {"Shell"};
	syscall(CREATE_P, (uint64_t)&initShell, 1, (uint64_t)argv, 1, 0, 0);
	
	return 0xDEADBEEF;
}