#ifndef IDTLOADER_H
#define IDTLOADER_H

#include <stdint.h>

void load_idt();
static void setup_IDT_entry (int index, uint64_t offset);


#endif