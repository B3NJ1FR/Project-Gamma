#include "TimeManagement.h"


TimeManagement::TimeManagement()
{
	
}


TimeManagement::~TimeManagement()
{

}

void TimeManagement::Initialisation(sf::Font* _font, const sf::Vector2i& _screenResolution)
{
	m_gameClock.restart();

	m_frameTime = m_gameClock.getElapsedTime();
	m_timer += m_frameTime;

	m_actualMonth = IANUARIUS;
	m_numberOfYears = RESET;

	m_isMonthHasChanged = false;
	m_isYearHasChanged = true;

	sf::Color colorActivated = { 236, 150, 55, 255 };

	LoadTextString(&m_monthText, "IANUARIUS", _font, 35, colorActivated, sf::Vector2f(_screenResolution.x - 280, 25));

	LoadTextString(&yearText, "", _font, 35, colorActivated, sf::Vector2f(_screenResolution.x - 350, 25));

	m_timesSprite[GAME_PAUSE] = LoadSprite("Data/Assets/Sprites/UI/TimeManagement/pause.png", 1);
	m_timesSprite[GAME_NORMAL_SPEED] = LoadSprite("Data/Assets/Sprites/UI/TimeManagement/one.png", 1);
	m_timesSprite[GAME_DOUBLE_SPEED] = LoadSprite("Data/Assets/Sprites/UI/TimeManagement/two.png", 1);
	m_timesSprite[GAME_TRIPLE_SPEED] = LoadSprite("Data/Assets/Sprites/UI/TimeManagement/three.png", 1);

	m_sundial = LoadSprite("Data/Assets/Sprites/UI/TimeManagement/sundial.png", 1);
	m_sundialArrowIndicator = LoadSprite("Data/Assets/Sprites/UI/TimeManagement/arrow.png", 2);

	SetTypeOfAcceleration(GAME_NORMAL_SPEED);
}



TimeManagement* TimeManagement::GetSingleton()
{
	static TimeManagement uniqueInstance;
	return &uniqueInstance;
}


void TimeManagement::UpdateFrameTime()
{	
	m_frameTime = m_gameClock.restart();
}

void TimeManagement::UpdateGeneralTime()
{
	if (m_accelerator == 1)
	{
		m_timer += m_frameTime;
	}
	else
	{
		for (int i = 0; i < m_accelerator; i++)
		{
			m_timer += m_frameTime;
		}
	}
	
	//std::cout << "Time : " << timer.asSeconds() << std::endl;

	// Actualisation of the years
	// X MINUTES * X MOIS
	if (m_timer.asSeconds() >= TEMPORARY_TIME * (12)
		&& m_actualMonth == DECEMBER)
	{
		m_isYearHasChanged = true;

		m_numberOfYears++;

		m_timer -= sf::seconds(TEMPORARY_TIME * (12));
	}
	
	// Actualisation of the current month if he has changed
	if (m_actualMonth != (int)(m_timer.asSeconds() / TEMPORARY_TIME))
	{
		m_isMonthHasChanged = true;

		m_actualMonth = (enum MonthsInOneYear)(int)(m_timer.asSeconds() / TEMPORARY_TIME);
	}
}



void TimeManagement::SetAccelerator(const short int& _accelerator)
{
	m_accelerator = _accelerator;
}

void TimeManagement::SetTypeOfAcceleration(const enum TypeOfTimeAcceleration &_typeOfAcceleration)
{
	m_typeOfAcceleration = _typeOfAcceleration;

	sf::Color colorActivated = { 236, 150, 55, 255 };
	sf::Color colorDesactivated = { 253, 248, 235, 255 };

	switch (m_typeOfAcceleration)
	{
	case GAME_PAUSE:
		m_timesSprite[GAME_PAUSE].setColor(colorActivated);
		m_timesSprite[GAME_NORMAL_SPEED].setColor(colorDesactivated);
		m_timesSprite[GAME_DOUBLE_SPEED].setColor(colorDesactivated);
		m_timesSprite[GAME_TRIPLE_SPEED].setColor(colorDesactivated);

		m_accelerator = 0;

		break;
	case GAME_NORMAL_SPEED:
		m_timesSprite[GAME_PAUSE].setColor(colorDesactivated);
		m_timesSprite[GAME_NORMAL_SPEED].setColor(colorActivated);
		m_timesSprite[GAME_DOUBLE_SPEED].setColor(colorDesactivated);
		m_timesSprite[GAME_TRIPLE_SPEED].setColor(colorDesactivated);

		m_accelerator = 1;

		break;
	case GAME_DOUBLE_SPEED:
		m_timesSprite[GAME_PAUSE].setColor(colorDesactivated);
		m_timesSprite[GAME_NORMAL_SPEED].setColor(colorDesactivated);
		m_timesSprite[GAME_DOUBLE_SPEED].setColor(colorActivated);
		m_timesSprite[GAME_TRIPLE_SPEED].setColor(colorDesactivated);

		m_accelerator = 3;

		break;
	case GAME_TRIPLE_SPEED:
		m_timesSprite[GAME_PAUSE].setColor(colorDesactivated);
		m_timesSprite[GAME_NORMAL_SPEED].setColor(colorDesactivated);
		m_timesSprite[GAME_DOUBLE_SPEED].setColor(colorDesactivated);
		m_timesSprite[GAME_TRIPLE_SPEED].setColor(colorActivated);

		m_accelerator = 5;

		break;
	default:
		break;
	}
}




void TimeManagement::InputTimeManagement(sf::RenderWindow &_window, const sf::Vector2i& _screenResolution)
{
	sf::Vector2i mousePostionAtScreen = sf::Mouse::getPosition(_window);

	// Button New Game
	if (mousePostionAtScreen.x > _screenResolution.x - 695 - (m_timesSprite[GAME_PAUSE].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.x < _screenResolution.x - 695 + (m_timesSprite[GAME_PAUSE].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.y > 45 - (m_timesSprite[GAME_PAUSE].getGlobalBounds().height / 2)
		&& mousePostionAtScreen.y < 45 + (m_timesSprite[GAME_PAUSE].getGlobalBounds().height / 2))
	{
		SetTypeOfAcceleration(GAME_PAUSE);
	}
	// Button Continue
	else if (mousePostionAtScreen.x > _screenResolution.x - 645 - (m_timesSprite[GAME_NORMAL_SPEED].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.x < _screenResolution.x - 645 + (m_timesSprite[GAME_NORMAL_SPEED].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.y > 45 - (m_timesSprite[GAME_NORMAL_SPEED].getGlobalBounds().height / 2)
		&& mousePostionAtScreen.y < 45 + (m_timesSprite[GAME_NORMAL_SPEED].getGlobalBounds().height / 2))
	{
		SetTypeOfAcceleration(GAME_NORMAL_SPEED);
	}
	// Button Options	
	else if (mousePostionAtScreen.x > _screenResolution.x - 595 - (m_timesSprite[GAME_DOUBLE_SPEED].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.x < _screenResolution.x - 595 + (m_timesSprite[GAME_DOUBLE_SPEED].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.y > 45 - (m_timesSprite[GAME_DOUBLE_SPEED].getGlobalBounds().height / 2)
		&& mousePostionAtScreen.y < 45 + (m_timesSprite[GAME_DOUBLE_SPEED].getGlobalBounds().height / 2))
	{
		SetTypeOfAcceleration(GAME_DOUBLE_SPEED);
	}
	// Button Quit
	else if (mousePostionAtScreen.x > _screenResolution.x - 535 - (m_timesSprite[GAME_TRIPLE_SPEED].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.x < _screenResolution.x - 535 + (m_timesSprite[GAME_TRIPLE_SPEED].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.y > 45 - (m_timesSprite[GAME_TRIPLE_SPEED].getGlobalBounds().height / 2)
		&& mousePostionAtScreen.y < 45 + (m_timesSprite[GAME_TRIPLE_SPEED].getGlobalBounds().height / 2))
	{
		SetTypeOfAcceleration(GAME_TRIPLE_SPEED);
	}
}

void TimeManagement::UpdateMonthToDisplay()
{
	if (m_isMonthHasChanged == true)
	{
		m_isMonthHasChanged = false;

		switch (m_actualMonth)
		{
		case IANUARIUS:
			m_monthText.setString("IANUARIUS");
			break;
		case FEBRUARIUS:
			m_monthText.setString("FEBRUARIUS");
			break;
		case MARTIUS:
			m_monthText.setString("MARTIUS");
			break;
		case APRILIS:
			m_monthText.setString("APRILIS");
			break;
		case MAïUS:
			m_monthText.setString("MAïUS");
			break;
		case IUNIUS:
			m_monthText.setString("IUNIUS");
			break;
		case QUINTILIS:
			m_monthText.setString("QUINTILIS");
			break;
		case SEXTILIS:
			m_monthText.setString("SEXTILIS");
			break;
		case SEPTEMBER:
			m_monthText.setString("SEPTEMBER");
			break;
		case OCTOBER:
			m_monthText.setString("OCTOBER");
			break;
		case NOVEMBER:
			m_monthText.setString("NOVEMBER");
			break;
		case DECEMBER:
			m_monthText.setString("DECEMBER");
			break;
		default:
			break;
		}
	}

	if (m_isYearHasChanged == true)
	{
		m_isYearHasChanged = false;

		UpdateDynamicsTexts(&yearText, m_numberOfYears);
	}
}

void TimeManagement::DisplayUITime(sf::RenderWindow &_window, const sf::Vector2i &_screenResolution)
{
	BlitString(m_monthText, _window);
	BlitString(yearText, _window);
		
	BlitSprite(m_timesSprite[GAME_PAUSE], _screenResolution.x - 695, 45, 0, _window);
	BlitSprite(m_timesSprite[GAME_NORMAL_SPEED], _screenResolution.x - 645, 45, 0, _window);
	BlitSprite(m_timesSprite[GAME_DOUBLE_SPEED], _screenResolution.x - 595, 45, 0, _window);
	BlitSprite(m_timesSprite[GAME_TRIPLE_SPEED], _screenResolution.x - 535, 45, 0, _window);

	BlitSprite(m_sundial, _screenResolution.x - 435, 45, 0, _window);
	BlitSprite(m_sundialArrowIndicator, _screenResolution.x - 435, 30, (int)(m_timer.asSeconds() * 3.75f) % (int)(TEMPORARY_TIME * 3.75f) - 20.0f, _window);
}



void TimeManagement::SavingTimeFromFile(std::ofstream *_file)
{
	// Save the timer
	_file->write((char *)&m_timer, sizeof(sf::Time));

	// Save the calendar
	_file->write((char *)&m_actualMonth, sizeof(enum MonthsInOneYear));
	_file->write((char *)&m_numberOfYears, sizeof(int));
}


void TimeManagement::LoadingTimeFromFile(std::ifstream *_file)
{
	// Load the timer
	_file->read((char *)&m_timer, sizeof(sf::Time));

	// Load the calendar
	_file->read((char *)&m_actualMonth, sizeof(enum MonthsInOneYear));
	_file->read((char *)&m_numberOfYears, sizeof(int));
	

	// We update the months and years display
	m_isMonthHasChanged = true;
	m_isYearHasChanged = true;

	UpdateMonthToDisplay();
}