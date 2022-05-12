#ifndef SEMLIB_H
#define SEMLIB_H

#include "sysCalls.h"

#define MAX_BLOCKED_PIDS 10

uint64_t semOpen(uint64_t id, uint64_t initValue);

int semWait(uint64_t id);

int semPost(uint64_t id);

int semClose(uint64_t id);

#endif