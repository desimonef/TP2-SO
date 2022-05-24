// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdlib.h"
#include "userlib.h"
#include "shell.h"
#include "clock.h"
#include "test_util.h"
#include "test_mm.h"
#include "test_processes.h"
#include "sysCalls.h"
#include "commands.h"
#include "phylo.h"
#include "procLib.h"
#include "semLib.h"
#include "pipeLib.h"
#include "test_sync.h"

int check_vowel(char a);

void displayHelpMenu();
void divZeroException();
void opCodeException();
void inforeg();
void printMem(char * hexaAddress);
void getDateAndTime();


void mem(int argc, char ** argv){
    syscall(PRINT_MEM, 0, 0, 0, 0, 0, 0);
}

void ps(int argc, char ** argv){
    syscall(PS, 0, 0, 0, 0, 0, 0);
}

int ticksElapsed(){
      return syscall(TICKS_ELAPSED, 0, 0, 0, 0, 0, 0);
}


void waitCycles(int cycles){
      int goal = ticksElapsed() + cycles;
      while (ticksElapsed() < goal);
}

void loop(int argc, char ** argv){
    int pid = syscall(CURRENT_P, 0, 0, 0, 0, 0, 0);
    int obj = -1;
    int counter = 1;
    while (1 && obj == -1){
        waitCycles(10);
        printf("%d\n", pid);
        if(counter == 100)
            obj = 0;
        else{
            counter++;
            if(counter%5 == 0){
                counter -= 2; //Solo para hacerlo tardar un poco mas
            }
        }
    }
}

void kill(int argc, char ** argv){
    int PID = atoi(argv[1]);
    if(PID == -1){
        printf("Invalid PID\n");
        return;
    }
    syscall(KILL_P, PID, 0, 0, 0, 0, 0);
}

void nice(int argc, char ** argv){
    int PID = atoi(argv[1]);
    int prio = atoi(argv[2]);
    if(PID == -1 || prio == -1){
        printf("Invalid parameters. Try again.\n");
        return;
    }
    syscall(NICE, PID, prio, 0, 0, 0, 0);
}

void block(int argc, char ** argv){
    int PID = atoi(argv[1]);
    if(PID == -1){
        printf("Invalid PID\n");
        return;
    }
    syscall(BLOCK_P, PID, 0, 0, 0, 0, 0);
}

void unblock(int argc, char ** argv){
    int PID = atoi(argv[1]);
    if(PID == -1){
        printf("Invalid PID\n");
        return;
    }
    syscall(UNBLOCK_P, PID, 0, 0, 0, 0, 0);
}

void sem(int argc, char ** argv){
    syscall(PRINT_SEMS, 0, 0, 0, 0, 0, 0);
}

void cat(int argc, char ** argv){
    int i = 0;
    while(argv[1][i] != '\0' && argv[1][i] != '\n')
        putchar(argv[1][i++]);
    printf("\n");
}

void wc(int argc, char ** argv){
    int count = 1;
    int i = 0;
    while(argv[1][i] != '\0' && argv[1][i] != '\n'){
        putchar(argv[1][i++]);
        if(argv[1][i++] == '\n')
            count++;
    }
    printf("\nNumber of lines: %d\n", count);
}

int check_vowel(char a){
      if (a >= 'A' && a <= 'Z')
            a = a + 'a' - 'A';
      if (a == 'a' || a == 'e' || a == 'i' || a == 'o' || a == 'u')
            return 1;
      return 0;
}

void filter(int argc, char ** argv){
    printf("\nPalabra a filtrar = ");
    printf(argv[1]);
    printf("\n");
    int i = 0;
    while (argv[1][i] != '\0' && argv[1][i] != '\n'){
        if (!check_vowel(argv[1][i]))
            putchar(argv[1][i]);
        i++;
    }
    printf("\n");
}

void pipe(int argc, char ** argv){
    syscall(PRINT_PIPES, 0, 0, 0, 0, 0, 0);
}

void testPrio(int argc, char ** argv){
    //test_prio();
};

void testMM(int argc, char ** argv){
    test_mm();
};

void testProc(int argc, char ** argv){
    test_processes();
};

void testSync(int argc, char ** argv){
    test_sync();
};
void testNosync(int argc, char ** argv){
    test_no_sync();
};

void helpWrp(int argc, char ** argv){
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

// Functions for wrappers

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

