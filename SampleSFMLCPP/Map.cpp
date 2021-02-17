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

						if (x == 10 && y == 8)
						{
							// Sol
							m_map[z - 3][y][x] = 34;

							// Sprite villa
							m_map[z][y][x] = 35;

							// Collision
							m_map[z - 2][y][x] = COLLISION;
							m_map[z - 2][y][x - 1] = COLLISION;
							m_map[z - 2][y][x - 2] = COLLISION;
							m_map[z - 2][y][x - 3] = COLLISION;
							m_map[z - 2][y][x - 4] = COLLISION;
							m_map[z - 2][y][x - 5] = COLLISION;

							m_map[z - 2][y - 1][x] = COLLISION;
							m_map[z - 2][y - 1][x - 1] = COLLISION;
							m_map[z - 2][y - 1][x - 2] = COLLISION;
							m_map[z - 2][y - 1][x - 3] = COLLISION;
							m_map[z - 2][y - 1][x - 4] = COLLISION;
							m_map[z - 2][y - 1][x - 5] = COLLISION;

							// Building ID
							m_map[z - 1][y][x] = BUILDING_VILLA;
							m_map[z - 1][y][x - 1] = BUILDING_VILLA;
							m_map[z - 1][y][x - 2] = BUILDING_VILLA;
							m_map[z - 1][y][x - 3] = BUILDING_VILLA;
							m_map[z - 1][y][x - 4] = BUILDING_VILLA;
							m_map[z - 1][y][x - 5] = BUILDING_VILLA;

							m_map[z - 1][y - 1][x] = BUILDING_VILLA;
							m_map[z - 1][y - 1][x - 1] = BUILDING_VILLA;
							m_map[z - 1][y - 1][x - 2] = BUILDING_VILLA;
							m_map[z - 1][y - 1][x - 3] = BUILDING_VILLA;
							m_map[z - 1][y - 1][x - 4] = BUILDING_VILLA;
							m_map[z - 1][y - 1][x - 5] = BUILDING_VILLA;
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

void Map::SavingMapFromFile(std::ofstream* _file)
{
	// Saving the map size
	_file->write((char*)&m_numberColumns, sizeof(unsigned short));
	_file->write((char*)&m_numberLines, sizeof(unsigned short));
	_file->write((char*)&m_numberLayers, sizeof(unsigned short));

	std::cout << "Size saved !\n";


	// Saving the map content
	for (unsigned short z = 0; z < m_numberLayers; z++)
	{
		for (unsigned short y = 0; y < m_numberLines; y++)
		{
			for (unsigned short x = 0; x < m_numberColumns; x++)
			{
				_file->write((char*) &m_map[z][y][x], sizeof(unsigned short));
			}
		}
	}

	std::cout << "Map saved !\n";
}


void Map::LoadingMapFromFile(std::ifstream* _file)
{
	// Loading the map size
	_file->read((char*)&m_numberColumns, sizeof(unsigned short));
	_file->read((char*)&m_numberLines, sizeof(unsigned short));
	_file->read((char*)&m_numberLayers, sizeof(unsigned short));

	std::cout << "Map size loaded : " << m_numberColumns << " " << m_numberLines<< " " << m_numberLayers << std::endl;

	// Loading the map content
	for (auto z = 0; z < m_numberLayers; z++)
	{
		for (auto y = 0; y < m_numberLines; y++)
		{
			for (auto x = 0; x < m_numberColumns; x++)
			{
				_file->read((char*) &m_map[z][y][x], sizeof(unsigned short));
			}
		}
	}

	std::cout << "Map loaded !\n";
}

bool Map::IsCoordinatesIsInMap(const sf::Vector2i& _mapPosition)
{
	if (_mapPosition.x >= 0
		&& _mapPosition.x < m_numberColumns
		&& _mapPosition.y >= 0
		&& _mapPosition.y < m_numberLines)
	{
		return true;
	}
	else
	{
		return false;
	}
}