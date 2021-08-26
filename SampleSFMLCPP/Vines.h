#pragma once

#include "EnumInternalState.h"
#include "Storage.h"
#include "TimeManagement.h"
#include "Buildings.h"
#include "LinkedList.h"

#define VINE_LIFE_3Y_GROWTHING_TO_READY_TO_PRODUCE 12
#define VINE_LIFE_READY_TO_PRODUCE_TO_NOT_MAINTAINED


enum VineStateOfGeneralLife
{
	PLANTED,
	THREE_YEARS_GROWTHING,
	READY_TO_PRODUCE,
	NOT_MAINTAINED,
	TOO_OLD,
};

enum VineStateOfAnnualLife
{
	NEED_PRUNE, // Taille
	PRUNED,
	NEED_PLOUGHING, // Labourage
	PLOUGHED,
	NEED_WEEDING, // Désherbage
	WEEDED,
	NEED_CARE, // Entretien
	CARED,
	NEED_HARVEST, // Vendange
	HARVESTED,
	ROTTEN_HARVESTS,
	PUTTING_IN_WINE_STOREHOUSE, // Mise en Chai
};

// Calendrier des Vignes
//
//	-	Janvier - Taille de la vigne
//		Février - Préparer et bouturer
//	-	Mars - Labourer la terre
//		Avril - Planter et palisser
//	-	Mai - Désherber et Protéger du Mildiou et de l'oïdium 
//		Juin - Floraison
//	-	Juillet - Entretien
//		Août - Surveillance
//	-	Septembre - Vendanges
//	-	Octobre - Mise en Chai
//		Novembre - Préparer l'hiver
//		Décembre - Mise en repos de la vigne



class Vines : public LinkedListClass
{
public:

	struct VinesData
	{
		sf::Vector2f mapPosition;
		enum VineStateOfGeneralLife generalState;
		enum VineStateOfAnnualLife annualState;
		InternalState internalState;
		MonthsInOneYear previousMonth;

		Storage* storage = nullptr;

		float lifeTime;
		float actualProductionTime;
		float secondaryTime;

		bool isChangingSprite;
		bool hasBeenBuilt;
		bool isProduced;
		bool isWorkerThere;
		bool isProdCanBeCollected;

		bool isPruned;
		bool isPloughed;
		bool isWeeded;
		bool isCared;

		int numberOfWorkersNeededToWorks;
		int currentNumberOfWorkersPresent;
	};
	typedef struct VinesData sVines;

	// Constructor & Destructor
	Vines();
	~Vines();

	// Getters
	Storage* GetStorage(const sf::Vector2f& _mapPosition);

	void InitialisationVines(Buildings *_vine);
	void ReadVineLinkedList();
	void AddNewVineToList(sf::Vector2f _mapPosition);
	void UpdateVineLife();
	void UpdateVineSprite();
	void UpdateVineProduction();

	bool IsBuildingIsWorking(const sf::Vector2f& _mapPosition) const;
	bool ConfirmVinePresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_thisIsAWorker = false);
	void WorkerEnteringInThisPosition(const sf::Vector2f& _mapPosition);
	void WorkerLeavingThisPosition(const sf::Vector2f& _mapPosition);
	int GetNumberOfWorkersPresents(const sf::Vector2f& _mapPosition) const; 
	bool GetWorkerIsThere(const sf::Vector2f &_mapPosition);
	bool CheckVineHasProducedRessource(const sf::Vector2f &_mapPosition);
	bool CheckVineHasBeenBuilt(const sf::Vector2f &_mapPosition);
	bool UpdateRessourcePickuping(const sf::Vector2f &_mapPosition);
	void RessourcePickedUp(const sf::Vector2f &_mapPosition);
	int CalculateQtyBunchOfGrapeProduced(Vines::sVines* _vineConcerned, int _maximalQty);

	bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);
	void ClearStorages();

	void SavingVinesListForFile(std::ofstream *_file);
	void LoadingVinesListFromFile(std::ifstream *_file);

private:
	LinkedListClass::sLinkedList *m_list;
	Buildings *m_vineBuilding;
};