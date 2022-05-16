// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "naiveConsole.h"
#include "stdint.h"
#include "naiveConsole.h"
#include "keyboard.h"
#include "screen.h"

static uint8_t * const video = (uint8_t *)0xB8000;
static uint32_t width;
static uint32_t height;
static char divider = '-';
static int screen = 1;
static int canMove = 1;
static int start = 0;
static int upCursor = 0;
static int downCursor = 0;

void initScreen()
{
    setCursor(0); //cursor = video
    setSize(80, 25);
    resetKeyboardFlags();
    start = 0;
    canMove = 1;
    upCursor = 0;
    downCursor = 0;
    screen = 1;
}

void initDoubleScreen(int mode) {
    initScreen();
    /*int half = width * height / 2;
    int halfLine = half - (half % width);
    setCursor(halfLine);
    do{
        ncPrintChar(divider);
        halfLine++;
        setCursor(halfLine);
    } while (halfLine % width != 0);
    if (mode == 0){
        setCursor(width);
        downCursor = halfLine;
        screen = 0;
    }
    else if (mode == 1){
        setCursor(halfLine+width);
    }*/
}

//cursor es un valor de 0 hasta (80x25)-1

void setSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
}

void setStart() {
    if(start <= 2)
        start++;
        /*
    if(start == 2)
        downCursor = getCursor();
        */
}

int canDelete() {
    return getCursor() % width > 2;
}

void setMove(int option) {
    if(start > 2)
        canMove = option;
}

void setCursor(int pos) {
    setCurrent(video + pos*2); //setea el cursor en una posición relativa a la de inicio de la pantalla
}

int getCursor() {
    uint8_t * pos = getCurrent();
    return ((int)(pos - video))/2; //retorno posición relativa
}

void moveCursor(int move) {
    int pos = getCursor();
    setCursor(pos + move);
}

void changeScreen(int whichScreen) {
   if(canMove) {
       if (screen != whichScreen)
       {
           screen = whichScreen;
           if (screen == 0)
           {
               downCursor = getCursor();
               setCursor(upCursor);
           }
           else if (screen == 1)
           {
               upCursor = getCursor();
               setCursor(downCursor);
           }
       }
   }
}

int getCurrentScreen(){
    return screen;
}
