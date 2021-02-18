#pragma once

#include "Common.hpp"
#include "Buildings.h"

enum Dimensions
{
	COLLISIONS_ID,
	BUILDING_ID,
	SPRITE_ID,
};

enum Floors
{
	ZERO_FLOOR,
	FIRST_FLOOR = 3,
	SECOND_FLOOR = 6,
	THIRD_FLOOR = 9,
};

enum TypesOfCollisions
{
	NO_COLLISION,
	COLLISION,
	BUILDING_ENTRANCE,
	BUILDING_EXIT,
	BUILDING_GHOST,
	BUILDING_WILL_BE_DESTROYED,
	PATH,
	STONE_PATH,
	ROAD,
};

class Map
{
private :
	unsigned short*** m_map;
	unsigned short m_numberLayers;
	unsigned short m_numberLines;
	unsigned short m_numberColumns;

public :

	// Constructor & Destructor
	Map();
	~Map();

	// Setters
	inline void SetNumberOfLayers(unsigned char _nbOfLayers) {  m_numberLayers = _nbOfLayers; }
	inline void SetNumberOfLines(unsigned char _nbOfLines) { m_numberLines = _nbOfLines; }
	inline void SetNumberOfColumns(unsigned char _nbOfColumns) { m_numberColumns = _nbOfColumns; }

	// Getters
	inline unsigned short GetNumberOfLayers() const { return m_numberLayers;  }
	inline unsigned short GetNumberOfLines() const { return m_numberLines;  }
	inline unsigned short GetNumberOfColumns() const { return m_numberColumns;  }
	inline unsigned short*** GetMap() { return m_map;  }

	// Methods
	void InitMapFromFile();

	void SavingMapFromFile(std::ofstream* _file);
	void LoadingMapFromFile(std::ifstream* _file);

	bool IsCoordinatesIsInMap(const sf::Vector2i& _mapPosition);
};

