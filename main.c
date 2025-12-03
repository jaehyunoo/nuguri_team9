#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h> //setlocale 및 LC_ALL 사용을 위해// 글자 깨짐 방지 <새로 추가한 헤더>

#ifdef _WIN32 //윈도우전용 헤더 <수정된 부분>
#include <windows.h>  
#include <conio.h>  

#else// 리눅스 전용 헤더
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif 

// 맵 및 게임 요소 정의 (수정된 부분)
#define MAX_STAGES 2
#define MAX_ENEMIES 15 // 최대 적 개수 증가
#define MAX_COINS 30   // 최대 코인 개수 증가

// 구조체 정의
typedef struct {
    int x, y;
    int dir; // 1: right, -1: left
} Enemy;

typedef struct {
    int x, y;
    int collected;
} Coin;

// 전역 변수
//char map[MAX_STAGES][MAP_HEIGHT][MAP_WIDTH + 1];
int player_x, player_y;
int stage = 0;
int score = 0;

char ***map = NULL;         // map[stage][y][x] -> 맵포인터
int mapWidth[MAX_STAGES];   // 각 stage의 가로 길이
int mapHeight[MAX_STAGES];  // 각 stage의 세로 길이
int stageCount = 0; 

// 플레이어 상태
int is_jumping = 0;
int velocity_y = 0;
int on_ladder = 0;
int user_Heart = 3;

// 게임 객체
Enemy enemies[MAX_ENEMIES];
int enemy_count = 0;
Coin coins[MAX_COINS];
int coin_count = 0;

// 터미널 설정
//disable_raw_mode()함수에서 콘솔상태 저장 변수 분기 <수정된 부분>
#ifdef _WIN32
static DWORD original_mode;
#else
struct termios orig_termios;
#endif

// 함수 선언
void disable_raw_mode();
void enable_raw_mode();
void loadMap();
void init_stage();
void draw_game();
void update_game(char input); 
void move_player(char input);
void move_enemies();
void check_collisions();
int game_overscr();
int game_clear1();
int game_clear2();
int kbhit();
void getCoin(int player_x, int player_y);

int opening(); //수정됨 게임 시작시 화면 띄우기
void clrscr(); //수정됨 화면 지우고 (1,1)로 커서 이동
void gotoxy(int x, int y); // 수정됨 화면 그대로 (x,y)로 이동
void beepsound(int sel);
void freeMap();

//delay함수 윈도우,리눅스용 분기 나눔<새로 추가한 함수>
void delay(int ms) {
#ifdef _WIN32
  Sleep(ms);      // Windows: 밀리초 단위
#else
usleep(ms * 1000); // Linux/macOS: 마이크로초 단위 1밀리초 = 1000 마이크로초
#endif
}

int first=1;

int main() {

    //한국어 로케일 설정, c언어 에서 한글 입출력을 정상적으로 처리하기 위해서 <수정된 부분>
    setlocale(LC_ALL, "ko_KR.UTF-8");

    //윈도우에서 글자 깨짐 방지 <수정된 부분 >
    #ifdef _WIN32
    system("chcp 65001");
    #endif

    loadMap();
    srand(time(NULL));

    while (1) {
    stage = 0;
    score = 0;
    user_Heart = 3;
    int game_over = 0;

    enable_raw_mode();//opening()실행되기전 실행

    if(first==1)
    {
        first=0;
        if (!opening()) {    //처음 오프닝때 p를 제외하고 누를경우 exit(0)가 바로호출되어 오류가생김(disable_raw_mode()실행못하고 맵을 freemap못함
            disable_raw_mode();
            freeMap();
            return 0;
        }
    }
    clrscr();// 초기화면 클리어 <수정된 부분>, 리눅스는 이상없으나 윈도우 더블버퍼링이 변경사항만 수정하도록 되어있어 초기화면 잔상이 남아 직접 클리어함 

    init_stage();

    while (!game_over && stage < stageCount) {

        char c = '\0';

         // 윈도우 리눅스 키보드 입력 분기<수정된 부분>
    #ifdef _WIN32
        while (_kbhit()) {  
            int chr = _getch();
            if (chr == 'q') {
                disable_raw_mode();
                freeMap(); 
                return 0;
            }   
            if (chr == 0 || chr == 224) { // 방향키 처리 
                chr = _getch();
                switch (chr) { 
                case 72: c = 'w'; break; // Up
                case 80: c = 's'; break; // Down
                case 75: c = 'a'; break; // Left
                case 77: c = 'd'; break; // Right
            }
        } 
        else {  
        c = chr;
        }
    }
    #else
        while (kbhit()) {//kbhit를 while에 넣어 한프레임당 키들이 즉각반응하고 남은키는 버려질수있도록 구현
            int chr = getchar();
            if (chr == 'q') {
                disable_raw_mode();
                freeMap(); 
                return 0;
            }
            if (chr == '\x1b') {
                getchar(); // '['
                switch (getchar()) {
                    case 'A': c = 'w'; break; // Up
                    case 'B': c = 's'; break; // Down
                    case 'C': c = 'd'; break; // Right
                    case 'D': c = 'a'; break; // Left
                }
            }
            else {
            c = chr;
            }
        }
        #endif 

        update_game(c);
        draw_game();
        delay(90);


        if (user_Heart == 0)
        {
            int re = game_overscr();
            disable_raw_mode();
            if (!re) {
                freeMap();
                return 0;
            }
            game_over = 1; 
        }
        
        if (map[stage][player_y][player_x] == 'E') {
            // stage++;
            score += 100;
            /*
        if (map[stage][player_y][player_x] == 'E') {
            stage++;
            score += 100;
            if (stage < MAX_STAGES) {
                init_stage();
            } else {
                game_over = 1;
                game_clear();
            }
            */
            if (stage + 1 < stageCount) {
                stage++;
                init_stage();
                int re = game_clear1(); // 첫 스테이지 클리어 메시지

                if (!re)
                {
                    disable_raw_mode();
                    freeMap();
                    return 0;
                }
            } else {
                int re = game_clear2();
                disable_raw_mode();
                if (!re) {
                    freeMap();
                    return 0;
                }
                game_over=1;
            }
            clrscr();// 초기화면 클리어 <수정된 부분>, 리눅스는 이상없으나 윈도우 더블버퍼링이 변경사항만 수정하도록 되어있어 초기화면 잔상이 남아 직접 클리어함 
        }
    }
    disable_raw_mode();//재시작할때마다 disable_raw_mode실행
  }
}

int opening(){
    clrscr();
    while(1){
        int ch=0;
        printf("\n\n\n\n\n");
        printf("\n           ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        printf("\n           ┃                                          ┃");
        printf("\n           ┃                                          ┃");
        printf("\n           ┃                NUGURI GAME               ┃");
        printf("\n           ┃                                          ┃");
        printf("\n           ┃                                          ┃");
        printf("\n           ┃              Press P to play             ┃");
        printf("\n           ┃              other keys to quit          ┃");
        printf("\n           ┃                                          ┃");
        printf("\n           ┃                                          ┃");
        printf("\n           ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        gotoxy(30, 15);
        ch = getchar();
        if (ch == 'P' || ch == 'p')
            return 1;
        else 
            return 0;
    }
}

void clrscr(){
    printf("\x1b[2J\x1b[H");
    fflush(stdout);
}

void gotoxy(int x, int y){
    printf("\x1b[%d;%dH", y, x);
    fflush(stdout);
}

void beepsound(int sel) {
#ifdef _WIN32
    // Windows: Beep API
    switch (sel) {
    case 1: Beep(700, 120); break;  // HP 감소
    case 2: Beep(1000, 120); break; // 점프
    case 3: Beep(1300, 120); break; // 코인
    }
#elif defined(__APPLE__)
    // macOS: afplay로 시스템 사운드 재생
    const char *sd = NULL;
    switch (sel) {
    case 1: sd = "/System/Library/Sounds/Basso.aiff"; break;
    case 2: sd = "/System/Library/Sounds/Pop.aiff";   break;
    case 3: sd = "/System/Library/Sounds/Glass.aiff"; break;
    }
    if (sd) {
        char c[256];
        snprintf(c, sizeof(c),
                 "afplay \"%s\" >/dev/null 2>&1 &",
                 sd);
        system(c);
    }
#else
    // Linux 등: 터미널 벨
    int repeat = 1;
    switch (sel) {
    case 1: repeat = 3; break;
    case 2: repeat = 1; break;
    case 3: repeat = 2; break;
    }
    for (int i = 0; i < repeat; i++) {
        fputc('\a', stdout);
        fflush(stdout);
        delay(40);
    }
#endif
}
/*
윈도우 버전 window.h 필요
void beepsound(int sel){
    switch(sel){
        case 1:
        Beep(900, 150);
        delay(30);
        Beep(700, 150);
        delay(30);
        Beep(500, 150);
        delay(30);
        Beep(350, 200);
        delay(30);
        break;

        case 2:
        Beep(900, 200);
        delay(30);
        Beep(1200, 200);
        break;

        case 3:
        Beep(1300, 150);
        delay(30);
        Beep(1600, 150);
        delay(150);
        Beep(2000, 200);
        delay(30);
        break;
}

*/



// 터미널 Raw 모드 활성화/비활성화
//disable_raw_mode()함수 분기 <수정된 부분>
void disable_raw_mode() { 

    #ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hStdin, original_mode);
    #else

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    #endif
 }
 //enable_raw_mode()함수 분기 <수정된 부분>
void enable_raw_mode() {

    #ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &original_mode);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(hStdin, mode);

    #else
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    #endif
}

void analyzeMap(FILE *file) {
    char line[1024];
    int s = 0;         // 현재 스테이지 인덱스
    int curHeight = 0; 
    int curWidth  = 0;

        //스테이지 별 widh와 height받아오기전 초기화
    for (int i = 0; i < MAX_STAGES; i++) {
        mapWidth[i] = 0;
        mapHeight[i] = 0;
    }
    stageCount = 0;

    while (fgets(line, sizeof(line), file)) {//1줄씩 입력받으면서 구조확인
        int blank = (line[0] == '\n' || line[0] == '\r' || line[0] == '\0');

        if (blank) {//공백을 기준으로 스테이지를 구별한다.
            if (curHeight > 0) {//공백이면 스테이지 너비,높이를 기록한다.
                mapWidth[s]  = curWidth;
                mapHeight[s] = curHeight;
                s++;

                if (s >= MAX_STAGES) {
                    fprintf(stderr,
                        "스테이지 개수가 MAX_STAGES(%d)를 초과했습니다.\n",
                        MAX_STAGES);
                    exit(1);
                }

                curHeight = 0;
                curWidth  = 0;
            }
            continue;
        }

        int len = (int)strcspn(line, "\n\r");  //문자로만된 길이를 받아오기위해서
        if (len > curWidth) curWidth = len;

        curHeight++;
    }

    if (curHeight > 0) {
        mapWidth[s]  = curWidth;
        mapHeight[s] = curHeight;
        s++;
    }

    if (s == 0) {
        fprintf(stderr, "map.txt 에 유효한 맵 데이터가 없습니다.\n");
        exit(1);
    }

    stageCount = s;
}

void allocateMap(void) {

    map = (char ***)malloc(stageCount * sizeof(char **));//가장 중요한 map동적할당
    if (!map) {
        perror("map malloc 실패");
        exit(1);
    }

    //스테이지 수만큼 동적할당된 메모리에 접근하여 스테이지수, 높이수만큼 배열공간을 확보한다.
    for (int s = 0; s < stageCount; s++) {
        map[s] = (char **)malloc(mapHeight[s] * sizeof(char *));
        if (!map[s]) {
            perror("map[s]쪽 malloc 실패");
            // 이전에 할당한 이전 스테이지 메모리 해제
            for (int i = 0; i < s; i++) {
                for (int y = 0; y < mapHeight[i]; y++) {
                    free(map[i][y]);
                }
                free(map[i]);
            }
            free(map);
            exit(1);
        }

        for (int y = 0; y < mapHeight[s]; y++) {
            map[s][y] = (char *)malloc(mapWidth[s] + 1);
            if (!map[s][y]) {
                perror("map[s][y]쪽 malloc 실패");
                // 이전에 할당한 행 해제
                for (int yy = 0; yy < y; yy++) {
                    free(map[s][yy]);
                }
                // 이전 스테이지 전체 메모리 해제
                for (int i = 0; i < s; i++) {
                    for (int yy = 0; yy < mapHeight[i]; yy++) {
                        free(map[i][yy]);
                    }
                    free(map[i]);
                }
                free(map);
                exit(1);
            }

            //공백으로 채우면서 마지막에 널삽입
            memset(map[s][y], ' ', mapWidth[s]);
            map[s][y][mapWidth[s]] = '\0';
        }
    }
}

void fillMap(FILE *file) {
    char line[1024];
    int s = 0;  // 현재 스테이지 변수
    int r = 0;  // 현재 스테이지에서의 줄 인덱스

    rewind(file);  // 다시 파일처음부터 확인하기위해서(파일포인터를 다시 처음으로)

    while (s < stageCount && fgets(line, sizeof(line), file)) {//analyzeMap함수떄 처럼 텍스트읽어오기
        int blank = (line[0] == '\n' || line[0] == '\r' || line[0] == '\0');

        if (blank) {
            if (r > 0) {
                s++;
                r = 0;
            }
            continue;
        }

        int len = (int)strcspn(line, "\n\r");
        if (len > mapWidth[s]) len = mapWidth[s];

        memcpy(map[s][r], line, len); 
        r++;
    }
}

void freeMap(void) {
    for (int s = 0; s < stageCount; s++) {
        for (int y = 0; y < mapHeight[s]; y++) {
            free(map[s][y]);
        }
        free(map[s]);
    }
        free(map);  
        map = NULL;
}



// 맵 파일 로드
void loadMap(void) {
    FILE *file = fopen("map.txt", "r");
    if (!file) {
        perror("map.txt 파일을 열 수 없습니다.");
        exit(1);
    }

    analyzeMap(file);
    allocateMap();
    fillMap(file);


    fclose(file);
}


// 현재 스테이지 초기화
void init_stage() {
    enemy_count = 0;
    coin_count = 0;
    is_jumping = 0;
    velocity_y = 0;

    for (int y = 0; y < mapHeight[stage]; y++) {
        for (int x = 0; x < mapWidth[stage]; x++) {
            char cell = map[stage][y][x];
            if (cell == 'S') {
                player_x = x;
                player_y = y;
            } else if (cell == 'X' && enemy_count < MAX_ENEMIES) {
                enemies[enemy_count] = (Enemy){x, y, (rand() % 2) * 2 - 1};
                enemy_count++;
            } else if (cell == 'C' && coin_count < MAX_COINS) {
                coins[coin_count++] = (Coin){x, y, 0};
            }
        }
    }
}

//게임 화면 그리기
void draw_game(void) {

    int W = mapWidth[stage];   // 현재 스테이지 가로 길이
    int H = mapHeight[stage];  // 현재 스테이지 세로 길이


    static char prev_buffer[4096] = {0};
    char buffer[4096] = {0};
    int cursor_location = 0;

    printf("\x1b[?25l");

#ifdef _WIN32
    printf("\x1b[H");          
#else
    printf("\x1b[2J\x1b[H");   
#endif

   
    cursor_location += sprintf(buffer + cursor_location,
                               "Stage: %d | Score: %d Heart: %d\n",
                               stage + 1, score, user_Heart);
    cursor_location += sprintf(buffer + cursor_location,
                               "조작: ← → (이동), ↑ ↓ (사다리), Space (점프), q (종료)\n");

 
    char **display_map = (char **)malloc(H * sizeof(char *));
    if (!display_map) {
        perror("malloc 실패");
        exit(1);
    }

    for (int y = 0; y < H; y++) {
        display_map[y] = (char *)malloc(W + 1);  
        if (!display_map[y]) {
            perror("malloc 실패");
            exit(1);
        }

        for (int x = 0; x < W; x++) {
            char cell = map[stage][y][x];
            display_map[y][x] =
                (cell == 'S' || cell == 'X' || cell == 'C') ? ' ' : cell;
        }
        display_map[y][W] = '\0'; 
    }


    for (int i = 0; i < coin_count; i++) {
        if(!coins[i].collected) {
            display_map[coins[i].y][coins[i].x] = 'C';
        }
    }


    for (int i = 0; i < enemy_count; i++) {
        display_map[enemies[i].y][enemies[i].x] = 'X';
    }


        display_map[player_y][player_x] = 'P';
    

    for (int y = 0; y < H; y++) {
        if (cursor_location + W + 1 >= (int)sizeof(buffer)) {
            break;
        }
        memcpy(buffer + cursor_location, display_map[y], W);
        cursor_location += W;
        buffer[cursor_location++] = '\n';
    }

    buffer[cursor_location] = '\0';

    if (strcmp(prev_buffer, buffer) != 0) {
#ifdef _WIN32
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE),
                      buffer, cursor_location, NULL, NULL);
#else
        printf("%s", buffer);
#endif
        strcpy(prev_buffer, buffer);
    }

    for (int y = 0; y < H; y++) {
        free(display_map[y]);
    }
    free(display_map);
}

// 게임 상태 업데이트
void update_game(char input) {
    move_player(input);
    move_enemies();
    check_collisions();
}

void getCoin(int player_x, int player_y) {//점프하는 도중에도 코인을 얻는 경우가 있기 때문에 해당기능을 함수로 만들어 재사용성 높임

        for (int i = 0; i < coin_count; i++) {
        if (!coins[i].collected && player_x == coins[i].x && player_y == coins[i].y) {
            coins[i].collected = 1;
            score += 20;
            beepsound(3);
        }
    }
}


int jumpLadderTop = 0;   //기존의 오류로 인해 사다리에 있기만 하면 위에 벽을통과 그래서 사다리 가장 위에 있을때만 점프가능

// 플레이어 이동 로직
void move_player(char input) {
    int next_x = player_x, next_y = player_y;
    char current_tile = map[stage][player_y][player_x];

    on_ladder = (current_tile == 'H');

    switch (input) {
        case 'a': next_x--; break;
        case 'd': next_x++; break;
        case 'w': if (on_ladder) next_y--; break;
        case 's': if (on_ladder && (player_y + 1 < mapHeight[stage]) && map[stage][player_y + 1][player_x] != '#') next_y++; break;
    }

    if (next_x >= 0 && next_x < mapWidth[stage]&& map[stage][player_y][next_x] != '#') player_x = next_x;

    char floor_tile = (player_y + 1 < mapHeight[stage]) ? map[stage][player_y + 1][player_x] : '#';//이전의 player_x값을 이용해서 floor을 결정하다보니깐 
                                                                                            //한타이밍 늦게 바닥#을 확인해 벽을 뚫어버리는 오류가 발생함 그래서 밑에 floor_title을 초기화시켜준다.

    
    if (input == ' ') {//점프하기위해서 ' '가 입력된 경우 새로 갱신된 floor_title을 활용
        if (!is_jumping && (floor_tile == '#' || floor_tile == 'H' || on_ladder)) {
            is_jumping = 1;
            beepsound(2);
            velocity_y = -2;

            if (map[stage][player_y][player_x] == 'H' && //사다리 끝에 있을때만 오를수있도록 jumpLadderTop전역변수 선언
                player_y > 0 &&
                map[stage][player_y - 1][player_x] == '#') {
                jumpLadderTop = 1;
            } else {
                jumpLadderTop= 0;
            }
        }
    }
    

    if (on_ladder && (input == 'w' || input == 's')) {
        if(next_y >= 0 && next_y < mapHeight[stage] && map[stage][next_y][player_x] != '#') {
            player_y = next_y;
            is_jumping = 0;
            velocity_y = 0;
        }
    } 
    else {
    if (is_jumping) {//점프했을때 만약 위에 #이있을경우 부딫히고 아래로 내려갈떄 #이 통과되는 오류를 해결 velocity_y>0때 조건문으로
                        //비교할수있게 수정함.

        next_y = player_y + velocity_y;
        if (next_y < 0) next_y = 0;

    

        if (velocity_y < 0) {
            int y_from = player_y - 1; 
            int y_to   = next_y;         
            if (y_to < 0) y_to = 0;

            for (int y = y_from; y >= y_to; y--) {
                char tile = map[stage][y][player_x];

                getCoin(player_x,y); 

                if (tile == 'X') {
                    user_Heart--;
                    beepsound(1);
                    init_stage();
                    return;  
                }

       
                if (tile == '#') {
                        //  사다리 맨끝에서 점프했을 때 바로 위에 #이 있는경우만 사다리를 오를수있음
                    if (jumpLadderTop) {//사다리 끝에 있는경우만오름
                          continue;  
                    }

                    next_y = y + 1;  
                    break;
                }
            }
        }
        else if (velocity_y > 0) {
        int y_from = player_y + 1;    
        int y_to   = next_y;
        if (y_to >= mapHeight[stage]) y_to = mapHeight[stage] - 1;

        for (int y = y_from; y <= y_to; y++) {
            char tile = map[stage][y][player_x];

            getCoin(player_x,y); 

            if (tile == '#') {
                next_y = y - 1;
                break;
            }
        }
    }

        velocity_y++;


        if (next_y < mapHeight[stage]) {
             player_y = next_y;
        }

 
        if (player_y + 1 < mapHeight[stage] &&
            map[stage][player_y + 1][player_x] == '#') {
            is_jumping = 0;
            velocity_y = 0;
        }
    } else {
        if (!on_ladder && floor_tile != '#' && floor_tile != 'H') {
             if (player_y + 1 < mapHeight[stage]) player_y++;
             else init_stage();
            }
        }
  }
    
    if (player_y >= mapHeight[stage]) init_stage();
}

// 적 이동 로직
void move_enemies() {
    for (int i = 0; i < enemy_count; i++) {
        int next_x = enemies[i].x + enemies[i].dir;
        if (next_x < 0 || next_x >= mapWidth[stage]|| map[stage][enemies[i].y][next_x] == '#' || (enemies[i].y + 1 < mapHeight[stage] && map[stage][enemies[i].y + 1][next_x] == ' ')) {
            enemies[i].dir *= -1;
        } else {
            enemies[i].x = next_x;
        }
    }
}


int game_overscr(){
    clrscr();
    printf("\n\n\n\n\n");
        printf("\n           ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        printf("\n           ┃                                          ┃");
        printf("\n           ┃                                          ┃");
        printf("\n           ┃                 GAME OVER                ┃");
        printf("\n           ┃                                          ┃");
        printf("\n           ┃                                          ┃");
        printf("\n           ┃              Press Y to restart          ┃");
        printf("\n           ┃                  N to quit               ┃");
        printf("\n           ┃                                          ┃");
        printf("\n           ┃                                          ┃");
        printf("\n           ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    char c;
    while (1) {
        c = getchar();
        if (c == 'y' || c == 'Y') {
            return 1;
        }
        if (c == 'n' || c == 'N') {
            printf("\n게임을 종료합니다.\n");
            return 0;
        }
    }
}

int game_clear1(){ // 첫 번째 스테이지 클리어 화면 출력 구현
    clrscr();
    printf("\n           ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    printf("\n           ┃                                          ┃");
    printf("\n           ┃                                          ┃");
    printf("\n           ┃             STAGE1 COMPLETE!             ┃");
    printf("\n           ┃                                          ┃");
    printf("\n           ┃                                          ┃");
    printf("\n           ┃                SCORE :   %3d             ┃", score);
    printf("\n           ┃          PRESS Y TO PLAY STAGE 2         ┃");
    printf("\n           ┃                 N TO QUIT                ┃");
    printf("\n           ┃                                          ┃");
    printf("\n           ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    char c;
    while (1) {
        c = getchar();
        if (c == 'y' || c == 'Y') {
            return 1;
        }
        if (c == 'n' || c == 'N') {
            printf("\n게임을 종료합니다.\n");
            return 0;
        }
    }
}

int game_clear2(){
    clrscr();
    printf("\n           ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    printf("\n           ┃                                          ┃");
    printf("\n           ┃                                          ┃");
    printf("\n           ┃                GAME CLEAR!               ┃");
    printf("\n           ┃                                          ┃");
    printf("\n           ┃                                          ┃");
    printf("\n           ┃                SCORE :   %3d             ┃", score);
    printf("\n           ┃             PRESS Y TO RESTART           ┃");
    printf("\n           ┃                 N TO QUIT                ┃");
    printf("\n           ┃                                          ┃");
    printf("\n           ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    char c;
    while (1) {
        c = getchar();
        if (c == 'y' || c == 'Y') {
            return 1;
        }
        if (c == 'n' || c == 'N') {
            printf("\n게임을 종료합니다.\n");
            return 0;
        }
    }
}

// 충돌 감지 로직
void check_collisions() {
    for (int i = 0; i < enemy_count; i++) {
        if (player_x == enemies[i].x && player_y == enemies[i].y) {
            score = (score > 50) ? score - 50 : 0;
            user_Heart--;
            beepsound(1);
            init_stage();
            return;
        }
    }
    getCoin(player_x,player_y);
}

// 비동기 키보드 입력 확인
// kbhit()함수 윈도우는 <conio.h>에 있으므로 리눅스에서만 정의<수정된 부분>
#ifndef _WIN32
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
#endif