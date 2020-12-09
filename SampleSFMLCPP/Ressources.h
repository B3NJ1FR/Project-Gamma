#pragma once

#include "Common.hpp"

enum TypesOfRessources
{
	GRAPE_VINE,
	BUNCH_OF_GRAPE,
	GRAPES_MUST,
	GRAPE_JUICE,
	PURE_GRAPE_JUICE,
	GRAPE_MARC,
	DOLIUM,
	AMPHORAS,
	AMPHORA_OF_WINE,
	TOOLS,
	WICKER_BASKET,
};

class Ressources
{
private :
	std::string m_name;
	int m_quantityOwned;

	sf::Sprite m_sprite;

public :
	// Constructor & Destructor
	Ressources();
	~Ressources();

	// Setters
	inline void SetSprite (const std::string _spriteFileName, const int _spriteOriginPosition = 0);
	inline void SetName (const std::string& _name);
	inline void SetInitialQuantityOwned (const int _quantityOwned = 0);
	inline void AddOrSubtractQuantityOwned(const int _quantityToAdd);
	
	// Getters
	inline sf::Sprite GetSprite() const { return m_sprite; };
	inline std::string GetName() const { return m_name; };
	inline int GetQuantityOwned() const { return m_quantityOwned; };
};
