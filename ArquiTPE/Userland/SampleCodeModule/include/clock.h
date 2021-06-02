#ifndef CLOCK_H
#define CLOCK_H

unsigned int seconds();
unsigned int minutes();
unsigned int hours();
unsigned int days();
unsigned int month();
unsigned int year();
void timeToStr (char * dest);
void dateToStr (char * dest);
static unsigned int decode(unsigned char time);


#endif