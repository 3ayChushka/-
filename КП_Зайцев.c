#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <stddef.h>
#include <stdlib.h>

int new_zapis(zapis, chislo_zapisey);  /*Добавление новой записи*/
int vivod_zapisey(zapis, chislo_zapisey); /* Вывод ВСЕХ записей*/
int poisk_code(zapis, chislo_zapisey, base); /* Поиск по коду*/
int poisk_time(zapis, chislo_zapisey, base, ind_poisk, hour_poisk, min_poisk, sec_poisk, hour_poisk_1, min_poisk_1, sec_poisk_1);  /*Поиск по диапазону времени*/
int edit_zapis(zapis, chislo_zapisey, base);  /*Изменить запись*/
void sort(zapis, zapis2, chislo_zapisey);  /*Сортировка*/
int compare_name(av, bv);  /*Функция для обработки сортировки*/

struct DataBase
{
	char name_app[50];
	int code;
	char lvl;
	struct tm time;
};

typedef struct DataBase base_t;

int main()
{
	setlocale(LC_ALL, "RUS");

	int vibor, chislo_zapisey = 0;
	base_t zapis[100], zapis2[100];
	FILE* base;

	do {
		printf("Выберите действие\n1-Добавить новую запись\n2-Поиск по полю\n3-Вывод записей\n4-Изменить запись\n5-Отсортировать по \"Частота событий на приложение\"\n6-Добавить произвольное количество записей\n0-Выход\n\n");
		vibor = _getch();

		if (vibor == '1')   /*Добавление новой записи  РАБОТАЕТ*/
		{
			base = fopen("База Данных.txt", "a");
			chislo_zapisey = new_zapis(zapis, chislo_zapisey, base);
			fclose(base);
		}

		else if (vibor == '2')  /*Поиск    РАБОТАЕТ  */
		{
			int vibor_poiska, ind_poisk, g=0;
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
				int hour_poisk, min_poisk, sec_poisk, hour_poisk_1, min_poisk_1, sec_poisk_1;
				ind_poisk = 0;

				printf("Введите диапазон времени: __:__:__-__:__:__\n");
				scanf("%d:%d:%d-%d:%d:%d", &hour_poisk, &min_poisk, &sec_poisk, &hour_poisk_1, &min_poisk_1, &sec_poisk_1);
				getchar();
				printf("\n");

				for (int u = 0; u < chislo_zapisey; u++)
				{
					ind_poisk = poisk_time(zapis, chislo_zapisey, base, ind_poisk, hour_poisk, min_poisk, sec_poisk, hour_poisk_1, min_poisk_1, sec_poisk_1);
					if (ind_poisk >= 0)
						printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[ind_poisk].name_app, zapis[ind_poisk].code, zapis[ind_poisk].lvl, zapis[ind_poisk].time.tm_hour, zapis[ind_poisk].time.tm_min, zapis[ind_poisk].time.tm_sec);
					
					if (ind_poisk + 1 >= chislo_zapisey)break;
					else u = ind_poisk+1;
					ind_poisk++;
				}
			}

			else
					{ 
					printf("Ошибка\n"); 
					ind_poisk = -1;
					}

			fclose(base);
		}

		else if (vibor == '3') /*Вывод всех записей   РАБОТАЕТ*/
		{
			base = fopen("База Данных.txt", "r");
			chislo_zapisey=vivod_zapisey(zapis, chislo_zapisey, base);  /*Возвращает число записей, которые записаны в файле*/
			fclose(base);
		}

		else if (vibor == '4')   /* Изменить запись     РАБОТАЕТ*/
		{
			base = fopen("База данных.txt", "r");
			edit_zapis(zapis, chislo_zapisey, base);
			fclose(base);
		}

		else if (vibor == '5')
		{
			printf("Сортировка по критерию Частота событий на приложение\n");
			base = fopen("База Данных.txt", "r");
			sort(zapis, zapis2, chislo_zapisey, base);
			fclose(base);
		}

		else if (vibor == '6')   /*Добавление нескольких записей     РАБОТАЕТ*/
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

int new_zapis(base_t* zapis, int chislo_zapisey, FILE *base) /*Добавление новой записи*/
{
	printf("Название приложения:\n"); 
	fgets(zapis[chislo_zapisey].name_app, 50, stdin);
	fprintf(base, "%s", zapis[chislo_zapisey].name_app);

	printf("Код операции:\n");  
	scanf("%d", &zapis[chislo_zapisey].code);
	getchar();
	fprintf(base, "%d\n", zapis[chislo_zapisey].code);

	printf("Введите уровень события\nA-Предупреждение\nB-Угроза\nC-Сбой\n");  
	scanf("%c", &zapis[chislo_zapisey].lvl);
	getchar();
	fprintf(base, "%c\n", zapis[chislo_zapisey].lvl);

	int hour, min, sec;
	printf("Введите время в формате __:__:__\n");
	scanf("%d:%d:%d", &hour, &min, &sec);
	getchar();

	zapis[chislo_zapisey].time.tm_hour = hour;
	zapis[chislo_zapisey].time.tm_min = min;
	zapis[chislo_zapisey].time.tm_sec = sec;

	fprintf(base, "%d %d %d", zapis[chislo_zapisey].time.tm_hour, zapis[chislo_zapisey].time.tm_min, zapis[chislo_zapisey].time.tm_sec);
	
	chislo_zapisey++;

	return chislo_zapisey;
}

int poisk_code(base_t* zapis, int chislo_zapisey, FILE *base)  /*Поиск по заданному полю*/
{
	int o = 0, code_poisk;
	char n[50];
	while (!feof(base))  /*Считывание всех записей из файла*/
	{
		fgets(zapis[o].name_app, 50, base);
		fscanf(base, "%d", &zapis[o].code);
		fgets(n, 50, base);

		fscanf(base, "%s", &zapis[o].lvl);
		fgets(n, 50, base);

		fscanf(base, "%d", &zapis[o].time.tm_hour);
		fscanf(base, "%d", &zapis[o].time.tm_min);
		fscanf(base, "%d", &zapis[o].time.tm_sec);
		o++;
	}

	printf("Введите код операции\n");
	scanf("%d", &code_poisk);
	getchar();

	for (int i = 0; i < chislo_zapisey; i++)
	{
		if (code_poisk == zapis[i].code) return i;
	}
	return -1;
}

int poisk_time(base_t* zapis, int chislo_zapisey, FILE* base, int ind_poisk, int hour_poisk, int min_poisk, int sec_poisk,int hour_poisk_1,int min_poisk_1,int sec_poisk_1)
{
	int o = 0;
	char n[50];
	while (!feof(base))  /*Считывание всех записей из файла*/
	{
		fgets(zapis[o].name_app, 50, base);
		fscanf(base, "%d", &zapis[o].code);
		fgets(n, 50, base);

		fscanf(base, "%s", &zapis[o].lvl);
		fgets(n, 50, base);

		fscanf(base, "%d", &zapis[o].time.tm_hour);
		fscanf(base, "%d", &zapis[o].time.tm_min);
		fscanf(base, "%d", &zapis[o].time.tm_sec);
		o++;
	}
	
	for (int i = ind_poisk; i < chislo_zapisey; i++)
	{
		if (zapis[i].time.tm_hour >= hour_poisk && zapis[i].time.tm_hour < hour_poisk_1)
		{
			return i;
		}
		else if (zapis[i].time.tm_hour == hour_poisk || zapis[i].time.tm_hour == hour_poisk_1)
			if (zapis[i].time.tm_min >= min_poisk && zapis[i].time.tm_min < min_poisk_1)
			{
				return i;
			}
			else if (zapis[i].time.tm_min == min_poisk || zapis[i].time.tm_min == min_poisk_1) 
				if (zapis[i].time.tm_sec >= sec_poisk && zapis[i].time.tm_sec < sec_poisk_1)
				{
					return i;
				}
	}
	return -1;
}

int vivod_zapisey(base_t* zapis, int chislo_zapisey, FILE *base)  /*Вывод записей*/
{
	int str = 0;
	while (!feof(base))     /*Цикл для подсчёта уже существующих записей в файле*/
	{
		if (fgetc(base) == '\n')
			str++;
	}
	str = str / 3;
	
	base = fopen("База Данных.txt", "r");

	for (int chislo_zapisey_vivod = 0; chislo_zapisey_vivod<str; chislo_zapisey_vivod++) /*Вывод записей*/
	{
		char n[20];  /*Массив, чтобы убрать всё неотсканированное*/

		if (feof(base) == 0)
		{
			printf("----------Запись %d---------\n", chislo_zapisey_vivod + 1);

			fgets(zapis[chislo_zapisey_vivod].name_app, 50, base);
			fscanf(base, "%d", &zapis[chislo_zapisey_vivod].code);
			fgets(n, 20, base);

			fscanf(base, "%s", &zapis[chislo_zapisey_vivod].lvl);
			fgets(n, 20, base);

			fscanf(base, "%d", &zapis[chislo_zapisey_vivod].time.tm_hour);
			fscanf(base, "%d", &zapis[chislo_zapisey_vivod].time.tm_min);
			fscanf(base, "%d", &zapis[chislo_zapisey_vivod].time.tm_sec);

			printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[chislo_zapisey_vivod].name_app, zapis[chislo_zapisey_vivod].code, zapis[chislo_zapisey_vivod].lvl, zapis[chislo_zapisey_vivod].time.tm_hour, zapis[chislo_zapisey_vivod].time.tm_min, zapis[chislo_zapisey_vivod].time.tm_sec);
			
		}
		else break;
	}
	return str;
}

int edit_zapis(base_t* zapis, int chislo_zapisey, FILE *base)
{
	int number;

	printf("Введите номер записи, которую хотите изменить\n");
	scanf("%d", &number);
	getchar();

	if (number - 1 > chislo_zapisey - 1)printf("Всего записей:%d\nЗапись не обнаружена\n\n", chislo_zapisey);
	else
	{
		printf("----------%d\n", chislo_zapisey);

		int o = 0;
		while (!feof(base))  /*Считывание всех записей из файла*/
		{
			char n[50];
			fgets(zapis[o].name_app, 50, base);
			fscanf(base, "%d", &zapis[o].code);
			fgets(n, 50, base);

			fscanf(base, "%s", &zapis[o].lvl);
			fgets(n, 50, base);

			fscanf(base, "%d", &zapis[o].time.tm_hour);
			fscanf(base, "%d", &zapis[o].time.tm_min);
			fscanf(base, "%d", &zapis[o].time.tm_sec);
			o++;
		}
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
		for (int i = 0; i < chislo_zapisey; i++)  /*Запись в файл записей с изменением*/
		{
			fprintf(base, "%s", zapis[i].name_app);
			fprintf(base, "%d\n", zapis[i].code);
			fprintf(base, "%c\n", zapis[i].lvl);
			fprintf(base, "%d %d %d", zapis[i].time.tm_hour, zapis[i].time.tm_min, zapis[i].time.tm_sec);
		}
	}
	return 0;
}

void sort(base_t* zapis, base_t* zapis2, int chislo_zapisey, FILE* base)  /*Сортировка*/
{
	for (int i = 0; i < chislo_zapisey; i++)
		zapis2[i] = zapis[i];

	qsort(zapis2, chislo_zapisey, sizeof(base_t), compare_name);
	vivod_zapisey(zapis2, chislo_zapisey, base);
}

int compare_name(const void* av, const void* bv)
{
	const base_t* a = av, * b = bv;
	return strcmp(a->name_app, b->name_app);
}
