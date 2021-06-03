#include "stdint.h"
#include "lib.h"
#include "keyboard.h"
#include "naiveConsole.h"

//entre codes de pressed y released hay un defasaje de 128
#define INPUT_BUFFER 255 //Cant max de caracteres en buffer de input
#define SHIFT_PRESS 42
#define SHIFT_RELEASE 170
#define CAPS_PRESS 58
#define ENTER 28
#define ENTER_RELEASE 156
#define TAB 15
#define UP 72
#define UP_RELEASE 200
#define LEFT 75
#define LEFT_RELEASE 203
#define RIGHT 77
#define RIGHT_RELEASE 205
#define DOWN 80
#define DOWN_RELEASE 208

static char keyboardBuffer[INPUT_BUFFER];
static unsigned int bufferSize = 0;
static unsigned int readIndex = 0;
static int shift = 0;
static int unread = 0;

static const char asciiTable[] = {
    0, 0, '1', '2',
    '3', '4', '5', '6',
    '7', '8', '9', '0',
    '\'', '¡', 0, 0,
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
    0, 0, 0, 0};

static const char asciiTableShift[] = {
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
  int isArrow = isArrowKey(scancode);
  if (scancode == ENTER || scancode == ENTER_RELEASE)
  {
    setMove(1);
  }
  else if (isArrow == 0)
  {
    setMove(0);
  }
  if (codeMatchesAscii(scancode)) {
    if (shift == 0)
      return asciiTable[scancode];
    else
      return asciiTableShift[scancode];
  }
  else if (scancode == TAB)
  {
    int current = getCursor();
    setCursor(current + 5);
  }
  return 0;
}

void isShiftPressed(int scancode)
{
  if (scancode == SHIFT_PRESS || scancode == SHIFT_RELEASE || scancode == CAPS_PRESS)
    shift = 1 - shift;
  
}

int isArrowKey(int scancode) {
  
  switch (scancode)
  {
    case UP:
      changeScreen(0);
    case LEFT:
      moveCursor(-1);
    case RIGHT:
      moveCursor(1);
    case DOWN:
      changeScreen(1);
  }
  if (scancode == UP || scancode == DOWN || scancode == RIGHT || scancode == LEFT || scancode == UP_RELEASE || scancode == LEFT_RELEASE || scancode == RIGHT_RELEASE || scancode == DOWN_RELEASE)
    return 1;
  return 0;
}

void checkIndex() {
  if (readIndex >= INPUT_BUFFER)
  {
    readIndex = readIndex % INPUT_BUFFER;
  }
}

int keyboardRead(char *ret, int count) {
  if(unread <= 0) {
    return -1;
  }
    
  int size = 0;
  for(int i=0; i<count; i++) {
    ret[size] = keyboardBuffer[readIndex];
    size++;
    readIndex++;
    checkIndex();
    unread--;
  }
  ret[size] = 0;
  return size;
}

void store(char c) {
    keyboardBuffer[bufferSize] = c;
    bufferSize = (bufferSize+1)%INPUT_BUFFER;
    unread++;
    
}

void keyboard_handler(){
  int scanCode = readKey();
  char c = getMatchingAscii(scanCode);
  store(c);

}
