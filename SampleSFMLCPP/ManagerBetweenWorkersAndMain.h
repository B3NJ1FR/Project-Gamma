#pragma once

#include "Common.hpp"
#include "MainCharacter.h"
#include "WorkersList.h"

#define SPACE_FROM_PORTRAIT_FRAME 70

class ManagerBetweenWorkersAndMain
{
private:
	sf::Sprite m_changementLeftArrow;
	sf::Sprite m_changementRightArrow;
	sf::Sprite m_portraitFrame;

	int m_valueNumberWorkerOrMainSelected;

public:
	// Constructor & Destructor
	ManagerBetweenWorkersAndMain(const sf::Vector2i& _screenResolution);
	~ManagerBetweenWorkersAndMain();


	// Methods
	void CheckClickOnArrows(MainCharacter* _mainCharacter, WorkersList* _workersList, sf::RenderWindow& _window);
	void CheckClickKeys(bool _isHigherRequired, MainCharacter* _mainCharacter, WorkersList* _workersList);
	void DisplayManagerBetweenWorkersAndMain(sf::RenderWindow& _window);
	void DisplayInsidePortraitFrame(sf::Sprite _spriteIntoPortraitFrame, sf::RenderWindow& _window);
};

