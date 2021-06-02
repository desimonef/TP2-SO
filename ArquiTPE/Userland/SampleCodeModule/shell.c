#include "stdlib.h"
#include "userlib.h"
#include "shell.h"
#include "clock.h"

#define MAXLEN 100
#define MAX_SIZE 150

int off = 0;

void initShell(){
    screenClear();
    //shellWelcomeMsg();
    shell();
    while(1){}
    //shell();
}

void shell(){
    char command[MAXLEN] = {0};
    char arg[MAXLEN] = {0};
    while(off == 0) {
        printf("\nBienvenidos a nuestro precario sistema operativo\n\n");
        //printf("%c : %s\n\n\n", 'a', "TOTO"); //sigue sin reconocer los chars, despues investigar
        printf("\n");
        //printMem("0x000000FF");
        printf("\n");
        displayHelpMenu();
        //getDateAndTime();
        //printf("\n\n\n");
        //divCeroExceptionTest();
        //opCodeExceptionTest();
        printf("\n");
        //printf("Volvio la ejecucion de la shell");
        //while(1);
        printf("$>");
        while(1){};


        //myScanf("%s %s", command, arg);
        //*command = 0;
        //*arg = 0;
        //scanf("%s %s", command, arg);
        //printf("Comando: %s\n\nArgumento: %s", command, arg);
        //execute(command, arg); 
    }
    while(1){
        printf("Volviste a la ejecución de shell\n");
        while(1);
    }
}

void execute(const char * command, const char * arg) {
    if(strcmp(command, "toto") == 0);
        //toto();
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

void divCeroExceptionTest(){
    int a = 1/0;
}

void opCodeExceptionTest(){
    UDcaller();
}

void shellWelcomeMsg(){
    printf("          -+#%%%%%%*-\n");              
    printf("        =@*-.......-*@=\n");            
    printf("       -@=....-+*#%%%@@#.\n");          
    printf("       @%...:%@+-:....:+%#:\n");        
    printf("   .:-+@+...*@*=:::.   .:+@=\n");       
    printf(" -@#++#@-...+@**+==------+@+\n");       
    printf(" %#:::#@-....%@#********#@#\n");        
    printf(" @#---@%-.....=*%%%%%%#**@-\n");        
    printf(".@*---@#-:..............:@=\n");        
    printf(".@*---@#--..............-@=\n");        
    printf(" @*---@#---.............+@-\n");        
    printf(" @%---@%-----:::.....:--*@.\n");        
    printf(" =@#+=%%----------------#@\n");         
    printf("  .-+*%@------#%%%@%#--=@*\n");         
    printf("      -@+----=@+ +@=---*@:\n");         
    printf("       @*----=@+ :@*===@#\n");          
    printf("      +@%##%%*:  :+##*+\n");
}


/*
void shell(){
    printf("\nBienvenidos a among-OS! Si necesita ayuda, ingresar el comando <help>\n");
    printf("Por favor, indique con ~izq~ o ~der~ su modulo para comenzar\n");
    char module[5] = {0};
    scanf("%s", module);
    if(strcmp(module, "izq") == 0){
        initLeftScreen();
    }
    else if(strcmp(module, "der") == 0){
        initRightScreen();
    }
    else{
        printf("Su comando fue invalido. Inicializando modulo izquierdo:\n");
    }
    while(1){
        printf("$>");
        char command[MAX_SIZE] ={0};
        char argument[MAX_SIZE] ={0};
        scanf("%s %s", command, argument);
        if(strcmp(command, "kill") != 0){
            if(strcmp(command, "help") == 0){
                displayHelpMenu();
            }
            else if(strcmp(command, "printmem") == 0){
                if(isValidHexaAddress(argument) == 1){
                    printMem(argument);
                }
                else{
                    printf("No es una dirección hexadecimal valida.\n");
                }
            }
            else if(strcmp(command, "divex") == 0){
                divCeroExceptionTest();
            }
            else if(strcmp(command, "opcodeex") == 0){
                opCodeExceptionTest();
            }
            else if(strcmp(command, "clearleft") == 0){
                //aca falta insertar una funcion para limpiar pantalla izq
            }
            else if(strcmp(command, "cleanright") == 0){
                //aca falta insertar una funcion para limpiar pantalla der
            }
        }
        else{
            return;
        }

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