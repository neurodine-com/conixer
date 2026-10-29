#pragma once

#include "fighting_structures.h"

class BaseBlock {
public:
	bool accessible{true};
	TPoint Cords{-1, -1};
	int EnemyMutualMov{};
	int TimMutualMov{};
	int EnemyShootMut{};
	int TimShootMut{};
	int FreeDirections{};
	TUID EnemysID_MutualMov{};
	TUID WariorsID_MutualMov{};
	TUID EnemysID_ShootMutual{};
	TUID WariorsID_ShootMutual{};

	BaseBlock() = default;
	explicit BaseBlock(int x, int y, bool fieldIsAccessible);

	bool BlockAccess() const;
	void SetAccessRights(bool value);
	void AfterMooveClearMutuals();
	void SetMutualFactor(int unitID, bool MyTeam, bool shooting);
	TPoint GetOrgin() const;
	static void restBufer(TUID& Data);

	BaseBlock& operator=(const BaseBlock&) = default;
};
