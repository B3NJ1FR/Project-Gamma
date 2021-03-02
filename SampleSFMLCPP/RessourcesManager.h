#pragma once

#include "Ressources.h"
#include "Storage.h"

// Can obtain every ressources present in the game through
// the differents storages and buildings
class RessourcesManager
{
private :
	std::vector<Storage*> m_vectorStorages;

	// Constructor
	RessourcesManager();

public :
	// Destructor
	~RessourcesManager();

	// Setters
	void AddNewStorage(Storage* _newStorage);
	void RemoveStorage(Storage* _oldStorage);

	// Getters
	static RessourcesManager* GetSingleton();
	unsigned int GetResourceQuantity(std::string _ressourceName);

	// Methods
	void DisplayAllStoragesContent();
	void DisplayBuildingsStoragesContent();
	void DisplayWorkersStoragesContent();
	void ClearAllStorages();
};

