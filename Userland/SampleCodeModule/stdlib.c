#include <stdarg.h>
#include "stdlib.h"
#include "userlib.h"

#define MAX_BUFFER 100
#define STDIN 0
#define STDOUT 1
char buffer[MAX_BUFFER];
int buffSize = 0;

int getchar() {
    char temp[2];
    int read = scan(STDIN, temp, 2);
    if(read <= 0)
        return -1;
    return *temp;
}

int putchar(char c) {
    char temp[2] = {0};
    temp[0] = c;
    return print(STDOUT, temp, 1);
}

int scanf(char *command, ...)
{
    va_list args;
    va_start(args, command);
    int c;
    while((c != getchar()) != '\n') {
        if (c != -1)
        { //EOF
            if (c == '\b')
            {
                //backspace();
            }
            else
            {
                putInBuff(c);
                putchar(c);
            }
        }
    }
    putchar('\n');
    buffer[buffSize++] = '\0';

    int flag = 0;
    int dim1=0;
    int dim2=0;

    char *temp;
    int *tmp;

    while(!flag) {
        if(buffer[dim1] == '\0' || command[dim2] == '\0') {
            flag = 1;
        }
        else {
            if(command[dim2] == '%') {
                switch(command[++dim2]) {
                    case 's':
                        temp = *(char *)va_arg(args, char *);
                        strcpyWithSeparator(&buffer[dim1], temp, ' ');
                        dim1 += strlen(temp);
                        break;
                    case 'd':
                        tmp = *(int *)va_arg(args, int *);
                        int len = myAtoi(&buffer[dim1], tmp);
                        dim1 += len;
                        break;
                    case 'c':
                        *(char *)va_arg(args, char *) = buffer[dim1];
                        dim1++;
                        break;
                    default:
                        flag = 1;
                        break;
                }
            }
            else {
                if(buffer[dim1] != ' ' || command[dim2] != ' ') {
                    flag = 1;
                } 
                else {
                    dim1++;
                }
            }
            dim2++;
        }
    }

    va_end(args);
    return dim1;
    
}

void myPrintf(char * format, ...){
    char buffer[MAX_BUFFER];
    char * reference = format; // uso var aux para no perder la referencia de donde comienza format

    va_list args;
    va_start(args, format);

    while(*reference != '\0'){
        if(*reference == '%'){
            reference++;
            char * auxStr;
            switch(*reference){
                case 'd':
                    auxStr = itoa(va_arg(args, int), buffer, 10);
                    break;
                case 'x':
                    auxStr = itoa(va_arg(args, int), buffer, 16);
                    break;
                case 's':
                case 'c':
                    auxStr = va_arg(args,char*);
                break; 
            }
            int auxStrLen = strlen(auxStr);
            for(int i = 0; i < auxStrLen; i++){
                putchar(*auxStr);
                auxStr++;
            }
        }
        else{
            putchar(*reference);
        }
        reference++;
    }
    va_end(args);
}

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

void strcpy(const char * str1, char * str2) {
    strcpyWithSeparator(str1, str2, '\0');
}

void strcpyWithSeparator(const char * str1, char * str2, char separator) {
    while(*str1 != separator) {
        *str2 = *str1;
        str1++;
        str2++;
    }

}

void putInBuff(char c) {
    if(buffSize <= MAX_BUFFER)
        buffer[buffSize++] = c;
}

int myAtoi(const char * num, int * dest) {
    *dest = 0;
    int temp = 0;
    int flag = 0;
    int len = 0;
    while(*num != '\0' && *num != ' ' && !flag) {
        if(*num < 1 || *num > 9)
            flag = 1;
        else {
            temp = *dest;
            *dest = temp * 10 + (*num - '\0');
            num++;
            len++;
        }

    }

    return len;
}

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