#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h>

// ȭ��ũ��, ����̿� ���� û�ұ� ũ�� ����
#define WIDTH 120      // ȭ�� �ʺ�
#define HEIGHT 25      // ȭ�� ����
#define CAT_WIDTH 7    // ����� ��Ʈ �ʺ�
#define CAT_HEIGHT 3   // ����� ��Ʈ ����
#define MAX_VACUUMS 10 // ����û�ұ� �ִ� ��

// Ŀ���� Ư�� ��ġ�� �̵���Ű�� �Լ�
void gotoxy(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// �ܼ� Ŀ���� ����� �Լ�
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

// Ư�� ����û�ұ⸦ �׸���
void drawVacuum(int index) {
    for (int i = 0; i < vacuumHeight; i++) {
        gotoxy(vacuumX[index], vacuumY[index] + i);
        printf("%s", vacuumArt[i]);
    }
}

// Ư�� ����û�ұ⸦ �����
void clearVacuum(int index) {
    for (int i = 0; i < vacuumHeight; i++) {
        gotoxy(vacuumX[index], vacuumY[index] + i);
        printf("      "); // ����û�ұ� ũ�⸸ŭ �����
    }
}