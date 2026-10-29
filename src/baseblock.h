
#include "fighter.h"

class BaseBlock {

	public:

		bool BlockAccess();
		void SetAccessRights(bool value);
		void AfterMooveClearMutuals();
		// WariorsID_xxx[..] i inne okreslaja czy jednostka
		// tora wykonuje wirtualny strzal lub ruch np jednostka druzyny(Tim==true)
		// ma mozliwosc strzelenia w ten punkt
		void SetMutualFactor(int unitID, bool MyTeam, bool shooting);
		TPoint GetOrgin();
		void restBufer(TUID& Data);

		BaseBlock(int X, int Y, bool FieldIsAccesible);
		// BaseBlock(BaseBlock &&) = delete;
		BaseBlock &operator=(const BaseBlock &) = default;
		// BaseBlock &operator=(BaseBlock &&) = delete;
};