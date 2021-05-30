#include <stdint.h>
#include <stdarg.h>

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

char * toHexa (long long number, int bytes){
	int size = bytes*+2;
	char res[size+1];
	res[size] = 0; //Null terminated
	res[0] = '0';
	res[1] = 'x';

	for (int i = 2; i < size; i++)
		res[i] = '0';

	if (number == 0)
		return res;
	
	size--;
	while (number != 0){
		int aux = number % 16;
		res[size--] = (aux >= 10)? (aux-10) + 'A' : aux + '0';
		number /= 16;
	}
	return res;
}

char *intToHexa(long long num, char *str, int bytes) 
{ 
    int i = 0;
    long long n = num;

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
  
    // Process individual digits 
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
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    reverse(str, i); 
	return str;
} 

void swap(char *x, char *y)
{
    char t = *x;
    *x = *y;
    *y = t;
}

// function to reverse buffer[i..j]
int hexaToInt(char* num){
    int len = strlen(num);

    // Initializing base value to 1, i.e 16^0
    int base = 1;

    int decVal = 0;

    // Extracting characters as digits from last character
    for (int i = len - 1; i >= 0; i--)
    {
        // if character lies in '0'-'9', converting
        // it to integral 0-9 by subtracting 48 from
        // ASCII value
        if (num[i] >= '0' && num[i] <= '9')
        {
            decVal += (num[i] - 48) * base;

            // incrementing base by power
            base = base * 16;
        }

        // if character lies in 'A'-'F' , converting
        // it to integral 10 - 15 by subtracting 55
        // from ASCII value
        else if (num[i] >= 'A' && num[i] <= 'F')
        {
            decVal += (num[i] - 55) * base;

            // incrementing base by power
        }

    }

    return decVal;
}

int strlen(char * string){
    int i = 0;
    while (string[i] != 0)
        i++;
    return i;
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
  /*
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

<<<<<<< HEAD
*/
=======
*/
>>>>>>> 664ba25eeda664f8e265c2f59e0b0abdb58c13f8
