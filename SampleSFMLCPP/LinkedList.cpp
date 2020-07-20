#include "LinkedList.h"
#include "ErrorsLogFile.h"


LinkedListClass::LinkedListClass()
{
	
}


LinkedListClass::~LinkedListClass()
{

}


//// First initialisation of a linked list which has been created
//void LinkedListClass::LinkedListInitialisation()
//{
//	this->list = new sLinkedList;
//
//	// We init the first and the last element of the linked list
//	this->list->first = nullptr;
//	this->list->last = nullptr;
//
//	this->list->size = RESET;
//	
//	std::cout << "First : " << list->first << "Last : " << list->last << std::endl;
//}

//void LinkedListClass::LinkedListInitialisation(sLinkedList *_list)
//{
//	std::cout << "List : " << _list << std::endl;
//
//	_list = new sLinkedList;
//
//	// We init the first and the last element of the linked list
//	_list->first = nullptr;
//	_list->last = nullptr;
//
//	_list->size = RESET;
//
//	std::cout << "First : " << _list->first << " & Last : " << _list->last << std::endl;
//}

LinkedListClass::sLinkedList *LinkedListClass::LinkedListInitialisation()
{
	LinkedListClass::sLinkedList *newList = new LinkedListClass::sLinkedList;
	
	// We init the first and the last element of the linked list
	newList->first = nullptr;
	newList->last = nullptr;

	newList->size = RESET;

	std::cout << "First : " << newList->first << " & Last : " << newList->last << std::endl;

	return newList;
}

//void LinkedListClass::AddElementToLinkedList(LinkedListClass::sElement *_element, int _index)
//{
//	//If we want to add the element to the end of the list
//	if (_index == -1)
//	{
//		// If the list has already an first element, we add the element at the end
//		if (this->list->first != nullptr)
//		{
//			LinkedListClass::sElement *currentElement = this->list->first;
//
//			while (currentElement->next != nullptr)
//			{
//				currentElement = currentElement->next;
//			}
//
//			currentElement->next = _element;
//			this->list->last = _element;
//			_element->next = nullptr;
//
//			(this->list->size)++;
//		}
//		// If we don't have a first element, we create it
//		else
//		{
//			this->list->first = _element;
//			this->list->last = _element;
//			_element->next = nullptr;
//
//			(this->list->size)++;
//		}
//	}
//	//If the index is equal to 1
//	else if (_index == 1)
//	{
//		// If the list has already an first element, we replace it and shift the first element
//		if (this->list->first != nullptr)
//		{
//			_element->next = this->list->first;
//			this->list->first = _element;
//
//			(this->list->size)++;
//		}
//		// If we don't have a first element, we create it
//		else
//		{
//			this->list->first = _element;
//			this->list->last = _element;
//			_element->next = nullptr;
//
//			(this->list->size)++;
//		}
//	}
//	else
//	{
//		// If the list has already an first element, we add the element at the selected position
//		if (this->list->first != nullptr)
//		{
//			LinkedListClass::sElement *currentElement = this->list->first;
//			LinkedListClass::sElement *temp;
//			int indexIsHigher = 0;
//
//			for (int i = 1; i < (_index - 1); i++)
//			{
//				//If the index is higher than the number of elements in the list,
//				//Set the next element to the element to add and leave the loop
//				if (currentElement->next == nullptr)
//				{
//					currentElement->next = _element;
//					i = _index;
//					indexIsHigher = 1;
//					(this->list->size)++;
//				}
//				//If the index is not higher, go to the next element
//				else
//				{
//					currentElement = currentElement->next;
//				}
//			}
//
//			//If the index is not higher than the number of elements in the list
//			if (!indexIsHigher)
//			{
//				temp = currentElement->next;
//				currentElement->next = _element;
//				_element->next = temp;
//				(this->list->size)++;
//			}
//		}
//		// If we don't have a first element, we create it
//		else
//		{
//			this->list->first = _element;
//			this->list->last = _element;
//			_element->next = nullptr;
//
//			(this->list->size)++;
//		}
//
//	}
//}

void LinkedListClass::AddElementToLinkedList(LinkedListClass::sLinkedList *_list, LinkedListClass::sElement *_element, int _index)
{
	//If we want to add the element to the end of the list
	if (_index == -1)
	{
		// If the list has already an first element, we add the element at the end
		if (_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = _list->first;

			while (currentElement->next != nullptr)
			{
				currentElement = currentElement->next;
			}

			currentElement->next = _element;
			_list->last = _element;
			_element->next = nullptr;

			(_list->size)++;
		}
		// If we don't have a first element, we create it
		else
		{
			_list->first = _element;
			_list->last = _element;
			_element->next = nullptr;

			(_list->size)++;
		}
	}
	//If the index is equal to 1
	else if (_index == 1)
	{
		// If the list has already an first element, we replace it and shift the first element
		if (_list->first != nullptr)
		{
			_element->next = _list->first;
			_list->first = _element;

			(_list->size)++;
		}
		// If we don't have a first element, we create it
		else
		{
			_list->first = _element;
			_list->last = _element;
			_element->next = nullptr;

			(_list->size)++;
		}
	}
	else
	{
		// If the list has already an first element, we add the element at the selected position
		if (_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = _list->first;
			LinkedListClass::sElement *temp;
			int indexIsHigher = 0;

			for (int i = 1; i < (_index - 1); i++)
			{
				//If the index is higher than the number of elements in the list,
				//Set the next element to the element to add and leave the loop
				if (currentElement->next == nullptr)
				{
					currentElement->next = _element;
					i = _index;
					indexIsHigher = 1;
					(_list->size)++;
				}
				//If the index is not higher, go to the next element
				else
				{
					currentElement = currentElement->next;
				}
			}

			//If the index is not higher than the number of elements in the list
			if (!indexIsHigher)
			{
				temp = currentElement->next;
				currentElement->next = _element;
				_element->next = temp;
				(_list->size)++;
			}
		}
		// If we don't have a first element, we create it
		else
		{
			_list->first = _element;
			_list->last = _element;
			_element->next = nullptr;

			(_list->size)++;
		}

	}
}

//void LinkedListClass::RemoveElementsOfLinkedList(bool _particularElement, int _position)
//{
//	if (this->list->first != nullptr)
//	{
//		if (_particularElement == true)
//		{
//			// Last element of the linked list must be deleted
//			if (_position == -1)
//			{
//				LinkedListClass::sElement *temporary = this->list->last;
//
//				// Case if this is the first and last element of the linked list
//				if (temporary == this->list->first
//					&& temporary == this->list->last)
//				{
//					free(temporary->data);
//					free(temporary);
//
//					// We init the first and the last element of the linked list
//					this->list->first = nullptr;
//					this->list->last = nullptr;
//
//					this->list->size = RESET;
//				}
//				else if (temporary == this->list->last)
//				{
//					LinkedListClass::sElement *current = this->list->first;
//
//					while (current->next != temporary)
//					{
//						current = current->next;
//					}
//
//					current->next = nullptr;
//					this->list->last = current;
//
//					free(temporary->data);
//					free(temporary);
//
//					(this->list->size)--;
//				}
//			}
//			// First element of the linked list must be deleted
//			else if(_position == 1)
//			{
//				LinkedListClass::sElement *temporary = this->list->first;
//
//				// Case if this is the first and last element of the linked list
//				if (temporary == this->list->first
//					&& temporary == this->list->last)
//				{
//					free(temporary->data);
//					free(temporary);
//
//					// We init the first and the last element of the linked list
//					this->list->first = nullptr;
//					this->list->last = nullptr;
//
//					this->list->size = RESET;
//				}
//				// Case if this is the first element of the linked list
//				else if (temporary == this->list->first)
//				{
//					LinkedListClass::sElement *nextElement = temporary->next;
//
//					this->list->first = temporary->next;
//					free(temporary->data);
//					free(temporary);
//
//					(this->list->size)--;
//
//					temporary = nextElement;
//				}
//				
//			}
//			// Any element of the linked list must be deleted
//			else
//			{
//				if (_position > this->list->size
//					|| _position == 0
//					|| _position < -1)
//				{
//					std::cout << "Error due to a linked list output at position : " << _position << std::endl;
//
//					// Implémentation du fichier log errors et ajout de l'erreur
//					std::string errorMessage;
//					errorMessage.append("LINKED LIST   |   Error due to a linked list in RemoveElementsOfLinkedList function's because we want to remove an element at position : ");
//					errorMessage.append(std::to_string(_position));
//
//					AddElementToErrorsLogFile(errorMessage);
//				}
//				else
//				{
//					int positionCounter(1);
//
//					LinkedListClass::sElement *temporary = this->list->first;
//					LinkedListClass::sElement *current = this->list->first;
//
//					while (positionCounter <= _position)
//					{
//						std::cout << "Pos : " << positionCounter << std::endl;
//
//						if (positionCounter != 1)
//						{
//							current = current->next;
//						}
//
//						temporary = temporary->next;
//						positionCounter++;
//					}
//
//					LinkedListClass::sElement *nextElement = temporary->next;
//
//					current->next = temporary->next;
//
//					free(temporary->data);
//					free(temporary);
//
//					(this->list->size)--;
//
//					temporary = nextElement;
//				}
//			}
//
//		}
//		else
//		{
//			LinkedListClass::sElement *temporary = this->list->first;
//
//			// If the first element isn't empty,
//			// that mean that the linked list is composed of one element or more
//			while (temporary != NULL)
//			{
//				if (temporary->status == ELEMENT_DELETION_REQUIRED)
//				{
//					// Case if this is the first and last element of the linked list
//					if (temporary == this->list->first
//						&& temporary == this->list->last)
//					{
//						free(temporary->data);
//						free(temporary);
//
//						// We init the first and the last element of the linked list
//						this->list->first = nullptr;
//						this->list->last = nullptr;
//
//						this->list->size = RESET;
//					}
//					// Case if this is the first element of the linked list
//					else if (temporary == this->list->first)
//					{
//						LinkedListClass::sElement *nextElement = temporary->next;
//
//						this->list->first = temporary->next;
//						free(temporary->data);
//						free(temporary);
//
//						(this->list->size)--;
//
//						temporary = nextElement;
//					}
//					else if (temporary == this->list->last)
//					{
//						LinkedListClass::sElement *current = this->list->first;
//
//						while (current->next != temporary)
//						{
//							current = current->next;
//						}
//
//						current->next = nullptr;
//						this->list->last = current;
//
//						free(temporary->data);
//						free(temporary);
//
//						(this->list->size)--;
//					}
//					else
//					{
//						LinkedListClass::sElement *nextElement = temporary->next;
//						LinkedListClass::sElement *current = this->list->first;
//
//						while (current->next != temporary)
//						{
//							current = current->next;
//						}
//
//						current->next = temporary->next;
//
//						free(temporary->data);
//						free(temporary);
//
//						(this->list->size)--;
//
//						temporary = nextElement;
//					}
//				}
//				else
//				{
//					// We're moving forward in the linked list to other element
//					temporary = temporary->next;
//				}
//			}
//		}
//	}
//	else
//	{
//		std::cout << "Error due to the linked list don't have any element and we need to remove some at position : " << _position << std::endl;
//
//		// Implémentation du fichier log errors et ajout de l'erreur
//		std::string errorMessage;
//		errorMessage.append("LINKED LIST   |   Error due to the linked list in RemoveElementsOfLinkedList function's because the list don't have any element and we need to remove some at position : ");
//		errorMessage.append(std::to_string(_position));
//
//		AddElementToErrorsLogFile(errorMessage);
//	}
//}


void LinkedListClass::RemoveElementsOfLinkedList(LinkedListClass::sLinkedList *_list, bool _particularElement, int _position)
{
	if (_list->first != nullptr)
	{
		if (_particularElement == true)
		{
			// Last element of the linked list must be deleted
			if (_position == -1)
			{
				LinkedListClass::sElement *temporary = _list->last;

				// Case if this is the first and last element of the linked list
				if (temporary == _list->first
					&& temporary == _list->last)
				{
					free(temporary->data);
					free(temporary);

					// We init the first and the last element of the linked list
					_list->first = nullptr;
					_list->last = nullptr;

					_list->size = RESET;
				}
				else if (temporary == _list->last)
				{
					LinkedListClass::sElement *current = _list->first;

					while (current->next != temporary)
					{
						current = current->next;
					}

					current->next = nullptr;
					_list->last = current;

					free(temporary->data);
					free(temporary);

					(_list->size)--;
				}
			}
			// First element of the linked list must be deleted
			else if (_position == 1)
			{
				LinkedListClass::sElement *temporary = _list->first;

				// Case if this is the first and last element of the linked list
				if (temporary == _list->first
					&& temporary == _list->last)
				{
					free(temporary->data);
					free(temporary);

					// We init the first and the last element of the linked list
					_list->first = nullptr;
					_list->last = nullptr;

					_list->size = RESET;
				}
				// Case if this is the first element of the linked list
				else if (temporary == _list->first)
				{
					LinkedListClass::sElement *nextElement = temporary->next;

					_list->first = temporary->next;
					free(temporary->data);
					free(temporary);

					(_list->size)--;

					temporary = nextElement;
				}

			}
			// Any element of the linked list must be deleted
			else
			{
				if (_position > _list->size
					|| _position == 0
					|| _position < -1)
				{
					std::cout << "Error due to a linked list output at position : " << _position << std::endl;

					// Implémentation du fichier log errors et ajout de l'erreur
					std::string errorMessage;
					errorMessage.append("LINKED LIST   |   Error due to a linked list in RemoveElementsOfLinkedList function's because we want to remove an element at position : ");
					errorMessage.append(std::to_string(_position));

					AddElementToErrorsLogFile(errorMessage);
				}
				else
				{
					int positionCounter(1);

					LinkedListClass::sElement *temporary = _list->first;
					LinkedListClass::sElement *current = _list->first;

					while (positionCounter <= _position)
					{
						std::cout << "Pos : " << positionCounter << std::endl;

						if (positionCounter != 1)
						{
							current = current->next;
						}

						temporary = temporary->next;
						positionCounter++;
					}

					LinkedListClass::sElement *nextElement = temporary->next;

					current->next = temporary->next;

					free(temporary->data);
					free(temporary);

					(_list->size)--;

					temporary = nextElement;
				}
			}

		}
		else
		{
			LinkedListClass::sElement *temporary = _list->first;

			// If the first element isn't empty,
			// that mean that the linked list is composed of one element or more
			while (temporary != NULL)
			{
				if (temporary->status == ELEMENT_DELETION_REQUIRED)
				{
					// Case if this is the first and last element of the linked list
					if (temporary == _list->first
						&& temporary == _list->last)
					{
						free(temporary->data);
						free(temporary);

						// We init the first and the last element of the linked list
						_list->first = nullptr;
						_list->last = nullptr;

						_list->size = RESET;
					}
					// Case if this is the first element of the linked list
					else if (temporary == _list->first)
					{
						LinkedListClass::sElement *nextElement = temporary->next;

						_list->first = temporary->next;
						free(temporary->data);
						free(temporary);

						(_list->size)--;

						temporary = nextElement;
					}
					else if (temporary == _list->last)
					{
						LinkedListClass::sElement *current = _list->first;

						while (current->next != temporary)
						{
							current = current->next;
						}

						current->next = nullptr;
						_list->last = current;

						free(temporary->data);
						free(temporary);

						(_list->size)--;
					}
					else
					{
						LinkedListClass::sElement *nextElement = temporary->next;
						LinkedListClass::sElement *current = _list->first;

						while (current->next != temporary)
						{
							current = current->next;
						}

						current->next = temporary->next;

						free(temporary->data);
						free(temporary);

						(_list->size)--;

						temporary = nextElement;
					}
				}
				else
				{
					// We're moving forward in the linked list to other element
					temporary = temporary->next;
				}
			}
		}
	}
	else
	{
		std::cout << "Error due to the linked list don't have any element and we need to remove some at position : " << _position << std::endl;

		// Implémentation du fichier log errors et ajout de l'erreur
		std::string errorMessage;
		errorMessage.append("LINKED LIST   |   Error due to the linked list in RemoveElementsOfLinkedList function's because the list don't have any element and we need to remove some at position : ");
		errorMessage.append(std::to_string(_position));

		AddElementToErrorsLogFile(errorMessage);
	}
}

//// Desallocation of every element of the linked list sent
//void LinkedListClass::FreeLinkedList()
//{
//	if (this->list != nullptr)
//	{
//		if (this->list->first != nullptr)
//		{
//			LinkedListClass::sElement *temporary;
//			LinkedListClass::sElement *currentElement = this->list->first;
//
//			while (currentElement != nullptr)
//			{
//				temporary = currentElement->next;
//
//				free(currentElement->data);
//				free(currentElement);
//
//				currentElement = temporary;
//			}
//		}
//
//		free(this->list);
//	}
//}

// Desallocation of every element of the linked list sent
void LinkedListClass::FreeLinkedList(LinkedListClass::sLinkedList *_list)
{
	if (_list != nullptr)
	{
		if (_list->first != nullptr)
		{
			LinkedListClass::sElement *temporary;
			LinkedListClass::sElement *currentElement = _list->first;

			while (currentElement != nullptr)
			{
				temporary = currentElement->next;

				free(currentElement->data);
				free(currentElement);

				currentElement = temporary;
			}
		}

		free(_list);
	}
}

//void LinkedListClass::ReadLinkedList()
//{
//	if (this->list != nullptr)
//	{
//		if (this->list->first != nullptr)
//		{
//			LinkedListClass::sElement *currentElement = this->list->first;
//
//			int positionCounter(1);
//
//			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
//			{
//				std::cout << "Element : " << positionCounter << "/" << this->list->size << "  -  Next : " << currentElement->next << std::endl;
//				positionCounter++;
//			}
//
//			std::cout << std::endl << std::endl << std::endl;
//		}
//	}
//}

void LinkedListClass::ReadLinkedList(LinkedListClass::sLinkedList *_list)
{
	if (_list != nullptr)
	{
		if (_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = _list->first;

			int positionCounter(1);

			for (currentElement = _list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				std::cout << "Element : " << positionCounter << "/" << _list->size << "  -  Next : " << currentElement->next << std::endl;
				positionCounter++;
			}

			std::cout << std::endl << std::endl << std::endl;
		}
	}
}
