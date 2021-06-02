#include <stdint.h>
#include "lib.h"

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

int strlen(char * string){
    int i = 0;
    while (string[i] != 0)
        i++;
    return i;
}

void swap(char *x, char *y)
{
    char t = *x;
    *x = *y;
    *y = t;
}

char * intToHexa(uint64_t num, char * str, int bytes) 
{ 
    int i = 0;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i++] = 'x';

        while (i < bytes*2 + 2) {
            str[i++] = '0';
        }

        str[i] = '\0'; 
        return str; 
    } 
  
    // Process individual digits 
    while (i < bytes*2 && num != 0) { 
        int aux = num % 16; 
        str[i++] = (aux >= 10)? (aux - 10) + 'A' : aux + '0'; 
        num = num/16; 
    } 

    while (i < bytes*2) {
        str[i++] = '0';
    }

    str[i++] = 'x';
    str[i++] = '0';
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    reverse(str, i); 
    return str;
} 

void reverse(char str[], int length)
 {
     int start = 0;
     int end = length -1;
     while (start < end)
     {
         char aux = str[start];
         str[start] = str[end];
         str[end] = aux;
         start++;
         end--;
     }
 }

 void hold(int secs){
    long int initTime,currTime;
    long int timeDif=0;
    int difference = 0;
    int passedDays =0;
    initTime = currTime = getNormSecsInDay();
    while(1){
        currTime = getNormSecsInDay();
        difference = currTime-initTime;
        if(difference >= 10){
            return;
        }
        timeDif = currTime - initTime + (passedDays * 86400);
        if (timeDif<0){ //Paso un dia
            passedDays++;
            timeDif+=86400;
        }

        if (timeDif>=secs)
            return;

    }
}

long int getNormSecsInDay(){
    return getDateTime(0x00)+getDateTime(0x02)*60+ getDateTime(0x04)*3600+getDateTime(0x07);
}