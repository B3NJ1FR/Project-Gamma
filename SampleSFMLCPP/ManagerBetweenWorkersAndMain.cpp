#include "ManagerBetweenWorkersAndMain.h"

ManagerBetweenWorkersAndMain::ManagerBetweenWorkersAndMain(const sf::Vector2i& _screenResolution)
{
	m_portraitFrame = LoadSprite("Data/Assets/Sprites/UI/Manager/portraitFrame.png", sf::Vector2f(150, _screenResolution.y - 100), 1);
	m_changementLeftArrow = LoadSprite("Data/Assets/Sprites/UI/Manager/left_arrow.png", sf::Vector2f(m_portraitFrame.getPosition().x - SPACE_FROM_PORTRAIT_FRAME, m_portraitFrame.getPosition().y), 1);
	m_changementRightArrow = LoadSprite("Data/Assets/Sprites/UI/Manager/right_arrow.png", sf::Vector2f(m_portraitFrame.getPosition().x + SPACE_FROM_PORTRAIT_FRAME, m_portraitFrame.getPosition().y), 1);

	m_valueNumberWorkerOrMainSelected = RESET;
}

ManagerBetweenWorkersAndMain::~ManagerBetweenWorkersAndMain()
{
	delete (m_changementLeftArrow.getTexture());
	delete (m_changementRightArrow.getTexture());
	delete (m_portraitFrame.getTexture());
}

void ManagerBetweenWorkersAndMain::CheckClickOnArrows(MainCharacter *_mainCharacter, WorkersList *_workersList, sf::RenderWindow& _window)
{
	sf::Vector2i mousePostionAtScreen = sf::Mouse::getPosition(_window);

	// Button New Game
	if (mousePostionAtScreen.x > m_changementLeftArrow.getPosition().x - (m_changementLeftArrow.getGlobalBounds().width / 2)
		&& mousePostionAtScreen.x <  m_changementLeftArrow.getPosition().x + (m_changementLeftArrow.getGlobalBounds().width / 2)
		&& mousePostionAtScreen.y > m_changementLeftArrow.getPosition().y - (m_changementLeftArrow.getGlobalBounds().height / 2)
		&& mousePostionAtScreen.y < m_changementLeftArrow.getPosition().y + (m_changementLeftArrow.getGlobalBounds().height / 2))
	{
		// PREVIOUS
		if (_workersList->GetNumberOfWorkers() != 0)
		{
			if (m_valueNumberWorkerOrMainSelected - 1 > 0)
			{
				m_valueNumberWorkerOrMainSelected -= 1;
				_mainCharacter->SetIsMainCharacterSelected(false);

				_workersList->ChangeWorkerNumberSelectedSubtract();
			}
			else if (m_valueNumberWorkerOrMainSelected - 1 == 0)
			{
				m_valueNumberWorkerOrMainSelected -= 1;
				_mainCharacter->SetIsMainCharacterSelected(true);
			}
			else if (m_valueNumberWorkerOrMainSelected - 1 < 0)
			{
				m_valueNumberWorkerOrMainSelected = _workersList->GetNumberOfWorkers();
				_mainCharacter->SetIsMainCharacterSelected(false);

				_workersList->ChangeWorkerNumberSelectedSubtract();
			}
		}
		else
		{
			_mainCharacter->SetIsMainCharacterSelected(true);
		}
	}
	else if (mousePostionAtScreen.x > m_changementRightArrow.getPosition().x - (m_changementRightArrow.getGlobalBounds().width / 2)
		&& mousePostionAtScreen.x <  m_changementRightArrow.getPosition().x + (m_changementRightArrow.getGlobalBounds().width / 2)
		&& mousePostionAtScreen.y > m_changementRightArrow.getPosition().y - (m_changementRightArrow.getGlobalBounds().height / 2)
		&& mousePostionAtScreen.y < m_changementRightArrow.getPosition().y + (m_changementRightArrow.getGlobalBounds().height / 2))
	{
		// NEXT
		if (_workersList->GetNumberOfWorkers() != 0)
		{
			if (m_valueNumberWorkerOrMainSelected + 1 <= _workersList->GetNumberOfWorkers())
			{
				m_valueNumberWorkerOrMainSelected += 1;
				_mainCharacter->SetIsMainCharacterSelected(false);

				_workersList->ChangeWorkerNumberSelectedAdd();
			}
			else if (m_valueNumberWorkerOrMainSelected + 1 > _workersList->GetNumberOfWorkers())
			{
				m_valueNumberWorkerOrMainSelected = 0;
				_mainCharacter->SetIsMainCharacterSelected(true);
			}
		}
		else
		{
			_mainCharacter->SetIsMainCharacterSelected(true);
		}
	}
}

void ManagerBetweenWorkersAndMain::CheckClickKeys(bool _isHigherRequired, MainCharacter* _mainCharacter, WorkersList* _workersList)
{
	// Button New Game
	if (!_isHigherRequired)
	{
		// PREVIOUS
		if (_workersList->GetNumberOfWorkers() != 0)
		{
			if (m_valueNumberWorkerOrMainSelected - 1 > 0)
			{
				m_valueNumberWorkerOrMainSelected -= 1;
				_mainCharacter->SetIsMainCharacterSelected(false);

				_workersList->ChangeWorkerNumberSelectedSubtract();
			}
			else if (m_valueNumberWorkerOrMainSelected - 1 == 0)
			{
				m_valueNumberWorkerOrMainSelected -= 1;
				_mainCharacter->SetIsMainCharacterSelected(true);
			}
			else if (m_valueNumberWorkerOrMainSelected - 1 < 0)
			{
				m_valueNumberWorkerOrMainSelected = _workersList->GetNumberOfWorkers();
				_mainCharacter->SetIsMainCharacterSelected(false);

				_workersList->ChangeWorkerNumberSelectedSubtract();
			}
		}
		else
		{
			_mainCharacter->SetIsMainCharacterSelected(true);
		}
	}
	else
	{
		// NEXT
		if (_workersList->GetNumberOfWorkers() != 0)
		{
			if (m_valueNumberWorkerOrMainSelected + 1 <= _workersList->GetNumberOfWorkers())
			{
				m_valueNumberWorkerOrMainSelected += 1;
				_mainCharacter->SetIsMainCharacterSelected(false);

				_workersList->ChangeWorkerNumberSelectedAdd();
			}
			else if (m_valueNumberWorkerOrMainSelected + 1 > _workersList->GetNumberOfWorkers())
			{
				m_valueNumberWorkerOrMainSelected = 0;
				_mainCharacter->SetIsMainCharacterSelected(true);
			}
		}
		else
		{
			_mainCharacter->SetIsMainCharacterSelected(true);
		}
	}
}

void ManagerBetweenWorkersAndMain::DisplayManagerBetweenWorkersAndMain(sf::RenderWindow& _window)
{
	// Display the two arrow and the portrait frame inside
	BlitSprite(m_changementLeftArrow, _window);
	BlitSprite(m_portraitFrame, _window);
	BlitSprite(m_changementRightArrow, _window);
}

void ManagerBetweenWorkersAndMain::DisplayInsidePortraitFrame(sf::Sprite _spriteIntoPortraitFrame, sf::RenderWindow& _window)
{
	BlitSprite(_spriteIntoPortraitFrame, m_portraitFrame.getPosition().x, m_portraitFrame.getPosition().y + 20, _window);
}