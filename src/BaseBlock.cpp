#include "BaseBlock.h"

bool BaseBlock::BlockAccess() const {
	return accessible;
}

void BaseBlock::SetAccessRights(bool value) {
	accessible = value;
	if (!value) {
		AfterMooveClearMutuals();
	}
}

void BaseBlock::AfterMooveClearMutuals() {
	EnemyMutualMov = 0;
	TimMutualMov = 0;
	EnemyShootMut = 0;
	TimShootMut = 0;
	restBufer(EnemysID_MutualMov);
	restBufer(WariorsID_MutualMov);
	restBufer(EnemysID_ShootMutual);
	restBufer(WariorsID_ShootMutual);
}

void BaseBlock::SetMutualFactor(int unitID, bool MyTeam, bool shooting) {
	if (shooting) {
		if (MyTeam) {
			++TimShootMut;
			WariorsID_ShootMutual[unitID] = 1;
		} else {
			++EnemyShootMut;
			EnemysID_ShootMutual[unitID] = 1;
		}
	} else if (MyTeam) {
		++TimMutualMov;
		WariorsID_MutualMov[unitID] = 1;
	} else {
		++EnemyMutualMov;
		EnemysID_MutualMov[unitID] = 1;
	}
}

TPoint BaseBlock::GetOrgin() const {
	return Cords;
}

void BaseBlock::restBufer(TUID& Data) {
	for (int& value : Data) {
		value = 0;
	}
}

BaseBlock::BaseBlock(int x, int y, bool fieldIsAccessible)
	: accessible(fieldIsAccessible), Cords{x, y} {}
