// Lab6.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab7.h"

#define MAX_LOADSTRING 100
#define ID_BUTTON_1 3000
#define ID_BUTTON_2 3001
#define ID_BUTTON_3 3002
#define ID_BUTTON_4 3003
#define ID_Edit 3004
#define ID_List1 3005
#define ID_List2 3006

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hBtn1;
HWND hBtn2;
HWND hBtn3;
HWND hBtn4;
HWND hEdit;
HWND hListBox1;
HWND hListBox2;

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
	LoadStringW(hInstance, IDC_LAB7, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB7));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB7));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB7);
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
	hBtn1 = CreateWindowW(L"BUTTON", L"Add", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 10, 10, 80, 50, hWnd, (HMENU)ID_BUTTON_1, hInstance, NULL);
	hBtn2 = CreateWindowW(L"BUTTON", L"Clear", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 100, 10, 80, 50, hWnd, (HMENU)ID_BUTTON_2, hInstance, NULL);
	hBtn3 = CreateWindowW(L"BUTTON", L"ToRight", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 190, 10, 80, 50, hWnd, (HMENU)ID_BUTTON_3, hInstance, NULL);
	hBtn4 = CreateWindowW(L"BUTTON", L"Delete", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 280, 10, 80, 50, hWnd, (HMENU)ID_BUTTON_4, hInstance, NULL);
	//hEdit= CreateWindowW(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 380, 10, 200, 120, hWnd, (HMENU)ID_Edit, hInstance, NULL);
	hEdit= CreateWindowEx(WS_EX_STATICEDGE, L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 380, 10, 200, 120, hWnd, (HMENU)ID_Edit, NULL, NULL);
	hListBox1 = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL, 10, 200, 300, 200, hWnd, (HMENU)ID_List1, hInstance, NULL);
	hListBox2 = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL, 400, 200, 300, 200, hWnd, (HMENU)ID_List2, hInstance, NULL);
	//hListBox2 = CreateWindowW(L"LISTBOX", L"LISTBOX", WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL, 400, 200, 300, 200, hWnd, (HMENU)ID_List2, hInstance, NULL);
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
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		int count;
		int iSelected;
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_BUTTON_1:           //Add
			char szBuff[255];
			GetWindowTextW(hEdit, (LPWSTR)szBuff, 255);                  //получаем текст
			if (SendMessageW(hListBox1, LB_FINDSTRING, -1, (LPARAM)szBuff) == LB_ERR)   //если нет такого
			{
				SendMessageW(hListBox1, LB_ADDSTRING, 0, (LPARAM)szBuff);	 //отправляем текст в ListBox1
			}
			break;
		case ID_BUTTON_2:           //Clear
			SendMessageW(hListBox1, LB_RESETCONTENT, 0, 0);
			SendMessageW(hListBox2, LB_RESETCONTENT, 0, 0);
			break;
		case ID_BUTTON_3:           //Toright
			if (SendMessageW(hListBox1, LB_GETCURSEL, 0, 0) != LB_ERR)   //если что то выделено
			{
				iSelected = SendMessageW(hListBox1, LB_GETCURSEL, 0, 0);
				SendMessageW(hListBox1, LB_GETTEXT, iSelected, (LPARAM)szBuff);	 //отправляем текст в ListBox1

				if (SendMessageW(hListBox2, LB_FINDSTRING, -1, (LPARAM)szBuff) == LB_ERR)   //если нет такого
				{
					SendMessageW(hListBox2, LB_ADDSTRING, 0, (LPARAM)szBuff);	 //отправляем текст в ListBox1
				}
			}
			break;
		case ID_BUTTON_4:               //Delete
			iSelected = SendMessageW(hListBox1, LB_GETCURSEL, 0, 0);
			if (iSelected != LB_ERR)
			{
				SendMessageW(hListBox1, LB_DELETESTRING, iSelected, 0);
			}
			iSelected = SendMessageW(hListBox2, LB_GETCURSEL, 0, 0);
			if (iSelected != LB_ERR)
			{
				SendMessageW(hListBox2, LB_DELETESTRING, iSelected, 0);
			}
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
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hWnd, &ps);
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
