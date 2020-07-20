#ifndef RESSOURCES__H
#define RESSOURCES__H

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
	std::string name;
	int quantityOwned;

	sf::Sprite sprite;

public :
	Ressources();
	~Ressources();

	void SetSprite(const std::string _spriteFileName, int _spriteOriginPosition = 0);
	void SetName(const std::string _name);
	void SetInitialQuantityOwned(int _quantityOwned = 0);
	void AddQuantityOwned(int _quantityToAdd);
	void SubtractQuantityOwned(int _quantityToSubtract);
	
	sf::Sprite GetSprite();
	std::string GetName();
	int GetQuantityOwned();
};

#endif // !RESSOURCES__H
