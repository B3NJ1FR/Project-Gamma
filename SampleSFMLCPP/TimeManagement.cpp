#include "TimeManagement.h"


TimeManagement::TimeManagement(sf::Font *_font)
{
	this->gameClock.restart();

	this->frameTime = gameClock.getElapsedTime();
	this->timer += this->frameTime;

	this->actualMonth = IANUARIUS;
	this->numberOfYears = RESET;
	
	this->isMonthHasChanged = false;
	this->isYearHasChanged = true;

	sf::Color colorActivated = { 236, 150, 55, 255 };

	LoadTextString(&this->monthText, "IANUARIUS", _font, 40, colorActivated, sf::Vector2f(1920 - 300, 20));

	LoadTextString(&this->yearText, "", _font, 40, colorActivated, sf::Vector2f(1920 - 370, 20));

	this->timesSprite[GAME_PAUSE] = LoadSprite("Data/Assets/Sprites/UI/TimeManagement/pause.png", 1);
	this->timesSprite[GAME_NORMAL_SPEED] = LoadSprite("Data/Assets/Sprites/UI/TimeManagement/one.png", 1);
	this->timesSprite[GAME_DOUBLE_SPEED] = LoadSprite("Data/Assets/Sprites/UI/TimeManagement/two.png", 1);
	this->timesSprite[GAME_TRIPLE_SPEED] = LoadSprite("Data/Assets/Sprites/UI/TimeManagement/three.png", 1);

	this->SetTypeOfAcceleration(GAME_NORMAL_SPEED);
}


TimeManagement::~TimeManagement()
{

}

void TimeManagement::UpdateFrameTime()
{	
	this->frameTime = this->gameClock.restart();
}

void TimeManagement::UpdateGeneralTime()
{
	if (this->accelerator)
	{
		this->timer += this->frameTime;
	}
	else
	{
		for (int i = 0; i < this->accelerator; i++)
		{
			this->timer += this->frameTime;
		}
	}
	
	//std::cout << "Time : " << this->timer.asSeconds() << std::endl;

	// Actualisation of the years
	// X MINUTES * X MOIS
	if (this->timer.asSeconds() >= TEMPORARY_TIME * (12)
		&& this->actualMonth == DECEMBER)
	{
		this->isYearHasChanged = true;

		this->numberOfYears++;

		this->timer -= sf::seconds(TEMPORARY_TIME * (12));
	}
	
	// Actualisation of the current month if he has changed
	if (this->actualMonth != (int)(this->timer.asSeconds() / TEMPORARY_TIME))
	{
		this->isMonthHasChanged = true;

		this->actualMonth = (enum MonthsInOneYear)(int)(this->timer.asSeconds() / TEMPORARY_TIME);
	}
}
short int TimeManagement::GetAccelerator()
{
	return this->accelerator;
}

enum TypeOfTimeAcceleration TimeManagement::GetTypeOfAcceleration()
{
	return this->typeOfAcceleration;
}

float TimeManagement::GetFrameTime()
{
	return this->frameTime.asSeconds() * this->accelerator;
}

float TimeManagement::GetContinuousFrameTime()
{
	return this->frameTime.asSeconds();
}

float TimeManagement::GetGeneralTime()
{
	return this->timer.asSeconds();
}

enum MonthsInOneYear TimeManagement::GetActualMonth()
{
	return this->actualMonth;
}




void TimeManagement::SetTypeOfAcceleration(const enum TypeOfTimeAcceleration &_typeOfAcceleration)
{
	this->typeOfAcceleration = _typeOfAcceleration;

	sf::Color colorActivated = { 236, 150, 55, 255 };
	sf::Color colorDesactivated = { 253, 248, 235, 255 };

	switch (this->typeOfAcceleration)
	{
	case GAME_PAUSE:
		this->timesSprite[GAME_PAUSE].setColor(colorActivated);
		this->timesSprite[GAME_NORMAL_SPEED].setColor(colorDesactivated);
		this->timesSprite[GAME_DOUBLE_SPEED].setColor(colorDesactivated);
		this->timesSprite[GAME_TRIPLE_SPEED].setColor(colorDesactivated);

		this->accelerator = 0;

		break;
	case GAME_NORMAL_SPEED:
		this->timesSprite[GAME_PAUSE].setColor(colorDesactivated);
		this->timesSprite[GAME_NORMAL_SPEED].setColor(colorActivated);
		this->timesSprite[GAME_DOUBLE_SPEED].setColor(colorDesactivated);
		this->timesSprite[GAME_TRIPLE_SPEED].setColor(colorDesactivated);

		this->accelerator = 1;

		break;
	case GAME_DOUBLE_SPEED:
		this->timesSprite[GAME_PAUSE].setColor(colorDesactivated);
		this->timesSprite[GAME_NORMAL_SPEED].setColor(colorDesactivated);
		this->timesSprite[GAME_DOUBLE_SPEED].setColor(colorActivated);
		this->timesSprite[GAME_TRIPLE_SPEED].setColor(colorDesactivated);

		this->accelerator = 3;

		break;
	case GAME_TRIPLE_SPEED:
		this->timesSprite[GAME_PAUSE].setColor(colorDesactivated);
		this->timesSprite[GAME_NORMAL_SPEED].setColor(colorDesactivated);
		this->timesSprite[GAME_DOUBLE_SPEED].setColor(colorDesactivated);
		this->timesSprite[GAME_TRIPLE_SPEED].setColor(colorActivated);

		this->accelerator = 5;

		break;
	default:
		break;
	}
}

void TimeManagement::SetAccelerator(const short int &_accelerator)
{
	this->accelerator = _accelerator;
}




void TimeManagement::InputTimeManagement(sf::RenderWindow &_window)
{
	sf::Vector2i mousePostionAtScreen = sf::Mouse::getPosition(_window);

	// Button New Game
	if (mousePostionAtScreen.x > 1920 - 595 - (timesSprite[GAME_PAUSE].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.x < 1920 - 595 + (timesSprite[GAME_PAUSE].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.y > 45 - (timesSprite[GAME_PAUSE].getGlobalBounds().height / 2)
		&& mousePostionAtScreen.y < 45 + (timesSprite[GAME_PAUSE].getGlobalBounds().height / 2))
	{
		this->SetTypeOfAcceleration(GAME_PAUSE);
	}
	// Button Continue
	else if (mousePostionAtScreen.x > 1920 - 545 - (this->timesSprite[GAME_NORMAL_SPEED].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.x < 1920 - 545 + (this->timesSprite[GAME_NORMAL_SPEED].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.y > 45 - (this->timesSprite[GAME_NORMAL_SPEED].getGlobalBounds().height / 2)
		&& mousePostionAtScreen.y < 45 + (this->timesSprite[GAME_NORMAL_SPEED].getGlobalBounds().height / 2))
	{
		this->SetTypeOfAcceleration(GAME_NORMAL_SPEED);
	}
	// Button Options	
	else if (mousePostionAtScreen.x > 1920 - 495 - (this->timesSprite[GAME_DOUBLE_SPEED].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.x < 1920 - 495 + (this->timesSprite[GAME_DOUBLE_SPEED].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.y > 45 - (this->timesSprite[GAME_DOUBLE_SPEED].getGlobalBounds().height / 2)
		&& mousePostionAtScreen.y < 45 + (this->timesSprite[GAME_DOUBLE_SPEED].getGlobalBounds().height / 2))
	{
		this->SetTypeOfAcceleration(GAME_DOUBLE_SPEED);
	}
	// Button Quit
	else if (mousePostionAtScreen.x > 1920 - 435 - (this->timesSprite[GAME_TRIPLE_SPEED].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.x < 1920 - 435 + (this->timesSprite[GAME_TRIPLE_SPEED].getGlobalBounds().width / 2)
		&& mousePostionAtScreen.y > 45 - (this->timesSprite[GAME_TRIPLE_SPEED].getGlobalBounds().height / 2)
		&& mousePostionAtScreen.y < 45 + (this->timesSprite[GAME_TRIPLE_SPEED].getGlobalBounds().height / 2))
	{
		this->SetTypeOfAcceleration(GAME_TRIPLE_SPEED);
	}
}

void TimeManagement::UpdateMonthToDisplay()
{
	if (this->isMonthHasChanged == true)
	{
		this->isMonthHasChanged = false;

		switch (this->actualMonth)
		{
		case IANUARIUS:
			this->monthText.setString("IANUARIUS");
			break;
		case FEBRUARIUS:
			this->monthText.setString("FEBRUARIUS");
			break;
		case MARTIUS:
			this->monthText.setString("MARTIUS");
			break;
		case APRILIS:
			this->monthText.setString("APRILIS");
			break;
		case MAïUS:
			this->monthText.setString("MAïUS");
			break;
		case IUNIUS:
			this->monthText.setString("IUNIUS");
			break;
		case QUINTILIS:
			this->monthText.setString("QUINTILIS");
			break;
		case SEXTILIS:
			this->monthText.setString("SEXTILIS");
			break;
		case SEPTEMBER:
			this->monthText.setString("SEPTEMBER");
			break;
		case OCTOBER:
			this->monthText.setString("OCTOBER");
			break;
		case NOVEMBER:
			this->monthText.setString("NOVEMBER");
			break;
		case DECEMBER:
			this->monthText.setString("DECEMBER");
			break;
		default:
			break;
		}
	}

	if (this->isYearHasChanged == true)
	{
		this->isYearHasChanged = false;

		UpdateDynamicsTexts(&this->yearText, this->numberOfYears);
	}
}

void TimeManagement::DisplayUITime(sf::RenderWindow &_window)
{
	BlitString(this->monthText, _window);
	BlitString(this->yearText, _window);
		
	BlitSprite(this->timesSprite[GAME_PAUSE], 1920 - 595, 45, 0, _window);
	BlitSprite(this->timesSprite[GAME_NORMAL_SPEED], 1920 - 545, 45, 0, _window);
	BlitSprite(this->timesSprite[GAME_DOUBLE_SPEED], 1920 - 495, 45, 0, _window);
	BlitSprite(this->timesSprite[GAME_TRIPLE_SPEED], 1920 - 435, 45, 0, _window);
}



void TimeManagement::SavingTimeFromFile(std::ofstream *_file)
{
	// Save the timer
	_file->write((char *)&this->timer, sizeof(sf::Time));

	// Save the calendar
	_file->write((char *)&this->actualMonth, sizeof(enum MonthsInOneYear));
	_file->write((char *)&this->numberOfYears, sizeof(int));
}


void TimeManagement::LoadingTimeFromFile(std::ifstream *_file)
{
	// Load the timer
	_file->read((char *)&this->timer, sizeof(sf::Time));

	// Load the calendar
	_file->read((char *)&this->actualMonth, sizeof(enum MonthsInOneYear));
	_file->read((char *)&this->numberOfYears, sizeof(int));
	

	// We update the months and years display
	this->isMonthHasChanged = true;
	this->isYearHasChanged = true;

	this->UpdateMonthToDisplay();
}