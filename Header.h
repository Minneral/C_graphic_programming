#pragma once

#include <windows.h>
#include <math.h>
#include <tchar.h>

TCHAR cname[] = _T("Class");
TCHAR title[] = _T("3.13");

#define M_PI           3.14159265358979323846

// ������� ��� ������ � ������� ������� ���������
double xLeft, xRight, yBottom, yTop;

// ������� ��� ������ � ������� � ���������� ������� ����
int nLeft, nRight, mBottom, mTop;

int x, y;

//���������� ����� �������� ������ MC � DC
double d;

//���� �������� ������� ������� ���������
struct ANGLS {
	double fi, teta;
};
ANGLS angl, anglOld;

//�������� ������������
double sf, cf, st, ct;

//������������ ����� ��������� ���� - ������� ����������
double xmax, ymax, zmax;

double xm;

//���������� ����� � ������� ������� ���������
struct POINT3 {
	double x, y, z;
};
POINT3 P[5];

//��������� ��� ���� ������ ������ RGB
struct ColorS {
	unsigned R, G, B;
};
// ����� �������� � ���� ������
unsigned long Np, Mp, NM;

//��������� ������ ������
struct FacetS {
	POINT3 P[3]; // ��� ������� �����
	double I[3]; // ������������� � ��������
	ColorS c; //���� �����
	double A, B, C, D;
};

const int NF = 4;
FacetS Pln[NF];

//����� ��� ������ � ������
struct TDATA {
	BOOL ButtonDown;
	BOOL Drawing;
};
TDATA Dat;

//���������� ����
struct CORD {
	int x, y;
};
CORD corOld;

// ��������� �������
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
