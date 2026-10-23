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
//  
// -------------------------------------------------------------------------------------------------------

#include "stdio.h"
#include "../src/FieldSpace.h"


typedef TAtackHQ TAtack[4];

#define DifP(P1, P2) ((TPoint){P1.x - P2.x , P1.y - P2.y})

class CPathFinder : FieldSpace {

	public:
	//okresla ilosc wykonanych ruchow w grze
	int RuchNR; 
	//okresla sumaryczna ilosc pol (z kazdego kierunku) jaka lezy w mozliwosci wyboru wszystkch jednostek mojej druzyny
	int TimFreeSpace;
	// ---------------------||------------------------------------pzreciwnika druzyny
	int EnFreeSpace;
	int TimUnidID;
	int EnUnitID;

	int FindLessRestrictedUnit(TPathPoints& Team){
		int max_move_ability=0;
		int fUnit=-1;
		for(int k=0;k<4;k++){
			if(( Team[k].PathCount()>0 )&&( Team[k].TempNotIntresting==false )) {
				if( max_move_ability == Team[k].PathCount() ) {
					int przypadek=rand() % 10;
					fUnit=(przypadek >= 5)? k:fUnit;
					if(fUnit<0){
						fUnit=k;
					};
				} else {
					if( Team[k].PathCount()>max_move_ability ){
						fUnit=k;
						max_move_ability=Team[k].PathCount();
					};
                };
			} else {
				Team[k].TempNotIntresting=true;
			};
		};
		return fUnit;
	};

	int FindMostRestrictedUnit(TPathPoints& Team){
		int min_move_ability=7;
		int fUnit=-1;
		for(int k=0;k<4;k++){
			if (Team[k].TempNotIntresting==false ) {
				if( Team[k].PathCount()>0 ) {
					if( min_move_ability == Team[k].PathCount() ) {
						int przypadek = rand()%10;
						fUnit = (przypadek >= 5)?k:fUnit;
						if(fUnit<0){
							fUnit=k;
						};
					} else {
						if( Team[k].PathCount()<min_move_ability ) {
							fUnit=k;
							min_move_ability=Team[k].PathCount();
						};
					};
				} else {
					Team[k].TempNotIntresting=true;
				};
			};
		};
		return fUnit;
	};

	TPoint GetNextPointInRadiusOf(int radius, TPoint maskToUse, TPoint Location) {
		TPoint y=Location;
		maskToUse.x*=radius; maskToUse.y*=radius;
		Step(y,maskToUse);
		return y;
	};

	void RunMutuals(){
		for(int t=0; t<4; t++) {
			//wywoluje shoot mutuals
			FindMoveMutuals(WariorLoc[t]);
			//wywoluje shoot mutuals
			FindMoveMutuals(EnemyLoc[t]);
		};
	};

	// procedura do szukania punkto wspolnych
	// **ustawiane sa odpowednie parametryna wszystkich polach mapy
	// po ktorych w danej tuze moga sie poruszac jednsotki
	// 
	void FindMoveMutuals(TPath& XUmut){
		if( XUmut.PathCount()>0 ) {
			for(int ivec=0; ivec<XUmut.PathCount(); ivec++) {
				TPoint tp = XUmut.fVV.Vect[ivec].DestPoint;
				TPoint o = XUmut.Org;
				int elementRad = XUmut.fVV.Vect[ivec].radius;
				if(XUmut.Tim==true){ 
					TimFreeSpace += elementRad;
				} else {
					EnFreeSpace+=elementRad;
				};
				TPoint maska=cMoveBlur[XUmut.fVV.Vect[ivec].Mask];
				int rad_it=0;
				do{	
					rad_it++;
					Step(o,maska);
					MOF[o.x][o.y].SetMutualFactor(XUmut.PathForMixer, XUmut.Tim, false);
					FindShootMutuals(XUmut,o);
					// tutaj beda dla tego punku wybierane
					// szukane wszystkie mozliwe miejca w ktore mozna strzelic - z punktu (TPoint)o
				}
				while(( rad_it <= elementRad )&&( IQl(o,tp)==false ));
			};
		};
	};

	void FindShootMutuals(TPath& VirtualShooting_Element, const TPoint Startup){
		VV Temporary;
		//czyszczenei struktory w celu wyszukania kierunkow i zasiegow(promieni)
		ClearTempVV(Temporary,Startup);
		VirtualShotsFromLocalization(Temporary);
		if(Temporary.VectCount>0) {
			for(int vec_i=0; vec_i<Temporary.VectCount; vec_i++) {
				TPoint base=Startup;
				TPoint MaxP=Temporary.Vect[vec_i].DestPoint;
				TPoint msk=cMoveBlur[Temporary.Vect[vec_i].Mask];
				do{
					Step(base,msk);
					MOF[base.x][base.y].SetMutualFactor(VirtualShooting_Element.PathForMixer, VirtualShooting_Element.Tim, true);
				}while( IQl(base,MaxP)==false );
			};
		};
	};

	void ClearTempVV(VV& _tmp, const TPoint Destination_or_Start) {
		for(int p=0;p<8;p++){
			_tmp.Vect[p].radius = -1;
			_tmp.Vect[p].Mask = -1;
			_tmp.Vect[p].DestPoint = Destination_or_Start;
		};
		_tmp.Ground00 = Destination_or_Start;
		_tmp.VectCount = 0;
	};

	bool Touching(TPoint B1, TPoint B2) {
		bool AreTuching=false;
		int cmb=0;
		do{
			AreTuching|=EQPoint(B2,(B1.x+cMoveBlur[cmb].x),(B1.y+cMoveBlur[cmb].y));
			cmb++;
		}while(( AreTuching==false )&&( cmb<8 ));
		return AreTuching;
	};

	void CleatParametersState(){
		ClearGameMutuals();
		TimFreeSpace=0;
		EnFreeSpace=0;
	};

	// procedura szuka okolice jednostki sprawdzajac czy jednsotka wroga ma bezposredni kontakt
	// strzalem badz ruchem z jednostka ktora bada
	// bezposredni kontakt z ruchem i strzalem =2
	// bezposredni kontakt ruchem BADZ strzalem =1
	int CheckHoodForThreat(TPath& CheckingUnit, TUID& ThreatsCount) { 
	// zalozenie ze przefiltrowane zostaly PathCount=0 i do tej metody nei sa pzrekazywane
		int ResultingThreatFromAllEnemys=0;
		restBufer(ThreatsCount);
		for(int nei=0; nei<CheckingUnit.PathCount();nei++){
			TPoint _m=cMoveBlur[CheckingUnit.fVV.Vect[nei].Mask];
			TPoint ElToCheck=CheckingUnit.Org;
			Step(ElToCheck,_m);
			for(int k=0;k<4;k++){
			// z zalozenia ThreatsCount[k] mzoe przyjac max wartosc 2 bo EnemysID_MutualMov
			// i ten drugi max wartosci 1 pzryjmuja
				int cumulativ=0;
				if(CheckingUnit.Tim==true){
					cumulativ=	MOF[ElToCheck.x][ElToCheck.y].EnemysID_MutualMov[k];
					cumulativ+=	MOF[ElToCheck.x][ElToCheck.y].EnemysID_ShootMutual[k];
				} else {
					cumulativ=	MOF[ElToCheck.x][ElToCheck.y].WariorsID_MutualMov[k];
					cumulativ+=	MOF[ElToCheck.x][ElToCheck.y].WariorsID_ShootMutual[k];
				};
				if(ThreatsCount[k]<2){
					ThreatsCount[k]=cumulativ;
				};
				ResultingThreatFromAllEnemys+=cumulativ;//ThreatsCount[k];
			};
		};
		#if DEBUGIN==1
		V4(ThreatsCount);
		#endif
		return ResultingThreatFromAllEnemys;
	};


	// procedura wykonana wtedy gdy nie ma dubletu ruchu i cementowania pzrez
	// jakakolwiek jdnostke druzyny pzeciwnej.. czyli wyszukiwana jedst jednostka w
	// druzynie do ktoerej jest najlatwiejszy dostep
	int ThreatCountSearch(TPathPoints& Tim/*, TUDI& Junction*/){
		#if DEBUGIN==1
			wia();
		#endif
		int WynikC=-1;
		int thrCount=0;
		int oldthrCount=0;
		TUID tmpAr;
		for(int k=0; k<4; k++) {
			if((Tim[k].PathCount()>0 )&&( Tim[k].TempNotIntresting==false )) {
				thrCount=CheckHoodForThreat(Tim[k],tmpAr);
				if(thrCount>oldthrCount){
					WynikC=k;
					oldthrCount=thrCount;
				};
			};
		};
		#if DEBUGIN==1
			pp(WynikC);
		#endif
		return WynikC;
	};


	// procedura liczy ilosc dwujek (mozliwosci rdostania sie i
	// zacementowania jednoczesne tego samego pola)
	// gdyz wtedy jest najwieksze prawdopodobienstwoe ze
	// jednostka zostanie unieruchomiona
	int CountDoubletsInFearFactor(TUID& FF){
		int wynik=0;
		for(int p=0; p<4; p++){
			wynik += (FF[p]==2)?1:0;
		};
		return wynik;
	};


	// /*procedura wybierze sposrod druzyny jednostke ktorej
	// pola otaczajace moga byc zasiedzone badz zacementowane pzrez
	// najwieksza ilosc jednostek pzreciwnej druzyny
	// ** w tym pzrypadku procedura szuka jednostki w druzynie do krorej
	// jest dostep jednoczesnie betonem i ruchem pzrez jak najwieksza
	// licznbe jednostek druzyny pzreciwnej
	// jelsi taki dostep nei istnieje (tzn.. nei ma takiej jednostki ktora mogla by sie ruszyc
	// w bezposrednie sasiedztwo jednostki i dodatkowo zacementowac jej sasiedztwo),
	// to po prostu wybeiraa jest jednsotka z druzyny do ktorej najlatwiejszy jest dostep
	// jak najwiekszej ilsoci jednostek druzyny pzreciwnej
	int FindMostMenacedUnit_DupletSearch( TPathPoints& LoopedTim, int& DoubletsFound ){
		#if DEBUGIN==1
			wia();
		#endif
		int RsultingUnitID=-1;
		int oldNumberOfDublets=0;
		int numberOfDublets=0;
		for(int op=0; op<4; op++) {
			if(( LoopedTim[op].PathCount()>0 )&&( LoopedTim[op].TempNotIntresting==false )){
			TUID ThreatStatus;
			int TreatsCount=CheckHoodForThreat(LoopedTim[op],ThreatStatus);
			numberOfDublets=CountDoubletsInFearFactor(ThreatStatus);
				if(oldNumberOfDublets<numberOfDublets) {
					oldNumberOfDublets=numberOfDublets;
					DoubletsFound=oldNumberOfDublets;
					RsultingUnitID=op;
				};
			};
		};
		return RsultingUnitID;
	};

	// funkcja robi to samo co popzredniczka lecz szuka obektow ktorych ilosc
	// dupletow jest mniejsza od wskazanego parametru
	int FindNextMostMenacedUnit_DupletSearch( TPathPoints& LoopedTim, int& IN_OUT_MeancedParam ) {
		int RsultingUnitID=-1;
		int oldNumberOfDublets=IN_OUT_MeancedParam;
		int local_min=0;
		int numberOfDublets=0;
		//hasDuplet=false;
		for(int op=0; op<4; op++) {
			if(LoopedTim[op].PathCount()>0 && LoopedTim[op].TempNotIntresting==false){
			TUID ThreatStatus;
			int TreatsCount=CheckHoodForThreat(LoopedTim[op],ThreatStatus);
			numberOfDublets=CountDoubletsInFearFactor(ThreatStatus);
				if( (local_min<numberOfDublets)&&(numberOfDublets<oldNumberOfDublets) ){
					local_min=numberOfDublets;
					IN_OUT_MeancedParam=local_min;
					RsultingUnitID=op;
					//hasDuplet=true;
				};
			};
		};
		return RsultingUnitID;
	};

	bool CheckVectorCorectness(TPoint V1, TPoint V2, TPoint V2Mask, int MaxRadius){
		bool IQlity=false;
		if(Touching(V1,V2)==false){
			TPoint meetPoint=V2;
			int tmpr=0;
			do{
				tmpr++;
				Step(meetPoint,V2Mask);
			}while( ( (IQlity = IQl(V1,meetPoint)) == false )&&( tmpr<MaxRadius ) );
		} else {
			IQlity=true;
		};
		return IQlity;
	};

	// z pola CementingFrom nastepuje cementowanie pol w obrebie jednostki E
	// a wyniki ktore to pola maja byc zacementowane sa pzekazywane
	// w postaci numeru maski ktora ma byc uzyta aby zacementowac jakies pole.
	// w ten sposob otzrymuje mozliwosc wyboru sposrod pol mozliwych do zacementowanie ,
	// tego pola ktory bedzie sluzylo mojej druzynie
	void SerchFieldToCement_OtherThanAndTouchingTo( TPoint CementingFrom, TPath& E, TUID& Result, int& FieldsToCementFound ){
		FieldsToCementFound=0;
		resetToMinusOne(Result);
		for(int z=0; z<E.PathCount(); z++) {
			TPoint P=E.Org;
			TPoint mm=cMoveBlur[E.fVV.Vect[z].Mask];
			Step(P,mm);
			if(IQl(P,CementingFrom) == false){
				bool validIs=Touching(CementingFrom,P);
				if(validIs==true) {
					Result[FieldsToCementFound] = E.fVV.Vect[z].Mask;
					FieldsToCementFound++;
					};
				};
			};
		};
	};

	bool CheckIfCanCementAndStay( TPath& EnUnitToCheck, TPath& MyUnit, 
		TPoint& OUTPossibleNEWLocation, int& OUTPossibilitysCount, 
		TUID& OUTPossibleMasks ) {
		bool Result=false;
		for(int i=0;i<EnUnitToCheck.PathCount();i++){
			if(Result==false){
				TPoint m=cMoveBlur[EnUnitToCheck.fVV.Vect[i].Mask];
				TPoint _org=EnUnitToCheck.Org;
				int MaxRad=EnUnitToCheck.fVV.Vect[i].radius;
				Step(_org,m);
				bool isValid=CheckVectorCorectness(MyUnit.Org,_org,m,MaxRad);
				if(isValid==true){
					// oznacza to ze w dany punbkt (_org) o pdanej masce m obektu wroga
					// moze zostac pzresuniety moj element
					// teraznalezy sprawidzic czy pola pzryegle do tego w ktore sie moze ruszyc
					// moga byc zacementowane
					SerchFieldToCement_OtherThanAndTouchingTo(_org,EnUnitToCheck,OUTPossibleMasks,OUTPossibilitysCount);
					if(OUTPossibilitysCount>0) {	
						OUTPossibleNEWLocation=_org;
						Result=true;
					};
				};
			};
		};
		return Result;
		//PossibilitysCount zwracana jest ilosc pzryleglych pol do wroga rowniez pzrleglych
		// do nowego polozenia mojeje jednostki(_org) natomiast w PossibleFieldsToCement zwracane sa indexy
		// maski wroga okreslajace te pola
	};

// Wybiez Najodpoiedniejsza Jedsotke Sposrod Moich
// Opis Parametrow:
// **int& MyAppropriateUnitsCount zwraca liczbe jednostek mojej druzyny
// kore nadaja sie do zadania zasiadniecia kolo
// jednostki woga i zacementowania jej najblizszej okolic
//
// **TUID& AppropriateUnitsIDsVector = to wektor o dlugosc MyAppropriateUnitsCount ktory
// zawiera indexy moich jednostek ktore nadaja sie do tego zadania
//
// **TPath& OnEn - jednostka pzreciwnika ddla ktorej sprawdzanei sie odbywa
//
// **TUID rawBuff - bufor okreslajacy ilosc pol wokol jednostki w ktorej
// wystapil dublet (moziwosc zasiadniecia i zacementoweania
// tego samego punktu w obrebie jednostki wroga (OnEn))
//
// **TPoint& NewWariorLocum - Jesli odpwiednia jednostka jest znaleziona
// wspolzredne do ktorych powinna sie pzremiescic sa okreslone ta zmienne
//
// **int& FineFieldsCount - okresla ile pol dla danej jednostki odpowiedniej
// do tego zadania jest w zsiegu cementowania (pol bedacych w bezposrednim sasiedztwie jednsotki wroga OnEn)
//
// **TUID& PointsMaskBufer - okresla indexy maski jednostki wroga ktore jesl iuzyte
// wyznacza punkty ktore moga byc zacementowane

	void WNJS( int& MyAppropriateUnitsCount, 
			TUID& AppropriateUnitsIDsVector,
			TAtack& AtackInfo,
			TPath& OnEn, TUID rawBuff ) {
		MyAppropriateUnitsCount=0;
		TPoint NewWariorLocum;
		int FineFieldsCount;
		TUID PointsMaskBufer;
		restBufer(AppropriateUnitsIDsVector);
		for(int k=0;k<4;k++){
			int tmp=rawBuff[k];
			if(tmp==2) {
				NewWariorLocum=NullPoint;
				FineFieldsCount=0;
				restBufer(PointsMaskBufer);
				bool C_A_S=CheckIfCanCementAndStay(OnEn, WariorLoc[k],NewWariorLocum,FineFieldsCount, PointsMaskBufer);
				if(C_A_S==true) {
					AppropriateUnitsIDsVector[MyAppropriateUnitsCount]=k;
					AtackInfo[MyAppropriateUnitsCount].NewORGForWariorUnit=NewWariorLocum;
					AtackInfo[MyAppropriateUnitsCount].PosibleFieldsCount=FineFieldsCount;
					CopyOneToOther(PointsMaskBufer,AtackInfo[MyAppropriateUnitsCount].UsableMasksIDsVector);
					MyAppropriateUnitsCount++;
					#if DEBUGIN==1
						pp(MyAppropriateUnitsCount-1);
						V4(AtackInfo[MyAppropriateUnitsCount-1].UsableMasksIDsVector);
						V4(PointsMaskBufer);
						pp(AppropriateUnitsIDsVector[MyAppropriateUnitsCount-1]);
						pp(AtackInfo[MyAppropriateUnitsCount-1].PosibleFieldsCount);
						pp(AtackInfo[MyAppropriateUnitsCount-1].NewORGForWariorUnit.x);
						pp(AtackInfo[MyAppropriateUnitsCount-1].NewORGForWariorUnit.y);
					#endif
				};
			};
		};
	};

	void ChooseFromGivenAndCement_DupletShoot(TBufor& OutputPlacement,int count, TUID& MyUnitsVec, TAtack& FInfo, TPath& UEnemy){
	// wybor jednostki  strzal
		int MyUnitsBigestThreat=0;
		int thisUnit=-1;
		int curr_id=-1;
		for(int co=0; co<count; co++) {
			int unitID=MyUnitsVec[co];
			TUID fuu;
			int ThreatTest=CheckHoodForThreat(WariorLoc[unitID],fuu);
			#if DEBUGIN==1
				pp(ThreatTest);
			#endif
			if(ThreatTest>MyUnitsBigestThreat){
				curr_id=co;
				MyUnitsBigestThreat=ThreatTest;
				thisUnit=unitID;
			};
		};
		TPoint ShootDestination=UEnemy.Org;
		//utaj mozna jakas dodatkowa funkcje wybrac
		int mask_to_use = rand() % FInfo[curr_id].PosibleFieldsCount;
		TPoint masc = cMoveBlur[FInfo[curr_id].UsableMasksIDsVector[mask_to_use]];
		#if DEBUGIN==1 || DEBUGIN==2
			pp(masc.x); pp(masc.y);
			pp(curr_id);
			pp(FInfo[curr_id].PosibleFieldsCount);
			pp(mask_to_use);
			V4(FInfo[curr_id].UsableMasksIDsVector);
			pp(FInfo[curr_id].PosibleFieldsCount);
		#endif
		Step(ShootDestination,masc);
		#if DEBUGIN==1
			pp(ShootDestination.x); pp(ShootDestination.y);
		#endif
		MoveAndShoot(OutputPlacement, WariorLoc[thisUnit],FInfo[curr_id].NewORGForWariorUnit,ShootDestination);
		//wywolanie rozkazu strzalu bezposrednei parametry - dwa punkty start i des i strzal
	};

	void CementOldPlace(TBufor& OutputPlacement, TPath& WolnyElement){
		int maxRadius=0;
		int Direction=-1;
		#if DEBUGIN==2
			fx<<"ruch Z miejsca:"<<endl;
			pp(WolnyElement.Org.x); pp(WolnyElement.Org.y);
			pp(WolnyElement.PathCount());
		#endif
		for(int i=0;i<WolnyElement.PathCount();i++) {
			if(maxRadius<WolnyElement.fVV.Vect[i].radius) {
				Direction = i;
				maxRadius = WolnyElement.fVV.Vect[i].radius;
			};
		};
		TPoint Destination = WolnyElement.fVV.Vect[Direction].DestPoint;
		#if DEBUGIN==2
			fx<<"ruch DO miejsca:"<<endl;
			pp(Destination.x); pp(Destination.y);
		#endif
		MoveAndShoot(OutputPlacement, WolnyElement, Destination, WolnyElement.Org);
	};

// procedura wykona sie zawsze gdy nei ma mozliwosci wykonania
// dupletu na jednej z jednostek pzreciwnika
	void ChooseShootAndRun(TBufor& OutputPlacement) {
		#if DEBUGIN==2
			wia();
		#endif
		int id=FindMostRestrictedUnit(WariorLoc);
		#if DEBUGIN==2
			pp(id);
			pp(FindMostRestrictedUnit(WariorLoc));
			pp(WariorLoc[id].Org.x); pp(WariorLoc[id].Org.y);
			pp(WariorLoc[id].PathCount());
		#endif
		if(id<0) {
			FindLessRestrictedUnit(WariorLoc);
			#if DEBUGIN==2
				pp(FindLessRestrictedUnit(WariorLoc));
			#endif
		};
		TUID RestrictionsForMyUnitFrom;
		int RestrictionsForHOOD = CheckHoodForThreat(WariorLoc[id],RestrictionsForMyUnitFrom);
		#if DEBUGIN==2
			pp(RestrictionsForHOOD);
			V4(RestrictionsForMyUnitFrom);
		#endif
		switch(RestrictionsForHOOD){
			case 0:{
				// move and CementOldPlace
				CementOldPlace(OutputPlacement,WariorLoc[id]);
				#if DEBUGIN==2
					wia();
				#endif		
				};break;
			default: {
				int EngagedEnemyIs=-1;
				int tmp=0;
				for(int re=0;re<4;re++){
					if(tmp < RestrictionsForMyUnitFrom[re]) {
						tmp=RestrictionsForMyUnitFrom[re];
						EngagedEnemyIs=re;
					} else {
						if( (tmp == RestrictionsForMyUnitFrom[re] )>0 ){
							EngagedEnemyIs=(rand()%10 >= 5)? EngagedEnemyIs:re;
						};
					};
				// koniec szukania jednostki ktora bede atakowal
				};
			};
		};
		#if DEBUGIN==2
			pp(EngagedEnemyIs);
			pp(EnemyLoc[EngagedEnemyIs].Org.x); pp(EnemyLoc[EngagedEnemyIs].Org.y);
			pp(WariorLoc[id].Org.x); pp(WariorLoc[id].Org.y);
		#endif
		TPoint CementX=ChooseFieldToCement(WariorLoc[id], EnemyLoc[EngagedEnemyIs]);
		// to pole bedze zacementowane, nastene jest znalezienei punktu do ktorego nalezy sie ruszyc*/
		#if DEBUGIN==2
			pp(CementX.x);pp(CementX.y);
		#endif
		if( IQl(CementX,NullPoint) == true ) {
			WariorLoc[id].TempNotIntresting=true;
			// fx<<"HOHO --- Wejscie w kolejna iteracie: "<<__FUNCTION__<<endl;
			ChooseShootAndRun(OutputPlacement);
		} else {
			TPoint StopField=GetConnectionField(OutputPlacement, WariorLoc[id], EnemyLoc[EngagedEnemyIs], CementX);
			#if DEBUGIN==2
			pp(StopField.x);pp(StopField.y);
			#endif
			MoveAndShoot(OutputPlacement, WariorLoc[id], StopField, CementX);
			#if DEBUGIN==2
				fx<<"koniec rcedury ChooseShootAndRun()"<<endl;
			#endif
		};
	};

	TPoint GetConnectionField( TBufor& OutputPlacement, TPath& Warior, TPath& Enemy, TPoint CementDest ) {
		VV Base;
		bool FoundSearchedField = false;
		TPoint NextP = NullPoint;
		TPoint _msk = NullPoint;
		for(int z=0;z<Warior.PathCount();z++){
			int z2=0;
			while(++z2 <= Warior.fVV.Vect[z].radius) {
				NextP=Warior.Org;
				_msk=cMoveBlur[Warior.fVV.Vect[z].Mask];
				_msk.x*=z2
				_msk.y*=z2;
				Step(NextP, _msk);
				#if DEBUGIN==2
					fx<<"Pole z ktoego testuje strzaly do pola:"<<" ["<<CementDest.x<<", "<<CementDest.y<<"]"<<endl;
					pp(NextP.x);pp(NextP.y);
				#endif
				ClearTempVV(Base,NextP);
				FindFieldToShoot(Base, CementDest, FoundSearchedField);
				if(FoundSearchedField==true) {
				// do tego punktu (NextP)
				// musi sie pzremiescic Warior zeby strzelic w CementDest
					#if DEBUGIN==2
						fx<<"znaleziony punkt:"<<endl;
						pp(NextP.x);pp(NextP.y);
					#endif
					return NextP;
				};
			};
		};
		return NullPoint
	};

	TPoint ChooseFieldToCement(TPath& Warior ,TPath& Enemy){
		TPoint Result=NullPoint;
		TPoint _m = {0, 0}
		TPoint _t = {0, 0}
		for(int x=0; x<Enemy.PathCount(); x++) {
			_t = Enemy.Org;
			_m = cMoveBlur[Enemy.fVV.Vect[x].Mask];
			Step(_t,_m);
			int HasShootMutual = 0
			HasShootMutual = MOF[_t.x][_t.y].WariorsID_ShootMutual[Warior.PathForMixer];
			if( HasShootMutual>0 ) {
				Result=_t;
				break;
			};
		};
		return Result;
	};

	void MoveAndShoot(TBufor& OutputPlacement, TPath& ActiveUnit, TPoint MoveDestination, TPoint ShootDestination ){
        #if DEBUGIN==1
			wia();
		#endif
		MoveWarior(ActiveUnit, MoveDestination);
		RuchNR++;
		ShotsFromLocalization(ActiveUnit,MoveDestination);
		ShootAtLocalization(ShootDestination);
		SendData(OutputPlacement);
	};

	void ChoosingUnits(TBufor& OutputPlacement){
		bool doesItHasDuplet;
		#if DEBUGIN==2
			fx<<"---"<<RuchNR<<"----"<<endl;
			fx<<"-=-=-=-=-=-=-=-New Begining-=-=-=-=-=-=-=-="<<endl;
		#endif
		EnemyLoc[0].TempNotIntresting = (EnemyLoc[0].PathCount()==0)?true:false;
		EnemyLoc[1].TempNotIntresting = (EnemyLoc[1].PathCount()==0)?true:false;
		EnemyLoc[2].TempNotIntresting = (EnemyLoc[2].PathCount()==0)?true:false;
		EnemyLoc[3].TempNotIntresting = (EnemyLoc[3].PathCount()==0)?true:false;
		WariorLoc[0].TempNotIntresting = (WariorLoc[0].PathCount()==0)?true:false;
		WariorLoc[1].TempNotIntresting = (WariorLoc[1].PathCount()==0)?true:false;
		WariorLoc[2].TempNotIntresting = (WariorLoc[2].PathCount()==0)?true:false;
		WariorLoc[3].TempNotIntresting = (WariorLoc[3].PathCount()==0)?true:false;
		bool FurtherOperationsGO=true;
		for(int i=0; i<4; i++) {
			if( ( true == FurtherOperationsGO )&&( false == WariorLoc[i].TempNotIntresting) ) {
				for(int j=0;j<4;j++){
					if( EnemyLoc[j].PathCount()<=2 ) {
					if( (true == FurtherOperationsGO)&&( true == Touching(WariorLoc[i].Org,EnemyLoc[j].Org )) {
						FurtherOperationsGO=false;
						#if DEBUGIN==2
							wia();
						#endif
						CementOldPlace(OutputPlacement,WariorLoc[i]);
						break;
						};
					};
				};
			};
		};
		if(FurtherOperationsGO==true){
			// okresla ilosc znalezionych dubletow
			int MenacedParameter=0;
			TUID Bufer;
			int id;
			int ThreatCount;
			// This Task My Units Count
			int TTMUC=0;
			// This Task My Units Vector
			TUID TTMUVec;
			// zbior Informacji o jednostkch i ich parametrach koncowych
			TAtack FinalInfo;
			id=FindMostMenacedUnit_DupletSearch(EnemyLoc, MenacedParameter);
			MenacedParameter++;
			int iterator=0;
			do{
				id=FindNextMostMenacedUnit_DupletSearch(EnemyLoc, MenacedParameter);
				ThreatCount=CheckHoodForThreat(EnemyLoc[id],Bufer);
				WNJS(TTMUC, TTMUVec, FinalInfo, EnemyLoc[id],Bufer);
			}while(( TTMUC==0 )&&( (++iterator)<4 ));
			#if DEBUGIN==2
				pp(TTMUC);
				pp(id);
			#endif
			// oznacza ze nie zostala znaleziona zadna jednostka z mozliwoscia wykonania dubletu
			if(id<0) {
				#if DEBUGIN==2
					wia();
				#endif
				ChooseShootAndRun(OutputPlacement);
			} else {
				int RestrictionLvl=EnemyLoc[id].PathCount();
				switch(RestrictionLvl) {
					case 1:{
							// pootzrena procedura korkujaca jednostke
							WykonajJalowyRuch(OutputPlacement);
							#if DEBUGIN==2
								wia();
							#endif
						}; break;
					default:{
							if(TTMUC==0){
								#if DEBUGIN==2
									pp(MenacedParameter);
									fx<<"SIMPLE computation"<<endl;
								#endif
							ChooseShootAndRun(OutputPlacement);
							} else {
								#if DEBUGIN==2
									pp(TTMUC);
									pp(id);
									pp(EnemyLoc[id].PathForMixer);
									pp(EnemyLoc[id].Org.x); pp(EnemyLoc[id].Org.y);
									fx<<"DUPLET computation"<<endl;
								#endif
								ChooseFromGivenAndCement_DupletShoot(OutputPlacement,TTMUC, TTMUVec, FinalInfo, EnemyLoc[id]);
							};
						};
					};
				};
			};
		};
	};

	void ChooseTimMoveCoordinates(TBufor& OutputPlacement) {
		CleatParametersState();
		//	okreslenie mozliwych drog
		RunPathFinder();
		RunMutuals();
		ChoosingUnits(OutputPlacement);
	};

	void WykonajJalowyRuch(TBufor& OutputPlacement) {
		int PathToMove;
		int RadiusToMove;
		int maxRad;
		int modRad;
		TPoint MoveDestination;
		TPoint ShootDestination;
		int id;
		id = FindLessRestrictedUnit(WariorLoc);
		PathToMove = rand() % WariorLoc[id].PathCount();
		maxRad = WariorLoc[id].fVV.Vect[PathToMove].radius;
		modRad = (maxRad>1)?(rand()%(maxRad+1)):maxRad;
		if( modRad == 0 ) {
			modRad = 1;
		};
		RadiusToMove = modRad;
		TPoint tmpMask = cMoveBlur[WariorLoc[id].fVV.Vect[PathToMove].Mask];
		MoveDestination = GetNextPointInRadiusOf(RadiusToMove, tmpMask, WariorLoc[id].Org);
		MoveWarior(WariorLoc[id], MoveDestination);
		RuchNR++;
		//1. <<END
		//2. Geting Point To Shoot To
		int ShootDirection;
		int ShootRadius;
		ShotsFromLocalization(WariorLoc[id], MoveDestination);
		if(WariorLoc[id].ShootDirectionCount()>1) {
			ShootDirection = rand() % WariorLoc[id].ShootDirectionCount();
		} else {
			ShootDirection=1;
		};
		ShootDirection = rand() % WariorLoc[id].ShootDirectionCount();
		maxRad = WariorLoc[id].BombHQ.Vect[ShootDirection].radius;
		modRad = (maxRad>1)?(rand()%maxRad)+1:maxRad;
		if(modRad==0) {
			modRad=1;
		};
		ShootRadius = modRad;
		TPoint tmpMask2 = cMoveBlur[WariorLoc[id].BombHQ.Vect[ShootDirection].Mask];
		ShootDestination = GetNextPointInRadiusOf(ShootRadius, tmpMask2, MoveDestination);
		//2. <<END
		ShootAtLocalization(ShootDestination);
		SendData(OutputPlacement);
	};

	void CopyOneToOther(TUID& Source, TUID& Dest) {
		Dest[0]=Source[0];
		Dest[1]=Source[1];
		Dest[2]=Source[2];
		Dest[3]=Source[3];
 	};

	void restBufer(TUID& Data) {
		Data={0, 0, 0, 0};
 	};

 	void resetToMinusOne(TUID& Data) {
		Data={-1, -1, -1, -1};
 	};


	void InvokeKoniec(TBufor& _OutputPlacement) {
		RuchNR++;
		_OutputPlacement[0]=-1;
		_OutputPlacement[1]=-1;
		_OutputPlacement[2]=-1;
		_OutputPlacement[3]=-1;
		_OutputPlacement[4]=-1;
		_OutputPlacement[5]=-1;
	};

	CPathFinder(int PlayerStartID): FieldSpace::FieldSpace(PlayerStartID) {
		TimFreeSpace=0;	EnFreeSpace=0; /*oba paramatry ustawiane w pzry szukaniu mutuali*/
		srand(time(0));
	};

	~CPathFinder() {
	};
};
