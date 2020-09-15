#include "WorkersList.h"



WorkersList::WorkersList()
{
	this->workerNumberSelected = 1;


	this->sprite = LoadSprite("Data/Assets/Sprites/Entities/worker_test.png", 5);

	this->actionsIcons[0] = LoadSprite("Data/Assets/Sprites/Entities/worker_selected.png", 1);
	this->actionsIcons[1] = LoadSprite("Data/Assets/Sprites/Entities/worker_waiting.png", 1);
	this->actionsIcons[2] = LoadSprite("Data/Assets/Sprites/Entities/worker_working.png", 1);
	this->actionsIcons[3] = LoadSprite("Data/Assets/Sprites/Entities/worker_pickuping.png", 1);
	this->actionsIcons[4] = LoadSprite("Data/Assets/Sprites/Entities/worker_depositing.png", 1);
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

void WorkersList::DisplayWorkersSprite(const sf::Vector2i &_actualPosition, const sf::Vector3f &_cameraPosition, const sf::Vector2f &_gameScale, sf::RenderWindow &_window)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			int positionCounter(1);

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (_actualPosition == sf::Vector2i(((Workers *)currentElement->data)->GetWorkerPosition()))
				{
					sf::Vector2f tileCoordinates = WorldToScreen(((Workers *)currentElement->data)->GetWorkerPosition()); // Faire une version stack
					sf::Vector2f cameraIso = WorldToScreen(_cameraPosition.x, _cameraPosition.y); // Faire une version stack

					this->sprite.setScale(_gameScale);

					BlitSprite(this->sprite,
						(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _cameraPosition.z),
						(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _cameraPosition.z),
						0, _window);

					if (((Workers *)currentElement->data)->GetWorkerStatus() == IDLE)
					{
						BlitSprite(this->actionsIcons[1],
							(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _cameraPosition.z),
							(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _cameraPosition.z) - 70,
							0, _window);
					}
					else if (((Workers *)currentElement->data)->GetWorkerStatus() == WORKING)
					{
						BlitSprite(this->actionsIcons[2],
							(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _cameraPosition.z),
							(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _cameraPosition.z) - 70,
							0, _window);
					}
					else if (((Workers *)currentElement->data)->GetWorkerStatus() == PICKUP_RESSOURCES)
					{
						BlitSprite(this->actionsIcons[3],
							(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _cameraPosition.z),
							(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _cameraPosition.z) - 70,
							0, _window);
					}
					else if (((Workers *)currentElement->data)->GetWorkerStatus() == DEPOSIT_RESSOURCES)
					{
						BlitSprite(this->actionsIcons[4],
							(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _cameraPosition.z),
							(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _cameraPosition.z) - 70,
							0, _window);
					}

					if (positionCounter == this->workerNumberSelected)
					{
						BlitSprite(this->actionsIcons[0],
							(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _cameraPosition.z),
							(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _cameraPosition.z) - 90,
							0, _window);
					}
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


void WorkersList::SavingWorkersListForFile(std::ofstream *_file)
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
				//_file->write((char *) (Workers *)currentElement->data, sizeof(Workers));

				//std::cout << "Map position : " << ((Workers *)currentElement->data)->GetWorkerPosition().x << " " << ((Workers *)currentElement->data)->GetWorkerPosition().y;
				// Save the worker status
				enum WorkerStatus status = ((Workers *)currentElement->data)->GetWorkerStatus();
				_file->write((char *) &status, sizeof(enum WorkerStatus));
				
				if (status == MOVEMENT || WAITING_MOVEMENT)
				{
					float mapEndingPositionX = ((Workers *)currentElement->data)->GetWorkerEndingPosition().x;
					float mapEndingPositionY = ((Workers *)currentElement->data)->GetWorkerEndingPosition().y;

					std::cout << "Save : End map position : " << mapEndingPositionX << " " << mapEndingPositionY << std::endl;
					_file->write((char *) &mapEndingPositionX, sizeof(float));
					_file->write((char *) &mapEndingPositionY, sizeof(float));
				}

				// Save the moneys cost
				int intNumber = ((Workers *)currentElement->data)->GetWorkerMoneyValue();
				_file->write((char *) &intNumber, sizeof(int));
				intNumber = ((Workers *)currentElement->data)->GetWorkerMoneyCostPerMonth();
				_file->write((char *) &intNumber, sizeof(int));

				bool value = ((Workers *)currentElement->data)->GetWorkerIsInWorkingPlace();
				_file->write((char *) &value, sizeof(bool));
				
				// Save the position in map
				sf::Vector2f mapPosition = ((Workers *)currentElement->data)->GetWorkerPosition();
				_file->write((char *)&mapPosition.x, sizeof(float));
				_file->write((char *)&mapPosition.y, sizeof(float));

				float floatingNumber = ((Workers *)currentElement->data)->GetTimeToDeposit();
				_file->write((char *) &floatingNumber, sizeof(float));

				
			}

			std::cout << "Number of workers : " << this->list->size;
		}
	}

	_file->write((char *)&this->workerNumberSelected, sizeof(int));

}


void WorkersList::LoadingWorkersListFromFile(std::ifstream *_file, unsigned short ***_map)
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

		//_file->read((char *)(Workers *)newWorker->data, sizeof(Workers));
		//((Workers *)newWorker->data)->PathfindingReset(); // PROBLEME DE PATHFINDING AU CHARGEMENTS

		// Save the worker status
		enum WorkerStatus status;
		_file->read((char *)&status, sizeof(enum WorkerStatus));

		if (status == MOVEMENT || WAITING_MOVEMENT)
		{
			float mapEndingPositionX(RESET);
			float mapEndingPositionY(RESET);
			_file->read((char *)&mapEndingPositionX, sizeof(float));
			_file->read((char *)&mapEndingPositionY, sizeof(float));

			std::cout << "End map position : " << mapEndingPositionX << " " << mapEndingPositionY << std::endl;

			sf::Vector2i mapEndingPosition = { (int)mapEndingPositionX, (int)mapEndingPositionY };
			((Workers *)newWorker->data)->SetEndingPosition(mapEndingPosition, _map);

			((Workers *)newWorker->data)->SetWorkerStatus(IDLE);
			((Workers *)newWorker->data)->ActiveLauchingMovement();
		}
		else
		{
			((Workers *)newWorker->data)->SetWorkerStatus(status);
		}

		// Save the moneys cost
		int number(RESET);
		_file->read((char *)&number, sizeof(int));
		((Workers *)newWorker->data)->SetWorkerMoneyValue(number);
		_file->read((char *)&number, sizeof(int));
		((Workers *)newWorker->data)->SetWorkerMoneyCostPerMonth(number);

		bool value(RESET);
		_file->read((char *)&value, sizeof(bool));
		((Workers *)newWorker->data)->SetWorkerIsInWorkingPlace(value);

		// Save the position and ending position in map

		sf::Vector2f mapPosition;
		_file->read((char *)&mapPosition.x, sizeof(float));
		_file->read((char *)&mapPosition.y, sizeof(float));
		((Workers *)newWorker->data)->SetWorkerPosition(mapPosition);
		

		float floatingNumber(RESET);
		_file->read((char *)&floatingNumber, sizeof(float));
		((Workers *)newWorker->data)->SetTimeToDeposit(floatingNumber);

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