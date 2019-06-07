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

char inputCache[5][30][30] = {0,};
int playerPos[5][2] = {0,};
int undoCount = 0;
int undoIndex = 0;

int currentRound = 0;

int rank[5];
int score[5];
int map[5];
char name[5];

/*게임이 끝난 후 점수와 맵을 랭킹에 저장*/
void addRanking(int map, int score)
{
    //맵과 점수를 저장
	sortRanking();
	saveRanking();
}
       
/*점수(움직인 횟수)에 따라 랭크 정렬*/
void sortRanking(void)
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
	saveRanking();
}

/*파일을 열어 저장되어있던 데이터를 불러옴*/
void loadRanking(void)
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
void saveRanking(void)
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
void drawRank(void)
{
    int i;

    loadRanking();
	sortRanking();
    
    for(i=0; i<5; i++){
        printf("[%d]위 닉네임: %s , 점수 : %d\n",rank[i], name[i], score[i]);
		Sleep(200);
    }
    cls();
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
            do {
                printf("t");
                scanf("%d", &x);
                getchar();
                printf("%d\n",map[x-1]);
                drawRank();
                scanf("%c", &Y);
            } while(Y == 'y');
        break;
    }
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
    fprintf(fp,"%d\n%d\n%d\n%d\n%d\n",numMove,undoCount,playerX,playerY,currentRound);
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
    fscanf(fp,"%d",&undoCount);
    fscanf(fp,"%d",&playerX);
    fscanf(fp,"%d",&playerY);
    fscanf(fp,"%d",&currentRound);
    fgetc(fp);
    cls();
    //gotoxy(1,1);
    //어차피 30,30 사이즈로 맵을 출력할건데 굳이 나눌 필요가 있을까
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
    puts("SOKOBAN");
    puts("e: 종료, r: 다시시작");
    puts("n: 다음, d: 도움말");
    printf("스테이지 : %d\n",currentRound+1);
    printf("이동 횟수 : %d\n",numMove);

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
            rankingCommand();
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
            //updateRanking();
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