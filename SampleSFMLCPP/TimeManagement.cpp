#include "TimeManagement.h"


TimeManagement::TimeManagement()
{
	this->gameClock.restart();

	this->frameTime = gameClock.getElapsedTime();
	this->timer += this->frameTime;

	this->actualMonth = IANUARIUS;
	this->numberOfYears = RESET;
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
	
	std::cout << "Time : " << this->timer.asSeconds() << std::endl;

	// Actualisation of the years
	// X MINUTES * X MOIS
	if (this->timer.asSeconds() >= (2 * 3600) * (12))
	{
		this->numberOfYears++;

		//this->timer -= (sf::Time::asSeconds)((2 * 3600) * (12));
	}
	
	// Actualisation of the current month
	//this->actualMonth = (enum MonthsInOneYear)(this->timer.asSeconds() / (2 * 3600));
}

float TimeManagement::GetFrameTime()
{
	return this->frameTime.asSeconds();
}

float TimeManagement::GetGeneralTime()
{
	return this->timer.asSeconds();
}

