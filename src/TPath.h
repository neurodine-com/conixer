#include "fighting_structures.h"

class TPath {

  public:
	VV fVV;
	// represents a filed which is used to as a point from which a "BOMB" will be deployed 
	VV BombHQ;
	// fields Vect and VectCount should get cleaned and only field GroundZero set
	int PathForMixer;
	TPoint Org = {-1,-1};
	// there are just two players so bool is 
	// enough to assign value for each of the teams;
	//  true - tema1, false - team2
	bool Tim;
	bool TempNotIntresting;

	int PathCount();
	int ShootDirectionCount();
	void SetNewPos(TPoint NewPos);
	void SetNewPos(int _x, int _y);
	void ClearMixer_VV_ToStartup();
	void SetHQ(const TPoint HQCording);
	// Constructors and destructor
	TPath(bool MyTeam, int mx, int my, int TeamId);
	TPath(const TPath& other);
	~TPath();
};