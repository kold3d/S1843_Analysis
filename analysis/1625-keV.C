{


    // Specify the runs to be included in the analysis chain.

    //int firstRun= 9919, lastRun= 9919, goodpoints=0; // 5 Torr
    int firstRun= 9918, lastRun= 9922, goodpoints=0; // 5 Torr

    // Fill the analysis chain vector

    vector<int> runs;

     for (int i = firstRun; i<=lastRun; i++){
      if((i==9870 )){
        continue;
      }
      runs.push_back(i);
    }


   // dragon::MakeChains(runs, "/data2/dragon/S1843/data/rootfiles/run%d.root");
    dragon::MakeChains(runs, "/data/astro/dragon/S1843/data/rootfiles/run%d.root");


    //Aliases
    t3->SetAlias("rf","trf.leading[2] - trf.leading[1]");


    // Cuts for coincidence events
    TCut bgo = "head.bgo.esort[0]>0.5 && head.bgo.esort[0]<11.0";
    //TCut SepTOF = "!(tail.io32.trigger_latch & 128) && !(coinc.xtofh==-1)";
    TCut pulser = "!(tail.io32.trigger_latch & 128) && !(coinc.xtofh==-1)";
    TCut recoils = "coinc.xtofh>1350 && coinc.xtofh<1450";
    TCut dsssd = "tail.dsssd.efront>1400 && tail.dsssd.efront<1900";
    TCut rftof = "rf>84 && rf<90";
    TCut real = "tail.dsssd.efront > 0";
    TCut mcp0 = "tail.mcp.tac > 0";


    // Plots

      TCanvas *c4 = new TCanvas();
      TH1F *bgohit = new TH1F("bgo.hit0","bgo.hit0",30,0,30);
      t5->Draw("head.bgo.hit0>>bgohit");
      c4->SetTitle("E_{c.m.} = 1625 keV - BGO Hit");
      c4->SaveAs("1625-keV/BGO_hit0.pdf");

    //BGO E

      TCanvas *c0 = new TCanvas();
      t5->Draw("head.bgo.esort[0]>>h_bgo(240,0,12)",bgo && recoils && "head.bgo.esort[0]>0.5");
      h_bgo->SetLineColor(1);
      h_bgo->SetTitle("E_{c.m.} = 1625 keV - #gamma_{0} Energy Spectrum");
      h_bgo->GetYaxis()->SetTitle("Counts [50 keV / bin]");
      h_bgo->GetXaxis()->SetTitle("#gamma_{0} Energy [MeV]");
      c0->SaveAs("1625-keV/1625_BGO_E.pdf");


    // MCP TAC vs DSSSD


    /*
    // Graph Settings

    TH2F *tofdsssd = new TH2F("dsssdmcp", "dsssdmcp",  500, 0, 5000, 1e3, 0, 2000);
    tofdsssd->SetMarkerStyle(20);
    tofdsssd->SetMarkerColor(1);
    tofdsssd->SetMarkerSize(0.6);

    tofdsssd->SetTitle("");
    tofdsssd->GetYaxis()->SetTitle("MCP/DSSSD TOF");
    tofdsssd->GetXaxis()->SetTitle("DSSSD energy");
    tofdsssd->GetXaxis()->SetTitleOffset(1.10);
    tofdsssd->GetYaxis()->SetTitleOffset(1.10);
    tofdsssd->GetXaxis()->CenterTitle();
    tofdsssd->GetYaxis()->CenterTitle();
    tofdsssd->SetStats(0);

    tofdsssd->GetXaxis()->SetRangeUser(100,5000);
    tofdsssd->GetYaxis()->SetRangeUser(100,1500);

    TH2F *tofdsssd1 = new TH2F("dsssdmcp1", "dsssdmcp1",  500, 0, 5000, 1e4, 0, 4000);
    tofdsssd1->SetMarkerStyle(20);
    tofdsssd1->SetMarkerColor(2);
    tofdsssd1->SetMarkerSize(0.7);


    // Draw the graph

      t5->Draw("tail.mcp.tac:tail.dsssd.efront>>dsssdmcp1", dsssd && recoils, "same");
// Graph Legend Settings

    leg1 = new TLegend(0.65,0.65,0.85,0.85);
    leg1->AddEntry(tofdsssd, "All coincidences","p");
    leg1->AddEntry(tofdsssd1, "Golden events","p");
    leg1->SetTextSize(0.03);
    leg1->Draw("same");
    leg1->SetBorderSize(0);

    // Ticks in both x- and y- axis

    c1-> SetTickx();
    c1-> SetTicky();

    // Print message about coincidences

    Double_t Coinc = dsssdmcp1->GetEntries();
    cout << "Coincidence events in Singles: "<< Coinc << endl;
    */
     //DSSSD efront singles + coinc

      TCanvas *c2 = new TCanvas();

    // Graph Settings

    t3->Draw("tail.dsssd.efront>>singles(1000,0,4000)");
    t5->Draw("tail.dsssd.efront>>coinc(1000,0,4000)", recoils && pulser && bgo, "same");
    //t5->Draw("tail.dsssd.efront>>coinc(1000,0,4000)", recoils && dsssd && bgo, "same");


    singles->SetLineColor(1);
    coinc->SetLineColor(2);
    //golden->SetLineColor(2);

    c2.SetLogy();

    singles->SetStats(0);
    singles->GetXaxis()->SetRangeUser(1300,2400);
    singles->GetXaxis()->SetTitle("DSSSD Efront [keV]");
    singles->GetYaxis()->SetTitle("Counts");
    singles->GetXaxis()->CenterTitle();
    singles->GetYaxis()->CenterTitle();
    singles->SetTitle("DSSSD Energy");
    // Graph Legend Settings

    leg2 = new TLegend(0.6,0.65,0.9,0.85);
    leg2->AddEntry(singles, "Singles");
    leg2->AddEntry(coinc,"All coincidences");
    //leg2->AddEntry(golden,"Coincidences in golden gate");
    leg2->Draw("same");

    // Ticks in both x- and y- axis

    c2->SetTickx();
    c2->SetTicky();

    c2->SaveAs("1625-keV/1625_DSSSD_E.pdf");

    // Print number or recoils in "golden" gate

    Double_t coinc_events = coinc->GetEntries();
    cout << "Coincidence in golden (DSSSD, Separator TOF, BGO) gate: " << coinc_events << endl;

    // DSSSD Hit Pattern
    //
    TCanvas *c14 = new TCanvas();
    //TH2F *dxy = new TH2F("dxy", "dxy", 15, 0, 15);
    t5->Draw("tail.dsssd.hit_back:tail.dsssd.hit_front>>dxy(16, 0, 15, 16, 16, 31)", "tail.dsssd.efront>0", "colz");
    dxy->GetXaxis()->SetTitle("Front strips");
    dxy->GetYaxis()->SetTitle("Back strips");
    c14->SaveAs("1625-keV/1625_dsssdXY.pdf");



    // Separator TOF

    TCanvas *c30 = new TCanvas();

    // Graph Settings

    t5->Draw("coinc.xtofh>>xtof1(1300, -1e4, 1e4)", pulser);
    t5->Draw("coinc.xtofh>>xtof2(1300,-1e4,1e4)", recoils && pulser && dsssd,"same");

    //xtof->SetLineColor(1);
    xtof1->SetLineColor(1);
    //recoilsbgo->SetLineColor(14);
    //xtof2->SetLineColor(2);
    xtof1->GetXaxis()->SetTitle("Time to travel through DRAGON in billionths of a second");
    //xtof1->GetXaxis()->SetRangeUser(0,6000);
    xtof1->GetXaxis()->CenterTitle();
    xtof1->GetYaxis()->SetTitle("Counts per 10 billionths of a second");
    //xtof1->GetYaxis()->SetRangeUser(0,200);
    xtof1->GetYaxis()->CenterTitle();
    xtof1->SetTitle("");

    // No stats and ticks in both x- and y- axis

    // xtof->SetStats(0);
    c30->SetTickx();
    c30->SetTicky();

    c30->SaveAs("1625-keV/1625_sepTOF.pdf");

    // Graph Legend Settings

    //leg = new TLegend(0.15,0.65,0.52,0.85);
    //leg->AddEntry(xtof1, "No cuts", "l");
    //leg->AddEntry(xtof, "1.2 MeV < E_{BGO} < 8.0 MeV ", "l");
    //leg->AddEntry(recoilsbgo, "xtof and 1.2  Mev < E_{BGO} < 8.0 MeV", "l");
    //leg->AddEntry(xtof2, "Golden gate", "l");
    //leg->SetBorderSize(0);
    //leg->Draw("same");

    
    // BGO Energy -- individual channels
	TCanvas *c31 = new TCanvas("c31", "bgo_energy",3840,2160);
	c31->Divide(6,5);
	TH1F *h_BGOenergy[30];
	for (int i = 0; i<30; i++) {
		c31->cd(i+1);
		TString nameT = Form("head.bgo.ecal[%d] >> h_BGOenergy%d(300,0,12)",i,i);
        TString cutName = Form("head.bgo.ecal[%d]>1.0", i);
		std::cout << nameT << std::endl;
		t1->Draw(nameT, cutName);
		//t1->Draw(nameT, bgo && recoils);
		//t5->Draw(nameT, pulser && recoils && cutName);
		c31->Update();
		//gPad->SetLogy(); 
        gPad->Modified(); gPad->Update(); // note: gPad needs #include "TPad.h"
    }

    // BGO Hit Pattern

    TCanvas *c5 = new TCanvas("BGO Hit Pattern");

    // Graph Settings

    //t5->Draw("head.bgo.z0>>bgoz(51,-20,20,20000)", recoils);
    t5->Draw("head.bgo.z0>>bgorec(51,-30,30)", recoils && pulser);
    t5->Draw("head.bgo.z0>>bgorec2(51,-30,30)", recoils && pulser && dsssd && bgo, "same");

    bgorec->GetXaxis()->SetTitle("#gamma_{0} z-Position (cm)");
    bgorec->GetXaxis()->CenterTitle();
    bgorec->GetYaxis()->SetTitle("Counts");
    bgorec->GetYaxis()->CenterTitle();
    bgorec->SetTitle("BGO hit pattern");
    bgorec->SetLineColor(2);
    //bgorec-> Draw("E");
    // Draw the Gauss fit

    //TF1 *bgogauss = new TF1("bgogauss", "gaus",-20,20);

    //bgogauss->SetLineColor(1);
    //bgogauss->SetLineStyle(2);
    //bgorec->Fit(bgogauss);

    //  bgoz->SetStats(0);

    // Ticks in both x- and y- axis

    c5->SetTickx();
    c5->SetTicky();


    TCanvas *c12 = new TCanvas();
    t5-> Draw("coinc.tail.ic.anode[1]:coinc.tail.ic.anode[0]>>ic10(1e3, 0, 3000, 1e3, 0, 3000)", "coinc.tail.ic.anode[1]>0 && coinc.tail.ic.anode[0]>0", "COLZ");



    TCanvas *c13 = new TCanvas();
    TH2F *icdsssd = new TH2F("icdsssd", "icdsssd", 100, 0, 4000, 1e3, 0, 4e3);
    icdsssd->SetMarkerStyle(20);
    icdsssd->SetMarkerColor(kAzure-2);
    icdsssd->SetMarkerSize(0.6);
    t3->Draw("tail.ic.sum:tail.dsssd.efront>>icdsssd", "tail.dsssd.efront>0 && tail.ic.sum>0");

    TH2F *icdsssd1 = new TH2F("icdsssd1", "icdsssd1", 100, 0, 4000, 1e3, 0, 1e4);
    icdsssd1->SetMarkerStyle(21);
    icdsssd1->SetMarkerColor(2);
    icdsssd1->SetMarkerSize(0.6);
    icdsssd1->SetColz();

    icdsssd1->SetTitle("IC sum vs DSSSD energy - E_{c.m} = 1625 keV");
    icdsssd1->GetXaxis()->SetTitle("DSSSD esort[0] [Channels]");
    icdsssd1->GetYaxis()->SetTitle("IC sum [Channels]");
    t5->Draw("tail.ic.sum:tail.dsssd.efront>>icdsssd1",recoils && pulser && bgo,"same");
    c13->SaveAs("1625-keV/1625_icdsssd_E.pdf");
}
