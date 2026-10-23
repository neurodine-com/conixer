// --------------------------------------------------------------------------------------------------------
// Author:          Blazej Kaczmarek
// ORCID:           https://orcid.org/0009-0003-1070-8870
// email:           fajny.masz.tornister@gmail.com
// github ord email:MasterDisaster@neurodine.com
// domain email:    Blazej.Kaczmarek@neurodine.com
// oid-tree:        http://oid-info.com/get/1.3.6.1.4.1.53685
// PEN:             1.3.6.1.4.1.53685
// oid ASN.1:       {iso(1) identified-organization(3) dod(6) internet(1) private(4) enterprise(1) 53685}
// Description: 	   So this is the main loop reading what other player did and later afer computations of my move 
// 		   this loops send to it's own stdout My Own request (move, move +place obstacle - BOMB with Concrate Mixer a concrate "shit"
// 		   so this is it. From this place the fun starts and 2 other files are used 
// 		   for the logic: path_finder.h, tel.h, FieldSpace.h 
// -----------------------------------------------------------------------------------------------

#include <cstdlib>
#include <iostream>
#include <fstream>
//#include <sstream>
using namespace std;

int StaringWith = 0;


#define DEBUGIN 2


#if DEBUGIN==1 || DEBUGIN==2
ofstream fx;
#define ZZ fx
	#define V4(vl4)	(ZZ<< __FUNCTION__ << "::"<< __LINE__<<":"<< #vl4 <<"["<< vl4[0] <<", " << vl4[1] \
			<< ", " << vl4[2] << ", " << vl4[3] << "]"<<endl)
	#define V6(vl6) (ZZ<< __FUNCTION__ << "::"<< __LINE__<<":"<< #vl6 <<"["<< vl6[0] <<", " << vl6[1] \
			<< ", " << vl6[2] << ", " << vl6[3] << ", " << vl6[4] << ", " << vl6[5] << "]"<<endl)
	#define pp(Val) (ZZ<< __FUNCTION__ << "::"<< __LINE__<< ":" << #Val <<"("<< Val<< ")"<<endl)

	#define wia() (ZZ<< __FUNCTION__ << "::"<< __LINE__<< endl)

#endif

#include "fighter.h"

#include "tel.h"
#include "FieldSpace.h"
#include "path_finder.h"


int MaxMoves;

int main(int argc, char *argv[]) {
	MaxMoves=92;
	cin >> StaringWith;
#if DEBUGIN==1 || DEBUGIN==2
	if(StaringWith==1) {
		fx.open("logout/smDebug1.log", ios::trunc);
	} else {
		fx.open("logout/smDebug2.log", ios::trunc);
	}
	if(fx.is_open()){
#endif
	tel call = tel(StaringWith);
	CPathFinder World = CPathFinder(StaringWith);
	TBufor MyReadBuff={0,0,0,0,0,0};
	TBufor MyWriteBuff={0,3,0,2,0,3};
	if(StaringWith==1) {
		call.GetLineOfLuckingTime(World.TimeLeft);
		World.ChooseTimMoveCoordinates(MyWriteBuff);
		call.writeLine(MyWriteBuff);
		call.readLine(MyReadBuff);
		World.RecordMovement(MyReadBuff);
	} else {
		call.readLine(MyReadBuff);
		World.RecordMovement(MyReadBuff);
	};
	World.RuchNR=1;
	while( World.RuchNR < 100 ){
		call.GetLineOfLuckingTime(World.TimeLeft);
		World.ChooseTimMoveCoordinates(MyWriteBuff);
		call.writeLine(MyWriteBuff);
		call.readLine(MyReadBuff);
		World.RecordMovement(MyReadBuff);
	};
#if DEBUGIN==1 || DEBUGIN==2
		fx.close();
	} else {
		cout << "Error opening file";
	}
#endif
	return 0;
};
