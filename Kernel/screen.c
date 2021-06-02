#include "naiveConsole.h"
#include "stdint.h"
#include "naiveConsole.h"

static uint8_t * const video = (uint8_t *)0xB8000;
static uint32_t width;
static uint32_t height;
static char divider = '-';
static int screen = 0;

void initScreen()
{
    setCursor(0); //cursor = video
    setSize(80, 25);
}

void initDoubleScreen() {
    initScreen();
    int half = width * height / 2;
    int halfLine = half - (half % width);
    setCursor(halfLine);
    do{
        ncPrintChar(divider);
        setCursor(getCursor() + 1);
        halfLine++;
    } while (halfLine % width != 0);
    
}

//cursor es un valor de 0 hasta (80x25)-1

void setSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
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
    if(screen != whichScreen) {
        screen = whichScreen;
        int half = width * height / 2;
        if(screen == 0) {
            moveCursor(half * -1);
        } else if (screen == 1) {
            moveCursor(half);
        }
    }
}

//setCursor((height - 1) * width); (para el scroll up)
