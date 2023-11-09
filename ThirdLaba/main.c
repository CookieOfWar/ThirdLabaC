#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#define MAXLEN 300
#include <locale.h>
#include <stdlib.h>

typedef struct list { //Объявление структуры list
	int Val;
	int Row;
	struct list* Next;
}list;

list* newlist(int value, int row);         //Создание заглавной структуры списка
list* add(int value, int row, list* prevp); //Добавление структуры в конец списка
void FindMaxNeibSum(int N, int M, int* maxrow, int* maxcol); //Определить максимальную сумму соседей
void printlist(list** STR, int N); //Вывести полноценную "структуру", "массив"
void deleteitem(list* prev, list* delv); //Удалить структуру из списка

int main() {
	setlocale(0, "Rus"); //Установка русского языка

	FILE* fp; //Указатель на файл
	int chislo, N, M, a; //Просматриваемое число, кол-во строк, кол-во столбцов, возращаемое значение для fscanf()
	char tmpc; //Временный символ для fscanf()
	fp = fopen("E:/Programming/C/ThirdLaba/matrix.txt", "r"); //Открытие файла
	if (fp == NULL) { //проверка файла на существование
		perror("ERROR");
		return 0;
	}

	for (int j = 0; j < MAXLEN; j++) { //Подсчет столбцов
		a = fscanf(fp, "%d%c", &chislo, &tmpc);
		if (tmpc == '\n') {
			M = j + 1;
			break;
		}
	}
	
	fclose(fp); //Закрытие файла для сброса прочтенного места в fscanf()
	fp = fopen("E:/Programming/C/ThirdLaba/matrix.txt", "r");
	if (fp == NULL) {
		perror("ERROR");
		return 0;
	}

	for (int i = 0; i < MAXLEN; i++) { //Подсчет строк
		int flag = 0; //Вместо EOF
		 while(1){
			a = fscanf(fp, "%d", &chislo);
			if (a != 1) { //Если вышли за пределы строк
				N = i + 1;
				flag = 1;
				break;
			}
			a = fscanf(fp, "%c", &tmpc);
			if (tmpc == '\n')
				break;
		 }
		if (flag == 1)
			break;
	}

	list** STR;  //Массив из указателей на структуру
	STR = (list**)malloc(N * sizeof(list*)); //Выделение памяти под массив из подсчитанныхстрок и столбцов
	if (STR) { //Проверка на удачность выделения памяти
		for (int i = 0; i < N; i++) {
			STR[i] = NULL; //Обнуление указателей массива
		}
	}
	else
		return 0;

	list* tmp; //Динамический указатель на структуру (постоянно меняется в программе)
	tmp = NULL;

	fclose(fp);
	fp = fopen("E:/Programming/C/ThirdLaba/matrix.txt", "r");
	if (fp == NULL) {
		perror("ERROR");
		return 0;
	}

	for (int i = 0; i < N; i++) { //Добавление всех элементов матрицы, не равных 0, в списки структур
		int newline = 1;
		for (int j = 0; j < M; j++) {
			a = fscanf(fp, "%d%*c", &chislo);
			if (chislo != 0) {
				if (newline) {
					tmp = newlist(chislo, j);
					STR[i] = tmp;
					newline = 0;
				}
				else {
					tmp = add(chislo, j, tmp);
				}
			}
		}
	}
	printf("Старая структура: ");
	printlist(STR, N);
	fclose(fp);

	int maxrow, maxcol; //Строка и столбик элемента с максимальной суммой соседей
	FindMaxNeibSum(N, M, &maxrow, &maxcol);
	printf("\n\n%d   %d", maxrow, maxcol); //Вывод найденных координат

	tmp = STR[maxrow];
	int ischange = 0; //Изменились ли списки структур (нужно только для красивого вывода)

	//Проверка есть ли элемент с данными координатами в списках
	if (tmp == NULL);
	else{
		if (tmp->Row == maxcol) {
			STR[maxrow] = (tmp->Next);
			ischange = 1;
			free(tmp);
		}
		else {
			while (1) {
				if (tmp == NULL) {
					break;
				}
				if ((tmp->Next)!=NULL) {
					if ((tmp->Next)->Row == maxcol) {
						deleteitem(tmp, (tmp->Next));
						ischange = 1;
						tmp = tmp->Next;
						break;
					}
					else
						tmp = tmp->Next;
				}
				else {
					break;
				}
			}
		}
	}
	//Тот самый красивый вывод
	if (ischange) {
		printf("\nИзмененная структура:\n");
		printlist(STR, N);
	}
	else
		printf("\nСтруктура не изменилась");
}



list* newlist(int value, int row) {
	list* tmp = (list*)malloc(sizeof(list));
	if (tmp) {
		tmp->Next = NULL;
		tmp->Val = value;
		tmp->Row = row;
		return tmp;
	}
	else
		exit(-1);
}

list* add(int value, int row, list* prevp) {
	list* tmp = (list*)malloc(sizeof(list));
	if (tmp) {
		prevp->Next = tmp;
		tmp->Val = value;
		tmp->Row = row;
		tmp->Next = NULL;
		return tmp;
	}
	else
		exit(-1);
}

void printlist(list** STR, int N) {
	for (int i = 0; i < N; i++) {
		printf("\n[]");
		if (STR[i] == NULL);
		else {
			list* current = STR[i];
			while (1) {
				printf(" %d %d ->", current->Val, current->Row);
				if (current->Next == NULL) {
					printf(" NULL");
					break;
				}
				current = current->Next;
			}
		}
	}
}

//Эту функцию очень больно объяснять, я потратил на нее хрен знает сколько времени
void FindMaxNeibSum(int N, int M, int *maxrow, int *maxcol) {
	FILE* filep = fopen("E:/Programming/C/ThirdLaba/matrix.txt", "r");
	if (filep == NULL) {
		perror("ERROR");
		exit(-1);
	}
	int chislo, maxsum, prevelem, a;
	int firstelem = 1;
	int currsum = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			currsum = 0;
			if (firstelem) {
				a = fscanf(filep, "%d%*c", &chislo);
				prevelem = chislo;
				a = fscanf(filep, "%d%*c", &chislo);
				maxsum = chislo;
				firstelem = 0;
			}
			else {
				if (j > 0) {
					currsum += prevelem;
					prevelem = chislo;
					if (j < M - 1) {
						a = fscanf(filep, "%d%*c", &chislo);
						currsum += chislo;
					}
				}
				else {
					a = fscanf(filep, "%d%*c", &chislo);
					prevelem = chislo;
					a = fscanf(filep, "%d%*c", &chislo);
					currsum += chislo;
				}
			}
			if (currsum >= maxsum) {
				maxsum = currsum;
				*maxrow = i;
				*maxcol = j;
			}
		}
	}
}

void deleteitem(list* prev, list* delv) {
	prev->Next = delv->Next;
	free(delv);
}