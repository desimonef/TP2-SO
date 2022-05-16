// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "semaphores.h"
#include "pipes.h"
#include "lib.h"

#define IN_USE 1
#define EMPTY 0

uint32_t semId = 420;
static PipeArray pipesArray;

static int putCharPipeByIdx(int pipeIndex, char c);
static int getPipeIdx(uint32_t pipeId);
static int getFreePipe();
static uint32_t newPipe(uint32_t pipeId);

uint32_t pipeOpen(uint32_t pipeId)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
    {
        pipeIndex = newPipe(pipeId);
        if (pipeIndex == -1)
            return -1;
    }

    pipesArray.pipes[pipeIndex].totalProcesses++;

    return pipeId;
}

int pipeClose(uint32_t pipeId)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    Pipe *pipe = &pipesArray.pipes[pipeIndex];

    pipe->totalProcesses--;
    if (pipe->totalProcesses > 0)
        return 1;

    semClose(pipe->lockR);
    semClose(pipe->lockW);
    pipe->state = EMPTY;

    return 1;
}

int pipeRead(uint32_t pipeId)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    Pipe *pipe = &pipesArray.pipes[pipeIndex];

    semWait(pipe->lockR);

    char c = pipe->buffer[pipe->readIndex];
    pipe->readIndex = (pipe->readIndex + 1) % LEN;

    semPost(pipe->lockW);

    return c;
}

uint32_t pipeWrite(uint32_t pipeId, char *str)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    while (*str != 0)
        putCharPipeByIdx(pipeIndex, *str++);

    return pipeId;
}

static int putCharPipeByIdx(int pipeIndex, char c)
{
    Pipe *pipe = &pipesArray.pipes[pipeIndex];

    semWait(pipe->lockW);

    pipe->buffer[pipe->writeIndex] = c;
    pipe->writeIndex = (pipe->writeIndex + 1) % LEN;

    semPost(pipe->lockR);

    return 0;
}

uint32_t putCharPipe(uint32_t pipeId, char c)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    putCharPipeByIdx(pipeIndex, c);

    return pipeId;
}

static uint32_t newPipe(uint32_t pipeId)
{
    int newIdx = getFreePipe();

    if (newIdx == -1)
        return -1;

    Pipe *pipe = &pipesArray.pipes[newIdx];

    pipe->id = pipeId;
    pipe->state = IN_USE;
    pipe->readIndex = pipe->writeIndex = pipe->totalProcesses = 0;

    if ((pipe->lockR = semOpen(semId++, 0)) == -1)
        return -1;

    if ((pipe->lockW = semOpen(semId++, LEN)) == -1)
        return -1;

    return pipeId;
}

static int getPipeIdx(uint32_t pipeId)
{
    for (int i = 0; i < MAX_PIPES; i++)
        if (pipesArray.pipes[i].state == IN_USE && pipesArray.pipes[i].id == pipeId)
            return i;
    return -1;
}

static int getFreePipe()
{
    for (int i = 0; i < MAX_PIPES; i++)
        if (pipesArray.pipes[i].state == EMPTY)
            return i;
    return -1;
}

void dumpPipes()
{
    
}