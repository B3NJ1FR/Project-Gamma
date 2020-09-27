#ifndef TIME_MANAGEMENT__H
#define TIME_MANAGEMENT__H

#include "Common.hpp"

#define TEMPORARY_TIME 60
#define OLD_TIME (2 * 3600)

enum MonthsInOneYear
{
	IANUARIUS,
	FEBRUARIUS,
	MARTIUS,
	APRILIS,
	MA�US,
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
//	Aprilis : (avril) 30 jours d�di�s � la d�esse grecque Aphrodite, et d�signant l'ouverture de l'ann�e
//	Ma�us : (mai) 31 jours en l'honneur des s�nateurs romains ou maiores
//	Iunius : (juin) 30 jours en l'honneur de la d�esse romaine Junon.
//	Quintilis : (juillet) 31 jours qui deviendra juillet en l'honneur Jules C�sar.
//	Sextilis : (ao�t) 31 jours qui deviendra ao�t en l'honneur de l'empereur Auguste.
//	September : (septembre) 30 jours
//	October : (octobre) 31 jours
//	November : (novembre) 30 jours
//	December : (d�cembre) 31 jours

enum TypeOfTimeAcceleration
{
	GAME_PAUSE,
	GAME_NORMAL_SPEED,
	GAME_DOUBLE_SPEED,
	GAME_TRIPLE_SPEED,
};

class TimeManagement
{
private:
	sf::Clock gameClock;
	sf::Time frameTime;
	sf::Time timer;

	bool isMonthHasChanged;
	bool isYearHasChanged;

	enum MonthsInOneYear actualMonth;
	int numberOfYears;

	sf::Text monthText;
	sf::Text yearText;

	short int accelerator;
	enum TypeOfTimeAcceleration typeOfAcceleration;

	sf::Sprite timesSprite[4];

public:
	TimeManagement(sf::Font *_font);
	~TimeManagement();
	
	void UpdateFrameTime();
	void UpdateGeneralTime();

	short int GetAccelerator();
	enum TypeOfTimeAcceleration GetTypeOfAcceleration();
	float GetFrameTime();
	float GetContinuousFrameTime();
	float GetGeneralTime();
	enum MonthsInOneYear GetActualMonth();

	void SetAccelerator(const short int &_accelerator);
	void SetTypeOfAcceleration(const enum TypeOfTimeAcceleration &_typeOfAcceleration);

	void InputTimeManagement(sf::RenderWindow &_window);
	void UpdateMonthToDisplay();
	void DisplayUITime(sf::RenderWindow &_window);

	void SavingTimeFromFile(std::ofstream *_file);
	void LoadingTimeFromFile(std::ifstream *_file);
};


#endif // !TIME_MANAGEMENT__H
