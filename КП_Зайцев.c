#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <stddef.h>
#include <stdlib.h>

struct DataBase
{
	char name_app[50];
	int code;
	char lvl;
	struct tm time;
};
typedef struct DataBase base_t;

struct sort
{
	base_t z2[100];
	int c;
};
typedef struct sort sort_t;


int new_zapis(base_t* zapis, int chislo_zapisey, FILE* base); /*Создание новой записи*/
int poisk_code(base_t* zapis, int chislo_zapisey, FILE* base);  /*Поиск по коду события*/
int poisk_time(base_t* zapis, int chislo_zapisey, FILE* base, int ind_poisk, struct tm t, struct tm t1);   /*Поиск по диапазону времени*/
int read_zapis(FILE* base, base_t* zapis);   /*Чтение из файла*/
void zapis_file(FILE* base, base_t* zapis, int chislo_zapisey, int q);
void vivod_zapisey(base_t* zapis, int chislo_zapisey);  /*Вывод записей*/
int edit_zapis(base_t* zapis, int chislo_zapisey, FILE* base);   /*Редактировать запись*/
void sort(base_t* zapis, sort_t* zapis2, int chislo_zapisey, FILE* base);  /*Сортировка*/
int compare_name(const void* av, const void* bv);  /*Сортировка по названию*/
int compare_count(const void* av, const void* bv);  /*Сортировка по частоте*/


int main()
{
	setlocale(LC_ALL, "RUS");

	int vibor, chislo_zapisey = 0;
	base_t *zapis=NULL;
	sort_t *zapis2=NULL;
	FILE* base;

	do {
		printf("Выберите действие\n1-Добавить новую запись\n2-Поиск по полю\n3-Вывод записей\n4-Изменить запись\n5-Отсортировать по \"Частота событий на приложение\"\n6-Добавить произвольное количество записей\n0-Выход\n\n");
		vibor = _getch();

		if (vibor == '1')   /*Добавление новой записи*/
		{
			base = fopen("База Данных.txt", "a");
			zapis = realloc(zapis, (chislo_zapisey + 1) * sizeof(base_t));

			chislo_zapisey = new_zapis(zapis, chislo_zapisey, base);
			fclose(base);
		}

		else if (vibor == '2')  /*Поиск*/
		{
			int vibor_poiska, ind_poisk, g = 0;
			printf("Введите поле для поиска:\n\n1-Код операции\n2-Диапазон времени\n\n");
			vibor_poiska = _getch();

			base = fopen("База данных.txt", "r");
			if (vibor_poiska == '1')
			{
				ind_poisk = poisk_code(zapis, chislo_zapisey, base);
				if (ind_poisk >= 0) printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[ind_poisk].name_app, zapis[ind_poisk].code, zapis[ind_poisk].lvl, zapis[ind_poisk].time.tm_hour, zapis[ind_poisk].time.tm_min, zapis[ind_poisk].time.tm_sec);
			}

			else if (vibor_poiska == '2')
			{
				ind_poisk = 0;
				struct tm t, t1;

				printf("Введите диапазон времени: __:__:__-__:__:__\n");
				scanf("%d:%d:%d-%d:%d:%d", &t.tm_hour, &t.tm_min, &t.tm_sec, &t1.tm_hour, &t1.tm_min, &t1.tm_sec);
				getchar();
				printf("\n");

				for (int u = 0; u < chislo_zapisey; u++)
				{
					ind_poisk = poisk_time(zapis, chislo_zapisey, base, ind_poisk, t, t1)+1;
					if (ind_poisk >= 0)
						printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[ind_poisk].name_app, zapis[ind_poisk].code, zapis[ind_poisk].lvl, zapis[ind_poisk].time.tm_hour, zapis[ind_poisk].time.tm_min, zapis[ind_poisk].time.tm_sec);

					if (ind_poisk >= chislo_zapisey) break;
					else u = ind_poisk;
				}
			} else
			{
				printf("Ошибка\n");
				ind_poisk = -1;
			}

			fclose(base);
		}

		else if (vibor == '3') /*Вывод всех записей*/
		{
			base = fopen("База данных.txt", "r");
			int str = 0;
			while (!feof(base))     /*Цикл для подсчёта записей в файле для выделения памяти*/
			{
				if (fgetc(base) == '\n')
					str++;
			}
			str = str / 3;
			zapis = (base_t*)malloc(str * sizeof(base_t));

			base = fopen("База Данных.txt", "r");
			chislo_zapisey = read_zapis(base, zapis);
			vivod_zapisey(zapis, chislo_zapisey); 
			fclose(base);
		}

		else if (vibor == '4')   /* Изменить запись*/
		{
			base = fopen("База данных.txt", "r");
			edit_zapis(zapis, chislo_zapisey, base);
			fclose(base);
		}

		else if (vibor == '5')   /*Сортировка*/
		{
			printf("Сортировка по критерию Частота событий на приложение\n\n");
			base = fopen("База Данных.txt", "r");
			zapis2 = malloc(chislo_zapisey*sizeof(sort_t));
			sort(zapis, zapis2, chislo_zapisey, base);
			fclose(base);
		}

		else if (vibor == '6')   /*Добавление нескольких записей*/
		{
			printf("Сколько вы хотите добавить записей?\n");
			int nes_zapis;
			scanf("%d", &nes_zapis);
			getchar();
			int max = chislo_zapisey + nes_zapis - 1;

			base = fopen("База Данных.txt", "a");
			for (int i = chislo_zapisey; i <= max; i++)
				chislo_zapisey = new_zapis(zapis, chislo_zapisey, base);
			fclose(base);
		}
	} while (vibor != '0');
}

int new_zapis(base_t* zapis, int chislo_zapisey, FILE* base) /*Добавление новой записи*/
{
	printf("Название приложения:\n");
	fgets(zapis[chislo_zapisey].name_app, 50, stdin);

	printf("Код операции:\n");
	scanf("%d", &zapis[chislo_zapisey].code);
	getchar();
	
	printf("Введите уровень события\nA-Предупреждение\nB-Угроза\nC-Сбой\n");
	scanf("%c", &zapis[chislo_zapisey].lvl);
	getchar();

	int hour, min, sec;
	printf("Введите время в формате __:__:__\n");
	scanf("%d:%d:%d", &hour, &min, &sec);
	getchar();

	zapis[chislo_zapisey].time.tm_hour = hour;
	zapis[chislo_zapisey].time.tm_min = min;
	zapis[chislo_zapisey].time.tm_sec = sec;

	zapis_file(base, zapis, chislo_zapisey, 1);
	chislo_zapisey++;

	return chislo_zapisey;
}

int read_zapis(FILE* base, base_t* zapis)
{

	int str = 0;
	while (!feof(base))     /*Цикл для подсчёта уже существующих записей в файле*/
	{
		if (fgetc(base) == '\n')
			str++;
	}
	str = str / 3;

	base = fopen("База Данных.txt", "r");

	for (int chislo_read = 0; chislo_read < str; chislo_read++) /*Вывод записей*/
	{
		char n[20];  /*Массив, чтобы убрать всё неотсканированное*/
		if (feof(base) == 0)
		{
			fgets(zapis[chislo_read].name_app, 50, base);
			fscanf(base, "%d", &zapis[chislo_read].code);
			fgets(n, 20, base);

			fscanf(base, "%s", &zapis[chislo_read].lvl);
			fgets(n, 20, base);

			fscanf(base, "%d", &zapis[chislo_read].time.tm_hour);
			fscanf(base, "%d", &zapis[chislo_read].time.tm_min);
			fscanf(base, "%d", &zapis[chislo_read].time.tm_sec);
		}
		else break;
	}
	return str;
}

void zapis_file(FILE* base, base_t* zapis, int chislo_zapisey, int q)
{
	for (int i = chislo_zapisey; i < chislo_zapisey + q; i++)
	{
		fprintf(base, "%s", zapis[i].name_app);
		fprintf(base, "%d\n", zapis[i].code);
		fprintf(base, "%c\n", zapis[i].lvl);
		fprintf(base, "%d %d %d", zapis[i].time.tm_hour, zapis[i].time.tm_min, zapis[i].time.tm_sec);
	}
}

int poisk_code(base_t* zapis, int chislo_zapisey, FILE* base)  /*Поиск по заданному полю*/
{
	int code_poisk;
	read_zapis(base, zapis);

	printf("Введите код операции\n");
	scanf("%d", &code_poisk);
	getchar();

	for (int i = 0; i < chislo_zapisey; i++)
	{
		if (code_poisk == zapis[i].code) return i;
	}
	return -1;
}

int poisk_time(base_t* zapis, int chislo_zapisey, FILE* base, int ind_poisk, struct tm t, struct tm t1)
{
	read_zapis(base, zapis);

	for (int i = ind_poisk; i < chislo_zapisey; i++)
	{
		if (zapis[i].time.tm_hour >= t.tm_hour && zapis[i].time.tm_hour < t1.tm_hour)
		{
			return i;
		}
		else if (zapis[i].time.tm_hour == t.tm_hour || zapis[i].time.tm_hour == t1.tm_hour)
			if (zapis[i].time.tm_min >= t.tm_min && zapis[i].time.tm_min < t1.tm_min)
			{
				return i;
			}
			else if (zapis[i].time.tm_min == t.tm_min || zapis[i].time.tm_min == t1.tm_min)
				if (zapis[i].time.tm_sec >= t.tm_sec && zapis[i].time.tm_sec < t1.tm_sec)
				{
					return i;
				}
	}
	return -1;
}

void vivod_zapisey(base_t* zapis, int chislo_zapisey)  /*Вывод записей*/
{
	for (int chislo_zapisey_vivod = 0; chislo_zapisey_vivod < chislo_zapisey; chislo_zapisey_vivod++)
	{
		printf("----------Запись %d---------\n", chislo_zapisey_vivod + 1);
		printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[chislo_zapisey_vivod].name_app, zapis[chislo_zapisey_vivod].code, zapis[chislo_zapisey_vivod].lvl, zapis[chislo_zapisey_vivod].time.tm_hour, zapis[chislo_zapisey_vivod].time.tm_min, zapis[chislo_zapisey_vivod].time.tm_sec);
	}
}

int edit_zapis(base_t* zapis, int chislo_zapisey, FILE* base)   /*Изменить запись*/
{
	int number;

	printf("Введите номер записи, которую хотите изменить\n");
	scanf("%d", &number);
	getchar();

	if (number - 1 > chislo_zapisey - 1) printf("Всего записей:%d\nЗапись не обнаружена\n\n", chislo_zapisey);
	else
	{
		read_zapis(base, zapis);
		fclose(base);

		printf("Название приложения:\n");            /*Изменение записи*/
		fgets(zapis[number - 1].name_app, 50, stdin);

		printf("Код операции:\n");
		scanf("%d", &zapis[number - 1].code);
		getchar();

		printf("Выберите уровень события:\nA-Предупреждение\nB-Угроза\nC-Сбой\n");
		scanf("%c", &zapis[number - 1].lvl);
		getchar();

		int hour, min, sec;
		printf("Введите время в формате __:__:__\n");
		scanf("%d:%d:%d", &hour, &min, &sec);
		getchar();

		zapis[number - 1].time.tm_hour = hour;
		zapis[number - 1].time.tm_min = min;
		zapis[number - 1].time.tm_sec = sec;

		base = fopen("База данных.txt", "w");
		zapis_file(base, zapis, 0, chislo_zapisey);
	}
	return 0;
}

void sort(base_t* zapis, sort_t* zapis2, int chislo_zapisey, FILE* base)  /*Сортировка*/
{
	int o = 1;
	qsort(zapis, chislo_zapisey, sizeof(base_t), compare_name);

	for (int i = 0; i < chislo_zapisey; i++)
	{
		(zapis2 + i)->c = 1;
		*(zapis2 + i)->z2 = *(zapis + i);

		if (strcmp((zapis + i)->name_app, (zapis + i + 1)->name_app) == 0)
		{
			o++;
			continue;
		}
		if (o != 1) for (int g = i + 1 - o; g <= i; g++)
			(zapis2 + g)->c = o;
		o = 1;
	}

	qsort(zapis2, chislo_zapisey, sizeof(sort_t), compare_count);

	for (int i = 0; i < chislo_zapisey; i++)
		printf("----------Запись %d---------\nНазвание приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", i + 1, zapis2[i].z2->name_app, zapis2[i].z2->code, zapis2[i].z2->lvl, zapis2[i].z2->time.tm_hour, zapis2[i].z2->time.tm_min, zapis2[i].z2->time.tm_sec);
}

int compare_name(const void* av, const void* bv)   /*Сортировка по названию, чтобы посчитать, сколько событий на каждое приложение*/
{
	const base_t* a = av, * b = bv;
	return strcmp(a->name_app, b->name_app);
}

int compare_count(const void* av, const void* bv)    /*Сортировка относительно количества операций*/
{
	const sort_t* a = av, * b = bv;
	if (a->c > b->c)return -1;
	if (a->c < b->c)return 1;
	return 0;
}
