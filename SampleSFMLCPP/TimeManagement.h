#ifndef TIME_MANAGEMENT__H
#define TIME_MANAGEMENT__H

#include "Common.hpp"

enum MonthsInOneYear
{
	IANUARIUS,
	FEBRUARIUS,
	MARTIUS,
	APRILIS,
	MAïUS,
	IUNIUS,
	QUINTILIS,
	SEXTILIS,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER,
};

//	Ianuarius : (mars) 31 jours en l'honneur du dieu romain Janus
//	Februarius : (mars) 28 jours 
//	Martius : (mars) 31 jours en l'honneur du dieu romain Mars
//	Aprilis : (avril) 30 jours dédiés à la déesse grecque Aphrodite, et désignant l'ouverture de l'année
//	Maïus : (mai) 31 jours en l'honneur des sénateurs romains ou maiores
//	Iunius : (juin) 30 jours en l'honneur de la déesse romaine Junon.
//	Quintilis : (juillet) 31 jours qui deviendra juillet en l'honneur Jules César.
//	Sextilis : (août) 31 jours qui deviendra août en l'honneur de l'empereur Auguste.
//	September : (septembre) 30 jours
//	October : (octobre) 31 jours
//	November : (novembre) 30 jours
//	December : (décembre) 31 jours

class TimeManagement
{
private:
	sf::Clock gameClock;
	sf::Time frameTime;
	sf::Time timer;

	bool isMonthHasChanged;

	enum MonthsInOneYear actualMonth;
	int numberOfYears;

	sf::Text monthText;
	sf::Text yearText;

public:
	TimeManagement(sf::Font *_font);
	~TimeManagement();
	
	void UpdateFrameTime();
	void UpdateGeneralTime();

	float GetFrameTime();
	float GetGeneralTime();

	void UpdateMonthToDisplay();
	void DisplayUITime(sf::RenderWindow &_window);
};


#endif // !TIME_MANAGEMENT__H
