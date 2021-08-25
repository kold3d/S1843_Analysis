#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include "TFile.h"
using namespace std;

void ssbcounts(){

// First = 8795
// Last = 8936
// 8823

int firstRun=9957, lastRun=9973, goodpoints=0;
	vector<int> runs;

	//for (int i = firstRun; i<=lastRun; i++){
	//	if(i>8799 && i<8803 || i==8804 || i==8808 || i>8812 && i<8818 || i==8833 || i>8834 && i<8843 || i==8844 || i==8852 || i==8853 || i>8864 && i<8871 || i>8871 && i<8875 || i==8878 || i==8879 || i==8882 || i==8893 || i>8897 && i<8911 || i==8921 || i>8922 && i<8925 || i>8925 && i<8930) { continue; }			
	for (int i = firstRun; i<=lastRun; i++){
		if( i==9968 || i==9969) {
            continue;} 
		runs.push_back(i);
	}

	int i = 1;
     	char fname[4096];

	ofstream outfile;
	outfile.open ("output_ssb.dat");
	
	Int_t N;
	Double_t Nsb1, Nsb0, Nsb1_err, Nsb0_err, event0, event0_120, eventf, eventf_120, Nsb1_tot, Nsb0_tot, Nsb1_t0, Nsb1_t0_err, Nsb1_t1, Nsb1_t1_err, Nsb0_t0, Nsb0_t0_err, Nsb0_t1, Nsb0_t1_err;

	if ( outfile.is_open() ) { 
		outfile << "Run Nsb0_tot Nsb0_tot_err Nsb0_start Nsb0_start_err Nsb0_end Nsb0_end_err Nsb1_tot Nsb1_tot_err Nsb1_start Nsb1_start_err Nsb1_end Nsb1_end_err" << endl;
	}

	// If not open diplay error:
	else cout << "Unable to open output file" << endl;

	cout << "Working . . . " << endl;

	for( i=0; i < runs.size(); i++) {

//	sprintf(fname,"/data/astro/dragon/S1843/data/rootfiles/run%i.root", runs.at(i));
	sprintf(fname,"/data/astro/dragon/S1843/data/rootfiles/run9957.root", runs.at(i));

	TFile *file=TFile::Open(fname);

		cout << "Run: " << runs.at(i) << endl;

		// Get time stamp for end of first 2 mins
		t3->GetEntry(0);
		event0 = t3->GetLeaf("header.fTimeStamp")->GetValue();
		event0_120 = event0+300;

		// get time stamp for start pf last 2 mins.
		N = t3->GetEntries();
		t3->GetEntry(N-1);
		eventf	= t3->GetLeaf("header.fTimeStamp")->GetValue();
		eventf_120 = eventf-300;
		

		// Make time cuts for first and last 2 mins.
		TCut start_time = Form("header.fTimeStamp<=%f", event0_120);
		TCut end_time = Form("header.fTimeStamp>=%f", eventf_120);
		TCut time = Form("header.fTimeStamp>=%f", event0);
		
		TCut sb0cut = "sb.ecal[0]>1700 && sb.ecal[0]<2800";
		//TCut sb0cut = "nai.ecal[1]>1300 && sb.ecal[0]<2400"; 
		TCut sb1cut = "sb.ecal[1]>1000 && sb.ecal[1]<2700";
		
		// Apply energy and time cuts on SB spectrum.
		
		//TCanvas *c1 = new TCanvas();

		//t3->Draw("nai.ecal[1]>>sb0(128,0,4096)",time + sb0cut,"goff");
		//t3->Draw("nai.ecal[1]>>sb0_t0(256,0,4096)",start_time + sb0cut,"goff");
		//t3->Draw("nai.ecal[1]>>sb0_t1(256,0,4096)",end_time + sb0cut, "goff");

		t3->Draw("sb.ecal[0]>>sb0(256,0,4096)",time + sb0cut,"goff");
		t3->Draw("sb.ecal[0]>>sb0_t0(256,0,4096)",start_time + sb0cut,"goff");
		t3->Draw("sb.ecal[0]>>sb0_t1(256,0,4096)",end_time + sb0cut, "goff");

		sb0->SetTitle("SB0");
		sb0->GetXaxis()->SetTitle("ADC chan");
		sb0->GetYaxis()->SetTitle("Counts (16 chan/bin)");

		Nsb0 = sb0->GetEntries();
		Nsb0_err = sqrt(Nsb0);
		Nsb0_t0 = sb0_t0->GetEntries();
		Nsb0_t0_err = sqrt(Nsb0_t0);
		Nsb0_t1 = sb0_t1->GetEntries();
		Nsb0_t1_err = sqrt(Nsb0_t1);
	
		cout << "SB0 Entries: " << Nsb0 << endl;

		Nsb0_tot += Nsb0;
/*
		TLine *l1 = new TLine(1000,0,1000,200000);
		TLine *l2 = new TLine(2500,0,2500,200000);

		l1->SetLineColor(2);
		l2->SetLineColor(2);

		l1->SetLineStyle(2);
		l2->SetLineStyle(2);

		l1->SetLineWidth(2);
		l2->SetLineWidth(2);

		l1->Draw();
		l2->Draw();
*/
		//TCanvas *c2 = new TCanvas();
		
		t3->Draw("sb.ecal[1]>>sb1(256,0,4096)",time + sb1cut,"goff");
		t3->Draw("sb.ecal[1]>>sb1_t0(256,0,4096)",start_time + sb1cut,"goff");
		t3->Draw("sb.ecal[1]>>sb1_t1(256,0,4096)",end_time + sb1cut,"goff");

		sb1->SetTitle("SB1");
		sb1->GetXaxis()->SetTitle("ADC chan");
		sb1->GetYaxis()->SetTitle("Counts (16 chan/bin)");
		sb1->SetLineColor(1);

		TLegend *leg0 = new TLegend(0.6,0.7,0.9,0.9);
		leg0->SetHeader("Legend"); // option "C" allows to center the header
   		leg0->AddEntry(sb1,"SB1","l");
   		leg0->AddEntry(sb0,"SB0","l");
   		//leg0->Draw();
/*
		TLine *l3 = new TLine(1000,0,1000,200000);
		TLine *l4 = new TLine(3000,0,3000,200000);

		l3->SetLineColor(2);
		l4->SetLineColor(2);

		l3->SetLineStyle(2);
		l4->SetLineStyle(2);

		l3->SetLineWidth(2);
		l4->SetLineWidth(2);

		l3->Draw();
		l4->Draw();
*/
		Nsb1 = sb1->GetEntries();
		Nsb1_err = sqrt(Nsb1);
		Nsb1_t0 = sb1_t0->GetEntries();
		Nsb1_t0_err = sqrt(Nsb1_t0);
		Nsb1_t1 = sb1_t1->GetEntries();
		Nsb1_t1_err = sqrt(Nsb1_t1);

		cout << "SB1 Entries: " << Nsb1 << endl;

		Nsb1_tot += Nsb1;

		outfile << runs.at(i) << " " << Nsb0 << " " << Nsb0_err << " " << Nsb0_t0 << " " << Nsb0_t0_err << " " << Nsb0_t1 << " " << Nsb0_t1_err << " " << Nsb1 << " " << Nsb1_err << " " << Nsb1_t0 << " " << Nsb1_t0_err << " " << Nsb1_t1 << " " << Nsb1_t1_err << endl;

	}
}
