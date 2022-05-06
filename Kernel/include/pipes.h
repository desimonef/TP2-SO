#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>

#define LEN 2048
#define MAX_PIPES 20

typedef struct
{
    uint32_t id;
    char buffer[LEN];
    int readIndex;
    int writeIndex;
    long totalProcesses;
    int lockW;
    int lockR;
    int state;
} Pipe;

typedef struct
{
    Pipe pipes[MAX_PIPES];
} PipeArray;

uint32_t pipeOpen(uint32_t pipeId);
int pipeClose(uint32_t pipeId);
int pipeRead(uint32_t pipeId);
uint32_t pipeWrite(uint32_t pipeId, char *str);
uint32_t putCharPipe(uint32_t pipeId, char c);
void dumpPipes();

#endif