#include "stdint.h"
#include "lib.h"
#include "keyboard.h"
#include "naiveConsole.h"
#include "screen.h"

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
#define ARROWS 224
#define BACKSPACE 14

static char keyboardBuffer[INPUT_BUFFER];
static unsigned int bufferSize = 0;
static unsigned int readIndex = 0;
static int shift = 0;
static int unread = 0;
static int first = 0;

static const char asciiTable[] = {
    0, 0, '1', '2',
    '3', '4', '5', '6',
    '7', '8', '9', '0',
    '\'', '¡', '\b', '\t',
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
    ' ', ' ', ' ', 0, 0,
    0, 0, 0, 0
  };

static const char asciiTableShift[] = {
    0, 0, '!', '"',
    '·', '$', '%', '/',
    '/', '(', ')', '=',
    '?', '¿', 0, '\t',
    'Q', 'W', 'E', 'R',
    'T', 'Y', 'U', 'I',
    'O', 'P', '^', '*',
    '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H',
    'J', 'K', 'L', 'Ñ',
    '¨', 'Ç', 0, '>',
    'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ';',
    ':', '_', 0, 0, 0,
    0, ' ', 0, 0,
    0, 0, 0, 0
};


char getAscii(int scancode)
{
  shiftPressing(scancode);
  int arrow = isArrow(scancode);
  if (arrow == 1)
    return -1;
  if(scancode == BACKSPACE && canDelete())
      backspace();
  if (arrow == 0)
    screenMove(scancode);

  if (codeMatchesAscii(scancode))
  {
      if (shift == 0)
        return asciiTable[scancode];
      else
        return asciiTableShift[scancode];
  }

  if (scancode == TAB)
    moveCursor(5);

  return 0;
}

int codeMatchesAscii(int scancode)
{
  return scancode >= 0 && scancode <= 57 && asciiTable[scancode] != 0;
}

int isArrow(int scancode)
{
  int toRet = 0;
  switch(scancode) {
    case UP:
      changeScreen(0);
      if (first == 0){
        first = 1;
        ncPrint("$>");
      }
      toRet = 1;
      break;
    case DOWN: 
      changeScreen(1);
      if (first == 0){
        first = 0;
        ncPrint("$>");
      }
      toRet = 1;
      break;
    case LEFT:
      moveCursor(-1);
      toRet = 1;
      break;
    case RIGHT:
      moveCursor(1);
      toRet = 1;
      break;
    case UP_RELEASE:
      toRet = 1;
      break;
    case DOWN_RELEASE:
      toRet = 1;
      break;
    case LEFT_RELEASE:
      toRet = 1;
      break;
    case RIGHT_RELEASE:
      toRet = 1;
      break;
    case ARROWS:
      toRet = 1;
      break;
  }
  
  return toRet;
}

void screenMove(int scancode)
{
  if (scancode == ENTER || scancode == ENTER_RELEASE)
  {
    setMove(1);
  }
  else
  {
    setMove(0);
  }
}

void shiftPressing(int scancode)
{
  if (scancode == SHIFT_PRESS || scancode == SHIFT_RELEASE || scancode == CAPS_PRESS)
    shift = 1 - shift;
}
void checkIndex()
{
  if (readIndex >= INPUT_BUFFER)
  {
    readIndex = readIndex % INPUT_BUFFER;
  }
}

int keyboardRead(char *ret, int count)
{
  if (unread <= 0)
  {
    return -1;
  }

  int i = 0;
  for ( ; i < count; i++)
  {
    ret[i] = keyboardBuffer[readIndex];
    readIndex++;
    checkIndex();
    unread--;
  }
  ret[i] = 0;
  return i;
}

void store(char c)
{
  keyboardBuffer[bufferSize] = c;
  bufferSize = (bufferSize + 1) % INPUT_BUFFER;
  unread++;
}


void keyboard_handler()
{
  int scancode = readKey();
  setStart();
  char c = getAscii(scancode);
  if (c != -1)
    store(c);
}

void resetKeyboardFlags(){
  first = 0;
}
