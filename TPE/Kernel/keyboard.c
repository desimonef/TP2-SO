#include <stdint.h>
#include <lib.h>
#include <keyboard.h>
#include <screen.h>


//entre codes de pressed y released hay un defasaje de 128
#define INPUT_BUFFER 255 //Cant max de caracteres en buffer de input
#define SHIFT_PRESS 42
#define SHIFT_RELEASE 170
#define CAPS_PRESS 58
#define ENTER 28
#define TAB 15
#define UP 0
#define LEFT 1
#define RIGHT 2
#define DOWN 3

static char keyboardBuffer[INPUT_BUFFER];
static unsigned int bufferSize = 0;
static unsigned int readIndex = 0;
static int shift = 0;

int readKey();


static const char asciiTable[] = {
    0, 0, '1', '2',
    '3', '4', '5', '6',
    '7', '8', '9', '0',
    '\'', '¡', 0, 0,
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i',
    'o', 'p', '`', '+',
    0, 0, 'a', 's',
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

uint8_t getMatchingAscii(int scancode){
  isShiftPressed(scancode);
  isArrowKey(scancode);
  if (codeMatchesAscii(scancode)) {
    if (shift == 0)
      return asciiTable[scancode];
    else
      return asciiTable2[scancode];
  }
  else if (scancode == TAB)
  {
    int current = getCursor();
    setCursor(current + 5);
  }
  return 0;
}

/*
void onKeyPressed(char chr) {
  if (bufferSize < INPUT_BUFFER)
    keyboardBuffer[bufferSize++] = chr;
} */

void isShiftPressed(int scancode)
{
  if (scancode == SHIFT_PRESS || scancode == SHIFT_RELEASE || scancode == CAPS_PRESS)
    shift = 1 - shift;
  
}

void isArrowKey(int scancode) {
  switch (scancode)
  {
  case UP:
    changeScreen(0);
    break;
  case LEFT:
    moveCursor(-1);
    break;
  case RIGHT:
    moveCursor(1);
    break;
  case DOWN:
    changeScreen(1);
    break;
  }
}

void checkIndex() {
  if (readIndex >= INPUT_BUFFER)
  {
    readIndex = readIndex % INPUT_BUFFER;
  }
}

int readFromKeyboard(char * dest, int amount) {
    int dim = 0;
    while(amount > 0) {
        dest[dim++] = keyboardBuffer[readIndex++];
        checkIndex();
        amount--;
    }
    return dim;
}

void store(char c) {
    keyboardBuffer[bufferSize] = c;
    bufferSize = (bufferSize+1)%INPUT_BUFFER; 
}

void keyboard_handler(){
    int scanCode = readKey();
    char c = getMatchingAscii(scanCode);
    store(c);
}