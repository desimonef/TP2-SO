// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <schedule.h>
#include <lib.h>
#include <memManager.h>
#include <stddef.h>
#include <interrupts.h>

#define STACK_SIZE (4 * 1024)
#define INIT_PRIO 1
#define INIT_PRIO_AUG 2
#define PRIO_CAP 40

typedef struct ProcessNode
{
      PCB pcb;
      State state;
      struct ProcessNode *next;
} ProcessNode;

typedef struct ProcessList
{
      uint32_t size;
      uint32_t readySize;
      ProcessNode *first;
      ProcessNode *last;
} ProcessList;

void strcpy(char *str1, char *str2);

static void setNewSF(void (*entryPoint)(int, char **), int argc, char **argv, void *rbp);
static int createPCB(PCB *process, char *name, int fg, int *fd);
static int argsCopy(char **buffer, char **argv, int argc);
static uint64_t getNewPID();
static void wrapper(void (*entryPoint)(int, char **), int argc, char **argv);
static void exit();
static void freeProcess(ProcessNode *process);
static ProcessNode *getProcessOfPID(uint64_t pid);

static void processQueue(ProcessNode *newProcess);
static ProcessNode *processDequeue();
static int queueIsEmpty();

static uint64_t newPIDVal = 0;
static ProcessList *processes;
static ProcessNode *currentProcess;
static uint64_t cyclesLeft;
static ProcessNode *idleProcess;

void strcpy(char *str1, char *str2)
{ 
      int i;
      for (i = 0; str1[i] != 0; i++)
            str2[i] = str1[i];
      str2[i] = 0;
}


static void haltFunc(int argc, char **argv)
{
      while (1)
            _hlt();
}

void initScheduler()
{
      processes = malloc(sizeof(ProcessList));

      if (processes == NULL)
            return;

      processes->first = NULL;
      processes->last = processes->first;
      processes->size = 0;
      processes->readySize = 0;

      char *argv[] = {"Halt Process"};
      addProcess(&haltFunc, 1, argv, 0, 0);
      idleProcess = processDequeue();
}

void *scheduler(void *oldRSP)
{
      //ncPrint("Entering sched with proc ");
      //ncPrint(currentProcess->pcb.name);
      //ncPrint("     ");
      if (currentProcess)
      {
            if (currentProcess->state == READY && cyclesLeft > 0)
            {
                  cyclesLeft--;
                  return oldRSP;
            }

            currentProcess->pcb.rsp = oldRSP;

            if (currentProcess->pcb.pid != idleProcess->pcb.pid)
            {
                  if (currentProcess->state == KILLED)
                  {
                        ProcessNode *parent = getProcessOfPID(currentProcess->pcb.ppid);
                        if (parent != NULL && currentProcess->pcb.fg && parent->state == BLOCKED)
                        {
                              unblockProcess(parent->pcb.pid);
                        }
                        freeProcess(currentProcess);
                  }
                  else
                        processQueue(currentProcess);
            }
      }

      if (processes->readySize > 0)
      {
            currentProcess = processDequeue();
            while (currentProcess->state != READY)
            {

                  if (currentProcess->state == KILLED)
                  {
                        freeProcess(currentProcess);
                  }
                  if (currentProcess->state == BLOCKED)
                  {
                        processQueue(currentProcess);
                  }
                  currentProcess = processDequeue();
            }
      }
      else
            currentProcess = idleProcess;
      cyclesLeft = currentProcess->pcb.priority;
      return currentProcess->pcb.rsp;
}

int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd)
{
      if (entryPoint == NULL)
            return -1;

      ProcessNode *newProcess = malloc(sizeof(ProcessNode));

      if (newProcess == NULL)
            return -1;

      if (createPCB(&newProcess->pcb, argv[0], fg, fd) == -1)
      {
            free(newProcess);
            return -1;
      }

      char **argvCopy = malloc(sizeof(char *) * argc);
      if (argvCopy == 0)
            return -1;
      argsCopy(argvCopy, argv, argc);

      newProcess->pcb.argc = argc;
      newProcess->pcb.argv = argvCopy;

      setNewSF(entryPoint, argc, argvCopy, newProcess->pcb.rbp);

      newProcess->state = READY;
      processQueue(newProcess);
      if (newProcess->pcb.fg && newProcess->pcb.ppid)
            blockProcess(newProcess->pcb.ppid);

      return newProcess->pcb.pid;
}

static int createPCB(PCB *process, char *name, int fg, int *fd)
{
      strcpy(name, process->name);
      process->pid = getNewPID();

      process->ppid = currentProcess == NULL ? 0 : currentProcess->pcb.pid;
      if (fg > 1 || fg < 0)
            return -1;

      process->fg = currentProcess == NULL ? fg : (currentProcess->pcb.fg ? fg : 0);
      process->rbp = malloc(STACK_SIZE);
      process->priority = process->fg ? INIT_PRIO_AUG : INIT_PRIO;
      process->fd[0] = fd ? fd[0] : 0;
      process->fd[1] = fd ? fd[1] : 1;

      if (process->rbp == NULL)
            return -1;

      process->rbp = (void *)((char *)process->rbp + STACK_SIZE - 1);
      process->rsp = (void *)((StackFrame *)process->rbp - 1);
      return 0;
}

static void setNewSF(void (*entryPoint)(int, char **), int argc, char **argv, void *rbp)
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

static uint64_t getNewPID()
{
      return newPIDVal++;
}

static void freeProcess(ProcessNode *process)
{
      for (int i = 0; i < process->pcb.argc; i++)
            free(process->pcb.argv[i]);
      free(process->pcb.argv);
      free((void *)((char *)process->pcb.rbp - STACK_SIZE + 1));
      free((void *)process);
}

static void exit()
{
      killProcess(currentProcess->pcb.pid);
      callTimerTick();
}

static void wrapper(void (*entryPoint)(int, char **), int argc, char **argv)
{
      entryPoint(argc, argv);
      exit();
}

static ProcessNode *getProcessOfPID(uint64_t pid)
{
      if (currentProcess != NULL && currentProcess->pcb.pid == pid)
            return currentProcess;

      for (ProcessNode *p = processes->first; p != NULL; p = p->next)
            if (p->pcb.pid == pid)
                  return p;

      return NULL;
}

static uint64_t setNewState(uint64_t pid, State newState)
{

      ProcessNode *process = getProcessOfPID(pid);

      if (process == NULL || process->state == KILLED)
            return -1;

      if (process == currentProcess)
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

      if (pid == currentProcess->pcb.pid)
            callTimerTick();

      return aux;
}

uint64_t blockProcess(uint64_t pid)
{
      int aux = setNewState(pid, BLOCKED);

      if (pid == currentProcess->pcb.pid)
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
            return "R";
            break;
      case BLOCKED:
            return "B";
      default:
            return "D";
            break;
      };
}

void printProcess(ProcessNode *process)
{
      //ncPrint()
      //if (process != NULL)
      //      print("%d        %d        %x        %x        %s            %s\n", process->pcb.pid, (int)process->pcb.fg,
      //            (uint64_t)process->pcb.rsp, (uint64_t)process->pcb.rbp, stateToStr(process->state), process->pcb.name);
}

void processDisplay()
{
      ncPrint("PID      FG       RSP              RBP              STATE        NAME");

      if (currentProcess != NULL)
            printProcess(currentProcess);

      ProcessNode *curr = processes->first;
      while (curr)
      {
            printProcess(curr);
            curr = curr->next;
      }
}

int getCurrPID()
{
      return currentProcess ? currentProcess->pcb.pid : -1;
}

void setNewCycle(uint64_t pid, int priority)
{

      if (priority < 0)
            priority = 0;
      if (priority > PRIO_CAP)
            priority = PRIO_CAP;

      ProcessNode *p = getProcessOfPID(pid);

      if (p != NULL)
            p->pcb.priority = priority;
}

void killFgProcess()
{
      if (currentProcess != NULL && currentProcess->pcb.fg && currentProcess->state == READY)
      {
            killProcess(currentProcess->pcb.pid);
            return;
      }
}

void yield()
{
      cyclesLeft = 0;
      callTimerTick();
}

int currentReadsFrom()
{
      if (currentProcess)
      {
            return currentProcess->pcb.fd[0];
      }
      return -1;
}

int currentWritesTo()
{
      if (currentProcess)
            return currentProcess->pcb.fd[1];
      return -1;
}

int currentProcessFg()
{
      if (currentProcess)
            return currentProcess->pcb.fg;
      return -1;
}

void waitForPID(uint64_t pid)
{
      ProcessNode *process = getProcessOfPID(pid);
      if (process)
      {
            process->pcb.fg = 1;
            blockProcess(currentProcess->pcb.pid);
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

static void processQueue(ProcessNode *newProcess)
{
      if (queueIsEmpty())
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

static ProcessNode *processDequeue()
{
      if (queueIsEmpty())
            return NULL;

      ProcessNode *p = processes->first;
      processes->first = processes->first->next;
      processes->size--;

      if (p->state == READY)
            processes->readySize--;

      return p;
}

static int queueIsEmpty()
{
      return processes->size == 0;
}