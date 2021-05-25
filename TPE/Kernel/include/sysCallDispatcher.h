#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>

void write(unsigned int fd, const char * buff, uint64_t count);
void sysCallDispatcher (uint64_t id, uint64_t arg1, uint64_t arg2, uint64_t agr3);


#endif