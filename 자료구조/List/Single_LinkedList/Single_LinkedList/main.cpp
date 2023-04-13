#include<iostream>

using namespace std;


struct node
{
	int el;
	node* next;
};
struct List
{
	node *head;
	node *tail;
	int size;
};
void initList(List* list)
{
	list->head = new node;
	list->tail = new node;
	list->head->next = list->tail;
	list->head->el = -1;
	list->tail->next = NULL;
	list->tail->el = -1;
	list->size = 0; 
}

void addFirst(List* list, int e)
{
	node* newNode = new node;
	newNode->el = e;
	
	newNode->next = list->head->next;
	list->head->next = newNode;
	list->size++;
}
void addLast(List* list, int e)
{
	node* newNode = new node;
	newNode->el = e;

	node* prevNode = list->head;
	while (prevNode->next != list->tail)
	{
		prevNode = prevNode->next;
	}

	newNode->next = list->tail;
	prevNode->next = newNode;
	list->size++;
}
void add(List* list, int r, int e)
{
	node* newNode = new node;
	newNode->el = e;

	node* prevNode = list->head;
	for (int i = 0; i < r; i++)
	{
		prevNode = prevNode->next;
	}

	newNode->next = prevNode->next;
	prevNode->next = newNode;
	list->size++;
}
int removeFirst(List* list)
{
	node* delNode = list->head->next;
	list->head = delNode->next;
	list->size--;

	int delEl = delNode->el;
	delete delNode;
	return delEl;
}
int removeLast(List* list)
{
	node* prevNode = list->head->next;
	while (prevNode->next->next != list->tail)
	{
		prevNode = prevNode->next;
	}
	node* delNode = prevNode->next;

	prevNode->next = delNode->next;
	list->size--;

	int delEl = delNode->el;
	delete delNode;
	return delEl;
}
int remove(List* list, int r)
{
	node* prevNode = list->head->next;
	for (int i = 0; i < r-1; i++)
	{
		prevNode = prevNode->next;
	}
	node* delNode = prevNode->next;

	prevNode->next = delNode->next;
	list->size--;

	int delEl = delNode->el;
	delete delNode;
	return delEl;
}

void print_list(List* list)
{
	node* getNode = list->head->next;
	while (getNode != list->tail)
	{
		cout << getNode->el << " ";
		getNode = getNode->next;
	}
}



int main()
{
	List list;
	initList(&list);
	addFirst(&list, 2);
	addFirst(&list, 3);
	addFirst(&list, 4);
	addFirst(&list, 5);
	addFirst(&list, 6);
	addFirst(&list, 7);
	addFirst(&list, 8);
	addLast(&list, 9);
	addLast(&list, 10);
	print_list(&list);

}