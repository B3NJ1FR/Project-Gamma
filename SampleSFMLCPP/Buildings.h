#ifndef BUILDINGS__H
#define BUILDINGS__H

#include "Common.hpp"

enum TypeOfBuilding
{
	BUILDING_VINES,
	BUILDING_GRAPE_STOMPING_VATS,
	BUILDING_WINE_PRESS,
	BUILDING_WINE_STOREHOUSE,
	BUILDING_STOREHOUSE,
	BUILDING_STALL,

	BUILDING_VILLA, // NEXT UPDATE
	BUILDING_DORMITORY, // NEXT UPDATE
};


class Buildings
{
protected :

	sf::Vector2i size;
	sf::Vector2i enterPosition;
	sf::Vector2i exitPosition;

	sf::Sprite sprite;
	sf::Sprite icon;

	std::string name;
	std::string description;

	unsigned int id;

	int moneyCostToBuild;
	float timeToBuild;
	float timeToProduce;

	float timeToPickUpRessources;
	float timeToDeposeRessources;

	unsigned char numberRessourceNeeded;
	int *ressourceIDNeeded;
	int *ressourceQuantityNeeded;

	unsigned char numberRessourceProduced;
	int *ressourceIDProduced;
	int *ressourceQuantityProduced;

public :

	Buildings();
	~Buildings();

	void SetName(const std::string _name);

	void SetSizeCaracteristics(const sf::Vector2i _buildingSize);
	void SetEntranceCaracteristics(const sf::Vector2i _enterPosition);
	void SetExitCaracteristics(const sf::Vector2i _exitPosition);

	void SetSprite(const std::string _spriteFileName, int _spriteOriginPosition = 0);
	void SetIcon(const std::string _iconFileName, int _iconOriginPosition = 0);
	void SetConstructionMoneyCost(const int _moneyCost);
	void SetConstructionTimeCost(const float &_timeCost);
	void SetProductionTimeCost(const float &_timeCost);
	void SetPickupingTimeCost(const float &_timeCost = 0);
	void SetDepositingTimeCost(const float &_timeCost = 0);
	void SetRessourceIDNeeded(int *_resssourceID, const unsigned char _numberRessources = 1);
	void SetRessourceQuantityNeeded(int *_resssourceQuantity = 0);
	void SetRessourceIDProduced(int *_resssourceID, const unsigned char _numberRessources = 1);
	void SetRessourceQuantityProduced(int *_resssourceQuantity = 0);

	std::string GetName();
	std::string GetDescription();
	sf::Sprite GetSprite();
	sf::Sprite GetIcon();
	sf::Vector2i GetSize();
	int GetConstructionCost();
	float GetConstructionTimeCost();
	float GetProductionTimeCost();
	float GetDepositingTimeCost();
	float GetPickupingTimeCost();
	int GetRessourceIDNeeded(const unsigned char &_value = 1);
	int GetRessourceQuantityNeeded(const unsigned char &_value = 1);
	int GetRessourceIDProduced(const unsigned char &_value = 1);
	int GetRessourceQuantityProduced(const unsigned char &_value = 1);
};




#endif // !BUILDINGS__H
