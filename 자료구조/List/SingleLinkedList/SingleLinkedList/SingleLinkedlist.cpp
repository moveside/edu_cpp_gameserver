#include"SingleLinkedlist.h"
#include<iostream>



using namespace std;

SingleLinkedlist::SingleLinkedlist()
{
	size = 0;
	head = new node;
	tail = new node;
	
	head->next = tail;
	head->el = -1;
	tail->next = nullptr;
	tail->el = -1;
}
SingleLinkedlist::~SingleLinkedlist()
{
	node* getNode = head;
	while (getNode != tail)
	{
		node* nextNode = getNode->next;
		delete getNode;
		getNode = nextNode;
	}
}

bool SingleLinkedlist::is_Empty()
{
	if (size == 0) return true;
	return false;
}

int SingleLinkedlist::getSize()
{
	return size;
}

int SingleLinkedlist::getElement(int r)
{
	if (size <= r)
	{
		cout << "invalidRank";
		return -1;
	}
	node* getNode = head->next;
	for (int i = 0; i < r; i++)
	{
		getNode = getNode->next;
	}

	return getNode->el;
}

void SingleLinkedlist::addFirst(int e)
{
	node* newNode = new node;
	newNode->el = e;

	newNode->next = head->next;
	head->next = newNode;

	size++;
}

void SingleLinkedlist::addLast(int e)
{
	node* newNode = new node;
	newNode->el = e;

	node* prevNode = head->next;
	for (int i = 0; i < size-2; i++) prevNode = prevNode->next;

	newNode->next = tail;
	prevNode->next = newNode;

	size++;
}
void SingleLinkedlist::add(int r, int e)
{
	if (size < r)
	{
		cout << "invalidRank";
		return;
	}

	node* newNode = new node;
	newNode->el = e;

	node* prevNode = head->next;
	for (int i = 0; i < r-1; i++) prevNode = prevNode->next;

	newNode->next = prevNode->next;
	prevNode->next = newNode;

	size++;
}

void SingleLinkedlist::setElement(int r, int e)
{
	if (size <= r)
	{
		cout << "invalidRank";
		return;
	}

	node* setNode = head->next;

	for (int i = 0; i < r; i++) setNode = setNode->next;

	setNode->el = e;
}

void SingleLinkedlist::removeFirst()
{
	node* delNode = head->next;
	head->next = delNode->next;

	delete delNode;

	size--;
}

void SingleLinkedlist::removeLast()
{
	node* prevNode = head->next;
	for (int i = 0; i < size - 2; i++)
	{
		prevNode = prevNode->next;
	}

	node* delNode = prevNode->next;
	prevNode->next = tail;

	delete delNode;

	size--;
}

void SingleLinkedlist::remove(int r)
{
	if (size <= r)
	{
		cout << "invalidRank";
	}

	node* prevNode = head->next;

	for (int i = 0; i < r - 1; i++) prevNode = prevNode->next;

	node* delNode = prevNode->next;

	prevNode->next = delNode->next;

	delete delNode;

	size--;
}

void SingleLinkedlist::print()
{
	node* getNode = head->next;
	while (getNode != tail)
	{
		cout << getNode->el << " ";
		getNode = getNode->next;
	}
}