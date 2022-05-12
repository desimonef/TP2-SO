#include "stdlib.h"
#include "userlib.h"
#include "shell.h"
#include "clock.h"
//#include "test_util.h"
//#include "test_mm.h"
#include "test_processes.h"
#include "sysCalls.h"
#include "commands.h"
#include "phylo.h"
#include "procLib.h"
#include "semLib.h"
#include "pipeLib.h"

#define MAXLEN 100
#define MAX_SIZE 150
#define N_ARGS 8
#define N_COMMANDS 26

void helpWrp(int argc, char ** argv);
void regWrp(int argc, char ** argv);
void dumpWrp(int argc, char ** argv);
void datetimeWrp(int argc, char ** argv);
void divZeroWrp(int argc, char ** argv);
void opCodeWrp(int argc, char ** argv);
void clear(int argc, char ** argv);

void (*cmds[])(int, char **) = {&helpWrp, &regWrp, &dumpWrp, &datetimeWrp,
&divZeroWrp, &opCodeWrp, &ps, &mem, &test_prio,
&testMM, &testProc, &testSync, &testNosync, &loop, &kill, &nice,
&block, &unblock, &sem, &pipe, &cat, &wc, &filter, &philosopherProblem, &clear};

char * cmdsNames[] = {"help", "inforeg", "dumpMem", "datetime", "zerodiv", "invopcode",
"ps", "mem", "testprio", "testmm", "testproc", "testsync", "testnosync", "loop", "kill", "nice",
"block", "unblock", "sem", "pipe", "cat", "wc", "filter", "phylo", "clear"};

int off = 0;
static int pipeCounter = 0;

int isItPiped(int argc, char **argv);
void * getCommand(char * str);
int runPipedCommands(int pipeIdx, int argc, char ** argv, int fg);
int runPipeCommand(int argc, char **argv, int fg, int fdIn, int fdOut);
int isBackground(int argc, char ** argv);
void shellNueva();


void helpWrp(int argc, char ** argv){
    printf("\nestoy entrando a la funcion wrapper\n");
    displayHelpMenu();
}

void regWrp(int argc, char ** argv){
    inforeg();
}

void dumpWrp(int argc, char ** argv){
    if(argc < 2){
        printf("Por favor, indicar direccion\n");
        return;
    }
    printMem(argv[1]);
}

void datetimeWrp(int argc, char ** argv){
    getDateAndTime();
}

void divZeroWrp(int argc, char ** argv){
    divZeroException();
}

void opCodeWrp(int argc, char ** argv){
    opCodeException();
}

void clear(int argc, char ** argv){
    screenClear(0);
}

void initShell(int argc, char ** argv){
    screenClear(0);
    /*shellWelcomeMsg();

    int c = -1;
    while (c != '1' && c != '2')
    {
        c = getchar();
    }

    //screenClear(c-'0');
    screenClear(0);*/
    //shellDivertida();
    shellNueva();
}

void shellWelcomeMsg(){
    printf("                    @###########@\n");              
    printf("                   @-------------@\n");            
    printf("                  @------@@@@@@@@@@\n");          
    printf("                 @------@          @\n");        
    printf("             @@@@------@            @\n");       
    printf("            @---@------@            @\n");       
    printf("           @----@-------@          @\n");        
    printf("           @----@#-------@@@@@@@@@@.\n");        
    printf("          .@----@#-----------------@\n");        
    printf("          .@----@#-----------------@\n");        
    printf("           @----@#----------------+@\n");        
    printf("           @----@#----------------*@\n");        
    printf("            @---@#----------------#@\n");         
    printf("             @@@#@------####@-----@\n");         
    printf("                 @-----=@   @-----@\n");         
    printf("                 @-----=@   @-----@\n");          
    printf("                 @######@   @#####@\n\n");
    printf("          ¡Hola!\n"); 
    printf("          Bienvenidos al sistema operativo Among-OS. Por favor, presione una tecla para continuar\n");
}

void shellNueva(){
    printf("Bienvenidos a Among-OS! Si necesita ayuda, ingresar el comando <help>\n");
    char buffer[MAX_SIZE] ={0};
    while(1){
        //emptyBuffer(buffer);
        printf("\n$>");
        scanf("%s", buffer);
        processBuffer(buffer);
    }
}

void processBuffer(char * buffer){
    int argc = 0;
    char * argv[N_ARGS] = {0};
    int fg = 1; //Unless stated otherwise ('&');
    argc = strtok(buffer, argv, ' ', N_ARGS);
    int pipe = isItPiped(argc, argv);
    if(pipe != -1){
        if(pipe == 0 || pipe == argc-1){
            printf("El pipe se debe colocar entre argumentos.");
            return;
        }
        if(runPipedCommands(pipe, argc, argv, fg) == -1){
        //TODO: int runPipedCommands(int pipeIdx, int argc, char ** argv, int fg);
            printf("Error corriendo comandos pipeados\n");
        }
        return;
    }
    if(isBackground(argc, argv) == 0){
        fg = 0;
        argc--;
    }
    int idx = getCommand(argv[0]);
    if(idx == -1){
        printf("Comando invalido\n");
        return;
    }
    createProcess(cmds[idx], argc, argv, fg, 0);
}

void * getCommand(char * str){
    int idx = 0; 
    while(idx < N_COMMANDS){
        if(strcmp(str, cmdsNames[idx]) == 0){
            return idx;
        }
        idx++;
    }
    return -1;
}

int isItPiped(int argc, char **argv)
{
      for (int i = 0; i < argc; i++)
      {
            if (strcmp(argv[i], "|") == 0)
                  return i;
      }
      return -1;
}

int runPipedCommands(int pipeIdx, int argc, char **argv, int fg)
{
    char *currentArgv[N_ARGS];
    int currentArgc = 0;
    uint32_t pids[2];

    int pipe = pipeOpen(pipeCounter++);

    if (pipe == -1)
    {
        printf("Error creating pipe");
        return -1;
    }

    for (int i = pipeIdx + 1, j = 0; i < argc; i++, j++)
    {
        currentArgv[j] = argv[i];
        currentArgc++;
    }

    pids[0] = runPipeCommand(currentArgc, currentArgv, BG, pipe, 1);

    if (pids[0] == -1)
    {
        pipeClose(pipe);
        return -1;
    }

    currentArgc = 0;

    for (int i = 0; i < pipeIdx; i++)
    {
        currentArgv[i] = argv[i];
        currentArgc++;
    }

    pids[1] = runPipeCommand(currentArgc, currentArgv, fg, 0, pipe);

    if (pids[1] == -1)
    {
        return -1;
        pipeClose(pipe);
    }

    int a = -1;
    if (fg == 0)
        semWait(pids[1]);
    pipeWrite(pipe, (char *)&a);
    semWait(pids[0]);
    pipeClose(pipe);

    return 1;
};

int runPipeCommand(int argc, char **argv, int fg, int fdIn, int fdOut)
{

      int fd[2];
      int cmdIdx = getCommand(argv[0]);

      if (cmdIdx == -1)
            return -1;

      fd[0] = fdIn;
      fd[1] = fdOut;

      return createProcess(cmds[cmdIdx], argc, argv, fg, fd);
}

int isBackground(int argc, char ** argv){
    if(argv[argc - 1][0] == '&'){
        return 0;
    }
    return -1;
}

void shellDivertida(){
    printf("Bienvenidos a Among-OS! Si necesita ayuda, ingresar el comando <help>\n");
 
    while(1){
        printf("$>");
        char command[MAX_SIZE] ={0};
        char argument[MAX_SIZE] ={0};
        scanf("%s %s", command, argument);
        if(strcmp(command, "kill") != 0){
            if(strcmp(command, "help") == 0){
                displayHelpMenu();
            }
            else if(strcmp(command, "clear") == 0){
                screenClear(0);
            }
            else if(strcmp(command, "inforeg") == 0){
                char * argv[] = {"inforeg"};
                inforeg(1, argv);
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
                    printf("Falta argumento.\n");
            }
            //TODO: Cuando armemos bien la shell, cambiar esto de los tests. 
            //Es momentaneo para testear
            else if(strcmp(command, "testproc") == 0){
                test_processes();
            }
            else if(strcmp(command, "testt") == 0){
                char * argv[] = {"inforeg"};
                syscall(CREATE_P, (uint64_t) &inforeg, 1, (uint64_t) argv, 0, 1, 0);
            }
            else if(strcmp(command, "testmm") == 0){
                test_mm();
            }
            else{
                printf("No entendi su comando, por favor ingresar nuevamente\n");
            }
        }
        else{
            screenClear(0);
            printf("Gracias por utilizar Among-OS! Vuelva pronto!\n");
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
    printf("~mem~: imprime estado actual de la memoria\n");
    printf("~ps~: imprime procesos\n");
    printf("~loop~: imprime el PID del proceso\n");
    printf("~nice~: cambia prioridad de un proceso\n");
    printf("~kill, block, unblock~: cambia estados de los procesos\n");
    printf("~sem~: imprime semaforos\n");
    printf("~cat~: imprime stdin como lo recibe\n");
    printf("~wc~: cuenta cantidad de lineas del input\n");
    printf("~filter~: filtra vocales del input\n");
    printf("~pipe~: imprime estado actual de los pipes\n");
    printf("~phylo~: implementacion del problema de los filosofos\n");
}

void divZeroException(){
    int prueba = 1/0;
}

void opCodeException(){
    UDcaller();
}

void inforeg(int argc, char ** argv){
    char * registers[16] = {"RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSI: ", "RDI: ", "RBP: ", "RSP: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: "};
    uint64_t buff[16];
    char hexa[20];
    dumpRegs(buff);
    printf("Los valores de los registros son:\n");
    for (int i=0 ;i < 16;i++){
        if(i % 2 == 0){
            printf("\n");
        }
        intToHex(buff[i],hexa,8);
        printf("%s:",registers[i]);
        printf("%s   ",hexa);
    }
    printf("\n");
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
	printf("\n");
}

void getDateAndTime(){
    char auxDaysBuff[3] = {0};
    char auxMonthsBuff[3] = {0};
    char auxYearsBuff[3] = {0};
    days(auxDaysBuff);
    months(auxMonthsBuff);
    years(auxYearsBuff);
    printf("\nLa fecha de hoy es: %s/%s/%s\n", auxDaysBuff, auxMonthsBuff, auxYearsBuff);
    char auxSecondsBuff[3] = {0};
    char auxMinutesBuff[3] = {0};
    char auxHoursBuff[3] = {0};
    seconds(auxSecondsBuff);
    minutes(auxMinutesBuff);
    hours(auxHoursBuff);
    printf("La hora actual es: %s:%s:%s\n", auxHoursBuff, auxMinutesBuff, auxSecondsBuff);
}

