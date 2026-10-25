#include "baseblock.h"


class BaseBlock{
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
	// BaseBlock(BaseBlock &&) = delete;
	// BaseBlock &operator=(const BaseBlock &) = default;
	// BaseBlock &operator=(BaseBlock &&) = delete;
	~BaseBlock(){};
};