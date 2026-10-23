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
#include "stdio.h"
#include "fighter.h"
typedef struct{
	TPoint NewORGForWariorUnit;
	int PosibleFieldsCount;
	TUID UsableMasksIDsVector;
} TAtackHQ;
const TUID ZeroThreat = {0, 0, 0, 0};
const TPoint NullPoint = {-1, -1};
const TPoint ZeroPoint = {0, 0};
const TPoints constWariorLoc = {
	{
		{3, 0},
		{6, 0},
		{0, 3},
		{9, 3}
	}
};
const TPoints constEnemyLoc = {
	{
		{0, 0}, // point[0]
		{0, 0}, // point[1]
		{0, 0}, // point[2]
		{0, 0}  // point[3]
	}
};
// const TPoint cMoveBlur[8] = {0};
// const TPoint cMoveBlur[8] = {{0}};
const TPoint cMoveBlur[8] = {
	{0, 0}, // [0]
	{0, 0}, // [1]
	{0, 0}, // [2]
	{0, 0}, // [3]
	{0, 0}, // [4]
	{0, 0}, // [5]
	{0, 0}, // [6]
	{0, 0}  // [7]
};
class BaseBlock {
	private:
	bool accessible;
	TPoint Cords;
	int EnemyMutualMov;
	int TimMutualMov;
	int EnemyShootMut;
	int TimShootMut;
	public:
	int FreeDiections;
	TUID EnemysID_MutualMov;
	TUID WariorsID_MutualMov;
	TUID EnemysID_ShootMutual;
	TUID WariorsID_ShootMutual;
	bool BlockAccess() {
		return accessible;
	};
	void SetAccessRights(bool value) {
		accessible=value;
		if(value==false) {
			AfterMooveClearMutuals();
		};
	};
	void AfterMooveClearMutuals() {
		EnemyMutualMov=0;
		TimMutualMov=0;
		EnemyShootMut=0;
		TimShootMut=0;
		restBufer(EnemysID_MutualMov);
		restBufer(WariorsID_MutualMov);
		restBufer(EnemysID_ShootMutual);
		restBufer(WariorsID_ShootMutual);
	};
	// WariorsID_xxx[..] i inne okreslaja czy jednostka
	// tora wykonuje wirtualny strzal lub ruch np jednostka druzyny(Tim==true)
	// ma mozliwosc strzelenia w ten punkt
	void SetMutualFactor(int unitID, bool MyTeam, bool shooting) {
		if (shooting) {
			if( MyTeam==true ) {
				TimShootMut++;
				WariorsID_ShootMutual[unitID]=1;
			} else {
				EnemyShootMut++;
				EnemysID_ShootMutual[unitID]=1;
			};
		} else {
			if( MyTeam==true ) {
				TimMutualMov++;
				WariorsID_MutualMov[unitID]=1;
			} else {
				EnemyMutualMov++;
				EnemysID_MutualMov[unitID]=1;
			};
		};
	};
	TPoint GetOrgin() {
		return Cords;
	};
	BaseBlock(int X, int Y, bool FieldIsAccesible) {
		accessible=FieldIsAccesible;
		Cords.x=X;
		Cords.y=Y;
		EnemyMutualMov=0;
		TimMutualMov=0;
		EnemyShootMut=0;
		TimShootMut=0;
		restBufer(EnemysID_MutualMov);
		restBufer(WariorsID_MutualMov);
		restBufer(EnemysID_ShootMutual);
		restBufer(WariorsID_ShootMutual);
	};
	void restBufer(TUID& Data) {
		Data[0]=0;
		Data[2]=0;
		Data[1]=0;
		Data[3]=0;
	};
	BaseBlock() {
		Cords.x = -1;
		Cords.y = -1;
		EnemyMutualMov = 0;
		TimMutualMov = 0;
		EnemyShootMut = 0;
		TimShootMut = 0;
		accessible = true;
		restBufer(EnemysID_MutualMov);
		restBufer(WariorsID_MutualMov);
		restBufer(EnemysID_ShootMutual);
		restBufer(WariorsID_ShootMutual);
	};
	BaseBlock(const BaseBlock &) = default;
	BaseBlock(BaseBlock &&) = delete;
	BaseBlock &operator=(const BaseBlock &) = default;
	BaseBlock &operator=(BaseBlock &&) = delete;
	~BaseBlock(){ };
};
typedef BaseBlock bb;
typedef struct {
	int radius;// denotes max path length aka max vector length
	TPoint DestPoint;//denotes a max point on FieldMap that the "ConcrateMixer" is managed to move
	int Mask;
} SimpPath;//Simple Bath aka. Basic Path
typedef SimpPath TVect[8];
const SimpPath Starter={-1, (TPoint){-1,-1}, -1};
typedef struct{
	TVect Vect;// a vector of possible maximum paths - thanks to radious z CMoveblur[.x] can set any coordinates on the filed on which the item Path has to move - that is my "Concrate Mixer"
	int VectCount;// takes a value that represents number of vectors found with possible moves
	TPoint Ground00;// represents a point (TPoint) for which all the above computations have been calculated
} VV;
class TPath {
	public:
	VV fVV;
	// represents a filed which is used to as a point from which a "BOMB" will be deployed 
	VV BombHQ;
	// fields Vect and VectCount should get cleaned and only field GroundZero set
	int PathForMixer;
	TPoint Org;
	// there are just two players so bool is enough to assign value for each of the teams; true - tema1, false - team2
	bool Tim;
	bool TempNotIntresting;
	int PathCount() {
		return fVV.VectCount;
	};
	int ShootDirectionCount() {
		return BombHQ.VectCount;
	};
	void SetNewPos(TPoint NewPos) {
		SetNewPos(NewPos.x, NewPos.y);
	};
	void SetNewPos(int _x, int _y) {
		Org.x = _x;
		Org.y = _y;
		for(int p=0; p<8; p++) {
			fVV.Vect[p].radius = -1;
			fVV.Vect[p].Mask = p;
			fVV.Vect[p].DestPoint = Org;
		};
		fVV.Ground00 = Org;
		fVV.VectCount = 0;
	};
	void ClearMixer_VV_ToStartup() {
		for(int p=0; p<8; p++) {
			fVV.Vect[p].radius = -1;
			fVV.Vect[p].Mask = -1;
			fVV.Vect[p].DestPoint = this->Org;
		};
		fVV.Ground00=Org;
		fVV.VectCount=0;
	};
	void SetHQ(const TPoint HQCording) {
		BombHQ.Ground00 = HQCording;
		BombHQ.VectCount = 0;
		for(int z=0; z<8; z++) {
			BombHQ.Vect[z].DestPoint=HQCording;
			BombHQ.Vect[z].Mask=-1;
			BombHQ.Vect[z].radius=-1;
		};
	};
	//constructor & destrucotr
	TPath(bool MyTeam, int mx, int my, int TeamId) {
		Tim = MyTeam;
		Org.x = mx;
		Org.y = my;
		PathForMixer = TeamId;
		TempNotIntresting = false;
		SetNewPos(Org);
	};
	TPath(){ };
	~TPath(){ };
};
typedef TPath TPathPoints[4];
typedef BaseBlock TPole[10][10];
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
	//
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
		TVect Di;
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
		TVect Di;
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
	FieldSpace(const FieldSpace &) = default;
	FieldSpace(FieldSpace &&) = delete;
	FieldSpace &operator=(const FieldSpace &) = default;
	FieldSpace &operator=(FieldSpace &&) = delete;
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
			BoardPos = (PlayersNumber == 1)?constWariorLoc[o]:constEnemyLoc[o];
			WariorLoc[o] = TPath(true, BoardPos.x, BoardPos.y, o);
			MOF[WariorLoc[o].Org.x][WariorLoc[o].Org.y].SetAccessRights(false);
			// Enemy Team
			BoardPos = (PlayersNumber == 2)?constWariorLoc[o]:constEnemyLoc[o];
			EnemyLoc[o] = TPath(false, BoardPos.x, BoardPos.y, o);
			MOF[EnemyLoc[o].Org.x][EnemyLoc[o].Org.y].SetAccessRights(false);
		};
	};
	//destructor
	~FieldSpace(){ };
};