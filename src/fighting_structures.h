
#pragma once

typedef int TBufor[6];
typedef int TUID[4];

typedef struct{
int x; int y;
} TPoint;

typedef struct{
TPoint point[4];
} TPoints;

// typedef TPoint T8Points[8];

// typedef struct {
// 	int radius;// denotes max path length aka max vector length
// 	TPoint DestPoint;//denotes a max point on FieldMap that the "ConcrateMixer" is managed to move
// 	int Mask;
// } SimpPath;//Simple Bath aka. Basic Path

//Simple Bath aka. Basic Path
typedef struct {
	// denotes max path length aka max vector length
	int radius;
	//denotes a max point on FieldMap that the "ConcrateMixer" is managed to move
	TPoint DestPoint;
	int Mask;
} SimpPath;
typedef struct{
	// a vector of possible maximum paths - thanks to radious z CMoveblur[.x] can set any coordinates on the filed on which the item Path has to move - that is my "Concrate Mixer"
	SimpPath Vect[8];
	// takes a value that represents number of vectors found with possible moves
	int VectCount;
	// represents a point (TPoint) for which all the above computations have been calculated
	TPoint Ground00;
} VV;

typedef struct{
	TPoint NewORGForWariorUnit;
	int PosibleFieldsCount;
	TUID UsableMasksIDsVector;
} TAtackHQ;


const TUID ZeroThreat = {0, 0, 0, 0};
const TPoint NullPoint = {-1, -1};
const TPoint ZeroPoint = {0, 0};
const TPoints constWariorLoc = {
	{
		{3, 0},
		{6, 0},
		{0, 3},
		{9, 3}
	}
};
const TPoints constEnemyLoc = {
	{
		{0, 0}, // point[0]
		{0, 0}, // point[1]
		{0, 0}, // point[2]
		{0, 0}  // point[3]
	}
};
// const TPoint cMoveBlur[8] = {0};
// const TPoint cMoveBlur[8] = {{0}};
const TPoint cMoveBlur[8] = {
	{0, 0}, // [0]
	{0, 0}, // [1]
	{0, 0}, // [2]
	{0, 0}, // [3]
	{0, 0}, // [4]
	{0, 0}, // [5]
	{0, 0}, // [6]
	{0, 0}  // [7]
};

const SimpPath Starter={-1, (TPoint){-1,-1}, -1};
