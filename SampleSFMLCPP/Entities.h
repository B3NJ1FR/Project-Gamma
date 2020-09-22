#ifndef ENTITIES__H
#define ENTITIES__H

#include "Pathfinding.h"

enum WorkerStatus
{
	IDLE,
	WAITING_MOVEMENT,
	MOVEMENT,
	BUILDING,
	WORKING,
	PICKUP_RESSOURCES,
	DEPOSIT_RESSOURCES,
};


class Entities : public Pathfinding
{
public : 
	Entities();
	~Entities();

protected :
	sf::Vector2f mapPosition;
	sf::Vector2f mapEndPosition;

	Pathfinding *path;
};



#endif // !ENTITIES__H