#ifndef PATHFINDING__H
#define PATHFINDING__H

#include "LinkedList.h"

#define TILE_SIZE 32

enum Tile
{
	NORMAL_TILE,
	STARTING_TILE,
	ENDING_TILE,
	PATH_TILE,
	OBSTACLE_TILE,
};

enum PathfindingElementStatus
{
	PATH_OPEN,
	PATH_CLOSE,
	PATH_FINAL,
};

enum PathfindingGeneralProcessStatus
{
	PATHFINDING_INITIALISATION,
	PATHFINDING_SEARCH,
	PATHFINDING_FIND_WAY_FIRST,
	PATHFINDING_FIND_WAY,
	PATHFINDING_FIND_NO_WAY,
	PATHFINDING_NEED_TO_BE_DELETED,
};

class Pathfinding : public LinkedListClass
{
private :

	struct PathfindingData
	{
		LinkedListClass::sElement *parentLink;

		sf::Vector2i mapPosition;

		int heuristic;
		int movementCost;

		int nodeTotalValue;

		enum PathfindingElementStatus status;
	};
	typedef struct PathfindingData sPathfindingData;

	bool isActive;

	unsigned short **testMap;
	sf::Sprite blackBackground; // Temporaire
	sf::Sprite tile[7]; // Temporaire

	float elapsedTime;

	LinkedListClass::sLinkedList *openList;

	sf::Vector2i startPosition;
	sf::Vector2i endPosition;
	sf::Vector2i mapSize;


	enum PathfindingGeneralProcessStatus actualStatus;

	LinkedListClass::sElement *currentNode;


public:
	Pathfinding();
	~Pathfinding();
	void InitMapCopyPathfinding(sf::Vector2i _mapSize, unsigned short ***_map, unsigned short _mapHeight);
	void AddObstacle(sf::Vector2i _position);

	void DisplayPathfinding(sf::RenderWindow &_window);
	int CalculatorHeuristic(sf::Vector2i _node, sf::Vector2i _endingNode);
	int CalculatorMovementCost(int _parentCost, bool isDiagonal = 0);

	void SetPathStartingPosition(sf::Vector2i _mapPosition);
	void SetPathEndingPosition(sf::Vector2i _mapPosition);

	void AddFirstNodeToList(sf::Vector2i _mapPosition);
	void AddNewNodeToList(LinkedListClass::sElement *_parentLink, sf::Vector2i _mapPosition, int _parentMovementCost, bool _isDiag);

	void ModifyParentLink(LinkedListClass::sElement *_nodeToModify, LinkedListClass::sElement *_newParentLink);
	void FindLowestNodeValue();
	void FindNodeArounds();

	void MainStatePathfinding();
	void WalkProcess(sf::Vector2f *_workerPosition, const float &_speed = 1);
	void T(); // Temporaire

	enum PathfindingGeneralProcessStatus GetActualStatus();

	bool isPressingStart; // Temporaire
	bool isPressingEnd; // Temporaire
};

#endif // !PATHFINDING__H