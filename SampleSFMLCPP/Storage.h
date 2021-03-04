#pragma once

#include <stdarg.h>
#include "Ressources.h"

enum class ResourceData
{
	RESOURCE_NOT_DEFINED = -2,
	RESOURCE_NORMAL = -1,
	RESOURCE_NEEDED,
	RESOURCE_PRODUCED,
	RESOURCE_NEEDED_N_PRODUCED,
};

class TypeMapStringResources : public std::map<std::string, std::pair<Ressources*, ResourceData>> {};

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
	friend std::ostream& operator<< (std::ostream& _ostream, const Storage& _storageToDisplay);
	
	// Setters
	inline void SetName(std::string _storageName) { m_storageName = _storageName; }
	void AddNewResourceToStorage(std::string _ressourceName, ResourceData _data = ResourceData::RESOURCE_NORMAL);
	void AddNewResourceToStorage(std::string _ressourceName, Ressources _ressource, ResourceData _data = ResourceData::RESOURCE_NORMAL);
	void AddNewResourceToStorage(int _quantityOfRessources, enum TypesOfRessources _ressources, ...);
	void RemoveResourceOfStorage(std::string _ressourceName);
	void AddOrSubtractResource(std::string _ressourceName, int _quantity);
	void SetResourceData(std::string _ressourceName, ResourceData _data);
	
	// Getters
	inline int GetNumberOfResources() const { return (int)m_mapOfResources.size(); }
	inline std::string GetName() const { return m_storageName; }
	inline TypeMapStringResources GetMapOfResources() const { return m_mapOfResources; }
	int GetResourceQuantity(std::string _ressourceName);
	std::vector<Ressources*> GetResourceFromData(ResourceData _ressourceData);
	Ressources* GetResource(std::string _ressourceName);
	Ressources* GetResource(int _index);
	ResourceData GetResourceData(std::string _ressourceName);
	ResourceData GetResourceData(int _index);

	// Methods
	static bool TransferOfResource(Storage *_source, Storage *_destination, std::string _ressourceName, unsigned int _quantity);
	static bool TransferOfTheWholeResource(Storage* _source, Storage* _destination, std::string _ressourceName);
	bool IsResourceExistHere(std::string _ressourceName);
	bool IsResourceExistHere(enum TypesOfRessources _ressourceName);
	bool IsStorageEmpty();

	void SavingForFile(std::ofstream* _file);
	void LoadingFromFile(std::ifstream* _file);
};

