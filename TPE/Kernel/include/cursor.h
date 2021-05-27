#ifndef CURSOR_H
#define CURSOR_H

#include <stdint.h>

uint8_t inPort(uint16_t port);
void outPort(uint16_t port, uint8_t data);
void setCursor(uint32_t offset);
uint32_t getCursor();

#endif