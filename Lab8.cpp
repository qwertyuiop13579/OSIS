// Lab6.cpp : Определяет точку входа для приложения.
//
#define OEMRESOURCE
#include "stdafx.h"
#include "Lab8.h"
#include "resource.h"
#include "winuser.h"

#define MAX_LOADSTRING 100
#define ID_BUTTON_1 3000
#define ID_BUTTON_2 3001

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hBtn1;
HWND hBtn2;
HWND g_myStatic;


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
	LoadStringW(hInstance, IDC_LAB8, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB8));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB8));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB8);
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

	hBtn1 = CreateWindowW(L"BUTTON", L"Draw", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_OWNERDRAW, 10, 10, 80, 50, hWnd, (HMENU)ID_BUTTON_1, hInstance, NULL);
	hBtn2 = CreateWindowW(L"BUTTON", L"Clear", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_OWNERDRAW, 100, 10, 80, 50, hWnd, (HMENU)ID_BUTTON_2, hInstance, NULL);

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
		case ID_BUTTON_1:                   //Draw
			PAINTSTRUCT ps;
			HDC hdc;
			int x1, y1;
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Добавьте сюда любой код прорисовки, использующий HDC...

			HBRUSH hBrush1;                               //создаём white объект-кисть
			hBrush1 = CreateSolidBrush(RGB(255, 255, 255));  //задаём сплошную кисть, закрашенную цветом RGB
			HBRUSH hBrush2;                               //создаём  blue объект-кисть
			hBrush2 = CreateSolidBrush(RGB(0, 0, 255));  //задаём сплошную кисть, закрашенную цветом RGB
			HBRUSH hBrush3;                               //создаём  red объект-кисть
			hBrush3 = CreateSolidBrush(RGB(255, 0, 0));  //задаём сплошную кисть, закрашенную цветом RGB
			HPEN hPen1; //Объявляется черную кисть
			hPen1 = CreatePen(1,2, RGB(0, 0, 0));			
			HPEN hPen2; //Объявляется  mallon кисть
			hPen2 = CreatePen(1, 6, RGB(128, 0, 0));
			HPEN hPen3; //Объявляется  green кисть
			hPen3 = CreatePen(1, 2, RGB(0, 128, 0));
			HPEN hPen4; //Объявляется  red кисть
			hPen4 = CreatePen(1, 2, RGB(255, 0, 0));
			HPEN hPen5; //Объявляется  blue кисть
			hPen5 = CreatePen(1, 1, RGB(0, 0, 255));

			x1 = 200;
			y1 = 250;

			SelectObject(hdc, hBrush1);
			SelectObject(hdc, hPen1);
			Ellipse(hdc, x1 - 50, y1 - 50, x1 + 50, y1 + 50);         // Тело
			Ellipse(hdc, x1 - 70, y1 + 50, x1 + 70, y1 + 190);
			Ellipse(hdc, x1 - 30, y1 - 110, x1 + 30, y1 - 50);
			Ellipse(hdc, x1 - 60, y1 + 190 - 15, x1 - 10, y1 + 190 + 15);
			Ellipse(hdc, x1 + 10, y1 + 190 - 15, x1 + 60, y1 + 190 + 15);

			SelectObject(hdc, hPen2);
			MoveToEx(hdc, x1 - 46, y1 - 20, NULL);                  //Руки
			LineTo(hdc, x1 - 80, y1 - 50);
			LineTo(hdc, x1 - 80, y1 - 70);
			MoveToEx(hdc, x1 - 80, y1 - 50, NULL);
			LineTo(hdc, x1 - 95, y1 - 45);
			MoveToEx(hdc, x1 + 46, y1 - 20, NULL);
			LineTo(hdc, x1 + 80, y1 - 50);
			LineTo(hdc, x1 + 80, y1 - 70);
			MoveToEx(hdc, x1 + 80, y1 - 50, NULL);
			LineTo(hdc, x1 + 95, y1 - 45);

			SelectObject(hdc, hPen3);                          //Глаза Нос Рот
			Ellipse(hdc, x1 - 15, y1 - 110 + 15, x1 - 9, y1 - 110 + 25);
			Ellipse(hdc, x1 + 15, y1 - 110 + 15, x1 + 9, y1 - 110 + 25);
			SelectObject(hdc, hPen4);
			MoveToEx(hdc, x1, y1 - 85, NULL);
			LineTo(hdc, x1 - 8, y1 - 75);
			LineTo(hdc, x1, y1 - 75);
			SelectObject(hdc, hPen1);
			MoveToEx(hdc, x1 - 10, y1 - 65, NULL);
			LineTo(hdc, x1 + 10, y1 - 65);

			SelectObject(hdc, hPen5);                   //Пуговицы
			SelectObject(hdc, hBrush1);
			Arc(hdc, x1 - 5, y1 - 40, x1 + 5, y1 - 30, x1 - 5, y1 - 40, x1 + 5, y1 - 30);
			Arc(hdc, x1 - 5, y1 - 40, x1 + 5, y1 - 30, x1 + 5, y1 - 30, x1 - 5, y1 - 40);
			Arc(hdc, x1 - 5, y1 - 20, x1 + 5, y1 - 10, x1 - 5, y1 - 20, x1 + 5, y1 - 10);
			Arc(hdc, x1 - 5, y1 - 20, x1 + 5, y1 - 10, x1 + 5, y1 - 10, x1 - 5, y1 - 20);
			Arc(hdc, x1 - 5, y1, x1 + 5, y1 + 10, x1 - 5, y1, x1 + 5, y1 + 10);
			Arc(hdc, x1 - 5, y1, x1 + 5, y1 + 10, x1 + 5, y1 + 10, x1 - 5, y1);
			Arc(hdc, x1 - 5, y1 + 20, x1 + 5, y1 + 30, x1 - 5, y1 + 20, x1 + 5, y1 + 30);
			Arc(hdc, x1 - 5, y1 + 20, x1 + 5, y1 + 30, x1 + 5, y1 + 30, x1 - 5, y1 + 20);

			SelectObject(hdc, hPen3);                     //Ведро
			SelectObject(hdc, hBrush3);
			POINT poly[4];
			poly[0].x = x1 - 25;
			poly[0].y = y1 - 101;
			poly[1].x = x1 + 25;
			poly[1].y = y1 - 101;
			poly[2].x = x1 + 15;
			poly[2].y = y1 - 140;
			poly[3].x = x1 - 15;
			poly[3].y = y1 - 140;
			Polygon(hdc, poly, 4);              //многоугольник

			EndPaint(hWnd, &ps);         // заканчиваем рисовать
			break;
		case ID_BUTTON_2:                   //Clear
			InvalidateRect(hWnd, 0, TRUE);
			UpdateWindow(hWnd);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
		//PAINTSTRUCT ps;
		//HDC hdc;
		//hdc = BeginPaint(hWnd, &ps);
		//здесь можно вставить какие-нибудь функции рисования
		
		//ValidateRect(hWnd, NULL);      //обновляем окно		
		//EndPaint(hWnd, &ps);       //заканчиваем рисовать
		break; 
	case WM_DRAWITEM:
		static DRAWITEMSTRUCT* pdis;
		pdis = (DRAWITEMSTRUCT*)lParam;
		static RECT rect;
		HICON hCurrIcon;
		switch (pdis->CtlID)
		{
		case ID_BUTTON_1:
			rect = pdis->rcItem;
			hCurrIcon = (HICON)LoadIconW(NULL, IDI_WINLOGO);
			if (!DrawIconEx(pdis->hDC, (int) 0.5 * (rect.right - rect.left - 33), (int) 0.5 * (rect.bottom - rect.top - 33), (HICON)hCurrIcon, rect.right - rect.left, rect.bottom - rect.top, 0, NULL, DI_NORMAL)) {}
			break;
		case ID_BUTTON_2:
			rect = pdis->rcItem;
			hCurrIcon = (HICON)LoadIconW(NULL, IDI_ERROR);
			if (!DrawIconEx(pdis->hDC, (int) 0.5 * (rect.right - rect.left - 33), (int) 0.5 * (rect.bottom - rect.top - 33), (HICON)hCurrIcon, rect.right - rect.left, rect.bottom - rect.top, 0, NULL, DI_NORMAL)) {}
			break;
		default:
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
