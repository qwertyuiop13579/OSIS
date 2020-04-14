// Lab2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.


#include "pch.h"
#include <cstdlib>
#include <clocale>
#include <cstdio>
#include <windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;

BOOL FindFileByClaster(TCHAR* volume, LONGLONG cluster) {

	HANDLE hDevice = CreateFile(volume,      // диск 
		GENERIC_READ,                        // доступ к диску
		FILE_SHARE_READ | FILE_SHARE_WRITE,  // режим обмена
		NULL,                                // атрибуты безопасности
		OPEN_EXISTING,                       // расположение
		FILE_FLAG_BACKUP_SEMANTICS,          // атрибуты файла
		NULL);

	if (hDevice == INVALID_HANDLE_VALUE)        // если неверный диск
	{
		return FALSE;
	}

	//входные параметры
	LOOKUP_STREAM_FROM_CLUSTER_INPUT input = { 0 };
	input.NumberOfClusters = 1;
	input.Cluster[0].QuadPart = cluster;

	//буфер для результатов
	BYTE output[5000] = {};
	DWORD dwRes = 0;
	LOOKUP_STREAM_FROM_CLUSTER_OUTPUT result = { 0 };

	//выполняем IOCTL(input output control) запрос
	BOOL bRes = DeviceIoControl((HANDLE)hDevice,   // дескриптор диска
		FSCTL_LOOKUP_STREAM_FROM_CLUSTER, // dwIoControlCode
		(LPVOID)&input,        // входной буфер
		(DWORD)sizeof(input),     // размер входного буфера
		(LPVOID)output,       // выходной буфер
		(DWORD)5000,    // размер выходного буфера
		(LPDWORD)&dwRes,   // количество фактических байт
		NULL);    // структура

	if (bRes == FALSE)    // если не найден
	{
		return FALSE;
	}

	//переносим результат из массива в структуру LOOKUP_STREAM_FROM_CLUSTER_OUTPUT
	memcpy(&result, output, sizeof(LOOKUP_STREAM_FROM_CLUSTER_OUTPUT));

	if (result.NumberOfMatches == 0)
	{
		cout << "Файл не найден.\n";
		return FALSE;
	}

	BYTE* p = (BYTE*)output + result.Offset;             // переносим в массив байт
	LOOKUP_STREAM_FROM_CLUSTER_ENTRY* str = (LOOKUP_STREAM_FROM_CLUSTER_ENTRY*)p;     // преобразовываем к структуре IOCTL

	wprintf(L"\nФайл: %s\n", str->FileName);          // выводим 
	return TRUE;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	long long inp = 0;
	printf_s("Введите номер кластера:\n");
	scanf_s("%llu", &inp);

	FindFileByClaster((TCHAR*)L"\\\\.\\C:", inp);
	return 0;
}
