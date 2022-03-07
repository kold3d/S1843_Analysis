#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

  void current() {

//  dragon::RossumData rossumData("rossumData","/data/dragon/S1565/rossum/November2016/Ne22_beam.rossumData");
//	dragon::BeamNorm BeamNorm("BeamNorm","/data/dragon/S1565/rossum/November2016/Ne22_beam.rossumData");
  dragon::RossumData rossumData("rossumData","/data/astro/dragon/S1843/rossum/N14_beam_April2021.rossumData");
  dragon::BeamNorm BeamNorm("BeamNorm","/data/astro/dragon/S1843/rossum/N14_beam_April2021.rossumData");

	bool StartRunCurrent = true; // setting to false doesn't yet work. Need to discriminate runs which don't contain rossum data. 

	if(StartRunCurrent) {

		Int_t firstRun = 9851;
		Int_t lastRun = 10014;

		//Int_t firstRun = 5848;
		//Int_t lastRun = 5887;

	// list array of unused runs:
		Int_t badruns[] = {5775, 5780, 5863, 5864};
	}

	else {
	
	Int_t firstRun = 5755;
	Int_t lastRun = 5792;

	// list array of unused runs:
	Int_t badruns[] = {5775, 5780, 5863, 5864};
	}	
	
	// set up vector of runs:
	std::vector<int> runs;

	int i;

	size_t badrunsSize = sizeof(badruns) / sizeof(int);

	int *end = badruns + badrunsSize;

	// Fill vector only with good runs:	

		for(i=firstRun; i<=lastRun; i++) {

			int *foo = std::find(badruns, end, i);
			
			if (foo != end) { }
			
			else { runs.push_back(i); }
		}

	// Initialize current varriables. a = First FC4 iteration, b = Last FC4 iteration.
	Double_t I	= 0;
	Double_t Ierr	= 0;
	// Initialize output files:
	ofstream outfile ("current_14N.dat");
	// Check files open and set column titles:
	if ( outfile.is_open())	{ 

		outfile << "Run AvCurrent ErrCurrent" << endl;
	 
	}
	// If not open diplay error:
	else cout << "Unable to open output file" << endl;

	// For each run output average current and error for first and second iteration of FC4 cup reading.
	for(i=0; i < runs.size(); i++) {

		if(!StartRunCurrent){
		I  = rossumData.AverageCurrent(runs[i],0,2).GetNominal();
		Ierr = I*(rossumData.AverageCurrent(runs[i],0,2).GetRelErrHigh());
		//I2 = rossumData.AverageCurrent(runs[i],0,2).GetNominal();
		//I2err = I*(rossumData.AverageCurrent(runs[i],0,2).GetRelErrHigh());
		
		}
		else {
		I  = rossumData.AverageCurrent(runs[i],0,2).GetNominal();
		Ierr = I*(rossumData.AverageCurrent(runs[i],0,2).GetRelErrHigh());
		}
		outfile << runs.at(i) << " " << I << " " << Ierr  << " " << endl; // associated with end of last run so runs.at(i) -1.
	}
		
	outfile.close();

	cout << "Currents written to output files" << endl;
						
}
