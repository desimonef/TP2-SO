// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "clock.h"
#include "stdint.h"
#include "userlib.h"
#include "stdlib.h"

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define DAYS 0x07
#define MONTH 0x08
#define YEAR 0x09
#define TIMEZONE -3


void seconds(char * buff){
    accessClock(SECONDS, buff);
}

void minutes(char * buff){
    accessClock(MINUTES, buff);
}

void hours(char * buff){
    accessClock(HOURS, buff);
    int hours = (buff[0] - '0')*10 + (buff[1] - '0');
    int finalHours = 0;
    if(hours<=2){
        finalHours = hours + (24 + TIMEZONE);
    }
    else{
        finalHours = hours + TIMEZONE;
    }
    buff[0] = ((finalHours/10)%10) + '0';
    buff[1] = (finalHours%10) + '0';
}

void days(char * buff){
    accessClock(DAYS, buff);
    char auxBuff[3] = {0};
    hours(auxBuff);
    if(auxBuff[0] == '2' && auxBuff[1] >= '1'){
        int finalDays = (buff[0] - '0')*10 + (buff[1] - '0') - 1;
        buff[0] = ((finalDays/10)%10) + '0';
        buff[1] = (finalDays%10) + '0';
    }
}

void months(char * buff){
    accessClock(MONTH, buff);
}

void years(char * buff){
    accessClock(YEAR, buff);
}
