#pragma once

#include "Workers.h"

#define NUMBER_OF_WORKERS_ACTIONS 5

class WorkersList : protected Workers
{
private:
	LinkedListClass::sLinkedList* m_list;
	int m_workerNumberSelected;

	sf::Sprite m_sprite;
	sf::Sprite m_actionsIcons[NUMBER_OF_WORKERS_ACTIONS];

public:
	// Constructor & Destructor
	WorkersList();
	~WorkersList();

	// Getters
	sf::Sprite GetCurrentWorkerSprite() const;
	inline int GetNumberOfWorkers() const { if(m_list != nullptr) { return m_list->size; } else { return 0; }; };

	// Methods
	void InitialisationWorkersList();
	void ReadWorkersLinkedList();
	void AddNewWorkersToList(const sf::Vector2f& _mapPosition);
	void UpdateWorkersLife(struct Game *_game);
	void DisplayWorkersSprite(const sf::Vector2i &_actualPosition, const sf::Vector3f &_cameraPosition, const sf::Vector2f &_gameScale, const bool& _characterIsSelected, sf::RenderWindow &_window);
	void ChangeWorkerNumberSelectedAdd();
	void ChangeWorkerNumberSelectedSubtract();
	void WorkerListSetEndPosition(const sf::Vector2i &_mapPosition, unsigned short ***_map);
	void CheckAndUpdateWorkersPath(unsigned short ***_map);

	// Save and Load
	void SavingWorkersListForFile(std::ofstream *_file);
	void LoadingWorkersListFromFile(std::ifstream *_file, unsigned short ***_map);

};
