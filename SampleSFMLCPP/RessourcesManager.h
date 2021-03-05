#pragma once

#include "Ressources.h"
#include "Storage.h"

// Can obtain every ressources present in the game through
// the differents storages and buildings
class RessourcesManager
{
private :
	std::vector<Storage*> m_vectorStorages;
	std::vector<Ressources*> m_vectorRessources;
	
	// Constructor
	RessourcesManager();

public :
	// Destructor
	~RessourcesManager();

	// Initialisator
	void Initialisation();
	void RessourcesInitialisation();

	// Setters
	void AddNewStorage(Storage* _newStorage);
	void RemoveStorage(Storage* _oldStorage);

	// Getters
	static RessourcesManager* GetSingleton();
	unsigned int GetResourceQuantity(std::string _ressourceName, std::string _specificBuildingName = std::string(""));
	sf::Sprite GetResourceSprite(TypesOfRessources _ressourceName);
	sf::Sprite GetResourceSprite(std::string _ressourceName);

	// Methods
	void DisplayAllStoragesContent();
	void DisplayBuildingsStoragesContent();
	void DisplayWorkersStoragesContent();
	void ClearAllStorages();
};

