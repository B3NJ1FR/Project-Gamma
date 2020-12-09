#include "BuildingManagement.h"


BuildingManagement::BuildingManagement()
{
	numberOfBuilding = RESET;

	buildings = nullptr;
	stall = nullptr;
	buildingsNameTexts = nullptr;

	InitBuildingsFromFile();

	if (buildings != nullptr)
	{
		stall = new Stalls(&buildings[BUILDING_STALL]);

		vines.InitialisationVines(&buildings[BUILDING_VINES]);
		stompingVats.InitialisationSpeBuilding(&buildings[BUILDING_GRAPE_STOMPING_VATS]);
		winePress.InitialisationSpeBuilding(&buildings[BUILDING_WINE_PRESS]);
		wineStorehouse.InitialisationSpeBuilding(&buildings[BUILDING_WINE_STOREHOUSE]);
		storehouse.InitialisationStorehouse(&buildings[BUILDING_STOREHOUSE]);
	}

	std::cout << "----- BuildingManagement Initialised ! -----\n\n\n";
}

BuildingManagement::~BuildingManagement()
{
	if (buildings != nullptr)
	{
		delete[] buildings;
	}

	if (stall != nullptr)
	{
		delete stall;
	}

	if (buildingsNameTexts != nullptr)
	{
		delete[] buildingsNameTexts;
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
			buildingsFile >> numberOfBuilding;
			std::cout << "Number of buildings : " << numberOfBuilding << std::endl << std::endl;

			// Buildings array allocation that will conserve all the buildings data
			buildings = new Buildings[numberOfBuilding];

			isAllocated = true;
		}

		if (temporaryString == "BUILDING" && isAllocated == true)
		{
			buildingsFile >> temporaryNumber;
			unsigned short buildingID = temporaryNumber;

			// Security to avoid to add elements into the void
			if (buildingID <= (numberOfBuilding - 1))
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

						for (int i = 0; i < differenceBetweenWords / 2 - 1; i++)
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

						for (int i = 0; i < differenceBetweenWords / 2 - 1; i++)
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

				buildings[buildingID].SetName(temporaryString);


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

						buildings[buildingID].SetDescription(temporaryString);
					}
					else if (temporaryString == "SIZE")
					{
						sf::Vector2i temporarySize = { RESET, RESET };

						buildingsFile >> temporarySize.x >> temporarySize.y;
						buildings[buildingID].SetSizeCaracteristics(temporarySize);

						std::cout << temporarySize.x << "x" << temporarySize.y << std::endl;

					}
					else if (temporaryString == "ENTRANCE")
					{
						sf::Vector2i temporaryEntrance = { RESET, RESET };

						buildingsFile >> temporaryEntrance.x >> temporaryEntrance.y;
						buildings[buildingID].SetEntranceCaracteristics(temporaryEntrance);

						std::cout << "\tEntrance :\t" << temporaryEntrance.x << " " << temporaryEntrance.y << std::endl;
					}
					else if (temporaryString == "EXIT")
					{
						sf::Vector2i temporaryExit = { RESET, RESET };

						buildingsFile >> temporaryExit.x >> temporaryExit.y;
						buildings[buildingID].SetExitCaracteristics(temporaryExit);

						std::cout << "\tExit :\t\t" << temporaryExit.x << " " << temporaryExit.y << std::endl;
					}
					else if (temporaryString == "SPRITE")
					{
						buildingsFile >> temporaryString;
						buildings[buildingID].SetSprite(temporaryString);
					}
					else if (temporaryString == "ICON")
					{
						buildingsFile >> temporaryString;
						buildings[buildingID].SetIcon(temporaryString);
					}
					else if (temporaryString == "MONEY")
					{
						buildingsFile >> temporaryNumber;
						buildings[buildingID].SetConstructionMoneyCost(temporaryNumber);
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


							buildings[buildingID].SetRessourceIDNeeded(resssourceID, counterRessources);

							buildings[buildingID].SetRessourceQuantityNeeded(resssourceQuantity);
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

							buildings[buildingID].SetRessourceIDNeeded(resssourceID, counterRessources);

							buildings[buildingID].SetRessourceQuantityNeeded(resssourceQuantity);
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

							buildings[buildingID].SetRessourceIDProduced(resssourceID, counterRessources);

							buildings[buildingID].SetRessourceQuantityProduced(resssourceQuantity);
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

							buildings[buildingID].SetRessourceIDProduced(resssourceID, counterRessources);

							buildings[buildingID].SetRessourceQuantityProduced(resssourceQuantity);
						}

					}
					else if (temporaryString == "TIME")
					{
						buildingsFile >> temporaryFloatingNumber;
						buildings[buildingID].SetConstructionTimeCost(temporaryFloatingNumber);

						buildingsFile >> temporaryFloatingNumber;
						buildings[buildingID].SetProductionTimeCost(temporaryFloatingNumber);

						buildingsFile >> temporaryFloatingNumber;
						buildings[buildingID].SetPickupingTimeCost(temporaryFloatingNumber);

						buildingsFile >> temporaryFloatingNumber;
						buildings[buildingID].SetDepositingTimeCost(temporaryFloatingNumber);
					}
					else if (temporaryString == "WORKERS")
					{
						// To configure
						buildingsFile >> temporaryNumber;
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