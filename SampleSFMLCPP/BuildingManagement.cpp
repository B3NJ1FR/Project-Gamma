#include "BuildingManagement.h"
#include "Buildings.h"
#include "BuildingDestruction.h"


BuildingManagement::BuildingManagement()
{
	m_numberOfBuilding = RESET;

	m_buildings = nullptr;
	m_stall = nullptr;
	m_buildingsNameTexts = nullptr;

	InitBuildingsFromFile();
	InitBuildingsSpritesFromFile();

	if (m_buildings != nullptr)
	{
		m_stall = new Stalls(&m_buildings[TypeOfBuilding::BUILDING_STALL]);

		m_vines.InitialisationVines(&m_buildings[TypeOfBuilding::BUILDING_VINES]);
		m_stompingVats.InitialisationSpeBuilding(&m_buildings[TypeOfBuilding::BUILDING_GRAPE_STOMPING_VATS]);
		m_winePress.InitialisationSpeBuilding(&m_buildings[TypeOfBuilding::BUILDING_WINE_PRESS]);
		m_wineStorehouse.InitialisationSpeBuilding(&m_buildings[TypeOfBuilding::BUILDING_WINE_STOREHOUSE]);
		m_storehouse.InitialisationStorehouse(&m_buildings[TypeOfBuilding::BUILDING_STOREHOUSE]);

		m_villa = Villa::GetSingleton();
		m_villa->Initialisation(&m_buildings[TypeOfBuilding::BUILDING_VILLA]);
	}

	BuildingDestruction::GetSingleton()->InitialisationBuildingsToDestroy();

	std::cout << "----- BuildingManagement Initialised ! -----\n\n\n";
}

BuildingManagement::~BuildingManagement()
{
	if (m_buildings != nullptr)
	{
		delete[] m_buildings;
	}

	if (m_stall != nullptr)
	{
		delete m_stall;
	}

	if (m_buildingsNameTexts != nullptr)
	{
		delete[] m_buildingsNameTexts;
	}
}



void BuildingManagement::InitBuildingsFromFile()
{
	std::ifstream buildingsFile("Data/Configurations/Buildings.data", std::ios::in);

	std::string temporaryString;
	temporaryString.erase();
	int temporaryNumber(RESET);
	float temporaryFloatingNumber(RESET);

	// Security to be sure to add elements to an existing array, and not into the void
	bool isAllocated = false;


	if (!buildingsFile.is_open())
	{
		std::cout << "Error accessing Buildings.data file" << std::endl;

		exit(EXIT_FAILURE);
	}

	while (!buildingsFile.eof())
	{
		buildingsFile >> temporaryString;

		if (temporaryString == "NUMBER_OF_BUILDINGS")
		{
			buildingsFile >> m_numberOfBuilding;
			std::cout << "Number of buildings : " << m_numberOfBuilding << std::endl << std::endl;

			// Buildings array allocation that will conserve all the buildings data
			m_buildings = new Buildings[m_numberOfBuilding];

			isAllocated = true;
		}

		if (temporaryString == "BUILDING" && isAllocated == true)
		{
			buildingsFile >> temporaryNumber;
			unsigned short buildingID = temporaryNumber;

			// Security to avoid to add elements into the void
			if (buildingID <= (m_numberOfBuilding - 1))
			{
				buildingsFile >> temporaryString;

				if (temporaryString.find("_") != -1)
				{
					temporaryString.replace(temporaryString.find("_"), 1, "\n", 1);
				}

				if (temporaryString.find("\n") != -1)
				{
					int firstWordLengh = temporaryString.find("\n");
					int secondWordLengh = temporaryString.length() - firstWordLengh - 1;

					if (firstWordLengh == secondWordLengh)
					{
						// This two words are equal
						//std::cout << "CES DEUX MOTS SONT EGAUX !!!\n"; 
					}
					else if (firstWordLengh < secondWordLengh)
					{
						int differenceBetweenWords = secondWordLengh - firstWordLengh;

						std::string newString;
						newString.erase();

						for (int i = 0; i < ((differenceBetweenWords / 2) - 1); i++)
						{
							newString.append("\t");
						}

						if (differenceBetweenWords % 2 == 1
							&& differenceBetweenWords / 2 != 0)
						{
							newString.append(" ");
						}

						temporaryString = newString + temporaryString;

						//std::cout << temporaryString << std::endl;
					}
					else if (firstWordLengh > secondWordLengh)
					{
						int differenceBetweenWords = firstWordLengh - secondWordLengh;

						std::string newString;
						newString.erase();

						char buffer[255];
						char buffer2[255];

						temporaryString.copy(buffer, firstWordLengh + 1, 0);
						buffer[firstWordLengh + 1] = '\0';
						temporaryString.copy(buffer2, secondWordLengh, temporaryString.find("\n") + 1);
						buffer2[secondWordLengh] = '\0';

						for (auto i = 0; i < differenceBetweenWords / 2 - 1; i++)
						{
							newString.append("\t");
						}

						if (differenceBetweenWords % 2 == 1)
						{
							newString.append(" ");
						}

						temporaryString.erase();
						temporaryString = buffer + newString + buffer2;

						//std::cout << temporaryString << std::endl;
					}
				}

				m_buildings[buildingID].SetName(temporaryString);


				// Pickup the ID and the name of the building
				std::cout << "Building " << buildingID << " - " << temporaryString << " - ";

				while (temporaryString != "WORKERS")
				{
					buildingsFile >> temporaryString;

					if (temporaryString == "DESCRIPTION")
					{
						buildingsFile >> temporaryString;

						while (temporaryString.find("_") != -1)
						{
							temporaryString.replace(temporaryString.find("_"), 1, " ", 1);
						}

						m_buildings[buildingID].SetDescription(temporaryString);
					}
					else if (temporaryString == "SIZE")
					{
						sf::Vector2i temporarySize = { RESET, RESET };

						buildingsFile >> temporarySize.x >> temporarySize.y;
						m_buildings[buildingID].SetSizeCaracteristics(temporarySize);

						std::cout << temporarySize.x << "x" << temporarySize.y << std::endl;

					}
					else if (temporaryString == "ENTRANCE")
					{
						sf::Vector2i temporaryEntrance = { RESET, RESET };

						buildingsFile >> temporaryEntrance.x >> temporaryEntrance.y;
						m_buildings[buildingID].SetEntranceCaracteristics(temporaryEntrance);

						std::cout << "\tEntrance :\t" << temporaryEntrance.x << " " << temporaryEntrance.y << std::endl;
					}
					else if (temporaryString == "EXIT")
					{
						sf::Vector2i temporaryExit = { RESET, RESET };

						buildingsFile >> temporaryExit.x >> temporaryExit.y;
						m_buildings[buildingID].SetExitCaracteristics(temporaryExit);

						std::cout << "\tExit :\t\t" << temporaryExit.x << " " << temporaryExit.y << std::endl;
					}
					else if (temporaryString == "SPRITES")
					{
						buildingsFile >> temporaryString;
						if (m_vBuildingsSpritesFiles == nullptr)
						{
							m_vBuildingsSpritesFiles = new std::vector<std::string>();
						}

						m_vBuildingsSpritesFiles->push_back(temporaryString);

						//m_buildings[buildingID].SetSprite(temporaryString);
					}
					else if (temporaryString == "ICON")
					{
						buildingsFile >> temporaryString;
						m_buildings[buildingID].SetIcon(temporaryString);
					}
					else if (temporaryString == "MONEY")
					{
						buildingsFile >> temporaryNumber;
						m_buildings[buildingID].SetConstructionMoneyCost(temporaryNumber);
					}
					else if (temporaryString == "RESSOURCES")
					{
						int counterRessources(RESET);
						buildingsFile >> counterRessources;
						std::cout << "Ress : " << counterRessources;

						if (counterRessources == 0)
						{
							buildingsFile >> temporaryNumber;
							buildingsFile >> temporaryNumber;
						}
						else if (counterRessources == 1)
						{
							int* resssourceID = new int;
							int* resssourceQuantity = new int;


							buildingsFile >> temporaryNumber;
							*resssourceID = temporaryNumber;

							buildingsFile >> temporaryNumber;
							*resssourceQuantity = temporaryNumber;


							m_buildings[buildingID].SetRessourceIDNeeded(resssourceID, counterRessources);

							std::cout << " Test : " << *resssourceID << ' ' << counterRessources<< std::endl;
 
							m_buildings[buildingID].SetRessourceQuantityNeeded(resssourceQuantity);
						}
						else
						{
							int* resssourceID = new int[counterRessources];
							int* resssourceQuantity = new int[counterRessources];

							for (int i = 0; i < counterRessources; i++)
							{
								buildingsFile >> temporaryNumber;
								resssourceID[i] = temporaryNumber;

								buildingsFile >> temporaryNumber;
								resssourceQuantity[i] = temporaryNumber;
							}

							m_buildings[buildingID].SetRessourceIDNeeded(resssourceID, counterRessources);
							std::cout << " Test : " << *resssourceID << ' ' << counterRessources << std::endl;
							m_buildings[buildingID].SetRessourceQuantityNeeded(resssourceQuantity);
						}


						buildingsFile >> counterRessources;
						std::cout << " et : " << counterRessources << std::endl;

						if (counterRessources == 0)
						{
							buildingsFile >> temporaryNumber;
							buildingsFile >> temporaryNumber;
						}
						else if (counterRessources == 1)
						{
							int* resssourceID = new int;
							int* resssourceQuantity = new int;


							buildingsFile >> temporaryNumber;
							*resssourceID = temporaryNumber;

							buildingsFile >> temporaryNumber;
							*resssourceQuantity = temporaryNumber;

							m_buildings[buildingID].SetRessourceIDProduced(resssourceID, counterRessources);

							m_buildings[buildingID].SetRessourceQuantityProduced(resssourceQuantity);
						}
						else
						{
							int* resssourceID = new int[counterRessources];
							int* resssourceQuantity = new int[counterRessources];

							for (int i = 0; i < counterRessources; i++)
							{
								buildingsFile >> temporaryNumber;
								resssourceID[i] = temporaryNumber;

								buildingsFile >> temporaryNumber;
								resssourceQuantity[i] = temporaryNumber;
							}

							m_buildings[buildingID].SetRessourceIDProduced(resssourceID, counterRessources);

							m_buildings[buildingID].SetRessourceQuantityProduced(resssourceQuantity);
						}

					}
					else if (temporaryString == "TIME")
					{
						buildingsFile >> temporaryFloatingNumber;
						m_buildings[buildingID].SetConstructionTimeCost(temporaryFloatingNumber);

						buildingsFile >> temporaryFloatingNumber;
						m_buildings[buildingID].SetProductionTimeCost(temporaryFloatingNumber);

						buildingsFile >> temporaryFloatingNumber;
						m_buildings[buildingID].SetPickupingTimeCost(temporaryFloatingNumber);

						buildingsFile >> temporaryFloatingNumber;
						m_buildings[buildingID].SetDepositingTimeCost(temporaryFloatingNumber);
					}
					else if (temporaryString == "WORKERS")
					{
						buildingsFile >> temporaryNumber;
						m_buildings[buildingID].SetNumberWorkersNeeded(temporaryNumber);
					}
				}

				std::cout << std::endl << std::endl << std::endl;
			}
			else
			{
				buildingsFile >> temporaryString;
				std::cout << "\n\n\nError during reading, the id building's : \"" << temporaryNumber << " - " << temporaryString << "\" is higher than number of buildings write before !\n\n\n";
			}
		}
	}

	std::cout << "\n\n\tBuildings Initialisation succeed !\n\n\n";

	buildingsFile.close();
}


void BuildingManagement::InitBuildingsSpritesFromFile()
{
	if (m_vBuildingsSpritesFiles != nullptr)
	{
		int currentBuildingID = 0;

		for (auto currentFileName : *m_vBuildingsSpritesFiles)
		{
			sf::Vector2i buildingSize = m_buildings[currentBuildingID].GetSize();
			std::vector<sf::Sprite> spriteVector;

			std::ifstream spritesFile(currentFileName, std::ios::in);

			std::string temporaryString;
			temporaryString.erase();
			int temporaryNumber = RESET;

			if (!spritesFile.is_open())
			{
				std::cout << "Error accessing Sprites.data file" << std::endl;

				exit(EXIT_FAILURE);
			}

			while (!spritesFile.eof())
			{
				spritesFile >> temporaryString;

				if (temporaryString == "ID")
				{
					DoubleVectorsOfSpritesID spritesIDVector;

					for (int z = 0; z < (int)FloorsInBuildingSprites::FIBS_ENUM_SIZE; z++)
					{
						std::vector <std::vector<int>> vectorOfIDVector;

						for (int y = 0; y < buildingSize.y; y++)
						{
							std::vector<int> idVector;
							for (int x = 0; x < buildingSize.x; x++)
							{
								std::cout << "Here\n";
								spritesFile >> temporaryString;

								if (temporaryString == "END_ID") break;

								idVector.push_back(std::stoi(temporaryString));
							}
							vectorOfIDVector.push_back(idVector);
						}

						spritesIDVector.push_back(vectorOfIDVector);
					}

					m_buildings[currentBuildingID].SetVecBuildingsSpritesID(spritesIDVector);
				}

				if (temporaryString == "SPRITE")
				{
					sf::Sprite newSprite;
					
					spritesFile >> temporaryNumber;
					spritesFile >> temporaryString;

					if (temporaryString != "VOID")
					{
						newSprite = LoadSprite(temporaryString, temporaryNumber);
					}

					spriteVector.push_back(newSprite);
				}
			}

			spritesFile.close();

			std::cout << currentFileName << std::endl;
			m_vBuildingsSprites.push_back(spriteVector);

			currentBuildingID += 1;
		}

		m_vBuildingsSpritesFiles->clear();
		delete m_vBuildingsSpritesFiles;
		m_vBuildingsSpritesFiles = nullptr;

	}
}

sf::Sprite BuildingManagement::GetSpriteFromBuildID(int _buildingID, int _index) const
{ 
	if (m_vBuildingsSprites.size() != 0 && _buildingID < m_vBuildingsSprites.size())
	{
		if (m_vBuildingsSprites[_buildingID].size() != 0 && _index - 1 >= 0 && _index - 1 < m_vBuildingsSprites[_buildingID].size())
		{
			return m_vBuildingsSprites[_buildingID][_index - 1];
		}
	}

	//std::cout << _buildingID << " " << _index "\n";
	return sf::Sprite();
}

void BuildingManagement::UpdateBuildingManagement(Map* _map)
{
	BuildingDestruction::GetSingleton()->UpdateBuildingDestruction(this, _map);
}