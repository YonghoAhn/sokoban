#pragma once
#include <stdbool.h>
#include <stdlib.h>

_Bool checkValidMap(int index);
void loadMap();
void drawStage();
void inputCommand();
void movePlayer(char ch);
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