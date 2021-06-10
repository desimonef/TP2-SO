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

void clearBuff() {
    buffSize = 0;
    int c;
    do {
        scan(0, buffer, 512);
    } while((c = getchar()) != -1);
    
}

int scanf(char *command, ...){
    va_list args;
    va_start(args, command);
    clearBuff();
    readLine(); //guarda en buffer los argumentos
    int bufferIndex = 0;
    int error = 0;
    int commandIndex = 0;
    char *string;
    int stringIndex;

    while (command[commandIndex] != '\0' && buffer[bufferIndex] != '\0' && !error){
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

void readLine(){
    buffSize = 0;
    int c;
    while ((c = getchar()) != '\n'){
        if (c == '\b'){
            if (buffSize != 0)
                buffSize--;
        }
        else if (c != -1){
            if (buffSize < MAX_BUFFER - 1)
                buffer[buffSize++] = c;
            putchar(c);
        }
    }
    putchar('\n');
    buffer[buffSize++] = '\0';
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

  
// Implementation of itoa() - https://www.geeksforgeeks.org/implement-itoa/ adaptada
char* itoa(int num, char* str, int base)
{
    int i = 0;
  
    //Handle 0 explicitely, otherwise empty string is printed for 0 
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
  
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'A' : rem + '0';
        num = num/base;
    }
  
    str[i] = '\0'; // Append string terminator
  
    // Reverse the string
    reverse(str, i);
  
    return str;
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


//Inspirado en itoa. La adaptamos para que str tenga la cantidad de bytes que necesitamos
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

