#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int StaringWith;
typedef int TBufor[6];



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

#include <src/tel.h>
#include <src/FieldSpace.h>
#include <src/path_finder.h>



int MaxMoves;


int main(int argc, char *argv[]){

MaxMoves=92;
	cin >> StaringWith;

#if DEBUGIN==1 || DEBUGIN==2
if(StaringWith==1){
	fx.open("logout/smDebug1.log", ios::trunc);
	}
	else
	{
		fx.open("logout/smDebug2.log", ios::trunc);
		}
if(fx.is_open()){
#endif

		tel call=tel::tel(StaringWith);
		CPathFinder World=CPathFinder::CPathFinder(StaringWith);

	TBufor MyReadBuff={0,0,0,0,0,0};
	TBufor MyWriteBuff={0,3,0,2,0,3};

	if(StaringWith==1){
		call.GetLineOfLuckingTime(World.TimeLeft);
		World.ChooseTimMoveCoordinates(MyWriteBuff);


		call.writeLine(MyWriteBuff);
		call.readLine(MyReadBuff);

		World.RecordMovement(MyReadBuff);
		}
		else
			{	call.readLine(MyReadBuff);
				World.RecordMovement(MyReadBuff);
				};

	World.RuchNR=1;
	while(World.RuchNR<100){

		call.GetLineOfLuckingTime(World.TimeLeft);

		World.ChooseTimMoveCoordinates(MyWriteBuff);
        call.writeLine(MyWriteBuff);

		call.readLine(MyReadBuff);
		World.RecordMovement(MyReadBuff);
		};

#if DEBUGIN==1 || DEBUGIN==2
	fx.close();
	}

	else
  {
    cout << "Error opening file";
  }
#endif

	return 0;

};
