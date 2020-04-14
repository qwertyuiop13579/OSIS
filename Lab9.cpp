// Lab6.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab9.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
int x = 200;
int y = 200;
int xglobal = 200;
int yglobal = 200;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void Draw(HDC, int, int);
void DrawBitmap(HDC hDC, HBITMAP hBitmap, RECT Rect);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB9, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB9));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB9));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB9);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}
//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT Rect;
	HDC hdc, hCmpDC;
	HBITMAP hBmp;
	int xcoor, ycoor;
	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_LBUTTONDOWN:
		xcoor = LOWORD(lParam);         //узнаём координаты
		ycoor = HIWORD(lParam);
		xglobal = xcoor;
		yglobal = ycoor;
		KillTimer(hWnd, 1);              //Убиваем таймер
		SetTimer(hWnd, 1, 5, NULL);      //Устанавливаем таймер
		InvalidateRect(hWnd, NULL, FALSE); //вызов события WM_PAINT
		break;

	case WM_TIMER:
		hdc = BeginPaint(hWnd, &ps);
		//FillRect(hdc, &Rect, CreateSolidBrush(RGB(255, 255, 255)));
		if (x == xglobal and y == yglobal)
		{
			KillTimer(hWnd, 1);     //Убиваем таймер
			InvalidateRect(hWnd, NULL, FALSE); //вызов события WM_PAINT
		}
		else
		{
			if (x < xglobal) x += 1;
			else if (x > xglobal) x -= 1;
			if (x == xglobal) if (y < yglobal) y += 1;
			else if (y > yglobal) y -= 1;
			InvalidateRect(hWnd, NULL, FALSE); //вызов события WM_PAINT
		}
		break;
	case WM_PAINT:
	{
		GetClientRect(hWnd, &Rect);
		hdc = BeginPaint(hWnd, &ps);

		// Создание теневого контекста для двойной буферизации
		hCmpDC = CreateCompatibleDC(hdc);
		hBmp = CreateCompatibleBitmap(hdc, Rect.right - Rect.left, Rect.bottom - Rect.top);
		SelectObject(hCmpDC, hBmp);

		// Закраска фоновым цветом
		HBRUSH hBrush;                                 //создаём объект-кисть
		hBrush = CreateSolidBrush(RGB(255, 255, 255)); //задаём сплошную кисть, закрашенную цветом RGB
		FillRect(hCmpDC, &Rect, hBrush);
		DeleteObject(hBrush);

		// Здесь рисуем на контексте hCmpDC

		
		//BITMAP bitmap;
		//HBITMAP hbmOld;
		//HDC hdcMem;
		//hdcMem = CreateCompatibleDC(hdc);
		//DrawBitmap(hdca, hBitmap, r1);
		//GetObject(hbit1, sizeof(bitmap), &bitmap);
		//BitBlt(hCmpDC, x-16,y-16, 32, 32, hdcMem, 0, 0, SRCCOPY);
		//hbmOld = (HBITMAP)SelectObject(hdcMem, hbit1);
		HBITMAP hbitD, hbitL, hbitR, hbitU, hbitNeed;
		hbitL = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(144));
		hbitD = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(145));
		hbitR = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(146));
		hbitU = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(147));
		if (x < xglobal) hbitNeed = hbitR;
		else if (x > xglobal) hbitNeed = hbitL;
		else if (y > yglobal) hbitNeed = hbitD;
		else if (y < yglobal) hbitNeed = hbitU;
		else hbitNeed = hbitU;


		RECT r;
		r.left = x - 16;
		r.top = y - 16;
		r.right = x + 16;
		r.bottom = y + 16;

		DrawBitmap(hCmpDC, hbitNeed, r);

		//HICON hCurrIcon;
		//hCurrIcon = (HICON)LoadIconW(hInst, MAKEINTRESOURCE(130));
		//if (!DrawIconEx(hCmpDC, x-16, y-16, (HICON)hCurrIcon, 32, 32, 0, NULL, DI_NORMAL)) {}

		// Копируем изображение из теневого контекста на экран
		SetStretchBltMode(hdc, COLORONCOLOR);
		BitBlt(hdc, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, hCmpDC, 0, 0, SRCCOPY);

		// Удаляем ненужные системные объекты
		DeleteDC(hCmpDC);
		DeleteObject(hBmp);
		hCmpDC = NULL;
		EndPaint(hWnd, &ps);
		break;
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
void DrawBitmap(HDC hDC, HBITMAP hBitmap, RECT Rect)
{
	BITMAP Bitmap;
	// Получаем размерность BMP
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	HDC hCompatibleDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hCompatibleDC, hBitmap);
	// Перетаскиваем битмап на окно c маштабированием
	StretchBlt(hDC, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, hCompatibleDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
	//Делаем старый битмап текущим
	SelectObject(hCompatibleDC, hOldBitmap);
	// Удаляем
	DeleteDC(hCompatibleDC);
}

