#ifndef WORKERS_LIST__H
#define WORKERS_LIST__H

#include "Workers.h"

class WorkersList : public Workers
{
public:
	WorkersList();
	~WorkersList();

	void InitialisationWorkersList();
	void ReadWorkersLinkedList();
	void AddNewWorkersToList(sf::Vector2f _mapPosition);
	void UpdateWorkersLife(struct Game *_game);
	void DisplayWorkersSprite(const sf::Vector2i &_actualPosition, const sf::Vector3f &_cameraPosition, const sf::Vector2f &_gameScale, sf::RenderWindow &_window);
	void UpdateWorkersProduction(Ressources *_ressource);
	void ChangeWorkerNumberSelectedAdd();
	void ChangeWorkerNumberSelectedSubtract();
	void WorkerListSetEndPosition(const sf::Vector2i &_mapPosition, unsigned short ***_map);
	void CheckAndUpdateWorkersPath(unsigned short ***_map);
	void SavingWorkersListForFile(std::ofstream *_file);
	void LoadingWorkersListFromFile(std::ifstream *_file, unsigned short ***_map);

private:
	LinkedListClass::sLinkedList *list;
	int workerNumberSelected;

	sf::Sprite sprite;
	sf::Sprite actionsIcons[5];
};

#endif // !WORKERS_LIST__H
