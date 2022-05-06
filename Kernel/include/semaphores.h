#ifndef _SEMAPHORES_H
#define _SEMAPHORES_H

#define MAX_BLOCKED 16

#include <stdint.h>

typedef struct Semaphore
{
    uint32_t id;
    uint32_t value;
    uint16_t attachedProc;
    uint32_t blockedPIDs[MAX_BLOCKED];
    uint16_t blockedPIDsSize;
    int mutex;
    struct Semaphore *next;

} Semaphore;

uint32_t semOpen(uint32_t id, uint32_t initValue);
int semWait(uint32_t id);
int semPost(uint32_t id);
int semClose(uint32_t id);
void semStatus();

#endif