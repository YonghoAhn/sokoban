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
char stage[30][30] = {
    ' ',
};                         //2차원 배열 ' '로 초기화
char StageData[5][30][30]; //Stage를 위한 배열
char username[10];         //유저 네임을 받기위한 변수
int numMove = 0;           //이동한 횟수
int playerX = 0;           //플레이어의 X좌표
int playerY = 0;           //플레이어의 y좌표
char inputCache[5][30][30] = {
    0,
}; //입력된 정보를 위한 임시 배열
int playerPos[5][2] = {
    0,
};                          // player의 위치를 표시하는 배열
int undoCount = 0;          //되돌리기를 위한 변수
int undoIndex = 0;          //되돌리기를 위한 뼌수
int currentRound = 0;       //현재 라운드를위한 변수
char rankingName[5][5][10]; // 랭킹의 이름을 저장을 위한 배열
int rankingMove[5][5];      // 랭킹의 움직인 횟수를 위한 배열
void loadRanking()
{
    FILE *fp = fopen("ranking", "r"); //리드모드로 ranking파일 불러옴
    char buffer[50];                  //버퍼를 위한 초기화
    int i = 0;                        //순위
    int map = 0;                      // 몇번째 맵인지를 위한 변수
    while (true)
    {
        fscanf(fp, "%s", &buffer); //read line
        if (buffer[0] == '#')
        {
            //buffer[1] will be the map number
            map = buffer[1] - ONE;
        }
        else if (buffer[0] == 'e') //버퍼가 e면 끝
        {
            break;
        }
        else
        {
            int score = 0;                            //스코어를 담기위한 변수
            fscanf(fp, "%d", &score);                 // 파일에서 스코어를 불러와 score에 담음
            rankingMove[map][i] = score;              //움직인 랭킹의 map번째의 랭킹 i에 score대입
            strncpy(rankingName[map][i], buffer, 10); //버퍼에 담긴 이름을 rankingName의 map번째의 랭킹 i에 대입
            i++;
        }
    }
    fclose(fp);
}
void updateRanking() //랭킹 업데이트를 위한 함수
{
    //Loop current stage ranking and change it
    int i, j = 0;
    for (i = 0; i < 5; i++)
    {
        //checking from top (lowest)
        //if lowest ranking was 0, then there is
        if (rankingMove[currentRound][i] > numMove || rankingMove[currentRound][i] == 0)
        {
            for (j = 4; j > i + 1; j--)
            {
                rankingMove[currentRound][j] = rankingMove[currentRound][j - 1];        //현재 라운드(현재 맵)의 j-1 번쨰 랭킹을 하나 아래로 내린다
                strcpy(rankingName[currentRound][j], rankingName[currentRound][j - 1]); //이름저장 배열도 하나 아래로 내린다
            }
            strcpy(rankingName[currentRound][i], username); // 현재 유저의 이름을 라운드의 랭킹i번제에 저장
            rankingMove[currentRound][i] = numMove;         // 현재 유저의 움직인 횟수를 라운드의 랭킹 i번째에 저장
            break;
        } //compare move count
    }
    saveRanking(); //랭킹저장
}
void saveRanking() //래킹저장을 위한 함수
{
    FILE *fp = fopen("ranking", "w"); //쓰기 모드로 ranking파일 불러온다
    for (int i = 0; i < 5; i++)       //맵은 5개이니 0부터 4까지 돌림
    {
        fprintf(fp, "#%d\n", i + 1); //i+1번쨰 맵이라고 파일에 쓴다
        for (int j = 0; j < 5; j++)  // 랭킹도 5개 0부터 5까지 돌림
        {
            if (strlen(rankingName[i][j]) > 0) // i번째맵의 j번쨰 랭킹의 이름이 존재한다면 (이름의 길이가 0보다 길면 존재)
            {
                fprintf(fp, "%s %d\n", rankingName[i][j], rankingMove[i][j]); //파일에 이름과 움직인 횟수를 저장한다
            }
        }
    }
    fprintf(fp, "e"); //마지막위치라고 파일에 쓴다 (파일의 마지막줄을 구별하기위해 e를 넣었음)
    fclose(fp);       //파일을 닫는다
}
void displayRanking(int map) //랭킹을 표시하는 부분
{
    cls();        //화면을 초기화한다
    if (map == 0) // 0이면 전체맵의 랭킹을 출력
    {
        for (int j = 0; j < 5; j++)
        {                                                   //5개의 맵 0부터 5보다 작을때까지 for문을 돌림
            printf("map %d\n", j + 1);                      // j+1번쨰 맵이라고 출력
            for (int i = 0; strlen(rankingName[j][i]); i++) // j번쨰 맵의 랭킹i가 있을때동안만 돌린다 (rnaking[j][i]의 길이가 0이면 존재하지않음) f
            {
                printf("%s\t%d\n", rankingName[j][i], rankingMove[j][i]); // 이름과 움직인 횟수 출력
            }
        }
    }
    else
    {
        getchar();
        printf("map %d\n", map);                              //map번째 map인지 출력
        for (int i = 0; strlen(rankingName[map - 1][i]); i++) // i번쨰 맵의 랭킹j가 있을때동안만 돌린다 (rnaking[j][i]의 길이가 0이면 존재하지않음) f
        {
            printf("%s\t%d\n", rankingName[map - 1][i], rankingMove[map - 1][i]); // 이름과 움직인 횟수 출력 한다
        }
    }
    while (1)
    {                       // 출력후 대기
        char c = getchar(); //입력을 받으면서
        if (c == '\n')
        {                   // \n이면
            cls();          //화면을 초기화하고
            drawStage();    //스테이지를 그리고
            inputCommand(); //커멘드를 입력받는다
            return;         //랭킹 출력을 마친다
        }
    }
}
void cls() // 화면 초기화를 위한 함수
{
    system("clear"); // system의 clear를 이용하여 화면을 초기화
}
void clearData() //데이터 초기화
{
    for (int t = 0; t < 5; t++)
    {                                //라운드 t
        for (int i = 0; i < 30; i++) //i의
        {
            for (int j = 0; j < 30; j++)
            {                              //j번쨰 입력된 캐시를
                inputCache[t][i][j] = ' '; //' '으로 초기화
            }
        }
    }
}
void saveStatus() //상항저장을 위한 함수
{
    FILE *fp = fopen("sokoban", "w");                                         // w모드로 sokoban파일을 열음
    fprintf(fp, "%d\n%d\n%d\n%d\n", numMove, playerX, playerY, currentRound); //움직인 횟수, 플레이어위치, 현재라운드를 저장
    for (int y = 0; y < 30; y++)                                              // 스테이지에 저장된값을 파일에 저장하기위해 돌린다
    {
        if (strlen(stage[y]) < 1)                          //stage의 y번재길이가 1보다 작으면
            fprintf(fp, "                            \n"); //                            \n을저장
        else
            fprintf(fp, "%s\n", stage[y]); // y번째 정보를 저장
    }
    fclose(fp); //파일을 닫는다
}
void loadStatus() //스테이터를 불러오기위한 함수
{
    FILE *fp = fopen("sokoban", "r"); //파일 읽어들임
    char buffer[30] = {
        0,
    };                               //버퍼를 위한 char배열
    int mapNum = 0;                  //몇번째맵인지 저장하는 변수
    int i = 0;                       //for문을 위한 변수
    clearData();                     //데이터 초기화
    fscanf(fp, "%d", &numMove);      //움직인 횟수 불러와 움직인 횟수에 저장
    fscanf(fp, "%d", &playerX);      //유저의 x좌표를 불러와 playerX에 저장
    fscanf(fp, "%d", &playerY);      //유저의 y좌표를 불러와 playerY에 저장
    fscanf(fp, "%d", &currentRound); //라운드를 불러와 currentRound에 저장
    fgetc(fp);                       // 파일에서 문자를 읽어들인다
    cls();                           //화면 청소 아이깨끗해 ^^
    for (i = 0; i < 30; i++)         // 쭈욱 좌표를 돈다
    {
        fgets(buffer, 30, fp);                 //파일에문자를 30만큼읽어와 버퍼에 담는다
        strcpy(stage[i], buffer);              //버퍼를 i번쨰 스테이지에 저장
        stage[i][strlen(stage[i]) - 1] = '\0'; //제일끝에 \0으로 초기화
    }
    fclose(fp);     //파일을 닫는다
    drawStage();    //스테이지를 그린다
    inputCommand(); //커멘드 입력
}
void replayStage() //리플레이를 위한 함수
{
    initStage(); //스테이지를 초기화
}
void newStage() //새로운 스테이지로 넘어갈때를 위한 함수
{
    numMove = 0;      //움직인횟수를 0으로 초기화
    currentRound = 0; //현재라운드를 0으로 초기화
    initStage();      //스테이지 초기화
}
void exitGame() //게임 종료
{
    cls();                                  // 화면 초기화
    printf("S E E\tY O U\t%s\n", username); //username에게 작별인사를 한다
    exit(0);                                //프로그램 종료
}
void drawHelp() //도움말 출력
{
    cls();                                       //화면초기화후
    puts("SOKOBAN");                             //SOKOBAN출력
    puts("e: 종료, r: 다시시작");                // e와 r
    puts("n: 다음, d: 도움말");                  //n과 d가 뭔지 알려준다
    printf("스테이지 : %d\n", currentRound + 1); //현재 스테이지를 출력해준다
    printf("이동 횟수 : %d\n", numMove);         //현재 움직인 횟수를 출력해준다
    while (1)
    {                       //출력후 대기
        char c = getchar(); //입력을 위한 임시변수
        if (c == '\n')
        {                   //\n이 입력되면
            cls();          //화면을 초기화
            drawStage();    //스테이지를 그린다
            inputCommand(); //커맨드입력
            return;         //도움말 종료
        }
    }
}
void recordUndo(char ch) // 상태저장을 위한 함수
{
    if (undoIndex < 4)
    {                //undo인덱스가 4보다 작으면
        undoIndex++; //undo인덱스증가
    }
    else
    { //undoindex가 4보다크면
        for (int i = 0; i < 4; i++)
        {                                   //스테이지 1부터 5까지 돌면서
            for (int cy = 0; cy < 30; cy++) //y좌표
            {
                for (int cx = 0; cx < 30; cx++) //x좌표를 확인
                {
                    inputCache[i][cy][cx] = inputCache[i + 1][cy][cx]; //저장된 스테이지를 하나 앞댕긴다
                }
            }
            playerPos[i][0] = playerPos[i + 1][0]; //플레이어위치 앞으로 댕긴다
            playerPos[i][1] = playerPos[i + 1][1]; //플레이어위치 앞으로 댕긴다
        }
    }
    for (int cy = 0; cy < 30; cy++) //y좌표
    {
        for (int cx = 0; cx < 30; cx++) //x좌표
        {
            inputCache[undoIndex][cy][cx] = stage[cy][cx]; //현제 위치를 cache에 저장
        }
    }
    playerPos[undoIndex][0] = playerX; //플레이어 x 좌표를 저장
    playerPos[undoIndex][1] = playerY; //플레이어 y 좌표 저장
}
void undoMovement() //움직임 취소를 위한 함수
{
    if (undoIndex <= 0)
        return; //undo인덱스가 0보다 작으면 함수종료
    if (undoCount++ > 4)
        return;                     //undo횟수가 4보다 크면 함수 종료
    for (int cy = 0; cy < 30; cy++) //y좌표
    {
        for (int cx = 0; cx < 30; cx++) //x좌표
        {
            stage[cy][cx] = inputCache[undoIndex][cy][cx]; //저장된 y좌표 x좌표를 현제 스테이지에 대입
        }
    }
    playerX = playerPos[undoIndex][0]; //유저의 x좌표를 저장된 값으로 지정
    playerY = playerPos[undoIndex][1]; //유저의 y좌표를 저장된 값으려 지정
    undoIndex--;                       //undoindex를 하나 감소시킨다
    numMove++;                         //이동횟수 증가
    drawStage();                       //스테이지를 그린다
    inputCommand();                    // 커멘드입력
}
_Bool checkValidMap(int index) //올바른 맵인지 체크
{
    int cntBox, cntHole = 0;     //박스와 홀을 체크한다
    for (int y = 0; y < 30; y++) //y좌표
    {
        for (int x = 0; x < 30; x++) //x좌표
        {
            if (StageData[index][y][x] == 'O') //스테이지 y의 x에 홀이있으면
            {
                cntHole++; //구멍하나증가
            }
            else if (StageData[index][y][x] == '$') //스테이지 y의 x에 박스가있으면
            {
                cntBox++; //박스증가
            }
        }
    }
    if (cntHole == cntBox) //둘이 갯수가 똑같으면
        return true;       //올바른 맵
    else
        return false; //올바르지않은 맵
}
void loadMap()
{
    char buffer[30] = {
        '#',
    };                            //Map size is 30 or less, init array to 0
    FILE *fp = fopen("map", "r"); //Open file pointer
    int line = 0;                 //count line index
    int mapIndex = 0;             //count map index
    while (true)                  //read file until EOF
    {
        fscanf(fp, "%s", &buffer);
        //check map ends : return;
        if (buffer[0] == 'e')
        {
            break;
        }
        //check next map
        else if (buffer[0] >= ONE && buffer[0] <= FIVE)
        {
            if (!checkValidMap(mapIndex)) //if not valid
            {
                cls();                                                                     //화면 클리어
                printf("%d 번 맵에서 오류가 발견되었습니다. 종료합니다.\n", mapIndex + 1); //에러출력
                exit(-1);                                                                  //프로그램 종료
            }
            mapIndex = buffer[0] - 49; //맵인덱스 변경
            line = 0;                  // 라인 초가화
        }
        else
        {
            strncpy(StageData[mapIndex][line], buffer, 30); //버퍼에담긴 데이터를 stagedata로 복사
            line += 1;
        }
        //get stage
    }
    fclose(fp); //파일 닫기
}
void drawStage()
{
    cls();                          //화면을 초기화한다
    int stageY;                     // 스테이지를 위한 변수
    printf("Hello %s\n", username); //저장된 유저이름을 출력하며 반긴다
    printf("Move : %d\n", numMove); //저장된 유저의 움직인 횟수를 출력

    for (stageY = 0; stageY < 30; stageY++)
    {
        if (strlen(stage[stageY]) > 0)     //stage의 stageY번째의 길이가 0보다 크면
            printf("%s\n", stage[stageY]); //스테이지 출력
    }
}
void inputCommand() //커멘드 입력용 함수
{
    char ch;             //입력을 위한 변수
    int rankingPage = 0; //랭킹페이지를 위한 변수
    ch = getch();        // 변수를 입력받음
    if (ch == 't')
    {                          //ch가 t일떄
        printf("t ");          //t 출력
        ch = getchar();        //한번더 입력
        if (ch == '\n')        //
            displayRanking(0); //모든 랭킹을 풀력
        else if (ch - ONE >= 0 && ch - ONE <= 4)
        {                           //입력값을 ascii 1로 뺴서 숫자 0과 4사이이면
            ch -= ONE;              //입력값을 숫자로 변경
            displayRanking(ch + 1); // 해당 스테이지의 랭킹을 출력
        }
    }
    else
    {
        switch (ch)
        {
        case 'u':           //u일때
            undoMovement(); //움직인걸 취소한다
            break;
        case 'r':          //r일때
            replayStage(); //스테이지를 다시 시작한다
            break;
        case 'n':
            newStage(); //새로운 스테이지로 간다
            break;
        case 'e':
            exitGame(); //게임 종료
            break;
        case 's':
            saveStatus(); //현재 스테이터스를 저장
            break;
        case 'f':
            loadStatus(); //스테이터스를 불러온다
            break;
        case 'd':
            drawHelp(); //도움말을 불러온다
            break;
        case 'h':
        case 'j':
        case 'k':
        case 'l':
            movePlayer(ch); //입력된 방향으로 이동
            break;
        default: //그외는 돌아가
            break;
        }
    }
}
void movePlayer(char ch) //움직임을 위한 함수
{
    int deltaX, deltaY = 0; //이동되는 방향을 위한 변수
    switch (ch)             //입력된값확인
    {
    case 'h':        // h일때
        deltaX = -1; //x 값을 -1
        deltaY = 0;  //y는 그대로
        break;
    case 'j':
        deltaX = 0; //x는 그대로
        deltaY = 1; // y는 1
        break;
    case 'k':
        deltaX = 0;  //x는 그대로
        deltaY = -1; //y는 -1
        break;
    case 'l':
        deltaX = 1; //x는  1
        deltaY = 0; //y는 그대로
        break;
    }
    if (stage[playerY + deltaY][playerX + deltaX] == '#') //스테이지의 유저y좌표를 deltaY만큼 & 유저 x좌표를 deltaX만큼 이동 거기가 #이면 함수종료
        return;
    else if (stage[playerY + deltaY][playerX + deltaX] == '$') //
    {
        //check two step forward
        switch (stage[playerY + (deltaY * 2)][playerX + (deltaX * 2)])
        {
        case '#':   //#이거나
        case '$':   //$이면
            return; //함수 종료
        }
    }
    recordUndo(ch);                                       //현재상태를저장
    if (stage[playerY + deltaY][playerX + deltaX] == '$') //플레이어의 이동위치가 $ 이면
    {
        switch (stage[playerY + (deltaY * 2)][playerX + (deltaX * 2)])
        {                                                            // 주변 확인
        case '.':                                                    //.이거나
        case 'O':                                                    //O이면
            stage[playerY + deltaY][playerX + deltaX] = '.';         //스테이지에 .등록
            stage[playerY + deltaY * 2][playerX + deltaX * 2] = '$'; //유저위치의 주변에 $등록
            break;
        }
    }
    if (StageData[currentRound][playerY][playerX] != 'O') //스테이지 정보에 유저 위치가 O이아니면
        stage[playerY][playerX] = '.';                    //스테이지에 .등록
    else
    {
        stage[playerY][playerX] = 'O'; //O이라면 O으로
    }

    playerX += deltaX; //델타 x만큼 이동
    playerY += deltaY; //델타 y만큼 이동

    stage[playerY][playerX] = '@'; //playerY의 playerX에 유저등록
    //if player collided to a wall, then don't refresh.
    numMove++; //이동횟수 증가

    drawStage(); //스테이지그림
}
_Bool isStageClear() //Stage 초기화 여부 확인
{
    for (int cy = 0; cy < 30; cy++)
    {
        for (int cx = 0; cx < 30; cx++)
        {
            if (StageData[currentRound][cy][cx] == 'O' && stage[cy][cx] != '$') //이번라운드의 y좌표와 x좌표가 0이면서 스테이지의 y x 좌표가 $라면 false
            {
                return false;
            }
        }
    }
    return true;
}
void initStage() //스테이지 초기화
{
    for (int cy = 0; cy < 30; cy++) // 0부터 30번쨰까지
    {
        for (int cx = 0; cx < 30; cx++)
        {
            stage[cy][cx] = StageData[currentRound][cy][cx]; //스테이지 데이터의 현재라운드 cy의 cx번쨰의 데이터를 stage에 저장한다
        }
    }
    findPlayerLocation(); //유저의 로케이션을 찾는다
    drawStage();          //스테이지를 그린다
}
void findPlayerLocation() //유저의 위치를 찾기 위한 함수
{
    for (int y = 0; y < 30; y++) //y
    {
        for (int x = 0; x < 30; x++)
        {
            if (StageData[currentRound][y][x] == '@') //현재라운드의 스테이지에 y좌표 x좌표에 유저*@)가있으면
            {
                playerX = x; // playerX를 x로 대입
                playerY = y; // playerY를 y로 대입
                //현재위치를 각변수에 대입해준다
            }
        }
    }
}
int main(void)
{
    printf("Start....\nInput name : ");
    scanf("%s", username); //제일 먼저 username을 받음
    loadMap();             //맵로딩하는 부분
    loadRanking();         //랭킹 로딩
    initStage();           //스테이지 초기화
    while (true)
    {
        inputCommand();     //커멘드 입력
        if (isStageClear()) //클르어 되었는지 확인
        {
            //if Stage cleared, then go to next map / or end game
            cls();
            //remove all undo data
            //clearData();
            updateRanking(); // 랭킹 업데이트
            currentRound++;  //라운드 추가
            if (currentRound >= MAXSIZE)
                exitGame(); //5판이상이되면 게임종료
            undoCount = 0;  //undo카운트를 0으로
            numMove = 0;    //이동횟수를 0으로
            initStage();    //스테이지 초기화
            continue;
        }
    }
    return 0;
}