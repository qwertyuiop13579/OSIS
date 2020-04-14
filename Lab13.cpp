// Lab14.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab13_1.h"
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "psapi.h"

#define MAX_LOADSTRING 100
#define ID_List1 3005
#define ID_List2 3006
#define ID_Prior1 3007
#define ID_Prior2 3008
#define ID_Prior3 3009
#define ID_Prior4 3010
#define ID_Prior5 3011
#define ID_Prior6 3012


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hListBox1;
HWND hListBox2;
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void setPrior(DWORD dwPrior);

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
	LoadStringW(hInstance, IDC_LAB131, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB131));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB131));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB131);
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


	hListBox1 = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY, 10, 10, 500, 300, hWnd, (HMENU)ID_List1, hInstance, NULL);
	hListBox2 = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, 600, 10, 500, 300, hWnd, (HMENU)ID_List2, hInstance, NULL);


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

std::vector<DWORD> ProcessesID;
VOID PrintModuleList(DWORD CONST dwProcessId)
{
	MODULEENTRY32 meModuleEntry;
	TCHAR szBuff[1024];
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);       //создает снапшот запущенных процессов, модулей, нитей и так далее.
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		DWORD error;
		error = GetLastError();
		wsprintfW(szBuff, L"Error %X\r\n", error);
		SendMessageW(hListBox2, LB_ADDSTRING, 0, (LPARAM)szBuff);
		//Error 5-доступ запрещён. Если указанный процесс является процессом Idle или одним из процессов CSRSS, эта функция завершается ошибкой, а последний код ошибки - ERROR_ACCESS_DENIED, поскольку их ограничения доступа не позволяют открывать код уровня пользователя.
		//Error 12B-Только часть запроса ReadProcessMemory или WriteProcessMemory была выполнена.  Если указанный процесс является 64-разрядным процессом, а вызывающая сторона - 32-разрядным процессом, эта функция завершается ошибкой, а последний код ошибки - ERROR_PARTIAL_COPY (299).
		return;
	}

	meModuleEntry.dwSize = sizeof(MODULEENTRY32);
	Module32First(hSnapshot, &meModuleEntry);
	do
	{
		wsprintfW(szBuff, L"%s", meModuleEntry.szModule);            //Название модуля
		SendMessageW(hListBox2, LB_ADDSTRING, 0, (LPARAM)szBuff);
	} while (Module32Next(hSnapshot, &meModuleEntry));

	CloseHandle(hSnapshot);
}

VOID PrintProcessList()
{
	PROCESSENTRY32 peProcessEntry;           //Описывает запись из списка процессов, находящихся в системном адресном пространстве, когда был сделан снимок.
	TCHAR szBuff[1024];
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);   //создает снапшот запущенных процессов, модулей, нитей и так далее.
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		MessageBoxW(NULL, L"Cannot create Snapshot", L"Fail", MB_OK);
		return;
	}
	/*
	Process32First принимает хэндл, полученный от CreateToolhelp32Snapshot, вызванной с dwFlags = TH32CS_SNAPPROCESS,
	а также указатель на структуру PROCESSENTRY32. Перед вызовом процедуры поле dwSize этой структуры должно быть установлено в sizeof(PROCESSENTRY32).
	В случае успеха возвращается TRUE, а в peProcessEntry записывается информация о первом процессе. В случае ошибки возвращается FALSE,
	а подробности поможет узнать GetLastError.
	*/
	ProcessesID.clear();
	peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &peProcessEntry);
	do
	{
		ProcessesID.push_back(peProcessEntry.th32ProcessID);
		wsprintfW(szBuff, L"ID: %X, %s, %d", peProcessEntry.th32ProcessID, peProcessEntry.szExeFile, peProcessEntry.pcPriClassBase); //ID  процесса, исполняемый файл, приоритет
		SendMessageW(hListBox1, LB_ADDSTRING, 0, (LPARAM)szBuff);	 //отправляем текст в ListBox1
	} while (Process32Next(hSnapshot, &peProcessEntry));

	CloseHandle(hSnapshot);
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int iSelected;
	switch (message)
	{
	case WM_RBUTTONDOWN:
		HMENU hPopupMenu;
		hPopupMenu = CreatePopupMenu();
		InsertMenuW(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, NULL, L"Отмена");
		InsertMenuW(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_Prior6, L"Real time class (значение 24)");
		InsertMenuW(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_Prior5, L"High class (значение 13)");
		InsertMenuW(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_Prior4, L"Above normal class (значение 10)");
		InsertMenuW(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_Prior3, L"Normal class (значение 8)");
		InsertMenuW(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_Prior2, L"Below normal class (значение 6)");
		InsertMenuW(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_Prior1, L"Idle class (значение 4)");
		SetForegroundWindow(hWnd);
		TrackPopupMenu(hPopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, LOWORD(lParam) + 120, HIWORD(lParam) + 260, 0, hWnd, NULL);
		break;
	case WM_LBUTTONDOWN:
		SendMessageW(hListBox1, LB_RESETCONTENT, 0, 0);
		PrintProcessList();
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
		case ID_List1:
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				SendMessageW(hListBox2, LB_RESETCONTENT, 0, 0);
				iSelected = SendMessageW(hListBox1, LB_GETCURSEL, 0, 0);
				if(iSelected!=-1) PrintModuleList(ProcessesID[iSelected]);
			}
			break;
		case ID_Prior1:
			setPrior(IDLE_PRIORITY_CLASS);
			break;
		case ID_Prior2:
			setPrior(BELOW_NORMAL_PRIORITY_CLASS);
			break;
		case ID_Prior3:
			setPrior(NORMAL_PRIORITY_CLASS);
			break;
		case ID_Prior4:
			setPrior(ABOVE_NORMAL_PRIORITY_CLASS);
			break;
		case ID_Prior5:
			setPrior(HIGH_PRIORITY_CLASS);
			break;
		case ID_Prior6:
			setPrior(REALTIME_PRIORITY_CLASS);
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

void setPrior(DWORD dwPrior)
{
	int iSelected;
	if (SendMessageW(hListBox1, LB_GETCURSEL, 0, 0) != LB_ERR)   //если что то выделено
	{
		iSelected = SendMessageW(hListBox1, LB_GETCURSEL, 0, 0);
		HANDLE Proc;
		Proc = OpenProcess(PROCESS_ALL_ACCESS, TRUE, ProcessesID[iSelected]);
		SetPriorityClass(Proc, dwPrior);
	}
}
