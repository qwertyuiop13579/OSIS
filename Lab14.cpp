// Lab141.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab141.h"
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <regex>
#include <algorithm>
#include <string>
#include <winreg.h>
using namespace std;

#define MAX_LOADSTRING 100
#define ID_BUTTON_1 3000
#define ID_BUTTON_2 3001
#define ID_Edit 3004
#define ID_List1 3005
#define ID_List2 3006


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hEdit;
HWND hBtn1;
HWND hBtn2;
HWND hListBox1;
HWND hListBox2;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
std::wstring s2ws(const std::string& s);

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
	LoadStringW(hInstance, IDC_LAB141, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB141));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB141));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB141);
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

	hEdit = CreateWindowEx(WS_EX_STATICEDGE, L"edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE, 10, 10, 350, 150, hWnd, (HMENU)ID_Edit, NULL, NULL);
	//hBtn1 = CreateWindowW(L"BUTTON", L"Go", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 400, 10, 80, 50, hWnd, (HMENU)ID_BUTTON_1, hInstance, NULL);
	hBtn2 = CreateWindowW(L"BUTTON", L"Find", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 400, 10, 80, 50, hWnd, (HMENU)ID_BUTTON_2, hInstance, NULL);
	hListBox1 = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY, 10, 170, 800, 300, hWnd, (HMENU)ID_List1, hInstance, NULL);
	hListBox2 = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY, 500, 10, 300, 150, hWnd, (HMENU)ID_List2, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	char szBuff[255];
	wsprintfW((LPWSTR)szBuff, L"HKEY_CLASSES_ROOT");
	SendMessageW(hListBox2, LB_ADDSTRING, 0, (LPARAM)szBuff);
	wsprintfW((LPWSTR)szBuff, L"HKEY_CURRENT_USER");
	SendMessageW(hListBox2, LB_ADDSTRING, 0, (LPARAM)szBuff);
	wsprintfW((LPWSTR)szBuff, L"HKEY_LOCAL_MACHINE");
	SendMessageW(hListBox2, LB_ADDSTRING, 0, (LPARAM)szBuff);
	wsprintfW((LPWSTR)szBuff, L"HKEY_USERS");
	SendMessageW(hListBox2, LB_ADDSTRING, 0, (LPARAM)szBuff);
	wsprintfW((LPWSTR)szBuff, L"HKEY_CURRENT_CONFIG");
	SendMessageW(hListBox2, LB_ADDSTRING, 0, (LPARAM)szBuff);
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

HKEY globalkey;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HKEY currentKey;
	char name[1024];
	DWORD dwSize;
	long result;
	char szBuff[255];
	DWORD index;
	DWORD dwMaxValueLen, dwCurLen, dwType;
	void* pData;
	char buff[1024];

	int iSelected;
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_BUTTON_2:                                               //Find
			SendMessageW(hListBox1, LB_RESETCONTENT, 0, 0);
			GetWindowTextW(hEdit, (LPWSTR)szBuff, 255);
			dwSize = 1024;
			index = 0;
			RegQueryInfoKeyW(globalkey, 0, 0, 0, 0, 0, 0, 0, 0, &dwMaxValueLen, 0, 0);
			pData = malloc(dwMaxValueLen);
			dwCurLen = dwMaxValueLen;

			while (ERROR_SUCCESS == RegEnumValueW(globalkey, index, (LPWSTR)name, (LPDWORD)&dwSize, NULL, &dwType, (LPBYTE)pData, &dwCurLen))
			{
				//Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\CodeBlocks
				//Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Akamai
				//Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Discord
				if (dwType == 1) wsprintfW((LPWSTR)buff, L"Имя: %s, Тип: REG_SZ, Значение: %s", name, pData);
				else if (dwType == 4)
				{
					DWORD temp;
					memcpy(&temp, (LPBYTE)pData, sizeof(DWORD));
					wsprintfW((LPWSTR)buff, L"Имя: %s, Тип: REG_DWORD, Значение: %#08x", name, temp);
				}
				else
				{
					wsprintfW((LPWSTR)buff, L"Имя: %s, Тип: %x, Значение: %s", name, dwType, (LPBYTE)pData);
				}
				regex re(szBuff);
				if (regex_match(name, re))
				{
					SendMessageW(hListBox1, LB_ADDSTRING, 0, (LPARAM)buff);
				}
				dwSize = 1024;
				dwCurLen = dwMaxValueLen;
				index++;
			}
			free(pData);
			break;
		case ID_List1:
			if (HIWORD(wParam) == LBN_DBLCLK)
			{
				//Выводим подключи
				iSelected = SendMessageW(hListBox1, LB_GETCURSEL, 0, 0);
				currentKey = globalkey;
				SendMessageW(hListBox1, LB_GETTEXT, iSelected, (LPARAM)szBuff);
				result = RegOpenKeyExW(currentKey, (LPCWSTR)szBuff, 0, KEY_ALL_ACCESS | KEY_QUERY_VALUE | KEY_SET_VALUE | KEY_EXECUTE, &currentKey);
				if (result != ERROR_SUCCESS)
				{
					MessageBoxW(NULL, L"Cannot find", L"Fail", MB_OK);
				}
				else
				{
					globalkey = currentKey;
					SendMessageW(hListBox1, LB_RESETCONTENT, 0, 0);
					index = 0;
					dwSize = 1024;
					while (ERROR_SUCCESS == RegEnumKeyW(currentKey, index, (LPWSTR)name, dwSize))
					{
						SendMessageW(hListBox1, LB_ADDSTRING, 0, (LPARAM)name);
						dwSize = 1024;
						index++;
					}

					//Выводим данные
					index = 0;
					RegQueryInfoKeyW(currentKey, 0, 0, 0, 0, 0, 0, 0, 0, &dwMaxValueLen, 0, 0);
					pData = malloc(dwMaxValueLen);
					dwCurLen = dwMaxValueLen;
					while (ERROR_SUCCESS == RegEnumValueW(currentKey, index, (LPWSTR)name, (LPDWORD)&dwSize, NULL, &dwType, (LPBYTE)pData, &dwCurLen))
					{
						if (dwType == 1) wsprintfW((LPWSTR)buff, L"Имя: %s, Тип: REG_SZ, Значение: %s", name, pData);
						else if (dwType == 4)
						{
							DWORD temp;
							memcpy(&temp, (LPBYTE)pData, sizeof(DWORD));
							wsprintfW((LPWSTR)buff, L"Имя: %s, Тип: REG_DWORD, Значение: %#08x", name, temp);
						}
						else
						{
							wsprintfW((LPWSTR)buff, L"Имя: %s, Тип: %x, Значение: %s", name, dwType, (LPBYTE)pData);
						}
						SendMessageW(hListBox1, LB_ADDSTRING, 0, (LPARAM)buff);
						dwSize = 1024;
						dwCurLen = dwMaxValueLen;
						index++;
					}
					free(pData);
				}	
			}
			break;
		case ID_List2:
			if (HIWORD(wParam) == LBN_DBLCLK)
			{
				//Выводим подключи
				iSelected = SendMessageW(hListBox2, LB_GETCURSEL, 0, 0);
				HKEY tempkey;
				if (iSelected == 0) tempkey = HKEY_CLASSES_ROOT;
				else if (iSelected == 1) tempkey = HKEY_CURRENT_USER;
				else if (iSelected == 2) tempkey = HKEY_LOCAL_MACHINE;
				else if (iSelected == 3) tempkey = HKEY_USERS;
				else tempkey = HKEY_CURRENT_CONFIG;
				result = RegOpenKeyExW(tempkey, (LPCWSTR)"", 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_SET_VALUE | KEY_EXECUTE, &currentKey);
				if (result != ERROR_SUCCESS)    // fail
				{
					MessageBoxW(NULL, L"Cannot find", L"Fail", MB_OK);
				}
				else
				{
					globalkey = currentKey;
					SendMessageW(hListBox1, LB_RESETCONTENT, 0, 0);
					index = 0;
					dwSize = 1024;
					while (ERROR_SUCCESS == RegEnumKeyW(currentKey, index, (LPWSTR)name, dwSize))
					{
						SendMessageW(hListBox1, LB_ADDSTRING, 0, (LPARAM)name);
						dwSize = 1024;
						index++;
					}
				}
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

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
