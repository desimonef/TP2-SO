#include <stdlib.h>
//#include <userfunctions.h>
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
        *command = 0;
        *arg = 0;
        scanf("%s %s", command, arg);
        execute(command, arg);
    }
}

void execute(const char * command, const char * arg) {
    if(strcmp(command, "toto") == 0)
        toto();
}

void toto(){
    printf("TOTO");
}