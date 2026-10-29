
#include "path_origin.h"

class TPath{
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
	TPath(){};
	~TPath(){};
};