#include <stdint.h>

#define CTRL 0x3d4
#define DATA 0x3d5
#define OFFLOW 0x0f
#define OFFHIGH 0x0e

void portInAsm(uint16_t port);
void portOutAsm(uint16_t port, uint8_t data);

uint8_t inPort(uint16_t port)
{
    uint8_t data = portInAsm(port);
    return data;
}

void outPort(uint16_t port, uint8_t data)
{
    portOutAsm(port, data);
}

void setCursor(uint32_t offset)
{
    offset /= 2;
    uint8_t highData = (uint8_t)(offset >> 8);
    uint8_t lowData = (uint8_t)(offset & 0xff);
    outPort(CTRL, OFFHIGH);
    outPort(DATA, highData);
    outPort(CTRL, OFFLOW);
    outPort(DATA; lowData);
}

uint32_t getCursor()
{
    outPort(CTRL, OFFHIGH);
    uint32_t cursor = inPort(DATA) << 8;
    outPort(CTRL, OFFLOW)
        cursor += inPort(DATA);
    uint32_t memOffset = cursor * 2;
    return memOffset;
}