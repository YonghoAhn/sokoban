#include "getch.h"
#include "sokoban.h"
#include <string.h>
#include <windows.h>

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

int currentRound = 0;
int undoCount = 0;

int rank[5];
int score[5];
int map[5];
char name[5];

/*게임이 끝난 후 점수와 맵을 랭킹에 저장*/
void AddRank(int map, int score)
{
    //맵과 점수를 저장
	RankSort();
	FileSave();
}
       
/*점수(움직인 횟수)에 따라 랭크 정렬*/
void RankSort(void)
{
    int cnt;
    rank[5];

	for (int i = 0; i < 5; i++) {
    	cnt = 0; //매번 i 바뀔 때 초기화.
		for (int j = 0; j < 5; j++) {
            if (score[i] > score[j]) { //자신보다 크다면 cnt 1증가.
                cnt++;
			}
		}
        rank[i] = cnt + 1; //등수 결정.
    }
	FileSave();
}

/*파일을 열어 저장되어있던 데이터를 불러옴*/
void FileLoad(void)
{
	int i;
	FILE *savefile;
	
	savefile=fopen("ranking.txt","rt");

	
	if(savefile == NULL){ //오류 or 파일 없을시 새로 생성 
		savefile=fopen("ranking.txt","a");
		fclose(savefile);
		return;
	}
	
	
	for(i=0; i<5; i++)
		fscanf(savefile,"%d %d %d %s\n",&rank[i], &map[i], &score[i], &name[i]);
		
	fclose(savefile);
}

/*파일을 열어 TOP 5위 랭킹기록 후 저장*/
void FileSave(void)
{	
	int i;
	FILE *savefile;
	savefile=fopen("ranking.txt","wt");
	
	for(i=0; i<5; i++){
		if(score[i] == 0) //점수가 0점이면
			fprintf(savefile,"0 0 ---\n");
		else	
		    fprintf(savefile,"%d %d %d %s\n",rank[i], map[i], score[i], name[i]);	
	}
	fclose(savefile);
}

/*랭킹 띄우기 */
void RankDraw(void)
{
    int i;

    FileLoad();
	RankSort();
    
    for(i=0; i<5; i++){
        printf("[%d]위 닉네임: %s , 점수 : %d\n",rank[i], name[i], score[i]);
		Sleep(200);
    }
    system("cls");
}

/*랭킹 커맨드 */
void rankingCommand()
{
    char ch;
    int x;
    char Y;
    ch = getch();
    switch(ch){
        case 't' :
        do{
            printf("t");
            scanf("%d", &x);
            getchar();
            if(x == 1){
            printf("%d\n", map[0]);
            RankDraw();
            }
            if(x == 2){
            printf("%d\n", map[1]);
            RankDraw();
            }
            if(x == 3){
            printf("%d\n", map[2]);
            RankDraw();
            }
            if(x == 4){
            printf("%d\n", map[3]);
            RankDraw();
            }
            if(x == 5){
            printf("%d\n", map[4]);
            RankDraw();
            }
            scanf("%c", &Y);
        }while(Y == 'y');
        break;
    }
}

void cls() 
{ 
    system("clear"); 
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
    switch(ch){
        case 'u' :
            break;
        case 'r' :
            break;
        case 'n' :
            break;
        case 'e' :
            break;
        case 's' :
            break;
        case 'f' :
            break;
        case 'd' :
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
                case '.' :
                    stage[playerY + deltaY * 2][playerX + deltaX * 2] = '$';
                    stage[playerY+deltaY][ playerX + deltaX] = '.';
                    break;
                case '#' :
                case '$' :
                    return;
                case 'O':
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
        //recordUndo(ch);
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
    printf("test");
    loadMap();
    initStage();
    while(true)
    {
        inputCommand();
        rankingCommand();
        if(isStageClear())
        {
            //if Stage cleared, then go to next map / or end game
            cls();
            //remove all undo data
            //clearData();
            //updateRanking();
            currentRound++;
            if(currentRound >= MAXSIZE) exit(0);
            undoCount = 0;
            numMove = 0;
            initStage();
            continue;   
        }
    }
    return 0;
}