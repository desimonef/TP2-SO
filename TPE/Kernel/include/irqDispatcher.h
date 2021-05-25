#ifndef IRQDISPATCHER_H
#define IRQDISPATCHER_H

#include <stdint.h>

void irqDispatcher(uint64_t irq);
void int_20();

#endif