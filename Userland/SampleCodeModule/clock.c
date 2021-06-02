#include "clock.h"
#include "stdint.h"
#include "userlib.h"

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define DAYS 0x07
#define MONTH 0x08
#define YEAR 0x09
#define TIMEZONE -3

void getDateAndTime(){
    char auxDaysBuff[3] = {0};
    char auxMonthsBuff[3] = {0};
    char auxYearsBuff[3] = {0};
    getDays(auxDaysBuff);
    getMonths(auxMonthsBuff);
    getYears(auxYearsBuff);
    printf("La fecha de hoy es: %s/%s/%s. ", auxDaysBuff, auxMonthsBuff, auxYearsBuff);
    char auxSecondsBuff[3] = {0};
    char auxMinutesBuff[3] = {0};
    char auxHoursBuff[3] = {0};
    getSeconds(auxSecondsBuff);
    getMinutes(auxMinutesBuff);
    getHours(auxHoursBuff);
    printf("La hora actual es: %s:%s:%s\n", auxHoursBuff, auxMinutesBuff, auxSecondsBuff);
}

void getDays(char * buff){
    accessClock(DAYS, buff);
}

void getMonths(char * buff){
    accessClock(MONTH, buff);
}

void getYears(char * buff){
    accessClock(YEAR, buff);
}

void getSeconds(char * buff){
    accessClock(SECONDS, buff);
}
void getMinutes(char * buff){
    accessClock(MINUTES, buff);
}
void getHours(char * buff){
    accessClock(HOURS, buff);
    int hours = (buff[0] - '0')*10 + (buff[1] - '0');
    int finalHours = hours + TIMEZONE;
    buff[0] = ((finalHours/10)%10) + '0';
    buff[1] = (finalHours%10) + '0';
}