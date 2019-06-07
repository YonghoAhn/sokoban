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

//황유호 ranking
void AddRank(int map, int score);
void RankSort(void);
void FileLoad(void);
void FileSave(void);
void RankDraw(void);
void rankingCommand();

//김태엽 status
void saveStatus();
void loadStatus();
void recordUndo(char);
void undoMovement();
void clearData();
