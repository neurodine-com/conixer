
#include "TPath.h"

int TPath::PathCount() {
	return fVV.VectCount;
}

int TPath::ShootDirectionCount() {
	return BombHQ.VectCount;
}

void TPath::SetNewPos(TPoint NewPos) {
	SetNewPos(NewPos.x, NewPos.y);
}

void TPath::SetNewPos(int _x, int _y) {
	Org.x = _x;
	Org.y = _y;
	for (int p = 0; p < 8; p++) {
		fVV.Vect[p].radius = -1;
		fVV.Vect[p].Mask = p;
		fVV.Vect[p].DestPoint = Org;
	}
	fVV.Ground00 = Org;
	fVV.VectCount = 0;
}

void TPath::ClearMixer_VV_ToStartup() {
	for (int p = 0; p < 8; p++) {
		fVV.Vect[p].radius = -1;
		fVV.Vect[p].Mask = -1;
		fVV.Vect[p].DestPoint = Org;
	}
	fVV.Ground00 = Org;
	fVV.VectCount = 0;
}

void TPath::SetHQ(const TPoint HQCording) {
	BombHQ.Ground00 = HQCording;
	BombHQ.VectCount = 0;
	for (int z = 0; z < 8; z++) {
		BombHQ.Vect[z].DestPoint = HQCording;
		BombHQ.Vect[z].Mask = -1;
		BombHQ.Vect[z].radius = -1;
	}
}

TPath::TPath(bool MyTeam, int mx, int my, int TeamId) {
	Tim = MyTeam;
	Org.x = mx;
	Org.y = my;
	PathForMixer = TeamId;
	TempNotIntresting = false;
	SetNewPos(Org);
}

TPath::TPath(const TPath& other) = default;

TPath::~TPath() = default;
