#include "TimeManagement.h"


TimeManagement::TimeManagement(sf::Font *_font)
{
	this->gameClock.restart();

	this->frameTime = gameClock.getElapsedTime();
	this->timer += this->frameTime;

	this->actualMonth = IANUARIUS;
	this->numberOfYears = RESET;
	

	LoadTextString(&this->monthText, "IANUARIUS", _font, 50, sf::Color::Black, sf::Vector2f(1920 - 200, 50));

	LoadTextString(&this->yearText, "", _font, 30, sf::Color::Black, sf::Vector2f(1920 - 50, 50));
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
	if (this->timer.asSeconds() >= (2 * 3600) * (12))
	{
		this->numberOfYears++;

		this->timer -= sf::seconds((2 * 3600) * (12));
	}
	
	// Actualisation of the current month if he has changed
	/*if (this->actualMonth != (enum MonthsInOneYear)(this->timer.asSeconds() / (2 * 3600)))
	{
		this->isMonthHasChanged = true;

		this->actualMonth = (enum MonthsInOneYear)(this->timer.asSeconds() / (2 * 3600));
	}*/
}

float TimeManagement::GetFrameTime()
{
	return this->frameTime.asSeconds();
}

float TimeManagement::GetGeneralTime()
{
	return this->timer.asSeconds();
}




void TimeManagement::UpdateMonthToDisplay()
{
	/*if (this->isMonthHasChanged == true)
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
	}*/
}

void TimeManagement::DisplayUITime(sf::RenderWindow &_window)
{
	//BlitString(this->monthText, _window);
	//BlitString(this->yearText, _window);
}
