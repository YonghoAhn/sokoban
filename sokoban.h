#pragma once
#include <stdbool.h>
#include <stdlib.h>

_Bool checkValidMap(int);
void loadMap();
void drawStage();
void inputCommand();
void movePlayer(char);
_Bool isStageClear();
void cls();
void initStage();
void findPlayerLocation();
void saveStatus();
void loadStatus();
void recordUndo(char);
void undoMovement();
void clearData();