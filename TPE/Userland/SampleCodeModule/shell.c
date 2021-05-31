#include "stdlib.h"
#define MAXLEN 100

void initShell(){
    printf("Bienvenidos a nuestro precario sistema operativo\n");
    shell();
}

void shell() {
    char command[MAXLEN] = {0};
    char arg[MAXLEN] = {0};
    while(1) {
        printf("$ ");
        scanf("%s %s", command, arg);
    }
}