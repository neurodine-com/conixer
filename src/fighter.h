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