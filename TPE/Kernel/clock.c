#include <clock.h>
#include <lib.h>

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define DAYS 0x07
#define MONTH 0x08
#define YEAR 0x09
#define TIMEZONE -3

unsigned char accessClock (unsigned char node);
static unsigned int decode(unsigned char time);

unsigned int seconds(){
    return decode(accessClock(SECONDS));
}

unsigned int minutes(){
    return decode(accessClock(MINUTES));
}

unsigned int hours(){
    return decode(accessClock(HOURS)) + TIMEZONE;
}

unsigned int days(){
    return decode(accessClock(DAYS));
}

unsigned int month(){
    return decode(accessClock(MONTH));
}

unsigned int year(){
    return decode(accessClock(YEAR));
}

void timeToStr (char * dest){
    dest[2] = dest[5] = ':';
    uint8_t h = hours();
    dest[0] = (h/10) % 10 + '0';
    dest[1] = h % 10 + '0';
    uint8_t m = minutes();
    dest[3] = (m/10) % 10 + '0';
    dest[4] = m % 10 + '0';
    uint8_t s = seconds();
    dest[6] = (s/10) % 10 + '0';
    dest[7] = s % 10 + '0';
}

void dateToStr (char * dest){
    dest[2] = dest[5] = '/';
    uint8_t d = days();
    dest[0] = (d/10) % 10 + '0';
    dest[1] = d % 10 + '0';
    uint8_t m = month();
    dest[3] = (m/10) % 10 + '0';
    dest[4] = m % 10 + '0';
    uint8_t y = year();
    dest[6] = (y/10) % 10 + '0';
    dest[7] = y % 10 + '0';
}

static unsigned int decode(unsigned char time){
    return (time >> 4) * 10 + (time & 0x0F);
}