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
#define MAP_WIDTH 40  // 맵 너비를 40으로 변경
#define MAP_HEIGHT 20
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
char map[MAX_STAGES][MAP_HEIGHT][MAP_WIDTH + 1];
int player_x, player_y;
int stage = 0;
int score = 0;

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
void load_maps();
void init_stage();
void draw_game();
void update_game(char input);
void move_player(char input);
void move_enemies();
void check_collisions();
void game_overscr();
void game_clear1();
void game_clear2();
int kbhit();

    
int kbhit();
void opening(); //수정됨 게임 시작시 화면 띄우기
void clrscr(); //수정됨 화면 지우고 (1,1)로 커서 이동
void gotoxy(int x, int y); // 수정됨 화면 그대로 (x,y)로 이동
void beepsound(int sel);
int kbhit();

//delay함수 윈도우,리눅스용 분기 나눔<새로 추가한 함수>
void delay(int ms) {
#ifdef _WIN32
  Sleep(ms);      // Windows: 밀리초 단위
#else
usleep(ms * 1000); // Linux/macOS: 마이크로초 단위 1밀리초 = 1000 마이크로초
#endif
}

int main() {

    //한국어 로케일 설정, c언어 에서 한글 입출력을 정상적으로 처리하기 위해서 <수정된 부분>
    setlocale(LC_ALL, "ko_KR.UTF-8");

    //윈도우에서 글자 깨짐 방지 <수정된 부분 >
    #ifdef _WIN32
    system("chcp 65001");
    #endif
    
    opening();
    srand(time(NULL));
    enable_raw_mode();
    load_maps();
    init_stage();

    char c = '\0';
    int game_over = 0;

    while (!game_over && stage < MAX_STAGES) {
        if (kbhit()) {
            c = getchar();
            if (c == 'q') {
                game_over = 1;
                continue;
            }
            if (c == '\x1b') {
                getchar(); // '['
                switch (getchar()) {
                    case 'A': c = 'w'; break; // Up
                    case 'B': c = 's'; break; // Down
                    case 'C': c = 'd'; break; // Right
                    case 'D': c = 'a'; break; // Left
                }
            }
        } else {
            c = '\0';
        }

        update_game(c);
        draw_game();
        usleep(90000);


        if(user_Heart==0)
        {
            game_overscr();
            exit(0);
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
            if (stage + 1 < MAX_STAGES) {
                stage++;
                init_stage();
                game_clear1(); // 첫 스테이지 클리어 메시지
            } else {
                game_clear2(); // 마지막 스테이지 클리어
                game_over = 1;
                clrscr();
                printf("축하합니다! 모든 스테이지를 클리어했습니다!\n");
                printf("최종 점수: %d\n", score);
            }
        }
    }

    disable_raw_mode();
    return 0;
}

void opening(){
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
            return;
        else 
            exit(0);
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

void beepsound(int sel){ //수정됨 추가기능2 리눅스는 헤더파일 추가 X, 윈도우는 window.h 필요. 추가 예정
    switch(sel){
        case 1: //수정됨 hp 감소시
        printf("\a");
        printf("\a");
        printf("\a");
        fflush(stdout);
        break;

        case 2: //수정됨 점프, 사다리 이동시
        printf("\a");
        fflush(stdout);

        case 3:
        printf("\a"); //수정됨 스테이지 이동시
        printf("\a");
        fflush(stdout);

        default:
        return;
    }
}

/*
윈도우 버전 window.h 필요
void beepsound(int sel){
    switch(sel){
        case 1:
        beep();
        break;
        
        case 2:
        beep();
        break;
        
}

*/

void sizecheck(){
    FILE *file = fopen("map.txt", "r");
    if (!file) {
        perror("map.txt 파일을 열 수 없습니다.");
        exit(1);
    }
    while(1){
        if scanf()
    }
}


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

// 맵 파일 로드
void load_maps() {
    FILE *file = fopen("map.txt", "r");
    if (!file) {
        perror("map.txt 파일을 열 수 없습니다.");
        exit(1);
    }
    int s = 0, r = 0;
    char line[MAP_WIDTH + 2]; // 버퍼 크기는 MAP_WIDTH에 따라 자동 조절됨
    while (s < MAX_STAGES && fgets(line, sizeof(line), file)) {
        if ((line[0] == '\n' || line[0] == '\r') && r > 0) {
            s++;
            r = 0;
            continue;
        }
        if (r < MAP_HEIGHT) {
            line[strcspn(line, "\n\r")] = 0;
            strncpy(map[s][r], line, MAP_WIDTH + 1);
            r++;
        }
    }
    fclose(file);
}


// 현재 스테이지 초기화
void init_stage() {
    enemy_count = 0;
    coin_count = 0;
    is_jumping = 0;
    velocity_y = 0;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
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
void draw_game() {

    //윈도우에서 매 프레임마다 printf()로 한 문자씩 그리면 깜빡임 현상이 너무 심해 더블 버퍼링 사용 <수정된 부분>
    static char prev_buffer[4096] = {0}; // 이전 화면 저장
    char buffer[4096] = {0}; // 현재 화면 버퍼
    int cursor_location = 0; // 현재 커서 위치

    // 커서 숨김 (Linux/Windows 공통)
    printf("\x1b[?25l");

    #ifdef _WIN32
        printf("\x1b[H"); // 커서만 이동 (화면 전체 지우지 않음)
    #else
        printf("\x1b[2J\x1b[H"); // 리눅스/맥은 빠르니까 전체 화면 지우기 유지
    #endif

    // 게임 UI 출력
    //sprintf() 는 문자열을 버퍼에 넣는 함수, cursor_location는 버퍼의 현재 위치를 가리킴
    //따라서 sprintf() 호출 후 반환값(문자열 길이)을 더해 다음 출력 위치로 이동 <수정된 부분>
    cursor_location += sprintf(buffer + cursor_location, "Stage: %d | Score: %d Heart: %d\n", stage + 1, score, user_Heart);
    cursor_location += sprintf(buffer + cursor_location, "조작: ← → (이동), ↑ ↓ (사다리), Space (점프), q (종료)\n");

    // 맵 구성
    char display_map[MAP_HEIGHT][MAP_WIDTH + 1];
    for(int y=0; y < MAP_HEIGHT; y++) {
        for(int x=0; x < MAP_WIDTH; x++) {
            char cell = map[stage][y][x];
            display_map[y][x] = (cell == 'S' || cell == 'X' || cell == 'C') ? ' ' : cell;
        }
    }

    for (int i = 0; i < coin_count; i++)
        if (!coins[i].collected)
            display_map[coins[i].y][coins[i].x] = 'C';

    for (int i = 0; i < enemy_count; i++)
        display_map[enemies[i].y][enemies[i].x] = 'X';

    display_map[player_y][player_x] = 'P';

    // 맵을 버퍼에 넣기<수정된 부분>
    for (int y = 0; y < MAP_HEIGHT; y++) {
        memcpy(buffer + cursor_location, display_map[y], MAP_WIDTH); //memcpy()는 메모리 블록을 복사하는 함수, 버퍼에 맵 데이터를 한 줄(display_map[y]) 씩 복사
        cursor_location += MAP_WIDTH; // 복사한 만큼 커서 위치 이동
        buffer[cursor_location++] = '\n'; // 각 줄 끝에 줄바꿈 추가, \n을 넣지 않으면 맵이 한 줄로 쭉 이어져서 출력
    }

    buffer[cursor_location] = '\0'; // 배열 끝에 널 문자 추가, 없으면 문자열 끝을 읽지 못해 오류 발생<수정된 부분>

    // 이전 프레임과 동일하면 아예 출력하지 않음, 즉 변경된 부분만 출력 <수정된 부분>
    if (strcmp(prev_buffer, buffer) != 0) {
        #ifdef _WIN32
            //Windows에서는 printf가 느리고 깜빡임이 생기기 때문에 콘솔 API인 WriteConsoleA() 를 사용해 출력.
            WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), buffer, cursor_location, NULL, NULL);
        #else
            printf("%s", buffer); // 리눅스/맥은 그냥 출력

        #endif
            strcpy(prev_buffer, buffer); // 현재 화면을 이전 화면으로 저장, 화면 변화 감지를 위해 <수정된 부분>
    }
}

// 게임 상태 업데이트
void update_game(char input) {
    move_player(input);
    move_enemies();
    check_collisions();
}

// 플레이어 이동 로직
void move_player(char input) {
    int next_x = player_x, next_y = player_y;
    char floor_tile = (player_y + 1 < MAP_HEIGHT) ? map[stage][player_y + 1][player_x] : '#';
    char current_tile = map[stage][player_y][player_x];

    on_ladder = (current_tile == 'H');

    switch (input) {
        case 'a': next_x--; break;
        case 'd': next_x++; break;
        case 'w': if (on_ladder) next_y--; break;
        case 's': if (on_ladder && (player_y + 1 < MAP_HEIGHT) && map[stage][player_y + 1][player_x] != '#') next_y++; break;
        case ' ':
            if (!is_jumping && (floor_tile == '#' || floor_tile  == 'H' ||  on_ladder)) {
            if (!is_jumping && (floor_tile == '#' || on_ladder)) {
                is_jumping = 1;
                velocity_y = -2;
            }
            break;
    }

    if (next_x >= 0 && next_x < MAP_WIDTH && map[stage][player_y][next_x] != '#') player_x = next_x;
    
    if (on_ladder && (input == 'w' || input == 's')) {
        if(next_y >= 0 && next_y < MAP_HEIGHT && map[stage][next_y][player_x] != '#') {
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

                if (tile == 'X') {
                    user_Heart--;
                    init_stage();
                    return;  
                }

       
                if (tile == '#') { //-> 이부분 보완 사다리위에서 이전의 기능으로는 충돌되서 점프가안됨
                    if (on_ladder && y + 1 < MAP_HEIGHT && 
                        map[stage][y + 1][player_x] == 'H') {
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
        if (y_to >= MAP_HEIGHT) y_to = MAP_HEIGHT - 1;

        for (int y = y_from; y <= y_to; y++) {
            char tile = map[stage][y][player_x];

            if (tile == '#') {
                next_y = y - 1;
                break;
            }
        }
    }

        velocity_y++;


        if (next_y < MAP_HEIGHT) {
            player_y = next_y;
        }

 
        if (player_y + 1 < MAP_HEIGHT &&
            map[stage][player_y + 1][player_x] == '#') {
            is_jumping = 0;
            velocity_y = 0;
        }
    } else {
        if (floor_tile != '#' && floor_tile != 'H') {
             if (player_y + 1 < MAP_HEIGHT) player_y++;
             else init_stage();
        }
    }
}
        if (is_jumping) {
            next_y = player_y + velocity_y;
            if(next_y < 0) next_y = 0;
            velocity_y++;

            if (velocity_y < 0 && next_y < MAP_HEIGHT && map[stage][next_y][player_x] == '#') {
                velocity_y = 0;
            } else if (next_y < MAP_HEIGHT) {
                player_y = next_y;
            }
            
            if ((player_y + 1 < MAP_HEIGHT) && map[stage][player_y + 1][player_x] == '#') {
                is_jumping = 0;
                velocity_y = 0;
            }
        } else {
            if (floor_tile != '#' && floor_tile != 'H') {
                 if (player_y + 1 < MAP_HEIGHT) player_y++;
                 else init_stage();
            }
        }
    }
    
    if (player_y >= MAP_HEIGHT) init_stage();
}


// 적 이동 로직
void move_enemies() {
    for (int i = 0; i < enemy_count; i++) {
        int next_x = enemies[i].x + enemies[i].dir;
        if (next_x < 0 || next_x >= MAP_WIDTH || map[stage][enemies[i].y][next_x] == '#' || (enemies[i].y + 1 < MAP_HEIGHT && map[stage][enemies[i].y + 1][next_x] == ' ')) {
            enemies[i].dir *= -1;
        } else {
            enemies[i].x = next_x;
        }
    }
}


void game_overscr(){
    printf("\x1b[2J\x1b[H");
    printf("gameover\n");
    printf("다시 게임을 시작하시겠습니까? 네(y),아니요(n)");
    char c;
    while (1) {
        c = getchar();
        if (c == 'y' || c == 'Y') {
            stage = 0;
            score = 0;
            user_Heart = 3;
            main();
            return;
        }
        if (c == 'n' || c == 'N') {
            printf("\n게임을 종료합니다.\n");
            exit(0);
        }
    }
}

void game_clear1(){ // 첫 번째 스테이지 클리어 화면 출력 구현
    printf("\x1b[2J\x1b[H");
    printf("첫 번째 스테이지를 클리어했습니다!\n");
    printf("현재 점수: %d\n", score);
    printf("다음 스테이지를 시작하시겠습니까? 네(y),아니요(n)");
    char c;
    while (1) {
        c = getchar();
        if (c == 'y' || c == 'Y') {
            // stage = 0;
            // score = 0;
            init_stage();
            // main();
            return;
        }
        if (c == 'n' || c == 'N') {
            printf("\n게임을 종료합니다.\n");
            exit(0);
        }
    }
}

void game_clear2(){
    printf("\x1b[2J\x1b[H");
    printf("축하합니다! 모든 스테이지를 클리어했습니다!\n");
    printf("최종 점수: %d\n", score);
    printf("다시 게임을 시작하시겠습니까? 네(y),아니요(n)");
    char c;
    while (1) {
        c = getchar();
        if (c == 'y' || c == 'Y') {
            stage = 0;
            score = 0;
            user_Heart = 3;
            // init_stage();
            main();
            return;
        }
        if (c == 'n' || c == 'N') {
            printf("\n게임을 종료합니다.\n");
            exit(0);
        }
    }
}

// 충돌 감지 로직
void check_collisions() {
    for (int i = 0; i < enemy_count; i++) {
        if (player_x == enemies[i].x && player_y == enemies[i].y) {
            score = (score > 50) ? score - 50 : 0;
            user_Heart--;
            init_stage();
            return;
        }
    }
    for (int i = 0; i < coin_count; i++) {
        if (!coins[i].collected && player_x == coins[i].x && player_y == coins[i].y) {
            coins[i].collected = 1;
            score += 20;
        }
    }
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
