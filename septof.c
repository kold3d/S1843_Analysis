{
	TCanvas *c1 = new TCanvas("c1","c1",800,800);

	int firstRun=9052, lastRun=9054, goodpoints=0;
	//int firstRun = 9070, lastRun = 9079;
	vector<int> runs;

	Int_t nruns = sizeof(runs)/sizeof(int);

	int i;
     	char fname0[4096];
        i=0;

	for (int i = firstRun; i<=lastRun; i++){
		if(i==9141 || i==9140) {continue;}
	    runs.push_back(i);
	}

	const char *format="/data/astro/dragon/S1843/data/rootfiles/run%d.root";
        dragon::MakeChains(runs,format);

    gROOT->ProcessLine(".x MCP_RF_cut.C");    
        
    TCutG *MCP_RF_cut = (TCutG*)gROOT->FindObjectAny("MCP_RF_cut");
        
	// Aliases

	t5->SetAlias("h","head");
	t5->SetAlias("t","tail");

	// Cuts
	TCut pulse = "!(t.io32.trigger_latch & 128) && !(xtofh==-1)";
// 	TCut septof = "xtofh>2060 && xtofh<2160"; // 7.5 Torr "xtofh>2850 && xtofh<3150"; // 5.0 Torr "xtofh>2800 && xtofh<3100";
    
    double minseptof = 2060, maxseptof = 2160;
    TCut septof = Form("xtofh>%f && xtofh<%f",minseptof,maxseptof);
    cout << "septof: " << septof << endl;
    
	//TCut septof = "xtofh>2800 && xtofh<2260";
	TCut mcptof = "t.tof.mcp>124 && t.tof.mcp<133";
	TCut bgo0 = "h.bgo.esort[0]>1.25 && h.bgo.esort[0]<12.0";
        TCut dsssdhit = "t.dsssd.efront>0.0 && t.dsssd.efront<4000.";
	TCut dsssdcut = "t.dsssd.efront>200 && t.dsssd.efront<650";
    TCut iccut = "tail.ic.anode[0]>840 && tail.ic.anode[0]<1040";

//         TCut bckgnd0 = "xtofh>1300 && xtofh<1600";
// 	TCut bckgnd1 = "xtofh>1600 && xtofh<1900";
//         TCut bckgnd2 = "xtofh>4700 && xtofh<2200";
// 	TCut bckgnd3 = "xtofh>2200 && xtofh<2500";
// 	TCut bckgnd4 = "xtofh>2500 && xtofh<2800";
// 	TCut bckgnd5 = "xtofh>3200 && xtofh<3500";
// 	TCut bckgnd6 = "xtofh>3500 && xtofh<3800";
// 	TCut bckgnd7 = "xtofh>3800 && xtofh<4100";
// 	TCut bckgnd8 = "xtofh>4100 && xtofh<4400";
// 	TCut bckgnd9 = "xtofh>4400 && xtofh<4700";

    TCut *bckgnd = new TCut[10];
    
    for(int i=0;i<5;i++)
        bckgnd[i] = Form("xtofh>%f && xtofh<%f",minseptof-(5-i)*(maxseptof-minseptof),minseptof-(4-i)*(maxseptof-minseptof));
    
    for(int i=5;i<10;i++)
        bckgnd[i] = Form("xtofh>%f && xtofh<%f",maxseptof+(i-5)*(maxseptof-minseptof),maxseptof+(i-4)*(maxseptof-minseptof));
    
    for(int i=0;i<10;i++)
        cout << "bckgnd[" << i << "]: " << bckgnd[i] << endl;
    
	// Separator ToF
	/*	TCanvas *C1 = new TCanvas();

        TH2F *h_septof_mcptof = new TH2F("h_septof_mcptof"," ",1000,0,10000,200,0,2000);
	t5->Draw("t.mcp.tac:xtofh>>h_septof_mcptof",pulse + dsssdhit,"colz");
	//h_septof_mcptof->SetMarkerSize(0.5);
	//h_septof_mcptof->SetMarkerStyle(20);
	h_septof_mcptof->SetTitle("E^{r}_{c.m.} = 323 keV");
	h_septof_mcptof->GetXaxis()->SetTitle("Separator TOF [ns]");
	h_septof_mcptof->GetYaxis()->SetTitle("MCP0 DSSD TAC [chan]");
	h_septof_mcptof->GetXaxis()->SetTitleOffset(1);
	h_septof_mcptof->GetXaxis()->SetTitleSize(0.05); h_septof_mcptof->GetXaxis()->SetLabelSize(0.04); h_septof_mcptof->GetXaxis()->SetLabelOffset(0.001);
	h_septof_mcptof->GetYaxis()->SetTitleSize(0.05); h_septof_mcptof->GetYaxis()->SetLabelSize(0.04);
	h_septof_mcptof->GetYaxis()->SetTitleOffset(1.2); h_septof_mcptof->GetXaxis()->SetTitleOffset(0.85);
	h_septof_mcptof->GetYaxis()->CenterTitle();
	h_septof_mcptof->GetXaxis()->CenterTitle();
	C1->SetTicky(); C1->SetTickx();
	h_septof_mcptof->GetYaxis()->SetRangeUser(800,1800);
	h_septof_mcptof->GetXaxis()->SetRangeUser(1000,5000);
	*/
	TCanvas *c2 = new TCanvas();
	TH1F *h_septof = new TH1F("h_septof","E^{r}_{c.m.} = 666 keV",1000,0,10000);
	t5->Draw("xtofh>>h_septof",pulse + dsssdhit + bgo0);
	//TH1F *h_septof_gated = new TH1F("h_septof_gated"," ",500,0,10000);
        //t5->Draw("xtofh>>h_septof_gated",pulse + dsssdcut,"same");
	TH1F *h_septof_signal = new TH1F("h_septof_signal"," ",500,0,10000);
	t5->Draw("xtofh>>h_septof_signal",pulse + dsssdhit + septof + bgo0 + dsssdcut,"goff");
	h_septof->SetLineColor(1); h_septof->SetLineWidth(2);
	//h_septof_gated->SetLineColor(1); h_septof_gated->SetLineWidth(2); h_septof_gated->SetFillColor(2);
	h_septof->GetXaxis()->SetTitle("Separator TOF [ns]");
	h_septof->GetYaxis()->SetTitle("Counts [20 ns/bin]");

	h_septof->GetXaxis()->SetTitleSize(0.05); h_septof->GetXaxis()->SetLabelSize(0.04); h_septof->GetXaxis()->SetLabelOffset(0.001);
	h_septof->GetYaxis()->SetTitleSize(0.05); h_septof->GetYaxis()->SetLabelSize(0.04);
	h_septof->GetYaxis()->SetTitleOffset(0.9); h_septof->GetXaxis()->SetTitleOffset(0.85);
	h_septof->GetYaxis()->CenterTitle();
	h_septof->GetXaxis()->CenterTitle();
	c2->SetTicky(); c2->SetTickx();
        //h_septof_mcpgate->GetYaxis()->SetRangeUser(0,14);
	//h_septof->GetXaxis()->SetRangeUser(1000,5000);

    TLatex *lat1 = new TLatex(4000,20,Form("Yield: %f\n",h_septof_signal->GetEntries()));
    lat1->Draw("");
    
	c2->SaveAs("SepTofFigure.png");
    c2->SaveAs("SepTofFigure.pdf");
    c2->SaveAs("SepTofFigure.C");

	Double_t Nsignal = h_septof_signal->GetEntries();
	cout << "Nsignal: " << Nsignal << endl;
	
	TH1F *h_septof_bg0 = new TH1F("h_septof_bg0"," ",500,0,10000);
	t5->Draw("xtofh>>h_septof_bg0",pulse + dsssdcut + bckgnd[0] + bgo0,"goff");
	TH1F *h_septof_bg1 = new TH1F("h_septof_bg1"," ",500,0,10000);
	t5->Draw("xtofh>>h_septof_bg1",pulse + dsssdcut + bckgnd[1] + bgo0,"goff");
	TH1F *h_septof_bg2 = new TH1F("h_septof_bg2"," ",500,0,10000);
	t5->Draw("xtofh>>h_septof_bg2",pulse + dsssdcut + bckgnd[2] + bgo0,"goff");
	TH1F *h_septof_bg3 = new TH1F("h_septof_bg3"," ",500,0,10000);
	t5->Draw("xtofh>>h_septof_bg3",pulse + dsssdcut + bckgnd[3] + bgo0,"goff");
	TH1F *h_septof_bg4 = new TH1F("h_septof_bg4"," ",500,0,10000);
	t5->Draw("xtofh>>h_septof_bg4",pulse + dsssdcut + bckgnd[4] + bgo0,"goff");
        TH1F *h_septof_bg5 = new TH1F("h_septof_bg5"," ",500,0,10000);
	t5->Draw("xtofh>>h_septof_bg5",pulse + dsssdcut + bckgnd[5] + bgo0,"goff");
        TH1F *h_septof_bg6 = new TH1F("h_septof_bg6"," ",500,0,10000);
        t5->Draw("xtofh>>h_septof_bg6",pulse + dsssdcut + bckgnd[6] + bgo0,"goff");
        TH1F *h_septof_bg7 = new TH1F("h_septof_bg7"," ",500,0,10000);
        t5->Draw("xtofh>>h_septof_bg7",pulse + dsssdcut + bckgnd[7] + bgo0,"goff");
        TH1F *h_septof_bg8 = new TH1F("h_septof_bg8"," ",500,0,10000);
        t5->Draw("xtofh>>h_septof_bg8",pulse + dsssdcut + bckgnd[8] + bgo0,"goff");
        TH1F *h_septof_bg9 = new TH1F("h_septof_bg9"," ",500,0,10000);
        t5->Draw("xtofh>>h_septof_bg9",pulse + dsssdcut + bckgnd[9] + bgo0,"goff");


	Double_t Nbg0, Nbg1, Nbg2, Nbg3, Nbg4, Nbg5, Nbg6, Nbg7, Nbg8, Nbg9;
        Nbg0 = h_septof_bg0->GetEntries();
	Nbg1 = h_septof_bg1->GetEntries();
	Nbg2 = h_septof_bg2->GetEntries();
	Nbg3 = h_septof_bg3->GetEntries();
	Nbg4 = h_septof_bg4->GetEntries();
	Nbg5 = h_septof_bg5->GetEntries();
	Nbg6 = h_septof_bg6->GetEntries();
	Nbg7 = h_septof_bg7->GetEntries();
	Nbg8 = h_septof_bg8->GetEntries();
	Nbg9 = h_septof_bg9->GetEntries();

        cout << "Background: " << Nbg0 << ", " << Nbg1 << ", " << Nbg2 << ", " << Nbg3 << ", " << Nbg4 << ", " << Nbg5 << ", " << Nbg6 << ", " << Nbg7 << ", " << Nbg8 << ", " << Nbg9 << endl;

        Double_t Nbg_tot = Nbg0 + Nbg1 + Nbg2 + Nbg3 + Nbg4 + Nbg5 + Nbg6 + Nbg7 + Nbg8 + Nbg9;
	Double_t Nbckgnd = Nbg_tot/10;

	cout << "Background = " << Nbckgnd << endl;
	cout << "Recoils = " << Nsignal - Nbckgnd << endl;

	c1->cd();

	TH1F *h_GammaRFTime = new TH1F("h_GammaRFTime","h_GammaRFTime",2000,10000,12000);
	t5->Draw("h.tcal_rf>>h_GammaRFTime","","");

	TH1F *h_GammaRFTimeGate = new TH1F("h_GammaRFTimeGate","h_GammaRFTimeGate",2000,10000,12000);
	h_GammaRFTimeGate->SetLineColor(2);
	t5->Draw("h.tcal_rf>>h_GammaRFTimeGate",pulse + dsssdhit + septof + bgo0,"same");
	
	h_GammaRFTime->Draw();
	h_GammaRFTimeGate->Draw("same");

	c1->SaveAs("trf_leading.png");
    
    TCanvas *c3 = new TCanvas("c3","c3",1200,800);

    t5->Draw("tail.mcp.tcal[0]-tail.trf.leading[1]:tail.dsssd.efront>>h2D(350,0,700,500,-1000,0)",pulse + dsssdhit,"col");
    t5->Draw("tail.mcp.tcal[0]-tail.trf.leading[1]:tail.dsssd.efront>>h2D2(350,0,700,500,-1000,0)",pulse + dsssdhit + septof,"goff");
    
    h2D2->SetMarkerStyle(8);
    h2D2->SetMarkerSize(0.25);
    h2D2->SetMarkerColor(2);
    h2D2->Draw("same");
    MCP_RF_cut->Draw("same");
    
    c3->SaveAs("MCP_RF_PID.C");
    c3->SaveAs("MCP_RF_PID.pdf");
    c3->SaveAs("MCP_RF_PID.png");
    
    TCanvas *c4 = new TCanvas();
    
	t5->Draw("t.dsssd.efront>>hdsssd_gate",pulse + dsssdhit + septof + bgo0,"");

	t5->Draw("t.dsssd.efront>>hdsssd_nogate",pulse + dsssdhit + "MCP_RF_cut","same");
	hdsssd_nogate->SetLineColor(2);
    
    TCanvas *c5 = new TCanvas();
    
    t5->Draw("tail.mcp.tcal[0]-tail.dsssd.tfront:tail.dsssd.efront>>h2DDSSSDTime(350,0,700,500,-1000,0)",pulse + dsssdhit + septof + bgo0,"goff");
    t5->Draw("tail.mcp.tcal[0]-tail.dsssd.tfront:tail.dsssd.efront>>h2DDSSSDTimeUngated(350,0,700,500,-1000,0)",pulse + dsssdhit,"col");

    h2DDSSSDTime->SetMarkerStyle(8);
    h2DDSSSDTime->SetMarkerSize(0.5);
    h2DDSSSDTime->SetMarkerColor(2);
    h2DDSSSDTime->Draw("same");
    
    TCanvas *c6 = new TCanvas();
    
    t5->Draw("head.bgo.ecal>>hBgo(2000,0,20)",pulse + dsssdhit,"");
    t5->Draw("head.bgo.ecal>>hBgoGate(2000,0,20)",pulse+dsssdhit+septof,"goff");
    
    hBgoGate->Draw("same");
    hBgoGate->SetLineColor(2);
}
