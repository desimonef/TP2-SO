#ifndef SHELL_H
#define SHELL_H

void shellWelcomeMsg();
void initShell();
void shell();
void execute(const char * command, const char * arg);

void printMem(char * hexaAddress);

#endif