// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "semaphores.h"
#include "pipes.h"
#include "lib.h"
#include "naiveConsole.h"

static int getFreePipe();
static uint32_t newPipe(uint32_t pipeId);

static int setCharAtIdx(int idx, char c);
static int getPipeIdx(uint32_t pipeId);

uint32_t baseSemID = 300;
static PipeArray pipesAdmin;

//    ----------------------------
//    |                          |
//    |     Pipes functions      |
//    |                          |
//    |                          |
//    ----------------------------

static uint32_t newPipe(uint32_t pipeId){
    int free = getFreePipe();
    if (free == -1)
        return -1;

    Pipe * pipe = &pipesAdmin.pipes[free];
    pipe->id = pipeId;
    pipe->state = OCCUPIED;
    pipe->readIdx = 0;
    pipe->writeIdx = 0;
    pipe->totalProcesses = 0;

    if ((pipe->readLock = semOpen(baseSemID++, 0)) == -1)
        return -1;

    if ((pipe->writeLock = semOpen(baseSemID++, LEN)) == -1)
        return -1;

    return pipeId;
}

uint32_t pipeOpen(uint32_t pipeId){
    int idx = getPipeIdx(pipeId);
    if (idx == -1)
    {
        idx = newPipe(pipeId);
        if (idx == -1){
            ncPrint("Pipe could not be created");
            return -1;
        }
    }
    pipesAdmin.pipes[idx].totalProcesses++;
    return pipeId;
}

int pipeClose(uint32_t pipeId){
    int idx = getPipeIdx(pipeId);
    if (idx == -1)
        return -1;

    Pipe *pipe = &pipesAdmin.pipes[idx];
    pipe->totalProcesses--;
    if (pipe->totalProcesses > 0)
        return 1;

    semClose(pipe->readLock);
    semClose(pipe->writeLock);
    pipe->state = EMPTY;
    return 1;
}

int pipeRead(uint32_t pipeId){
    int idx = getPipeIdx(pipeId);
    if (idx == -1)
        return -1;

    Pipe * pipe = &pipesAdmin.pipes[idx];
    semWait(pipe->readLock);

    char c = pipe->buffer[pipe->readIdx];
    pipe->readIdx = (pipe->readIdx + 1) % LEN;

    semPost(pipe->writeLock);
    return c;
}

uint32_t pipeWrite(uint32_t pipeId, char *str){
    int idx = getPipeIdx(pipeId);
    if (idx == -1)
        return -1;

    while (*str != 0)
        setCharAtIdx(idx, *str++);

    return pipeId;
}

static int setCharAtIdx(int idx, char c){
    Pipe * pipe = &pipesAdmin.pipes[idx];
    semWait(pipe->writeLock);

    pipe->buffer[pipe->writeIdx] = c;
    pipe->writeIdx = (pipe->writeIdx + 1) % LEN;

    semPost(pipe->readLock);
    return 0;
}

uint32_t putCharPipe(uint32_t pipeId, char c){
    int idx = getPipeIdx(pipeId);
    if (idx == -1)
        return -1;

    setCharAtIdx(idx, c);
    return pipeId;
}

static int getPipeIdx(uint32_t pipeId){
    for (int i = 0; i < MAX_PIPES; i++)
        if (pipesAdmin.pipes[i].state == OCCUPIED && pipesAdmin.pipes[i].id == pipeId)
            return i;
    return -1;
}

static int getFreePipe(){
    for (int i = 0; i < MAX_PIPES; i++)
        if (pipesAdmin.pipes[i].state == EMPTY)
            return i;
    return -1;
}

void dumpPipes(){
    //TODO
}