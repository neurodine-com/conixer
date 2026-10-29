#pragma once

using TBufor = int[6];
using TUID = int[4];

struct TPoint {
	int x{};
	int y{};
};

struct TPoints {
	TPoint point[4]{};
};

struct SimpPath {
	int radius{};
	TPoint DestPoint{};
	int Mask{};
};

struct VV {
	SimpPath Vect[8]{};
	int VectCount{};
	TPoint Ground00{};
};

struct TAtackHQ {
	TPoint NewORGForWariorUnit{};
	int PosibleFieldsCount{};
	TUID UsableMasksIDsVector{};
};

using TAtack = TAtackHQ[4];

inline constexpr TUID ZeroThreat = {0, 0, 0, 0};
inline constexpr TPoint NullPoint = {-1, -1};
inline constexpr TPoint ZeroPoint = {0, 0};
inline constexpr TPoints constWariorLoc{{{3, 0}, {6, 0}, {0, 3}, {9, 3}}};
inline constexpr TPoints constEnemyLoc{{{0, 0}, {0, 0}, {0, 0}, {0, 0}}};
inline constexpr TPoint cMoveBlur[8] = {
	{0, 0}, {0, 0}, {0, 0}, {0, 0},
	{0, 0}, {0, 0}, {0, 0}, {0, 0}
};
inline constexpr SimpPath Starter{-1, {-1, -1}, -1};
