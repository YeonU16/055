#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h>

// 화면크기, 고양이와 진공 청소기 크기 정의
#define WIDTH 120      // 화면 너비
#define HEIGHT 25      // 화면 높이
#define CAT_WIDTH 7    // 고양이 아트 너비
#define CAT_HEIGHT 3   // 고양이 아트 높이
#define MAX_VACUUMS 10 // 진공청소기 최대 수

// 고양이 텍스트 아트
char* catArt[] = {
    " /\\_/\\ ",
    "( o.o )",
    " > ^ < "
};

// 진공청소기 텍스트 아트
char* vacuumArt[] = {
    "[====]",
    " |||| "
};

// 엔딩 장면 텍스트 아트
char* basketArt[] = {
    "      /\\_/\\   ",
    "     ( o.o )  ",
    "    |       |",
    "    |_______|",
    "                                         ",
    "                                         ",
    "고양이를 무사히 구출하는데 성공했습니다!",
    "                                         ",

};

// 게임 오버 텍스트 아트
char* gameOverArt[] = {
    "  /\\_/\\  ",
    " ( x.x ) ",
    "  > ^ <  ",
    " 게임 오버!"
};

// 진공청소기와 고양이의 크기
const int vacuumWidth = 6;
const int vacuumHeight = 2;
const int basketHeight = 7;

// 게임 상태 변수
int catX = WIDTH / 2 - CAT_WIDTH / 2, catY = HEIGHT - CAT_HEIGHT - 1;  //고양이 초기 위치(가로,세로)
int vacuumX[MAX_VACUUMS], vacuumY[MAX_VACUUMS]; // 진공청소기 위치
int vacuumDirection[MAX_VACUUMS]; // 각 진공청소기의 방향
int goalY = 3;    // 결승선 Y 좌표
int level = 1;    // 초기 단계
int speed = 150;  // 초기 속도


// 커서를 특정 위치로 이동시키는 함수
void gotoxy(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 콘솔 커서를 숨기는 함수
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = 0;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void drawCat() {
    for (int i = 0; i < CAT_HEIGHT; i++) {
        gotoxy(catX, catY + i);
        printf("%s", catArt[i]);
    }
}

// 결승선을 그리기
void drawGoal() {
    for (int x = 0; x < WIDTH; x++) {
        gotoxy(x, goalY);
        printf("=");   // 결승선은 '=' 문자로 그림
    }
}

// 특정 진공청소기를 그리기
void drawVacuum(int index) {
    for (int i = 0; i < vacuumHeight; i++) {
        gotoxy(vacuumX[index], vacuumY[index] + i);
        printf("%s", vacuumArt[i]);
    }
}

// 특정 진공청소기를 지우기
void clearVacuum(int index) {
    for (int i = 0; i < vacuumHeight; i++) {
        gotoxy(vacuumX[index], vacuumY[index] + i);
        printf("      "); // 진공청소기 크기만큼 지우기
    }
}

// 진공청소기 이동 및 충돌 영역 업데이트
void moveVacuums() {
    for (int i = 0; i < level; i++) { // 현재 단계에 따라 진공청소기 수 증가
        clearVacuum(i);     // 기존 위치의 진공청소기를 지움
        vacuumX[i] += vacuumDirection[i] * 2;    // 기존 위치의 진공청소기를 지움
        if (vacuumX[i] <= 0 || vacuumX[i] + vacuumWidth >= WIDTH) {
            vacuumDirection[i] *= -1; // 화면 경계를 넘으면 방향 반전
        }
        drawVacuum(i);     // 새 위치에 진공청소기 그리기
    }
}

// 고양이 이동 처리
void moveCat(char input) {
    clearCat();       // 기존 고양이 위치 지우기
    if (input == 'a' && catX > 0) {                       // 왼쪽 이동
        catX -= 2;
    }
    else if (input == 'd' && catX + CAT_WIDTH < WIDTH) { // 오른쪽 이동
        catX += 2;
    }
    else if (input == 'w' && catY > goalY) {             // 위로 이동
        catY -= 1;
    }
    else if (input == 's' && catY + CAT_HEIGHT < HEIGHT) { // 아래로 이동
        catY += 1;
    }
    drawCat();         // 새 위치에 고양이 그리기
}

// 고양이와 진공청소기 충돌 여부 확인
int checkCollision() {
    // 진공청소기와 고양이 충돌
    for (int i = 0; i < level; i++) {
        if (catY + CAT_HEIGHT > vacuumY[i] && catY < vacuumY[i] + vacuumHeight &&
            catX + CAT_WIDTH > vacuumX[i] && catX < vacuumX[i] + vacuumWidth) {
            return 1; // 충돌 발생
        }
    }
    return 0;     // 충돌 없음
}

// 게임 레벨 증가 및 속도 조절
void increaseLevel() {
    level++;
    if (level > 3) level = 3; // 최대 3단계
    if (level == 2) speed = 120; // 2단계 속도
    if (level == 3) speed = 90; // 3단계 속도
}

// 게임 시작 화면 출력
void showStartScreen() {
    system("cls");
    gotoxy(WIDTH / 2 - 10, HEIGHT / 2 - 2);
    printf("현재 단계: %d", level);
    gotoxy(WIDTH / 2 - 20, HEIGHT / 2);
    printf("고양이를 움직이면 게임이 시작됩니다!!");
    drawCat(); // 고양이 표시
}

// 게임 오버 화면 출력
void showGameOverScreen() {
    system("cls");
    int startX = WIDTH / 2 - 5; // 게임 오버 화면 중앙 배치
    int startY = HEIGHT / 2 - 2;
    for (int i = 0; i < 4; i++) {
        gotoxy(startX, startY + i);
        printf("%s", gameOverArt[i]);
    }
    Sleep(100);
    _getch();
}

// 엔딩 화면 출력
void showEndingScreen() {
    system("cls");
    int startX = WIDTH / 2 - 6; // 바구니 아트 중앙 배치
    int startY = HEIGHT / 2 - basketHeight / 2;
    for (int i = 0; i < basketHeight; i++) {
        gotoxy(startX, startY + i);
        printf("%s", basketArt[i]);
    }
}