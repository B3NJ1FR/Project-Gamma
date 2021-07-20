#pragma once

#include "Common.hpp"
#include "LinkedList.h"
#include "EnumInternalState.h"

struct AnnualCostsNRevenues
{
	int m_numberOfBuilding;

	int m_quantityProduced;
	int m_comparisonWithLastYear;

	bool m_isCanBeSold;
	int m_previousMerchantPrice;
};
typedef struct AnnualCostsNRevenues sAnnualCostsNRevenues;

// std::map< [YEAR], [List of resources present at this year]>
typedef std::map<int, LinkedListClass::sLinkedList*> AnnualCostsNRevenuesMapData;

//
//
// WARNING : ALL THE METHODS ONLY WORK FOR THE YEAR 0 -> TO CHANGE
//
//

// CNR = Costs And Revenues
class ListOfAnnualCostsNRevenues : LinkedListClass
{
private:
	AnnualCostsNRevenuesMapData m_listOfAnnualCNRData;

	sf::Sprite m_papyrusBackground;

	InternalState internalState;

	//sf::Text** m_textsData;
	//sf::Text* m_textsCategoriesTitles;
	sf::Text *m_textPapyrusTitles;
	sf::Text m_textYear;
	sf::Font m_font;

	std::string m_stringTextYear;

	// Constructor
	ListOfAnnualCostsNRevenues();

public:
	// Destructor
	~ListOfAnnualCostsNRevenues();

	// Singleton
	static ListOfAnnualCostsNRevenues* GetSingleton();

	// Getters
	inline AnnualCostsNRevenuesMapData GetMapData() const { return m_listOfAnnualCNRData; }

	// Setters
	/*void CreateNewYearInDataMap(unsigned int _yearNumber);
	void AddResourceToYear(TypesOfRessources _resource, unsigned int _yearNumber);
	void AddResourceDataToYear(sAnnualResourceData _resourceData, unsigned int _yearNumber);
	void SetResourceNumberOfBuilding(unsigned int _yearNumber, TypesOfRessources _resource, int _numberOfBuilding);
	void SetResourceQuantityProduced(unsigned int _yearNumber, TypesOfRessources _resource, int _quantityProduced);
	void SetResourceComparisonWithLastYear(unsigned int _yearNumber, TypesOfRessources _resource, int _comparisonWithLastYear);
	void SetResourceIsCanBeSold(unsigned int _yearNumber, TypesOfRessources _resource, bool _isCanBeSold);
	void SetResourcePreviousMerchantPrice(unsigned int _yearNumber, TypesOfRessources _resource, int _previousMerchantPrice);*/

	// Methods
	//bool IsResourceExistInLinkedList(LinkedListClass::sLinkedList* _list, TypesOfRessources _resource);

	void Update();
	void Display(sf::RenderWindow& _window, const sf::Vector2i& _screenResolution);

	// Save & Load
	void SavingDataForFile(std::ofstream* _file);
	void LoadingDataFromFile(std::ifstream* _file);
};