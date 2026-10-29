// --------------------------------------------------------------------------------------------------------
// Author:          Blazej Kaczmarek
// ORCID:           https://orcid.org/0009-0003-1070-8870
// email:           fajny.masz.tornister@gmail.com
// github ord email:MasterDisaster@neurodine.com
// domain email:    Blazej.Kaczmarek@neurodine.com
// oid-tree:        http://oid-info.com/get/1.3.6.1.4.1.53685
// PEN:             1.3.6.1.4.1.53685
// oid ASN.1:       {iso(1) identified-organization(3) dod(6) internet(1) private(4) enterprise(1) 53685}
// Description:     this is the file that creates the structures that are used in path_finding file
//			there is not much logic here.. it's simply definitions and not much except setting some
// 			constant valuses, ZERO states and base values for structures itself - usual
// 			init a man would do when writing a chess łike game
// --------------------------------------------------------------------------------------------------------

#ifndef CONIXER_FIELDSPACE_H
#define CONIXER_FIELDSPACE_H

#include "fighting_structures.h"
#include "BaseBlock.h"
#include "TPath.h"

using TPathPoints = TPath[4];


class FieldSpace {
	public:
		using TPathPoints = ::TPathPoints;

		TPath WariorLoc[4] = {
			TPath(true, 0, 0, 0),
			TPath(true, 0, 0, 1),
			TPath(true, 0, 0, 2),
			TPath(true, 0, 0, 3)
		};
			// represents coordinates of enemy units
		TPath EnemyLoc[4] = {
			TPath(false, 0, 0, 0),
			TPath(false, 0, 0, 1),
			TPath(false, 0, 0, 2),
			TPath(false, 0, 0, 3)
		};
		BaseBlock MOF[10][10];
		TBufor FinalBuff{};

		int TimeLeft;
		int EmptyFields;
// represents coordinates of my units
// time left for doing the computations... this is thik i did not totaly used but the Applet deefines some maximum time
// represents the number of all free fields on the board
// ustawia pola na lanszy z zajetych na wolne i zapisuje:
// 		na planszy pole ktore zostalo zacementowane
// 		pole ktore zostalo zajete
// sets fields on the board from occupied to NOT-occupied and saves them
// 		a field on board that got occupied (by obsticle, not the player itself)
// 		a field on boeard that is used (taken by player itself)
	bool EQPoint(TPoint P1,int  X, int Y);
	bool IQl(TPoint P1, TPoint P2);
	bool GoodP(TPoint A);
	void SendData(TBufor& WriteIn);
	bool Valid(int fx, int fy);
	bool Valid(TPoint _org);
	// Player To Coordinates
	int PTC(TPathPoints& Team, int X, int Y);
	int PTC(TPathPoints& Team, const TPoint Pkt);
	void ConcrateXY(int shoot_x, int shoot_y);
	void ConcrateXY(TPoint ShootPos);
	bool MaskOK(int MAskId);
	bool RadOK(int radius_length);
	TPoint GetPoint(TPath& Element,  int radius, int MoveBlurID);
	void RecordMovement(TBufor& MovVector);
	void RunPathFinder();
	void Step(TPoint& Base, TPoint oVektor);
	TPoint Conv(int x, int y);
	// okresla mozliwe rodziny mozliwych strzalow
	void ShotsFromLocalization(TPath& TeamMemb, const TPoint ShootingFrom);
	//w proc wywolujacej ustawiane sa wszelkie wartosci wstepne (czyli w pathFinder::FindShootMutuals(..))
	void VirtualShotsFromLocalization(VV& VirtualShotsPlacement);
	// okresla pzremieszczenie danej jednostki - nei wywoluje niczego
	void MoveWarior(TPath& WariorToBeMoved, TPoint MoveTo);
	// okresla punkt ktory ma zostac ostrzelany - nei wywoluje niczego
	void ShootAtLocalization(const TPoint LocalizationToBeShot);
	// Argumentem procedury jest element typu VV
	// sa zapisywane wyniki szukania mozliwych max drog/ich ilosc
	// a pole Ground00 tego elementu oznacza pole na planszy dla
	// ktorego wykonywane so oblczenia
	void GenerateMoves_or_Shots(VV& XUnit);
	void FindFieldToShoot(VV& XUnit, const TPoint StopOnThisOne, bool& Found);
	void ClearGameMutuals();
	FieldSpace(const FieldSpace &) = delete;
	// FieldSpace(FieldSpace &&) = delete;
	FieldSpace &operator=(const FieldSpace &) = default;
	// FieldSpace &operator=(FieldSpace &&) = delete;
	explicit FieldSpace(int PlayersNumber);
	virtual ~FieldSpace() = default;
};

#endif // CONIXER_FIELDSPACE_H
