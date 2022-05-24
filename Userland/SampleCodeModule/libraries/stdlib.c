// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <stdarg.h>
#include "stdlib.h"
#include "userlib.h"
#include "clock.h"

#define MAX_BUFFER 100
#define STDIN 0
#define STDOUT 1
char buffer[MAX_BUFFER];
int buffSize = 0;

int getchar(){
    char temp[2] = {0};
    int read = scan(0, temp, 2);
    if (read <= 0)
        return -1;
    return *temp;
}

int putchar(char c){
    char temp[2] = {0};
    *temp = c;
    return print(STDOUT, temp, 2);
}

void emptyBuffer() {
    buffSize = 0;
    int c = 0;
    while(c != -1)
        c = getchar();
    
}

int scanf(char *command, ...){
    va_list args;
    va_start(args, command);
    getCommandWithArgs(); //guarda en buffer los argumentos
    int bufferIndex = 0;
    int error = 0;
    int commandIndex = 0;
    char *string;
    int stringIndex;

    while (!error && command[commandIndex] && buffer[bufferIndex]){
        if (command[commandIndex] == '%'){
            commandIndex++;
            switch (command[commandIndex]){
            case 's':
                string = va_arg(args, char *);
                stringIndex = 0;
                while (buffer[bufferIndex] != ' ' && buffer[bufferIndex] != '\0')
                    string[stringIndex++] = buffer[bufferIndex++];
                string[stringIndex] = '\0';
                if (buffer[bufferIndex] != ' ')
                    bufferIndex++;
                break;
            default:
                error = 1;
                break;
            }
        }
        else
        {
            if (buffer[bufferIndex] != command[commandIndex])
                error = 1;
            else
                bufferIndex++;
        }
        commandIndex++;
    }
    va_end(args);
    return bufferIndex;
}

void getCommandWithArgs(){
    emptyBuffer();
    int c = 0;
    while ((c = getchar()) != '\n'){
        if (c == '\b'){
            if (buffSize != 0)
                buffSize--;
        }
        else if (c != -1){
            putInBuff(c);
            putchar(c);
        }
    }
    putchar('\n');
    putInBuff('\0');
}

void strcpy(char *str1, char *str2)
{ //copies str1 into str2
      int i;
      for (i = 0; str1[i] != 0; i++)
            str2[i] = str1[i];
      str2[i] = 0;
}

char * getCommandWithArgsBis(){
    emptyBuffer();
    int c = 0;
    while ((c = getchar()) != '\n'){
        if (c == '\b'){
            if (buffSize != 0)
                buffSize--;
        }
        else if (c != -1){
            putInBuff(c);
            putchar(c);
        }
    }
    putchar('\n');
    putInBuff('\0');
    char retString[buffSize];
    strcpy(buffer, retString);
    return retString;
}

void printf(char * command, ...){
    char buffer[MAX_BUFFER];
    va_list args;
    va_start(args, command);
    while(*command != 0){
        if(*command == '%'){
            command++;
            char * string;
            switch(*command){
                case 'd':
                    string = itoa(va_arg(args, int), buffer, 10);
                break;
                case 'x':
                    string = itoa(va_arg(args, int), buffer, 16);
                break;
                case 's':
                    string = va_arg(args,char*);
                break;  
            }
            print(1, string, strlen(string));
        }
        else{
            print(1, command, 1);
        }
    command++;                   
    }
    va_end(args);
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

void cleanBuffer(char * buffer){
    int len = strlen(buffer);
    for(int i = 0; i < len; i++)
        buffer[i] = 0;
}

int strtok(char * source, char ** dest, char token, int max){
    int index = 0;

    if (*source != token && *source != '\0')
        dest[index++] = source;

    while (*source != '\0')
    {
        if (*source == token)
        {
                *source = 0;
                if (*(source + 1) != token && (*(source + 1) != '\0'))
                {
                    if (index >= max)
                            return index;
                    dest[index++] = source + 1;
                }
        }
        source++;
    }
    return index;
}

int tokenizeBuffer(char token, char **dest, char *source, int max){
      int index = 0;

      if (*source != token && *source != '\0')
            dest[index++] = source;

      while (*source != '\0'){
            if (*source == token){
                  *source = 0;
                  if (*(source + 1) != token && (*(source + 1) != '\0')){
                        if (index >= max)
                              return index;
                        dest[index++] = source + 1;
                  }
            }
            source++;
      }
      return index;
}

void putInBuff(char c) {
    if(buffSize < MAX_BUFFER - 1)
        buffer[buffSize++] = c;
}

void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end){
        char aux = *(str+start);
        *(str+start) = *(str+end);
        *(str+end) = aux;
        start++;
        end--;
    }
}

  
// Implementation of itoa() - https://www.geeksforgeeks.org/implement-itoa/
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

int atoi(char * str){
    int number = 0;
    int mult = 1;
    int n = strlen(str);
    n = (int)n < 0 ? -n : n; /* quick absolute value check  */
    /* for each character in array */
    while (n--)
    {
        /* if not digit or '-', check if number > 0, break or continue */
        if ((str[n] < '0' || str[n] > '9') && str[n] != '-')
        {
                if (number)
                    break;
                else
                    continue;
        }

        if (str[n] == '-')
        { /* if '-' if number, negate, break */
                if (number)
                {
                    number = -number;
                    break;
                }
        }
        else
        { /* convert digit to numeric value   */
                number += (str[n] - '0') * mult;
                mult *= 10;
        }
    }

    return number;
}
 
//https://stackoverflow.com/questions/10156409/convert-hex-string-char-to-int
int hexToInt(char * num){
    if (num[0] == '0' && (num[1] != 'x' || num[1] != 'X')){
        int val = 0;
        while (*num) {
            // get current character then increment
            char byte = *num++; 
            // transform hex character to the 4bit equivalent number, using the ascii table indexes
            if (byte >= '0' && byte <= '9') byte = byte - '0';
            else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
            else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
            // shift 4 to make space for new digit, and add the 4 bits of the new digit 
            val = (val << 4) | (byte & 0xF);
        }
        return val;
    }
    return -1;
}

//Inspirada en itoa. La adaptamos para que str tenga la cantidad de bytes que queremos
char * intToHex(uint64_t num, char * str, int bytes) 
{ 
    int i = 0;

    if (num == 0) {
        str[i++] = '0';
        str[i++] = 'x';

        while (i < bytes*2 + 2) {
            str[i++] = '0';
        }

        str[i] = '\0'; 
        return str; 
    } 

    while (i < bytes*2 && num != 0) { 
        int rem = num % 16; 
        str[i++] = (rem >= 10)? (rem - 10) + 65 : rem + 48; 
        num = num/16; 
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

void sleep(int secs){
    char buffer[3] = {0};
    seconds(buffer);
    int initialSecs = (buffer[0] - '0') * 10 + (buffer[1] - '0');
    while(1){
        seconds(buffer);
        int auxSeconds = (buffer[0] - '0') * 10 + (buffer[1] - '0');
        if (auxSeconds - initialSecs >= secs)
            return;
    }
}

char *strstrip(char *s, char c) {
  while (*s != 0) {
    if (*s != c)
      break;
    s++;
  }
  return s;
}

char *strtokLib(char *s, char delim) {
  char *ptr = s;

  if (s == 0) {
    return 0;
  }

  int flag = 0;
  while (*ptr != 0) {
    if (*ptr == delim) {
      flag = 1;
      *ptr = 0;
    } else if (flag == 1)
      return ptr;
    ptr++;
  }
  return ptr;
}