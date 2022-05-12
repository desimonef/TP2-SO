#ifndef PIPELIB_H
#define PIPELIB_H

#include <stdint.h>

uint64_t pipeOpen(uint64_t id);

int pipeRead(uint64_t id);

int pipeWrite(uint64_t id, char *str);

int pipeClose(uint64_t id);

int printPipes();

#endif