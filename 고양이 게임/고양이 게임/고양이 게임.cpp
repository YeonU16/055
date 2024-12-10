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