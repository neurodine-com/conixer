// --------------------------------------------------------------------------------------------------------
// Author:          Blazej Kaczmarek
// ORCID:           https://orcid.org/0009-0003-1070-8870
// email:           fajny.masz.tornister@gmail.com
// github ord email:MasterDisaster@neurodine.com
// domain email:    Blazej.Kaczmarek@neurodine.com
// oid-tree:        http://oid-info.com/get/1.3.6.1.4.1.53685
// PEN:             1.3.6.1.4.1.53685
// oid ASN.1:       {iso(1) identified-organization(3) dod(6) internet(1) private(4) enterprise(1) 53685}
// Description:     So here I am creating a structures that hold some lists/arrays of vectors. this 
// 		   vectors are divided in general into filed holding a:
// 			a.) location where the Conctare Comixer is exactly now, as a address on the board.
// 			b.) this filed is used to calculte all possible allowwed moves I can make
// 			c.) the are sorted in order to moves with "placing a bomb" and moves without it
// 			d.) additionally the most effective move is calcutated - this is also a vector 
// 			    in the logic so that it is not that predictible and get OUT from local minima 
// 		   The goal is to Calculate the same structure for Me - making the move and for my oponent and
// 		   based on this relation to select the coordinates of my next move that will:
// 		    a.) limit in MOST ability of my oponent for future moves
// 		    b.) leave him smalles amount of possible location to place a obsticle that i will not be able to cross over
// 		   Each of this set of records is calculated in a way that sth reletive to a probability is 
// 		   calculated and first on the list is used to achieve one of this 2 goals but... which one this is dependant
// 		   on calculating which move will be most effective BUT this is also factorized with a probability of a scalar that represents 
// 	           the UNKNOW factor so that my o[ponent does not prepera itself to fight me with predicting my strategy.
// 		   ( and i have more then one strategy and which one is chosen is also  unknown for me as a 
// 		   probability filter is used to choose which one will be used in iteration)
// -------------------------------------------------------------------------------------------------------

#pragma once
#include "stdio.h"

#include "fighting_structures.h"
#include "BaseBlock.h"
#include "TPath.h"

#define DifP(P1, P2) ((TPoint){P1.x - P2.x , P1.y - P2.y})

class CPathFinder {
	public:

	int FindLessRestrictedUnit(TPathPoints& Team);
	int FindMostRestrictedUnit(TPathPoints& Team);
	TPoint GetNextPointInRadiusOf(int radius, TPoint maskToUse, TPoint Location);
	void RunMutuals();
	// procedura do szukania punkto wspolnych
	// **ustawiane sa odpowednie parametryna wszystkich polach mapy
	// po ktorych w danej tuze moga sie poruszac jednsotki
	void FindMoveMutuals(TPath& XUmut);
	void FindShootMutuals(TPath& VirtualShooting_Element, const TPoint Startup);
	void ClearTempVV(VV& _tmp, const TPoint Destination_or_Start);
	bool Touching(TPoint B1, TPoint B2);
	void CleatParametersState();
	// procedura szuka okolice jednostki sprawdzajac czy jednsotka wroga ma bezposredni kontakt
	// strzalem badz ruchem z jednostka ktora bada
	// bezposredni kontakt z ruchem i strzalem =2
	// bezposredni kontakt ruchem BADZ strzalem =1
	int CheckHoodForThreat(TPath& CheckingUnit, TUID& ThreatsCount);
	// procedura wykonana wtedy gdy nie ma dubletu ruchu i cementowania pzrez
	// jakakolwiek jdnostke druzyny pzeciwnej.. czyli wyszukiwana jedst jednostka w
	// druzynie do ktoerej jest najlatwiejszy dostep
	int ThreatCountSearch(TPathPoints& Tim/*, TUDI& Junction*/);
	// procedura liczy ilosc dwujek (mozliwosci rdostania sie i
	// zacementowania jednoczesne tego samego pola)
	// gdyz wtedy jest najwieksze prawdopodobienstwoe ze
	// jednostka zostanie unieruchomiona
	int CountDoubletsInFearFactor(TUID& FF);
	// procedura wybierze sposrod druzyny jednostke ktorej
	// pola otaczajace moga byc zasiedzone badz zacementowane pzrez
	// najwieksza ilosc jednostek pzreciwnej druzyny
	// ** w tym pzrypadku procedura szuka jednostki w druzynie do krorej
	// jest dostep jednoczesnie betonem i ruchem pzrez jak najwieksza
	// licznbe jednostek druzyny pzreciwnej
	// jelsi taki dostep nei istnieje (tzn.. nei ma takiej jednostki ktora mogla by sie ruszyc
	// w bezposrednie sasiedztwo jednostki i dodatkowo zacementowac jej sasiedztwo),
	// to po prostu wybeiraa jest jednsotka z druzyny do ktorej najlatwiejszy jest dostep
	// jak najwiekszej ilsoci jednostek druzyny pzreciwnej
	int FindMostMenacedUnit_DupletSearch( TPathPoints& LoopedTim, int& DoubletsFound );
	// funkcja robi to samo co popzredniczka lecz szuka obektow ktorych ilosc
	// dupletow jest mniejsza od wskazanego parametru
	int FindNextMostMenacedUnit_DupletSearch( TPathPoints& LoopedTim, int& IN_OUT_MeancedParam );
	bool CheckVectorCorectness(TPoint V1, TPoint V2, TPoint V2Mask, int MaxRadius);
	// z pola CementingFrom nastepuje cementowanie pol w obrebie jednostki E
	// a wyniki ktore to pola maja byc zacementowane sa pzekazywane
	// w postaci numeru maski ktora ma byc uzyta aby zacementowac jakies pole.
	// w ten sposob otzrymuje mozliwosc wyboru sposrod pol mozliwych do zacementowanie ,
	// tego pola ktory bedzie sluzylo mojej druzynie
	void SerchFieldToCement_OtherThanAndTouchingTo(TPoint CementingFrom, TPath& E, TUID& Result, int& FieldsToCementFound);
	bool CheckIfCanCementAndStay(TPath& EnUnitToCheck, TPath& MyUnit, TPoint& OUTPossibleNEWLocation, int& OUTPossibilitysCount, TUID& OUTPossibleMasks);
// Wybiez Najodpoiedniejsza Jedsotke Sposrod Moich
// Opis Parametrow:
// int& MyAppropriateUnitsCount zwraca liczbe jednostek mojej druzyny
// kore nadaja sie do zadania zasiadniecia kolo
// jednostki woga i zacementowania jej najblizszej okolic
//
// TUID& AppropriateUnitsIDsVector = to wektor o dlugosc MyAppropriateUnitsCount ktory
// zawiera indexy moich jednostek ktore nadaja sie do tego zadania
//
// TPath& OnEn - jednostka pzreciwnika ddla ktorej sprawdzanei sie odbywa
//
// TUID rawBuff - bufor okreslajacy ilosc pol wokol jednostki w ktorej
// wystapil dublet (moziwosc zasiadniecia i zacementoweania
// tego samego punktu w obrebie jednostki wroga (OnEn))
//
// TPoint& NewWariorLocum - Jesli odpwiednia jednostka jest znaleziona
// wspolzredne do ktorych powinna sie pzremiescic sa okreslone ta zmienne
//
// int& FineFieldsCount - okresla ile pol dla danej jednostki odpowiedniej
// do tego zadania jest w zsiegu cementowania (pol bedacych w bezposrednim sasiedztwie jednsotki wroga OnEn)
//
// TUID& PointsMaskBufer - okresla indexy maski jednostki wroga ktore jesl iuzyte
// wyznacza punkty ktore moga byc zacementowane
	void WNJS( int& MyAppropriateUnitsCount, TUID& AppropriateUnitsIDsVector, TAtack& AtackInfo,TPath& OnEn, TUID rawBuff );
	void ChooseFromGivenAndCement_DupletShoot(TBufor& OutputPlacement,int count, TUID& MyUnitsVec, TAtack& FInfo, TPath& UEnemy);
	void CementOldPlace(TBufor& OutputPlacement, TPath& WolnyElement);
	// procedura wykona sie zawsze gdy nei ma mozliwosci wykonania
	// dupletu na jednej z jednostek pzreciwnika
	void ChooseShootAndRun(TBufor& OutputPlacement);
	TPoint GetConnectionField( TBufor& OutputPlacement, TPath& Warior, TPath& Enemy, TPoint CementDest );
	TPoint ChooseFieldToCement(TPath& Warior ,TPath& Enemy);
	void MoveAndShoot(TBufor& OutputPlacement, TPath& ActiveUnit, TPoint MoveDestination, TPoint ShootDestination );
	void ChoosingUnits(TBufor& OutputPlacement);
	void ChooseTimMoveCoordinates(TBufor& OutputPlacement);
	void WykonajJalowyRuch(TBufor& OutputPlacement);
	void CopyOneToOther(TUID& Source, TUID& Dest);
	void restBufer(TUID& Data);
 	void resetToMinusOne(TUID& Data);
	void InvokeKoniec(TBufor& _OutputPlacement);
	CPathFinder(int PlayerStartID);
	~CPathFinder();
};