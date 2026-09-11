using namespace std;

class tel{
	protected:
	int fStaringWith;
	public:

	void GetLineOfLuckingTime(int& TimeToEnd){
		cin >> TimeToEnd;
		#if DEBUGIN==1
		pp(TimeToEnd);
		#endif
		};

	void writeLine(TBufor MMB){
		#if DEBUGIN==1
		V6(MMB);
		#endif
		for(int y=0;y<6;y++){MMB[y]++;};//dostosowanie do protokolu czyli od 1..10 a nie jak u mnie w programie od 0..9
		cout << MMB[0] << " " << MMB[1]<< " " << MMB[2]<< " " << MMB[3]<< " " << MMB[4]<< " " << MMB[5]<< endl;
		fflush(stdout);
		};

	void readLine(TBufor buff){
		#if DEBUGIN==1
		V6(buff);
		#endif
		cin >> buff[0] >> buff[1] >> buff[2] >> buff[3] >> buff[4] >> buff[5] ;
		for(int i=0; i<6;i++){buff[i]--;};
		};


	bool GOonGO(){
		return (fStaringWith==1);
	};


	tel(int StartZNumerem){
		if(!(StartZNumerem==1 || StartZNumerem==2)){
		#if DEBUGIN==1
		pp(StartZNumerem);
		#endif
		cout <<"ERROR: Program start with bad number: " << StartZNumerem << " :D" << endl;
		};
		};

	~tel(){
		cout << "Program end."<< endl;
		};

	};