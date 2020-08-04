#ifndef STOREHOUSE__H
#define STOREHOUSE__H

#include "Ressources.h"
#include "Buildings.h"
#include "LinkedList.h"


class Storehouse : public LinkedListClass
{
private:
	//Buildings *building;


	sf::Vector2f mapPosition;
	//enum BuildingStatus constructionState;
	enum StallStatus actualState;

	//int quantitativeThreshold; // Seuil minimum de lancement
	//int maximalQuantity; // Seuil maximal
	//int internalImportRessourceCounter;
	//int internalExportRessourceCounter;

	//float lifeTime;
	//float actualProductionTime;
	//float secondaryTime;

	//bool isChangingSprite;
	//bool isProduced;
	//bool isWorkerThere;
	//bool isPurchaserThere;

public:
	
	Storehouse();
	~Storehouse();

	/*void SetStatus(const enum StallStatus &_newStatus);
	enum StallStatus GetStatus();
	void InitialisationStall(Buildings *_specificBuildingConcerned);
	void UpdateQuantityConvertedToSell(Purchasers *_purchasers);
	void UpdateSellingWindowTexts(struct Game *_game, Purchasers *_purchasers);
	void DisplaySellingWindow(struct Game *_game);
	void InputSellingWindow(struct Game *_game, bool *_isOfferAccepted, enum GameState *_state);
	void AddNewBuilding(sf::Vector2f _mapPosition);
	void UpdateBuildingConstruction(const float &_frametime);
	void UpdateInternalCycles(class Money *_money, enum GameState *_state, const float &_frametime, Ressources *_ressource, Purchasers *_purchasers);*/
	//void UpdateBuildingSprite(unsigned short ***_map);
	//void UpdateBuildingProduction(Ressources *_ressource);

	//bool ConfirmPresenceAtWorkerPosition(const sf::Vector2f &_mapPosition);
	//bool CheckSpecificBuildingHasProducedRessource(const sf::Vector2f &_mapPosition);
	//int SpecificsBuildingsSendRessourceProducedToPresentWorker(const sf::Vector2f &_mapPosition, const float &_frametime);
	//sf::Vector2i SpecificsBuildingsFindNearestBuilding(const sf::Vector2f &_mapPosition);

	//bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);

};

#endif // !STOREHOUSE__H