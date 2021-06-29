#include "Pathfinding.h"
#include "Map.h"


Pathfinding::Pathfinding()
{
	/*blackBackground = LoadSprite("Data/Assets/Sprites/Menu/blackbackground.png", 0);
	tile[NORMAL_TILE] = LoadSprite("Data/Assets/Sprites/Menu/tile.png", 0);
	tile[STARTING_TILE] = LoadSprite("Data/Assets/Sprites/Menu/tile2.png", 0);
	tile[ENDING_TILE] = LoadSprite("Data/Assets/Sprites/Menu/tile3.png", 0);
	tile[PATH_TILE] = LoadSprite("Data/Assets/Sprites/Menu/tile4.png", 0);
	tile[OBSTACLE_TILE] = LoadSprite("Data/Assets/Sprites/Menu/tile5.png", 0);
	tile[5] = LoadSprite("Data/Assets/Sprites/Menu/tile6.png", 0);
	tile[6] = LoadSprite("Data/Assets/Sprites/Menu/tile7.png", 0);*/

	currentNode = nullptr;

	actualStatus = PATHFINDING_INITIALISATION;
	isActive = false;

	elapsedTime = RESET;

	// Init of the linkedList
	openList = LinkedListInitialisation();
	isPressingEnd = false;
	isPressingStart = false;
}


Pathfinding::~Pathfinding()
{
	if (temporaryMap != nullptr)
	{
		for (int i = 0; i < mapSize.y; i++)
		{
			delete temporaryMap[i];
		}

		delete[] temporaryMap;
		temporaryMap = nullptr;
	}

	if (openList != nullptr)
	{
		delete[] openList;
	}

	if (currentNode != nullptr)
	{
		delete currentNode;
	}
}

void Pathfinding::PathfindingReset()
{
	currentNode = nullptr;

	actualStatus = PATHFINDING_INITIALISATION;
	isActive = false;

	elapsedTime = RESET;

	// Init of the linkedList
	openList = LinkedListInitialisation();
	isPressingEnd = false;
	isPressingStart = false;
}


void Pathfinding::InitMapCopyPathfinding(unsigned short _mapHeightWanted)
{
	Map* pMap = Map::GetSingleton();

	mapSize = pMap->GetMapSize();

	// Init of the temporary map
	temporaryMap = pMap->GetMapLayerWithoutSpecificsCollisions(_mapHeightWanted);
}


void Pathfinding::AddObstacle(const sf::Vector2i& _position)
{	
	temporaryMap[_position.y][_position.x] = OBSTACLE_TILE;
}

void Pathfinding::DisplayPathfinding(sf::RenderWindow &_window)
{
//	BlitSprite(blackBackground, 0, 0, 0, _window);
//	
//	for (int x = 0; x < mapSize.x; x++)
//	{
//		// MAP DISPLAY
//		for (int y = 0; y < mapSize.y; y++)
//		{
//			BlitSprite(tile[testMap[y][x]], (int)(150 + (x * (TILE_SIZE + 3))), (int)(50 + (y * (TILE_SIZE + 3))), 0, _window);
//
//			if (openList != nullptr)
//			{
//				if (openList->first != nullptr)
//				{
//					LinkedListClass::sElement *currentElement = openList->first;
//
//					for (currentElement = openList->first; currentElement != NULL; currentElement = currentElement->next)
//					{
//						if (((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition == sf::Vector2i(x, y)
//							&& ((Pathfinding::sPathfindingData *)currentElement->data)->status == PATH_FINAL)
//						{
//							BlitSprite(tile[PATH_TILE], (int)(150 + (x * (TILE_SIZE + 3))), (int)(50 + (y * (TILE_SIZE + 3))), 0, _window);
//						}
//						else if (((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition == sf::Vector2i(x, y)
//							&& ((Pathfinding::sPathfindingData *)currentElement->data)->status == PATH_OPEN)
//						{
//							BlitSprite(tile[5], (int)(150 + (x * (TILE_SIZE + 3))), (int)(50 + (y * (TILE_SIZE + 3))), 0, _window);
//						}
//						else if (((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition == sf::Vector2i(x, y)
//							&& ((Pathfinding::sPathfindingData *)currentElement->data)->status == PATH_CLOSE)
//						{
//							BlitSprite(tile[6], (int)(150 + (x * (TILE_SIZE + 3))), (int)(50 + (y * (TILE_SIZE + 3))), 0, _window);
//						}
//					}
//				}
//			}
//		}
//	}
}


void Pathfinding::SetPathStartingPosition(const sf::Vector2i& _mapPosition)
{
	// We replace the current starting position's case to a normal tile
	temporaryMap[startPosition.y][startPosition.x] = NORMAL_TILE;

	// We set the map's new starting position
	startPosition = _mapPosition;
	temporaryMap[_mapPosition.y][_mapPosition.x] = STARTING_TILE;
}

void Pathfinding::SetPathEndingPosition(const sf::Vector2i& _mapPosition)
{
	// We replace the current ending position's case to a normal tile
	temporaryMap[endPosition.y][endPosition.x] = NORMAL_TILE;

	// We set the map's new ending position
	endPosition = _mapPosition;
	temporaryMap[_mapPosition.y][_mapPosition.x] = ENDING_TILE;
}

int Pathfinding::CalculatorHeuristic(const sf::Vector2i& _node, const sf::Vector2i& _endingNode) const
{
	// Manhattan mathematic formula
	return (int)(abs(_node.x - _endingNode.x) + abs(_node.y - _endingNode.y)) * 10;
}

int Pathfinding::CalculatorMovementCost(int _parentCost, bool isDiagonal) const
{
	// If the node is in the diagonale, we add 14, else we add 10 at the parent node cost
	return isDiagonal ? _parentCost + 14 : _parentCost + 10;
}

void Pathfinding::AddFirstNodeToList(const sf::Vector2i& _mapPosition)
{
	LinkedListClass::sElement* firstNode = new LinkedListClass::sElement;
	firstNode->data = new Pathfinding::sPathfindingData;


	((Pathfinding::sPathfindingData *)firstNode->data)->parentLink = nullptr;

	// Save the position in map
	((Pathfinding::sPathfindingData *)firstNode->data)->mapPosition = _mapPosition;


	((Pathfinding::sPathfindingData *)firstNode->data)->heuristic = 99999;
	((Pathfinding::sPathfindingData *)firstNode->data)->movementCost = RESET;

	((Pathfinding::sPathfindingData *)firstNode->data)->nodeTotalValue = ((Pathfinding::sPathfindingData *)firstNode->data)->heuristic + ((Pathfinding::sPathfindingData *)firstNode->data)->movementCost;
	((Pathfinding::sPathfindingData *)firstNode->data)->status = PATH_OPEN;

	firstNode->status = ELEMENT_ACTIVE;
	//std::cout << "First Pos : " << ((Pathfinding::sPathfindingData *)firstNode->data)->mapPosition.x << " " << ((Pathfinding::sPathfindingData *)firstNode->data)->mapPosition.y << std::endl;

	// Add this node at the begging of the list
	AddElementToLinkedList(openList, firstNode, 1);
}


void Pathfinding::AddNewNodeToList(LinkedListClass::sElement *_parentLink, sf::Vector2i _mapPosition, int _parentMovementCost, bool _isDiag)
{
	LinkedListClass::sElement* newNode = new LinkedListClass::sElement;
	newNode->data = new Pathfinding::sPathfindingData;


	((Pathfinding::sPathfindingData *)newNode->data)->parentLink = _parentLink;

	// Save the position in map
	((Pathfinding::sPathfindingData *)newNode->data)->mapPosition = _mapPosition;


	((Pathfinding::sPathfindingData *)newNode->data)->heuristic = CalculatorHeuristic(_mapPosition, endPosition);
	((Pathfinding::sPathfindingData *)newNode->data)->movementCost = CalculatorMovementCost(_parentMovementCost, _isDiag);

	((Pathfinding::sPathfindingData *)newNode->data)->nodeTotalValue = ((Pathfinding::sPathfindingData *)newNode->data)->heuristic + ((Pathfinding::sPathfindingData *)newNode->data)->movementCost;
	((Pathfinding::sPathfindingData *)newNode->data)->status = PATH_OPEN;

	newNode->status = ELEMENT_ACTIVE;

	// Add this node at the begging of the list
	AddElementToLinkedList(openList, newNode, -1);
}


void Pathfinding::ModifyParentLink(LinkedListClass::sElement *_nodeToModify, LinkedListClass::sElement *_newParentLink)
{
	((Pathfinding::sPathfindingData *)_nodeToModify->data)->parentLink = _newParentLink;
}


void Pathfinding::FindLowestNodeValue()
{
	// On parcourt la liste des nodes pour trouver celui qui a la plus petite node value
	if (openList != nullptr)
	{
		if (openList->first != nullptr)
		{
			LinkedListClass::sElement *lowestValueAdress = openList->first;
			LinkedListClass::sElement *currentElement = openList->first;

			int lowestNodeTotalValue = ((Pathfinding::sPathfindingData *)currentElement->data)->nodeTotalValue;
			int lowestHeuristicValue = ((Pathfinding::sPathfindingData *)currentElement->data)->heuristic;

			bool isListIsEmpty = false;

			for (currentElement = openList->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Pathfinding::sPathfindingData *)currentElement->data)->status == PATH_OPEN)
				{
					if (lowestNodeTotalValue > ((Pathfinding::sPathfindingData *)currentElement->data)->nodeTotalValue)
					{
						// Sauvegarder la nouvelle adresse
						lowestValueAdress = currentElement;
						lowestNodeTotalValue = ((Pathfinding::sPathfindingData *)currentElement->data)->nodeTotalValue;
					}
					else if (lowestNodeTotalValue == ((Pathfinding::sPathfindingData *)currentElement->data)->nodeTotalValue)
					{
						// check l'heuristique le plus bas
						if (lowestHeuristicValue > ((Pathfinding::sPathfindingData *)currentElement->data)->heuristic)
						{
							lowestValueAdress = currentElement;
							lowestNodeTotalValue = ((Pathfinding::sPathfindingData *)currentElement->data)->nodeTotalValue;
						}
					}
				}
			}

			if (isListIsEmpty)
			{
				// Cela veut dire que la liste est vide est qu'il n'y a pas de chemin
				actualStatus = PATHFINDING_FIND_NO_WAY;
			}

			currentNode = lowestValueAdress;

			//std::cout << "Adress : " << currentNode << " " << ((Pathfinding::sPathfindingData *)currentNode->data)->heuristic << " " << ((Pathfinding::sPathfindingData *)currentNode->data)->movementCost << " " << ((Pathfinding::sPathfindingData *)currentNode->data)->nodeTotalValue << std::endl;
			//std::cout << "Map Position x : " << ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x << " & " << ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y << std::endl;
			//std::cout << "Parent adress : " << ((Pathfinding::sPathfindingData *)currentNode->data)->parentLink << std::endl << std::endl;

			((Pathfinding::sPathfindingData *)currentNode->data)->status = PATH_CLOSE;
		}
		else
		{
			//std::cout << "List : " << list->first << std::endl;
		}
	}
}


void Pathfinding::FindNodeArounds()
{
	if (actualStatus == PATHFINDING_SEARCH)
	{
		// We check every tiles arround the node targeted
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				if (!(x == 0 && y == 0))
				{
					if (((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x + x >= 0
						&& ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x + x < mapSize.x
						&& ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y + y >= 0
						&& ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y + y < mapSize.y)
					{
						if (temporaryMap[(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y + y][(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x + x] == 0)
						{
							bool isDiagIsFree = true;

							if ((abs(x) + abs(y)) % 2 == 0)
							{
								if (x < 0)
								{
									if (y > 0)
									{
										if (temporaryMap[(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y][(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x - 1] != 0
											&& temporaryMap[(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y + 1][(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x] != 0)
										{
											isDiagIsFree = false;
											//std::cout << "Direction principales libres\n";
										}
									}
									else
									{
										if (temporaryMap[(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y][(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x - 1] != 0
											&& temporaryMap[(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y - 1][(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x] != 0)
										{
											isDiagIsFree = false;
											//std::cout << "Direction principales libres\n";
										}
									}
								}
								else if (x > 0)
								{
									if (y > 0)
									{
										if (temporaryMap[(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y][(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x + 1] != 0
											&& temporaryMap[(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y + 1][(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x] != 0)
										{
											isDiagIsFree = false;
											//std::cout << "Direction principales libres\n";
										}
									}
									else
									{
										if (temporaryMap[(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y][(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x + 1] != 0
											&& temporaryMap[(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y - 1][(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x] != 0)
										{
											isDiagIsFree = false;
											//std::cout << "Direction principales libres\n";
										}
									}
								}
							}

							if (isDiagIsFree)
							{
								LinkedListClass::sElement *currentElement = openList->first;
								bool isAlreadyExist = false;

								for (currentElement = openList->first; currentElement != NULL; currentElement = currentElement->next)
								{
									if ((((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x + x == ((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition.x)
										&& (((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y + y == ((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition.y))
									{
										isAlreadyExist = true;

										// If the direction sent is horizontal or verticale, we send false at the end of the AddNewNodeToList function's
										if ((abs(x) + abs(y)) % 2 == 1)
										{
											if (((Pathfinding::sPathfindingData *)currentElement->data)->movementCost >
												CalculatorMovementCost(((Pathfinding::sPathfindingData *)currentNode->data)->movementCost, false))
											{
												//std::cout << "HEREEEE : " << ((Pathfinding::sPathfindingData *)currentElement->data)->movementCost << " & " << CalculatorMovementCost(((Pathfinding::sPathfindingData *)currentNode->data)->movementCost, false) << std::endl;

												ModifyParentLink(currentElement, currentNode);
											}
										}
										// If it's a diagonal direction sent, we send true at the end of the AddNewNodeToList function's
										else
										{
											if (((Pathfinding::sPathfindingData *)currentElement->data)->movementCost >
												CalculatorMovementCost(((Pathfinding::sPathfindingData *)currentNode->data)->movementCost, true))
											{
												//std::cout << "HEREEEE : " << ((Pathfinding::sPathfindingData *)currentElement->data)->movementCost << " & " << CalculatorMovementCost(((Pathfinding::sPathfindingData *)currentNode->data)->movementCost, true) << std::endl;

												ModifyParentLink(currentElement, currentNode);
											}
										}
									}
								}

								if (!isAlreadyExist)
								{
									// If the direction sent is horizontal or verticale, we send false at the end of the AddNewNodeToList function's
									if ((abs(x) + abs(y)) % 2 == 1)
									{
										AddNewNodeToList(currentNode, sf::Vector2i(((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x + x, ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y + y), ((Pathfinding::sPathfindingData *)currentNode->data)->movementCost, false);
									}
									// If it's a diagonal direction sent, we send true at the end of the AddNewNodeToList function's
									else
									{
										AddNewNodeToList(currentNode, sf::Vector2i(((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x + x, ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y + y), ((Pathfinding::sPathfindingData *)currentNode->data)->movementCost, true);
									}
								}
							}
						}
						else if (temporaryMap[(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y + y][(int)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x + x] == ENDING_TILE)
						{
							actualStatus = PATHFINDING_FIND_WAY_FIRST;
							int counter(1);

							// If the direction sent is horizontal or verticale, we send false at the end of the AddNewNodeToList function's
							if ((abs(x) + abs(y)) % 2 == 1)
							{
								AddNewNodeToList(currentNode, sf::Vector2i(((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x + x, ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y + y), ((Pathfinding::sPathfindingData *)currentNode->data)->movementCost, false);
							}
							// If it's a diagonal direction sent, we send true at the end of the AddNewNodeToList function's
							else
							{
								AddNewNodeToList(currentNode, sf::Vector2i(((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x + x, ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y + y), ((Pathfinding::sPathfindingData *)currentNode->data)->movementCost, true);
							}


							LinkedListClass::sElement *currentElement = openList->first;

							currentElement = openList->last;

							if (((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition.x == endPosition.x
								&& ((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition.y == endPosition.y)
							{
								((Pathfinding::sPathfindingData *)currentElement->data)->status = PATH_FINAL;

								while (((Pathfinding::sPathfindingData *)currentElement->data)->parentLink != nullptr)
								{
									counter++;
									//std::cout << "Map Pos : " << ((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition.x << " " << ((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition.y << std::endl;
									//std::cout << "Adress : " << element << std::endl;
									((Pathfinding::sPathfindingData *)currentElement->data)->status = PATH_FINAL;

									currentElement = (LinkedListClass::sElement*)((Pathfinding::sPathfindingData *)currentElement->data)->parentLink;
								}

								//std::cout << "Longueur liste : " << counter << " elements \n";


								LinkedListClass::sElement *finalElement = openList->first;

								for (finalElement = openList->first; finalElement != NULL; finalElement = finalElement->next)
								{
									if (((Pathfinding::sPathfindingData *)finalElement->data)->status != PATH_FINAL)
									{
										finalElement->status = ELEMENT_DELETION_REQUIRED;
									}
								}

								RemoveElementsOfLinkedList(openList);
							}
							else
							{
								// ERROR LOG à ajouter
								std::cout << "ERREUR PATHFINDING\n";
							}

							// TO OPTIMIZE
							// Cut and stop the loop
							x = 2;
							y = 2;
						}
					}
				}
			}
		}
	}
}


void Pathfinding::T() // Temporaire
{
	isActive = true;
}

void Pathfinding::MainStatePathfinding()
{
	// First initialisation
	if (actualStatus == PATHFINDING_INITIALISATION)
	{
		// Init of the linkedList
		//openList = LinkedListInitialisation();

		// Init of the start and ending position
		temporaryMap[startPosition.y][startPosition.x] = STARTING_TILE;
		temporaryMap[endPosition.y][endPosition.x] = ENDING_TILE;
		
		// Add the first node with the starting position entered
		AddFirstNodeToList(startPosition);

		actualStatus = PATHFINDING_SEARCH;
	}
	


	while (actualStatus == PATHFINDING_SEARCH)
	{
		FindLowestNodeValue();
		
		FindNodeArounds();
	}

	// Step by step
	/*if (isActive == true)
	{
		isActive = false;

		FindLowestNodeValue();

		FindNodeArounds();
	}*/
}


void Pathfinding::WalkProcess(sf::Vector2f *_workerPosition, const float &_speed)
{
	if (actualStatus == PATHFINDING_FIND_WAY_FIRST)
	{
		// Set le premier élément à la fin de la découverte du chemin ?
		currentNode = openList->first;

		_workerPosition->x = (float)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x;
		_workerPosition->y = (float)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y;

		//std::cout << "First Pos : " << ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x << " " << ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y << std::endl;
		
		actualStatus = PATHFINDING_FIND_WAY;
	}
	else if (actualStatus == PATHFINDING_FIND_WAY)
	{
		if (currentNode->next != nullptr)
		{
			LinkedListClass::sElement *nextElement = currentNode->next;

			sf::Vector2f difference(_workerPosition->x - (float)((Pathfinding::sPathfindingData *)nextElement->data)->mapPosition.x,
									_workerPosition->y - (float)((Pathfinding::sPathfindingData *)nextElement->data)->mapPosition.y);
			
			if (difference.x > 0)
			{
				_workerPosition->x -= _speed;
			}
			else if (difference.x < 0)
			{
				_workerPosition->x += _speed;
			}

			if (difference.y > 0)
			{
				_workerPosition->y -= _speed;
			}
			else if (difference.y < 0)
			{
				_workerPosition->y += _speed;
			}

			elapsedTime += _speed;

			if (elapsedTime >= 1
				&& DistanceFormula(*(_workerPosition), (sf::Vector2f)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition) < 1.45f)
			{
				elapsedTime = RESET;

				currentNode = currentNode->next;

				//std::cout << "Pos : " << ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x << " " << ((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y << std::endl;
				_workerPosition->x = (float)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.x;
				_workerPosition->y = (float)((Pathfinding::sPathfindingData *)currentNode->data)->mapPosition.y;
			}
		}
		else
		{
			actualStatus = PATHFINDING_NEED_TO_BE_DELETED;
		}
	}
}