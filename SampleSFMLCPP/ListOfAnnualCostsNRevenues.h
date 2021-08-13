#pragma once

#include "Common.hpp"
#include "EnumInternalState.h"
#include "EnumListOfCostsNRevenues.h"
#include "EnumVillaManagementStateMachine.h"

#define ARROW_CHANGE_YEAR_OFFSET_X 750.0f
#define ARROW_CHANGE_YEAR_OFFSET_Y 25.0f


struct AnnualCostsNRevenues
{
	int m_purchasingWorkers = 0;
	int m_buildingConstruction = 0;

	int m_salesOfAmphorasOfWine = 0;

	int m_finalResult = 0;
};
typedef struct AnnualCostsNRevenues sAnnualCostsNRevenues;

// std::map< [YEAR], [Values of CNR]>
typedef std::map<int, sAnnualCostsNRevenues*> AnnualCostsNRevenuesMapData;

//
//
// WARNING : ALL THE METHODS ONLY WORK FOR THE YEAR 0 -> TO CHANGE
//
//

// CNR = Costs And Revenues
class ListOfAnnualCostsNRevenues
{
private:
	AnnualCostsNRevenuesMapData m_annualCNRDataMap;

	sf::Sprite m_papyrusBackground;
	sf::Sprite m_separationLine;
	sf::Sprite m_leftArrow;
	sf::Sprite m_rightArrow;

	InternalState internalState;

	sf::Text *m_textPapyrusTitles;
	sf::Text *m_textsCategoriesTitles;
	sf::Text *m_textsCategoriesValues;
	sf::Text m_textFinalCNRResult;
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
	ListOfAnnualCostsNRevenues();

public:
	// Destructor
	~ListOfAnnualCostsNRevenues();

	// Singleton
	static ListOfAnnualCostsNRevenues* GetSingleton();

	// Getters
	inline AnnualCostsNRevenuesMapData GetMapData() const { return m_annualCNRDataMap; }
	int GetFinalRevenues(int _yearNumber);

	// Setters
	void CreateNewYearInDataMap(unsigned int _yearNumber);
	void AddCNRValueToYear(EnumListOfCostsNRevenues _costOrRevenuesWanted, int _value, unsigned int _yearNumber);
	/*void AddResourceDataToYear(sAnnualResourceData _resourceData, unsigned int _yearNumber);
	void SetResourceNumberOfBuilding(unsigned int _yearNumber, TypesOfRessources _resource, int _numberOfBuilding);
	void SetResourceQuantityProduced(unsigned int _yearNumber, TypesOfRessources _resource, int _quantityProduced);
	void SetResourceComparisonWithLastYear(unsigned int _yearNumber, TypesOfRessources _resource, int _comparisonWithLastYear);
	void SetResourceIsCanBeSold(unsigned int _yearNumber, TypesOfRessources _resource, bool _isCanBeSold);
	void SetResourcePreviousMerchantPrice(unsigned int _yearNumber, TypesOfRessources _resource, int _previousMerchantPrice);*/

	// Methods
	void CalculateFinalRevenues(int _yearNumber);

	void Input(sf::Event _event, sf::RenderWindow& _window, const sf::Vector2i& _screenResolution);
	void UpdateTextsContent();
	void Update(VillaManagementStateMachine* _internalStateMachine);
	void Display(sf::RenderWindow& _window, const sf::Vector2i& _screenResolution);

	// Save & Load
	void SavingDataForFile(std::ofstream* _file);
	void LoadingDataFromFile(std::ifstream* _file);
};