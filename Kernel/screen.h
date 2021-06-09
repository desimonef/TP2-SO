#ifndef SCREEN_H
#define SCREEN_H

void initScreen();
void initDoubleScreen(int screen);
void setSize(int newWidth, int newHeight); 
void setStart();
void setMove(int option);
void setCursor(int pos);
int getCursor();
void moveCursor(int move);
void changeScreen(int whichScreen);
int getCurrentScreen();

#endif
