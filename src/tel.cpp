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


#include "tel.h"
#include <iostream>
#include <ostream>

int fStaringWith;

	void tel::GetLineOfLuckingTime(int& TimeToEnd){
		std::cin >> TimeToEnd;
#if DEBUGIN==1
		pp(TimeToEnd);
#endif
	};

	void tel::writeLine(TBufor MMB){
#if DEBUGIN==1
		V6(MMB);
#endif
		for(int y=0;y<6;y++){MMB[y]++;};//dostosowanie do protokolu czyli od 1..10 a nie jak u mnie w programie od 0..9
		std::cout << MMB[0] << " " << MMB[1]<< " " << MMB[2]<< " " << MMB[3]<< " " << MMB[4]<< " " << MMB[5]<< std::endl;
		fflush(stdout);
	};

	void tel::readLine(TBufor buff){
#if DEBUGIN==1
		V6(buff);
#endif
		std::cin >> buff[0] >> buff[1] >> buff[2] >> buff[3] >> buff[4] >> buff[5] ;
		for(int i=0; i<6; i++){
			buff[i]--;
		};
	};

	bool tel::GOonGO(){
		return (fStaringWith==1);
	};

	tel::tel(int StartZNumerem) {
		if (!(StartZNumerem == 1 || StartZNumerem == 2)) {
#if DEBUGIN==1
			pp(StartZNumerem);
#endif
			std::cout <<"ERROR: Program start with bad number: " << StartZNumerem << " :D" << std::endl;
		};
	};

	tel::~tel() {
		std::cout << "Program end."<< std::endl;
	};
