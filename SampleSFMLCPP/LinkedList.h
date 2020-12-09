#pragma once

#include "Common.hpp"

enum ElementStatus
{
	ELEMENT_ACTIVE,
	ELEMENT_DELETION_REQUIRED,
};

class LinkedListClass
{
public:

	struct Element
	{
		void *data;
		struct Element *next;
		enum ElementStatus status;
	};
	typedef struct Element sElement;

	struct LinkedList
	{
		struct Element *first;
		struct Element *last;
		int size;
	};
	typedef struct LinkedList sLinkedList;



	LinkedListClass();
	~LinkedListClass();

	//void LinkedListInitialisation();
	//void LinkedListInitialisation(sLinkedList *_list);
	LinkedListClass::sLinkedList *LinkedListInitialisation();

	//void AddElementToLinkedList(LinkedListClass::sElement *_element, int _index);
	void AddElementToLinkedList(LinkedListClass::sLinkedList *_list, LinkedListClass::sElement *_element, int _index);
	
	//void RemoveElementsOfLinkedList(bool _particularElement = false, int _position = 0);
	void RemoveElementsOfLinkedList(LinkedListClass::sLinkedList *_list, bool _particularElement = false, int _position = 0);
	
	void FreeLinkedList(LinkedListClass::sLinkedList *_list);
	void ReadLinkedList(LinkedListClass::sLinkedList *_list);

protected:
	//sLinkedList *list;

};