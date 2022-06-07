// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "memManager.h"
#include "lib.h"
#include "semaphores.h"
#include "schedule.h"
#include "naiveConsole.h"
#include "defs.h"

Semaphore * semaphoreArray = NULL;

static void printBlockedPIDsForSem(uint32_t *blockedPIDs, uint16_t blockedPIDsSize);
void newSem(uint32_t id, int val);
static Semaphore * getSem(uint32_t id);


//    ----------------------------
//    |                          |
//    |   Semaphore functions    |
//    |                          |
//    |                          |
//    ----------------------------

uint32_t semOpen(uint32_t id, uint32_t val){
    Semaphore * sem = getSem(id);
    
    if (sem == NULL){
        newSem(id, val);
    }

    if (sem->attachedProcesses >= MAX_BLOCKED){
        ncPrint("Max number of attached processes reached");
        ncNewline();
        return -1;
    }

    sem->attachedProcesses++;
    return id;
}

int semPost(uint32_t id){
    Semaphore * sem = getSem(id);
    if (sem == NULL)
        return -1;

    
    acquire(&(sem->mutex));
    if (sem->blockedPIDsSize > 0){
        int nextPid = sem->blockedPIDs[0];
        for (int i = 0; i < sem->blockedPIDsSize - 1; i++)
            sem->blockedPIDs[i] = sem->blockedPIDs[i + 1];
        sem->blockedPIDsSize--;
        unblock(nextPid);
        release(&(sem->mutex));
        return 0;
    }
    else{
        sem->value++;
    }

    release(&(sem->mutex));
    return 0;
}

int semWait(uint32_t id)
{
    Semaphore * sem = getSem(id);
    if (sem == NULL)
        return -1;

    acquire(&(sem->mutex));
    if (sem->value > 0){
        sem->value--;
        release(&(sem->mutex));
        return 0;
    }
    else{
        int pid = getCurrentPID();
        sem->blockedPIDs[sem->blockedPIDsSize++] = pid;
        release(&(sem->mutex));
        block(pid);
    }

    return 0;
}

int semClose(uint32_t id)
{
    Semaphore * sem = getSem(id);
    if (sem == NULL)
        return -1;

    sem->attachedProcesses--;
    if (sem->attachedProcesses > 0)
        return 0;

    Semaphore *aux = semaphoreArray;
    if (aux == sem){
        semaphoreArray = aux->next;
    }
    else{
        while (aux->next != sem)
            aux = aux->next;
        aux->next = sem->next;
    }

    free(sem);
    return 0;
}

void printSemsRecursive(){
    Semaphore * auxPointer = semaphoreArray;
    while(auxPointer != NULL){
        ncPrintDec(auxPointer->id);
        ncPrint("         ");
        ncPrintDec(auxPointer->value);
        ncPrint("         ");
        ncPrintDec(auxPointer->attachedProcesses);
        ncPrint("         ");
        ncPrintDec(auxPointer->blockedPIDsSize);
        ncNewline();
        auxPointer = auxPointer->next;
    }
}

void semStatus()
{
    
    ncPrint("ID         Value         NOfAttachedProcs         NOfBlocked");
    ncNewline();
    printSemsRecursive();
}

void newSem(uint32_t id, int val) {
        Semaphore * sem = malloc(sizeof(Semaphore));

        sem->value = val;
        sem->id = id;

        sem->attachedProcesses = 0;
        sem->blockedPIDsSize = 0;

        sem->next = NULL;
        sem->mutex = 0;

        Semaphore * auxSem = semaphoreArray;

        if(auxSem != NULL){
            while (auxSem->next != NULL)
                auxSem = auxSem->next;
            auxSem->next = sem;
        }
        else{
            semaphoreArray = sem;
        }
}

static Semaphore *getSem(uint32_t id)
{
    Semaphore * sem = semaphoreArray;
    while (sem){
        if (sem->id == id)
            return sem;
        sem = sem->next;
    }
    return NULL;
}