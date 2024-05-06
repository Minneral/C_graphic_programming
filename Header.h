#pragma once

#include <windows.h>
#include <math.h>
#include <tchar.h>

TCHAR cname[] = _T("Class");
TCHAR title[] = _T("3.13");

#define M_PI           3.14159265358979323846

// размеры пол¤ вывода в мировой системе координат
double xLeft, xRight, yBottom, yTop;

// размеры пол¤ вывода в пиксел¤х в клиентской области окна
int nLeft, nRight, mBottom, mTop;

int x, y;

//расстояние между центрами систем MC и DC
double d;

//углы поворота видовой системы координат
struct ANGLS {
	double fi, teta;
};
ANGLS angl, anglOld;

//уголовые коэффициенты
double sf, cf, st, ct;

//максимальные длины координат осей - мировые координаты
double xmax, ymax, zmax;

double xm;

//координаты точек в мировой системе координат
struct POINT3 {
	double x, y, z;
};
POINT3 P[5];

//структура для трех цветов модели RGB
struct ColorS {
	unsigned R, G, B;
};
// число пикселей в окне вывода
unsigned long Np, Mp, NM;

//структура граней фигуры
struct FacetS {
	POINT3 P[3]; // три вершины грани
	double I[3]; // интенсивности в вершинах
	ColorS c; //цвет грани
	double A, B, C, D;
};

const int NF = 4;
FacetS Pln[NF];

//флаги для работы с мышкой
struct TDATA {
	BOOL ButtonDown;
	BOOL Drawing;
};
TDATA Dat;

//координаты мыши
struct CORD {
	int x, y;
};
CORD corOld;

// прототипы функции
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LinePaint(HWND);
inline int ym(double);
inline int xn(double);
inline double Xe(double, double);
inline double Ye(double, double, double);
inline double Ze(double, double, double);
void LineCreate();
void Axes(HDC);
void Picture(HDC);
void Line2(HDC, int, int);
void LButtonDown(int, int);
void MouseMove(int, int);
void LButtonUp();
void LineDC(HWND);
void Poligon3(HDC, int);
