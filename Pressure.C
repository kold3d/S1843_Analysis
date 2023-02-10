#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

void Pressure(){

  //        dragon::RossumData rossumData("rossumData","/data2/dragon/S1565/rossum/November2016/Ne22_beam.rossumData");
  //      dragon::BeamNorm BeamNorm("BeamNorm","/data2/dragon/S1565/rossum/November2016/Ne22_beam.rossumData");
  dragon::RossumData rossumData("rossumData","/data/astro/dragon/S1843/rossum/N14_beam_April2021.rossumData");
  dragon::BeamNorm BeamNorm("BeamNorm","/data/astro/dragon/S1843/rossum/N14_beam_April2021.rossumData");

//For 265 keV/u
				 Int_t firstRun = 9821;
        Int_t lastRun = 10014;
//For 483.5 keV/u:
				//Int_t firstRun = 5192;
				//Int_t lastRun = 5198;
//For 250.5 keV/u
				//Int_t firstRun = 5241;
				//Int_t lastRun = 5253;
// For 191.3 keV/u
				//	Int_t firstRun = 5259;
				//Int_t lastRun = 5264;
// For 196.27 keV/u
				//Int_t firstRun = 5268;
				//Int_t lastRun = 5287;
//For 660.53 keV/u at 5 Torr
				//	Int_t firstRun = 5169;
				//Int_t lastRun = 5170;
//For 660.53 keV/u at 3 Torr
				//Int_t firstRun = 5177;
				//Int_t lastRun = 5177;
//For 660.53 keV/u at 2.2Torr
				//	Int_t firstRun = 5187;
				//Int_t lastRun = 5187;
				
				
        // list array of unused runs:
        Int_t badruns[] = {9828, ,9829, 9831, 9832, 9833, 9834, 9835, 9836, 9837, 9838, 9839, 9852,9853,9870,9872,9873,9874,9876,9877,9880,9885,9886,9887,9888,9895,9913,9914,9925,9937,9958,9960,9967,9968,9969,9970,9972,9975,9976,
                            9977,9978,9979,9980,9981,9982,9984,9985,9986,9995,10008,10009};
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

        char fname0[4094];

        i=0;

        ofstream outfile ("pressure.dat");

        if ( outfile.is_open() ) { 
                outfile << "Run AvPressure error First2mins error Last2mins error" << endl;
        }

        // If not open diplay error:
        else cout << "Unable to open output file" << endl;

        cout << "Working . . . " << endl;

        for( i=0; i < runs.size(); i++) {

        sprintf(fname0,"/data/astro/dragon/S1843/data/rootfiles/run%i.root", runs.at(i));

        TFile *file0=TFile::Open(fname0);
        cout << "Run Number: " << runs.at(i) << endl;

        t20->GetEntry(0);
        Double_t event0 = t20->GetLeaf("header.fTimeStamp")->GetValue();
        Double_t event0_120 = event0+120;
        //TTimeStamp event0_ts=TTimeStamp(event0, kTRUE, -7*3600, kFALSE);
        //TTimeStamp event0_ts_120 = TTimeStamp(event0_120,kTRUE, -7*3600, kFALSE);
        //TString event0_string(event0_ts.AsString());
        //TString event0_120_string(event0_ts_120.AsString());
        //event0_string->Resize(25);
        //event0_120_string->Resize(25);

        Int_t N=t20->GetEntries();
        t20->GetEntry(N-1);
        Double_t eventf = t20->GetLeaf("header.fTimeStamp")->GetValue();
        Double_t eventf_120 = eventf-120;

        Double_t P0, P1, P2, sdP0, sdP1, sdP2;

				TCanvas *c0 = new TCanvas();
        t20->Draw("val>>hP0",Form("ch==%i && header.fTimeStamp>=%f",0,event0),"goff"); //Get pressure for entire run.
        P0 = hP0->GetMean();
        sdP0 = hP0->GetRMS();
        
        //t20->Draw("val:header.fTimeStamp>>hP0",Form("ch==%i",0),"g");        // draw pressure profile for entire run.
					TCanvas *c1 = new TCanvas();
        t20->Draw("val>>hP1",Form("ch==%i && header.fTimeStamp<=%f",0,event0+120),"goff");
// Get pressure for first 2 mins of run.
        P1 = hP1->GetMean();
        sdP1 = hP1->GetRMS();
					TCanvas *c2 = new TCanvas();
        t20->Draw("val>>hP2",Form("ch==%i && header.fTimeStamp>=%f",0,eventf_120),"goff");
// get pressure for last 2 mins of run.
        P2 = hP2->GetMean();
        sdP2 = hP2->GetRMS();

        outfile << runs.at(i) << " " << P0 << " " << sdP0 << " " << P1 << " " << sdP1 << " " << P2 << " " << sdP2 << endl;
		
        }
		cout <<"Pressure data written to txt file...done!" << endl;
}				/*  i=0;

        //for( i=0; i < runs.size(); i++) {

        sprintf(fname0,"/data2/dragon/S1565/data/rootfiles/run%i.root", runs.at(i));

        TFile *file0=TFile::Open(fname0);

        Double_t deltaT = 60;
        t20->GetEntry(0);
        Double_t event0 = t20->GetLeaf("header.fTimeStamp")->GetValue();
        Int_t N=t20->GetEntries();
        t20->GetEntry(N-1);
        Double_t eventf = t20->GetLeaf("header.fTimeStamp")->GetValue();
        Double_t runtime = eventf - event0; 
        Double_t eventj;
        Double_t P[60];
        //Double_t Pdat;
				cout <<"got to here" << endl;
        Int_t j;

        eventj = event0 + j*deltaT;
        t20->Draw("val>>hP0",Form("ch==%i && header.fTimeStamp<=%f",0,eventj));

                for(j=0; j < 60; j++) {
                        
                        eventj = event0 + j*deltaT;
                        t20->Draw("val>>hP0",Form("ch==%i && header.fTimeStamp<=%f",0,eventj),"goff"); //Get pressure for entire run.
                        P[j] = hP0->GetMean();
                        cout << "P[" << j << "]: " << P[j] << endl; 
                }
        //}         

        outfile.close();

        cout << "Pressures written to output file." << endl;

}
				*/
