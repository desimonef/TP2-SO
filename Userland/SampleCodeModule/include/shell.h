#ifndef SHELL_H
#define SHELL_H

void initShell(int argc, char ** argv);
void shellWelcomeMsg();
void shellDivertida();
void displayHelpMenu();
void divZeroException();
void opCodeException();
void inforeg();
void printMem(char * hexaAddress);
void getDateAndTime();

#endif