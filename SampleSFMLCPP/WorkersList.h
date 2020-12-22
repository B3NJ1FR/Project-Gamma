#pragma once

#include "Workers.h"

class WorkersList : public Workers
{
private:
	LinkedListClass::sLinkedList* m_list;
	int m_workerNumberSelected;

	sf::Sprite m_sprite;
	sf::Sprite m_actionsIcons[5];

public:
	// Constructor & Destructor
	WorkersList();
	~WorkersList();

	// Methods
	void InitialisationWorkersList();
	void ReadWorkersLinkedList();
	void AddNewWorkersToList(const sf::Vector2f& _mapPosition);
	void UpdateWorkersLife(struct Game *_game);
	void DisplayWorkersSprite(const sf::Vector2i &_actualPosition, const sf::Vector3f &_cameraPosition, const sf::Vector2f &_gameScale, sf::RenderWindow &_window);
	void ChangeWorkerNumberSelectedAdd();
	void ChangeWorkerNumberSelectedSubtract();
	void WorkerListSetEndPosition(const sf::Vector2i &_mapPosition, unsigned short ***_map);
	void CheckAndUpdateWorkersPath(unsigned short ***_map);
	void SavingWorkersListForFile(std::ofstream *_file);
	void LoadingWorkersListFromFile(std::ifstream *_file, unsigned short ***_map);

};
