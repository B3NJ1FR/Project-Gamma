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
	void DisplayWorkersSprite(const sf::Vector3f &_cameraPosition, sf::Sprite *_sprite, sf::RenderWindow &_window, sf::Sprite *_iconsSprite);
	void UpdateWorkersProduction(Ressources *_ressource);
	void ChangeWorkerNumberSelectedAdd();
	void ChangeWorkerNumberSelectedSubtract();
	void WorkerListSetEndPosition(const sf::Vector2i &_mapPosition, unsigned short ***_map);
	void CheckAndUpdateWorkersPath(unsigned short ***_map);
	void SavingWorkersListFromFile(std::ofstream *_file);
	void LoadingWorkersListFromFile(std::ifstream *_file);

private:
	LinkedListClass::sLinkedList *list;
	int workerNumberSelected;
};

#endif // !WORKERS_LIST__H
