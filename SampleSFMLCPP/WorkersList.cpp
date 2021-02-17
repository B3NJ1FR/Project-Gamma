#include "WorkersList.h"
#include "GameDefinitions.h"



WorkersList::WorkersList()
{
	m_workerNumberSelected = 1;


	m_sprite = LoadSprite("Data/Assets/Sprites/Entities/worker_test.png", 5);

	m_actionsIcons[0] = LoadSprite("Data/Assets/Sprites/Entities/worker_selected.png", 1);
	m_actionsIcons[1] = LoadSprite("Data/Assets/Sprites/Entities/worker_waiting.png", 1);
	m_actionsIcons[2] = LoadSprite("Data/Assets/Sprites/Entities/worker_working.png", 1);
	m_actionsIcons[3] = LoadSprite("Data/Assets/Sprites/Entities/worker_pickuping.png", 1);
	m_actionsIcons[4] = LoadSprite("Data/Assets/Sprites/Entities/worker_depositing.png", 1);

	InitialisationWorkersList();
}

WorkersList::~WorkersList()
{
	if (m_list != nullptr)
	{
		delete[] m_list;
	}

	delete (m_sprite.getTexture());

	for (size_t i = 0; i < NUMBER_OF_WORKERS_ACTIONS; i++)
	{
		delete (m_actionsIcons[i].getTexture());
	}
}

void WorkersList::InitialisationWorkersList()
{
	std::cout << "List before : " << m_list << std::endl;

	m_list = LinkedListInitialisation();
	
	std::cout << "List " << m_list << " Size : " << m_list->size << " Real First : " << m_list->first << " & Last : " << m_list->last << std::endl;
}


sf::Sprite WorkersList::GetCurrentWorkerSprite() const
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement* currentElement = m_list->first;

			int positionCounter(1);

			for (currentElement = m_list->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				if (positionCounter == m_workerNumberSelected)
				{
					// IN THE FUTURE : GIVE THE CORRECT SPRITE OF THE CURRENT WORKER
					return m_sprite;
				}

				positionCounter++;
			}

		}
	}
}


void WorkersList::ReadWorkersLinkedList()
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			int positionCounter(1);

			for (currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				std::cout << "Worker : " << positionCounter << "/" << m_list->size << "  -  Position : " << ((Workers *)currentElement->data)->GetWorkerPosition().x << " " << ((Workers *)currentElement->data)->GetWorkerPosition().y << std::endl;
				positionCounter++;
			}

			//std::cout << std::endl << std::endl << std::endl;
		}
	}
}

void WorkersList::AddNewWorkersToList(const sf::Vector2f& _mapPosition)
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
	AddElementToLinkedList(m_list, newWorker, -1);

	//ReadVineLinkedList();
	//ReadLinkedList(list);
}

void WorkersList::UpdateWorkersLife(struct Game *_game)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;
			
			for (currentElement = m_list->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				((Workers *)currentElement->data)->InitPathfinding(&_game->m_map);
				((Workers *)currentElement->data)->UpdatePathAndActivities(&_game->m_map, _game->m_time, &_game->m_builds, _game->m_ressources);
			}
		}
	}
}

void WorkersList::DisplayWorkersSprite(const sf::Vector2i &_actualPosition, const sf::Vector3f &_cameraPosition, const sf::Vector2f &_gameScale, const bool &_characterIsSelected, sf::RenderWindow &_window)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			int positionCounter(1);

			for (currentElement = m_list->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				if (_actualPosition == sf::Vector2i(((Workers *)currentElement->data)->GetWorkerPosition()))
				{
					sf::Vector2f tileCoordinates = WorldToScreen(((Workers *)currentElement->data)->GetWorkerPosition());
					sf::Vector2f cameraIso = WorldToScreen(_cameraPosition.x, _cameraPosition.y);

					m_sprite.setScale(_gameScale);


					if (((Workers *)currentElement->data)->GetWorkerStatus() != WORKING
						|| (((Workers *)currentElement->data)->GetWorkerStatus() == WORKING
							&& (((Workers *)currentElement->data)->GetWorkerActualBuilding() == BUILDING_VINES
								|| ((Workers *)currentElement->data)->GetWorkerActualBuilding() == BUILDING_STALL)))
					{
						BlitSprite(m_sprite,
							(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _cameraPosition.z),
							(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _cameraPosition.z),
							0, _window);
					}
					

					switch (((Workers*)currentElement->data)->GetWorkerStatus())
					{
					case IDLE:
						BlitSprite(m_actionsIcons[1],
							(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _cameraPosition.z),
							(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _cameraPosition.z) - 70,
							0, _window);
						break;
					case WORKING:
						//BlitSprite(actionsIcons[2],
						//	(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _cameraPosition.z),
						//	(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _cameraPosition.z) - 70,
						//	0, _window);
						break;
					case PICKUP_RESSOURCES:
						BlitSprite(m_actionsIcons[3],
							(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _cameraPosition.z),
							(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _cameraPosition.z) - 70,
							0, _window);
						break;
					case DEPOSIT_RESSOURCES:
						BlitSprite(m_actionsIcons[4],
							(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _cameraPosition.z),
							(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _cameraPosition.z) - 70,
							0, _window);
						break;
					default:
						break;
					}

					if (!_characterIsSelected)
					{
						if (positionCounter == m_workerNumberSelected)
						{
							BlitSprite(m_actionsIcons[0],
								(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _cameraPosition.z),
								(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _cameraPosition.z) - 90,
								0, _window);
						}
					}
				}

				positionCounter++;
			}

		}
	}
}

void WorkersList::ChangeWorkerNumberSelectedAdd()
{
	if (m_workerNumberSelected + 1 <= m_list->size)
	{
		m_workerNumberSelected += 1;
	}
	else
	{
		m_workerNumberSelected = 1;
	}
}

void WorkersList::ChangeWorkerNumberSelectedSubtract()
{
	if (m_workerNumberSelected - 1 >= 1)
	{
		m_workerNumberSelected -= 1;
	}
	else
	{
		m_workerNumberSelected = m_list->size;
	}
}

void WorkersList::WorkerListSetEndPosition(const sf::Vector2i &_mapPosition, unsigned short ***_map)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			int positionCounter(1);

			while (positionCounter != m_workerNumberSelected)
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
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			int positionCounter(1);

			for (currentElement = m_list->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				if (((Workers *)currentElement->data)->GetWorkerStatus() == WAITING_MOVEMENT
					|| ((Workers *)currentElement->data)->GetWorkerStatus() == MOVEMENT)
				{
					//std::cout << "The worker number " << positionCounter++ << "/" << list->size << " need to change his path\n\n";

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
	auto listSize = m_list->size;
	_file->write((char *)&listSize, sizeof(int));

	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			for (currentElement = m_list->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				// Save the worker status
				enum WorkerStatus status = ((Workers *)currentElement->data)->GetWorkerStatus();
				_file->write((char *) &status, sizeof(enum WorkerStatus));


				if (status == MOVEMENT || status == WAITING_MOVEMENT)
				{
					float mapEndingPositionX = ((Workers *)currentElement->data)->GetWorkerEndingPosition().x;
					float mapEndingPositionY = ((Workers *)currentElement->data)->GetWorkerEndingPosition().y;

					//std::cout << "Save : End map position : " << mapEndingPositionX << " " << mapEndingPositionY << std::endl;
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

			//std::cout << "Number of workers : " << m_list->size;
		}
	}

	_file->write((char *)&m_workerNumberSelected, sizeof(int));

}


void WorkersList::LoadingWorkersListFromFile(std::ifstream *_file, unsigned short ***_map)
{
	// Delete every paths if workers are moving
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			for (currentElement = m_list->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				if (((Workers *)currentElement->data)->GetWorkerStatus() == MOVEMENT)
				{
					if (m_path != nullptr)
					{
						delete m_path;
						m_path = nullptr;
					}
				}
			}
		}

		FreeLinkedList(m_list);
	}
	

	// We reinit the workers list
	InitialisationWorkersList();


	// Load the number of workers
	int previousListSize = 0;
	_file->read((char *) &previousListSize, sizeof(int));

	//std::cout << "Number of workers : " << previousListSize;

	// We add every workers data to the list
	for (int i = 0; i < previousListSize; i++)
	{
		LinkedListClass::sElement* newWorker = new LinkedListClass::sElement;
		newWorker->data = new Workers;

		// Save the worker status
		enum WorkerStatus status;
		_file->read((char *)&status, sizeof(enum WorkerStatus));

		if (status == MOVEMENT || status == WAITING_MOVEMENT)
		{
			float mapEndingPositionX(RESET);
			float mapEndingPositionY(RESET);
			_file->read((char *)&mapEndingPositionX, sizeof(float));
			_file->read((char *)&mapEndingPositionY, sizeof(float));

			//std::cout << "End map position : " << mapEndingPositionX << " " << mapEndingPositionY << std::endl;

			sf::Vector2i mapEndingPosition = { (int)mapEndingPositionX, (int)mapEndingPositionY };
			((Workers *)newWorker->data)->SetEndingPosition(mapEndingPosition, _map);

			((Workers *)newWorker->data)->SetWorkerStatus(IDLE);
			((Workers *)newWorker->data)->ActiveLauchingMovement();
		}
		else
		{
			((Workers *)newWorker->data)->SetWorkerStatus(status);
		}

		// Load the moneys cost
		int number(RESET);
		_file->read((char *)&number, sizeof(int));
		((Workers *)newWorker->data)->SetWorkerMoneyValue(number);
		_file->read((char *)&number, sizeof(int));
		((Workers *)newWorker->data)->SetWorkerMoneyCostPerMonth(number);

		bool value(RESET);
		_file->read((char *)&value, sizeof(bool));
		((Workers *)newWorker->data)->SetWorkerIsInWorkingPlace(value);

		// Load the position and ending position in map
		sf::Vector2f mapPosition;
		_file->read((char *)&mapPosition.x, sizeof(float));
		_file->read((char *)&mapPosition.y, sizeof(float));
		((Workers *)newWorker->data)->SetWorkerPosition(mapPosition);
		

		float floatingNumber(RESET);
		_file->read((char *)&floatingNumber, sizeof(float));
		((Workers *)newWorker->data)->SetTimeToDeposit(floatingNumber);

		newWorker->status = ELEMENT_ACTIVE;

		//std::cout << "Map position : " << ((Workers *)newWorker->data)->GetWorkerPosition().x << " " << ((Workers *)newWorker->data)->GetWorkerPosition().y << std::endl;

		if (i == 0)
		{
			// Add this worker at the top of the list
			AddElementToLinkedList(m_list, newWorker, 1);
		}
		else
		{
			// Add this worker at the end of the list
			AddElementToLinkedList(m_list, newWorker, -1);
		}

		//std::cout << "Map position : " << ((Workers *)newWorker->data)->GetWorkerPosition().x << " " << ((Workers *)newWorker->data)->GetWorkerPosition().y;
	}

	_file->read((char *)&m_workerNumberSelected, sizeof(int));
}