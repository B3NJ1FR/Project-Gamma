#ifndef SPECIFICS_BUILDINGS__H
#define SPECIFICS_BUILDINGS__H

#include "Ressources.h"
#include "Buildings.h"
#include "LinkedList.h"

enum BuildingStatus
{
	PLANNED,
	CONSTRUCTION,
	BUILT,
	BUILDING_NOT_MAINTAINED,
};


enum MainBuildingStatus
{
	BUILDING_READY_TO_PRODUCE,
	BUILDING_FILLING,
	BUILDING_WORKS,
	BUILDING_COLLECTING_PRODUCTION,
	BUILDING_NEED_TO_BE_CLEANED,
};


class SpecificsBuildings : public LinkedListClass
{
public:	
	struct BuildingData
	{
		sf::Vector2f mapPosition;
		enum BuildingStatus constructionState;
		enum MainBuildingStatus actualState;

		int quantitativeThreshold; // Seuil minimum de lancement
		int maximalQuantity; // Seuil minimum de lancement
		int internalRessourceCounter;

		float lifeTime;
		float actualProductionTime;

		bool isChangingSprite;
		bool isProduced;
		bool isWorkerThere;
	};
	typedef struct BuildingData sBuildingData;

	SpecificsBuildings();
	~SpecificsBuildings();

	void InitialisationSpeBuilding(Buildings *_specificBuildingConcerned);
	//void ReadVineLinkedList();
	void AddNewBuildingToList(sf::Vector2f _mapPosition);
	void UpdateBuildingConstruction(const float &_frametime);
	void UpdateInternalCycles(const float &_frametime, Ressources *_ressource, Ressources *_ressourceProduced);
	void UpdateBuildingSprite(unsigned short ***_map);
	void UpdateBuildingProduction(Ressources *_ressource);
	bool CheckSpecificBuildingPresenceAtPosition(const sf::Vector2f &_mapPosition);

private:
	LinkedListClass::sLinkedList *list;
	Buildings *building;
};

#endif // !SPECIFICS_BUILDINGS__H