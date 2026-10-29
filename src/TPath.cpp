
#include "TPath.h"

int TPath::PathCount() {
	return fVV.VectCount;
};

int TPath::ShootDirectionCount() {
	return BombHQ.VectCount;
};

void TPath::SetNewPos(TPoint NewPos) {
	SetNewPos(NewPos.x, NewPos.y);
};

void TPath::SetNewPos(int _x, int _y) {
	this->Org.x = _x;
	this->Org.y = _y;
	for(int p=0; p<8; p++) {
		this->fVV.Vect[p].radius = -1;
		this->fVV.Vect[p].Mask = p;
		this->fVV.Vect[p].DestPoint = this->Org;
	};
	this->fVV.Ground00 = this->Org;
	this->fVV.VectCount = 0;
};

void TPath::ClearMixer_VV_ToStartup() {
	for(int p=0; p<8; p++) {
		this->fVV.Vect[p].radius = -1;
		this->fVV.Vect[p].Mask = -1;
		this->fVV.Vect[p].DestPoint = this->Org;
	};
	this->fVV.Ground00=this->Org;
	this->fVV.VectCount=0;
};

void TPath::SetHQ(const TPoint HQCording) {
	this->BombHQ.Ground00 = HQCording;
	this->BombHQ.VectCount = 0;
	for(int z=0; z<8; z++) {
		this->BombHQ.Vect[z].DestPoint=HQCording;
		this->BombHQ.Vect[z].Mask=-1;
		this->BombHQ.Vect[z].radius=-1;
	};
};

TPath::TPath(bool MyTeam, int mx, int my, int TeamId) {
	this->Tim = MyTeam;
	this->Org.x = mx;
	this->Org.y = my;
	this->PathForMixer = TeamId;
	this->TempNotIntresting = false;
	SetNewPos(this->Org);
};