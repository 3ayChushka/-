#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <stddef.h>

int new_zapis(zapis, chislo_zapisey);  /*Добавление новой записи*/
int vivod_zapisey(zapis, chislo_zapisey); /* Вывод ВСЕХ записей*/
int poisk(zapis, chislo_zapisey);  /*Поиск*/
int vivod_zapisey_poiska(zapis,chislo_zapisey);  /*Вывод результата поиска*/

struct DataBase
{
	char name_app[50];
	int code;
	char lvl;
	struct tm time[100];
};

typedef struct DataBase db;

int main()
{
	setlocale(LC_ALL, "RUS");
	
	int vibor, chislo_zapisey = 0;
	db zapis[100], *ptr_zapis= &zapis[chislo_zapisey];

	do {
		printf("Выберите действие(работает 1-3)\n1-Добавить новую запись\n2-Поиск по полю\n3-Вывод записей\n4-Изменить запись\n5-Отсортировать по \"Частота событий на приложение\"\n6-Добавить произвольное количество записей\n0-Выход\n\n");
		vibor = _getch();

		if (vibor == '1')/*Добавление новой записи*/
			chislo_zapisey = new_zapis(ptr_zapis, chislo_zapisey);  
		else if (vibor == '2')  /*Поиск по полю*/
		{
			printf("Введите поле для поиска:\n1-Название\n2-Код операции\n3-Уровень события\n4-Время операции\n");
			poisk(ptr_zapis, chislo_zapisey);
		}
		else if (vibor == '3') /*Вывод всех записей*/
		vivod_zapisey(ptr_zapis, chislo_zapisey);  
	} while (vibor != '0');
}

int new_zapis(db *zapis, int chislo_zapisey) /*Добавление новой записи(пока в цикле)*/
{
	for (int i = 0; i <= 0; i++)
	{
		chislo_zapisey++;
		printf("Название приложения:\n"); /*Название приложения*/
		if(chislo_zapisey==1)fgets(zapis[chislo_zapisey].name_app, 50, stdin);
		else
		{
			getchar();
			fgets(zapis[chislo_zapisey].name_app, 50, stdin);
		}

		printf("Код операции:\n");  /*Код операции*/
		scanf("%d", &zapis[chislo_zapisey].code);

		printf("Выберите уровень события:\nA-Предупреждение\nB-Угроза\nC-Сбой\n");  /*Уровень события*/
		getchar();
		scanf("%c", &zapis[chislo_zapisey].lvl);

		int hour, min, sec;
		printf("Введите время в формате __:__:__\n");   /*Время операции*/
		getchar();
		scanf("%d:%d:%d", &hour, &min, &sec);

		zapis[chislo_zapisey].time[chislo_zapisey].tm_hour = hour;
		zapis[chislo_zapisey].time[chislo_zapisey].tm_min = min;
		zapis[chislo_zapisey].time[chislo_zapisey].tm_sec = sec;
	}
	return chislo_zapisey;
}  

int vivod_zapisey(db *zapis, int chislo_zapisey)  /*Вывод записей*/
{
	for (int chislo_zapisey_vivod = 1; chislo_zapisey_vivod <= chislo_zapisey; chislo_zapisey_vivod++) /*Вывод записей*/
	{
		printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[chislo_zapisey_vivod].name_app, zapis[chislo_zapisey_vivod].code, zapis[chislo_zapisey_vivod].lvl, zapis[chislo_zapisey_vivod].time[chislo_zapisey_vivod].tm_hour, zapis[chislo_zapisey_vivod].time[chislo_zapisey_vivod].tm_min, zapis[chislo_zapisey_vivod].time[chislo_zapisey_vivod].tm_sec);
	}
	return 0;
}

int vivod_zapisey_poiska(db* zapis, int chislo_zapisey)  /*Вывод записей по поиску*/
{
	printf("Название приложения: %sКод события: %d\nУровень события: %c\nВремя события: %d:%d:%d\n\n", zapis[chislo_zapisey].name_app, zapis[chislo_zapisey].code, zapis[chislo_zapisey].lvl, zapis[chislo_zapisey].time[chislo_zapisey].tm_hour, zapis[chislo_zapisey].time[chislo_zapisey].tm_min, zapis[chislo_zapisey].time[chislo_zapisey].tm_sec);
	return 0;
}

int poisk(db *zapis, int chislo_zapisey)  /*Поиск по заданному полю*/
{
	int vibor_poiska;
	char name_poick[50];

	vibor_poiska = _getch();/*Выбор поля для поиска*/

	switch (vibor_poiska)
	{
	case '1':  /*Поиск по названию*/
		{
			printf("Введите название:\n");
			getchar();
			fgets(name_poick, 50, stdin);
			for (int i = 1; i <= chislo_zapisey; i++)
				if (strcmp(name_poick,zapis[i].name_app)==0) vivod_zapisey_poiska(zapis, i);
			break;
		}
	case '2':  /*Поиск по коду */
	{
		printf("Введите код операции\n");
		int code_poisk;
		scanf("%d", &code_poisk);
		for (int i = 1; i <= chislo_zapisey; i++)
			if (code_poisk == zapis[i].code) vivod_zapisey_poiska(zapis, i);
		break;
	}
	case '3':  /*Поиск по уровню события*/
	{
		printf("Введите уровень события\n");
		char lvl_poisk;
		getchar();
		scanf("%c", &lvl_poisk);
		for (int i = 1; i <= chislo_zapisey; i++)
			if (lvl_poisk== zapis[i].lvl) vivod_zapisey_poiska(zapis, i);
		break;
	}
	case '4':  /*Поиск по времени*/
	{
		printf("Введите время __:__:__\n");
		int hour_poisk, min_poisk, sec_poisk;
		scanf("%d:%d:%d", &hour_poisk, &min_poisk, &sec_poisk);
		for (int i = 1; i <= chislo_zapisey; i++)
			if (zapis[i].time[i].tm_hour == hour_poisk && zapis[i].time[i].tm_min == min_poisk && zapis[i].time[i].tm_sec == sec_poisk) vivod_zapisey_poiska(zapis, i);
		break;
	}
	}
	return 0;
}