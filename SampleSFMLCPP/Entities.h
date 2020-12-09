#pragma once

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
protected :
	sf::Vector2f m_mapPosition;
	sf::Vector2f m_mapEndPosition;

	Pathfinding *m_path = nullptr;

public:
	// Constructor & Destructor
	Entities();
	~Entities();
};