// --------------------------------------------------------------------------------------------------------
// Author:          Blazej Kaczmarek
// ORCID:           https://orcid.org/0009-0003-1070-8870
// email:           fajny.masz.tornister@gmail.com
// github ord email:MasterDisaster@neurodine.com
// domain email:    Blazej.Kaczmarek@neurodine.com
// oid-tree:        http://oid-info.com/get/1.3.6.1.4.1.53685
// PEN:             1.3.6.1.4.1.53685
// oid ASN.1:       {iso(1) identified-organization(3) dod(6) internet(1) private(4) enterprise(1) 53685}
// Description:     So this is a file called "tel" like a "telephone" it's purpose is to d othe communication
// 		   with the applet that runs the game between players. So this is to read properly from STDIN
// 		   and write properly to stdout.
// 
// ---------------------------------------------------------------------------------------------------------

#ifndef CONIXER_TEL_H
#define CONIXER_TEL_H

#include "fighting_structures.h"

class tel {
	public:
		int fStaringWith{};

	void GetLineOfLuckingTime(int& TimeToEnd);
	void writeLine(TBufor MMB);
	void readLine(TBufor buff);
	bool GOonGO();
	explicit tel(int StartZNumerem);
	~tel();
};

#endif // CONIXER_TEL_H