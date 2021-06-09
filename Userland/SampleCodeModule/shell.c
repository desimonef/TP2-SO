#include "stdlib.h"
#include "userlib.h"
#include "shell.h"
#include "clock.h"

#define MAXLEN 100
#define MAX_SIZE 150

int off = 0;

void initShell(){
    screenClear(0);
    shellWelcomeMsg();

    int c = -1;
    while (c == -1)
    {
        c = getchar();
    }

    screenClear(c-'0');
    shellDivertida();
    //shell();
}

void printMem(char * hexaAddress){
	char buffer[MAXLEN] ={0}; // Aca guardo los valores de los registros
	char byte[MAXLEN] = {0}; // Guardo cada byte individualmente para imprimir
	dumpMem(buffer, hexToInt(hexaAddress), 32);
	printf("Dump de 32 bytes a partir de la direccion %s :\n", hexaAddress);
	for(int i = 0; i < 32; i++){
		if(i%4 == 0){
			printf("\n");
		}
		printf("    %s ", intToHex(buffer[i], byte, 1));
	}
	printf("\n\n");
}

void divCeroException(){
    int prueba = 1/0;
}

void opCodeExceptionTest(){
    UDcaller();
}

void shellWelcomeMsg(){
    /*
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
    printf("      +@%##%%*:  :+##*+\n");*/
    printf("Hola! Bienvenidos al sistema operativo Among-OS. Por favor, presione 1 o 2 para elegir que shell");
    printf("quiere correr.\n\n1:Shell superior\n2:Shell inferior\n");
}


void shellDivertida(){
    printf("\nBienvenidos a among-OS! Si necesita ayuda, ingresar el comando <help>\n");
    //printf("Por favor, indique con ~izq~ o ~der~ su modulo para comenzar\n");
    
    /*
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
    */
    while(1){
        printf("$>");
        char command[MAX_SIZE] ={0};
        char argument[MAX_SIZE] ={0};
        scanf("%s %s", command, argument);
        if(strcmp(command, "kill") != 0){
            if(strcmp(command, "help") == 0){
                displayHelpMenu();
            }
            else if(strcmp(command, "inforeg") == 0){
                printRegisters();
            }
            else if(strcmp(command, "divex") == 0){
                divCeroException();
            }
            else if(strcmp(command, "opcodeex") == 0){
                opCodeExceptionTest();
            }
            else if(strcmp(command, "datetime") == 0){
                getDateAndTime();
            }
            else if(strcmp(command, "printmem") == 0){
                if (argument != 0)
                    if (argument[0] == '0' && (argument[1] == 'x' || argument[1] == 'X'))
                        printMem(argument);
                    else
                        printf("Argumento incorrecto. Ingrese la direccion con formato 0x seguido de la\ndireccion\n");
                else
                    printf("Falta argumento.\n");
            }
            else{
                printf("\nNo entendi su comando, por favor ingresar nuevamente\n");
            }
        }
        else{
            printf("\nGracias por utilizar among-OS! Vuelva pronto!\n");
            return;
        }
    }
}

void displayHelpMenu(){
    printf("\nLos comandos disponibles para ejecucion son:\n\n");
    printf("~help~: muestra el menu de ayuda\n");
    printf("~kill~: termina la ejecución del SO\n");
    printf("~divex~: se genera una excepcion de division por cero\n");
    printf("~opcodex~: se genera una excepcion de codigo de operacion equivocado\n");
    printf("~printmem~: muestra el contenido de 32 bytes de memoria a partir de\n");
    printf("la direccion hexadecimal provista\n");
    printf("~inforeg~: muestra los valores de los registros en el momento de ejecucion\n");
    printf("~datetime~: muestra la hora y la fecha a la hora de ejecutarse\n\n");
}

void getDateAndTime(){
    char auxDaysBuff[3] = {0};
    char auxMonthsBuff[3] = {0};
    char auxYearsBuff[3] = {0};
    testingDays(auxDaysBuff);
    testingMonths(auxMonthsBuff);
    testingYears(auxYearsBuff);
    printf("\nLa fecha de hoy es: %s/%s/%s\n\n", auxDaysBuff, auxMonthsBuff, auxYearsBuff);
    char auxSecondsBuff[3] = {0};
    char auxMinutesBuff[3] = {0};
    char auxHoursBuff[3] = {0};
    testingSeconds(auxSecondsBuff);
    testingMinutes(auxMinutesBuff);
    testingHours(auxHoursBuff);
    printf("La hora actual es: %s:%s:%s\n\n", auxHoursBuff, auxMinutesBuff, auxSecondsBuff);
}

void printRegisters(){
    char * registers[16] = {"RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSI: ", "RDI: ", "RBP: ", "RSP: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: "};
    uint64_t buff[16];
    char hexa[20];
    dumpRegs(buff);
    printf("\n\nLos valores de los registros son:\n\n");
    for (int i=0 ;i < 16;i++){
        if(i % 2 == 0){
            printf("\n");
        }
        intToHex(buff[i],hexa,8);
        printf("%s:",registers[i]);
        printf("%s   ",hexa);
    }
    printf("\n\n");
}

