#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define MAX_FILE_NAME 50
#define MAX_PLAYERS_NAME 50
#define MAX_CELL_ENERGY 100
#define MAX_BLOCK_ENERGY 100

struct player
{
	int score, ncells;
	char key;
	char name[MAX_PLAYERS_NAME];
	int* cell_energy;
};

struct block
{
	int row, column, type, energy;
	char cell_name[4];
	struct block* next;
};