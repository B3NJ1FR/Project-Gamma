#pragma 

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
	PATH,
	STONE_PATH,
	ROAD,
};

class Map
{
private :
	unsigned short*** map;
	unsigned short numberLayers;
	unsigned short numberLines;
	unsigned short numberColumns;

public :

	// Constructor & Destructor
	Map();
	~Map();

	// Methods
	void InitMapFromFile();
};

