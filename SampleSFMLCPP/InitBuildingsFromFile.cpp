#include "InitBuildingsFromFile.h"
#include "Buildings.h"
#include "GameDefinitions.h"


void InitBuildingsFromFile(struct Game *_game)
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
			buildingsFile >> _game->numberOfBuilding;
			std::cout << "Number of buildings : " << _game->numberOfBuilding << std::endl << std::endl;

			// Buildings array allocation that will conserve all the buildings data
			_game->buildings = new Buildings[_game->numberOfBuilding];

			isAllocated = true;
		}
		
		if (temporaryString == "BUILDING" && isAllocated == true)
		{
			buildingsFile >> temporaryNumber;
			unsigned short buildingID = temporaryNumber;

			// Security to avoid to add elements into the void
			if (buildingID <= (_game->numberOfBuilding - 1))
			{
				buildingsFile >> temporaryString;
				_game->buildings[buildingID].SetName(temporaryString);

				// Pickup the ID and the name of the building
				std::cout << "Building " << buildingID << " - " << temporaryString << " - " ;
				
				while (temporaryString != "WORKERS")
				{
					buildingsFile >> temporaryString;

					if (temporaryString == "SIZE")
					{
						sf::Vector2i temporarySize = { RESET, RESET };

						buildingsFile >> temporarySize.x >> temporarySize.y;
						_game->buildings[buildingID].SetSizeCaracteristics(temporarySize);

						std::cout << temporarySize.x << "x" << temporarySize.y << std::endl;

					}
					else if (temporaryString == "ENTRANCE")
					{
						sf::Vector2i temporaryEntrance = { RESET, RESET };

						buildingsFile >> temporaryEntrance.x >> temporaryEntrance.y;
						_game->buildings[buildingID].SetEntranceCaracteristics(temporaryEntrance);

						std::cout << "\tEntrance :\t" << temporaryEntrance.x << " " << temporaryEntrance.y << std::endl;
					}
					else if (temporaryString == "EXIT")
					{
						sf::Vector2i temporaryExit = { RESET, RESET };

						buildingsFile >> temporaryExit.x >> temporaryExit.y;
						_game->buildings[buildingID].SetExitCaracteristics(temporaryExit);

						std::cout << "\tExit :\t\t" << temporaryExit.x << " " << temporaryExit.y << std::endl;
					}
					else if (temporaryString == "SPRITE")
					{
						buildingsFile >> temporaryString;
						_game->buildings[buildingID].SetSprite(temporaryString);
					}
					else if (temporaryString == "ICON")
					{
						buildingsFile >> temporaryString;
						_game->buildings[buildingID].SetIcon(temporaryString);
					}
					else if (temporaryString == "MONEY")
					{
						buildingsFile >> temporaryNumber;
						_game->buildings[buildingID].SetConstructionMoneyCost(temporaryNumber);
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
							int *resssourceID = new int;
							int *resssourceQuantity = new int;


							buildingsFile >> temporaryNumber;
							*resssourceID = temporaryNumber;

							buildingsFile >> temporaryNumber;
							*resssourceQuantity = temporaryNumber;


							_game->buildings[buildingID].SetRessourceIDNeeded(resssourceID, counterRessources);

							_game->buildings[buildingID].SetRessourceQuantityNeeded(resssourceQuantity);
						}
						else
						{
							int *resssourceID = new int[counterRessources];
							int *resssourceQuantity = new int[counterRessources];

							for (int i = 0; i < counterRessources; i++)
							{
								buildingsFile >> temporaryNumber;
								resssourceID[i] = temporaryNumber;

								buildingsFile >> temporaryNumber;
								resssourceQuantity[i] = temporaryNumber;
							}

							_game->buildings[buildingID].SetRessourceIDNeeded(resssourceID, counterRessources);

							_game->buildings[buildingID].SetRessourceQuantityNeeded(resssourceQuantity);
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
							int *resssourceID = new int;
							int *resssourceQuantity = new int;


							buildingsFile >> temporaryNumber;
							*resssourceID = temporaryNumber;

							buildingsFile >> temporaryNumber;
							*resssourceQuantity = temporaryNumber;

							_game->buildings[buildingID].SetRessourceIDProduced(resssourceID, counterRessources);

							_game->buildings[buildingID].SetRessourceQuantityProduced(resssourceQuantity);
						}
						else
						{
							int *resssourceID = new int[counterRessources];
							int *resssourceQuantity = new int[counterRessources];

							for (int i = 0; i < counterRessources; i++)
							{
								buildingsFile >> temporaryNumber;
								resssourceID[i] = temporaryNumber;

								buildingsFile >> temporaryNumber;
								resssourceQuantity[i] = temporaryNumber;
							}

							_game->buildings[buildingID].SetRessourceIDProduced(resssourceID, counterRessources);

							_game->buildings[buildingID].SetRessourceQuantityProduced(resssourceQuantity);
						}
						
					}
					else if (temporaryString == "TIME")
					{
						buildingsFile >> temporaryFloatingNumber;
						_game->buildings[buildingID].SetConstructionTimeCost(temporaryFloatingNumber);

						buildingsFile >> temporaryFloatingNumber;
						_game->buildings[buildingID].SetProductionTimeCost(temporaryFloatingNumber);
						
						buildingsFile >> temporaryFloatingNumber;
						_game->buildings[buildingID].SetPickupingTimeCost(temporaryFloatingNumber);

						buildingsFile >> temporaryFloatingNumber;
						_game->buildings[buildingID].SetDepositingTimeCost(temporaryFloatingNumber);
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