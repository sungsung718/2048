#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define SIZE 4
#define END 2048

enum OPTION {START = 1, RULE, RANK, EXIT};

void menu(void);
void playGame(void);
void printRule(void);
void printRank(void);
void printTable(const int[SIZE][SIZE]);
void initTable(int[SIZE][SIZE]);
int isOverOrClear(const int[SIZE][SIZE]);
int move(int[SIZE][SIZE]);
void makeNum(int[SIZE][SIZE]);

int main(void) {
	int option;

	while (1) {
		menu();
		if (scanf(" %d", &option) == 0) {
			rewind(stdin);
			printf("[Enter] digit number!\n");
		}
		else if (option == START) playGame();
		else if (option == RULE) printRule();
		else if (option == RANK) printRank();
		else if (option == EXIT) break;
		else printf("[Enter] number 1~4!\n");
	}
	return 0;
}

void menu(void) {
	printf("\n");
	printf("Type the following number and press the [Enter].\n");
	printf("1. Game Start\n");
	printf("2. How to Play\n");
	printf("3. Rank\n");
	printf("4. Exit\n\n");
}

void playGame(void) {
	int ret;
	int table[SIZE][SIZE] = { {0, }, };

	system("cls");
	initTable(table);
	printTable(table);
	
	while (1) {
		if (ret = move(table) == 0) {
			printf("Not allowed\n");
			Sleep(1000);
		}
		else {
			makeNum(table);
		}
		system("cls");
		printTable(table);
		if (isOverOrClear(table)) break;
	}

}

void printRule(void) {
	system("cls");
	printf("\n");
	printf("HOW TO PLAY: Use W,A,S,D to move the tiles.\n");
	printf("Tiles with the same number merge into one when they touch.\n");
	printf("Add them up to reach 2048!\n");
	printf("----------------------------------\n\n");
}

void printRank(void) {
	system("cls");
	return;
}

void printTable(const int table[SIZE][SIZE]) {
	int i, j, num;

	printf("\n\n");
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (num = table[i][j]) printf("\t%d\t|", num);
			else printf("\t \t|");
		}
		printf("\n\n\n");
	}
}

int isOverOrClear(const int table[SIZE][SIZE]) {
	int i, j, num, full=1;

	//isClear
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (table[i][j] == END) {
				printf("Game Clear\n");
				Sleep(1000);
				return 1;
			}
			if (table[i][j]==0) full = 0;
		}
	}

	//isOver
	if (full) {
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				num = table[i][j];
				if (j == 0) {
					if (num == table[i][j + 1]) return 0;
				}
				else if (j == SIZE - 1) {
					if (num == table[i][j - 1]) return 0;
				}
				else if (i == 0) {
					if (num == table[i + 1][j]) return 0;
				}
				else if (i == SIZE - 1) {
					if (num == table[i - 1][j]) return 0;
				}
				else {
					if ((num == table[i][j + 1]) || (num == table[i][j - 1])) return 0;
					if ((num == table[i + 1][j]) || (num == table[i - 1][j])) return 0;
				}
			}
		}
	} //full인 상태에서 인접한 값이 같으면 game over나 clear가 아니다.
	else return 0; //full이 아니면 game over나 clear가 아니다.

	printf("Game over\n");
	Sleep(1000);
	return 1;
}

void initTable(int table[SIZE][SIZE]) {
	int cnt=0, row, col, num;
	
	srand((unsigned int)time(NULL));
	while (cnt < 2) {
		num = rand() % 10;
		row = rand() % 4;
		col = rand() % 4;
		if (!table[row][col]) {
			if (num) table[row][col] = 2;
			else table[row][col] = 4;
			cnt++;
		}		
	}
}

int move(int table[SIZE][SIZE]) {
	char c;
	int i, j, k;
	int moved = 0;

	c = _getch();
	if (c == 'w') {
		for (j = 0; j < SIZE; j++) {
			for (i = 1; i < SIZE; i++) {
				if (table[i][j]) {
					for (k = 0; k < i; k++) {
						if (table[k][j] == 0) {
							table[k][j] = table[i][j];
							table[i][j] = 0;
							moved = 1;
							break;
						}
					}
				}
			}
		}
		for (j = 0; j < SIZE; j++) {
			for (i = 1; i < SIZE; i++) {
				if (table[i][j] && (table[i][j] == table[i - 1][j])) {
					moved = 1;
					table[i - 1][j] *= 2;
					table[i][j] = 0;
					if (i == SIZE - 1) table[i][j] = 0;
					else {
						for (k = i+1; k < SIZE; k++) {
							if (table[k][j] != 0) {
								table[k-1][j] = table[k][j];
								table[k][j] = 0;
							}
						}
					}
					
				}
			}
		}

		if (moved) return 1;
		return 0;
	}

	else if (c=='s') {
		for (j = 0; j < SIZE; j++) {
			for (i = SIZE-2; i >= 0; i--) {
				if (table[i][j]) {
					for (k = SIZE-1; k > i; k--) {
						if (table[k][j] == 0) {
							table[k][j] = table[i][j];
							table[i][j] = 0;
							moved = 1;
							break;
						}
					}
				}
			}
		}
		for (j = 0; j < SIZE; j++) {
			for (i = SIZE - 2; i >= 0; i--) {
				if (table[i][j] && (table[i][j] == table[i + 1][j])) {
					moved = 1;
					table[i + 1][j] *= 2;
					table[i][j] = 0;
					if (i == 0) table[i][j] = 0;
					else {
						for (k = i - 1; k >= 0; k--) {
							if (table[k][j] != 0) {
								table[k + 1][j] = table[k][j];
								table[k][j] = 0;
							}
						}
					}
					
				}
			}
		}

		if (move) return 1;
		return 0;
	}

	else if (c=='a') {
		for (i = 0; i < SIZE; i++) {
			for (j = 1; j < SIZE; j++) {
				if (table[i][j]) {
					for (k = 0; k < j; k++) {
						if (table[i][k] == 0) {
							table[i][k] = table[i][j];
							table[i][j] = 0;
							moved = 1;
							break;
						}
					}
				}
			}
		}
		for (i = 0; i < SIZE; i++) {
			for (j = 1; j < SIZE; j++) {
				if ((table[i][j]) && (table[i][j] == table[i][j-1])) {
					moved = 1;
					table[i][j-1] *= 2;
					table[i][j] = 0;
					if (j == SIZE - 1) table[i][j] = 0;
					else {
						for (k = j + 1; k < SIZE; k++) {
							if (table[i][k] != 0) {
								table[i][k - 1] = table[i][k];
								table[i][k] = 0;
							}
						}
					}
					
				}
			}
		}

		if (moved) return 1;
		return 0;
	}

	else if (c=='d') {
		for (i = 0; i < SIZE; i++) {
			for (j = SIZE - 2; j >= 0; j--) {
				if (table[i][j]) {
					for (k = SIZE - 1; k > j; k--) {
						if (table[i][k] == 0) {
							table[i][k] = table[i][j];
							table[i][j] = 0;
							moved = 1;
							break;
						}
					}
				}
			}
		}
		for (i = 0; i < SIZE; i++) {
			for (j = SIZE - 2; j >= 0; j--) {
				if (table[i][j] && (table[i][j] == table[i][j+1])) {
					moved = 1;
					table[i][j+1] *= 2;
					table[i][j] = 0;
					if (j == 0) table[i][j] = 0;
					else {
						for (k = j - 1; k >= 0; k--) {
							if (table[i][k] != 0) {
								table[i][k + 1] = table[i][k];
								table[i][k] = 0;
							}
						}
					}
				}
			}
		}

		if (moved) return 1;
		return 0;
	}

	return 0;
}

void makeNum(int table[SIZE][SIZE]) {
	int blank = 0;
	int cnt = 0;
	int i, j, idx, num;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (table[i][j] == 0) blank++;
		}
	}

	idx = rand()%blank + 1;
	num = rand() % 10;
	
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (table[i][j] == 0) {
				if (++cnt == idx) {
					if (num) table[i][j] = 2; //2가 나올 확률 90%
					else table[i][j] = 4; //4가 나올 확률 10%
					break;
				}
			}
		}
	}
}