// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <schedule.h>
#include <lib.h>
#include <memManager.h>
#include <stddef.h>
#include <interrupts.h>
#include "lib.h"
#include <stdio.h>

#define STACK_SIZE (4 * 1024)
#define MAX_PRIORITY 40
#define MAX_LEN 30

typedef struct PNode{
      struct PNode * next;

      uint64_t pid;
      uint64_t ppid;

      State state;

      void * rsp;
      void * rbp;

      int fd[2];
      int priority;
      int fg;

      char name[NAME_SIZE];
      int argc;
      char ** argv;
      
} PNode;

typedef struct PList{
      PNode * first;
      PNode * last;

      uint32_t size;
      uint32_t prepared;
} PList;

// General declarations

static void haltFunc(int argc, char **argv);
void emptyBuff(char * buff);
static void createStackFrame(void (*entryPoint)(int, char **), int argc, char **argv, void *rbp);
static int frame(PNode * newP);
static uint64_t getPID();
static void wrapper(void (*entryPoint)(int, char **), int argc, char **argv);
static void exit();
static void freeP(PNode *process);
static PNode * getProcess(uint64_t pid);

// Queue declarations

static void pEnqueue(PNode *newP);
static PNode * pDequeue();


static PNode * currentP;
static PList * processQueue;
static uint64_t PID = 0;
static uint64_t ticks;
static PNode * haltP;

//    ----------------------------
//    |                          |
//    |     General scheduling   |
//    |        functions         |
//    |                          |
//    ----------------------------

void initScheduler(){
      processQueue = malloc(sizeof(PList));
      if (processQueue == NULL){
            ncPrint("Error initializing scheduler.");
            return;
      }
      
      processQueue->first = NULL;
      //último y primero apuntan al mismo lugar*/
      processQueue->last = processQueue->first;
      processQueue->size = 0;
      //ambas colas comienzan en cero*/
      processQueue->prepared = 0;


      //preparo función halt
      char *argv[] = {"hlt"};
      newProcess(&haltFunc, 1, argv, 0, 0);
      haltP = pDequeue();
}

int newProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd){


      if (entryPoint == NULL)
            return -1;

      PNode * newP = malloc(sizeof(PNode));
      if (newP == NULL){
            return -1;
      }

      strcpy(argv[0], newP->name);

      if (fg > 1 || fg < 0){
            ncPrint("[Kernel] ERROR: Error creating process, fg value out of bounds");
            return -1;
      }

      if(frame(newP) == -1){
            return -1;
      }

      newP->pid = getPID();

      if(currentP == NULL){
            newP->ppid = 0;
            newP->fg = fg;
      }
      else{
            newP->ppid = currentP->pid;
            newP->fg = currentP->fg ? fg : 0;
      }

      newP->priority = newP->fg ? 1 : 2;

      newP->fd[0] = fd ? fd[0] : 0;
      newP->fd[1] = fd ? fd[1] : 1;
      newP->state = READY;

      char ** newArgvs = args(argv, argc);
      newP->argc = argc;
      newP->argv = newArgvs;
      createStackFrame(entryPoint, argc, newArgvs, newP->rbp);
      pEnqueue(newP);
      
      if (newP->fg && newP->ppid){
            block(newP->ppid);
      }

      return newP->pid;
}

static int frame(PNode * newP) {
      newP->rbp = malloc(STACK_SIZE);
      if (newP->rbp == NULL){
            ncPrint("[Kernel] ERROR: Malloc failure.");
            return -1;
      }

      newP->rbp = (void *)((char *)newP->rbp + STACK_SIZE - 1);
      newP->rsp = (void *)((StackFrame *)newP->rbp - 1);

      return 0;
}

void *scheduler(void * rsp){
 
      //HASTA ACÁ
      //si el proceso actual está listo y tiene ciclos, que siga
            if (currentP->state == READY && ticks > 0){
                  ticks--;
                  return rsp;
            }

            currentP->rsp = rsp;
            if (currentP->pid != haltP->pid){
                  if(currentP->state == KILLED) {
                        PNode * fatherProcess = getProcess(currentP->ppid);
                        if(fatherProcess != NULL && fatherProcess->state == BLOCKED) 
                              unblock(fatherProcess->pid);
                        freeP(currentP);
                  } else {
                        pEnqueue(currentP);
                  }
            }
      
      if (processQueue->prepared > 0){
            PNode * aux;
            do {
                  aux = pDequeue();
                  if (aux->state == KILLED){
                        freeP(aux);
                  }
                  if (aux->state == BLOCKED){
                        pEnqueue(aux);
                  }
            }
            while (aux->state != READY);
            currentP = aux;
            ticks = currentP->priority;
      }
      else{
            currentP = haltP; // No tenemos procesos listos --> Haltea el kernel
      }
      
      return currentP->rsp;
}



static void freeP(PNode * process){
      free((void *)((char *)process->rbp - STACK_SIZE + 1));
      for (int i = 0; i < process->argc; i++){
            free(process->argv[i]);
      }
      free(process->argv);
      
      free((void *)process);
}

static PNode * getProcess(uint64_t pid){
      // Chequeamos si es el que está corriendo
      if (currentP != NULL && currentP->pid == pid){
            return currentP;
      }
      PNode * aux = processQueue->first;
      for (; aux != NULL; aux = aux->next){
            if (aux->pid == pid){
                  return aux;
            }
      }
      return NULL;
}

void changePriority(uint64_t pid, int priority){
      if (priority < 0){
            ncPrint("[Kernel] WARNING: Priority required is invalid. Setting to minimum.");
            priority = 0;
      }
      if (priority > MAX_PRIORITY){
            ncPrint("[Kernel] WARNING: Priority required is invalid. Setting to maximum.");
            priority = MAX_PRIORITY;
      }
      PNode * aux = getProcess(pid);
      if (aux != NULL){
            uint64_t prevPriority = aux->priority;
            aux->priority = priority;
            ncPrint("New priority set for process with PID: ");
            ncPrintDec(pid);
            ncPrint(" from ");
            ncPrintDec(prevPriority);
            ncPrint(" to ");
            ncPrintDec(priority);
            ncNewline();
      }
      else{
            ncPrint("[Kernel] ERROR: Process PID does not match with any existing processQueue. Returning.");
            ncNewline();
      }
}

void yield(){
      ticks = 0;
      _timerTick();
}

//    ----------------------------
//    |                          |
//    |     Queue functions      |
//    |                          |
//    |                          |
//    ----------------------------

static void pEnqueue(PNode * newP){ 
      if (processQueue->size == 0){ //soy el primero?
            processQueue->first = newP;
            processQueue->last = processQueue->first;
      }
      else{
            processQueue->last->next = newP;
            newP->next = NULL;
            processQueue->last = newP;
      }

      processQueue->size++;
      
      if (newP->state == READY){
            processQueue->prepared++;
      }
      
}

static PNode * pDequeue(){

      
      PNode * aux = processQueue->first;
      processQueue->size--;
      
      if (aux->state == READY)
            processQueue->prepared--;


      processQueue->first = processQueue->first->next;
      return aux;
}

//    ----------------------------
//    |                          |
//    |     Status functions     |
//    |                          |
//    |                          |
//    ----------------------------

char * stateName(State state){
      if(state == READY)
            return "Ready";
      else if(state == BLOCKED)
            return "Blocked";
      else
            return "Killed";
}

static uint64_t changeState(uint64_t pid, State newState){
      PNode * process = getProcess(pid);
      if (process == NULL || process->state == KILLED){
            ncPrint("[Kernel] ERROR: Process PID is not valid.");
            return -1;
      }
      if (process == currentP){
            process->state = newState;
            return process->pid;
      }
      if (process->state != READY && newState == READY){
            processQueue->prepared++;
      }
      if (process->state == READY && newState != READY){
            processQueue->prepared--;
      }
      // State oldState = process->state;
      process->state = newState;
      // ncPrint("Process of PID: ");
      // ncPrintDec(pid);
      // ncPrint(" changed status from ");
      // ncPrint(stateName(oldState));
      // ncPrint(" to ");
      // ncPrint(stateName(newState));
      // ncNewline();
      return process->pid;
}

uint64_t kill(uint64_t pid){
      if (pid <= 2){
            ncPrint("[Kernel] ERROR: Invalid PID.");
            return -1;
      }
      int aux = changeState(pid, KILLED);
      if (pid == currentP->pid)
            yield();
      return aux;
}

uint64_t block(uint64_t pid){
      int aux = changeState(pid, BLOCKED);
      if (pid == currentP->pid){
           yield();
      }
      return aux;
}

uint64_t unblock(uint64_t pid){
      return changeState(pid, READY);
}

//    ----------------------------
//    |                          |
//    |      Misc functions      |
//    |                          |
//    |                          |
//    ----------------------------

static uint64_t getPID(){
      return PID++;
}

static void createStackFrame(void (*entryPoint)(int, char **), int argc, char **argv, void *rbp){
      StackFrame *frame = (StackFrame *)rbp - 1;
      frame->gs = 0x001;
      frame->fs = 0x002;
      frame->r15 = 0x003;
      frame->r14 = 0x004;
      frame->r13 = 0x005;
      frame->r12 = 0x006;
      frame->r11 = 0x007;
      frame->r10 = 0x008;
      frame->r9 = 0x009;
      frame->r8 = 0x00A;
      frame->rsi = (uint64_t)argc;
      frame->rdi = (uint64_t)entryPoint;
      frame->rbp = 0x00D;
      frame->rdx = (uint64_t)argv;
      frame->rcx = 0x00F;
      frame->rbx = 0x010;
      frame->rax = 0x011;
      frame->rip = (uint64_t)wrapper;
      frame->cs = 0x008;
      frame->eflags = 0x202;
      frame->rsp = (uint64_t)(&frame->base);
      frame->ss = 0x000;
      frame->base = 0x000;
}

void emptyBuff(char * buff){
      for(int i = 0; buff[i] != '\0'; i++){
            buff[i] = 0;
      }
}

void printIndividualProcess(PNode * process){
      if(process != NULL){
            char buff[MAX_LEN] = {0};
            ncPrint("   ");
            ncPrint(itoa(process->pid, buff, 10));
            emptyBuff(buff);
            ncPrint("     ");
            ncPrint(itoa(process->fg,buff,10));
            emptyBuff(buff);
            ncPrint("     ");
            ncPrint(itoa((uint64_t)process->rsp, buff, 16));
            emptyBuff(buff);
            ncPrint("     ");
            ncPrint(itoa((uint64_t)process->rbp, buff, 16));
            ncPrint("     ");
            ncPrint(stateName(process->state));
            ncPrint("     ");
            ncPrint(process->name);
            ncNewline();
      }
}

void processDisplay(){
      ncPrint("PID      FG       RSP              RBP              STATE        NAME");
      ncNewline();
      if (currentP != NULL){
            printIndividualProcess(currentP);
      }
      PNode * iter = processQueue->first;
      while(iter){
            printIndividualProcess(iter);
            iter = iter->next;
      }
}

int isForeground() {
      return currentP->fg;
}

int getCurrentPID(){
      return currentP ? currentP->pid : -1;
}

static void haltFunc(int argc, char ** argv){
      while (1){
            _hlt();
      }
}

static void exit(){
      kill(currentP->pid);
      _timerTick();
}

int getCurrentOutFD(){
      if(currentP != NULL){
            return currentP->fd[1];
      }
      return -1;
}

int getCurrentInFD(){
      if(currentP != NULL){
            return currentP->fd[0];
      }
      return -1;
}

static void wrapper(void (*entryPoint)(int, char **), int argc, char ** argv){
      entryPoint(argc, argv);
      exit();
}
