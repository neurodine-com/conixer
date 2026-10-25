
#include "fighter.h"

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
	bool BlockAccess();
	void SetAccessRights(bool value);
	void AfterMooveClearMutuals();
	// WariorsID_xxx[..] i inne okreslaja czy jednostka
	// tora wykonuje wirtualny strzal lub ruch np jednostka druzyny(Tim==true)
	// ma mozliwosc strzelenia w ten punkt
	void SetMutualFactor(int unitID, bool MyTeam, bool shooting);
	TPoint GetOrgin();
	BaseBlock(int X, int Y, bool FieldIsAccesible);
	void restBufer(TUID& Data);
	BaseBlock();
	BaseBlock(const BaseBlock &) = default;
	// BaseBlock(BaseBlock &&) = delete;
	// BaseBlock &operator=(const BaseBlock &) = default;
	// BaseBlock &operator=(BaseBlock &&) = delete;
	~BaseBlock();
};