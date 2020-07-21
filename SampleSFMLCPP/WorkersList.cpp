#include "WorkersList.h"



WorkersList::WorkersList()
{
	this->workerNumberSelected = 1;
}

WorkersList::~WorkersList()
{

}

void WorkersList::InitialisationWorkersList()
{
	std::cout << "List before : " << this->list << std::endl;

	this->list = LinkedListInitialisation();
	
	std::cout << "List " << this->list << " Size : " << this->list->size << " Real First : " << this->list->first << " & Last : " << this->list->last << std::endl;
}

void WorkersList::ReadWorkersLinkedList()
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			int positionCounter(1);

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				std::cout << "Worker : " << positionCounter << "/" << this->list->size << "  -  Position : " << ((Workers *)currentElement->data)->GetWorkerPosition().x << " " << ((Workers *)currentElement->data)->GetWorkerPosition().y << std::endl;
				positionCounter++;
			}

			//std::cout << std::endl << std::endl << std::endl;
		}
	}
}

void WorkersList::AddNewWorkersToList(sf::Vector2f _mapPosition)
{
	LinkedListClass::sElement* newWorker = new LinkedListClass::sElement;
	newWorker->data = new Workers;

	// Save the position in map
	((Workers *)newWorker->data)->SetWorkerPosition(_mapPosition);

	// Init of the worker status
	((Workers *)newWorker->data)->SetWorkerStatus(IDLE);

		
	((Workers *)newWorker->data)->SetWorkerMoneyValue(RESET);
	((Workers *)newWorker->data)->SetWorkerMoneyCostPerMonth(RESET);
	

	newWorker->status = ELEMENT_ACTIVE;

	// Add this new vine at the end of the list
	this->AddElementToLinkedList(this->list, newWorker, -1);

	//this->ReadVineLinkedList();
	this->ReadLinkedList(this->list);
}

void WorkersList::UpdateWorkersLife(struct Game *_game)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;
			
			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				((Workers *)currentElement->data)->InitPathfinding(_game);
				((Workers *)currentElement->data)->UpdatePathAndActivities(_game);
			}
		}
	}
}

void WorkersList::DisplayWorkersSprite(const sf::Vector3f &_cameraPosition, sf::Sprite *_sprite, sf::RenderWindow &_window)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			int positionCounter(1);

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				sf::Vector2f tileCoordinates = WorldToScreen(((Workers *)currentElement->data)->GetWorkerPosition()); // Faire une version stack
				sf::Vector2f cameraIso = WorldToScreen(_cameraPosition.x, _cameraPosition.y); // Faire une version stack

				BlitSprite(*_sprite,
					(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _cameraPosition.z),
					(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _cameraPosition.z),
					0, _window);
			}

		}
	}
}

void WorkersList::UpdateWorkersProduction(Ressources *_ressource)
{
	/*if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			int positionCounter(1);

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				std::cout << "Worker : " << positionCounter << "/" << this->list->size << "  -  Position : " << ((Workers *)currentElement->data)->GetWorkerPosition().x << " " << ((Workers *)currentElement->data)->GetWorkerPosition().y << std::endl;
				positionCounter++;
			}
		}
	}*/
}


void WorkersList::ChangeWorkerNumberSelectedAdd()
{
	if (this->workerNumberSelected + 1 <= this->list->size)
	{
		this->workerNumberSelected++;
	}
	else
	{
		this->workerNumberSelected = 1;
	}
}

void WorkersList::ChangeWorkerNumberSelectedSubtract()
{
	if (this->workerNumberSelected - 1 >= 1)
	{
		this->workerNumberSelected--;
	}
	else
	{
		this->workerNumberSelected = this->list->size;
	}
}

void WorkersList::WorkerListSetEndPosition(const sf::Vector2i &_mapPosition, unsigned short ***_map)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			int positionCounter(1);

			while (positionCounter != this->workerNumberSelected)
			{
				currentElement = currentElement->next;
				positionCounter++;
			}

			((Workers *)currentElement->data)->SetEndingPosition(_mapPosition, _map);
			((Workers *)currentElement->data)->SetWorkerStatus(IDLE);
			((Workers *)currentElement->data)->ActiveLauchingMovement();
		}
	}
}


bool WorkersList::CheckWorkersPresenceAtPosition(const sf::Vector2f &_mapPosition)
{
	/*if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			int positionCounter(1);

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				std::cout << "Worker : " << positionCounter << "/" << this->list->size << "  -  Position : " << ((Workers *)currentElement->data)->GetWorkerPosition().x << " " << ((Workers *)currentElement->data)->GetWorkerPosition().y << std::endl;
				positionCounter++;
			}
		}
	}*/
}
