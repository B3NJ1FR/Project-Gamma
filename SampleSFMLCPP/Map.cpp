#include "Map.h"


Map::Map()
{
	m_map = nullptr;
	m_numberLayers = 0;
	m_numberLines = 0;
	m_numberColumns = 0;

	InitMapFromFile();
}

Map::~Map()
{
	for (int i = 0; i < m_numberLayers; i++)
	{
		for (int j = 0; j < m_numberLines; j++)
		{
			delete m_map[i][j];
		}
	}

	for (int i = 0; i < m_numberLayers; i++)
	{
		delete m_map[i];
	}

	delete m_map;
	m_map = nullptr;
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
			mapDataFile >> m_numberColumns >> m_numberLines >> m_numberLayers;

			std::cout << "Map size : " << m_numberColumns << " " << m_numberLines << " " << m_numberLayers << std::endl;

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
		m_map = new unsigned short** [m_numberLayers];

		for (int i = 0; i < m_numberLayers; i++)
		{
			m_map[i] = new unsigned short* [m_numberLines];
		}

		for (int i = 0; i < m_numberLayers; i++)
		{
			for (int j = 0; j < m_numberLines; j++)
			{
				m_map[i][j] = new unsigned short[m_numberColumns];
			}
		}

		for (int z = 0; z < m_numberLayers; z++)
		{
			for (int y = 0; y < m_numberLines; y++)
			{
				for (int x = 0; x < m_numberColumns; x++)
				{
					m_map[z][y][x] = RESET;

					if (z % 3 == BUILDING_ID)
					{
						m_map[z][y][x] = -1;
					}

					if (z == (ZERO_FLOOR + COLLISIONS_ID) && z % 3 == COLLISIONS_ID)
					{
						// Collisions of the road
						if (y == 4)
						{
							m_map[z][y][x] = ROAD;
						}
						else if (y == 5)
						{
							m_map[z][y][x] = ROAD;
						}
					}

					if (z == (ZERO_FLOOR + SPRITE_ID) && z % 3 == SPRITE_ID)
					{
						// Display of the road
						if (y == 4)
						{
							m_map[z][y][x] = 5;
						}
						else if (y == 5)
						{
							m_map[z][y][x] = 6;
						}
						else
						{
							m_map[z][y][x] = rand() % 2 + 1;
						}
					}

					if (z == (FIRST_FLOOR + SPRITE_ID))
					{
						if (y <= 3)
						{
							bool randomNumber(rand() % 2);

							if (randomNumber == 1)
							{
								m_map[z][y][x] = 11;
								m_map[z - 2][y][x] = COLLISION;
							}
							else
							{
								m_map[z][y][x] = 0;
							}
						}

						if (x == 5 && y == 8)
						{
							m_map[z][y][x] = 33;
							m_map[z - 2][y][x] = COLLISION;
							m_map[z - 1][y][x] = BUILDING_VILLA;
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