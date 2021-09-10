{

int firstRun=9916, lastRun=9922, goodpoints=0;
	vector<int> runs;

	for (int i = firstRun; i<=lastRun; i++){
		if( i==9993 || i==9994 || i==9995 || i==9996 || i==9997 || i==9998 || i==10000 || i==10001 || i==10002 || i==10003 || i==10004 || i==10005) {
            continue;}
		runs.push_back(i);
	}
    std::cout << "runsize: " << runs.size() << std::endl;
    int i = i;
    char fname[4096];

    ofstream outfile;
    outfile.open ("output_ssb.dat");

	Int_t N;
	Double_t Nsb1, Nsb0, Nsb1_err, Nsb0_err, event0, event0_120, eventf, eventf_120, Nsb1_tot, Nsb0_tot, Nsb1_t0, Nsb1_t0_err, Nsb1_t1, Nsb1_t1_err, Nsb0_t0, Nsb0_t0_err, Nsb0_t1, Nsb0_t1_err;

	if ( outfile.is_open() ) {
		outfile << "Run Nsb0_tot Nsb0_tot_err Nsb0_start Nsb0_start_err Nsb0_end Nsb0_end_err Nsb1_tot Nsb1_tot_err Nsb1_start Nsb1_start_err Nsb1_end Nsb1_end_err" << endl;
	}

	// If not open diplay error:
	else std::cout << "Unable to open output file" << std::endl;

    std::cout << "Working . . . " << std::endl;

//    TH1F *sb0Total = new TH1F("sb0Total","SB0_Total", 256, 0, 4096);
//    for (i = 0; i < runs.size(); i++) {
//        TH1F *sb0_temp[i];
//        TString name = Form("sb0_%d", i);
//        sb0_temp[i] = new TH1F(name,name,256,0,4096);
//    }
    for( i=0; i < runs.size(); i++) {

	sprintf(fname,"/data/astro/dragon/S1843/data/rootfiles/run%i.root", runs.at(i));
//	sprintf(fname,"run%i.root", runs.at(i));

	TFile *file=TFile::Open(fname);

		cout << "Run: " << runs.at(i) << endl;

		// Get time stamp for end of first 2 mins
        t3->GetEntry(0);
		event0 = t3->GetLeaf("header.fTimeStamp")->GetValue();
        cout << "event0: " << event0 << endl;
		event0_120 = event0+300;

		// get time stamp for start pf last 2 mins.
		N = t3->GetEntries();
		t3->GetEntry(N-1);
		eventf	= t3->GetLeaf("header.fTimeStamp")->GetValue();
        cout << "eventf: " << eventf << endl;
		eventf_120 = eventf-300;

		// Make time cuts for first and last 2 mins.
		TCut start_time = Form("header.fTimeStamp<=%f", event0_120);
		TCut end_time = Form("header.fTimeStamp>=%f", eventf_120);
		TCut time = Form("header.fTimeStamp>=%f", event0);

		TCut sb0cut = "sb.ecal[0]>2400 && sb.ecal[0]<2750";
		//TCut sb0cut = "nai.ecal[1]>1300 && sb.ecal[0]<2400";
		TCut sb1cut = "sb.ecal[1]>2200 && sb.ecal[1]<3000";
		//TCut sb1cut = "sb.ecal[1]>1000 && sb.ecal[1]<2700";

		// Apply energy and time cuts on SB spectrum.

	    TCanvas *c1 = new TCanvas();
        //
        TString sb0Run = Form("SB0_Run%d", runs.at(i));
        TH1F *sb0 = new TH1F("sb0","SB0", 256, 0, 4096);
        TH1F *sb0_t0 = new TH1F("sb0_t0","SB0_t0", 256, 0, 4096);
        TH1F *sb0_t1 = new TH1F("sb0_t1","SB0_t1", 256, 0, 4096);

		t3->Draw("sb.ecal[0]>>sb0",time + sb0cut,"goff");
//		t3->Draw("sb.ecal[0]>>sb0",time,"goff");
		t3->Draw("sb.ecal[0]>>sb0_t0",start_time + sb0cut,"goff");
		t3->Draw("sb.ecal[0]>>sb0_t1",end_time + sb0cut, "goff");

		sb0->SetTitle(sb0Run);
		sb0->GetXaxis()->SetTitle("ADC chan");
		sb0->GetYaxis()->SetTitle("Counts (16 chan/bin)");
        sb0->Draw();


		Nsb0 = sb0->GetEntries();
		Nsb0_err = sqrt(Nsb0);
		Nsb0_t0 = sb0_t0->GetEntries();
		Nsb0_t0_err = sqrt(Nsb0_t0);
		Nsb0_t1 = sb0_t1->GetEntries();
		Nsb0_t1_err = sqrt(Nsb0_t1);

		cout << "SB0 Entries: " << Nsb0 << endl;

		Nsb0_tot += Nsb0;

		//TCanvas *c2 = new TCanvas();

        TString sb1Run = Form("SB1_Run%d", runs.at(i));
        TH1F *sb1 = new TH1F("sb1","SB0", 256, 0, 4096);
        TH1F *sb1_t0 = new TH1F("sb1_t0","SB0_t0", 256, 0, 4096);
        TH1F *sb1_t1 = new TH1F("sb1_t1","SB0_t1", 256, 0, 4096);


		t3->Draw("sb.ecal[1]>>sb1",time + sb1cut,"goff");
//		t3->Draw("sb.ecal[1]>>sb1",time,"goff");
		t3->Draw("sb.ecal[1]>>sb1_t0",start_time + sb1cut,"goff");
		t3->Draw("sb.ecal[1]>>sb1_t1",end_time + sb1cut,"goff");

        sb1->SetTitle(sb1Run);
		sb1->GetXaxis()->SetTitle("ADC chan");
		sb1->GetYaxis()->SetTitle("Counts (16 chan/bin)");
		sb1->SetLineColor(2);
        sb1->Draw("same");

		TLegend *leg0 = new TLegend(0.6,0.7,0.9,0.9);
		leg0->SetHeader("Legend"); // option "C" allows to center the header
   		leg0->AddEntry(sb1,"SB1","l");
   		leg0->AddEntry(sb0,"SB0","l");
   		leg0->Draw();
	    //TCanvas *c3 = new TCanvas();
        //sb0->Draw();
        //sb1->Draw("same");

		//TLegend *leg1 = new TLegend(0.6,0.7,0.9,0.9);
		//leg1->SetHeader("Legend"); // option "C" allows to center the header
   		//leg1->AddEntry(sb1,"SB1","l");
   		//leg1->AddEntry(sb0,"SB0","l");
   		//leg1->Draw();

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
