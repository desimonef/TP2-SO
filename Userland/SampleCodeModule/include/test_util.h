#include <stdint.h>

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
void busyWait(uint64_t n);
//void endlessLoop(int argc, char *argv[]);
