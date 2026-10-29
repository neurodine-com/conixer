
#include "fighting_structures.h"
#include "BaseBlock.h"


	bool accessible;
	TPoint Cords;
	int EnemyMutualMov;
	int TimMutualMov;
	int EnemyShootMut;
	int TimShootMut;

	int FreeDiections;
	TUID EnemysID_MutualMov;
	TUID WariorsID_MutualMov;
	TUID EnemysID_ShootMutual;
	TUID WariorsID_ShootMutual;

bool BaseBlock::BlockAccess() {
	return accessible;
};
void BaseBlock::SetAccessRights(bool value) {
	accessible=value;
	if(value==false) {
		AfterMooveClearMutuals();
	};
};
void BaseBlock::AfterMooveClearMutuals() {
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
void BaseBlock::SetMutualFactor(int unitID, bool MyTeam, bool shooting) {
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
TPoint BaseBlock::GetOrgin() {
	return Cords;
};
void BaseBlock::restBufer(TUID& Data) {
	Data[0]=0;
	Data[2]=0;
	Data[1]=0;
	Data[3]=0;
};
BaseBlock::BaseBlock(int X, int Y, bool FieldIsaccessible) {
	accessible=FieldIsaccessible;
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
BaseBlock::BaseBlock() {
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
// BaseBlock &operator=(const BaseBlock &) = default;
// BaseBlock &operator=(BaseBlock &&) = delete;