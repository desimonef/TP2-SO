
#include "stdlib.h"
#include "userlib.h"
#include "shell.h"
#include "clock.h"

#define MAXLEN 100

void initShell(){
    clearScreen();
    //shellWelcomeMsg();
    shell();
}

void shell() {
    char command[MAXLEN] = {0};
    char arg[MAXLEN] = {0};
    int off = 0;
    while(!off) {
        printf("\nBienvenidos a nuestro precario sistema operativo\n\n");
        //printf("%c : %s\n\n\n", 'a', "TOTO"); //sigue sin reconocer los chars, despues investigar
        printf("\n");
        //printMem("0x000000FF");
        //displayHelpMenu();
        //getDateAndTime();
        printRegisters();
        //printf("\n\n\n");
        //divCeroExceptionTest();
        //opCodeExceptionTest();
        printf("\n");
        //printf("Volvio la ejecucion de la shell");
        //while(1);
        printf("$>");
        while(1);
        //myScanf("%s %s", command, arg);
        //*command = 0;
        //*arg = 0;
        //scanf("%s %s", command, arg);
        //printf("Comando: %s\n\nArgumento: %s", command, arg);
        //execute(command, arg); 
    }
}

void execute(const char * command, const char * arg) {
    if(strcmp(command, "toto") == 0)
        printf("toto");
}

void printRegisters(){
    char * registers[16] = {"RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSI: ", "RDI: ", "RBP: ", "RSP: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: "};
    uint64_t buff[16];
    char hexa[20];
    printf("\n");
    dumpRegs(buff);
    for (int i=0 ;i < 16;i++){
        intToHex(buff[i],hexa,8);
        printf("%s",registers[i]);
        printf("%s\n",hexa);
    }
}


void printMem(char * hexaAddress){
	unsigned char buffer[35]; // Aca guardo los valores de los registros
	char byte[10]; // Guardo cada byte individualmente para imprimir
	dumpMem(buffer, hexToInt(hexaAddress), 32);
	printf("Dump de 32 bytes a partir de la direccion %s :\n", hexaAddress);
	for(int i = 0; i < 32; i++){
		if(i%4 == 0){
			printf("\n");
		}
		printf("    %s ", intToHex(buffer[i], byte, 1));
	}
	printf("\n");
}
/*
void divCeroExceptionTest(){
    int a = 1/0;
}

void opCodeExceptionTest(){
    UDcaller();
}
*/
void displayHelpMenu(){
    printf("\nLos comandos disponibles para ejecucion son:\n\n");
    printf("~help~: muestra el menu de ayuda\n");
    printf("~divex~: se genera una excepcion de division por cero\n");
    printf("~opcodex~: se genera una excepcion de codigo de operacion equivocado\n");
    printf("~printmem~: muestra el contenido de 32 bytes de memoria a partir de\n");
    printf("la direccion hexadecimal provista\n");
    printf("~printregs~: muestra los valores de los registros en el momento de ejecucion\n");
    printf("~datetime~: muestra la hora y la fecha a la hora de ejecutarse\n");
}

void shellWelcomeMsg(){
    printf("          -+#%%%%%%*-\n");              
    printf("        =@*-.......-*@=\n");            
    printf("       -@=....-+*#%%*@@#.\n");          
    printf("       @*...:*@+-:....:+*#:\n");        
    printf("   .:-+@+...*@*=:::.   .:+@=\n");       
    printf(" -@#++#@-...+@**+==------+@+\n");       
    printf(" *#:::#@-....*@#********#@#\n");        
    printf(" @#---@*-.....=*%%%%%%#**@-\n");        
    printf(".@*---@#-:..............:@=\n");        
    printf(".@*---@#--..............-@=\n");        
    printf(" @*---@#---.............+@-\n");        
    printf(" @*---@*-----:::.....:--*@.\n");        
    printf(" =@#+=%%----------------#@\n");         
    printf("  .-+**@------#%%*@*#--=@*\n");         
    printf("      -@+----=@+ +@=---*@:\n");         
    printf("       @*----=@+ :@*===@#\n");          
    printf("      +@%##%**:  :+##*+\n");           
    
}