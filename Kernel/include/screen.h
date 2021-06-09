#ifndef SCREEN_H
#define SCREEN_H

void initScreen();
void setSize(int newWidth, int newHeight); 
void setCursor(int pos);
int getCursor();
void moveCursor(int move);
void changeScreen(int whichScreen);
void initDoubleScreen(int screen);
void setMove(int option);
void setStart();
int getCurrentScreen();


#endif
