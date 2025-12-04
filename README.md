#  너구리게임 프로젝트 보고서
## 깃허브 레포지토리 주소 https://github.com/jaehyunoo/nuguri_team9

---

## 1. 팀원 소개
| 학번 | 이름|
|------|------|
| 20253163 | 유재현 |
| 20223082 | 강세렘  |
| 20223148| 홍화수  |
| 20243122 | 이준영  |


---

## 2. 개발 환경

| 항목 | 내용 |
|------|------|
| 사용 언어 | C Language |
| 개발 툴 | Visual Studio Code / Xcode / VMware Oracle VirtualBox Ubuntu |
| 운영체제 | Windows, macOS, Linux |
| 버전 관리 | GitHub |
| 협업 방식 | 기능 분업 + 코드 리뷰 + 병합, HackMD를 이용한 보고서 공동 작성 |

---
## 깃허브 브랜치 설명 및 역할 분담

### 3-1. 깃허브 정보
__본 프로젝트에서 main 브랜치를 제외한 다른 브랜치의 nuguri.c 파일은 
해당 브랜치의 main.c와 동일한 내용을 포함하고 있습니다.__

__또한, 병합 과정에서 이력이 정상적으로 반영되지 않았을 가능성에 대비하여, 각 브랜치의 커밋 히스토리를 함께 확인해 주시기를 정중히 요청드립니다.__

__아울러, 개발 과정 중 nuguri.c에서 main.c로의 파일명 변경 작업이 있었으므로,
일부 브랜치에서는 파일명 변경 이전의 커밋 히스토리까지 함께 확인해 주시면 감사하겠습니다.__

| 브랜치 | 설명|
|------|------|
| main | 최종 결과물 업로드 브랜치 |
| dev | 팀원 각자 개발한 부분 병합 브랜치 |
| yjh | 유재현 브랜치 |
| seram | 강세렘 브랜치 |
| HS/Branch| 홍화수 브랜치 |
| lee | 이준영 브랜치 |

### 3-2. 역할 분담
| 이름 | 역할|
|------|------|
| 유재현 | 스테이지클리어 구현, 게임오버 구현, 동적메모리 할당 실패 시 해제 구현 |
| 강세렘 | os별 크로스 플랫폼지원 구현, 동적할당 구현  |
| 홍화수| 하트 생명력 시스템 구현, 동적 할당 구현, 점프로직 수정 구현, mac 비프음 구현  |
| 이준영 | 게임시작 전 타이틀 구현, 비프음 구현  |



---

## 4. OS별 컴파일 및 실행 방법

### Windows, Linux, Mac OS
**1) 컴파일**
공통 Makefile를 통하여 main.c를 컴파일

**2) 실행**
Makefile에 정의된 run 명령어를 통하여 실행
```
#소스파일 이름
SRC = main.c

#실행파일 이름
TARGET = final

#컴파일러 설정
CC = gcc

#OS 구분
ifeq ($(OS),Windows_NT)
    #----- Windows -----
    RM = del /Q
    TARGET := $(TARGET).exe
    CFLAGS = -Wall -O2 -D_WIN32
else
    #----- Linux / macOS -----
    RM = rm -f
    CFLAGS = -Wall -O2
endif

all: $(TARGET)

$(TARGET): $(SRC)
   $(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

run: $(TARGET)
   ./$(TARGET)

clean:
   $(RM) $(TARGET)
```

공통 부분
소스파일, 타겟파일, 컴파일러 변수로 설정하여 유지보수를 용이하게 함.

Window 부분
삭제 명령어 따로 설정

Linux, Mac OS 부분
삭제 명령어 따로 설정



---

## 5. 구현 기능 리스트 / 게임 스크린샷

### 5.1 구현된 기능 목록

> ### 5.1.1 크로스 플랫폼

| 변경사항 | 주요 구현 내용 |
|------|------|
| 헤더추가 | 언어설정 <locale.h>, 윈도우 헤더 <windows.h>, <conio.h> 추가 |
| 로케일 설정 | setlocale 함수로 로케일 설정, system("chcp 65001")로 윈도우 글자 깨짐 방지  |
| 터미널 Raw 모드 | 비표준 입력모드 처리함수 disable_raw_mode(), enable_raw_mode() 각각 조건부 컴파일  |
| delay함수 | Sleep(), usleep()를 조건부 컴파일하는 delay함수 구현 |
| 키보드 입력 |  wasd 방향키 입력 윈도우/리눅스 조건부 컴파일  |
| draw_game() | window에서 더블 버퍼링으로 맵을 출력하도록 설계  |
| kbhit() | kbhit()함수 조건부 컴파일 |

> ### 5.1.2 기능구현 

| 변경사항 | 주요 구현 내용 |
|------|------|
| 점프로직 변경 | 함수 맨위에서 이전 캐릭터 위치값을 기준으로 초기화 되었던 floor_title을 player_x값을 갱신한 후 밑에 정의하도록 수정|
| 오프닝 화면 | 게임오프닝 화면 추가 P를 입력해 게임 실행가능 |
| 엔딩 화면 |  |
| heart 추가 |   |
| 스테이지클리어 화면 | 스테이지 클리어 화면 추가, C로 획득한 점수 표기, Y를 입력해 다음 스테이지로 이동, N을 입력해 게임 종료 |
| 게임오버 구현 | 하트를 전부 소진했을 경우 게임 종료 화면 출력, Y를 입력해 게임 다시 시작, N을 입력해  종료  |
|  |  |
|  |  |

> ### 5.1.3 선택구현 

| 변경사항 | 주요 구현 내용 |
|------|------|
| 사운드 효과 |  |
| 동적 맵 할당 |  |

### 5.2 실제 게임 화면


### 5.3 기능구현 세부사항

    //너구리 점프로직 변경(move_player())
     char floor_tile = (player_y + 1 < mapHeight[stage]) ? map[stage][player_y + 1][player_x] : '#';//
     
기존의 코드와 비교해 봤을때 floor_title이 함수 맨위에서 이전 캐릭터 위치값을 기준으로 초기화 되어 문제가 발생하였습니다. 이전 위치를 기준으로 초기화된 floor_title을 이용할경우 하강을 할때 이전 캐릭터의 x값을 기준으로 충돌을 체그하여 대각선 밑에 블럭이 있는경우 관통하는 문제가 있어 floor_title을 player_x값을 갱신한 후 밑에 정의하여 문제를 해결함. if (input == ' ') { 키가 ' '이 들어왔을때 즉 점프를 하게 될때 floor_title을 이용하여 바닥위에서 점프 할수 있게 설계되어있어 floor_title이 선언된 후로 조건문 위치를 변경하였습니다.

     //if (input == ' ') {//점프하기위해서 ' '가 입력된 경우 새로 갱신된 floor_title을 활용
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
    }//
    
player가 h(사다리)위에 있는 경우에 점프 시도시 벽이 관통되는 문제를 해결하기 위해 사다리위에 있을때는 사다리 맨위(#바로아래 h)에서만 벽이 관통되어 점프할수 있도록 설계하였습니다.(jumpLadderTop변수 사용)


        //if (is_jumping) {//점프했을때 만약 위에 #이있을경우 부딫히고 아래로 내려갈떄 #이 통과되는 오류를 해결 velocity_y>0때 조건문으로
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
        }   //
        
현재 게임에서 점프를 하게 되면 velocity_y를 이용해서 프레임단위로 점프가 진행 되기 때문에 is_numping 분기문에서 점프를 하고 있는중에 velocity_y가 음수였을때(상승중,velocity_y = -2,-1)일때와 velocity_y가 양수였을때 (하강중, velocity_y = 1,2,++)이었을때로 구분하여 코드를 작성하였습니다. 만약 점프를 시도해 상승중이나 하강중일때 x(적)이나 c(코인), #(벽)을 만날 경우가 있기 때문에 반복문을 사용하여 미리 사용자 바로 위에서 부터(playery -1) 점프했을때 높이(next_y)까지 순회하며 장애물등을 확인하게 했습니다. 점프를 할경우 -2,-1,0,1,.. 이런식으로 velocity_y값이 프레임마다 증가되면서y위치가 바뀌게 구현했기 때문에 각 증가량마다 위나 아래로 y위치를 옮기기 전에 for문으로 검사를 할수있는 방식으로 구현했습니다.

        // if (tile == '#') {
                        //  사다리 맨끝에서 점프했을 때 바로 위에 #이 있는경우만 사다리를 오를수있음
                    if (jumpLadderTop) {//사다리 끝에 있는경우만오름
                          continue;  
                    }

                    next_y = y + 1;  
                    break;
                }//
아까 선언했던 jumpLadderTop이 1일경우 즉 사다리맨끝에서 점프했을경우 위에 벽 #을 관통할수있도록 코드를 작성했습니다.

            // } else {
        if (!on_ladder && floor_tile != '#' && floor_tile != 'H') {
             if (player_y + 1 < mapHeight[stage]) player_y++;
             else init_stage();
            }
        }//
        
점프를 하지 않고 이동중에 밑으로 떨어지는 경우 바닥이 #이 아니거나 H가 아니면 계속 y값을 증가하게 하는 중력이 구현되어 있습니다.






     
     
    
    

### 5.4 선택구현 세부사항

#### 5.4.1 동적 맵 할당

> #### 기존의 정적할당 맵을 동적할당으로 변경

    //게임 화면 그리기
    void draw_game(void) {

        int W = mapWidth[stage];   // 현재 스테이지 가로 길이
        int H = mapHeight[stage];  // 현재 스테이지 세로 길이

                              {...}
 
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
    
문자열 포인터 배열 char **display_map를 선언하여 맵의 각 줄(row)을 가리키는 포인터들의 배열을 만듭니다. 
display_map안에 전체 맵의 세로크기(H = 열의 갯수)만큼 문자열 포인터를 생성할 공간이 마련되며 각 열(row)이 개별적으로 할당합니다. 각 열의 가로길이는 W + 1 만큼 할당되며(+ 1 은 \n) H와 W는 스테이지마다 달라지기 때문에 동적할당(malloc)을 사용합니다.

malloc이 실패하면 프로그램을 종료하며 perror()로 이유를 출력합니다.

display_map에 공간이 만들어 지면 원본 맵(map)을 한 줄씩 읽어서, display_map[y]에 복사합니다. 이때 배경(벽, 빈칸 등)만 복사하고 출발점( S ), 적( X ), 코인( C ), 플레이어( P )는 공백으로 지운뒤 나중에 따로 출력합니다. 이렇게 하는 이유는 적, 코인, 플레이어는 계속 움직이기 때문에 출력할 때 마다 그 위치를 계산해서 따로 그려야 한다면 매 동작 마다 원본맵을 수정해야 해서 코드가 복잡해지기 때문입니다. 또한 충돌 처리, 경로 처리 등 게임 로직이 꼬이고 원본 맵이 오염되어 스테이지를 다시 시작할 때 문제가 생기는 등의 오류가 생기는 것도 방지할 수 있습니다.

오브젝트를 찍는 순서도 코인 -> 적 -> 플레이어 순서로 출력합니다. 플레이어를 마지막에 출력하는 이유는 플레이어가 코인을 먹거나 적과 충돌 할 때 적과 코인보다 앞에 출력되도록(덮어쓰기)하기 위함입니다.

플레이어의 위치 까지 맵에 표시되고 나면 맵이 display_map에서 버퍼로 옮겨지며 최종적으로 콘솔 화면에 출력됩니다. 

마지막으로 malloc으로 만든 메모리는 자동으로 사라지지 않기 때문에 사용후 free하지 않으면 메모리 누수가 발생합니다. free(display_map[y])과 free(display_map)을 순차적으로 사용해 각 행의 char 배열을 해제하고 마지막으로 display_map 포인터 배열 해제하여 메모리를 깨끗하게 회수합니다.


---

#### 5.4.2 사운드 효과 

> #### OS에 따라 다른 방식으로 효과음을 내기 위한 함수

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

윈도우와 맥 리눅스에서 각각 소리를 추력하는 방식이 달라 ifdef 전처리기로 조건부 컴파일 하였고 각 os별로 switch문을 사용해 체력 감소시, 점프시, 코인 획득시 출력되는 소리를 구분하였습니다.

윈도우는 자체적으로 Beep(frequency, duration) 함수를 제공하기 때문에 이를 이용하여 HP가 감소할 때 700Hz, 점프 시 1000Hz, 코인 획득 시 1300Hz 소리가 나도록 구현했습니다.

맥은 Windows처럼 Beep API가 없어서 OS에 있는 사운드 파일을 재생하는 방식을 사용했습니다.

리눅스에서는 콘솔에서 직접 소리 나는 경우가 제한적이기 때문에 가장 기본적인 "터미널 벨 소리"를 사용했습니다.
___

## 6. OS 호환성 문제와 해결 과정 

### 6.1 헤더추가

> #### 언어설정 <locale.h>, 윈도우 헤더 <windows.h>, <conio.h> 추가 
 
    #include <locale.h> //setlocale 및 LC_ALL 사용을 위해// 글자 깨짐 방지 <새로 추가한 헤더>

    #ifdef _WIN32 //윈도우전용 헤더 <수정된 부분>
    #include <windows.h>  
    #include <conio.h>  
    
<windows.h>는 Windows 운영체제 전용 API(WinAPI) 헤더로 코드에서 Windows 기능을 사용하기 위해 선언되어야 합니다.
Sleep()이 Window.h에 포함되어 있으며 화면을 제어하는 역할을 하기에 clrscr(), gotoxy()등의 함수를 사용하기 위해 필요합니다.

<conio.h>는 콘솔 입출력 함수를 제공하는 헤더입니다. getch(), kbhit() 같은 함수가 포함되어 있습니다.

<locale.h>는 C 표준 라이브러리에 포함된 헤더로 로케일(언어, 국가별 지역 설정)을 바꾸기 위해 사용 됩니다.

---

### 6.2 로케일 설정 및 글자 깨짐 방지

> #### setlocale 함수로 로케일 설정, 윈도우 글자 깨짐 방지
 
    //한국어 로케일 설정, c언어 에서 한글 입출력을 정상적으로 처리하기 위해서 <수정된 부분>
    setlocale(LC_ALL, "ko_KR.UTF-8");

    //윈도우에서 글자 깨짐 방지 <수정된 부분 >
    #ifdef _WIN32
    system("chcp 65001");
    #endif
    
setlocale(category, locale)은 현재 프로그램의 지역 설정(locale)을 바꿉니다.

system("chcp 65001")은 Windows 콘솔(cmd) 환경에서 UTF-8 인코딩을 강제로 설정하는 명령어로 한글 깨짐 방지용 코드 입니다.

setlocale()은 프로그램 내부의 로케일을 바꾸는 함수이고, system("chcp 65001")은 콘솔의 문자 인코딩 설정을 바꾸는 명령어로 둘은 서로 다른 레벨에서 동작합니다. 따라서 둘 중 하나라도 없으면 한글 출력이 정상적으로 작동되지 않습니다.
    
---
    
### 6.3 터미널 Raw 모드

> #### 비표준 입력모드 처리함수 조건부 컴파일

    // 터미널 설정
    //disable_raw_mode()함수에서 콘솔상태 저장 변수 분기 <수정된 부분>
    #ifdef _WIN32
    static DWORD original_mode;
    #else
    struct termios orig_termios;
    #endif
    
***
    
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
    
게임에서 실시간 입력을 하려면 표준 입력을 비표준 모드(Raw Mode)로 바꿔야 합니다. 
하지만 윈도우에서는 GetConsoleMode, SetConsoleMode, 리눅스에서는 termios, tcsetattr로 기능을 사용하기위해 쓰는 API가 다르기때문에 전처리문을 이용해 조건부 컴파일 하였습니다.

콘솔상태 저장 변수는 Raw 모드를 사용하기 전에 현재 터미널 설정을 저장하고 Raw 모드 종료 시 이 값을 다시 복구할 때 쓰입니다. 이 저장변수를 리눅스는 termios 구조체를, 윈도우는 DWORD로 사용합니다.

enable_raw_mode()는 터미널을 게임용 RAW모드(빠른 입력 모드)로 바꾸는 함수이고 disable_raw_mode()는 게임 실행 중 터미널을 Raw 모드로 바꿨다면, 게임이 끝날 때 원래 상태로 되돌리는 함수입니다.

리눅스에서는 ICANON(입력시 Enter 필요)과 ECHO(입력 문자를 화면에 출력하는)를 꺼서 RAW모드를 만들고
윈도우에서는 ENABLE_LINE_INPUT(사용자가 Enter를 누를 때까지 입력을 버퍼링)과 ENABLE_ECHO_INPUT(입력한 문자를 화면에 출력을)을 꺼서 RAW 모드를 활성화합니다.

이후 각각 disable_raw_mode()에서 이전 터미널 상태를 저장해 두었던 original_mode, orig_termios를 다시 적용해 RAW 모드 에서 바뀐 옵션을 원상 복구합니다.
___

### 6.4 delay함수

> #### Sleep(), usleep() 조건부 컴파일
 
    //delay함수 윈도우,리눅스용 분기 나눔<새로 추가한 함수>
    void delay(int ms) {
    #ifdef _WIN32
      Sleep(ms);      // Windows: 밀리초 단위
    #else
    usleep(ms * 1000); // Linux/macOS: 마이크로초 단위 1밀리초 = 1000 마이크로초
    #endif
    }
    
화면을 잠시 멈추는 Sleep() 함수가 윈도우에서는 밀리초 단위를 사용하는 Sleep()으로 리눅스/맥 에서는 마이크로초 단위의 usleep()으로 다르기에 delay 함수안에 조건부 컴파일 하여 delay를 사용하면 일일이 구분하여 작성 할 필요없이 사용 가능하게 구현하였습니다. 
    
---

### 6.5 키보드 입력

> #### wasd 방향키 입력 조건부 컴파일 

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
                _raw_mode();
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

기존의 방향키 입력 코드는 ESC 시퀀스를 getchar()로 받아서 WASD로 움직이도록 처리하는 방식입니다. 하지만 윈도우에서 이 방법을 그대로 사용했을 때 방향키 입력시 렉이 생기는 버그가 발생했습니다. 때문에 윈도우에서는 kbhit()로 입력을 확인하고 getch()로 문자를 받는 큰 틀은 유지한채 WASD에 ESC 시퀀스 대신 방향키 스캔코드를 대응해서 작동하도록 조건부 컴파일 하였습니다.  

___
        
### 6.6 draw_game()

> #### window에서 더블 버퍼링으로 맵을 출력하도록 설계 

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

                                       {...}
      
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

printf를 이용하여 문자열을 하나씩 출력하고 지우는 방식이 기존의 화면출력 방식입니다.
그러나 이 방식는 윈도우 콘솔에서 너무 느리게 처리되고 화면 깜빡임 현상도 일어나서 더블 버퍼링을 사용하였습니다.

기본적으로 더블버퍼링의 작동방식은 현재 화면을 버퍼(buffer)에 저장한뒤 이전 화면(pre_buffer)와 현재 화면(buffer)을 비교하여 변경 사항이 있을 시 버퍼(buffer)안의 문자열을 화면에 출력하는 방식입니다.

출력 후 buffer의 문자열은 pre_buffer에 저장되며 이후 새 화면을 buffer에 입력합니다. 

이 코드에서는 기본적으로 윈도우와 리눅스 모두 더블 버퍼링을 사용하나 약간의 차이가 존재합니다.

윈도우는 화면을 그릴 때 터미널 화면의 커서 위치만 초기화 시킨 뒤 이전 화면을 지우지 않고 그 위에 덮어쓰는 방식을 사용합니다. 
리눅스/맥은 화면을 그릴 때 커서 위치를 초기화 하고 이전 화면도 싹다 지운 후 새로 출력하는 방식을 사용합니다.

리눅스/맥 콘솔은 처리속도가 빨라서 화면을 전부 지우고 다시 그리는 방식이 가능하나 윈도우 콘솔은 처리속도가 느려 전체를 지웠다 다시 그리는 것이 깜빡임 현상을 유도할 수 있기때문입니다.

---

### 6.7 kbhit()

> #### kbhit()함수 조건부 컴파일 

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

kbhit()함수는 키보드가 눌렸는지 안누렸는지 여부를 즉시 알려주는 함수입니다. 키보드가 눌렸으면 1을, 아무 입력도 없으면 0을 리턴합니다. 
입력을 실제로 읽지는 않고 버퍼만 검사합니다. 
윈도우에서는 <conio.h>헤더에 포함되어 있어 그대로 사용하고 윈도우가 아닌 경우에는 따로 커스텀을 해서 사용합니다. 

---


