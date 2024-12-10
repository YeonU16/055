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

// ����� �ؽ�Ʈ ��Ʈ
char* catArt[] = {
    " /\\_/\\ ",
    "( o.o )",
    " > ^ < "
};

// ����û�ұ� �ؽ�Ʈ ��Ʈ
char* vacuumArt[] = {
    "[====]",
    " |||| "
};

// ���� ��� �ؽ�Ʈ ��Ʈ
char* basketArt[] = {
    "      /\\_/\\   ",
    "     ( o.o )  ",
    "    |       |",
    "    |_______|",
    "                                         ",
    "                                         ",
    "����̸� ������ �����ϴµ� �����߽��ϴ�!",
    "                                         ",

};

// ���� ���� �ؽ�Ʈ ��Ʈ
char* gameOverArt[] = {
    "  /\\_/\\  ",
    " ( x.x ) ",
    "  > ^ <  ",
    " ���� ����!"
};

// ����û�ұ�� ������� ũ��
const int vacuumWidth = 6;
const int vacuumHeight = 2;
const int basketHeight = 7;

// ���� ���� ����
int catX = WIDTH / 2 - CAT_WIDTH / 2, catY = HEIGHT - CAT_HEIGHT - 1;  //����� �ʱ� ��ġ(����,����)
int vacuumX[MAX_VACUUMS], vacuumY[MAX_VACUUMS]; // ����û�ұ� ��ġ
int vacuumDirection[MAX_VACUUMS]; // �� ����û�ұ��� ����
int goalY = 3;    // ��¼� Y ��ǥ
int level = 1;    // �ʱ� �ܰ�
int speed = 150;  // �ʱ� �ӵ�


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

// ��¼��� �׸���
void drawGoal() {
    for (int x = 0; x < WIDTH; x++) {
        gotoxy(x, goalY);
        printf("=");   // ��¼��� '=' ���ڷ� �׸�
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

// ����û�ұ� �̵� �� �浹 ���� ������Ʈ
void moveVacuums() {
    for (int i = 0; i < level; i++) { // ���� �ܰ迡 ���� ����û�ұ� �� ����
        clearVacuum(i);     // ���� ��ġ�� ����û�ұ⸦ ����
        vacuumX[i] += vacuumDirection[i] * 2;    // ���� ��ġ�� ����û�ұ⸦ ����
        if (vacuumX[i] <= 0 || vacuumX[i] + vacuumWidth >= WIDTH) {
            vacuumDirection[i] *= -1; // ȭ�� ��踦 ������ ���� ����
        }
        drawVacuum(i);     // �� ��ġ�� ����û�ұ� �׸���
    }
}

// ����� �̵� ó��
void moveCat(char input) {
    clearCat();       // ���� ����� ��ġ �����
    if (input == 'a' && catX > 0) {                       // ���� �̵�
        catX -= 2;
    }
    else if (input == 'd' && catX + CAT_WIDTH < WIDTH) { // ������ �̵�
        catX += 2;
    }
    else if (input == 'w' && catY > goalY) {             // ���� �̵�
        catY -= 1;
    }
    else if (input == 's' && catY + CAT_HEIGHT < HEIGHT) { // �Ʒ��� �̵�
        catY += 1;
    }
    drawCat();         // �� ��ġ�� ����� �׸���
}

// ����̿� ����û�ұ� �浹 ���� Ȯ��
int checkCollision() {
    // ����û�ұ�� ����� �浹
    for (int i = 0; i < level; i++) {
        if (catY + CAT_HEIGHT > vacuumY[i] && catY < vacuumY[i] + vacuumHeight &&
            catX + CAT_WIDTH > vacuumX[i] && catX < vacuumX[i] + vacuumWidth) {
            return 1; // �浹 �߻�
        }
    }
    return 0;     // �浹 ����
}

// ���� ���� ���� �� �ӵ� ����
void increaseLevel() {
    level++;
    if (level > 3) level = 3; // �ִ� 3�ܰ�
    if (level == 2) speed = 120; // 2�ܰ� �ӵ�
    if (level == 3) speed = 90; // 3�ܰ� �ӵ�
}

// ���� ���� ȭ�� ���
void showStartScreen() {
    system("cls");
    gotoxy(WIDTH / 2 - 10, HEIGHT / 2 - 2);
    printf("���� �ܰ�: %d", level);
    gotoxy(WIDTH / 2 - 20, HEIGHT / 2);
    printf("����̸� �����̸� ������ ���۵˴ϴ�!!");
    drawCat(); // ����� ǥ��
}

// ���� ���� ȭ�� ���
void showGameOverScreen() {
    system("cls");
    int startX = WIDTH / 2 - 5; // ���� ���� ȭ�� �߾� ��ġ
    int startY = HEIGHT / 2 - 2;
    for (int i = 0; i < 4; i++) {
        gotoxy(startX, startY + i);
        printf("%s", gameOverArt[i]);
    }
    Sleep(100);
    _getch();
}

// ���� ȭ�� ���
void showEndingScreen() {
    system("cls");
    int startX = WIDTH / 2 - 6; // �ٱ��� ��Ʈ �߾� ��ġ
    int startY = HEIGHT / 2 - basketHeight / 2;
    for (int i = 0; i < basketHeight; i++) {
        gotoxy(startX, startY + i);
        printf("%s", basketArt[i]);
    }
}