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
#define INITIAL_P_NOT_MASTER 2
#define MAX_PRIORITY 40
#define MAX_LEN 30

typedef struct PNode
{
      struct PNode * next;

      PCB pcb;
      State state;
} PNode;

typedef struct PList
{
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
static int argsCopy(char **buffer, char **argv, int argc);
static uint64_t getPID();
static void wrapper(void (*entryPoint)(int, char **), int argc, char **argv);
static void exit();
static void freeP(PNode *process);
static PNode * getProcess(uint64_t pid);

// Queue declarations

static void pEnqueue(PNode *newProcess);
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
      if (currentP){
            if (currentP->state == READY && ticks > 0){
                  ticks--;
                  return setRSP;
            }

            currentP->pcb.rsp = setRSP;
            if (currentP->pcb.pid != haltP->pcb.pid){
                  if (currentP->state == KILLED){
                        PNode *parent = getProcess(currentP->pcb.ppid);
                        if (parent != NULL && currentP->pcb.fg && parent->state == BLOCKED){
                              unblockProcess(parent->pcb.pid);
                        }
                        freeP(currentP);
                  }
                  else{
                        pEnqueue(currentP);
                  }
            }
      }

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
            currentP = haltP;
      }
      ticks = currentP->pcb.priority;
      return currentP->pcb.rsp;
}

int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd)
{
      if (entryPoint == NULL)
            return -1;

      PNode *newProcess = malloc(sizeof(PNode));
      if (newProcess == NULL){
            return -1;
      }

      if (createPCB(&newProcess->pcb, argv[0], fg, fd) == -1){
            free(newProcess);
            return -1;
      }

      char **argvCopy = malloc(sizeof(char *) * argc);
      if (argvCopy == 0)
            return -1;

      argsCopy(argvCopy, argv, argc);
      newProcess->pcb.argc = argc;
      newProcess->pcb.argv = argvCopy;
      createStackFrame(entryPoint, argc, argvCopy, newProcess->pcb.rbp);

      newProcess->state = READY;
      pEnqueue(newProcess);
      if (newProcess->pcb.fg && newProcess->pcb.ppid){
            blockProcess(newProcess->pcb.ppid);
      }

      return newProcess->pcb.pid;
}

static int createPCB(PCB *process, char *name, int fg, int *fd){
      process->pid = getPID();

      process->ppid = currentP == NULL ? 0 : currentP->pcb.pid;
      if (fg > 1 || fg < 0){
            ncPrint("Error creating process, fg value out of bounds");
            return -1;
      }

      strcpy(name, process->name);
      process->fg = currentP == NULL ? fg : (currentP->pcb.fg ? fg : 0);

      process->rbp = malloc(STACK_SIZE);
      if (process->rbp == NULL){
            return -1;
      }
      process->rbp = (void *)((char *)process->rbp + STACK_SIZE - 1);
      process->rsp = (void *)((StackFrame *)process->rbp - 1);

      process->priority = process->fg ? INITIAL_P_NOT_MASTER : INITIAL_P;
      process->fd[0] = fd ? fd[0] : 0;
      process->fd[1] = fd ? fd[1] : 1;

      return 0;
}

static void createStackFrame(void (*entryPoint)(int, char **), int argc, char **argv, void *rbp)
{
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

static uint64_t getPID()
{
      return currPID++;
}

static void freeP(PNode *process)
{
      for (int i = 0; i < process->pcb.argc; i++){
            free(process->pcb.argv[i]);
      }
      free(process->pcb.argv);
      free((void *)((char *)process->pcb.rbp - STACK_SIZE + 1));
      free((void *)process);
}

static PNode *getProcess(uint64_t pid)
{
      if (currentP != NULL && currentP->pcb.pid == pid)
            return currentP;

      for (PNode *p = processes->first; p != NULL; p = p->next)
            if (p->pcb.pid == pid)
                  return p;

      return NULL;
}



void setNewCycle(uint64_t pid, int priority)
{

      if (priority < 0)
            priority = 0;
      if (priority > MAX_PRIORITY)
            priority = MAX_PRIORITY;

      PNode *p = getProcess(pid);

      if (p != NULL)
            p->pcb.priority = priority;
}

void yield()
{
      ticks = 0;
      _timerTick();
}

int currentReadsFrom()
{
      if (currentP)
      {
            return currentP->pcb.fd[0];
      }
      return -1;
}

int currentWritesTo()
{
      if (currentP)
            return currentP->pcb.fd[1];
      return -1;
}

int currentPFg()
{
      if (currentP)
            return currentP->pcb.fg;
      return -1;
}

void waitForPID(uint64_t pid)
{
      PNode *process = getProcess(pid);
      if (process)
      {
            process->pcb.fg = 1;
            blockProcess(currentP->pcb.pid);
      }
}

static int argsCopy(char **buffer, char **argv, int argc)
{
      for (int i = 0; i < argc; i++)
      {
            buffer[i] = malloc(sizeof(char) * (strlen(argv[i]) + 1));
            strcpy(argv[i], buffer[i]);
      }
      return 1;
}

//    ----------------------------
//    |                          |
//    |     Queue functions      |
//    |                          |
//    |                          |
//    ----------------------------

static void pEnqueue(PNode *newProcess)
{
      if (isEmptyQ())
      {
            processes->first = newProcess;
            processes->last = processes->first;
      }
      else
      {
            processes->last->next = newProcess;
            newProcess->next = NULL;
            processes->last = newProcess;
      }

      if (newProcess->state == READY)
            processes->readySize++;

      processes->size++;
}

static PNode *pDequeue()
{
      if (isEmptyQ())
            return NULL;

      PNode *p = processes->first;
      processes->first = processes->first->next;
      processes->size--;

      if (p->state == READY)
            processes->readySize--;

      return p;
}

static int isEmptyQ()
{
      return processes->size == 0;
}

//    ----------------------------
//    |                          |
//    |     Status functions     |
//    |                          |
//    |                          |
//    ----------------------------

static uint64_t setNewState(uint64_t pid, State newState)
{

      PNode *process = getProcess(pid);

      if (process == NULL || process->state == KILLED)
            return -1;

      if (process == currentP)
      {
            process->state = newState;
            return process->pcb.pid;
      }

      if (process->state != READY && newState == READY)
            processes->readySize++;

      if (process->state == READY && newState != READY)
            processes->readySize--;

      process->state = newState;

      return process->pcb.pid;
}

uint64_t killProcess(uint64_t pid)
{
      if (pid <= 2)
            return -1;

      int aux = setNewState(pid, KILLED);

      if (pid == currentP->pcb.pid)
            callTimerTick();

      return aux;
}

uint64_t blockProcess(uint64_t pid)
{
      int aux = setNewState(pid, BLOCKED);

      if (pid == currentP->pcb.pid)
            callTimerTick();
      return aux;
}

uint64_t unblockProcess(uint64_t pid)
{
      return setNewState(pid, READY);
}

char *stateToStr(State state)
{
      switch (state)
      {
      case READY:
            return "Ready";
            break;
      case BLOCKED:
            return "Blocked";
      default:
            return "Killed";
            break;
      };
}


void killFgProcess()
{
      if (currentP != NULL && currentP->pcb.fg && currentP->state == READY)
      {
            killProcess(currentP->pcb.pid);
            return;
      }
}

//    ----------------------------
//    |                          |
//    |      Misc functions      |
//    |                          |
//    |                          |
//    ----------------------------

void emptyBuff(char * buff){
      for(int i = 0; buff[i] != '\0'; i++){
            buff[i] = 0;
      }
}

void printProcess(PNode *process)
{
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
            ncPrint(stateToStr(process->state));
            ncPrint("     ");
            ncPrint(process->pcb.name);
            ncNewline();
      }
}

void processDisplay()
{
      ncPrint("PID      FG       RSP              RBP              STATE        NAME");
      ncNewline();

      if (currentP != NULL)
            printProcess(currentP);

      PNode *curr = processes->first;
      while (curr)
      {
            printProcess(curr);
            curr = curr->next;
      }
}

int getCurrPID()
{
      return currentP ? currentP->pcb.pid : -1;
}

static void haltFunc(int argc, char **argv){
      while (1)
            _hlt();
}

static void exit()
{
      killProcess(currentP->pcb.pid);
      callTimerTick();
}

static void wrapper(void (*entryPoint)(int, char **), int argc, char **argv)
{
      entryPoint(argc, argv);
      exit();
}