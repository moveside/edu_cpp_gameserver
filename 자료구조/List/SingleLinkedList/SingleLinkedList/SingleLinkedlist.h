#pragma once

struct node
{
	int el;
	node* next;
};

class SingleLinkedlist
{
private:
	int size;
	node* head;
	node* tail;
public:
	SingleLinkedlist();
	~SingleLinkedlist();

	bool is_Empty();

	int getSize();
	int getElement(int r);

	void addFirst(int e);
	void addLast(int e);
	void add(int r, int e);

	void setElement(int r, int e);
	void removeFirst();
	void removeLast();
	void remove(int r);

	void print();

};