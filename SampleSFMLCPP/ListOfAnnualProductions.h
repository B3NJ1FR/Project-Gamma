#pragma once

#include "Common.hpp"
#include "LinkedList.h"
#include "Ressources.h"
#include "TimeManagement.h"
#include "EnumInternalState.h"
#include "EnumVillaManagementStateMachine.h"

#define PAPYRUS_BG_OFFSET_TOP 290.0f
#define PAPYRUS_BG_OFFSET_BOT 160.0f
#define ARROW_CHANGE_YEAR_OFFSET_X 760.0f
#define ARROW_CHANGE_YEAR_OFFSET_Y 25.0f

struct AnnualResourceData
{
	TypesOfRessources m_resource;

	int m_numberOfBuilding;

	int m_quantityProduced;
	int m_comparisonWithLastYear;

	bool m_isCanBeSold;
	int m_previousMerchantPrice;

	AnnualResourceData() : m_resource(TypesOfRessources::GRAPE_VINE), m_numberOfBuilding(0), m_quantityProduced(0), m_comparisonWithLastYear(0), m_isCanBeSold(false), m_previousMerchantPrice(0) {};
	AnnualResourceData(TypesOfRessources _res, int _nbOfBuildings, int _qtyProduced, int _comparisonWLY, bool _canBeSold, int _prevMerchantPrice) : m_resource(_res), m_numberOfBuilding(_nbOfBuildings), m_quantityProduced(_qtyProduced), m_comparisonWithLastYear(_comparisonWLY), m_isCanBeSold(_canBeSold), m_previousMerchantPrice(_prevMerchantPrice) {};
};
typedef struct AnnualResourceData sAnnualResourceData;

// std::map< [YEAR], [List of resources present at this year]>
typedef std::map<int, LinkedListClass::sLinkedList*> AnnualResourcesProducedMapData;

class ListOfAnnualProductions : LinkedListClass
{
private:
	AnnualResourcesProducedMapData m_listOfAnnualResourcesData;
	TimeManagement* m_time;

	InternalState internalState;

	sf::Sprite m_papyrusBackground;
	sf::Sprite m_greenCheck;
	sf::Sprite m_redCross;
	sf::Sprite m_leftArrow;
	sf::Sprite m_rightArrow;
	sf::Sprite m_returnButton;

	sf::Text **m_textsData;
	int m_textsDataMaxSize;
	sf::Text *m_textsCategoriesTitles;
	sf::Text m_textPapyrusTitle;
	sf::Text m_textYear;
	sf::Text m_textArrowPrevYear;
	sf::Text m_textArrowNextYear;
	sf::Font m_font;

	std::string m_stringTextYear;

	bool m_isLeftArrowActived;
	bool m_isRightArrowActived;
	bool m_yearAsChanged;

	int m_currentYearDisplayed;

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
	void CreateNewResourceInList(LinkedListClass::LinkedList* _list, sAnnualResourceData _resourceData);
	void AddResourceToYear(TypesOfRessources _resource, unsigned int _yearNumber);
	void AddResourceDataToYear(sAnnualResourceData _resourceData, unsigned int _yearNumber);
	void AddResourceNumberOfBuilding(unsigned int _yearNumber, TypesOfRessources _resource, int _numberOfBuilding);
	void AddResourceQuantityProduced(unsigned int _yearNumber, TypesOfRessources _resource, int _quantityProduced);
	void SetResourceDataToYear(sAnnualResourceData _resourceData, unsigned int _yearNumber);
	void SetResourceNumberOfBuilding(unsigned int _yearNumber, TypesOfRessources _resource, int _numberOfBuilding);
	void SetResourceQuantityProduced(unsigned int _yearNumber, TypesOfRessources _resource, int _quantityProduced);
	void SetResourceComparisonWithLastYear(unsigned int _yearNumber, TypesOfRessources _resource, int _comparisonWithLastYear);
	void SetResourceIsCanBeSold(unsigned int _yearNumber, TypesOfRessources _resource, bool _isCanBeSold);
	void SetResourcePreviousMerchantPrice(unsigned int _yearNumber, TypesOfRessources _resource, int _previousMerchantPrice);

	// Methods
	bool IsResourceExistInLinkedList(LinkedListClass::sLinkedList* _list, TypesOfRessources _resource);
	void TransferNumberOfBuildingFromPreviousYear(int _yearNumber);
	void CalculateResourcesProductionDifference(int _yearNumber);
	void ClearTexts();
	void UpdateTextsContent();

	void Input(sf::Event _event, sf::RenderWindow& _window, const sf::Vector2i& _screenResolution);
	void Update(VillaManagementStateMachine* _internalStateMachine);
	void Display(sf::RenderWindow& _window, const sf::Vector2i& _screenResolution);

	// Save & Load
	void SavingDataForFile(std::ofstream* _file);
	void LoadingDataFromFile(std::ifstream* _file);
};

