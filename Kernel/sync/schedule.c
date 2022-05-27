// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <schedule.h>
#include <lib.h>
#include <memManager.h>
#include <stddef.h>
#include <interrupts.h>
#include "lib.h"

#define STACK_SIZE (4 * 1024)
#define INITIAL_P 1
#define INITIAL_P_AUX 2
#define MAX_PRIORITY 40
#define MAX_LEN 30

typedef struct PNode{
      struct PNode * next;

      PCB pcb;
      State state;
} PNode;

typedef struct PList{
      PNode * first;
      PNode * last;

      uint32_t size;
      uint32_t readySize;
} PList;

// General declarations

static void haltFunc(int argc, char **argv);
void emptyBuff(char * buff);
static void createStackFrame(void (*entryPoint)(int, char **), int argc, char **argv, void *rbp);
static int createPCB(PCB *process, char *name, int fg, int *fd);
static uint64_t getPID();
static void wrapper(void (*entryPoint)(int, char **), int argc, char **argv);
static void exit();
static void freeP(PNode *process);
static PNode * getProcess(uint64_t pid);

// Queue declarations

static void pEnqueue(PNode *newP);
static PNode * pDequeue();
static int isEmptyQ();

static uint64_t currPID = 0;
static PList * processes;
static PNode * currentP;
static uint64_t ticks;
static PNode * haltP;

//    ----------------------------
//    |                          |
//    |     General scheduling   |
//    |        functions         |
//    |                          |
//    ----------------------------

void initScheduler(){
      processes = malloc(sizeof(PList));
      if (processes == NULL){
            ncPrint("Error initializing scheduler.");
            return;
      }
      processes->first = NULL;
      processes->last = processes->first;
      processes->size = 0;
      processes->readySize = 0;

      char *argv[] = {"hlt"};
      addProcess(&haltFunc, 1, argv, 0, 0);
      haltP = pDequeue();
}

void *scheduler(void * setRSP){
      if (currentP != NULL){
            if (currentP->state == READY && ticks > 0){
                  ticks--;
                  return setRSP;
            }

            currentP->pcb.rsp = setRSP;
            if (currentP->pcb.pid != haltP->pcb.pid){
                  if(currentP->state == READY || currentP->state == BLOCKED){
                        //Reschedule process, its not done
                        pEnqueue(currentP);
                  }
                  else{
                        PNode * parent = getProcess(currentP->pcb.ppid);
                        if (parent != NULL && currentP->pcb.fg && parent->state == BLOCKED){
                              unblock(parent->pcb.pid);
                        }
                        freeP(currentP);
                  }
            }
      }
      
      // There is no active running process
      if (processes->readySize > 0){
            currentP = pDequeue();
            while (currentP->state != READY){
                  if (currentP->state == KILLED){
                        freeP(currentP);
                  }
                  if (currentP->state == BLOCKED){
                        pEnqueue(currentP);
                  }
                  currentP = pDequeue();
            }
      }
      else{
            currentP = haltP; // No tenemos procesos listos --> Haltea el kernel
      }
      ticks = currentP->pcb.priority;
      return currentP->pcb.rsp;
}

int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd){
      if (entryPoint == NULL)
            return -1;

      PNode * newP = malloc(sizeof(PNode));
      if (newP == NULL){
            return -1;
      }

      if (createPCB(&newP->pcb, argv[0], fg, fd) == -1){
            free(newP);
            return -1;
      }

      char ** argvCopy = malloc(sizeof(char *) * argc);
      if (argvCopy == 0)
            return -1;

      argsCopy(argvCopy, argv, argc);
      newP->pcb.argc = argc;
      newP->pcb.argv = argvCopy;
      createStackFrame(entryPoint, argc, argvCopy, newP->pcb.rbp);

      newP->state = READY;
      pEnqueue(newP);
      if (newP->pcb.fg && newP->pcb.ppid){
            block(newP->pcb.ppid);
      }

      return newP->pcb.pid;
}

static int createPCB(PCB * process, char * name, int fg, int * fd){
      process->pid = getPID();
      if (fg > 1 || fg < 0){
            ncPrint("[Kernel] ERROR: Error creating process, fg value out of bounds");
            return -1;
      }
      if(currentP == NULL){
            process->ppid = 0;
            process->fg = fg;
      }
      else{
            process->ppid = currentP->pcb.pid;
            process->fg = currentP->pcb.fg ? fg : 0;
      }

      strcpy(name, process->name);

      process->rbp = malloc(STACK_SIZE);
      if (process->rbp == NULL){
            ncPrint("[Kernel] ERROR: Malloc failure.");
            return -1;
      }

      process->rbp = (void *)((char *)process->rbp + STACK_SIZE - 1);
      process->rsp = (void *)((StackFrame *)process->rbp - 1);

      process->priority = process->fg ? INITIAL_P_AUX : INITIAL_P;

      process->fd[0] = fd ? fd[0] : 0;
      process->fd[1] = fd ? fd[1] : 1;

      return 0;
}

static void freeP(PNode * process){
      for (int i = 0; i < process->pcb.argc; i++){
            free(process->pcb.argv[i]);
      }
      free(process->pcb.argv);
      free((void *)((char *)process->pcb.rbp - STACK_SIZE + 1));
      free((void *)process);
}

static PNode * getProcess(uint64_t pid){
      // Chequeamos si es el que está corriendo
      if (currentP != NULL && currentP->pcb.pid == pid){
            return currentP;
      }
      PNode * aux = processes->first;
      for (; aux != NULL; aux = aux->next){
            if (aux->pcb.pid == pid){
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
            uint64_t prevPriority = aux->pcb.priority;
            aux->pcb.priority = priority;
            ncPrint("New priority set for process with PID: ");
            ncPrintDec(pid);
            ncPrint(" from ");
            ncPrintDec(prevPriority);
            ncPrint(" to ");
            ncPrintDec(priority);
            ncNewline();
      }
      else{
            ncPrint("[Kernel] ERROR: Process PID does not match with any existing processes. Returning.");
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
      if (isEmptyQ()){
            processes->first = newP;
            processes->last = processes->first;
      }
      else{
            processes->last->next = newP;
            newP->next = NULL;
            processes->last = newP;
      }
      if (newP->state == READY){
            processes->readySize++;
      }
      processes->size++;
}

static PNode * pDequeue(){
      if (isEmptyQ()){
            return NULL;
      }
      PNode * aux = processes->first;
      processes->first = processes->first->next;
      processes->size--;

      if (aux->state == READY)
            processes->readySize--;

      return aux;
}

static int isEmptyQ(){
      return processes->size == 0;
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
            return process->pcb.pid;
      }
      if (process->state != READY && newState == READY){
            processes->readySize++;
      }
      if (process->state == READY && newState != READY){
            processes->readySize--;
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
      return process->pcb.pid;
}

uint64_t kill(uint64_t pid){
      if (pid <= 2){
            ncPrint("[Kernel] ERROR: Invalid PID.");
            return -1;
      }
      int aux = changeState(pid, KILLED);
      if (pid == currentP->pcb.pid)
            _timerTick();
      return aux;
}

uint64_t block(uint64_t pid){
      int aux = changeState(pid, BLOCKED);
      if (pid == currentP->pcb.pid){
            _timerTick();
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
      return currPID++;
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
            ncPrint(itoa(process->pcb.pid, buff, 10));
            emptyBuff(buff);
            ncPrint("     ");
            ncPrint(itoa(process->pcb.fg,buff,10));
            emptyBuff(buff);
            ncPrint("     ");
            ncPrint(itoa((uint64_t)process->pcb.rsp, buff, 16));
            emptyBuff(buff);
            ncPrint("     ");
            ncPrint(itoa((uint64_t)process->pcb.rbp, buff, 16));
            ncPrint("     ");
            ncPrint(stateName(process->state));
            ncPrint("     ");
            ncPrint(process->pcb.name);
            ncNewline();
      }
}

void processDisplay(){
      ncPrint("PID      FG       RSP              RBP              STATE        NAME");
      ncNewline();
      if (currentP != NULL){
            printIndividualProcess(currentP);
      }
      PNode * iter = processes->first;
      while(iter){
            printIndividualProcess(iter);
            iter = iter->next;
      }
}

int getCurrPID(){
      return currentP ? currentP->pcb.pid : -1;
}

static void haltFunc(int argc, char ** argv){
      while (1){
            _hlt();
      }
}

static void exit(){
      kill(currentP->pcb.pid);
      _timerTick();
}

int getCurrentOutFD(){
      if(currentP != NULL){
            return currentP->pcb.fd[1];
      }
      return -1;
}

int getCurrentInFD(){
      if(currentP != NULL){
            return currentP->pcb.fd[0];
      }
      return -1;
}

static void wrapper(void (*entryPoint)(int, char **), int argc, char ** argv){
      entryPoint(argc, argv);
      exit();
}