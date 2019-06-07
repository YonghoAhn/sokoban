#include "getch.h"
#include "sokoban.h"
#include <string.h>

#define UP 106
#define DOWN 107
#define LEFT 104
#define RIGHT 108
#define ONE 49
#define FIVE 53
#define MAXSIZE 5

char stage[30][30] = {' ', };
char StageData[5][30][30];
char username[10];

int numMove = 0;

int playerX = 0;
int playerY = 0;

char inputCache[5][30][30] = {0,};
int playerPos[5][2] = {0,};
int undoCount = 0;
int undoIndex = 0;

int currentRound = 0;

char rankingName[5][5][10];
int rankingMove[5][5];

void loadRanking()
{
    FILE *fp = fopen("ranking","r");
    char buffer[50];
    int i = 0;
    int map = 0;
    while(true)
    {
        fscanf(fp,"%s",&buffer); //read line
        if(buffer[0] == '#')
        {
            //buffer[1] will be the map number
            map = buffer[1] - ONE;
        }
        else if (buffer[0] == 'e')
        { break; }
        else
        {
            int score = 0;
            fscanf(fp,"%d",&score);
            rankingMove[map][i] = score;
            strncpy(rankingName[map][i], buffer,10);
            i++;
        }
    }
    fclose(fp);
}

void updateRanking()
{
    //Loop current stage ranking and change it
    int i, j = 0;
    for(i = 0; i < 5; i++)
    {
        //checking from top (lowest)
        //if lowest ranking was 0, then there is 
        if(rankingMove[currentRound][i] > numMove || rankingMove[currentRound][i] == 0)
        {
            for(j = 4; j > i+1; j--)
            {
                rankingMove[currentRound][j] = rankingMove[currentRound][j-1];
                strcpy(rankingName[currentRound][j],rankingName[currentRound][j-1]);
            }

            strcpy(rankingName[currentRound][i], username);
            rankingMove[currentRound][i] = numMove;
            break;
        } //compare move count
    }
    saveRanking();
}

void saveRanking()
{
    FILE *fp = fopen("ranking", "w");
    for(int i = 0; i < 5; i++)
    {
        fprintf(fp,"#%d\n",i+1);
        for(int j = 0; j < 5; j++)
        {
            if(strlen(rankingName[i][j]) > 0)
            {
                fprintf(fp,"%s %d\n", rankingName[i][j], rankingMove[i][j]);
            }
        }
    }
    fprintf(fp,"e");
    fclose(fp);
}


void displayRanking(int map)
{
    cls();
    if(map == 0)
    {
        for(int j = 0; j < 5; j++) { 
            printf("map %d\n",j+1);
            for(int i = 0; strlen(rankingName[j][i]);i++)
            {
                printf("%s\t%d\n",rankingName[j][i],rankingMove[j][i]);
            }
        }
    }
    else 
    {
        getchar();
        printf("map %d\n",map);
        for(int i = 0; strlen(rankingName[map-1][i]);i++)
        {
            printf("%s\t%d\n",rankingName[map-1][i],rankingMove[map-1][i]);
        }
    }
    while(1) {
        char c = getchar();
        if(c == '\n') {
            cls();
            drawStage();
            inputCommand();
            return;
        }
    }
}

/*랭킹 커맨드 */
void rankingCommand()
{

}

void cls() 
{ 
    system("clear"); 
}

void clearData()
{
     for(int t = 0; t < 5; t++) {
        for(int i = 0; i < 30; i++)
        {
            for(int j = 0; j < 30; j++) {
                inputCache[t][i][j] = ' ';           
            }
        }
    }
}

void saveStatus() 
{
    FILE *fp = fopen("sokoban","w");
    fprintf(fp,"%d\n%d\n%d\n%d\n",numMove,playerX,playerY,currentRound);
    for(int y = 0; y < 30; y++)
    {   
        if(strlen(stage[y]) < 1)
            fprintf(fp,"                            \n");
        else
            fprintf(fp,"%s\n",stage[y]);
    }
    fclose(fp);
}

void loadStatus() 
{
    FILE *fp = fopen("sokoban","r");
    char buffer[30] = {0,};
    int mapNum = 0;
    int i = 0;

    clearData();
    fscanf(fp,"%d",&numMove);
    fscanf(fp,"%d",&playerX);
    fscanf(fp,"%d",&playerY);
    fscanf(fp,"%d",&currentRound);
    fgetc(fp);
    cls();
    for(i = 0; i < 30; i++)
    {
        fgets(buffer,30,fp);
        strcpy(stage[i],buffer);
        stage[i][strlen(stage[i])-1] = '\0';
    }
    fclose(fp);
    drawStage();
    inputCommand();
}

void replayStage()
{
    initStage();
}

void newStage()
{
    numMove = 0;
    currentRound = 0;
    initStage();
}

void exitGame()
{
    cls();
    printf("S E E\tY O U\t%s\n",username);
    exit(0);
}

void drawHelp()
{
    cls();
    puts("SOKOBAN");
    puts("e: 종료, r: 다시시작");
    puts("n: 다음, d: 도움말");
    printf("스테이지 : %d\n",currentRound+1);
    printf("이동 횟수 : %d\n",numMove);
    while(1) {
        char c = getchar();
        if(c == '\n') {
            cls();
            drawStage();
            inputCommand();
            return;
        }
    }
}

void recordUndo(char ch)
{
    if(undoIndex < 4) {
        undoIndex++;
    }
    else {
        for(int i = 0; i < 4; i++){
            for(int cy = 0; cy < 30; cy++)
            {
                for(int cx = 0; cx < 30; cx++)
                {
                    inputCache[i][cy][cx] = inputCache[i+1][cy][cx];
                }
            }
            playerPos[i][0] = playerPos[i+1][0];
            playerPos[i][1] = playerPos[i+1][1];
        }
    }

    for(int cy = 0; cy < 30; cy++)
    {
        for(int cx = 0; cx < 30; cx++)
        {
           inputCache[undoIndex][cy][cx] = stage[cy][cx];
        }
    }

    playerPos[undoIndex][0] = playerX;
    playerPos[undoIndex][1] = playerY;
}

void undoMovement()
{
    if(undoIndex <= 0) return;
    if(undoCount++ > 4) return;
    for(int cy = 0; cy < 30; cy++)
    {
        for(int cx = 0; cx < 30; cx++)
        {
            stage[cy][cx] = inputCache[undoIndex][cy][cx];
        }
    }
    playerX = playerPos[undoIndex][0];
    playerY = playerPos[undoIndex][1];
    undoIndex--;
    numMove++;
    drawStage();
    inputCommand();    
}

_Bool checkValidMap(int index)
{
    int cntBox, cntHole = 0;
    for(int y = 0; y < 30; y++)
    {
        for(int x = 0; x < 30; x++)
        {
            if(StageData[index][y][x] == 'O')
            {
                cntHole++;
            }
            else if(StageData[index][y][x] == '$')
            {
                cntBox++;
            }
        }
    }
    if(cntHole == cntBox)
        return true;
    else
        return false;
}

void loadMap()
{
    char buffer[30] = {'#', }; //Map size is 30 or less, init array to 0
    FILE *fp = fopen("map","r"); //Open file pointer
    int line = 0; //count line index
    int mapIndex = 0; //count map index
    while(true) //read file until EOF
    {
        fscanf(fp,"%s",&buffer);
        //check map ends : return;
        if(buffer[0] == 'e') { break; }
        //check next map
        else if(buffer[0] >= ONE && buffer[0] <= FIVE) 
        {
             if(!checkValidMap(mapIndex)) //if not valid
             {
                cls();
                printf("%d 번 맵에서 오류가 발견되었습니다. 종료합니다.\n",mapIndex+1);
                exit(-1);
             }
             mapIndex = buffer[0] - 49; 
             line = 0; 
        }
        else
        {
            strncpy(StageData[mapIndex][line],buffer,30); 
            line+=1;
        }
        //get stage
    }
    fclose(fp);
}

void drawStage() 
{
    cls();
    int stageX, stageY;
    printf("Hello %s\n",username);
    printf("Move : %d\n",numMove);
    
    for(stageY = 0; stageY < 30; stageY++)
    {
        if(strlen(stage[stageY]) > 0)
            printf("%s\n",stage[stageY]);
    }
}

void inputCommand()
{
    char ch;
    int rankingPage = 0;
    ch = getch();
    if(ch == 't') {
        printf("t ");
        int map = 0;
        ch = getchar();
        if(ch == '\n')
            displayRanking(0);
        else if(ch - ONE >= 0 && ch - ONE <= 4) {
            ch -= ONE;
            displayRanking(ch+1);
        }
    } else {
        switch(ch){
            case 'u' :
                undoMovement();
                break;
            case 'r' :
                replayStage();
                break;
            case 'n' :
                newStage();
                break;
            case 'e' :
                exitGame();
                break;
            case 's' :
                saveStatus();
                break;
            case 'f' :
                loadStatus();
                break;
            case 'd' :
                drawHelp();
                break;
            case 'h' :
            case 'j' :
            case 'k' :
            case 'l' :
                movePlayer(ch);
                break;
            default : 
                break;
        }
    }
} 

void movePlayer(char ch)
{
    int deltaX, deltaY = 0;
    switch(ch)
        {
            case 'h' :
                deltaX = -1;
                deltaY = 0;
                break;
            case 'j' :
                deltaX = 0;
                deltaY = 1;
                break;
            case 'k' :
                deltaX = 0;
                deltaY = -1;
                break;
            case 'l' :
                deltaX = 1;
                deltaY = 0;
                break;
        }

        if (stage[playerY+deltaY][ playerX + deltaX] == '#')
            return;
        else if (stage[playerY+deltaY][playerX + deltaX] == '$')
        {   
            //check two step forward
            switch(stage[playerY + (deltaY * 2)][playerX + (deltaX * 2)]) {
                case '#' :
                case '$' :
                    return;
            }
        }

        recordUndo(ch);

        if (stage[playerY+deltaY][playerX + deltaX] == '$')
        {
            switch(stage[playerY + (deltaY * 2)][playerX + (deltaX * 2)]) {
                case '.' :
                case 'O' :
                    stage[playerY+deltaY][ playerX + deltaX] = '.';
                    stage[playerY + deltaY * 2][playerX + deltaX * 2] = '$';
                    break;
            }
        }

        if(StageData[currentRound][playerY][playerX] != 'O')
            stage[playerY][playerX] = '.';
        else
        {
            stage[playerY][playerX] = 'O';
        }
        
        playerX+=deltaX;
        playerY+=deltaY;
        
        stage[playerY][playerX] = '@';

        //if player collided to a wall, then don't refresh.
        numMove++; 
         
        drawStage();
}

_Bool isStageClear()
{
    for(int cy = 0; cy < 30; cy++)
    {
        for(int cx = 0; cx < 30; cx++)
        {
            if(StageData[currentRound][cy][cx] == 'O' && stage[cy][cx] != '$')
            {
                return false;
            }
        }
    }
    return true;
}

void initStage()
{
    for(int cy = 0; cy < 30; cy++)
    {
        for(int cx = 0; cx < 30; cx++)
        {
            stage[cy][cx] = StageData[currentRound][cy][cx];
        }
    }
    findPlayerLocation();
    drawStage();
}

void findPlayerLocation()
{
    for(int y = 0; y < 30; y++)
    {
        for(int x = 0; x < 30; x++)
        {
            if(StageData[currentRound][y][x] == '@')
            {
                playerX = x;
                playerY = y;
            }
        }
    }
}


int main(void)
{
    printf("Start....\nInput name : ");
    scanf("%s",username);    
    loadMap();
    loadRanking();
    initStage();
    while(true)
    {
        inputCommand();
        if(isStageClear())
        {
            //if Stage cleared, then go to next map / or end game
            cls();
            //remove all undo data
            //clearData();
            updateRanking();
            currentRound++;
            if(currentRound >= MAXSIZE) exitGame();
            undoCount = 0;
            numMove = 0;
            initStage();
            continue;   
        }
    }
    return 0;
}