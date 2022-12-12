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
int poisk(zapis, chislo_zapisey);  /*Поиск*/
//Поиск по полям:
int poisk_po_nazvaniu(zapis, chislo_zapisey);
int poisk_po_code(zapis, chislo_zapisey);
int poisk_po_lvl(zapis, chislo_zapisey);
int poisk_po_time(zapis, chislo_zapisey);

int edit_zapis(zapis, chislo_zapisey);  /*Изменить запись*/

int compare_name(av, bv);  /*Функция для обработки сортировки*/

struct DataBase
{
	char name_app[50];
	int code;
	char lvl;
	struct tm time;
};

typedef struct DataBase db;

int main()
{
	setlocale(LC_ALL, "RUS");
	
	int vibor, chislo_zapisey = 0;
	db zapis[100], zapis2[100];

	do {
		printf("Выберите действие\n1-Добавить новую запись\n2-Поиск по полю\n3-Вывод записей\n4-Изменить запись\n5-Отсортировать по \"Частота событий на приложение\"\n6-Добавить произвольное количество записей\n0-Выход\n\n");
		vibor = _getch();

		if (vibor == '1')/*Добавление новой записи*/
			chislo_zapisey = new_zapis(zapis, chislo_zapisey);  
		else if (vibor == '2')  /*Поиск по полю*/
		{
			printf("Введите поле для поиска:\n1-Название\n2-Код операции\n3-Уровень события\n4-Диапазон времени\n");
			poisk(zapis, chislo_zapisey);
		}
		else if (vibor == '3') /*Вывод всех записей*/
		{
			vivod_zapisey(zapis, chislo_zapisey);
		}
		else if (vibor == '4')
		{
			printf("Введите номер записи, которую хотите изменить\n");
			edit_zapis(zapis, zapis2, chislo_zapisey);
		}
		else if (vibor == '5')
		{
			printf("Сортировка по критерию Частота событий на приложение\n");
			sort(zapis, zapis2, chislo_zapisey);
		}
		else if (vibor == '6')
		{
			printf("Сколько вы хотите добавить записей?\n");
			int nes_zapis;
			scanf("%d", &nes_zapis);
			getchar();
			int max = chislo_zapisey + nes_zapis-1;

			for(int i=chislo_zapisey;i<=max;i++)
				chislo_zapisey=new_zapis(zapis, chislo_zapisey);
		}
	} while (vibor != '0');
}

int new_zapis(db *zapis, int chislo_zapisey) /*Добавление новой записи(пока в цикле)*/
{
	int a[100];
	for (int i = 0; i < 100; i++)
		a[i] = 1;

	printf("Название приложения:\n"); /*Название приложения*/
	fgets(zapis[chislo_zapisey].name_app, 50, stdin);

	printf("Код операции:\n");  /*Код операции*/
	scanf("%d", &zapis[chislo_zapisey].code);
	getchar();

	printf("Выберите уровень события:\nA-Предупреждение\nB-Угроза\nC-Сбой\n");  /*Уровень события*/
	scanf("%c", &zapis[chislo_zapisey].lvl);
	getchar();

	int hour, min, sec;
	printf("Введите время в формате __:__:__\n");   /*Время операции*/
	scanf("%d:%d:%d", &hour, &min, &sec);
	getchar();

	zapis[chislo_zapisey].time.tm_hour = hour;
	zapis[chislo_zapisey].time.tm_min = min;
	zapis[chislo_zapisey].time.tm_sec = sec;

	chislo_zapisey++;

	return chislo_zapisey;
}  

int vivod_zapisey(db *zapis, int chislo_zapisey)  /*Вывод записей*/
{
	for (int chislo_zapisey_vivod = 0; chislo_zapisey_vivod <= chislo_zapisey-1; chislo_zapisey_vivod++) /*Вывод записей*/
	{
		printf("----------Запись %d---------\n", chislo_zapisey_vivod + 1);
		printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[chislo_zapisey_vivod].name_app, zapis[chislo_zapisey_vivod].code, zapis[chislo_zapisey_vivod].lvl, zapis[chislo_zapisey_vivod].time.tm_hour, zapis[chislo_zapisey_vivod].time.tm_min, zapis[chislo_zapisey_vivod].time.tm_sec);
	}
	return 0;
}

int poisk(db *zapis, int chislo_zapisey)  /*Поиск по заданному полю*/
{
	int vibor_poiska;
	vibor_poiska = _getch();/*Выбор поля для поиска*/

	switch (vibor_poiska)
	{
	case '1':  /*Поиск по названию*/
		{
			poisk_po_nazvaniu(zapis, chislo_zapisey);
			break;
		}
	case '2':  /*Поиск по коду */
	{
		poisk_po_code(zapis, chislo_zapisey);
		break;
	}
	case '3':  /*Поиск по уровню события*/
	{
		poisk_po_lvl(zapis, chislo_zapisey);
		break;
	}
	case '4':  /*Поиск по времени*/
	{
		poisk_po_time(zapis, chislo_zapisey);
		break;
	}
	}
	return 0;
}

int poisk_po_nazvaniu(db* zapis, int chislo_zapisey)
{
	char name_poisk[50];

	printf("Введите название:\n");
	fgets(name_poisk, 50, stdin);
	for (int i = 0; i < chislo_zapisey; i++)
	{
		if (strcmp(name_poisk, zapis[i].name_app) == 0)
		{
			printf("---------Запись %d---------\n", i + 1);
			printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[i].name_app, zapis[i].code, zapis[i].lvl, zapis[i].time.tm_hour, zapis[i].time.tm_min, zapis[i].time.tm_sec);
		}
	}
	return 0;
}
int poisk_po_code(db* zapis, int chislo_zapisey)
{
	int code_poisk;

	printf("Введите код операции\n");
	scanf("%d", &code_poisk);
	getchar();

	for (int i = 0; i <= chislo_zapisey - 1; i++)
		if (code_poisk == zapis[i].code)
		{
			printf("---------Запись %d---------\n", i + 1);
			printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[i].name_app, zapis[i].code, zapis[i].lvl, zapis[i].time.tm_hour, zapis[i].time.tm_min, zapis[i].time.tm_sec);
		}
		return 0;
}
int poisk_po_lvl(db* zapis, int chislo_zapisey)
{
	printf("Введите уровень события\n");
	char lvl_poisk;
	scanf("%c", &lvl_poisk);
	getchar();

	for (int i = 0; i <= chislo_zapisey - 1; i++)
		if (lvl_poisk == zapis[i].lvl)
		{
			printf("---------Запись %d---------\n", i + 1);
			printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[i].name_app, zapis[i].code, zapis[i].lvl, zapis[i].time.tm_hour, zapis[i].time.tm_min, zapis[i].time.tm_sec);
		}
	return 0;
}
int poisk_po_time(db* zapis, int chislo_zapisey)
{
	int hour_poisk, min_poisk, sec_poisk, hour_poisk_1, min_poisk_1, sec_poisk_1;

	printf("Введите диапазон времени: __:__:__-__:__:__\n");
	scanf("%d:%d:%d-%d:%d:%d", &hour_poisk, &min_poisk, &sec_poisk, &hour_poisk_1, &min_poisk_1, &sec_poisk_1);
	getchar();
	printf("\n");

	for (int i = 0; i <= chislo_zapisey - 1; i++)
	{
		if (zapis[i].time.tm_hour >= hour_poisk && zapis[i].time.tm_hour < hour_poisk_1)
		{
			printf("---------Запись %d---------\n", i + 1);
			printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[i].name_app, zapis[i].code, zapis[i].lvl, zapis[i].time.tm_hour, zapis[i].time.tm_min, zapis[i].time.tm_sec);
		}
		else if (zapis[i].time.tm_hour == hour_poisk || zapis[i].time.tm_hour == hour_poisk_1)
			if (zapis[i].time.tm_min >= min_poisk && zapis[i].time.tm_min < min_poisk_1)
			{
				printf("---------Запись %d---------\n", i + 1);
				printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[i].name_app, zapis[i].code, zapis[i].lvl, zapis[i].time.tm_hour, zapis[i].time.tm_min, zapis[i].time.tm_sec);
			}
			else if (zapis[i].time.tm_min == min_poisk || zapis[i].time.tm_min == min_poisk_1) if (zapis[i].time.tm_sec >= sec_poisk && zapis[i].time.tm_sec < sec_poisk_1)
			{
				printf("---------Запись %d---------\n", i + 1);
				printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[i].name_app, zapis[i].code, zapis[i].lvl, zapis[i].time.tm_hour, zapis[i].time.tm_min, zapis[i].time.tm_sec);
			}
	}
	return 0;
}

int edit_zapis(db* zapis, int chislo_zapisey)
{
	int number;
	scanf("%d", &number);
	getchar();

	if (number - 1 > chislo_zapisey)printf("Запись не обнаружена\n\n");
	else
	{
		printf("Название приложения:\n"); /*Название приложения*/
		fgets(zapis[number - 1].name_app, 50, stdin);

		printf("Код операции:\n");  /*Код операции*/
		scanf("%d", &zapis[number - 1].code);
		getchar();

		printf("Выберите уровень события:\nA-Предупреждение\nB-Угроза\nC-Сбой\n");  /*Уровень события*/
		scanf("%c", &zapis[number - 1].lvl);
		getchar();

		int hour, min, sec;
		printf("Введите время в формате __:__:__\n");   /*Время операции*/
		scanf("%d:%d:%d", &hour, &min, &sec);
		getchar();

		zapis[number - 1].time.tm_hour = hour;
		zapis[number - 1].time.tm_min = min;
		zapis[number - 1].time.tm_sec = sec;
	}
	return 0;
}

int sort(db* zapis, db* zapis2, int chislo_zapisey)
{
	for (int i = 0; i < chislo_zapisey; i++)
		zapis2[i] = zapis[i];
	qsort(zapis2, chislo_zapisey, sizeof(db), compare_name);
	vivod_zapisey(zapis2, chislo_zapisey);
}

int compare_name(const void* av, const void* bv)
{
	const db* a = av, * b = bv;
	return -strcmp(a->name_app, b->name_app);
}
