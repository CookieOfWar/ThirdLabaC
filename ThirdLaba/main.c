#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#define MAXLEN 300
#include <locale.h>
#include <stdlib.h>

typedef struct list { //���������� ��������� list
	int Val;
	int Row;
	struct list* Next;
}list;

list* newlist(int value, int row);         //�������� ��������� ��������� ������
list* add(int value, int row, list* prevp); //���������� ��������� � ����� ������
void FindMaxNeibSum(int N, int M, int* maxrow, int* maxcol); //���������� ������������ ����� �������
void printlist(list** STR, int N); //������� ����������� "���������", "������"
void deleteitem(list* prev, list* delv); //������� ��������� �� ������

int main() {
	setlocale(0, "Rus"); //��������� �������� �����

	FILE* fp; //��������� �� ����
	int chislo, N, M, a; //��������������� �����, ���-�� �����, ���-�� ��������, ����������� �������� ��� fscanf()
	char tmpc; //��������� ������ ��� fscanf()
	fp = fopen("E:/Programming/C/ThirdLaba/matrix.txt", "r"); //�������� �����
	if (fp == NULL) { //�������� ����� �� �������������
		perror("ERROR");
		return 0;
	}

	for (int j = 0; j < MAXLEN; j++) { //������� ��������
		a = fscanf(fp, "%d%c", &chislo, &tmpc);
		if (tmpc == '\n') {
			M = j + 1;
			break;
		}
	}
	
	fclose(fp); //�������� ����� ��� ������ ����������� ����� � fscanf()
	fp = fopen("E:/Programming/C/ThirdLaba/matrix.txt", "r");
	if (fp == NULL) {
		perror("ERROR");
		return 0;
	}

	for (int i = 0; i < MAXLEN; i++) { //������� �����
		int flag = 0; //������ EOF
		 while(1){
			a = fscanf(fp, "%d", &chislo);
			if (a != 1) { //���� ����� �� ������� �����
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

	list** STR;  //������ �� ���������� �� ���������
	STR = (list**)malloc(N * sizeof(list*)); //��������� ������ ��� ������ �� ����������������� � ��������
	if (STR) { //�������� �� ��������� ��������� ������
		for (int i = 0; i < N; i++) {
			STR[i] = NULL; //��������� ���������� �������
		}
	}
	else
		return 0;

	list* tmp; //������������ ��������� �� ��������� (��������� �������� � ���������)
	tmp = NULL;

	fclose(fp);
	fp = fopen("E:/Programming/C/ThirdLaba/matrix.txt", "r");
	if (fp == NULL) {
		perror("ERROR");
		return 0;
	}

	for (int i = 0; i < N; i++) { //���������� ���� ��������� �������, �� ������ 0, � ������ ��������
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
	printf("������ ���������: ");
	printlist(STR, N);
	fclose(fp);

	int maxrow, maxcol; //������ � ������� �������� � ������������ ������ �������
	FindMaxNeibSum(N, M, &maxrow, &maxcol);
	printf("\n\n%d   %d", maxrow, maxcol); //����� ��������� ���������

	tmp = STR[maxrow];
	int ischange = 0; //���������� �� ������ �������� (����� ������ ��� ��������� ������)

	//�������� ���� �� ������� � ������� ������������ � �������
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
	//��� ����� �������� �����
	if (ischange) {
		printf("\n���������� ���������:\n");
		printlist(STR, N);
	}
	else
		printf("\n��������� �� ����������");
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

//��� ������� ����� ������ ���������, � �������� �� ��� ���� ����� ������� �������
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