#include <stdint.h>
#include <lib.h>
#include <keyboard.h>


//entre codes de pressed y released hay un defasaje de 128
#define INPUT_BUFFER 255 //Cant max de caracteres en buffer de input

static char keyboardBuffer[INPUT_BUFFER];
static unsigned int bufferSize = 0;
static int shift = 0;

int readKey();


static const char asciiTable[] = {
    0, 0, '1', '2',
    '3', '4', '5', '6',
    '7', '8', '9', '0',
    '\'', '¡', 0, '\t',
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i',
    'o', 'p', '`', '+',
    '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h',
    'j', 'k', 'l', 'ñ',
    '´', 'ç', 0, '<',
    'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',',
    '.', '-', 0, 0, 0,
    ' ', 0, 0, 0, 0,
    0, 0, 0, 0
  };

static const char asciiTable2[] = {
    0, 0, '!', '"',
    '#', '$', '%', '&',
    '/', '(', ')', '=',
    '¿', 0, 0, '\t',
    'Q', 'W', 'E', 'R',
    'T', 'Y', 'U', 'I',
    'O', 'P', '[', ']',
    '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H',
    'J', 'K', 'L', 'Ñ',
    '{', '{', 0, '}',
    'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ';',
    ':', '_', 0, 0, 0,
    ' ', 0, 0, 0, 0,
    0, 0, 0, 0};

int codeMatchesAscii(int scancode){
  return scancode >= 0 && scancode <= 57 && asciiTable[scancode] != 0;
}


int isLetter(char c){
  return (c >= 'a' && c <= 'z');
}

uint8_t getMatchingAscii(int scancode){
  if (codeMatchesAscii(scancode)) {
    if (shift == 0)
      return asciiTable[scancode];
    else
      return asciiTable2[scancode];
  }
  return 0;
}

void onKeyPressed(char chr) {
  if (bufferSize < INPUT_BUFFER)
    keyboardBuffer[bufferSize++] = chr;
}

void isShiftPressed(int scancode)
{
  if (scancode == 42)
    shift = 1;
  if (scancode == 170)
    shift = 0;
  if(scancode == 58)
    shift = 1 - shift;
}

uint8_t keyboard_handler(){
    int scanCode = readKey();
    isShiftPressed(scanCode);
    return getMatchingAscii(scanCode);
    //char c = getMatchingAscii(scanCode);
    //onKeyPressed(c);
}