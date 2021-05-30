#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>

<<<<<<< HEAD
void sysCallDispatcher (uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx);
=======
void sysCallDispatcher (uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx);
>>>>>>> 664ba25eeda664f8e265c2f59e0b0abdb58c13f8
void sysRead (char * buff, uint64_t address, uint16_t amount);
void sysWrite(int fd, char * buff, uint64_t count);
uint64_t sysGetRegs(uint64_t buffer, uint64_t rdx, uint64_t rcx);
void sysGetMem(char * buff, uint64_t address, uint64_t amount);
uint64_t sysGetDateTime(uint64_t id, uint64_t rdx, uint64_t rcx);
<<<<<<< HEAD
#endif
=======


#endif
>>>>>>> 664ba25eeda664f8e265c2f59e0b0abdb58c13f8
