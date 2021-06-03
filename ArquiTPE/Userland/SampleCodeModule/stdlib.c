#include <stdarg.h>
#include "stdlib.h"
#include "userlib.h"

#define MAX_BUFFER 100
#define STDIN 0
#define STDOUT 1
char buffer[MAX_BUFFER];
int buffSize = 0;

int getchar()
{
    char temp[2] = {0};
    int read = scan(0, temp, 2);
    if (read <= 0)
        return -1;
    return *temp;
}

int putchar(char c)
{
    char temp[2] = {0};
    *temp = c;
    return print(STDOUT, temp, 2);
}

void clearBuff() {
    buffSize = 0;
    int c;
    do {
        scan(0, buffer, 512);
    } while((c = getchar()) != -1);
    
}

int scanf(char *format, ...)
{
    va_list args;
    va_start(args, format);
    clearBuff();
    readLine(); //guarda en buffer los argumentos
    int bufferIndex = 0;
    int error = 0;
    int formatIndex = 0;
    char *string;
    int stringIndex;

    int *integer;
    int integerLen;

    while (format[formatIndex] != '\0' && buffer[bufferIndex] != '\0' && !error)
    {
        if (format[formatIndex] == '%')
        {
            formatIndex++;
            switch (format[formatIndex])
            {
            case 's':
                string = va_arg(args, char *);
                stringIndex = 0;
                while (buffer[bufferIndex] != ' ' && buffer[bufferIndex] != '\0')
                    string[stringIndex++] = buffer[bufferIndex++];
                string[stringIndex] = '\0';
                if (buffer[bufferIndex] != ' ')
                    bufferIndex++;
                break;
            case 'd':
                integer = va_arg(args, int *);
                integerLen = 0;
                integer = strToInt(&buffer[bufferIndex], &integerLen);
                bufferIndex += integerLen;
                break;
            case 'c':
                *(char *)va_arg(args, char *) = buffer[bufferIndex++];
                break;
            default:
                error = 1;
                break;
            }
        }
        else
        {
            if (buffer[bufferIndex] != format[formatIndex])
                error = 1;
            else
                bufferIndex++;
        }
        formatIndex++;
    }
    va_end(args);
    return bufferIndex;
}

void readLine()
{
    buffSize = 0;
    int c;
    while ((c = getchar()) != '\n')
    {
        
        if (c == '\b')
        {
            if (buffSize != 0)
            {
                buffSize--;
            }
        }
        else if (c != -1)
        {
            if (buffSize < MAX_BUFFER - 1)
            {
                buffer[buffSize++] = c;
            }
            putchar(c);
        }
    }
    putchar('\n');
    buffer[buffSize++] = '\0';
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
                    auxStr = va_arg(args,char*);
                break;
                case 'c':
                    auxStr = va_arg(args,char *);
                break; 
            }
                int auxStrLen = strlen(auxStr);
                for(int i = 0; i < auxStrLen; i++){
                    putChar(*auxStr);
                    auxStr++;
                }
        }
        else{
            putChar(*reference);
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
    //print(1, "\n", 1);
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

int isValidHexaAddress(char * buff){
    for(int i = 0; i < 8; i++){
        if(i == 0 && buff[i] != '0'){
            return 0;
        }
        else if(i == 1 && buff[i] != 'x'){
            return 0;
        }
        else{
            if(!((buff[i] >= '0' && buff[i] <= '9') || (buff[i] >= 'A' && buff[i] <= 'F'))){
                return 0;
            }
        }
    }
    return 1;
}

int hexToInt(char* num){
    
    int len = strlen(num);
    int base = 1;
    int returnValue = 0;

    for (int i = len - 1; i >= 0; i--)
    {
        if (num[i] >= '0' && num[i] <= '9')
        {
            returnValue += (num[i] - '0') * base;
        }
        else if (num[i] >= 'A' && num[i] <= 'F')
        {
            returnValue += (num[i] - 'A') * base;
        }
	else if(num[i] != 'x'){
	    printf("Direccion hexadecimal invalida\n");
	    return -1;
	}	
	base = base * 16;
    }
    return returnValue;
}

char *intToHex(long long num, char *str, int bytes) 
{ 
    int i = 0;
    long long n = abs(num);

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (n == 0) 
    {
        str[i++] = '0';
        str[i++] = 'x';

        while (i < bytes*2 + 2) {
            str[i++] = '0';
        }

        str[i] = '\0'; 
        return str; 
    } 

    while (i < bytes*2 && n != 0) 
    { 
        int rem = n % 16; 
        str[i++] = (rem >= 10)? (rem - 10) + 65 : rem + 48; 
        n = n/16; 
    } 

    while (i < bytes*2) {
        str[i++] = '0';
    }

    str[i++] = 'x';
    str[i++] = '0';
    str[i] = '\0';
    reverse(str, i);
    return str; 
} 

int abs(int num){
    return num < 0? -num : num;
}

