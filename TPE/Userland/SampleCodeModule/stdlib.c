#include <stdarg.h>
#include "stdlib.h"
#include "userlib.h"

#define MAX_BUFFER 100
char buffer[MAX_BUFFER];
int buffSize = 0;



void printf(char * format, ...){
    char buffer[MAX_BUFFER];
    va_list args;
    va_start(args, format);
    while(*format != 0){
        if(*format == '%'){
            format++;
            char * string;
            switch(*format){
                case 'd':
                    string = itoa(va_arg(args, int), buffer, 10);
                break;
                case 'x':
                    string = itoa(va_arg(args, int), buffer, 16);
                break;
                case 's':
                case 'c':
                    string = va_arg(args,char*);
                break;  
            }
            print(1, string, strlen(string));
        }
        else{
            print(1, format, 1);
        }
    format++;                   
    }
    va_end(args);
    print(1, "\n", 1);
}

int strlen(char * string){
    int i = 0;
    while (string[i] != 0)
        i++;
    return i;
}

int strcmp(const char * str1, const char * str2){
    if(*str1 == 0 || *str2 == 0){
        return 1;
    }
    while(*str1 != 0 && *str2 != 0){
        if(*str1 != *str2){
            return 1;
        }
        str1++;
        str2++;
    }
    return *str1 - *str2;
}
/*
void readLine(){
    int auxChar;
    while(auxChar = getChar() != '\0'){
        if(auxChar != -1){ // codigo de error de getChar
            if(buffSize < MAX_BUFFER-1){
                buffer[buffSize] = auxChar;
            }
            putChar(auxChar);
        }
    }
    putChar('\n');
    buffer[buffSize++] = '\0';
    return buffSize;
}

int scan(const char * format, ...){
    va_list args;
    va_start(args, format);

    readLine();
    int fmtIdx = 0;
    int buffIdx = 0;
    int notAValidEntry = 0;
    int intValueSize = 0;

    while(format[fmtIdx] != '\0' && buffer[buffIdx] != '\0' && notAValidEntry == 0){
        if(format[fmtIdx] == '%'){
            fmtIdx++;
            switch(format[fmtIdx]){
                case 'c':
                    *(char *) va_arg(args, char *) = buffer[buffIdx]; //preguntar 
                    buffIdx++;
                    break;
                case 'd':
                    *(int *) va_arg(args, int *) = strToInt(&buffer[buffIdx], &intValueSize);
                    buffIdx += intValueSize;
                    break;
                case 's': // necesito copiar el string desde buffIdx hasta el primer espacio encontrado
                    char * auxStr;
                    strCpyLimited(&buffer[buffIdx], auxStr);
                    *(char *) va_arg(args, char *) = auxStr;
                    buffIdx += strlen(auxStr);
                    break;
                default:
                    notAValidEntry = -1;
                    break;
            }
        }
        else{
            if(format[fmtIdx] != buffer[bffIdx]){
                notAValidEntry = -1;
            }
            else{
                bffIdx++;
            }
        }
       
    }
        fmtIdx++;
        return buffIdx;

}
*/
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        char aux = *(str+start);
        *(str+start) = *(str+end);
        *(str+end) = aux;
        start++;
        end--;
    }
}
  
// Implementation of itoa()
char* itoa(int num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;
  
    //Handle 0 explicitely, otherwise empty string is printed for 0 
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
  
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }
  
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'A' : rem + '0';
        num = num/base;
    }
  
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
  
    str[i] = '\0'; // Append string terminator
  
    // Reverse the string
    reverse(str, i);
  
    return str;
}