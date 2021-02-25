#pragma once

#include <stdarg.h>
#include "Ressources.h"

class TypeMapStringResources : public std::map<std::string, Ressources*> {};

class Storage
{
private:
	std::string m_storageName;
	TypeMapStringResources m_mapOfResources;

public:
	// Constructor & Destructor
	Storage();
	Storage(int _quantityOfRessources, enum TypesOfRessources _ressources, ...);
	~Storage();

	// Accessors
	Ressources* operator[] (int _index);
	friend std::ostream& operator<< (std::ostream& _ostream, const Storage& _storageToDisplay);
	
	// Setters
	inline void SetName(std::string _storageName) { m_storageName = _storageName; }
	void AddNewResourceToStorage(std::string _ressourceName);
	void RemoveResourceOfStorage(std::string _ressourceName);
	void AddOrSubtractResource(std::string _ressourceName, int _quantity);
	
	// Getters
	inline int GetNumberOfResources() const { return m_mapOfResources.size(); }
	inline std::string GetName() const { return m_storageName; }
	inline TypeMapStringResources GetMapOfResources() const { return m_mapOfResources; }
	int GetResourceQuantity(std::string _ressourceName);

	// Methods
	static bool TransferOfResource(Storage *_source, Storage *_destination, std::string _ressourceName, unsigned int _quantity);
	static bool TransferOfTheWholeResource(Storage* _source, Storage* _destination, std::string _ressourceName);
	bool IsResourceExistHere(std::string _ressourceName);
	bool IsResourceExistHere(enum TypesOfRessources _ressourceName);
	bool IsStorageEmpty();
};

