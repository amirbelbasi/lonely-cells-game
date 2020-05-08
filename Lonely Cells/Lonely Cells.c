#include "myheader.h"

void first_place(struct block* header, struct player p, int n);
void show_map(struct block* header, int n);
void PVE(struct block* header, int n, struct player* passed_p);
void PVP(struct block* header, int n, struct player* passed_p);
void game(struct block* header, struct player* passed_p, int mode);
void blue();//Energy
void green();//Mitosis
void red();//Forbidden
void reset();
void map_editor();
int PVE_save_game(struct block* header, struct player p);
void PVE_load_game(struct block* header, struct player* passed_p, int* mode);
void PVP_load_game(struct block* header, struct player* passed_p, int* mode);
int PVE_cell_menu(struct block* header, struct player* p, int n);
int PVP_cell_menu(struct block* header, struct player* p1, struct player* p2, int n);
int load_map(struct block* header);
int player_score(int* cell_energy, int ncells);

int main()
{
	struct block* header = (struct block*) malloc(sizeof(struct block));
	time_t t = time(NULL);
	srand(t);
	printf("Lonely Cells\npress enter to continue");
	getchar();
	system("cls");
	while (true)
	{
		int inp;
		system("cls");
		printf("choose one of below:\n1) start a new game\n2) load a game\n3) create a map\n4) exit game\n");
		scanf_s("%d", &inp);
		system("cls");
		if (inp == 1)
		{
			if (load_map(header) == 1) continue;
			game(header, NULL, 0);
		}
		else if (inp == 2)
		{
			int mode;
			struct player passed_p;
			PVE_load_game(header, &passed_p, &mode);
			if (mode == 3) continue;
			game(header, &passed_p, mode);
		}
		else if (inp == 3) map_editor();
		else if (inp == 4) break;
		else
		{
			printf("invalid input");
			Sleep(1500);
			system("cls");
		}
	}
	return 0;
}

void game(struct block* header, struct player* passed_p, int mode)
{
	int inp, n;
	struct block* current = header;
	for (int i = 1;; i++)
	{
		if (current == NULL)
		{
			n = sqrt(i);
			break;
		}
		current = current->next;
	}
	if (passed_p == NULL)
	{
		system("cls");
		printf("choose game mode:\n1) PVE\n2) PVP\n");
		scanf_s("%d", &inp);
		system("cls");
		if (inp == 1) PVE(header, n, passed_p);
		else if (inp == 2) PVP(header, n, passed_p);
		else
		{
			system("cls");
			printf("invalid input");
			Sleep(1500);
		}
	}
	else if (mode == 1) PVE(header, n, passed_p);
	else if (mode == 2) PVP(header, n, passed_p);
}

void PVE(struct block* header, int n, struct player* passed_p)
{
	struct player p;
	if (passed_p == NULL)
	{
		int possible_blocks = 0;
		struct block* current = header;
		getchar();
		printf("enter your name: ");
		gets_s(p.name, MAX_PLAYERS_NAME);
		printf("enter a key character to be shown as your cells in the map: ");
		scanf_s("%c", &p.key);
		printf("enter number of your cells: ");
		scanf_s("%d", &p.ncells);
		system("cls");
		getchar();
		p.cell_energy = (int*)malloc(sizeof(int) * p.ncells);
		p.score = 0;
		for (int i = 0; i < p.ncells; i++)
		{
			p.cell_energy[i] = 0;
		}
		while (current != NULL)
		{
			if (current->type != 3) possible_blocks++;
			current = current->next;
		}
		if (possible_blocks < p.ncells)
		{
			system("cls");
			printf("not enough block available");
			Sleep(1500);
			return;
		}
		first_place(header, p, n);
		while (true)
		{
			if (PVE_cell_menu(header, &p, n) == 1) break;
		}
	}
	else
	{
		while (true)
		{
			if (PVE_cell_menu(header, passed_p, n) == 1) break;
		}
	}
}

void PVP(struct block* header, int n, struct player* passed_p)
{
	if (passed_p == NULL)
	{
		int possible_blocks = 0;
		struct block* current = header;
		struct player p1, p2;
		getchar();
		printf("player 1 enter your name: ");
		gets_s(p1.name, MAX_PLAYERS_NAME);
		printf("player 1 enter a key character to be shown as your cells in the map: ");
		scanf_s("%c", &p1.key);
		printf("player 1 enter number of your cells: ");
		scanf_s("%d", &p1.ncells);
		getchar();
		p1.cell_energy = (int*)malloc(sizeof(int) * p1.ncells);
		p1.score = 0;
		for (int i = 0; i < p1.ncells; i++)
		{
			p1.cell_energy[i] = 0;
		}
		while (current != NULL)
		{
			if (current->type != 3) possible_blocks++;
			current = current->next;
		}
		if (possible_blocks < p1.ncells)
		{
			system("cls");
			printf("not enough block available");
			Sleep(1500);
			return;
		}
		first_place(header, p1, n);
		current = header;
		printf("player 2 enter your name: ");
		gets_s(p2.name, MAX_PLAYERS_NAME);
		printf("player 2 enter a key character to be shown as your cells in the map: ");
		scanf_s("%c", &p2.key);
		printf("player 2 enter number of your cells: ");
		scanf_s("%d", &p2.ncells);
		system("cls");
		getchar();
		p2.cell_energy = (int*)malloc(sizeof(int) * p2.ncells);
		p2.score = 0;
		for (int i = 0; i < p2.ncells; i++)
		{
			p2.cell_energy[i] = 0;
		}
		while (current != NULL)
		{
			if (current->type != 3) possible_blocks++;
			current = current->next;
		}
		if (possible_blocks < p2.ncells + p1.ncells)
		{
			system("cls");
			printf("not enough block available");
			Sleep(1500);
			return;
		}
		first_place(header, p2, n);
		while (true)
		{
			if (PVP_cell_menu(header, &p1, &p2, n) == 1) break;
		}
	}
	else;
}

void first_place(struct block* header, struct player p, int n)
{
	int index = 0;
	int* arr = (int*)malloc(sizeof(int) * p.ncells);
	for (int i = 0; i < p.ncells; i++)
	{
		arr[i] = -1;
	}
	while (true)
	{
		struct block* current = header;
		int flag = 0;
		int a = rand() % (n * n);
		for (int i = 0; i < p.ncells; i++)
		{
			if (arr[i] == a) flag++;
		}
		if (flag == 0)
		{
			for (int i = 0; i < n * n; i++)
			{
				if (i == a)
				{
					if (current->type == 3 || current->cell_name[0] != '-')
					{
						arr[index] = a;
						break;
					}
					current->cell_name[0] = p.key;
					current->cell_name[1] = (index / 10) + 48;
					current->cell_name[2] = (index % 10) + 48;
					arr[index] = a;
					index++;
					break;
				}
				current = current->next;
			}
		}
		if (index >= p.ncells) break;
	}
}

void show_map(struct block* header, int n)
{
	printf(" ");
	for (int i = 0; i < ceil((double)n / 2); i++)
	{
		printf("___     ");
	}
	printf("\n");
	struct block* current = header;
	for (int i = 0; i < n; i++)
	{
		int j;
		struct block* holder = current;
		for (j = 0; j < floor((double)n / 2); j++)
		{
			printf("/");
			if (current->type == 1) blue();
			else if (current->type == 2) green();
			else if (current->type == 3)red();
			else;
			printf("%s", current->cell_name);
			reset();
			printf("\\___");
			if (j < ceil((double)n / 2) - 1) current = current->next->next;
		}
		if (n % 2 == 0)
		{
			if (i > 0) printf("/");
		}
		else
		{
			printf("/");
			if (current->type == 1) blue();
			else if (current->type == 2) green();
			else if (current->type == 3)red();
			else;
			printf("%s", current->cell_name);
			reset();
			printf("\\");
		}
		printf("\n");
		current = holder->next;
		for (j = 0; j < floor((double)n / 2); j++)
		{
			printf("\\___/");
			if (current->type == 1) blue();
			else if (current->type == 2) green();
			else if (current->type == 3)red();
			else;
			printf("%s", current->cell_name);
			reset();
			if (j < floor((double)n / 2) - 1) current = current->next->next;
		}
		if (n % 2 == 0) printf("\\");
		else printf("\\___/");
		printf("\n");
		current = header;
		for (j = 0; j < (n * (i + 1)); j++)
		{
			current = current->next;
		}
	}
	printf(" ");
	for (int i = 0; i < floor((double)n / 2); i++)
	{
		printf("   \\___/");
	}
	printf("\n");
}

void blue()
{
	printf("\033[1;34m");
}

void green()
{
	printf("\033[1;32m");
}

void red()
{
	printf("\033[1;31m");
}

void reset()
{
	printf("\033[0m");
}

int PVE_save_game(struct block* header, struct player p)
{
	int n;
	FILE* fp;
	struct block* current = header;
	for (n = 0; current != NULL; n++)
	{
		current = current->next;
	}
	current = header;
	char file_name[MAX_FILE_NAME];
	system("cls");
	printf("eneter save file name: ");
	getchar();
	gets_s(file_name, MAX_FILE_NAME);
	fopen_s(&fp, file_name, "wb");
	if (fp == NULL)
	{
		system("cls");
		printf("can not create %s", file_name);
		Sleep(1500);
		return 1;
	}
	fwrite(&n, sizeof(int), 1, fp);
	while (current != NULL)
	{
		fwrite(&current->row, sizeof(int), 1, fp);
		fwrite(&current->column, sizeof(int), 1, fp);
		fwrite(&current->type, sizeof(int), 1, fp);
		fwrite(&current->energy, sizeof(int), 1, fp);
		fwrite(&current->cell_name[0], sizeof(char), 1, fp);
		fwrite(&current->cell_name[1], sizeof(char), 1, fp);
		fwrite(&current->cell_name[2], sizeof(char), 1, fp);
		fwrite(&current->cell_name[3], sizeof(char), 1, fp);
		current = current->next;
	}
	fwrite(&p.score, sizeof(int), 1, fp);
	fwrite(&p.ncells, sizeof(int), 1, fp);
	fwrite(&p.key, sizeof(char), 1, fp);
	int j = 0;
	char* N = p.name;
	while (true)
	{
		fwrite(&p.name[j], sizeof(char), 1, fp);
		if (p.name[j] == '\0') break;
		j++;
	}
	for (int i = 0; i < p.ncells; i++)
	{
		fwrite(&p.cell_energy[i], sizeof(int), 1, fp);
	}
	fclose(fp);
	return 0;
}

void PVE_load_game(struct block* header, struct player* passed_p, int* mode)
{
	int n;
	FILE* fp;
	struct block* current = header;
	char file_name[MAX_FILE_NAME];
	system("cls");
	printf("eneter load file name: ");
	getchar();
	gets_s(file_name, MAX_FILE_NAME);
	fopen_s(&fp, file_name, "rb");
	if (fp == NULL)
	{
		system("cls");
		printf("file %s not found", file_name);
		Sleep(1500);
		*mode = 3;
		return;
	}
	fread(&n, sizeof(int), 1, fp);
	for (int i = 0; i < n; i++)
	{
		fread(&current->row, sizeof(int), 1, fp);
		fread(&current->column, sizeof(int), 1, fp);
		fread(&current->type, sizeof(int), 1, fp);
		fread(&current->energy, sizeof(int), 1, fp);
		fread(&current->cell_name[0], sizeof(char), 1, fp);
		fread(&current->cell_name[1], sizeof(char), 1, fp);
		fread(&current->cell_name[2], sizeof(char), 1, fp);
		fread(&current->cell_name[3], sizeof(char), 1, fp);
		current->next = (struct block*) malloc(sizeof(struct block));
		if (i == n - 1)
		{
			current->next = NULL;
			break;
		}
		current = current->next;
	}
	current->next = NULL;
	fread(&passed_p->score, sizeof(int), 1, fp);
	fread(&passed_p->ncells, sizeof(int), 1, fp);
	fread(&passed_p->key, sizeof(char), 1, fp);
	int j = 0;
	char* N = passed_p->name;
	while (true)
	{
		fread(&passed_p->name[j], sizeof(char), 1, fp);
		if (passed_p->name[j] == '\0') break;
		j++;
	}
	passed_p->cell_energy = (int*)malloc(sizeof(int) * passed_p->ncells);
	for (int i = 0; i < passed_p->ncells; i++)
	{
		fread(&passed_p->cell_energy[i], sizeof(int), 1, fp);
	}
	fclose(fp);
	*mode = 1;
}

void PVP_load_game(struct block* header, struct player* passed_p, int* mode)
{
	int n;
	FILE* fp;
	struct block* current = header;
	char file_name[MAX_FILE_NAME];
	system("cls");
	printf("eneter load file name: ");
	getchar();
	gets_s(file_name, MAX_FILE_NAME);
	fopen_s(&fp, file_name, "rb");
	if (fp == NULL)
	{
		system("cls");
		printf("%s not found", file_name);
		Sleep(1500);
		*mode = 3;
		return;
	}
	fread(&n, sizeof(int), 1, fp);
	for (int i = 0; i < n; i++)
	{
		fread(&current->row, sizeof(int), 1, fp);
		fread(&current->column, sizeof(int), 1, fp);
		fread(&current->type, sizeof(int), 1, fp);
		fread(&current->energy, sizeof(int), 1, fp);
		fread(&current->cell_name[0], sizeof(char), 1, fp);
		fread(&current->cell_name[1], sizeof(char), 1, fp);
		fread(&current->cell_name[2], sizeof(char), 1, fp);
		fread(&current->cell_name[3], sizeof(char), 1, fp);
		current->next = (struct block*) malloc(sizeof(struct block));
		if (i == n - 1)
		{
			current->next = NULL;
			break;
		}
		current = current->next;
	}
	current->next = NULL;
	fread(&passed_p->score, sizeof(int), 1, fp);
	fread(&passed_p->ncells, sizeof(int), 1, fp);
	fread(&passed_p->key, sizeof(char), 1, fp);
	int j = 0;
	char* N = passed_p->name;
	while (passed_p->name[j] != '\0')
	{
		fread(&passed_p->name[j], sizeof(char), 1, fp);
		j++;
	}
	for (int i = 0; i < passed_p->ncells; i++)
	{
		fread(&passed_p->cell_energy[i], sizeof(int), 1, fp);
	}
	fclose(fp);
}

void PVP_save_game(struct block* header)
{
	char file_name[MAX_FILE_NAME];
	printf("eneter save file name: ");
	gets_s(file_name, MAX_FILE_NAME);
	//fwrite()
}

int load_map(struct block* header)
{
	int n;
	char map_name[MAX_FILE_NAME];
	FILE* mapfp;
	struct block* current = header;
	printf("enter map file name: ");
	getchar();
	gets_s(map_name, MAX_FILE_NAME);
	fopen_s(&mapfp, map_name, "rb");
	if (mapfp == NULL)
	{
		system("cls");
		printf("file %s not found", map_name);
		Sleep(1500);
		return 1;
	}
	fread(&n, 4, 1, mapfp);
	for (int i = 0; i < n * n; i++)
	{
		current->next = (struct block*) malloc(sizeof(struct block));
		if (i == n * n - 1) current->next = NULL;
		current = current->next;
	}
	current = header;
	for (int r = 0; r < n; r++)
	{
		char holder;
		for (int c = 0; c < n; c++)
		{
			fread(&holder, 1, 1, mapfp);
			current->row = r;
			current->column = c;
			current->type = holder - 48;
			current = current->next;
		}
	}
	current = header;
	while (current != NULL)
	{
		current->cell_name[0] = '-';
		current->cell_name[1] = '-';
		current->cell_name[2] = '-';
		current->cell_name[3] = '\0';
		current->energy = 0;
		if (current->type == 1) current->energy = MAX_BLOCK_ENERGY;
		current = current->next;
	}
	fclose(mapfp);
	return 0;
}

int player_score(int* cell_energy, int ncells)
{
	int res = 0;
	for (int i = 0; i < ncells; i++)
	{
		res += cell_energy[i];
	}
	return res;
}

int PVE_cell_menu(struct block* header, struct player* p, int n)
{
	int* Arr = (int*)malloc(sizeof(int) * p->ncells);
	int Inp, inp, Ncells = p->ncells, Flagg = 0, index = 0, Index = 0;
	struct block* cell_current = header;
	for (int i = 0; i < p->ncells; i++)
	{
		Arr[i] = -1;
	}
	while (true)
	{
		int cell_num, end;
		cell_current = header;
		if (Index == Ncells) break;
		system("cls");
		show_map(header, n);
		printf("%s enter your cell number: ", p->name);
		scanf_s("%d", &cell_num);
		while (cell_current != NULL)
		{
			end = 0;
			Flagg = 0;
			if (cell_current->cell_name[2] != (cell_num % 10) + 48 || cell_current->cell_name[1] != (cell_num / 10) + 48)
			{
				cell_current = cell_current->next;
				continue;
			}
			for (int i = 0; i < p->ncells; i++)
			{
				if (Arr[i] == cell_num) Flagg++;
			}
			if (Flagg > 0 || cell_num >= Ncells) break;
			while (true)
			{
				int flag = 0;
				struct block* current = header;
				system("cls");
				show_map(header, n);
				int b = ((cell_current->cell_name[1] - 48) * 10) + (cell_current->cell_name[2] - 48);
				printf("%s (cell energy = %d):\n1) move cell\n2) split cell\n3) boost cell energy\n4) pass\n5) save and exit to main menu\nscore of %s: %d\n", cell_current->cell_name, p->cell_energy[b], p->name, player_score(p->cell_energy, p->ncells));
				scanf_s("%d", &Inp);
				if (Inp == 1)
				{
					system("cls");
					show_map(header, n);
					printf("%s:\n1) north\n2) south\n3) north east\n4) north west\n5) south east\n6) south west\n", cell_current->cell_name);
					scanf_s("%d", &inp);
					if (inp == 1)
					{
						if (cell_current->row - 1 < 0)
						{
							system("cls");
							show_map(header, n);
							printf("invalid move");
							Sleep(1500);
							continue;
						}
						while (current != NULL)
						{
							if (current->row == cell_current->row - 1 && current->column == cell_current->column)
							{
								if (current->cell_name[0] != '-' || current->type == 3)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									flag++;
									break;
								}
								current->cell_name[0] = cell_current->cell_name[0];
								current->cell_name[1] = cell_current->cell_name[1];
								current->cell_name[2] = cell_current->cell_name[2];
								cell_current->cell_name[0] = '-';
								cell_current->cell_name[1] = '-';
								cell_current->cell_name[2] = '-';
								break;
							}
							current = current->next;
						}
						if (flag == 0) break;
						else continue;
					}
					else if (inp == 2)
					{
						if (cell_current->row + 1 >= n)
						{
							system("cls");
							show_map(header, n);
							printf("invalid move");
							Sleep(1500);
							continue;
						}
						while (current != NULL)
						{
							if (current->row == cell_current->row + 1 && current->column == cell_current->column)
							{
								if (current->cell_name[0] != '-' || current->type == 3)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									flag++;
									break;
								}
								current->cell_name[0] = cell_current->cell_name[0];
								current->cell_name[1] = cell_current->cell_name[1];
								current->cell_name[2] = cell_current->cell_name[2];
								cell_current->cell_name[0] = '-';
								cell_current->cell_name[1] = '-';
								cell_current->cell_name[2] = '-';
								break;
							}
							current = current->next;
						}
						if (flag == 0) break;
						else continue;
					}
					else if (inp == 3)
					{
						if (cell_current->column % 2 == 0)
						{
							if (cell_current->row - 1 < 0 || cell_current->column + 1 >= n)
							{
								system("cls");
								show_map(header, n);
								printf("invalid move");
								Sleep(1500);
								continue;
							}
							while (current != NULL)
							{
								if (current->row == cell_current->row - 1 && current->column == cell_current->column + 1)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										system("cls");
										show_map(header, n);
										printf("invalid move");
										Sleep(1500);
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = cell_current->cell_name[1];
									current->cell_name[2] = cell_current->cell_name[2];
									cell_current->cell_name[0] = '-';
									cell_current->cell_name[1] = '-';
									cell_current->cell_name[2] = '-';
									break;
								}
								current = current->next;
							}
							if (flag == 0) break;
							else continue;
						}
						else
						{
							if (cell_current->column + 1 >= n)
							{
								system("cls");
								show_map(header, n);
								printf("invalid move");
								Sleep(1500);
								continue;
							}
							while (current != NULL)
							{
								if (current->row == cell_current->row && current->column == cell_current->column + 1)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										system("cls");
										show_map(header, n);
										printf("invalid move");
										Sleep(1500);
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = cell_current->cell_name[1];
									current->cell_name[2] = cell_current->cell_name[2];
									cell_current->cell_name[0] = '-';
									cell_current->cell_name[1] = '-';
									cell_current->cell_name[2] = '-';
									break;
								}
								current = current->next;
							}
							if (flag == 0) break;
							else continue;
						}
					}
					else if (inp == 4)
					{
						if (cell_current->column % 2 == 0)
						{
							if (cell_current->row - 1 < 0 || cell_current->column - 1 < 0)
							{
								system("cls");
								show_map(header, n);
								printf("invalid move");
								Sleep(1500);
								continue;
							}
							while (current != NULL)
							{
								if (current->row == cell_current->row - 1 && current->column == cell_current->column - 1)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										system("cls");
										show_map(header, n);
										printf("invalid move");
										Sleep(1500);
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = cell_current->cell_name[1];
									current->cell_name[2] = cell_current->cell_name[2];
									cell_current->cell_name[0] = '-';
									cell_current->cell_name[1] = '-';
									cell_current->cell_name[2] = '-';
									break;
								}
								current = current->next;
							}
							if (flag == 0) break;
							else continue;
						}
						else
						{
							if (cell_current->column - 1 < 0)
							{
								system("cls");
								show_map(header, n);
								printf("invalid move");
								Sleep(1500);
								continue;
							}
							while (current != NULL)
							{
								if (current->row == cell_current->row && current->column == cell_current->column - 1)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										system("cls");
										show_map(header, n);
										printf("invalid move");
										Sleep(1500);
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = cell_current->cell_name[1];
									current->cell_name[2] = cell_current->cell_name[2];
									cell_current->cell_name[0] = '-';
									cell_current->cell_name[1] = '-';
									cell_current->cell_name[2] = '-';
									break;
								}
								current = current->next;
							}
							if (flag == 0) break;
							else continue;
						}
					}
					else if (inp == 5)
					{
						if (cell_current->column % 2 == 1)
						{
							if (cell_current->row + 1 >= n || cell_current->column + 1 >= n)
							{
								system("cls");
								show_map(header, n);
								printf("invalid move");
								Sleep(1500);
								continue;
							}
							while (current != NULL)
							{
								if (current->row == cell_current->row + 1 && current->column == cell_current->column + 1)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										system("cls");
										show_map(header, n);
										printf("invalid move");
										Sleep(1500);
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = cell_current->cell_name[1];
									current->cell_name[2] = cell_current->cell_name[2];
									cell_current->cell_name[0] = '-';
									cell_current->cell_name[1] = '-';
									cell_current->cell_name[2] = '-';
									break;
								}
								current = current->next;
							}
							if (flag == 0) break;
							else continue;
						}
						else
						{
							if (cell_current->column + 1 >= n)
							{
								system("cls");
								show_map(header, n);
								printf("invalid move");
								Sleep(1500);
								continue;
							}
							while (current != NULL)
							{
								if (current->row == cell_current->row && current->column == cell_current->column + 1)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										system("cls");
										show_map(header, n);
										printf("invalid move");
										Sleep(1500);
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = cell_current->cell_name[1];
									current->cell_name[2] = cell_current->cell_name[2];
									cell_current->cell_name[0] = '-';
									cell_current->cell_name[1] = '-';
									cell_current->cell_name[2] = '-';
									break;
								}
								current = current->next;
							}
							if (flag == 0) break;
							else continue;
						}
					}
					else if (inp == 6)
					{
						if (cell_current->column % 2 == 1)
						{
							if (cell_current->row + 1 >= n || cell_current->column - 1 < 0)
							{
								system("cls");
								show_map(header, n);
								printf("invalid move");
								Sleep(1500);
								continue;
							}
							while (current != NULL)
							{
								if (current->row == cell_current->row + 1 && current->column == cell_current->column - 1)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										system("cls");
										show_map(header, n);
										printf("invalid move");
										Sleep(1500);
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = cell_current->cell_name[1];
									current->cell_name[2] = cell_current->cell_name[2];
									cell_current->cell_name[0] = '-';
									cell_current->cell_name[1] = '-';
									cell_current->cell_name[2] = '-';
									break;
								}
								current = current->next;
							}
							if (flag == 0) break;
							else continue;
						}
						else
						{
							if (cell_current->column - 1 < 0)
							{
								system("cls");
								show_map(header, n);
								printf("invalid move");
								Sleep(1500);
								continue;
							}
							while (current != NULL)
							{
								if (current->row == cell_current->row && current->column == cell_current->column - 1)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										system("cls");
										show_map(header, n);
										printf("invalid move");
										Sleep(1500);
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = cell_current->cell_name[1];
									current->cell_name[2] = cell_current->cell_name[2];
									cell_current->cell_name[0] = '-';
									cell_current->cell_name[1] = '-';
									cell_current->cell_name[2] = '-';
									break;
								}
								current = current->next;
							}
							if (flag == 0) break;
							else continue;
						}
					}
					else
					{
						system("cls");
						show_map(header, n);
						printf("invalid input");
						Sleep(1500);
					}
				}
				else if (Inp == 2)
				{
					if (cell_current->type != 2 || p->cell_energy[b] < 80)
					{
						system("cls");
						show_map(header, n);
						printf("invalid split");
						Sleep(1500);
						continue;
					}
					int arr[6] = { 0 };
					int index = 0, Flag = 0;
					while (true)
					{
						int flagg = 0;
						flag = 0;
						current = header;
						if (Flag > 0) break;
						inp = (rand() % 6) + 1;
						for (int i = 0; i < 6; i++)
						{
							if (arr[i] == inp) flagg++;
						}
						if (flagg == 0)
						{
							arr[index] = inp;
							index++;
						}
						else if (index < 6) continue;
						else
						{
							system("cls");
							show_map(header, n);
							printf("invalid split");
							Sleep(1500);
							Flag = 2;
							continue;
						}
						if (inp == 1)
						{
							if (cell_current->row - 1 < 0) continue;
							while (current != NULL)
							{
								if (current->row == cell_current->row - 1 && current->column == cell_current->column)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = (p->ncells / 10) + 48;
									current->cell_name[2] = (p->ncells % 10) + 48;
									(p->ncells)++;
									p->cell_energy = (int*)realloc(p->cell_energy, sizeof(int) * p->ncells);
									p->cell_energy[b] = 40;
									p->cell_energy[p->ncells - 1] = 40;
									break;
								}
								current = current->next;
							}
							if (flag == 0)
							{
								Flag = 1;
								break;
							}
							else continue;
						}
						else if (inp == 2)
						{
							if (cell_current->row + 1 >= n) continue;
							while (current != NULL)
							{
								if (current->row == cell_current->row + 1 && current->column == cell_current->column)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = (p->ncells / 10) + 48;
									current->cell_name[2] = (p->ncells % 10) + 48;
									(p->ncells)++;
									p->cell_energy = (int*)realloc(p->cell_energy, sizeof(int) * p->ncells);
									p->cell_energy[b] = 40;
									p->cell_energy[p->ncells - 1] = 40;
									break;
								}
								current = current->next;
							}
							if (flag == 0)
							{
								Flag = 1;
								break;
							}
							else continue;
						}
						else if (inp == 3)
						{
							if (cell_current->column % 2 == 0)
							{
								if (cell_current->row - 1 < 0 || cell_current->column + 1 >= n) continue;
								while (current != NULL)
								{
									if (current->row == cell_current->row - 1 && current->column == cell_current->column + 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = (p->ncells / 10) + 48;
										current->cell_name[2] = (p->ncells % 10) + 48;
										(p->ncells)++;
										p->cell_energy = (int*)realloc(p->cell_energy, sizeof(int) * p->ncells);
										p->cell_energy[b] = 40;
										p->cell_energy[p->ncells - 1] = 40;
										break;
									}
									current = current->next;
								}
								if (flag == 0)
								{
									Flag = 1;
									break;
								}
								else continue;
							}
							else
							{
								if (cell_current->column + 1 >= n) continue;
								while (current != NULL)
								{
									if (current->row == cell_current->row && current->column == cell_current->column + 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = (p->ncells / 10) + 48;
										current->cell_name[2] = (p->ncells % 10) + 48;
										(p->ncells)++;
										p->cell_energy = (int*)realloc(p->cell_energy, sizeof(int) * p->ncells);
										p->cell_energy[b] = 40;
										p->cell_energy[p->ncells - 1] = 40;
										break;
									}
									current = current->next;
								}
								if (flag == 0)
								{
									Flag = 1;
									break;
								}
								else continue;
							}
						}
						else if (inp == 4)
						{
							if (cell_current->column % 2 == 0)
							{
								if (cell_current->row - 1 < 0 || cell_current->column - 1 < 0) continue;
								while (current != NULL)
								{
									if (current->row == cell_current->row - 1 && current->column == cell_current->column - 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = (p->ncells / 10) + 48;
										current->cell_name[2] = (p->ncells % 10) + 48;
										(p->ncells)++;
										p->cell_energy = (int*)realloc(p->cell_energy, sizeof(int) * p->ncells);
										p->cell_energy[b] = 40;
										p->cell_energy[p->ncells - 1] = 40;
										break;
									}
									current = current->next;
								}
								if (flag == 0)
								{
									Flag = 1;
									break;
								}
								else continue;
							}
							else
							{
								if (cell_current->column - 1 < 0) continue;
								while (current != NULL)
								{
									if (current->row == cell_current->row && current->column == cell_current->column - 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = (p->ncells / 10) + 48;
										current->cell_name[2] = (p->ncells % 10) + 48;
										(p->ncells)++;
										p->cell_energy = (int*)realloc(p->cell_energy, sizeof(int) * p->ncells);
										p->cell_energy[b] = 40;
										p->cell_energy[p->ncells - 1] = 40;
										break;
									}
									current = current->next;
								}
								if (flag == 0)
								{
									Flag = 1;
									break;
								}
								else continue;
							}
						}
						else if (inp == 5)
						{
							if (cell_current->column % 2 == 1)
							{
								if (cell_current->row + 1 >= n || cell_current->column + 1 >= n) continue;
								while (current != NULL)
								{
									if (current->row == cell_current->row + 1 && current->column == cell_current->column + 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = (p->ncells / 10) + 48;
										current->cell_name[2] = (p->ncells % 10) + 48;
										(p->ncells)++;
										p->cell_energy = (int*)realloc(p->cell_energy, sizeof(int) * p->ncells);
										p->cell_energy[b] = 40;
										p->cell_energy[p->ncells - 1] = 40;
										break;
									}
									current = current->next;
								}
								if (flag == 0)
								{
									Flag = 1;
									break;
								}
								else continue;
							}
							else
							{
								if (cell_current->column + 1 >= n) continue;
								while (current != NULL)
								{
									if (current->row == cell_current->row && current->column == cell_current->column + 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = (p->ncells / 10) + 48;
										current->cell_name[2] = (p->ncells % 10) + 48;
										(p->ncells)++;
										p->cell_energy = (int*)realloc(p->cell_energy, sizeof(int) * p->ncells);
										p->cell_energy[b] = 40;
										p->cell_energy[p->ncells - 1] = 40;
										break;
									}
									current = current->next;
								}
								if (flag == 0)
								{
									Flag = 1;
									break;
								}
								else continue;
							}
						}
						else if (inp == 6)
						{
							if (cell_current->column % 2 == 1)
							{
								if (cell_current->row + 1 >= n || cell_current->column - 1 < 0) continue;
								while (current != NULL)
								{
									if (current->row == cell_current->row + 1 && current->column == cell_current->column - 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = (p->ncells / 10) + 48;
										current->cell_name[2] = (p->ncells % 10) + 48;
										(p->ncells)++;
										p->cell_energy = (int*)realloc(p->cell_energy, sizeof(int) * p->ncells);
										p->cell_energy[b] = 40;
										p->cell_energy[p->ncells - 1] = 40;
										break;
									}
									current = current->next;
								}
								if (flag == 0)
								{
									Flag = 1;
									break;
								}
								else continue;
							}
							else
							{
								if (cell_current->column - 1 < 0) continue;
								while (current != NULL)
								{
									if (current->row == cell_current->row && current->column == cell_current->column - 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = (p->ncells / 10) + 48;
										current->cell_name[2] = (p->ncells % 10) + 48;
										(p->ncells)++;
										p->cell_energy = (int*)realloc(p->cell_energy, sizeof(int) * p->ncells);
										p->cell_energy[b] = 40;
										p->cell_energy[p->ncells - 1] = 40;
										break;
									}
									current = current->next;
								}
								if (flag == 0)
								{
									Flag = 1;
									break;
								}
								else continue;
							}
						}
						else
						{
							system("cls");
							show_map(header, n);
							printf("invalid input");
							Sleep(1500);
						}
					}
					if (Flag == 1) break;
					else if (Flag == 2) continue;
				}
				else if (Inp == 3)
				{
					if (p->cell_energy[b] == 100 || cell_current->energy == 0)
					{
						system("cls");
						show_map(header, n);
						printf("invalid boost");
						Sleep(1500);
						continue;
					}
					int c = fmin(MAX_CELL_ENERGY - p->cell_energy[b], fmin(15, cell_current->energy));
					cell_current->energy -= c;
					p->cell_energy[b] += c;
					break;
				}
				else if (Inp == 4) break;
				else if (Inp == 5)
				{
					if(PVE_save_game(header, *p) == 1) continue;
					return 1;
				}
				else
				{
					system("cls");
					show_map(header, n);
					printf("invalid input");
					Sleep(1500);
					continue;
				}
			}
			cell_current = cell_current->next;
			Arr[Index] = cell_num;
			Index++;
			end++;
			break;
		}
		if (end == 0)
		{
			system("cls");
			show_map(header, n);
			printf("invalid cell");
			Sleep(1500);
		}
	}
	system("cls");
	show_map(header, n);
	printf("score of %s: %d\n", p->name, player_score(p->cell_energy, p->ncells));
	Sleep(1500);
	return 0;
}

int PVP_cell_menu(struct block* header, struct player* p1, struct player* p2, int n)
{
	int* Arr1 = (int*)malloc(sizeof(int) * p1->ncells);
	int* Arr2 = (int*)malloc(sizeof(int) * p2->ncells);
	int Inp, inp, Ncells1 = p1->ncells, Ncells2 = p2->ncells, Flagg = 0, index = 0, Index1 = 0, Index2 = 0;
	struct block* cell_current = header;
	for (int i = 0; i < p1->ncells; i++)
	{
		Arr1[i] = -1;
	}
	for (int i = 0; i < p2->ncells; i++)
	{
		Arr2[i] = -1;
	}
	for (int x = 0;; x++)
	{
		if (x % 2 == 0)
		{
			int cell_num, end;
			cell_current = header;
			if (Index1 == Ncells1) break;
			system("cls");
			show_map(header, n);
			printf("%s enter your cell number: ", p1->name);
			scanf_s("%d", &cell_num);
			while (cell_current != NULL)
			{
				end = 0;
				Flagg = 0;
				if (cell_current->cell_name[2] != (cell_num % 10) + 48 || cell_current->cell_name[1] != (cell_num / 10) + 48 || cell_current->cell_name[0] != p1->key)
				{
					cell_current = cell_current->next;
					continue;
				}
				for (int i = 0; i < p1->ncells; i++)
				{
					if (Arr1[i] == cell_num) Flagg++;
				}
				if (Flagg > 0 || cell_num >= Ncells1) break;
				while (true)
				{
					int flag = 0;
					struct block* current = header;
					system("cls");
					show_map(header, n);
					int b = ((cell_current->cell_name[1] - 48) * 10) + (cell_current->cell_name[2] - 48);
					printf("%s (cell energy = %d):\n1) move cell\n2) split cell\n3) boost cell energy\n4) pass\n5) save and exit to main menu\nscore of %s: %d\n", cell_current->cell_name, p1->cell_energy[b], p1->name, player_score(p1->cell_energy, p1->ncells));
					scanf_s("%d", &Inp);
					if (Inp == 1)
					{
						system("cls");
						show_map(header, n);
						printf("%s:\n1) north\n2) south\n3) north east\n4) north west\n5) south east\n6) south west\n", cell_current->cell_name);
						scanf_s("%d", &inp);
						if (inp == 1)
						{
							if (cell_current->row - 1 < 0)
							{
								system("cls");
								show_map(header, n);
								printf("invalid move");
								Sleep(1500);
								continue;
							}
							while (current != NULL)
							{
								if (current->row == cell_current->row - 1 && current->column == cell_current->column)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										system("cls");
										show_map(header, n);
										printf("invalid move");
										Sleep(1500);
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = cell_current->cell_name[1];
									current->cell_name[2] = cell_current->cell_name[2];
									cell_current->cell_name[0] = '-';
									cell_current->cell_name[1] = '-';
									cell_current->cell_name[2] = '-';
									break;
								}
								current = current->next;
							}
							if (flag == 0) break;
							else continue;
						}
						else if (inp == 2)
						{
							if (cell_current->row + 1 >= n)
							{
								system("cls");
								show_map(header, n);
								printf("invalid move");
								Sleep(1500);
								continue;
							}
							while (current != NULL)
							{
								if (current->row == cell_current->row + 1 && current->column == cell_current->column)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										system("cls");
										show_map(header, n);
										printf("invalid move");
										Sleep(1500);
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = cell_current->cell_name[1];
									current->cell_name[2] = cell_current->cell_name[2];
									cell_current->cell_name[0] = '-';
									cell_current->cell_name[1] = '-';
									cell_current->cell_name[2] = '-';
									break;
								}
								current = current->next;
							}
							if (flag == 0) break;
							else continue;
						}
						else if (inp == 3)
						{
							if (cell_current->column % 2 == 0)
							{
								if (cell_current->row - 1 < 0 || cell_current->column + 1 >= n)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row - 1 && current->column == cell_current->column + 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
							else
							{
								if (cell_current->column + 1 >= n)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row && current->column == cell_current->column + 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
						}
						else if (inp == 4)
						{
							if (cell_current->column % 2 == 0)
							{
								if (cell_current->row - 1 < 0 || cell_current->column - 1 < 0)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row - 1 && current->column == cell_current->column - 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
							else
							{
								if (cell_current->column - 1 < 0)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row && current->column == cell_current->column - 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
						}
						else if (inp == 5)
						{
							if (cell_current->column % 2 == 1)
							{
								if (cell_current->row + 1 >= n || cell_current->column + 1 >= n)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row + 1 && current->column == cell_current->column + 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
							else
							{
								if (cell_current->column + 1 >= n)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row && current->column == cell_current->column + 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
						}
						else if (inp == 6)
						{
							if (cell_current->column % 2 == 1)
							{
								if (cell_current->row + 1 >= n || cell_current->column - 1 < 0)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row + 1 && current->column == cell_current->column - 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
							else
							{
								if (cell_current->column - 1 < 0)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row && current->column == cell_current->column - 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
						}
						else
						{
							system("cls");
							show_map(header, n);
							printf("invalid input");
							Sleep(1500);
						}
					}
					else if (Inp == 2)
					{
						if (cell_current->type != 2 || p1->cell_energy[b] < 80)
						{
							system("cls");
							show_map(header, n);
							printf("invalid split");
							Sleep(1500);
							continue;
						}
						int arr[6] = { 0 };
						int index = 0, Flag = 0;
						while (true)
						{
							int flagg = 0;
							flag = 0;
							current = header;
							if (Flag > 0) break;
							inp = (rand() % 6) + 1;
							for (int i = 0; i < 6; i++)
							{
								if (arr[i] == inp) flagg++;
							}
							if (flagg == 0)
							{
								arr[index] = inp;
								index++;
							}
							else if (index < 6) continue;
							else
							{
								system("cls");
								show_map(header, n);
								printf("invalid split");
								Sleep(1500);
								Flag = 2;
								continue;
							}
							if (inp == 1)
							{
								if (cell_current->row - 1 < 0) continue;
								while (current != NULL)
								{
									if (current->row == cell_current->row - 1 && current->column == cell_current->column)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = (p1->ncells / 10) + 48;
										current->cell_name[2] = (p1->ncells % 10) + 48;
										(p1->ncells)++;
										p1->cell_energy = (int*)realloc(p1->cell_energy, sizeof(int) * p1->ncells);
										p1->cell_energy[b] = 40;
										p1->cell_energy[p1->ncells - 1] = 40;
										break;
									}
									current = current->next;
								}
								if (flag == 0)
								{
									Flag = 1;
									break;
								}
								else continue;
							}
							else if (inp == 2)
							{
								if (cell_current->row + 1 >= n) continue;
								while (current != NULL)
								{
									if (current->row == cell_current->row + 1 && current->column == cell_current->column)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = (p1->ncells / 10) + 48;
										current->cell_name[2] = (p1->ncells % 10) + 48;
										(p1->ncells)++;
										p1->cell_energy = (int*)realloc(p1->cell_energy, sizeof(int) * p1->ncells);
										p1->cell_energy[b] = 40;
										p1->cell_energy[p1->ncells - 1] = 40;
										break;
									}
									current = current->next;
								}
								if (flag == 0)
								{
									Flag = 1;
									break;
								}
								else continue;
							}
							else if (inp == 3)
							{
								if (cell_current->column % 2 == 0)
								{
									if (cell_current->row - 1 < 0 || cell_current->column + 1 >= n) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row - 1 && current->column == cell_current->column + 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p1->ncells / 10) + 48;
											current->cell_name[2] = (p1->ncells % 10) + 48;
											(p1->ncells)++;
											p1->cell_energy = (int*)realloc(p1->cell_energy, sizeof(int) * p1->ncells);
											p1->cell_energy[b] = 40;
											p1->cell_energy[p1->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
								else
								{
									if (cell_current->column + 1 >= n) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row && current->column == cell_current->column + 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p1->ncells / 10) + 48;
											current->cell_name[2] = (p1->ncells % 10) + 48;
											(p1->ncells)++;
											p1->cell_energy = (int*)realloc(p1->cell_energy, sizeof(int) * p1->ncells);
											p1->cell_energy[b] = 40;
											p1->cell_energy[p1->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
							}
							else if (inp == 4)
							{
								if (cell_current->column % 2 == 0)
								{
									if (cell_current->row - 1 < 0 || cell_current->column - 1 < 0) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row - 1 && current->column == cell_current->column - 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p1->ncells / 10) + 48;
											current->cell_name[2] = (p1->ncells % 10) + 48;
											(p1->ncells)++;
											p1->cell_energy = (int*)realloc(p1->cell_energy, sizeof(int) * p1->ncells);
											p1->cell_energy[b] = 40;
											p1->cell_energy[p1->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
								else
								{
									if (cell_current->column - 1 < 0) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row && current->column == cell_current->column - 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p1->ncells / 10) + 48;
											current->cell_name[2] = (p1->ncells % 10) + 48;
											(p1->ncells)++;
											p1->cell_energy = (int*)realloc(p1->cell_energy, sizeof(int) * p1->ncells);
											p1->cell_energy[b] = 40;
											p1->cell_energy[p1->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
							}
							else if (inp == 5)
							{
								if (cell_current->column % 2 == 1)
								{
									if (cell_current->row + 1 >= n || cell_current->column + 1 >= n) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row + 1 && current->column == cell_current->column + 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p1->ncells / 10) + 48;
											current->cell_name[2] = (p1->ncells % 10) + 48;
											(p1->ncells)++;
											p1->cell_energy = (int*)realloc(p1->cell_energy, sizeof(int) * p1->ncells);
											p1->cell_energy[b] = 40;
											p1->cell_energy[p1->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
								else
								{
									if (cell_current->column + 1 >= n) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row && current->column == cell_current->column + 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p1->ncells / 10) + 48;
											current->cell_name[2] = (p1->ncells % 10) + 48;
											(p1->ncells)++;
											p1->cell_energy = (int*)realloc(p1->cell_energy, sizeof(int) * p1->ncells);
											p1->cell_energy[b] = 40;
											p1->cell_energy[p1->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
							}
							else if (inp == 6)
							{
								if (cell_current->column % 2 == 1)
								{
									if (cell_current->row + 1 >= n || cell_current->column - 1 < 0) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row + 1 && current->column == cell_current->column - 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p1->ncells / 10) + 48;
											current->cell_name[2] = (p1->ncells % 10) + 48;
											(p1->ncells)++;
											p1->cell_energy = (int*)realloc(p1->cell_energy, sizeof(int) * p1->ncells);
											p1->cell_energy[b] = 40;
											p1->cell_energy[p1->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
								else
								{
									if (cell_current->column - 1 < 0) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row && current->column == cell_current->column - 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p1->ncells / 10) + 48;
											current->cell_name[2] = (p1->ncells % 10) + 48;
											(p1->ncells)++;
											p1->cell_energy = (int*)realloc(p1->cell_energy, sizeof(int) * p1->ncells);
											p1->cell_energy[b] = 40;
											p1->cell_energy[p1->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
							}
							else
							{
								system("cls");
								show_map(header, n);
								printf("invalid input");
								Sleep(1500);
							}
						}
						if (Flag == 1) break;
						else if (Flag == 2) continue;
					}
					else if (Inp == 3)
					{
						if (p1->cell_energy[b] == 100 || cell_current->energy == 0)
						{
							system("cls");
							show_map(header, n);
							printf("invalid boost");
							Sleep(1500);
							continue;
						}
						int c = fmin(MAX_CELL_ENERGY - p1->cell_energy[b], fmin(15, cell_current->energy));
						cell_current->energy -= c;
						p1->cell_energy[b] += c;
						break;
					}
					else if (Inp == 4) break;
					else if (Inp == 5)
					{
						PVP_save_game(header);
						return 1;
					}
					else
					{
						system("cls");
						show_map(header, n);
						printf("invalid input");
						Sleep(1500);
						continue;
					}
				}
				cell_current = cell_current->next;
				Arr1[Index1] = cell_num;
				Index1++;
				end++;
				break;
			}
			if (end == 0)
			{
				system("cls");
				show_map(header, n);
				printf("invalid cell");
				Sleep(1500);
			}
		}
		else
		{
			int cell_num, end;
			cell_current = header;
			if (Index2 == Ncells2) break;
			system("cls");
			show_map(header, n);
			printf("%s enter your cell number: ", p2->name);
			scanf_s("%d", &cell_num);
			while (cell_current != NULL)
			{
				end = 0;
				Flagg = 0;
				if (cell_current->cell_name[2] != (cell_num % 10) + 48 || cell_current->cell_name[1] != (cell_num / 10) + 48 || cell_current->cell_name[0] != p2->key)
				{
					cell_current = cell_current->next;
					continue;
				}
				for (int i = 0; i < p2->ncells; i++)
				{
					if (Arr1[i] == cell_num) Flagg++;
				}
				if (Flagg > 0 || cell_num >= Ncells1) break;
				while (true)
				{
					int flag = 0;
					struct block* current = header;
					system("cls");
					show_map(header, n);
					int b = ((cell_current->cell_name[1] - 48) * 10) + (cell_current->cell_name[2] - 48);
					printf("%s (cell energy = %d):\n1) move cell\n2) split cell\n3) boost cell energy\n4) pass\n5) save and exit to main menu\nscore of %s: %d\n", cell_current->cell_name, p2->cell_energy[b], p2->name, player_score(p2->cell_energy, p2->ncells));
					scanf_s("%d", &Inp);
					if (Inp == 1)
					{
						system("cls");
						show_map(header, n);
						printf("%s:\n1) north\n2) south\n3) north east\n4) north west\n5) south east\n6) south west\n", cell_current->cell_name);
						scanf_s("%d", &inp);
						if (inp == 1)
						{
							if (cell_current->row - 1 < 0)
							{
								system("cls");
								show_map(header, n);
								printf("invalid move");
								Sleep(1500);
								continue;
							}
							while (current != NULL)
							{
								if (current->row == cell_current->row - 1 && current->column == cell_current->column)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										system("cls");
										show_map(header, n);
										printf("invalid move");
										Sleep(1500);
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = cell_current->cell_name[1];
									current->cell_name[2] = cell_current->cell_name[2];
									cell_current->cell_name[0] = '-';
									cell_current->cell_name[1] = '-';
									cell_current->cell_name[2] = '-';
									break;
								}
								current = current->next;
							}
							if (flag == 0) break;
							else continue;
						}
						else if (inp == 2)
						{
							if (cell_current->row + 1 >= n)
							{
								system("cls");
								show_map(header, n);
								printf("invalid move");
								Sleep(1500);
								continue;
							}
							while (current != NULL)
							{
								if (current->row == cell_current->row + 1 && current->column == cell_current->column)
								{
									if (current->cell_name[0] != '-' || current->type == 3)
									{
										system("cls");
										show_map(header, n);
										printf("invalid move");
										Sleep(1500);
										flag++;
										break;
									}
									current->cell_name[0] = cell_current->cell_name[0];
									current->cell_name[1] = cell_current->cell_name[1];
									current->cell_name[2] = cell_current->cell_name[2];
									cell_current->cell_name[0] = '-';
									cell_current->cell_name[1] = '-';
									cell_current->cell_name[2] = '-';
									break;
								}
								current = current->next;
							}
							if (flag == 0) break;
							else continue;
						}
						else if (inp == 3)
						{
							if (cell_current->column % 2 == 0)
							{
								if (cell_current->row - 1 < 0 || cell_current->column + 1 >= n)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row - 1 && current->column == cell_current->column + 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
							else
							{
								if (cell_current->column + 1 >= n)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row && current->column == cell_current->column + 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
						}
						else if (inp == 4)
						{
							if (cell_current->column % 2 == 0)
							{
								if (cell_current->row - 1 < 0 || cell_current->column - 1 < 0)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row - 1 && current->column == cell_current->column - 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
							else
							{
								if (cell_current->column - 1 < 0)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row && current->column == cell_current->column - 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
						}
						else if (inp == 5)
						{
							if (cell_current->column % 2 == 1)
							{
								if (cell_current->row + 1 >= n || cell_current->column + 1 >= n)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row + 1 && current->column == cell_current->column + 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
							else
							{
								if (cell_current->column + 1 >= n)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row && current->column == cell_current->column + 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
						}
						else if (inp == 6)
						{
							if (cell_current->column % 2 == 1)
							{
								if (cell_current->row + 1 >= n || cell_current->column - 1 < 0)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row + 1 && current->column == cell_current->column - 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
							else
							{
								if (cell_current->column - 1 < 0)
								{
									system("cls");
									show_map(header, n);
									printf("invalid move");
									Sleep(1500);
									continue;
								}
								while (current != NULL)
								{
									if (current->row == cell_current->row && current->column == cell_current->column - 1)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											system("cls");
											show_map(header, n);
											printf("invalid move");
											Sleep(1500);
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = cell_current->cell_name[1];
										current->cell_name[2] = cell_current->cell_name[2];
										cell_current->cell_name[0] = '-';
										cell_current->cell_name[1] = '-';
										cell_current->cell_name[2] = '-';
										break;
									}
									current = current->next;
								}
								if (flag == 0) break;
								else continue;
							}
						}
						else
						{
							system("cls");
							show_map(header, n);
							printf("invalid input");
							Sleep(1500);
						}
					}
					else if (Inp == 2)
					{
						if (cell_current->type != 2 || p2->cell_energy[b] < 80)
						{
							system("cls");
							show_map(header, n);
							printf("invalid split");
							Sleep(1500);
							continue;
						}
						int arr[6] = { 0 };
						int index = 0, Flag = 0;
						while (true)
						{
							int flagg = 0;
							flag = 0;
							current = header;
							if (Flag > 0) break;
							inp = (rand() % 6) + 1;
							for (int i = 0; i < 6; i++)
							{
								if (arr[i] == inp) flagg++;
							}
							if (flagg == 0)
							{
								arr[index] = inp;
								index++;
							}
							else if (index < 6) continue;
							else
							{
								system("cls");
								show_map(header, n);
								printf("invalid split");
								Sleep(1500);
								Flag = 2;
								continue;
							}
							if (inp == 1)
							{
								if (cell_current->row - 1 < 0) continue;
								while (current != NULL)
								{
									if (current->row == cell_current->row - 1 && current->column == cell_current->column)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = (p2->ncells / 10) + 48;
										current->cell_name[2] = (p2->ncells % 10) + 48;
										(p2->ncells)++;
										p2->cell_energy = (int*)realloc(p2->cell_energy, sizeof(int) * p2->ncells);
										p2->cell_energy[b] = 40;
										p2->cell_energy[p2->ncells - 1] = 40;
										break;
									}
									current = current->next;
								}
								if (flag == 0)
								{
									Flag = 1;
									break;
								}
								else continue;
							}
							else if (inp == 2)
							{
								if (cell_current->row + 1 >= n) continue;
								while (current != NULL)
								{
									if (current->row == cell_current->row + 1 && current->column == cell_current->column)
									{
										if (current->cell_name[0] != '-' || current->type == 3)
										{
											flag++;
											break;
										}
										current->cell_name[0] = cell_current->cell_name[0];
										current->cell_name[1] = (p2->ncells / 10) + 48;
										current->cell_name[2] = (p2->ncells % 10) + 48;
										(p2->ncells)++;
										p2->cell_energy = (int*)realloc(p2->cell_energy, sizeof(int) * p2->ncells);
										p2->cell_energy[b] = 40;
										p2->cell_energy[p2->ncells - 1] = 40;
										break;
									}
									current = current->next;
								}
								if (flag == 0)
								{
									Flag = 1;
									break;
								}
								else continue;
							}
							else if (inp == 3)
							{
								if (cell_current->column % 2 == 0)
								{
									if (cell_current->row - 1 < 0 || cell_current->column + 1 >= n) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row - 1 && current->column == cell_current->column + 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p2->ncells / 10) + 48;
											current->cell_name[2] = (p2->ncells % 10) + 48;
											(p2->ncells)++;
											p2->cell_energy = (int*)realloc(p2->cell_energy, sizeof(int) * p2->ncells);
											p2->cell_energy[b] = 40;
											p2->cell_energy[p2->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
								else
								{
									if (cell_current->column + 1 >= n) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row && current->column == cell_current->column + 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p2->ncells / 10) + 48;
											current->cell_name[2] = (p2->ncells % 10) + 48;
											(p2->ncells)++;
											p2->cell_energy = (int*)realloc(p2->cell_energy, sizeof(int) * p2->ncells);
											p2->cell_energy[b] = 40;
											p2->cell_energy[p2->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
							}
							else if (inp == 4)
							{
								if (cell_current->column % 2 == 0)
								{
									if (cell_current->row - 1 < 0 || cell_current->column - 1 < 0) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row - 1 && current->column == cell_current->column - 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p2->ncells / 10) + 48;
											current->cell_name[2] = (p2->ncells % 10) + 48;
											(p2->ncells)++;
											p2->cell_energy = (int*)realloc(p2->cell_energy, sizeof(int) * p2->ncells);
											p2->cell_energy[b] = 40;
											p2->cell_energy[p2->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
								else
								{
									if (cell_current->column - 1 < 0) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row && current->column == cell_current->column - 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p2->ncells / 10) + 48;
											current->cell_name[2] = (p2->ncells % 10) + 48;
											(p2->ncells)++;
											p2->cell_energy = (int*)realloc(p2->cell_energy, sizeof(int) * p2->ncells);
											p2->cell_energy[b] = 40;
											p2->cell_energy[p2->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
							}
							else if (inp == 5)
							{
								if (cell_current->column % 2 == 1)
								{
									if (cell_current->row + 1 >= n || cell_current->column + 1 >= n) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row + 1 && current->column == cell_current->column + 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p2->ncells / 10) + 48;
											current->cell_name[2] = (p2->ncells % 10) + 48;
											(p2->ncells)++;
											p2->cell_energy = (int*)realloc(p2->cell_energy, sizeof(int) * p2->ncells);
											p2->cell_energy[b] = 40;
											p2->cell_energy[p2->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
								else
								{
									if (cell_current->column + 1 >= n) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row && current->column == cell_current->column + 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p2->ncells / 10) + 48;
											current->cell_name[2] = (p2->ncells % 10) + 48;
											(p2->ncells)++;
											p2->cell_energy = (int*)realloc(p2->cell_energy, sizeof(int) * p2->ncells);
											p2->cell_energy[b] = 40;
											p2->cell_energy[p2->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
							}
							else if (inp == 6)
							{
								if (cell_current->column % 2 == 1)
								{
									if (cell_current->row + 1 >= n || cell_current->column - 1 < 0) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row + 1 && current->column == cell_current->column - 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p2->ncells / 10) + 48;
											current->cell_name[2] = (p2->ncells % 10) + 48;
											(p2->ncells)++;
											p2->cell_energy = (int*)realloc(p2->cell_energy, sizeof(int) * p2->ncells);
											p2->cell_energy[b] = 40;
											p2->cell_energy[p2->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
								else
								{
									if (cell_current->column - 1 < 0) continue;
									while (current != NULL)
									{
										if (current->row == cell_current->row && current->column == cell_current->column - 1)
										{
											if (current->cell_name[0] != '-' || current->type == 3)
											{
												flag++;
												break;
											}
											current->cell_name[0] = cell_current->cell_name[0];
											current->cell_name[1] = (p2->ncells / 10) + 48;
											current->cell_name[2] = (p2->ncells % 10) + 48;
											(p2->ncells)++;
											p2->cell_energy = (int*)realloc(p2->cell_energy, sizeof(int) * p2->ncells);
											p2->cell_energy[b] = 40;
											p2->cell_energy[p2->ncells - 1] = 40;
											break;
										}
										current = current->next;
									}
									if (flag == 0)
									{
										Flag = 1;
										break;
									}
									else continue;
								}
							}
							else
							{
								system("cls");
								show_map(header, n);
								printf("invalid input");
								Sleep(1500);
							}
						}
						if (Flag == 1) break;
						else if (Flag == 2) continue;
					}
					else if (Inp == 3)
					{
						if (p2->cell_energy[b] == 100 || cell_current->energy == 0)
						{
							system("cls");
							show_map(header, n);
							printf("invalid boost");
							Sleep(1500);
							continue;
						}
						int c = fmin(MAX_CELL_ENERGY - p2->cell_energy[b], fmin(15, cell_current->energy));
						cell_current->energy -= c;
						p2->cell_energy[b] += c;
						break;
					}
					else if (Inp == 4) break;
					else if (Inp == 5)
					{
						PVP_save_game(header);
						return 1;
					}
					else
					{
						system("cls");
						show_map(header, n);
						printf("invalid input");
						Sleep(1500);
						continue;
					}
				}
				cell_current = cell_current->next;
				Arr2[Index2] = cell_num;
				Index2++;
				end++;
				break;
			}
			if (end == 0)
			{
				system("cls");
				show_map(header, n);
				printf("invalid cell");
				Sleep(1500);
			}
		}
	}
	system("cls");
	show_map(header, n);
	printf("score of %s: %d\nscore of %s: %d", p1->name, player_score(p1->cell_energy, p1->ncells), p2->name, player_score(p2->cell_energy, p2->ncells));
	Sleep(1500);
	return 0;

}

void map_editor()
{
	int n;
	FILE* mapfp;
	char file_name[MAX_FILE_NAME];
	getchar();
	printf("please enter your new map file name: ");
	gets_s(file_name, MAX_FILE_NAME);
	printf("please enter n (n > 1): ");
	scanf_s("%d", &n);
	fopen_s(&mapfp, file_name, "wb");
	fwrite(&n, 4, 1, mapfp);
	printf("please declear block types (row major):\n");
	for (int i = 0; i < n * n; i++)
	{
		char holder;
		scanf_s(" %c", &holder);
		fwrite(&holder, 1, 1, mapfp);
	}
	system("cls");
	printf("new map successfully created");
	Sleep(1500);
	system("cls");
	fclose(mapfp);
}