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

	LoadTextString(&this->monthText, "IANUARIUS", _font, 40, sf::Color::Red, sf::Vector2f(1920 - 300, 90));

	LoadTextString(&this->yearText, "", _font, 40, sf::Color::Red, sf::Vector2f(1920 - 370, 90));
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
	this->timer += this->frameTime;
	
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

float TimeManagement::GetFrameTime()
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