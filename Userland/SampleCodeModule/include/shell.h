#ifndef SHELL_H
#define SHELL_H

void initShell();
void shell();
void execute(const char * command, const char * arg);
void shellWelcomeMsg();
void printMem(char * hexaAddress);
void printRegisters();
void divCeroExceptionTest();
void opCodeExceptionTest();
void displayHelpMenu();

#endif