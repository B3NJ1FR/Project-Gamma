#include "ListOfAnnualProductions.h"
#include "RessourcesManager.h"
#include "Fonts.h"

ListOfAnnualProductions::ListOfAnnualProductions()
{
	m_listOfAnnualResourcesData.clear();

	m_time = TimeManagement::GetSingleton();

	m_papyrusBackground = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Production_Summary/sellingWindow_background.png", 1);

	m_font = Fonts::GetSingleton()->GetCharlemagneFont();
	LoadTextString(&m_textPapyrusTitle, "Production Summary", &m_font, 35, sf::Color::Black, 1);

	// TESTS
	CreateNewYearInDataMap(0);
	AddResourceToYear(TypesOfRessources::BUNCH_OF_GRAPE, 0);
	AddResourceToYear(TypesOfRessources::GRAPES_MUST, 0);
	AddResourceToYear(TypesOfRessources::GRAPE_JUICE, 0);
	AddResourceToYear(TypesOfRessources::GRAPE_MARC, 0);
	AddResourceToYear(TypesOfRessources::DOLIUM, 0);
	AddResourceToYear(TypesOfRessources::AMPHORA_OF_WINE, 0);

	internalState = InternalState::STATE_INIT;
}

ListOfAnnualProductions::~ListOfAnnualProductions()
{
	m_listOfAnnualResourcesData.clear();
}

ListOfAnnualProductions* ListOfAnnualProductions::GetSingleton()
{
	static ListOfAnnualProductions uniqueInstance;
	return &uniqueInstance;
}


void ListOfAnnualProductions::CreateNewYearInDataMap(unsigned int _yearNumber)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);
	if (it == m_listOfAnnualResourcesData.end())
	{
		m_listOfAnnualResourcesData[_yearNumber] = LinkedListInitialisation();
	}
	else
	{
		std::cout << "[ListOfAnnualProductions] - Error when creating new year in Data Map because the year " << _yearNumber << " already exist";
	}
}

void ListOfAnnualProductions::AddResourceToYear(TypesOfRessources _resource, unsigned int _yearNumber)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it == m_listOfAnnualResourcesData.end())
	{
		std::cout << "[ListOfAnnualProductions] - Error when adding resources to year because the year : " << _yearNumber << " doesn't exist in the data map, but has now been created.";

		// We create the year in the Data Map
		CreateNewYearInDataMap(_yearNumber);
	}

	// We verify that the resource doesn't already exist in the list
	if (!IsResourceExistInLinkedList(m_listOfAnnualResourcesData[_yearNumber], _resource))
	{
		LinkedListClass::sElement* newResource = new LinkedListClass::sElement;
		newResource->data = new sAnnualResourceData;

		((sAnnualResourceData*)newResource->data)->m_resource = _resource;

		((sAnnualResourceData*)newResource->data)->m_numberOfBuilding = 0;

		((sAnnualResourceData*)newResource->data)->m_quantityProduced = 0;
		((sAnnualResourceData*)newResource->data)->m_comparisonWithLastYear = 0;

		((sAnnualResourceData*)newResource->data)->m_isCanBeSold = false;
		((sAnnualResourceData*)newResource->data)->m_previousMerchantPrice = 0;

		newResource->status = ELEMENT_ACTIVE;

		// Add this new resource at the end of the list of resources produced this year
		AddElementToLinkedList(m_listOfAnnualResourcesData[_yearNumber], newResource, -1);
	}
}

void ListOfAnnualProductions::AddResourceDataToYear(sAnnualResourceData _resourceData, unsigned int _yearNumber)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it != m_listOfAnnualResourcesData.end())
	{
		if (m_listOfAnnualResourcesData[_yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[_yearNumber]->first != nullptr)
			{
				for (LinkedListClass::sElement* currentElement = m_listOfAnnualResourcesData[_yearNumber]->first; currentElement != nullptr; currentElement = currentElement->next)
				{
					if (((sAnnualResourceData*)currentElement->data)->m_resource == _resourceData.m_resource)
					{
						((sAnnualResourceData*)currentElement->data)->m_numberOfBuilding = _resourceData.m_numberOfBuilding;

						((sAnnualResourceData*)currentElement->data)->m_quantityProduced = _resourceData.m_quantityProduced;
						((sAnnualResourceData*)currentElement->data)->m_comparisonWithLastYear = _resourceData.m_comparisonWithLastYear;

						((sAnnualResourceData*)currentElement->data)->m_isCanBeSold = _resourceData.m_isCanBeSold;
						((sAnnualResourceData*)currentElement->data)->m_previousMerchantPrice = _resourceData.m_previousMerchantPrice;
					}
				}
			}
		}
	}
}

void ListOfAnnualProductions::SetResourceNumberOfBuilding(unsigned int _yearNumber, TypesOfRessources _resource, int _numberOfBuilding)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it != m_listOfAnnualResourcesData.end())
	{
		if (m_listOfAnnualResourcesData[_yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[_yearNumber]->first != nullptr)
			{
				for (LinkedListClass::sElement* currentElement = m_listOfAnnualResourcesData[_yearNumber]->first; currentElement != nullptr; currentElement = currentElement->next)
				{
					if (((sAnnualResourceData*)currentElement->data)->m_resource == _resource)
					{
						((sAnnualResourceData*)currentElement->data)->m_numberOfBuilding = _numberOfBuilding;
					}
				}
			}
		}
	}
}

void ListOfAnnualProductions::SetResourceQuantityProduced(unsigned int _yearNumber, TypesOfRessources _resource, int _quantityProduced)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it != m_listOfAnnualResourcesData.end())
	{
		if (m_listOfAnnualResourcesData[_yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[_yearNumber]->first != nullptr)
			{
				for (LinkedListClass::sElement* currentElement = m_listOfAnnualResourcesData[_yearNumber]->first; currentElement != nullptr; currentElement = currentElement->next)
				{
					if (((sAnnualResourceData*)currentElement->data)->m_resource == _resource)
					{
						((sAnnualResourceData*)currentElement->data)->m_quantityProduced = _quantityProduced;
					}
				}
			}
		}
	}
}

void ListOfAnnualProductions::SetResourceComparisonWithLastYear(unsigned int _yearNumber, TypesOfRessources _resource, int _comparisonWithLastYear)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it != m_listOfAnnualResourcesData.end())
	{
		if (m_listOfAnnualResourcesData[_yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[_yearNumber]->first != nullptr)
			{
				for (LinkedListClass::sElement* currentElement = m_listOfAnnualResourcesData[_yearNumber]->first; currentElement != nullptr; currentElement = currentElement->next)
				{
					if (((sAnnualResourceData*)currentElement->data)->m_resource == _resource)
					{
						((sAnnualResourceData*)currentElement->data)->m_comparisonWithLastYear = _comparisonWithLastYear;
					}
				}
			}
		}
	}
}

void ListOfAnnualProductions::SetResourceIsCanBeSold(unsigned int _yearNumber, TypesOfRessources _resource, bool _isCanBeSold)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it != m_listOfAnnualResourcesData.end())
	{
		if (m_listOfAnnualResourcesData[_yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[_yearNumber]->first != nullptr)
			{
				for (LinkedListClass::sElement* currentElement = m_listOfAnnualResourcesData[_yearNumber]->first; currentElement != nullptr; currentElement = currentElement->next)
				{
					if (((sAnnualResourceData*)currentElement->data)->m_resource == _resource)
					{
						((sAnnualResourceData*)currentElement->data)->m_isCanBeSold = _isCanBeSold;
					}
				}
			}
		}
	}
}

void ListOfAnnualProductions::SetResourcePreviousMerchantPrice(unsigned int _yearNumber, TypesOfRessources _resource, int _previousMerchantPrice)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it != m_listOfAnnualResourcesData.end())
	{
		if (m_listOfAnnualResourcesData[_yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[_yearNumber]->first != nullptr)
			{
				for (LinkedListClass::sElement* currentElement = m_listOfAnnualResourcesData[_yearNumber]->first; currentElement != nullptr; currentElement = currentElement->next)
				{
					if (((sAnnualResourceData*)currentElement->data)->m_resource == _resource)
					{
						((sAnnualResourceData*)currentElement->data)->m_previousMerchantPrice = _previousMerchantPrice;
					}
				}
			}
		}
	}
}


bool ListOfAnnualProductions::IsResourceExistInLinkedList(LinkedListClass::sLinkedList *_list, TypesOfRessources _resource)
{
	if (_list != nullptr)
	{
		if (_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = _list->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				if (((sAnnualResourceData*)currentElement->data)->m_resource == _resource) return true;
			}
		}
	}

	return false;
}


void ListOfAnnualProductions::Update()
{
	int yearNumber = 0;

	switch (internalState)
	{
	case InternalState::STATE_INIT:
		if (m_listOfAnnualResourcesData[yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[yearNumber]->first != nullptr)
			{
				// Allocation of the text array
				m_textsData = new sf::Text* [m_listOfAnnualResourcesData[yearNumber]->size];

				for (int i = 0; i < m_listOfAnnualResourcesData[yearNumber]->size; i++)
				{
					m_textsData[i] = new sf::Text[5];
				}

				LinkedListClass::sElement* currentElement;
				int counter;

				for (currentElement = m_listOfAnnualResourcesData[yearNumber]->first, counter = 0; 
					currentElement != nullptr;
					currentElement = currentElement->next, counter++)
				{
					sAnnualResourceData* curResource = ((sAnnualResourceData*)currentElement->data);

					for (int i = 0; i < 5; i++)
					{
						LoadTextString(&m_textsData[counter][i], "", &m_font, 25, sf::Color::Black);
					}

					UpdateDynamicsTexts(&m_textsData[counter][0], curResource->m_numberOfBuilding);
					UpdateDynamicsTexts(&m_textsData[counter][1], curResource->m_quantityProduced);
					UpdateDynamicsTexts(&m_textsData[counter][2], curResource->m_comparisonWithLastYear);
					UpdateDynamicsTexts(&m_textsData[counter][3], curResource->m_isCanBeSold);
					UpdateDynamicsTexts(&m_textsData[counter][4], curResource->m_previousMerchantPrice);
				}
			}
		}
		internalState = InternalState::STATE_UPDATE;
		break;
	case InternalState::STATE_UPDATE:
		break;
	case InternalState::STATE_EXIT:
		// Désallouer le tableau de texts
		break;
	case InternalState::STATE_WAITING:
		break;
	default:
		break;
	}
}

void ListOfAnnualProductions::Display(sf::RenderWindow& _window, const sf::Vector2i& _screenResolution)
{
	if (internalState != InternalState::STATE_UPDATE) return;
	
	// Display of the background
	BlitSprite(m_papyrusBackground, (float)(_screenResolution.x / 2), (float)(_screenResolution.y / 2), 0, _window);


	// Display papyrus title
	sf::Vector2f screenCenter = sf::Vector2f(_screenResolution.x / 2, _screenResolution.y / 2);
	BlitString(m_textPapyrusTitle, screenCenter.x, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + PAPYRUS_BG_OFFSET_TOP - 40.0f, _window);

	int yearNumber = 0;

	// Display of the resources data
	if (m_listOfAnnualResourcesData[yearNumber] != nullptr)
	{
		if (m_listOfAnnualResourcesData[yearNumber]->first != nullptr)
		{
			LinkedListClass::sElement* currentElement;
			int counter;
			
			float backgroundHeightAvailable = m_papyrusBackground.getGlobalBounds().height - PAPYRUS_BG_OFFSET_TOP - PAPYRUS_BG_OFFSET_BOT;
			float spaceBetweenResources = backgroundHeightAvailable / m_listOfAnnualResourcesData[yearNumber]->size;

			for (currentElement = m_listOfAnnualResourcesData[yearNumber]->first, counter = 0;
				currentElement != nullptr;
				currentElement = currentElement->next, counter++)
			{
				sAnnualResourceData* curResource = ((sAnnualResourceData*)currentElement->data);
				float currentHeight = screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + PAPYRUS_BG_OFFSET_TOP + (counter * spaceBetweenResources);

				BlitSprite(RessourcesManager::GetSingleton()->GetResourceSprite(curResource->m_resource), screenCenter.x - 375.0f, currentHeight, _window);
			
				for (int i = 0; i < 5; i++)
				{
					BlitString(m_textsData[counter][i], screenCenter.x - 325.0f + (i + 1) * 125, currentHeight, _window);
				}
			}
		}
	}
}

void ListOfAnnualProductions::SavingDataForFile(std::ofstream* _file)
{

}

void ListOfAnnualProductions::LoadingDataFromFile(std::ifstream* _file)
{

}