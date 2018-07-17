// matrix-lr2-supercomp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <intrin.h>
#include <iostream>
#include <conio.h>

#pragma intrinsic(__rdtsc)

using namespace std;

unsigned __int64 t1, t2;
double T;
__int64 FRENC = 4*1000.0*1000.0*1000.0;
double value; //для проверки правильности подсчета

//-------------------------------------------------------------------
//Анализ влияния производительности памяти на производительность программы
//--------------------------------------------------------------------
int blocks[5] = { 2,100,200,500,1000 };
int n = 2000;
int main()
{
	setlocale(LC_ALL, "Russian");
	//-------------------------------------------------------------------
	//Инициализация матриц
	//--------------------------------------------------------------------
	int **a, **b, **c;
	a = static_cast<int **>(malloc(n * sizeof(int*)));
	b = static_cast<int **>(malloc(n * sizeof(int*)));
	c = static_cast<int **>(malloc(n * sizeof(int*)));
	for (auto i = 0; i < n; i++) {
		a[i] = static_cast<int*>(malloc(n * sizeof(int)));
		b[i] = static_cast<int*>(malloc(n * sizeof(int)));
		c[i] = static_cast<int*>(malloc(n * sizeof(int)));
	}
	printf("Созданы три квадратные матрицы (a,b,c) со стороной n=%d\n", n);
	for (int j = 0; j < n; j++)
	{
		for (int m = 0; m < n; m++)
		{
			b[j][m] = m;
			a[j][m] = m;
			c[j][m] = 0;
		}
	}
	printf("Матрицы заполнены элементами\n");
	//----------------------------------------------------------------
	//Перемножение матриц обычным способом
	//----------------------------------------------------------------
	printf("Перемножение матриц обычным способом\n");
	t1 = __rdtsc();
	for (int j = 0; j < n; j++)
		for (int m = 0; m < n; m++)
			for (int l = 0; l < n; l++)
				c[j][m] += a[j][l] * b[l][m];
	t2 = __rdtsc();
	value = 0;
	for (int j = 0; j < n; j++) for (int m = 0; m < n; m++) value += c[j][m];
	T = (t2 - t1) / FRENC;
	printf("n=%d, T=%fsec, value=%f\n", n, T, value);
	//----------------------------------------------------------------
	//Обнуление результирующей матрицы
	//----------------------------------------------------------------
	for (int j = 0; j < n; j++) for (int m = 0; m < n; m++) c[j][m] = 0;
	//----------------------------------------------------------------
	//Транспонирование второй матрицы
	//----------------------------------------------------------------
	printf("Перемножение матрицы на транспонированную\n");
	t1 = __rdtsc();
	for (int j = 0; j < n - 1; j++)
		for (int m = j + 1; m < n; m++)
		{
			value = b[j][m];
			b[j][m] = b[m][j];
			b[m][j] = value;
		}
	for (int j = 0; j < n; j++)
		for (int m = 0; m < n; m++)
			for (int l = 0; l < n; l++)
				c[j][m] += a[j][l] * b[m][l];
	t2 = __rdtsc();
	value = 0;
	for (int j = 0; j < n; j++) for (int m = 0; m < n; m++) value += c[j][m];
	T = (t2 - t1) / FRENC;
	printf("n=%d, T=%fsec, value=%f\n", n, T, value);
	//----------------------------------------------------------------
	//Обнуление результирующей матрицы
	//----------------------------------------------------------------
	for (int j = 0; j < n; j++) for (int m = 0; m < n; m++) c[j][m] = 0;
	//----------------------------------------------------------------
	//Обратное транспонирование второй матрицы
	//----------------------------------------------------------------
	for (int j = 0; j < n - 1; j++)
		for (int m = j + 1; m < n; m++)
		{
			value = b[j][m];
			b[j][m] = b[m][j];
			b[m][j] = value;
		}
	//----------------------------------------------------------------
	//Блочное перемножение
	//----------------------------------------------------------------
	printf("Блочное перемножение\n");
	for (int ii = 0; ii < 5; ii++)
	{
		auto nblock = blocks[ii];
		printf("nblock=%d:\n", nblock);
		t1 = __rdtsc();
		for (int i = 0; i < n; i += nblock)
			for (int j = 0; j < n; j += nblock)
				for (int m = 0; m < n; m += nblock)
					for (int l = 0; l < nblock; l++)
						for (int k = 0; k < nblock; k++)
							for (int h = 0; h < nblock; h++)
								c[i + l][j + k] += a[i + l][m + h] * b[m + h][j + k];
		t2 = __rdtsc();
		value = 0;
		for (int j = 0; j < n; j++) for (int m = 0; m < n; m++) value += c[j][m];
		T = (t2 - t1) / FRENC;
		printf("n=%d, T=%fsec, value=%f\n", n, T, value);
	}
	system("pause");
	return 0;
}