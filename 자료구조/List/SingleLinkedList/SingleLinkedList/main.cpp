#include"SingleLinkedlist.h"


int main()
{
	SingleLinkedlist list;
	list.addFirst(2);
	list.addFirst(1);
	list.addFirst(3);
	list.addFirst(4);
	list.removeFirst();
	list.removeLast();
	list.print();
}