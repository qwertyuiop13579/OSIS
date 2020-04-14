// Lab3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <clocale>

using namespace std;

struct Node
{
	string data;
	Node *Next;
	Node *Prev;
};

class List
{
	Node *Head, *Tail;
public:
	List() :Head(NULL), Tail(NULL) {};
	~List();
	void Show();
	void AddToTail(string x);
	void AddToHead(string x);
	void Delete(int k);
	void DeleteAll();
	int Count();
};

List::~List()                           //Деструктор
{
	while (Head)
	{
		Tail = Head->Next;
		delete Head;
		Head = Tail;
	}
}

void List::AddToTail(string x)     //Добавить в хвост
{
	Node *temp = new Node;
	temp->Next = NULL;
	temp->data = x;

	if (Head != NULL)
	{
		temp->Prev = Tail;
		Tail->Next = temp;
		Tail = temp;
	}
	else
	{
		temp->Prev = NULL;
		Head = Tail = temp;
	}
}

void List::AddToHead(string x)       //Добавить в начало
{
	Node *temp = new Node;
	temp->Prev = NULL;
	temp->data = x;

	if (Head != NULL)
	{
		temp->Next = Head;
		Head->Prev = temp;
		Head = temp;
	}
	else
	{
		temp->Next = NULL;
		Head = Tail = temp;
	}
}

void List::Show()             //Выводим список с начала
{
	Node *temp = Head;
	while (temp != NULL)
	{
		cout << temp->data << " ";
		temp = temp->Next;
	}
	cout << "\n";
}

int List::Count()             //Количество элементов
{
	Node *temp = Head;
	int count = 0;
	while (temp != NULL)
	{
		count++;
		temp = temp->Next;
	}
	return count;
}

void List::Delete(int k)                        //Удаление
{
	if (k > Count())
		return;

	if ((k == 1) and (Head->Next))  //Если удаляем первый и неединственный
	{
		Node *temp = Head;
		Head = Head->Next;
		Head->Prev = NULL;
		delete temp;
		return;
	}
	else if ((k == 1) and (Head == Tail))       //Если удаляем первый и единственный элемент
	{
		Head->Next = NULL;
		Head = NULL;
		delete Head;
		return;
	}

	if (k == Count())      //Если удаляем хвост
	{
		Node *temp = Tail;
		Tail = Tail->Prev;
		Tail->Next = NULL;
		delete temp;
		return;
	}

	Node *temp = Head, *temp2;          //Удаляем с середины
	for (int i = 0; i < k - 1; i++) temp = temp->Next;
	temp2 = temp;
	temp2->Prev->Next = temp->Next;
	temp2->Next->Prev = temp->Prev;
	delete temp;
}

void List::DeleteAll()         //Удалить весь
{
	while (Head)
	{
		Tail = Head->Next;
		delete Head;
		Head = Tail;
	}
}

int main()
{

	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("CLS");
	int t, k;
	string str;
	List lst;
	lst.AddToTail("1");
	lst.AddToTail("2");
	lst.AddToTail("3");
	while (true)
	{
		//cout << "Что сделать?\n1-Вывести\n2-Добавить в начало\n3-Добавить в хвост\n4-Удалить\n5-Удалить весь\n6-Выход";
		cout << "Что сделать? 1-Вывести 2-Добавить в начало  3-Добавить в хвост  4-Удалить  5-Удалить весь  6-Выход" << endl;
		cin >> t;
		switch (t)
		{
		case 1:
			lst.Show();
			break;
		case 2:
			cin >> str;
			lst.AddToHead(str);
			break;
		case 3:
			cin >> str;
			lst.AddToTail(str);
			break;
		case 4:
			cin >> k;
			lst.Delete(k);
			break;
		case 5:
			lst.DeleteAll();
			break;
		case 6:
		default:
			system("PAUSE");
			return 0;
			break;
		}
	}
}

