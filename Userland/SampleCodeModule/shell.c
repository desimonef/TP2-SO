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
    while (c != '1' && c != '2')
    {
        c = getchar();
    }

    screenClear(c-'0');
    shellDivertida();
    //shell();
}

void shellWelcomeMsg(){
    printf("          @###########@\n");              
    printf("         @-------------@\n");            
    printf("        @------@@@@@@@@@@\n");          
    printf("       @------@          @\n");        
    printf("   @@@@------@            @\n");       
    printf("  @---@------@            @\n");       
    printf(" @----@-------@          @\n");        
    printf(" @----@#-------@@@@@@@@@@.\n");        
    printf(".@----@#-----------------@\n");        
    printf(".@----@#-----------------@\n");        
    printf(" @----@#----------------+@\n");        
    printf(" @----@#----------------*@\n");        
    printf("  @---@#----------------#@\n");         
    printf("   @@@#@------####@-----@\n");         
    printf("       @-----=@   @-----@\n");         
    printf("       @-----=@   @-----@\n");          
    printf("       @######@   @#####@\n\n");
    printf("Hola! Bienvenidos al sistema operativo Among-OS. Por favor, presione 1 o 2 para elegir que shell\n");
    printf("quiere correr.\n\n1:Shell superior\n2:Shell inferior\n");
}


void shellDivertida(){
    printf("\nBienvenidos a among-OS! Si necesita ayuda, ingresar el comando <help>\n");
 
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
                inforeg();
            }
            else if(strcmp(command, "zerodiv") == 0){
                divZeroException();
            }
            else if(strcmp(command, "invopcode") == 0){
                opCodeException();
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
                    printf("\nFalta argumento.\n");
            }
            else{
                printf("\nNo entendi su comando, por favor ingresar nuevamente\n");
            }
        }
        else{
            screenClear(0);
            printf("\nGracias por utilizar among-OS! Vuelva pronto!\n");
            sleep(5);
            return;
        }
    }
}

void displayHelpMenu(){
    printf("\nLos comandos disponibles para ejecucion son:\n\n");
    printf("~help~: muestra el menu de ayuda\n");
    printf("~kill~: termina la ejecución del SO\n");
    printf("~zerodiv~: se genera una excepcion de division por cero\n");
    printf("~invopcode~: se genera una excepcion de codigo de operacion equivocado\n");
    printf("~inforeg~: muestra los valores de los registros en el momento de ejecucion\n");
    printf("~printmem~: muestra el contenido de 32 bytes de memoria a partir de\n");
    printf("la direccion hexadecimal provista\n");
    printf("~datetime~: muestra la hora y la fecha a la hora de ejecutarse\n");
}

void divZeroException(){
    int prueba = 1/0;
}

void opCodeException(){
    UDcaller();
}

void inforeg(){
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

void getDateAndTime(){
    char auxDaysBuff[3] = {0};
    char auxMonthsBuff[3] = {0};
    char auxYearsBuff[3] = {0};
    days(auxDaysBuff);
    months(auxMonthsBuff);
    years(auxYearsBuff);
    printf("\nLa fecha de hoy es: %s/%s/%s\n\n", auxDaysBuff, auxMonthsBuff, auxYearsBuff);
    char auxSecondsBuff[3] = {0};
    char auxMinutesBuff[3] = {0};
    char auxHoursBuff[3] = {0};
    seconds(auxSecondsBuff);
    minutes(auxMinutesBuff);
    hours(auxHoursBuff);
    printf("La hora actual es: %s:%s:%s\n\n", auxHoursBuff, auxMinutesBuff, auxSecondsBuff);
}

