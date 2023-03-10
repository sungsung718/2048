#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define SIZE 4
#define END 2048
#define TIME_LIMIT 600
#define NAME_LIMIT 64
#define USER_LIMIT 100

enum OPTION { START = 1, RULE, RANK, EXIT };

typedef struct User {
	char name[NAME_LIMIT];
	int isClear;
	int score;
	int numMove;
	int combo;
	time_t playTime;
} USER;

void menu(void);
void playGame(FILE*);
void printRule(void);
void printRank(FILE*);
void printTable(const int[SIZE][SIZE], const int*, const int*, const int*, const int*);
void printTime(const time_t);
void initTable(int[SIZE][SIZE]);
int isOverOrClear(const int[SIZE][SIZE]);
int move(int[SIZE][SIZE], int*, int*, int*);
void makeNum(int[SIZE][SIZE]);
void getName(char*);
USER getUser(const char*, int, int, int, int, time_t);
void addRecord(FILE*, USER);
int readRecord(FILE* fp, USER user[]);
int cmpScore(const void* a, const void* b);

int main(void) {
	char input;
	FILE* fp;

	fp = fopen("data.txt", "a+");
	if (fp == NULL) {
		printf("data.txt 파일을 열 수 없습니다.");
		exit(1);
	}

	while (1) {
		menu();
		input = _getch();
		if (!isdigit(input)) {
			printf("[Enter] digit number!\n");
		}
		else {
			int option = atoi(&input);
			if (option == START) playGame(fp);
			else if (option == RULE) printRule();
			else if (option == RANK) printRank(fp);
			else if (option == EXIT) break;
			else printf("[Enter] number 1~4!\n");
		}

	}
	fclose(fp);
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

void playGame(FILE* fp) {
	int ret, result;
	int table[SIZE][SIZE] = { {0, }, };
	int point = 0, combo = 0, max_combo = 0, num_move = 0;
	char name[NAME_LIMIT];
	time_t start_time;
	USER user;

	system("cls");
	initTable(table);
	start_time = time(NULL);
	printf("\n\tRemaining time: ");
	printTime(start_time);
	printf("\n");
	printTable(table, &point, &combo, &max_combo, &num_move);


	while (1) {
		if (ret = move(table, &point, &combo, &num_move) == 0) {
			printf("Not allowed\n");
			Sleep(1000);
		}
		else {
			makeNum(table);
		}
		max_combo = (combo > max_combo) ? combo : max_combo;
		system("cls");
		printf("\n\tRemaining time: ");
		printTime(start_time);
		printf("\n");
		printTable(table, &point, &combo, &max_combo, &num_move);
		if (time(NULL) - start_time > TIME_LIMIT) {
			printf("Time Over\n");
			Sleep(1000);
			getName(name);
			user = getUser(name, 0, point, num_move, max_combo, TIME_LIMIT);
			addRecord(fp, user);
			break;
		}
		if (result = isOverOrClear(table) == 1) {
			getName(name);
			user = getUser(name, 0, point, num_move, max_combo, time(NULL) - start_time);
			addRecord(fp, user);
			break;
		}
		if (result == 2) {
			getName(name);
			user = getUser(name, 1, point, num_move, max_combo, time(NULL) - start_time);
			addRecord(fp, user);
			break;
		}
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

void printRank(FILE* fp) {
	USER list[USER_LIMIT];
	int len, i;

	
	len = readRecord(fp, list)-1;
	qsort(list, len, sizeof(USER), cmpScore);

	system("cls");
	printf("\n");
	printf("Rank\tName\tScore\tMoves\tCombo\tTime(s)\tResult\n");
	for (i = 0; i < len; i++) {
		printf("%d\t%s\t%d\t%d\t%d\t%d\t", i+1, list[i].name, list[i].score, list[i].numMove, list[i].combo, list[i].playTime);
		if (list[i].isClear) printf("Game Clear\n");
		else printf("Game Over\n");
	}
}

void printTable(const int table[SIZE][SIZE], const int* point, const int* combo, const int* max_combo, const int* num_move) {
	int i, j, num;

	printf("\n");
	printf("\tScore: %d\tCombo: %d\tMax combo: %d\tNumber of moves: %d", *point, *combo, *max_combo, *num_move);
	printf("\n\n\n");
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (num = table[i][j]) printf("\t%d\t|", num);
			else printf("\t \t|");
		}
		printf("\n\n\n");
	}
}

void printTime(const time_t start_time) {
	int min = 0, sec = 0;
	time_t current_time = time(NULL);
	time_t rem_sec = 600 - (current_time - start_time);

	min = rem_sec / 60;
	sec = rem_sec % 60;

	printf("%02d:%02d", min, sec);
}

int isOverOrClear(const int table[SIZE][SIZE]) {
	int i, j, num, full = 1;

	//isClear
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (table[i][j] == END) {
				printf("Game Clear\n");
				Sleep(1000);
				return 2;
			}
			if (table[i][j] == 0) full = 0;
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
	}
	else return 0;

	printf("Game over\n");
	Sleep(1000);
	return 1;
}

void initTable(int table[SIZE][SIZE]) {
	int cnt = 0, row, col, num;

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

int move(int table[SIZE][SIZE], int* point, int* combo, int* num_move) {
	char c;
	int i, j, k;
	int moved = 0;
	int merged = 0;

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
					merged = 1;
					*point += table[i][j] * 2;
					++* combo;
					table[i - 1][j] *= 2;
					table[i][j] = 0;
					if (i == SIZE - 1) table[i][j] = 0;
					else {
						for (k = i + 1; k < SIZE; k++) {
							if (table[k][j] != 0) {
								table[k - 1][j] = table[k][j];
								table[k][j] = 0;
							}
						}
					}
				}
			}
		}
		if (!merged) *combo = 0;
		if (moved) {
			++* num_move;
			return 1;
		}
		return 0;
	}

	else if (c == 's') {
		for (j = 0; j < SIZE; j++) {
			for (i = SIZE - 2; i >= 0; i--) {
				if (table[i][j]) {
					for (k = SIZE - 1; k > i; k--) {
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
					merged = 1;
					*point += table[i][j] * 2;
					++* combo;
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
		if (!merged) *combo = 0;
		if (moved) {
			++* num_move;
			return 1;
		}
		return 0;
	}

	else if (c == 'a') {
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
				if ((table[i][j]) && (table[i][j] == table[i][j - 1])) {
					moved = 1;
					merged = 1;
					*point += table[i][j] * 2;
					++* combo;
					table[i][j - 1] *= 2;
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

		if (!merged) *combo = 0;
		if (moved) {
			++* num_move;
			return 1;
		}
		return 0;
	}

	else if (c == 'd') {
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
				if (table[i][j] && (table[i][j] == table[i][j + 1])) {
					moved = 1;
					merged = 1;
					*point += table[i][j] * 2;
					++* combo;
					table[i][j + 1] *= 2;
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
		if (!merged) *combo = 0;
		if (moved) {
			++* num_move;
			return 1;
		}
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

	idx = rand() % blank + 1;
	num = rand() % 10;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (table[i][j] == 0) {
				if (++cnt == idx) {
					if (num) table[i][j] = 2;
					else table[i][j] = 4;
					break;
				}
			}
		}
	}
}

void getName(char* name) {
	char c;
	printf("Username: ");
	rewind(stdin);
	scanf(" %s", name);
	while (c = getchar() != '\n');
	system("cls");
}

USER getUser(const char* name, int isClear, int score, int numMove, int combo, time_t playTime) {
	USER user;
	strcpy(user.name, name);
	user.isClear = isClear;
	user.score = score;
	user.numMove = numMove;
	user.combo = combo;
	user.playTime = playTime;

	return user;
}

void addRecord(FILE* fp, USER user) {
	fseek(fp, 0L, SEEK_END);
	fputs(user.name, fp);
	fprintf(fp, " %d %d %d %d %d\n", user.isClear, user.score, user.numMove, user.combo, user.playTime);
}

int readRecord(FILE* fp, USER user[]) {
	int i = 0;
	
	fseek(fp, 0L, SEEK_SET);
	while (!feof(fp)) {
		fscanf(fp, "%s %d %d %d %d %d", user[i].name,  &user[i].isClear, &user[i].score, & user[i].numMove, &user[i].combo, &user[i].playTime);
		i++;
	}

	return i;
}

int cmpScore(const void* a, const void* b) {
	return -(((USER*)a)->score - ((USER*)b)->score);
}