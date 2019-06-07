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
void updateRanking(void);
void loadRanking(void);
void saveRanking(void);
void displayRanking(int);

//김태엽 status
void saveStatus();
void loadStatus();
void recordUndo(char);
void undoMovement();
void clearData();

//배준서 key
void replayStage();
void newStage();
void exitGame();
void drawHelp();
