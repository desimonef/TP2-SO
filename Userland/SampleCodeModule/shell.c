// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

#define MAXLEN 100
#define MAX_SIZE 150
#define N_ARGS 8
#define MAX_ARGS 4
#define N_COMMANDS 25


void (*cmds[])(int, char **) = {&helpWrp, &regWrp, &dumpWrp, &datetimeWrp,
&divZeroWrp, &opCodeWrp, &ps, &mem, 
&testPrio, &testMM, &testProc, &testSync, 
&testNosync, &loop, &kill, &nice,
&block, &unblock, &sem, &pipe, 
&cat, &wc, &filter, &runPhilos, 
&clear};

char * cmdsNames[] = {"help", "inforeg", "dumpMem", "datetime", 
"zerodiv", "invopcode", "ps", "mem", 
"testprio", "testmm", "testproc", "testsync", 
"testnosync", "loop", "kill", "nice",
"block", "unblock", "sem", "pipe", 
"cat", "wc", "filter", "phylo", 
"clear"};

int off = 0;
static int pipeCounter = 2;

void processBuffer(char * buffer);
int isItPiped(int argc, char **argv);
int getCommand(char * str);
int execPipes(int pipeIdx, int argc, char ** argv, int fg);
int pipeCmd(int argc, char **argv, int fg, int fdIn, int fdOut);
void shellNueva();


void initShell(int argc, char ** argv){
    screenClear(0);
    shellWelcomeMsg();

    int c = -1;
    while (c == -1){
        c = getchar();
    }
    screenClear(0);
    shellNueva();
}

void shellWelcomeMsg(){
    printf("                    @###########@          .-. .  . .-. . . .-.   .-. .-. \n");              
    printf("                   @-------------@         |-| |\/| | | |\| |..   | | `-. \n");            
    printf("                  @------@@@@@@@@@@        ` ' '  ` `-' ' ` `-'   `-' `-'  \n");          
    printf("                 @------@          @          \n");        
    printf("             @@@@------@            @          \n");       
    printf("            @---@------@            @          \n");       
    printf("           @----@-------@          @          \n");        
    printf("           @----@#-------@@@@@@@@@@.          \n");        
    printf("          .@----@#-----------------@          \n");        
    printf("          .@----@#-----------------@          \n");        
    printf("           @----@#----------------+@          \n");        
    printf("           @----@#----------------*@          \n");        
    printf("            @---@#----------------#@          \n");         
    printf("             @@@#@------####@-----@          \n");         
    printf("                 @-----=@   @-----@          \n");         
    printf("                 @-----=@   @-----@          \n");          
    printf("                 @######@   @#####@          \n\n");
    printf("¡Hola!\n"); 
    printf("Bienvenidos al sistema operativo Among-OS. Por favor, presione una tecla para continuar\n");
}


void shellNueva(){
    printf("Bienvenidos a Among-OS! Si necesita ayuda, ingresar el comando <help>\n");
    char buffer[MAX_SIZE] ={0};
    while(1){
        printf("$>");
        cleanBuffer(buffer);
        //int c = getchar();
        
        char * parsed = getCommandWithArgsBis(buffer);
        processBuffer(parsed);
    }
}

void processBuffer(char * buffer){
    int argc = 0;
    char *argv[MAX_ARGS] = {0};
    tokenize(' ', &argc, argv, buffer, MAX_ARGS);

    int fg = 1; 
    int pipe = isItPiped(argc, argv);
    if(pipe != -1){
        if(pipe == 0 || pipe == argc-1){
            printf("El pipe se debe colocar entre argumentos.");
            return;
        }
        if(execPipes(pipe, argc, argv, fg) == -1){
            printf("Error corriendo comandos pipeados\n");
        }
        return;
    }
    if(isBackground() == 1){
        fg = 0;
    }
    int idx = getCommand(argv[0]);
    char buff[MAXLEN]; // Nota: Esta declaración no aporta nada al código. Sin embargo, sacarla
                       // no permite reconocer los comandos. Se queda.
    if(idx == -1){
        printf("Comando invalido\n");
        return;
    }
    createProcess(cmds[idx], argc, argv, fg, 0);
}

int getCommand(char * str){
    int idx = 0; 
    while(idx < N_COMMANDS){
        if(strcmp(str, cmdsNames[idx]) == 0){
            return idx;
        }
        idx++;
    }
    return -1;
}

int isItPiped(int argc, char **argv){
      for (int i = 0; i < argc; i++){
            if (strcmp(argv[i], "|") == 0) {
                return i;
            }
                  
      }
      return -1;
}

int execPipes(int pipeIdx, int argc, char **argv, int fg)
{
    char *currentArgv[N_ARGS];
    int currentArgc = 0;
    uint32_t pids[2];

    int pipe = pipeOpen(pipeCounter++);

    if (pipe == -1){
        printf("Error creating pipe");
        return -1;
    }

    for (int i = pipeIdx + 1, j = 0; i < argc; i++, j++){
        currentArgv[j] = argv[i];
        currentArgc++;
    }


    pids[0] = pipeCmd(currentArgc, currentArgv, BG, 0, pipe);

    if (pids[0] == -1){
        pipeClose(pipe);
        return -1;
    }

    currentArgc = 0;

    for (int i = 0; i < pipeIdx; i++){
        currentArgv[i] = argv[i];
        currentArgc++;
    }

    pids[1] = pipeCmd(currentArgc, currentArgv, fg, pipe, 1);

    if (pids[1] == -1){
        pipeClose(pipe);
        return -1;
    }

    int a = -1;
    if (fg == 0)
        semWait(pids[1]);

    pipeWrite(pipe, (char *)&a);
    semWait(pids[0]);
    pipeClose(pipe);

    return 1;
};

int pipeCmd(int argc, char **argv, int fg, int fdIn, int fdOut)
{
      int fd[2];
      int cmdIdx = getCommand(argv[0]);

      if (cmdIdx == -1)
            return -1;

      fd[0] = fdIn;
      fd[1] = fdOut;


      return createProcess(cmds[cmdIdx], argc, argv, fg, fd);
}
