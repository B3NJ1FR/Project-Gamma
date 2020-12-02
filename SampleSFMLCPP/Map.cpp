#include "Map.h"


Map::Map()
{
	map = nullptr;
	numberLayers = 0;
	numberLines = 0;
	numberColumns = 0;

	InitMapFromFile();
}

Map::~Map()
{
	for (int i = 0; i < numberLayers; i++)
	{
		for (int j = 0; j < numberLines; j++)
		{
			delete map[i][j];
		}
	}

	for (int i = 0; i < numberLayers; i++)
	{
		delete map[i];
	}

	delete map;
	map = nullptr;
}


void Map::InitMapFromFile()
{
	// WARNING : THE MAP IS DEFINED WITH UNSIGNED SHORT !

	std::ifstream mapDataFile("Data/Configurations/Settings.data", std::ios::in);

	std::string temporaryString;
	temporaryString.erase();

	bool isMapHasBeenDefined = false;

	if (!mapDataFile.is_open())
	{
		std::cout << "Error accessing Settings.data file" << std::endl;

		exit(EXIT_FAILURE);
	}

	while (!mapDataFile.eof())
	{
		mapDataFile >> temporaryString;

		if (temporaryString == "MAP"
			&& isMapHasBeenDefined == false)
		{
			// Read the map size infos
			mapDataFile >> numberColumns >> numberLines >> numberLayers;

			std::cout << "Map size : " << numberColumns << " " << numberLines << " " << numberLayers << std::endl;

			isMapHasBeenDefined = true;
			temporaryString.erase();
		}
	}

	mapDataFile.close();


	// Temporary, to define into a config file in the futur
	/*numberLayers = THIRD_FLOOR;
	numberLines = 35;
	numberColumns = 40;*/

	if (isMapHasBeenDefined == true)
	{
		map = new unsigned short** [numberLayers];

		for (int i = 0; i < numberLayers; i++)
		{
			map[i] = new unsigned short* [numberLines];
		}

		for (int i = 0; i < numberLayers; i++)
		{
			for (int j = 0; j < numberLines; j++)
			{
				map[i][j] = new unsigned short[numberColumns];
			}
		}

		for (int z = 0; z < numberLayers; z++)
		{
			for (int y = 0; y < numberLines; y++)
			{
				for (int x = 0; x < numberColumns; x++)
				{
					map[z][y][x] = RESET;

					if (z % 3 == BUILDING_ID)
					{
						map[z][y][x] = -1;
					}

					if (z == (ZERO_FLOOR + COLLISIONS_ID) && z % 3 == COLLISIONS_ID)
					{
						// Collisions of the road
						if (y == 4)
						{
							map[z][y][x] = ROAD;
						}
						else if (y == 5)
						{
							map[z][y][x] = ROAD;
						}
					}

					if (z == (ZERO_FLOOR + SPRITE_ID) && z % 3 == SPRITE_ID)
					{
						// Display of the road
						if (y == 4)
						{
							map[z][y][x] = 5;
						}
						else if (y == 5)
						{
							map[z][y][x] = 6;
						}
						else
						{
							map[z][y][x] = rand() % 2 + 1;
						}
					}

					if (z == (FIRST_FLOOR + SPRITE_ID))
					{
						if (y <= 3)
						{
							bool randomNumber(rand() % 2);

							if (randomNumber == 1)
							{
								map[z][y][x] = 11;
								map[z - 2][y][x] = COLLISION;
							}
							else
							{
								map[z][y][x] = 0;
							}
						}

						if (x == 5 && y == 8)
						{
							map[z][y][x] = 33;
							map[z - 2][y][x] = COLLISION;
							map[z - 1][y][x] = BUILDING_VILLA;
						}

					}

				}
			}
		}
	}
	else
	{
		// ERROR LOG
		std::cout << "ERROR OF MAP DEFINITION\n\n\n";
	}
}