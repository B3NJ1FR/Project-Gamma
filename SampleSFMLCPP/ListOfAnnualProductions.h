#pragma once

#include "Common.hpp"
#include "LinkedList.h"
#include "Ressources.h"
#include "TimeManagement.h"
#include "EnumInternalState.h"

#define PAPYRUS_BG_OFFSET_TOP 290.0f
#define PAPYRUS_BG_OFFSET_BOT 160.0f

struct AnnualResourceData
{
	TypesOfRessources m_resource;

	int m_numberOfBuilding;

	int m_quantityProduced;
	int m_comparisonWithLastYear;

	bool m_isCanBeSold;
	int m_previousMerchantPrice;
};
typedef struct AnnualResourceData sAnnualResourceData;

// std::map< [YEAR], [List of resources present at this year]>
typedef std::map<int, LinkedListClass::sLinkedList*> AnnualResourcesProducedMapData;

//
//
// WARNING : ALL THE METHODS ONLY WORK FOR THE YEAR 0 -> TO CHANGE
//
//

class ListOfAnnualProductions : LinkedListClass
{
private:
	AnnualResourcesProducedMapData m_listOfAnnualResourcesData;
	TimeManagement* m_time;

	InternalState internalState;

	sf::Sprite m_papyrusBackground;
	sf::Sprite m_greenCheck;
	sf::Sprite m_redCross;

	sf::Text **m_textsData;
	sf::Text *m_textsCategoriesTitles;
	sf::Text m_textPapyrusTitle;
	sf::Text m_textYear;
	sf::Font m_font;

	std::string m_stringTextYear;

	// Constructor
	ListOfAnnualProductions();

public:
	// Destructor
	~ListOfAnnualProductions();

	// Singleton
	static ListOfAnnualProductions* GetSingleton();

	// Getters
	inline AnnualResourcesProducedMapData GetMapData() const { return m_listOfAnnualResourcesData; }
	
	// Setters
	void CreateNewYearInDataMap(unsigned int _yearNumber);
	void AddResourceToYear(TypesOfRessources _resource, unsigned int _yearNumber);
	void AddResourceDataToYear(sAnnualResourceData _resourceData, unsigned int _yearNumber);
	void SetResourceNumberOfBuilding(unsigned int _yearNumber, TypesOfRessources _resource, int _numberOfBuilding);
	void SetResourceQuantityProduced(unsigned int _yearNumber, TypesOfRessources _resource, int _quantityProduced);
	void SetResourceComparisonWithLastYear(unsigned int _yearNumber, TypesOfRessources _resource, int _comparisonWithLastYear);
	void SetResourceIsCanBeSold(unsigned int _yearNumber, TypesOfRessources _resource, bool _isCanBeSold);
	void SetResourcePreviousMerchantPrice(unsigned int _yearNumber, TypesOfRessources _resource, int _previousMerchantPrice);

	// Methods
	bool IsResourceExistInLinkedList(LinkedListClass::sLinkedList* _list, TypesOfRessources _resource);

	void Update();
	void Display(sf::RenderWindow& _window, const sf::Vector2i& _screenResolution);

	// Save & Load
	void SavingDataForFile(std::ofstream* _file);
	void LoadingDataFromFile(std::ifstream* _file);
};
