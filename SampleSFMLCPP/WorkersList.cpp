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
	//this->ReadLinkedList(this->list);
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

void WorkersList::DisplayWorkersSprite(const sf::Vector3f &_cameraPosition, sf::Sprite *_sprite, sf::RenderWindow &_window, sf::Sprite *_iconsSprite)
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

				if (((Workers *)currentElement->data)->GetWorkerStatus() == IDLE)
				{
					BlitSprite(_iconsSprite[1],
						(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _cameraPosition.z),
						(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _cameraPosition.z) - 70,
						0, _window);
				}
				else if (((Workers *)currentElement->data)->GetWorkerStatus() == WORKING)
				{
					BlitSprite(_iconsSprite[2],
						(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _cameraPosition.z),
						(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _cameraPosition.z) - 70,
						0, _window);
				}
				else if (((Workers *)currentElement->data)->GetWorkerStatus() == PICKUP_RESSOURCES)
				{
					BlitSprite(_iconsSprite[3],
						(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _cameraPosition.z),
						(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _cameraPosition.z) - 70,
						0, _window);
				}
				else if (((Workers *)currentElement->data)->GetWorkerStatus() == DEPOSIT_RESSOURCES)
				{
					BlitSprite(_iconsSprite[4],
						(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _cameraPosition.z),
						(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _cameraPosition.z) - 70,
						0, _window);
				}

				if (positionCounter == this->workerNumberSelected)
				{
					BlitSprite(_iconsSprite[0],
						(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _cameraPosition.z),
						(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _cameraPosition.z) - 90,
						0, _window);
				}

				positionCounter++;
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


void WorkersList::CheckAndUpdateWorkersPath(unsigned short ***_map)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			int positionCounter(1);

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Workers *)currentElement->data)->GetWorkerStatus() == WAITING_MOVEMENT
					|| ((Workers *)currentElement->data)->GetWorkerStatus() == MOVEMENT)
				{
					//std::cout << "The worker number " << positionCounter++ << "/" << this->list->size << " need to change his path\n\n";

					((Workers *)currentElement->data)->SetEndingPosition((sf::Vector2i)((Workers *)currentElement->data)->GetWorkerEndingPosition(), _map);
					
					((Workers *)currentElement->data)->SetWorkerStatus(IDLE);
					((Workers *)currentElement->data)->ActiveLauchingMovement();
				}

				positionCounter++;
			}
		}
	}
}


void WorkersList::SavingWorkersListFromFile(std::ofstream *_file)
{
	// Save the number of workers
	_file->write((char *)&this->list->size, sizeof(int));


	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				_file->write((char *) (Workers *)currentElement->data, sizeof(Workers));

				std::cout << "Map position : " << ((Workers *)currentElement->data)->GetWorkerPosition().x << " " << ((Workers *)currentElement->data)->GetWorkerPosition().y;
				// Save the worker status
				//_file->write((char *) ((Workers *)currentElement->data)->GetWorkerStatus(), sizeof(enum WorkerStatus));

				//// Save the moneys cost
				//_file->write((char *) ((Workers *)currentElement->data)->GetWorkerMoneyValue(), sizeof(int));
				//_file->write((char *) ((Workers *)currentElement->data)->GetWorkerMoneyCostPerMonth(), sizeof(int));

				//_file->write((char *) ((Workers *)currentElement->data)->GetWorkerIsInWorkingPlace(), sizeof(bool));
				//
				//// Save the position and ending position in map
				//_file->write((char *) ((Workers *)currentElement->data)->GetWorkerPosition().x, sizeof(float));
				//_file->write((char *) ((Workers *)currentElement->data)->GetWorkerPosition().y, sizeof(sf::Vector2f));
				//_file->write((char *) ((Workers *)currentElement->data)->GetWorkerEndingPosition().x, sizeof(sf::Vector2f));
				//_file->write((char *) ((Workers *)currentElement->data)->GetWorkerEndingPosition().y, sizeof(sf::Vector2f));
				//
				//_file->write((char *) ((Workers *)currentElement->data)->GetTimeToDeposit(), sizeof(float));

				
			}

			std::cout << "Number of workers : " << this->list->size;
		}
	}

	_file->write((char *)&this->workerNumberSelected, sizeof(int));

}


void WorkersList::LoadingWorkersListFromFile(std::ifstream *_file)
{
	// Delete every paths if workers are moving
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Workers *)currentElement->data)->GetWorkerStatus() == MOVEMENT)
				{
					if (this->path != nullptr)
					{
						delete this->path;
						this->path = nullptr;
					}
				}
			}
		}

		this->FreeLinkedList(this->list);
	}
	

	// We reinit the workers list
	this->InitialisationWorkersList();


	// Load the number of workers
	int previousListSize(RESET);
	_file->read((char *)&previousListSize, sizeof(int));
	
	// We add every workers data to the list
	for (int i = RESET; i < previousListSize; i++)
	{
		LinkedListClass::sElement* newWorker = new LinkedListClass::sElement;
		newWorker->data = new Workers;

		_file->read((char *)(Workers *)newWorker->data, sizeof(Workers));
		((Workers *)newWorker->data)->PathfindingReset(); // PROBLEME DE PATHFINDING AU CHARGEMENTS

		newWorker->status = ELEMENT_ACTIVE;

		std::cout << "Map position : " << ((Workers *)newWorker->data)->GetWorkerPosition().x << " " << ((Workers *)newWorker->data)->GetWorkerPosition().y;

		if (i == 0)
		{
			// Add this worker at the top of the list
			this->AddElementToLinkedList(this->list, newWorker, 1);
		}
		else
		{
			// Add this worker at the end of the list
			this->AddElementToLinkedList(this->list, newWorker, -1);
		}

		//std::cout << "Map position : " << ((Workers *)newWorker->data)->GetWorkerPosition().x << " " << ((Workers *)newWorker->data)->GetWorkerPosition().y;
	}

	_file->read((char *)&this->workerNumberSelected, sizeof(int));

}