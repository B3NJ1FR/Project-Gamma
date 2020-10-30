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

	unsigned int numberRessourceNeeded;
	int *ressourceIDNeeded;
	int *ressourceQuantityNeeded;

	unsigned int numberRessourceProduced;
	int *ressourceIDProduced;
	int *ressourceQuantityProduced;

public :

	Buildings();
	~Buildings();

	void SetName(const std::string _name);
	void SetDescription(std::string _description);

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
	void SetRessourceIDNeeded(int *_resssourceID, const unsigned char &_numberRessources = 1);
	void SetRessourceQuantityNeeded(int *_resssourceQuantity = 0);
	void SetRessourceIDProduced(int *_resssourceID, const unsigned char &_numberRessources = 1);
	void SetRessourceQuantityProduced(int *_resssourceQuantity = 0);

	inline std::string GetName() const { return name; };
	inline std::string GetDescription() const { return description; };
	inline sf::Sprite GetSprite() const { return sprite; };
	inline sf::Sprite GetIcon() const { return icon; };
	inline sf::Vector2i GetSize() const { return size; };
	inline int GetConstructionCost() const { return moneyCostToBuild; };
	inline float GetConstructionTimeCost() const { return timeToBuild; };
	inline float GetProductionTimeCost() const { return timeToProduce; };
	inline float GetDepositingTimeCost() const { return timeToDeposeRessources; };
	inline float GetPickupingTimeCost() const { return timeToPickUpRessources; };
	inline unsigned int GetRessourceNumberNeeded() const { return numberRessourceNeeded; };
	inline unsigned int GetRessourceNumberProduced() const { return numberRessourceProduced; };
	
	int GetRessourceIDNeeded(const unsigned char &_value = 1) const;
	int GetRessourceQuantityNeeded(const unsigned char &_value = 1) const;
	int GetRessourceIDProduced(const unsigned char &_value = 1) const;
	int GetRessourceQuantityProduced(const unsigned char &_value = 1) const;
};




#endif // !BUILDINGS__H
