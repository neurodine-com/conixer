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

#include "fighter.h"
#include "baseblock.h"
#include "path_origin.h"
#include "FieldSpace.h"


class FieldSpace{
	private:
	TBufor FinalBuff;
	public:
	TPole MOF;
	// represents coordinates of my units
	TPathPoints WariorLoc;
	// represents coordinates of enemy units 
	TPathPoints EnemyLoc;
	// time left for doing the computations... this is thik i did not totaly used but the Applet deefines some maximum time 
	int TimeLeft;
	// represents the number of all free fields on the board
	int EmptyFields;
	// ustawia pola na lanszy z zajetych na wolne i zapisuje:
	// **na planszy pole ktore zostalo zacementowane
	// **pole ktore zostalo zajete
	// sets fields on the board from occupied to NOT-occupied and saves them
	// **a field on board that got occupied (by obsticle, not the player itself)
	// **a field on boeard that is used (taken by player itself)
	bool EQPoint(TPoint P1,int  X, int Y) {
		return (P1.x==X && P1.y==Y);
	};
	bool IQl(TPoint P1, TPoint P2) {
		 return (P1.x==P2.x && P1.y==P2.y);
	};
	bool GoodP(TPoint A) {
		 return  (A.x>=0 && A.x<10 && A.y>=0 && A.y<10 );
	};
	void SendData(TBufor& WriteIn) {
		for(int o=0; o<6;o++) {
			WriteIn[o]=FinalBuff[o];
		};
	};
	bool Valid(int fx, int fy) {
		return (bool)MOF[fx][fy].BlockAccess();
	};
	bool Valid(TPoint _org) {
		if(GoodP(_org)){
			return Valid(_org.x,_org.y);
		} else {
			return false;
		};
	};
	// Player To Coordinates
	int PTC(TPathPoints& Team, int X, int Y) {
		int i=-1;
		while(( EQPoint(Team[++i].Org,X,Y)==false )&&( i<4 )){
		};
		return (i<4)?i:-1;
	};
	int PTC(TPathPoints& Team, const TPoint Pkt) {
		return PTC(Team,Pkt.x, Pkt.y);
	};
	void ConcrateXY(int shoot_x, int shoot_y) {
		MOF[shoot_x][shoot_y].SetAccessRights(false);
		EmptyFields--;
	};
	void ConcrateXY(TPoint ShootPos) {
		if(GoodP(ShootPos)) {
			ConcrateXY(ShootPos.x, ShootPos.y);
		};
	};
	bool MaskOK(int MAskId) {
		return (MAskId>=0 || MAskId<=7);
	};
	bool RadOK(int radius_length) {
		return (radius_length>0 && radius_length<10);
	};
	TPoint GetPoint(TPath& Element,  int radius, int MoveBlurID) {
		if( (RadOK(radius) == false ) || ( MaskOK(MoveBlurID) == false )){
			std::cout<< "radius: "<< radius << " or mask ID: "<< MoveBlurID << " couses error";
			std::cin>> radius;
			return NullPoint;
		} else {
			TPoint Local=Element.Org;
			Local.x = (radius*cMoveBlur[MoveBlurID].x) + Element.Org.x;
			Local.y = (radius*cMoveBlur[MoveBlurID].y) + Element.Org.y;
			return Local;
		};
	};
	void RecordMovement(TBufor& MovVector) {
		int lx = MovVector[0];
		int ly = MovVector[1];
		int _a = PTC(EnemyLoc,lx,ly);
		int nx = MovVector[2];
		int ny = MovVector[3];
		int cx = MovVector[4];
		int cy = MovVector[5];
		EnemyLoc[_a].SetNewPos(nx, ny);
		EmptyFields--;
		MOF[lx][ly].SetAccessRights(true);
		MOF[nx][ny].SetAccessRights(false);
		MOF[cx][cy].SetAccessRights(false);
#if DEBUGIN==1 || DEBUGIN==2
		fx<<"Nowe polozenie wroga:"<<endl;
		V6(MovVector);
#endif
	};
	void RunPathFinder() {
		for(int TeamMemb=0; TeamMemb<4; TeamMemb++) {
			WariorLoc[TeamMemb].ClearMixer_VV_ToStartup();
			GenerateMoves_or_Shots(WariorLoc[TeamMemb].fVV);
			EnemyLoc[TeamMemb].ClearMixer_VV_ToStartup();
			GenerateMoves_or_Shots(EnemyLoc[TeamMemb].fVV);
		};
	};
	void Step(TPoint& Base, TPoint oVektor) {
		Base.x += oVektor.x;
		Base.y += oVektor.y;
	};
	TPoint Conv(int x, int y) {
		TPoint tmp;
		tmp.x = x;
		tmp.y = y;
		return tmp;
	};
	// okresla mozliwe rodziny mozliwych strzalow
	void ShotsFromLocalization(TPath& TeamMemb, const TPoint ShootingFrom) {
		TeamMemb.SetHQ(ShootingFrom);
		GenerateMoves_or_Shots(TeamMemb.BombHQ);
	};
	//w proc wywolujacej ustawiane sa wszelkie wartosci wstepne (czyli w pathFinder::FindShootMutuals(..))
	void VirtualShotsFromLocalization(VV& VirtualShotsPlacement) {
		GenerateMoves_or_Shots(VirtualShotsPlacement);
	};
	// okresla pzremieszczenie danej jednostki - nei wywoluje niczego
	void MoveWarior(TPath& WariorToBeMoved, TPoint MoveTo) {
		// 2
		FinalBuff[0]=WariorToBeMoved.Org.x;
		FinalBuff[1]=WariorToBeMoved.Org.y;
		MOF[WariorToBeMoved.Org.x][WariorToBeMoved.Org.y].SetAccessRights(true);
		WariorToBeMoved.SetNewPos(MoveTo);
		MOF[WariorToBeMoved.Org.x][WariorToBeMoved.Org.y].SetAccessRights(false);
		// 4
		FinalBuff[2]=MoveTo.x;
		FinalBuff[3]=MoveTo.y;
	};
	// okresla punkt ktory ma zostac ostrzelany - nei wywoluje niczego
	void ShootAtLocalization(const TPoint LocalizationToBeShot) {
		// 5
		ConcrateXY(LocalizationToBeShot);
		// 6
		FinalBuff[4]=LocalizationToBeShot.x; FinalBuff[5]=LocalizationToBeShot.y;
	};
	// Argumentem procedury jest element typu VV
	// sa zapisywane wyniki szukania mozliwych max drog/ich ilosc
	// a pole Ground00 tego elementu oznacza pole na planszy dla
	// ktorego wykonywane so oblczenia
	void GenerateMoves_or_Shots(VV& XUnit) {
		SimpPath Di;
		// XUnit musi byc koniecznie przygotowane do wpisywania pzred wejsciem do tej procedury;
		// jelsi obliczanei dla elementu zasigow ruchw w roznych kierunkach to robi to procedura ClearMixer_VV_ToStartup()
		// jeli inne pochodzenie wywolania elementu to musi to robic inna procedura
		// wstepne ustawienie mozliwych wektorow
		for(int it=0; it<8; it++) {
			//-1 ze wzglefu na algorytm obliczania mozliwych drog -> petla do{..}while();
			Di[it].radius = -1;
			Di[it].DestPoint = XUnit.Ground00;
		};
		for(int t=0; t<8; t++) {
			//min(XUnit.Ground00.x, XUnit.Ground00.y);
			int tot=0;
			do{
				Di[t].radius++;
				Step(Di[t].DestPoint,cMoveBlur[t]);
				//tot++;
			} while(( GoodP(Di[t].DestPoint) == true )&&( Valid(Di[t].DestPoint) == true ));
		};
		// ponizej, przypisane danemu elementowi jego wszystkich mozliwosci ruchu
		for(int iter=0; iter<8; iter++) {
			int rad = Di[iter].radius;
			if( rad>0 ) {
				XUnit.Vect[XUnit.VectCount].radius = rad;
				TPoint Offset = cMoveBlur[iter];
				Offset.x *= rad;
				Offset.y *= rad;
				Step(XUnit.Vect[XUnit.VectCount].DestPoint,Offset);
				XUnit.Vect[XUnit.VectCount].Mask = iter;
				XUnit.VectCount++;
			};
		};
	};
	void FindFieldToShoot(VV& XUnit, const TPoint StopOnThisOne, bool& Found) {
		SimpPath Di;
#if DEBUGIN == 2
		pp(Found);
		pp(StopOnThisOne.x); pp(StopOnThisOne.y);
		pp(XUnit.Ground00.x); pp(XUnit.Ground00.y);
#endif
		for(int it=0; it<8; it++) {
			//Di[it].radius=-1;//-1 ze wzglefu na algorytm obliczania mozliwych drog -> petla do{..}while();
			Di[it].DestPoint = XUnit.Ground00;
		};
		for(int t=0; t<8; t++) {
			do{	
				//Di[t].radius++;
				Step(Di[t].DestPoint,cMoveBlur[t]);
			} while(
				( GoodP(Di[t].DestPoint) == true )&&(
					( Found = IQl(Di[t].DestPoint, StopOnThisOne ) ) == false )&&(
						Valid(Di[t].DestPoint) == true )
			);
			if(Found==true){
				break;
			};
		};
	};
	void ClearGameMutuals() {
		for(int d=0; d<10; d++) {
			for(int dd=0; dd<10; dd++) {
				MOF[d][dd].AfterMooveClearMutuals();
			};
		};
	};
	// FieldSpace &operator=(FieldSpace &&) = delete;
	FieldSpace(int PlayersNumber) {
		EmptyFields = 92;
		TimeLeft = -1;
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 10; y++) {
				MOF[x][y] = BaseBlock(x, y, true);
			};
		};
		TPoint BoardPos = NullPoint;
		for (int o = 0; o < 4; o++) {
			// My Team
			if (PlayersNumber == 1) {
				BoardPos = constWariorLoc.point[o];
				WariorLoc[o] = TPath(true, BoardPos.x, BoardPos.y, o);
				MOF[WariorLoc[o].Org.x][WariorLoc[o].Org.y].SetAccessRights(false);
			}else if (PlayersNumber == 2) {
				// Enemy Team
				BoardPos = constEnemyLoc.point[o];
				EnemyLoc[o] = TPath(false, BoardPos.x, BoardPos.y, o);
				MOF[EnemyLoc[o].Org.x][EnemyLoc[o].Org.y].SetAccessRights(false);
			}
		};
	};
	//destructor
	~FieldSpace(){ };
};