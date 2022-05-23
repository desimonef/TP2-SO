// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "memManager.h"
#include "lib.h"
#include "semaphores.h"
#include "schedule.h"
#include "naiveConsole.h"
#include "defs.h"

Semaphore * semaphores = NULL;

static void printBlockedPIDsForSem(uint32_t *blockedPIDs, uint16_t blockedPIDsSize);
static Semaphore * findSem(uint32_t id);


//    ----------------------------
//    |                          |
//    |   Semaphore functions    |
//    |                          |
//    |                          |
//    ----------------------------

uint32_t semOpen(uint32_t id, uint32_t initValue){
    Semaphore * sem = findSem(id);
    if (sem == NULL){
        sem = malloc(sizeof(Semaphore));
        if (sem == NULL)
            return -1;

        sem->value = initValue;
        sem->id = id;

        sem->attachedProcesses = 0;
        sem->blockedPIDsSize = 0;

        sem->next = NULL;
        sem->mutex = 0;

        Semaphore * auxSem = semaphores;

        if(auxSem != NULL){
            while (auxSem->next != NULL)
                auxSem = auxSem->next;
            auxSem->next = sem;
        }
        else{
            semaphores = sem;
        }
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
    Semaphore * sem = findSem(id);
    if (sem == NULL)
        return -1;

    // PAra evaluar valor de semáforo, necesitamos su mutex (sección crítica)
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
    Semaphore * sem = findSem(id);
    if (sem == NULL)
        return -1;

    acquire(&(sem->mutex));
    if (sem->value > 0){
        sem->value--;
        release(&(sem->mutex));
        return 0;
    }
    else{
        int currPid = getCurrPID();
        sem->blockedPIDs[sem->blockedPIDsSize++] = currPid;
        release(&(sem->mutex));
        block(currPid);
    }

    return 0;
}

int semClose(uint32_t id)
{
    Semaphore * sem = findSem(id);
    if (sem == NULL)
        return -1;

    sem->attachedProcesses--;
    if (sem->attachedProcesses > 0)
        return 0;

    Semaphore *aux = semaphores;
    if (aux == sem){
        semaphores = aux->next;
    }
    else{
        while (aux->next != sem)
            aux = aux->next;
        aux->next = sem->next;
    }

    free(sem);
    return 0;
}

void semStatus()
{
    //TODO
}

static void printBlockedPIDsForSem(uint32_t *blockedPIDs, uint16_t blockedPIDsSize)
{
    //TODO
}

static Semaphore *findSem(uint32_t id)
{
    Semaphore * sem = semaphores;
    while (sem){
        if (sem->id == id)
            return sem;
        sem = sem->next;
    }
    return NULL;
}