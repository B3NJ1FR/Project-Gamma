#include "Pathfinding.h"


Pathfinding::Pathfinding()
{
	/*this->blackBackground = LoadSprite("Data/Assets/Sprites/Menu/blackbackground.png", 0);
	this->tile[NORMAL_TILE] = LoadSprite("Data/Assets/Sprites/Menu/tile.png", 0);
	this->tile[STARTING_TILE] = LoadSprite("Data/Assets/Sprites/Menu/tile2.png", 0);
	this->tile[ENDING_TILE] = LoadSprite("Data/Assets/Sprites/Menu/tile3.png", 0);
	this->tile[PATH_TILE] = LoadSprite("Data/Assets/Sprites/Menu/tile4.png", 0);
	this->tile[OBSTACLE_TILE] = LoadSprite("Data/Assets/Sprites/Menu/tile5.png", 0);
	this->tile[5] = LoadSprite("Data/Assets/Sprites/Menu/tile6.png", 0);
	this->tile[6] = LoadSprite("Data/Assets/Sprites/Menu/tile7.png", 0);*/

	this->currentNode = nullptr;

	this->actualStatus = PATHFINDING_INITIALISATION;
	this->isActive = false;

	this->elapsedTime = RESET;

	// Init of the linkedList
	this->openList = LinkedListInitialisation();
	this->isPressingEnd = false;
	this->isPressingStart = false;
}


Pathfinding::~Pathfinding()
{

}

void Pathfinding::PathfindingReset()
{
	this->currentNode = nullptr;

	this->actualStatus = PATHFINDING_INITIALISATION;
	this->isActive = false;

	this->elapsedTime = RESET;

	// Init of the linkedList
	this->openList = LinkedListInitialisation();
	this->isPressingEnd = false;
	this->isPressingStart = false;
}


void Pathfinding::InitMapCopyPathfinding(sf::Vector2i _mapSize, unsigned short ***_map, unsigned short _mapHeight)
{
	this->mapSize = _mapSize;

	// Init of the factice map
	this->testMap = new unsigned short *[this->mapSize.y];

	for (int i = 0; i < this->mapSize.y; i++)
	{
		this->testMap[i] = new unsigned short[this->mapSize.x];
	}

	// Copying the collisions of the real map
	for (int y = 0; y < this->mapSize.y; y++)
	{
		for (int x = 0; x < this->mapSize.x; x++)
		{
			this->testMap[y][x] = _map[_mapHeight][y][x];
		}
	}
}



void Pathfinding::AddObstacle(sf::Vector2i _position)
{	
	this->testMap[_position.y][_position.x] = OBSTACLE_TILE;
}

void Pathfinding::DisplayPathfinding(sf::RenderWindow &_window)
{
//	BlitSprite(this->blackBackground, 0, 0, 0, _window);
//	
//	for (int x = 0; x < this->mapSize.x; x++)
//	{
//		// MAP DISPLAY
//		for (int y = 0; y < this->mapSize.y; y++)
//		{
//			BlitSprite(this->tile[this->testMap[y][x]], (int)(150 + (x * (TILE_SIZE + 3))), (int)(50 + (y * (TILE_SIZE + 3))), 0, _window);
//
//			if (this->openList != nullptr)
//			{
//				if (this->openList->first != nullptr)
//				{
//					LinkedListClass::sElement *currentElement = this->openList->first;
//
//					for (currentElement = this->openList->first; currentElement != NULL; currentElement = currentElement->next)
//					{
//						if (((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition == sf::Vector2i(x, y)
//							&& ((Pathfinding::sPathfindingData *)currentElement->data)->status == PATH_FINAL)
//						{
//							BlitSprite(this->tile[PATH_TILE], (int)(150 + (x * (TILE_SIZE + 3))), (int)(50 + (y * (TILE_SIZE + 3))), 0, _window);
//						}
//						else if (((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition == sf::Vector2i(x, y)
//							&& ((Pathfinding::sPathfindingData *)currentElement->data)->status == PATH_OPEN)
//						{
//							BlitSprite(this->tile[5], (int)(150 + (x * (TILE_SIZE + 3))), (int)(50 + (y * (TILE_SIZE + 3))), 0, _window);
//						}
//						else if (((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition == sf::Vector2i(x, y)
//							&& ((Pathfinding::sPathfindingData *)currentElement->data)->status == PATH_CLOSE)
//						{
//							BlitSprite(this->tile[6], (int)(150 + (x * (TILE_SIZE + 3))), (int)(50 + (y * (TILE_SIZE + 3))), 0, _window);
//						}
//					}
//				}
//			}
//		}
//	}
}


void Pathfinding::SetPathStartingPosition(sf::Vector2i _mapPosition)
{
	this->testMap[startPosition.y][startPosition.x] = NORMAL_TILE;
	this->startPosition = _mapPosition;
	this->testMap[_mapPosition.y][_mapPosition.x] = STARTING_TILE;
}

void Pathfinding::SetPathEndingPosition(sf::Vector2i _mapPosition)
{
	this->testMap[endPosition.y][endPosition.x] = NORMAL_TILE;
	this->endPosition = _mapPosition;
	this->testMap[_mapPosition.y][_mapPosition.x] = ENDING_TILE;
}

int Pathfinding::CalculatorHeuristic(sf::Vector2i _node, sf::Vector2i _endingNode)
{
	// Manhattan mathematic formula
	return (int)(abs(_node.x - _endingNode.x) + abs(_node.y - _endingNode.y)) * 10;
}

int Pathfinding::CalculatorMovementCost(int _parentCost, bool isDiagonal)
{
	// If the node is in the diagonale, we add 14, else we add 10 at the parent node cost
	return isDiagonal ? _parentCost + 14 : _parentCost + 10;
}

void Pathfinding::AddFirstNodeToList(sf::Vector2i _mapPosition)
{
	LinkedListClass::sElement* firstNode = new LinkedListClass::sElement;
	firstNode->data = new Pathfinding::sPathfindingData;


	((Pathfinding::sPathfindingData *)firstNode->data)->parentLink = nullptr;

	// Save the position in map
	((Pathfinding::sPathfindingData *)firstNode->data)->mapPosition = _mapPosition;


	// En fonction de la période actuelle, mettre l'annuel state concerné en conséquence
	((Pathfinding::sPathfindingData *)firstNode->data)->heuristic = 99999;
	((Pathfinding::sPathfindingData *)firstNode->data)->movementCost = RESET;

	((Pathfinding::sPathfindingData *)firstNode->data)->nodeTotalValue = ((Pathfinding::sPathfindingData *)firstNode->data)->heuristic + ((Pathfinding::sPathfindingData *)firstNode->data)->movementCost;
	((Pathfinding::sPathfindingData *)firstNode->data)->status = PATH_OPEN;

	firstNode->status = ELEMENT_ACTIVE;
	//std::cout << "First Pos : " << ((Pathfinding::sPathfindingData *)firstNode->data)->mapPosition.x << " " << ((Pathfinding::sPathfindingData *)firstNode->data)->mapPosition.y << std::endl;

	// Add this new vine at the end of the list
	this->AddElementToLinkedList(this->openList, firstNode, 1);
}


void Pathfinding::AddNewNodeToList(LinkedListClass::sElement *_parentLink, sf::Vector2i _mapPosition, int _parentMovementCost, bool _isDiag)
{
	LinkedListClass::sElement* newNode = new LinkedListClass::sElement;
	newNode->data = new Pathfinding::sPathfindingData;


	((Pathfinding::sPathfindingData *)newNode->data)->parentLink = _parentLink;

	// Save the position in map
	((Pathfinding::sPathfindingData *)newNode->data)->mapPosition = _mapPosition;


	// En fonction de la période actuelle, mettre l'annuel state concerné en conséquence
	((Pathfinding::sPathfindingData *)newNode->data)->heuristic = this->CalculatorHeuristic(_mapPosition, this->endPosition);
	((Pathfinding::sPathfindingData *)newNode->data)->movementCost = this->CalculatorMovementCost(_parentMovementCost, _isDiag);

	((Pathfinding::sPathfindingData *)newNode->data)->nodeTotalValue = ((Pathfinding::sPathfindingData *)newNode->data)->heuristic + ((Pathfinding::sPathfindingData *)newNode->data)->movementCost;
	((Pathfinding::sPathfindingData *)newNode->data)->status = PATH_OPEN;

	newNode->status = ELEMENT_ACTIVE;

	// Add this new vine at the end of the list
	this->AddElementToLinkedList(this->openList, newNode, -1);
}


void Pathfinding::ModifyParentLink(LinkedListClass::sElement *_nodeToModify, LinkedListClass::sElement *_newParentLink)
{
	((Pathfinding::sPathfindingData *)_nodeToModify->data)->parentLink = _newParentLink;
}


void Pathfinding::FindLowestNodeValue()
{
	// On parcourt la liste des nodes pour trouver celui qui a la plus petite node value
	if (this->openList != nullptr)
	{
		if (this->openList->first != nullptr)
		{
			LinkedListClass::sElement *lowestValueAdress = this->openList->first;
			LinkedListClass::sElement *currentElement = this->openList->first;

			int lowestNodeTotalValue = ((Pathfinding::sPathfindingData *)currentElement->data)->nodeTotalValue;
			int lowestHeuristicValue = ((Pathfinding::sPathfindingData *)currentElement->data)->heuristic;

			bool isListIsEmpty = false;

			for (currentElement = this->openList->first; currentElement != NULL; currentElement = currentElement->next)
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
						// check le H le plus bas
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
				this->actualStatus = PATHFINDING_FIND_NO_WAY;
			}

			this->currentNode = lowestValueAdress;

			//std::cout << "Adress : " << this->currentNode << " " << ((Pathfinding::sPathfindingData *)this->currentNode->data)->heuristic << " " << ((Pathfinding::sPathfindingData *)this->currentNode->data)->movementCost << " " << ((Pathfinding::sPathfindingData *)this->currentNode->data)->nodeTotalValue << std::endl;
			//std::cout << "Map Position x : " << ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x << " & " << ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y << std::endl;
			//std::cout << "Parent adress : " << ((Pathfinding::sPathfindingData *)this->currentNode->data)->parentLink << std::endl << std::endl;

			((Pathfinding::sPathfindingData *)this->currentNode->data)->status = PATH_CLOSE;
		}
		else
		{
			//std::cout << "List : " << this->list->first << std::endl;
		}
	}
}


void Pathfinding::FindNodeArounds()
{
	if (this->actualStatus == PATHFINDING_SEARCH)
	{
		// We check every tiles arround the node targeted
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				if (!(x == 0 && y == 0))
				{
					if (((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x + x >= 0
						&& ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x + x < this->mapSize.x
						&& ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y + y >= 0
						&& ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y + y < this->mapSize.y)
					{
						if (this->testMap[(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y + y][(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x + x] == 0)
						{
							bool isDiagIsFree = true;

							if ((abs(x) + abs(y)) % 2 == 0)
							{
								if (x < 0)
								{
									if (y > 0)
									{
										if (this->testMap[(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y][(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x - 1] != 0
											&& this->testMap[(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y + 1][(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x] != 0)
										{
											isDiagIsFree = false;
											//std::cout << "Direction principales libres\n";
										}
									}
									else
									{
										if (this->testMap[(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y][(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x - 1] != 0
											&& this->testMap[(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y - 1][(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x] != 0)
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
										if (this->testMap[(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y][(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x + 1] != 0
											&& this->testMap[(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y + 1][(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x] != 0)
										{
											isDiagIsFree = false;
											//std::cout << "Direction principales libres\n";
										}
									}
									else
									{
										if (this->testMap[(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y][(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x + 1] != 0
											&& this->testMap[(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y - 1][(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x] != 0)
										{
											isDiagIsFree = false;
											//std::cout << "Direction principales libres\n";
										}
									}
								}
							}

							if (isDiagIsFree)
							{
								LinkedListClass::sElement *currentElement = this->openList->first;
								bool isAlreadyExist = false;

								for (currentElement = this->openList->first; currentElement != NULL; currentElement = currentElement->next)
								{
									if ((((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x + x == ((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition.x)
										&& (((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y + y == ((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition.y))
									{
										isAlreadyExist = true;

										// If the direction sent is horizontal or verticale, we send false at the end of the AddNewNodeToList function's
										if ((abs(x) + abs(y)) % 2 == 1)
										{
											if (((Pathfinding::sPathfindingData *)currentElement->data)->movementCost >
												CalculatorMovementCost(((Pathfinding::sPathfindingData *)this->currentNode->data)->movementCost, false))
											{
												//std::cout << "HEREEEE : " << ((Pathfinding::sPathfindingData *)currentElement->data)->movementCost << " & " << CalculatorMovementCost(((Pathfinding::sPathfindingData *)this->currentNode->data)->movementCost, false) << std::endl;

												ModifyParentLink(currentElement, this->currentNode);
											}
										}
										// If it's a diagonal direction sent, we send true at the end of the AddNewNodeToList function's
										else
										{
											if (((Pathfinding::sPathfindingData *)currentElement->data)->movementCost >
												CalculatorMovementCost(((Pathfinding::sPathfindingData *)this->currentNode->data)->movementCost, true))
											{
												//std::cout << "HEREEEE : " << ((Pathfinding::sPathfindingData *)currentElement->data)->movementCost << " & " << CalculatorMovementCost(((Pathfinding::sPathfindingData *)this->currentNode->data)->movementCost, true) << std::endl;

												ModifyParentLink(currentElement, this->currentNode);
											}
										}
									}
								}

								if (!isAlreadyExist)
								{
									// If the direction sent is horizontal or verticale, we send false at the end of the AddNewNodeToList function's
									if ((abs(x) + abs(y)) % 2 == 1)
									{
										AddNewNodeToList(this->currentNode, sf::Vector2i(((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x + x, ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y + y), ((Pathfinding::sPathfindingData *)this->currentNode->data)->movementCost, false);
									}
									// If it's a diagonal direction sent, we send true at the end of the AddNewNodeToList function's
									else
									{
										AddNewNodeToList(this->currentNode, sf::Vector2i(((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x + x, ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y + y), ((Pathfinding::sPathfindingData *)this->currentNode->data)->movementCost, true);
									}
								}
							}
						}
						else if (this->testMap[(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y + y][(int)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x + x] == ENDING_TILE)
						{
							this->actualStatus = PATHFINDING_FIND_WAY_FIRST;
							int counter(1);

							// If the direction sent is horizontal or verticale, we send false at the end of the AddNewNodeToList function's
							if ((abs(x) + abs(y)) % 2 == 1)
							{
								AddNewNodeToList(this->currentNode, sf::Vector2i(((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x + x, ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y + y), ((Pathfinding::sPathfindingData *)this->currentNode->data)->movementCost, false);
							}
							// If it's a diagonal direction sent, we send true at the end of the AddNewNodeToList function's
							else
							{
								AddNewNodeToList(this->currentNode, sf::Vector2i(((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x + x, ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y + y), ((Pathfinding::sPathfindingData *)this->currentNode->data)->movementCost, true);
							}


							LinkedListClass::sElement *currentElement = this->openList->first;

							currentElement = this->openList->last;

							if (((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition.x == this->endPosition.x
								&& ((Pathfinding::sPathfindingData *)currentElement->data)->mapPosition.y == this->endPosition.y)
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


								LinkedListClass::sElement *finalElement = this->openList->first;

								for (finalElement = this->openList->first; finalElement != NULL; finalElement = finalElement->next)
								{
									if (((Pathfinding::sPathfindingData *)finalElement->data)->status != PATH_FINAL)
									{
										finalElement->status = ELEMENT_DELETION_REQUIRED;
									}
								}

								RemoveElementsOfLinkedList(this->openList);
							}
							else
							{
								// ERROR LOG à ajouter
								std::cout << "ERREUR PATHFINDING\n";
							}

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
	this->isActive = true;
}

void Pathfinding::MainStatePathfinding()
{
	// First initialisation
	if (this->actualStatus == PATHFINDING_INITIALISATION)
	{
		// Init of the linkedList
		//this->openList = LinkedListInitialisation();

		// Init of the start and ending position
		this->testMap[this->startPosition.y][this->startPosition.x] = STARTING_TILE;
		this->testMap[this->endPosition.y][this->endPosition.x] = ENDING_TILE;
		

		// TEMPORARY
		/*this->testMap[3][17] = OBSTACLE_TILE;
		this->testMap[4][17] = OBSTACLE_TILE;
		this->testMap[3][18] = OBSTACLE_TILE;
		this->testMap[5][17] = OBSTACLE_TILE;

		this->testMap[4][9] = OBSTACLE_TILE;
		this->testMap[3][12] = OBSTACLE_TILE;
		this->testMap[4][12] = OBSTACLE_TILE;
		this->testMap[5][12] = OBSTACLE_TILE;
		this->testMap[6][12] = OBSTACLE_TILE;
		this->testMap[7][12] = OBSTACLE_TILE;
		this->testMap[8][12] = OBSTACLE_TILE;

		this->testMap[2][10] = OBSTACLE_TILE;
		this->testMap[3][10] = OBSTACLE_TILE;
		this->testMap[4][10] = OBSTACLE_TILE;
		this->testMap[5][10] = OBSTACLE_TILE;*/

		// Add the first node with the starting position entered
		AddFirstNodeToList(this->startPosition);

		this->actualStatus = PATHFINDING_SEARCH;
	}
	


	while (this->actualStatus == PATHFINDING_SEARCH)
	{
		this->FindLowestNodeValue();
		
		this->FindNodeArounds();
	}

	// Step by step
	/*if (this->isActive == true)
	{
		this->isActive = false;

		this->FindLowestNodeValue();

		this->FindNodeArounds();
	}*/
}


void Pathfinding::WalkProcess(sf::Vector2f *_workerPosition, const float &_speed)
{
	if (this->actualStatus == PATHFINDING_FIND_WAY_FIRST)
	{
		// Set le premier élément à la fin de la découverte du chemin ?
		this->currentNode = this->openList->first;

		_workerPosition->x = (float)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x;
		_workerPosition->y = (float)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y;

		//std::cout << "First Pos : " << ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x << " " << ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y << std::endl;
		
		this->actualStatus = PATHFINDING_FIND_WAY;
	}
	else if (this->actualStatus == PATHFINDING_FIND_WAY)
	{
		if (this->currentNode->next != nullptr)
		{
			LinkedListClass::sElement *nextElement = this->currentNode->next;

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

			this->elapsedTime += _speed;

			if (this->elapsedTime >= 1
				&& DistanceFormula(*(_workerPosition), (sf::Vector2f)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition) < 1.45f)
			{
				this->elapsedTime = RESET;

				this->currentNode = this->currentNode->next;

				//std::cout << "Pos : " << ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x << " " << ((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y << std::endl;
				_workerPosition->x = (float)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.x;
				_workerPosition->y = (float)((Pathfinding::sPathfindingData *)this->currentNode->data)->mapPosition.y;
			}
		}
		else
		{
			this->actualStatus = PATHFINDING_NEED_TO_BE_DELETED;
		}
	}
}


enum PathfindingGeneralProcessStatus Pathfinding::GetActualStatus()
{
	return this->actualStatus;
}