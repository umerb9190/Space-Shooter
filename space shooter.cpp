#include<iostream>
#include<conio.h>
#include<dos.h> 
#include <windows.h>


#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define Enemy_DIF 45

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;
int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char bird[3][5] = { 206,' ',' ',' ',' ',
				' ',' ','  ',' ',' ',
                ' ',' ',' ',' ',' ' };         //enemy formation
int birdPos = WIN_WIDTH / 2;
int score = 0;
int bullets[20][4];
int bulletsLife[20];
int bIndex = 0;

void gotoxy(int x, int y) {
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
void setcursor(bool visible, DWORD size) {
	if (size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}
void drawBorder() {

	for (int i = 0; i < SCREEN_WIDTH; i++) {
		gotoxy(i, SCREEN_HEIGHT); cout << "_";
	}

	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(0, i); cout << "|";
		gotoxy(SCREEN_WIDTH, i); cout << "|";
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(WIN_WIDTH, i); cout << "|";
	}
}
void genEnemy(int ind) {
	enemyX[ind] = 3 + rand() % (WIN_WIDTH - 10);
}
void drawEnemy(int ind) {
	if (enemyFlag[ind] == true) {
		gotoxy(enemyX[ind], enemyY[ind]);   cout << " ** ";
		gotoxy(enemyX[ind], enemyY[ind] + 1); cout << "****";

	}
}
void eraseEnemy(int ind) {
	if (enemyFlag[ind] == true) {
		gotoxy(enemyX[ind], enemyY[ind]); cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 1); cout << "    ";
	}
}
void resetEnemy(int ind) {
	eraseEnemy(ind);
	enemyY[ind] = 4;
	genEnemy(ind);
}
void genBullet() {
	bullets[bIndex][0] = 21;
	bullets[bIndex][1] = birdPos;
	bIndex++;
	if (bIndex == 20)
		bIndex = 0;
}
void moveBullet() {
	for (int i = 0; i < 19; i++) {
		if (bullets[i][0] > 2)
			bullets[i][0]--;
		else
			bullets[i][0] = 1;
	}
}
void drawBullets() {
	for (int i = 0; i < 19; i++) {
		if (bullets[i][0] > 1) {
			gotoxy(bullets[i][1], bullets[i][0]); cout << "^";
		}
	}
}
void eraseBullets() {
	for (int i = 0; i < 20; i++) {
		if (bullets[i][0] >= 1) {
			gotoxy(bullets[i][1], bullets[i][0]); cout << " ";
			gotoxy(bullets[i][3], bullets[i][2]); cout << " ";
		}
	}
}
void eraseBullet(int i) {
	gotoxy(bullets[i][1], bullets[i][0]); cout << " ";
	gotoxy(bullets[i][3], bullets[i][2]); cout << " ";
}
void drawBird() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			gotoxy(j + birdPos, i + 22); cout << bird[i][j];
		}
	}
}
void eraseBird() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			gotoxy(j + birdPos, i + 22); cout << " ";
		}
	}
}

int collision() {
	if (enemyY[0] + 4 >= 23) {
		if (enemyX[0] + 4 - birdPos >= 0 && enemyX[0] + 4 - birdPos < 8) {
			return 1;
		}
	}
	return 0;
}
int bulletHit() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 4; j += 2) {
			if (bullets[i][j] != 0) {
				if (bullets[i][j] >= enemyY[0] && bullets[i][j] <= enemyY[0] + 4) {
					if (bullets[i][j + 1] >= enemyX[0] && bullets[i][j + 1] <= enemyX[0] + 4) {
						eraseBullet(i);
						bullets[i][j] = 0;
						resetEnemy(0);
						return 1;
					}
				}
				if (bullets[i][j] >= enemyY[1] && bullets[i][j] <= enemyY[1] + 4) {
					if (bullets[i][j + 1] >= enemyX[1] && bullets[i][j + 1] <= enemyX[1] + 4) {
						eraseBullet(i);
						resetEnemy(1);
						bullets[i][j] = 0;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}
void gameover() {
	system("cls");
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	cout << "                 ______   ______   ______ ______    ______         ______            ______   ______            " << endl;
	cout << "                |        |      | |      |      |  |              |      | |      | |        |      |           " << endl;
	cout << "                | _____  |______| |      |      |  |_____         |      | |      | |_____   |______|           " << endl;
	cout << "                |      | |      | |      |      |  |              |      |  |    |  |        |    |_            " << endl;
	cout << "                |______| |      | |      |      |  |______        |______|   |__|   |______  |      |           " << endl;
	cout << endl << endl << endl << endl;
	cout << "                                          Press any key to go back to menu.";
	_getch();
}
void updateScore() {
	gotoxy(WIN_WIDTH + 7, 14); cout << "Score: " << score << endl;
}

void play() {
	if (score < 50)
	{
		birdPos = -1 + WIN_WIDTH / 2;
		score = 0;
		enemyFlag[0] = 1;
		enemyFlag[1] = 1;
		enemyY[0] = enemyY[1] = 4;

		for (int i = 0; i < 20; i++) {
			bullets[i][0] = bullets[i][1] = 0;
		}

		system("cls");
		drawBorder();
		genEnemy(0);
		genEnemy(1);
		updateScore();

		gotoxy(WIN_WIDTH + 5, 2); cout << "Space Shooter";
		gotoxy(WIN_WIDTH + 1, 10); cout << "___________________";
		gotoxy(WIN_WIDTH + 1, 19); cout << "Press Ecs to exit ";
		gotoxy(WIN_WIDTH + 1, 17); cout << "___________________";
		gotoxy(10, 5); cout << "Press any key to start";
		_getch();
		gotoxy(10, 5); cout << "                      ";

		while (1) {
			if (_kbhit()) {
				char ch = _getch();
				if (ch == 'a' || ch == 'A') {
					if (birdPos > 2)
						birdPos -= 2;
				}
				if (ch == 'd' || ch == 'D') {
					if (birdPos < WIN_WIDTH - 7)
						birdPos += 2;
				}
				if (ch == 32) {
					genBullet();
				}
				if (ch == 27) {
					break;
				}
			}

			drawBird();
			drawEnemy(0);
			drawEnemy(1);
			drawBullets();
			if (collision() == 1) {
					gameover();
					return;
			}
			if (bulletHit() == 1) {
				score++;
				updateScore();
			}
			Sleep(200);
			eraseBird();
			eraseEnemy(0);
			eraseEnemy(1);
			eraseBullets();
			moveBullet();

			if (enemyFlag[0] == 1)
				enemyY[0] += 1;

			if (enemyFlag[1] == 1)
				enemyY[1] += 1;

			if (enemyY[0] > SCREEN_HEIGHT - 5) {
				resetEnemy(0);
			}
			if (enemyY[1] > SCREEN_HEIGHT - 5) {
				resetEnemy(1);
			}
		}
	}
	system("COLOR 09");
}

int main()
{
	int x;
	system("COLOR 05");
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	cout << "        _____   _____   ______   ______  ______       _____            ______   ______  _______  ______  ______     " << endl;
	cout << "       |       |     | |      | |       |            |       |      | |      | |      |    |    |       |      |    " << endl;
	cout << "       |_____  |_____| |______| |       |_____       |_____  |______| |      | |      |    |    |_____  |______|    " << endl;
	cout << "             | |       |      | |       |                  | |      | |      | |      |    |    |       |   |__     " << endl;
	cout << "       ______| |       |      | |______ |______      ______| |      | |______| |______|    |    |______ |      |    " << endl;
	cout << endl << endl << endl;
	cout << "                                               PRESS ANY BUTTON TO CONTINUE" << endl;
	_getch();
	system("cls");
	setcursor(0, 0);
	srand((unsigned)time(NULL));

	do {
		system("COLOR 09");
		system("cls");
		gotoxy(10, 5); cout << endl;
		gotoxy(10, 7); cout << endl << endl << endl;
		gotoxy(10, 9); cout << "                                                 Instructions" << endl << endl << endl;
		gotoxy(10, 10); cout << "                                     Use arrow keys (A and D) to move around" << endl;
		gotoxy(10, 11); cout << "                                     Press Space to fire Bullets" << endl;
		gotoxy(10, 13); cout << "                                     Press Esc during game to reach main menu " << endl << endl << endl;
		gotoxy(10, 14); cout << "				          Press Space key to start level 1  " << endl;
		char op = _getche();
		if (op == 32) play();

	} while (1);

	return 0;
}