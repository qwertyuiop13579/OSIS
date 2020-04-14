// Lab6.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab12.h"
#include "math.h"

#define MAX_LOADSTRING 100
#define ID_BUTTON_1 3000
#define ID_BUTTON_2 3001

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hBtn1;
HWND hBtn2;

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
	LoadStringW(hInstance, IDC_LAB12, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB12));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB12));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB12);
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
HWND hWnd;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	//hBtn1 = CreateWindowW(L"BUTTON", L"Start", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 10, 10, 100, 50, hWnd, (HMENU)ID_BUTTON_1, hInstance, NULL);
	//hBtn2 = CreateWindowW(L"BUTTON", L"Stop", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 150, 10, 100, 50, hWnd, (HMENU)ID_BUTTON_2, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	return TRUE;
}
LPWSTR CharToLPWSTR(LPCSTR char_string)
{
	LPWSTR res;
	DWORD res_len = MultiByteToWideChar(1251, 0, char_string, -1, NULL, 0);
	res = (LPWSTR)GlobalAlloc(GPTR, (res_len + 1) * sizeof(WCHAR));
	MultiByteToWideChar(1251, 0, char_string, -1, res, res_len);
	return res;
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
char str[5]="AAAA";
HANDLE hEvent;
HANDLE hThread[4];
CRITICAL_SECTION cs;

DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
	for (;;)
	{
		if ((int)lpParam == 1)
		{
			WaitForSingleObject(hEvent, 1);
			SetEvent(hEvent);
			EnterCriticalSection(&cs);         //входим в критическую секцию
			strcpy(str, "AAAA");
			Sleep(1000);
			LeaveCriticalSection(&cs);          //покидаем критическую секцию
			ResetEvent(hEvent);
		}
		else if((int)lpParam == 2)
		{		
			WaitForSingleObject(hEvent, 1);

			SetEvent(hEvent);			       // устанавливаем в сигнальное положение
			EnterCriticalSection(&cs);         //входим в критическую секцию
			strcpy(str, "BBBB");
			Sleep(1000);
			LeaveCriticalSection(&cs);          //покидаем критическую секцию
			ResetEvent(hEvent);
		}
		else if ((int)lpParam == 3)
		{
			WaitForSingleObject(hEvent, 1);
			SetEvent(hEvent);
			EnterCriticalSection(&cs);         //входим в критическую секцию
			strcpy(str, "CCCC");
			Sleep(1000);
			LeaveCriticalSection(&cs);          //покидаем критическую секцию
			ResetEvent(hEvent);
		}
		else if ((int)lpParam == 4)
		{
			WaitForSingleObject(hEvent, 1);
			SetEvent(hEvent);
			EnterCriticalSection(&cs);         //входим в критическую секцию
			strcpy(str, "DDDD");
			Sleep(1000);
			LeaveCriticalSection(&cs);          //покидаем критическую секцию
			ResetEvent(hEvent);
		}
		InvalidateRect(hWnd, NULL, FALSE);
	}
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	DWORD dwThreadId[4];
	int param[4] = { 1,2,3,4 };
	switch (message)
	{
	case WM_CREATE:
		hEvent = CreateEventW(NULL, TRUE, FALSE, L"DRAW");
		InitializeCriticalSection(&cs);      //создаем критическую секцию
		hThread[0] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)param[0], 0, &dwThreadId[0]);
		hThread[1] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)param[1], 0, &dwThreadId[1]);
		hThread[2] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)param[2], 0, &dwThreadId[2]);
		hThread[3] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)param[3], 0, &dwThreadId[3]);
		//атрибуты без-ти, размер стека, функция, аргумент функции,флажки создания, идентификатор потока
		if (hThread[0] == NULL) MessageBoxW(NULL, L"Cannot create first thread", L"Fail", MB_OK);
		if (hThread[1] == NULL) MessageBoxW(NULL, L"Cannot create second thread", L"Fail", MB_OK);
		if (hThread[2] == NULL) MessageBoxW(NULL, L"Cannot create third thread", L"Fail", MB_OK);
		if (hThread[3] == NULL) MessageBoxW(NULL, L"Cannot create fourth thread", L"Fail", MB_OK);
		/*
		if (hEvent != NULL)
		{
			SetEvent(hEvent);           // устанавливаем в сигнальное положение
			Sleep(1000);
			ResetEvent(hEvent);         // устанавливаем в несигнальное положение
			CloseHandle(hEvent);
		}
		*/
		//WaitForSingleObject(hEvent,100);
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
			DeleteCriticalSection(&cs);        //удаляем критическую секцию
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		RECT Rect;
		HFONT hf;
		GetClientRect(hWnd, &Rect);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HBRUSH hBrush;                                 //создаём объект-кисть
		hBrush = CreateSolidBrush(RGB(255, 255, 255)); //задаём сплошную кисть, закрашенную цветом RGB
		FillRect(hdc, &Rect, hBrush);
		//Создаём свой шрифт
		long lfHeight;
		//lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		hf = CreateFontW(30, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
		SelectObject(hdc, hf);
		SetTextColor(hdc, RGB(0, 0, 255));            // синий
		TextOutW(hdc, 200, 200, CharToLPWSTR(str), strlen(str));
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		CloseHandle(hThread[0]);
		CloseHandle(hThread[1]);
		CloseHandle(hThread[2]);
		CloseHandle(hThread[3]);
		DeleteCriticalSection(&cs);        //удаляем критическую секцию
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
