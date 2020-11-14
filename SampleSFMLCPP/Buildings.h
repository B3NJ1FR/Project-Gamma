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

	sf::Vector2i m_size;
	sf::Vector2i m_enterPosition;
	sf::Vector2i m_exitPosition;

	sf::Sprite m_sprite;
	sf::Sprite m_icon;

	std::string m_name;
	std::string m_description;

	unsigned int m_id;

	int m_moneyCostToBuild;
	float m_timeToBuild;
	float m_timeToProduce;

	float m_timeToPickUpRessources;
	float m_timeToDeposeRessources;

	unsigned int m_numberRessourceNeeded;
	int *m_ressourceIDNeeded;
	int *m_ressourceQuantityNeeded;

	unsigned int m_numberRessourceProduced;
	int *m_ressourceIDProduced;
	int *m_ressourceQuantityProduced;

public :

	Buildings();
	~Buildings();

	void SetName(const std::string _name);
	void SetDescription(std::string _description);

	void SetSizeCaracteristics(const sf::Vector2i _buildingSize);
	void SetEntranceCaracteristics(const sf::Vector2i _enterPosition);
	void SetExitCaracteristics(const sf::Vector2i _exitPosition);

	void SetSprite(const std::string& _spriteFileName, int _spriteOriginPosition = 0);
	void SetIcon(const std::string& _iconFileName, int _iconOriginPosition = 0);
	void SetConstructionMoneyCost(const int _moneyCost);
	void SetConstructionTimeCost(const float _timeCost);
	void SetProductionTimeCost(const float _timeCost);
	void SetPickupingTimeCost(const float _timeCost = 0);
	void SetDepositingTimeCost(const float _timeCost = 0);
	void SetRessourceIDNeeded(int *_resssourceID, const unsigned char &_numberRessources = 1);
	void SetRessourceQuantityNeeded(int *_resssourceQuantity = 0);
	void SetRessourceIDProduced(int *_resssourceID, const unsigned char &_numberRessources = 1);
	void SetRessourceQuantityProduced(int *_resssourceQuantity = 0);

	inline std::string GetName() const { return m_name; };
	inline std::string GetDescription() const { return m_description; };
	inline sf::Sprite GetSprite() const { return m_sprite; };
	inline sf::Sprite GetIcon() const { return m_icon; };
	inline sf::Vector2i GetSize() const { return m_size; };
	inline int GetConstructionCost() const { return m_moneyCostToBuild; };
	inline float GetConstructionTimeCost() const { return m_timeToBuild; };
	inline float GetProductionTimeCost() const { return m_timeToProduce; };
	inline float GetDepositingTimeCost() const { return m_timeToDeposeRessources; };
	inline float GetPickupingTimeCost() const { return m_timeToPickUpRessources; };
	inline unsigned int GetRessourceNumberNeeded() const { return m_numberRessourceNeeded; };
	inline unsigned int GetRessourceNumberProduced() const { return m_numberRessourceProduced; };
	
	int GetRessourceIDNeeded(const unsigned char &_value = 1) const;
	int GetRessourceQuantityNeeded(const unsigned char &_value = 1) const;
	int GetRessourceIDProduced(const unsigned char &_value = 1) const;
	int GetRessourceQuantityProduced(const unsigned char &_value = 1) const;
};




#endif // !BUILDINGS__H
