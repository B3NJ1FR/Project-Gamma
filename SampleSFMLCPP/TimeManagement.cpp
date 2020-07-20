#include "TimeManagement.h"


TimeManagement::TimeManagement()
{
	this->gameClock.restart();

	this->frameTime = gameClock.getElapsedTime();
	this->timer += this->frameTime;
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
}

float TimeManagement::GetFrameTime()
{
	return this->frameTime.asSeconds();
}

float TimeManagement::GetGeneralTime()
{
	return this->timer.asSeconds();
}

