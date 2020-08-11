#ifndef STOREHOUSE__H
#define STOREHOUSE__H

#include "Ressources.h"
#include "SpecificsBuildings.h"
#include "LinkedList.h"


class Storehouse : public LinkedListClass
{
public:
	struct StorehouseData
	{
		sf::Vector2f mapPosition;
		enum BuildingStatus constructionState;

		float lifeTime;

		bool isChangingSprite;
		bool isWorkerThere;
	};
	typedef struct StorehouseData sStorehouseData;

	Storehouse();
	~Storehouse();

	void InitialisationStorehouse(Buildings *_specificBuildingConcerned);
	
	void AddNewBuildingToList(sf::Vector2f _mapPosition);
	void UpdateBuildingConstruction(const float &_frametime);
	//void UpdateInternalCycles(const float &_frametime, Ressources *_ressource, Ressources *_ressourceProduced);
	void UpdateBuildingSprite(unsigned short ***_map);
	//void UpdateBuildingProduction(Ressources *_ressource);

	bool ConfirmStorehousePresenceAtWorkerPosition(const sf::Vector2f &_mapPosition);
	//bool CheckSpecificBuildingHasProducedRessource(const sf::Vector2f &_mapPosition);
	int SpecificsBuildingsSendRessourceProducedToPresentWorker(const sf::Vector2f &_mapPosition, const float &_frametime);
	sf::Vector2i StorehouseFindNearestBuilding(const sf::Vector2f &_mapPosition);

	bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);

private:
	LinkedListClass::sLinkedList *list;
	Buildings *building;
};

#endif // !STOREHOUSE__H