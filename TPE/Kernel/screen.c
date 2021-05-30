#include <naiveConsole.h>
#include <screen.h>

static uint8_t *const video = (uint8_t *)0xB8000;
static uint32_t width;
static uint32_t height;

void initScreen() {
    setCursor(0); //cursor = video
    setSize(80, 25);
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

void printString(char * string, int pos, char colour, char background) {
    setCursor(pos);
    char attribute = (background << 4) | colour;
    for (int i = 0; string[i] != 0; i++)
        ncPrintCharAtt(string[i], attribute);
}

void newline()
{
    int cursor = getCursor();
    do
    {
        ncPrintChar(' ');
    } while ((uint64_t)cursor % width != 0);
}

void scrollUp()
{
    for (int i = 0; i < height - 1; i++)
    {
        for (int j = 0; j < width * 2; j++)
        {
            video[j + i * width * 2] = video[j + (i + 1) * width * 2];
        }
    }
    for (int k = 0; k < width * 2; k++)
        video[(height - 1) * width * 2 + k] = '\0';
    setCursor((height - 1) * width);
}

void checkPosition()
{
    if (getCursor() >= width * height)
        scrollUp();
}