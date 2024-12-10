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