void Temp(){

	gStyle->SetOptStat(1111);
	Char_t fname[1024];
	const char *format = "~/../../data/astro/dragon/S1843/data/rootfiles/run%i.root";
	//Int_t firstRun = 5697, lastRun = 5697;
	//Int_t firstRun = 5187, lastRun = 5187;
	//Int_t firstRun = 5192, lastRun = 5198; //For Ecm=458 keV
	//Int_t firstRun = 5213, lastRun = 5236; //For Ecm=249 keV
  //Int_t firstRun = 5268, lastRun = 5287; //For Ecm=181 keV
//	Int_t firstRun = 5658, lastRun = 5665; //For Ecm=1.222 keV
	//Int_t firstRun = 5705,   lastRun = 5709; //For Ecm=0.510 keV

    int firstRun=10001; lastRun=10001;
    vector<int> runs;

   for (int i = firstRun; i<=lastRun; i++){
      if((i==7911 || i==7910 || (i>=7902 && i<=7905) || (i>=7850 && i<=7856) || i==7858 || i==7861 || i==7864 || (i>=7875 && i<=7877) ) )
        {
        continue;
      }
      cout << i << "\n";
      runs.push_back(i);
}

  Double_t sumrun[3]   = {0,0,0};
  Double_t sumbusy[3]  = {0,0,0};
  Double_t livetime[3] = {0,0,0};
  Double_t runtime[3]  = {0,0,0};
  Double_t busytime[3] = {0,0,0};
  
  for(Int_t i = 0; i < runs.size(); ++i){
    
    sprintf(fname,format,runs.at(i));

    //===== Open file0 =====\\
    TFile *file0=TFile::Open(fname);
    if(file0 == 0) {
      cout << "Error opening file run" << runs[i]-1 << ".root; skipping \n";
      continue;
    }

		
  
		
    dragon::LiveTimeCalculator LTC(file0,1);
    sumrun[0]  += LTC.GetRuntime("head");
    sumrun[1]  += LTC.GetRuntime("tail");
    sumrun[2]  += LTC.GetRuntime("coinc");
    sumbusy[0] += LTC.GetBusytime("head");
    sumbusy[1] += LTC.GetBusytime("tail");
    sumbusy[2] += LTC.GetBusytime("coinc");

		cout << "busytime tail"<< " " << runs.at(i) << " "  << sumbusy[1] << endl;
		cout << "busytime head"<< " " << runs.at(i) << " "  << sumbusy[0] << endl;
		cout << "busytime coinc"<< " " << runs.at(i) << " "  << sumbusy[2] << endl;
		
    file0->Close();

  }

  for(Int_t i=0;i<3;i++){
    runtime[i]  = sumrun[i];
    busytime[i] = sumbusy[i];
    livetime[i] = (sumrun[i] - sumbusy[i]) / sumrun[i];
		
  }

	dragon::MakeChains(runs,format);

	new TCanvas();
	t20->Draw("val>>Ph(1024,0,10)","ch==0");
	Double_t Pmean  = Ph->GetMean();
	Double_t Psigma = Ph->GetRMS();
	Double_t uP     = Psigma / sqrt(Ph->GetEntries());

	new TCanvas();
	t20->Draw("val>>Th(1024,0,50)","ch==1");
	Double_t Tmean  = Th->GetMean();
	Double_t Tsigma = Th->GetRMS();
	Double_t uT     = Tsigma / sqrt(Th->GetEntries());

	new TCanvas();
	t20->Draw("val>>Sc(1024,0,10)","ch==6");


	
  cout << "Runtime_head = " << runtime[0] << "\n";
  cout << "Runtime_tail = " << runtime[1] << "\n";
  cout << "Runtime_coinc = " << runtime[2] << "\n";
  cout << "Livetime_head = " << livetime[0] << " +/- " << livetime[0]*50/1e6 << "\n";
  cout << "Livetime_tail = " << livetime[1] << " +/- " << livetime[1]*50/1e6 << "\n";
  cout << "Livetime_coinc = " << livetime[2] << " +/- " << livetime[2]*50/1e6 << "\n";

	cout << livetime[1] / livetime[2] << "\n";

	cout << Pmean << "+/-" << uP << "\n";
	cout << Tmean << "+/-" << uT << "\n";

}
