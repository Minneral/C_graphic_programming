#include "Header.h"

//главная функция
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdParam,
	int nCmdShow)
{
	//регистрация класса окна приложения
	WNDCLASS wc;

	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDC_ARROW);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = cname;

	if (RegisterClass(&wc) == 0)
	{
		DWORD error = GetLastError();
		return 0;
	}

	HWND hWnd;

	hWnd = CreateWindow(cname,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
		return 0;

	//рисуем окно
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// запускаем цикл обработки сообщений
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		LineCreate();
		break;

	case WM_SIZE:
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		LinePaint(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		LButtonDown(x, y);
		break;

	case WM_LBUTTONUP:
		LButtonUp();
		break;

	case WM_MOUSEMOVE:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		MouseMove(x, y);
		if (Dat.Drawing == TRUE) {
			//LinePaint(hWnd);
			//LineDC(hWnd);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			angl.fi += 10;
			LineDC(hWnd);
			break;
		case VK_RIGHT:
			angl.fi -= 10;
			LineDC(hWnd);
			break;
		case VK_UP:
			angl.teta += 10;
			LineDC(hWnd);
			break;
		case VK_DOWN:
			angl.teta -= 10;
			LineDC(hWnd);
			break;
		default:
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

inline int xn(double x)
{
	return (int)((x - xLeft) / (xRight - xLeft) * (nRight - nLeft)) + nLeft;
}
inline int ym(double y)
{
	return (int)((y - yBottom) / (yTop - yBottom) * (mTop - mBottom)) + mBottom;
}

inline double Xe(double x, double y)
{
	return -sf * x + cf * y;
}
inline double Ye(double x, double y, double z)
{
	return -ct * cf * x - ct * sf * y + st * z;
}
inline double Ze(double x, double y, double z)
{
	return st * cf * x + st * sf * y + ct * z - d;
}

void LinePaint(HWND hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	nLeft = rc.left + 50; nRight = rc.right - 50;
	mBottom = rc.bottom - 50; mTop = rc.top + 50;

	PAINTSTRUCT ps;

	sf = sin(3.14159 * angl.fi / 180);
	cf = cos(3.14159 * angl.fi / 180);
	st = sin(3.14159 * angl.teta / 180);
	ct = cos(3.14159 * angl.teta / 180);

	HDC hdc = BeginPaint(hWnd, &ps); //дескриптор контекста устройства

	Picture(hdc);
	Axes(hdc);

	EndPaint(hWnd, &ps);
}
void LineCreate()
{
	//расстояние между центрами систем MC и DC
	d = 8;

	xLeft = -6; xRight = 6; yBottom = -6; yTop = 6;

	xmax = 4; ymax = 4; zmax = 4;

	angl.fi = 30; angl.teta = 60;

	P[0].x = 0; P[0].y = 0; P[0].z = 5;
	P[1].x = 4; P[1].y = 0; P[1].z = 0;
	P[2].x = 0; P[2].y = 4; P[2].z = 0;
	P[3].x = -4; P[3].y = 0; P[3].z = 0;
	P[4].x = 0; P[4].y = -4; P[4].z = 0;

	POINT3 pt[3];

	pt[0] = P[0]; pt[1] = P[1]; pt[2] = P[2];
	for (int i = 0; i < 3; i++)
	{
		Pln[0].P[i] = pt[i];
	}
	Pln[0].c.R = 200;
	Pln[0].c.G = 0;
	Pln[0].c.B = 0;

	pt[0] = P[0]; pt[1] = P[2]; pt[2] = P[3];
	for (int i = 0; i < 3; i++)
	{
		Pln[1].P[i] = pt[i];
	}
	Pln[1].c.R = 0;
	Pln[1].c.G = 200;
	Pln[1].c.B = 0;

	pt[0] = P[0]; pt[1] = P[3]; pt[2] = P[4];
	for (int i = 0; i < 3; i++)
	{
		Pln[2].P[i] = pt[i];
	}
	Pln[2].c.R = 0;
	Pln[2].c.G = 0;
	Pln[2].c.B = 200;

	pt[0] = P[0]; pt[1] = P[4]; pt[2] = P[1];
	for (int i = 0; i < 3; i++)
	{
		Pln[3].P[i] = pt[i];
	}
	Pln[3].c.R = 200;
	Pln[3].c.G = 0;
	Pln[3].c.B = 200;
}
void Axes(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);

	double xe, ye;
	int x1, y1, x2, y2;

	//рисуем Ox
	xe = Xe(-xmax / 3, 0);
	ye = Ye(-xmax / 3, 0, 0);

	x1 = xn(xe);
	y1 = ym(ye);

	xe = Xe(1.5 * xmax, 0);
	ye = Ye(1.5 * xmax, 0, 0);

	x2 = xn(xe);
	y2 = ym(ye);

	MoveToEx(hdc, x1, y1, 0);
	LineTo(hdc, x2, y2);

	//надпись на оси
	SetBkColor(hdc, RGB(0xC0, 0xC0, 0xC0));
	SetTextColor(hdc, RGB(120, 120, 120));
	TextOut(hdc, x2, y2, _T("X"), 1);


	//рисуем Oy
	xe = Xe(0, -ymax / 3);
	ye = Ye(0, -ymax / 3, 0);

	x1 = xn(xe);
	y1 = ym(ye);

	xe = Xe(0, 1.5 * ymax);
	ye = Ye(0, 1.5 * ymax, 0);

	x2 = xn(xe);
	y2 = ym(ye);

	MoveToEx(hdc, x1, y1, 0);
	LineTo(hdc, x2, y2);

	TextOut(hdc, x2, y2, _T("Y"), 1);

	//рисуем Oz
	xe = Xe(0, 0);
	ye = Ye(0, 0, -zmax / 3);

	x1 = xn(xe);
	y1 = ym(ye);

	xe = Xe(0, 0);
	ye = Ye(0, 0, 1.5 * zmax);

	x2 = xn(xe);
	y2 = ym(ye);

	MoveToEx(hdc, x1, y1, 0);
	LineTo(hdc, x2, y2);

	TextOut(hdc, x2, y2, _T("Z"), 1);

	//восстанавливаем в контексте старое перо
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
}
void Picture(HDC hdc)
{
	HPEN hpen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
	HPEN hpenOld = (HPEN)SelectObject(hdc, hpen);
	HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);

	//к-ты проектируемой точки в видовой сист коорд.
	double xe, ye;
	//к-ты пикселей
	int x1, y1;

	//рисуем опорные точки в виде кружков
	for (int i = 0; i < 5; i++)
	{
		xe = Xe(P[i].x, P[i].y);
		ye = Ye(P[i].x, P[i].y, P[i].z);

		x1 = xn(xe);
		y1 = ym(ye);

		Ellipse(hdc, x1 - 6, y1 - 6, x1 + 6, y1 + 6);
	}

	//соединяем вершины пирамиды ребрами
	Line2(hdc, 0, 1);
	Line2(hdc, 0, 2);
	Line2(hdc, 0, 3);
	Line2(hdc, 0, 4);
	Line2(hdc, 1, 2);
	Line2(hdc, 2, 3);
	Line2(hdc, 3, 4);
	Line2(hdc, 4, 1);

	//рисуем грани
	for (int i = 0; i < 4; i++)
	{
		Poligon3(hdc, i); // i
	}

	SelectObject(hdc, hbrushOld);
	SelectObject(hdc, hpenOld);
	DeleteObject(hpen);
	DeleteObject(hbrush);
}
void Line2(HDC hdc, int n, int m)
{
	HPEN hpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hpenOld = (HPEN)SelectObject(hdc, hpen);

	//к-ты проектируемой точки в видовой сист корд
	double xe, ye;
	//к-ты пикселей
	int x1, y1, x2, y2;

	//Рисуем ребро по паре вершин
	//--------- n - m -------------
	xe = Xe(P[n].x, P[n].y);
	ye = Ye(P[n].x, P[n].y, P[n].z);

	x1 = xn(xe);
	y1 = ym(ye);
	MoveToEx(hdc, x1, y1, 0);

	xe = Xe(P[m].x, P[m].y);
	ye = Ye(P[m].x, P[m].y, P[m].z);

	x2 = xn(xe);
	y2 = ym(ye);
	LineTo(hdc, x2, y2);

	SelectObject(hdc, hpenOld);
	DeleteObject(hpen);
}
void LButtonDown(int x, int y)
{
	Dat.ButtonDown = TRUE;
	Dat.Drawing = FALSE;

	anglOld.fi = angl.fi;
	anglOld.teta = angl.teta;
	corOld.x = x;
	corOld.y = y;
}
void MouseMove(int x, int y)
{
	if (Dat.ButtonDown == TRUE)
	{
		Dat.Drawing = TRUE;

		angl.fi += corOld.x - x;
		angl.teta += corOld.y - y;

		corOld.x = x; corOld.y = y;

		anglOld.fi = angl.fi;
		anglOld.teta = angl.teta;
	}
}
void LButtonUp()
{
	if (Dat.ButtonDown && Dat.Drawing)
	{
		Dat.Drawing = FALSE;
	}
	Dat.ButtonDown = FALSE;
}
void LineDC(HWND hwnd)
{
	RECT rc;
	GetClientRect(hwnd, &rc);

	int dn = (rc.right - rc.left) / 10;
	int dm = (rc.bottom - rc.top) / 10;

	nLeft = rc.left + dn; nRight = rc.right - dn;
	mBottom = rc.bottom - dm; mTop = rc.top + dm;

	HDC hdc = GetDC(hwnd); //дескриптор контекста устройства

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, nRight, mBottom);
	SelectObject(hdcMem, hBitmap);

	//отсекаем графические объекты вне окна вывода
	HRGN hrgn = CreateRectRgn(nLeft, mTop, nRight, mBottom);

	HBRUSH hbrush = CreateSolidBrush(RGB(0xC0, 0xC0, 0xC0));
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	SelectObject(hdcMem, hbrush);

	//ограничиваем область вывода изображения
	SelectClipRgn(hdcMem, hrgn);

	//заливаем область вывода
	FillRgn(hdcMem, hrgn, hbrush);

	//копируем контекст памяти в контекст экрана
	BitBlt(hdc, nLeft, mTop, nRight, mBottom, hdcMem, nLeft, mTop, SRCCOPY);

	SelectObject(hdcMem, hbrushOld);
	DeleteObject(hbrush);
	DeleteObject(hrgn);


	sf = sin(3.14159 * angl.fi / 180);
	cf = cos(3.14159 * angl.fi / 180);
	st = sin(3.14159 * angl.teta / 180);
	ct = cos(3.14159 * angl.teta / 180);

	Picture(hdc);
	Axes(hdc);

	DeleteObject(hBitmap);
	DeleteDC(hdcMem);

	ReleaseDC(hwnd, hdc);
	InvalidateRect(hwnd, NULL, TRUE);
}
void Poligon3(HDC hdc, int n)
{
	FacetS Plnt;
	Plnt = Pln[n];

	HPEN hpen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
	HPEN hpenOld = (HPEN)SelectObject(hdc, hpen);

	HBRUSH hbrush = CreateSolidBrush(RGB(Plnt.c.R, Plnt.c.G, Plnt.c.B));
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);

	//к-ты точки в мировой сист. корд
	double x, y, z;
	//к-ты проекции точки в видовой сист корд
	double xe, ye, ze;

	//создаем полигон в экранных кордах
	POINT P1[3];

	for (int i = 0; i < 3; i++)
	{
		x = Plnt.P[i].x; y = Plnt.P[i].y; z = Plnt.P[i].z;
		xe = Xe(x, y); ye = Ye(x, y, z);
		P1[i].x = xn(xe); P1[i].y = ym(ye);
	}

	//рисуем полигон
	Polygon(hdc, P1, 3);

	SelectObject(hdc, hbrushOld);
	DeleteObject(hbrush);

	SelectObject(hdc, hpenOld);
	DeleteObject(hpen);
}