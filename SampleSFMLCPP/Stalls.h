#ifndef STALLS__H
#define STALLS__H

#include "Ressources.h"
#include "Buildings.h"
#include "LinkedList.h"
#include "Purchasers.h"


enum StallStatus
{
	STALL_READY_TO_WORKS,
	STALL_FILLING,
	STALL_SEND_REQUEST_PURCHASER,
	STALL_PURCHASER_IS_PRESENT,
	STALL_OFFER_HANDLED,
};


class Stalls : public LinkedListClass
{
private:
	Buildings *building;
	

	sf::Vector2f mapPosition;
	enum BuildingStatus constructionState;
	enum StallStatus actualState;

	int quantitativeThreshold; // Seuil minimum de lancement
	int maximalQuantity; // Seuil maximal
	int internalImportRessourceCounter;
	int internalExportRessourceCounter;

	float lifeTime;
	float actualProductionTime;
	//float secondaryTime;

	bool isChangingSprite;
	//bool isProduced;
	bool isWorkerThere;
	bool isPurchaserThere;

	sf::Sprite sellingWindowBackground;
	sf::Sprite sellingWindowAcceptButton;
	sf::Sprite sellingWindowRejectButton;
	sf::Sprite sellingWindowScrollButton;
	sf::Sprite sellingWindowScrollLine;
	sf::Text sellingWindowProvenance;
	sf::Text sellingWindowPrice[3];
	sf::Text sellingWindowRessourceQuantity[4];
	int sellingWindowScrollButtonPosition;
	int quantityConvertedToSell;
	int priceAccepted;
	bool wasCursorPressed;

public:
	
	bool isOfferAccepted;

	Stalls(sf::Font *_font);
	~Stalls();

	void SetStatus(const enum StallStatus &_newStatus);
	enum StallStatus GetStatus();
	void InitialisationStall(Buildings *_specificBuildingConcerned);
	void UpdateQuantityConvertedToSell(Purchasers *_purchasers);
	void UpdateSellingWindowTexts(struct Game *_game, Purchasers *_purchasers);
	void DisplaySellingWindow(struct Game *_game);
	void InputSellingWindow(struct Game *_game, bool *_isOfferAccepted, enum GameState *_state);
	//void AddNewBuildingToList(sf::Vector2f _mapPosition);
	void UpdateBuildingConstruction(const float &_frametime);
	void UpdateInternalCycles(class Money *_money, enum GameState *_state, const float &_frametime, Ressources *_ressource, Purchasers *_purchasers);
	//void UpdateBuildingSprite(unsigned short ***_map);
	//void UpdateBuildingProduction(Ressources *_ressource);

	//bool ConfirmSpecificBuildingPresenceAtWorkerPosition(const sf::Vector2f &_mapPosition);
	//bool CheckSpecificBuildingHasProducedRessource(const sf::Vector2f &_mapPosition);
	//int SpecificsBuildingsSendRessourceProducedToPresentWorker(const sf::Vector2f &_mapPosition, const float &_frametime);
	//sf::Vector2i SpecificsBuildingsFindNearestBuilding(const sf::Vector2f &_mapPosition);

	//bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);

};

#endif // !STALLS__H