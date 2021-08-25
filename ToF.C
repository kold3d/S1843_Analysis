{

	int firstRun=9957, lastRun=9973, goodpoints=0; //first: 8231 last: 8349

	vector<int> runs;

	Int_t nruns = sizeof(runs)/sizeof(int);

	int i;
     	char fname0[4096];
        i=0;

	// Take out bad runs

	for (int i = firstRun; i<=lastRun; i++){
//		if( i==9936 || i==9937) {
		if( i==9968 || i==9969) {
            continue;} 
	
	runs.push_back(i);
	}

	const char *format="/data/astro/dragon/S1843/data/rootfiles/run%d.root";
	dragon::MakeChains(runs,format);

	// Aliases

	t5->SetAlias("h","head");
	t5->SetAlias("t","tail");

//	t3->SetAlias("rf","trf.leading[2] - trf.leading[1]");
//	t3->SetAlias("dsssd_rftof","trf.leading[1] - dsssd.tfront");
	t5->SetAlias("dsssd_rftofc","t.trf.leading[1] - t.dsssd.tfront");
//	t3->SetAlias("MCP0_rftof","trf.leading[1] - mcp.tcal[0]");
//	t5->SetAlias("MCP0_rftofc","t.trf.leading[1] - t.mcp.tcal[0]");
//	t3->SetAlias("MCP1_rftof","trf.leading[1] - mcp.tcal[1]");
//	t5->SetAlias("MCP1_rftofc","t.trf.leading[2] - t.mcp.tcal[1]");
//    t5->SetAlias("ic_anodeEsum","t.ic.anode[0]+t.ic.anode[1]+t.ic.anode[2]+t.ic.anode[3]+t.ic.anode[4]");
    t5->SetAlias("ic_anodeEsum","t.ic.sum");

	Double_t rfperiod = 86.86;
//
//	// Cuts
//	TCut pulse = "!(t.io32.trigger_latch & 128) && !(xtofh==-1)";
	TCut septof = "xtofh>1750 && xtofh<1850";
//	TCut bgo = "(h.bgo.esort[0]+h.bgo.esort[1])<9.0 && h.bgo.esort[0]>2.0";
	TCut dsssd = "t.dsssd.efront>0";
//	TCut dsssd = "t.dsssd.efront>2.5 && t.dsssd.efront<3.0";
//	TCut dsssdc = "t.dsssd.efront>300 && t.dsssd.efront<800";
//	TCut mcptof = "t.tof.mcp>110 && t.tof.mcp>140";
//	TCut bgo = "(h.bgo.esort[0]+h.bgo.esort[1])<9.0 && h.bgo.esort[0]>1.0";
//
//	TCut dsssd_rfcut = "dsssd_rftofc>110.0 && dsssd_rftofc<130.0";
    TCut icEsum_cut = "ic_anodeEsum<2750. && ic_anodeEsum>2000";

    // xtofh
    TCanvas *C0 = new TCanvas();
    TH1F *h_xtofh = new TH1F("h_xtofh", "X TOF Head", 500, 0, 2000);
    t5->Draw("xtofh>>h_xtofh",dsssd && icEsum_cut);
    h_xtofh->SetTitle("X TOF Head");
    h_xtofh->GetYaxis()->SetTitle("Counts");
    h_xtofh->GetXaxis()->SetTitle("Time [ns]");

    //IC Anone E sum
    TCanvas *ICSum = new TCanvas();
    TH1F *h_icEsum = new TH1F("h_icEsum", "IC E Sum", 500, 0, 5000);
    t5->Draw("ic_anodeEsum>>h_icEsum",dsssd && icEsum_cut);
//    t5->Draw("t.ic.anode[0]+t.ic.anode[1]+t.ic.anode[2]+t.ic.anode[3]+t.ic.anode[4]>>h_icEsum");

/*
	// RF period
	TCanvas *C1 = new TCanvas();
	TH1F *h_rf = new TH1F("h_rf","RF Period",50,85,90);	
	t3->Draw("rf>>h_rf");
	h_rf->SetTitle("RF Period");
	h_rf->GetYaxis()->SetTitle("Counts [0.1 ns / bin]");
	h_rf->GetXaxis()->SetTitle("Time [ns]");
	h_rf->GetYaxis()->SetTitleOffset(1.4);
*/	
	// dsssd tfront
	TCanvas *C2 = new TCanvas();
	TH1F *h_dsssdtfront = new TH1F("h_dsssdtfront", "dsssd.tfront",150, 10100, 10400);
	t3->Draw("dsssd.tfront>>h_dsssdtfront","dsssd.tfront>0");
	h_dsssdtfront->SetTitle("dsssd.tfront");
	h_dsssdtfront->GetYaxis()->SetTitle("Counts [2 ns / bin]");
	h_dsssdtfront->GetXaxis()->SetTitle("Time [ns]");

	// trf - dsssd.tfront
	TCanvas *C4 = new TCanvas();
	TH1F *h_rftof1 = new TH1F("h_rftof1","RF ToF DSSSD",500,0,500);	
	//t3->Draw("dsssd_rftof>>h_rftof1","dsssd_rftof<10000 && dsssd_rftof>0");
	TH1F *h_rftof2 = new TH1F("h_rftof2","RF ToF DSSSD",500,0,500);	
	t5->Draw("dsssd_rftofc>>h_rftof2",icEsum_cut);
/*
	h_rftof2->SetLineColor(2);
	h_rftof1->SetTitle("RF ToF dsssd.tfront");
	h_rftof1->GetYaxis()->SetTitle("Counts [2 ns / bin]");
	h_rftof1->GetXaxis()->SetTitle("Time [ns]");
	h_rftof1->GetYaxis()->SetTitleOffset(1.4);

	leg1 = new TLegend(0.1,0.7,0.48,0.9);
	leg1->SetHeader("Legend");
	leg1->AddEntry(h_rftof1,"Singles DSSSD TOF");
	leg1->AddEntry(h_rftof2,"Coincidence DSSSD TOF");
	leg1->Draw();


	// trf - mcp0
	TCanvas *C5 = new TCanvas();
	TH1F *h_MCP0rftof1 = new TH1F("h_MCP0rftof1","RF ToF MCP0",1000,0,1000);	
	//t3->Draw("MCP0_rftof>>h_MCP0rftof1");
	TH1F *h_MCP0rftof2 = new TH1F("h_MCP0rftof2","Coinc RF ToF MCP0",1000,0,1000);	
	t5->Draw("MCP0_rftofc>>h_MCP0rftof2");
	h_MCP0rftof2->SetLineColor(2);
	h_MCP0rftof1->SetTitle("RF ToF MCP0");
	h_MCP0rftof1->GetYaxis()->SetTitle("Counts [2 ns / bin]");
	h_MCP0rftof1->GetXaxis()->SetTitle("Time [ns]");
	h_MCP0rftof1->GetYaxis()->SetTitleOffset(1.4);

	leg2 = new TLegend(0.1,0.7,0.48,0.9);
	leg2->SetHeader("Legend");
	leg2->AddEntry(h_MCP0rftof1,"Singles MCP0 RF-TOF");
	leg2->AddEntry(h_MCP0rftof2,"Coincidence MCP0 RF-TOF");
	leg2->Draw();

	// trf - mcp1
	TCanvas *C6 = new TCanvas();
	TH1F *h_MCP1rftof1 = new TH1F("h_MCP1rftof1","RF ToF MCP1",1000,0,1000);	
	//t3->Draw("MCP1_rftof>>h_MCP1rftof1");
	TH1F *h_MCP1rftof2 = new TH1F("h_MCP1rftof2","RF ToF MCP1",1000,0,1000);	
	t5->Draw("MCP1_rftofc>>h_MCP1rftof2");
	h_MCP1rftof2->SetLineColor(2);
	h_MCP1rftof1->SetTitle("RF ToF MCP1");
	h_MCP1rftof1->GetYaxis()->SetTitle("Counts [2 ns / bin]");
	h_MCP1rftof1->GetXaxis()->SetTitle("Time [ns]");
	h_MCP1rftof1->GetYaxis()->SetTitleOffset(1.4);

	leg3 = new TLegend(0.1,0.7,0.48,0.9);
	leg3->SetHeader("Legend");
	leg3->AddEntry(h_MCP1rftof1,"Singles MCP1 RF-TOF");
	leg3->AddEntry(h_MCP1rftof2,"Coincidence MCP1 RF-TOF");
	leg3->Draw();

	// mcp ToF
	TCanvas *C6 = new TCanvas();
	TH1F *h_MCPtof = new TH1F("h_MCPtof","MCP ToF",500,0,500);	
	//t3->Draw("tof.mcp>>h_MCPtof");
	TH1F *h_MCPtofc = new TH1F("h_MCPtofc","MCP ToF",500,0,500);	
	t5->Draw("t.tof.mcp>>h_MCPtofc");
	h_MCPtofc->SetLineColor(2);
	h_MCPtof->SetTitle("MCP ToF");
	h_MCPtof->GetYaxis()->SetTitle("Counts [2 ns / bin]");
	h_MCPtof->GetXaxis()->SetTitle("Time [ns]");
	h_MCPtof->GetYaxis()->SetTitleOffset(1.4);

	leg4 = new TLegend(0.1,0.7,0.48,0.9);
	leg4->SetHeader("Legend");
	leg4->AddEntry(h_MCPtof,"Singles MCP TOF");
	leg4->AddEntry(h_MCPtofc,"Coincidence MCP TOF");
	leg4->Draw();
*/

	// Separator ToF
//	TCanvas *C8 = new TCanvas();
//
//	TH1F *h_septof = new TH1F("h_septof","Separator ToF",10000,0,10000);
//	t5->Draw("xtofh>>h_septof",pulse + dsssd_rfcut + bgo + dsssdc);
//
//	h_septof->SetTitle("Separator Time of Flight");
//	h_septof->GetYaxis()->SetTitle("Counts [1 ns / bin]");
//	h_septof->GetXaxis()->SetTitle("Time [ns]");
//	h_septof->GetYaxis()->SetTitleOffset(1.4);
//
//
//	// Separator ToF
//	TCanvas *C9 = new TCanvas();
//
//	TH2F *h_sepmcptof = new TH2F("h_sepmcptof","Separator vs MCP ToF",10000,0,10000,500,0,500);
//	t5->Draw("t.tof.mcp:xtofh>>h_sepmcptof",pulse + dsssd_rfcut + bgo + dsssdc);
//
//	h_sepmcptof->SetTitle("Separator Time of Flight");
//	h_sepmcptof->GetYaxis()->SetTitle("MCP TOF");
//	h_sepmcptof->GetXaxis()->SetTitle("Sep TOF");
//	h_sepmcptof->GetYaxis()->SetTitleOffset(1.4);
//	h_sepmcptof->SetMarkerStyle(20);
//
//
//	TCanvas *C9 = new TCanvas();
//
//	TH2F *h_sepdsssdtof = new TH2F("h_sepdsssdtof","Separator vs DSSSD ToF",10000,0,10000,500,0,500);
//	t5->Draw("dsssd_rftofc:xtofh>>h_sepdsssdtof",pulse + dsssdc);
//
//	h_sepdsssdtof->SetTitle("Separator Time of Flight");
//	h_sepdsssdtof->GetYaxis()->SetTitle("DSSD RF-TOF");
//	h_sepdsssdtof->GetXaxis()->SetTitle("Sep TOF");
//	h_sepdsssdtof->GetYaxis()->SetTitleOffset(1.4);
//	h_sepdsssdtof->SetMarkerStyle(20);
//
//	TCanvas *C10 = new TCanvas();
//
//	TH2F *h_mcpdsssdtof = new TH2F("h_mcpdsssdtof","MCP vs DSSSD ToF",500,0,500,500,0,500);
//	t5->Draw("dsssd_rftofc:t.tof.mcp>>h_mcpdsssdtof",pulse + dsssdc + septof);
//
//	h_mcpdsssdtof->SetTitle("MCP vs DSSD TOF");
//	h_mcpdsssdtof->GetYaxis()->SetTitle("DSSSD RF-TOF");
//	h_mcpdsssdtof->GetXaxis()->SetTitle("MCP TOF");
//	h_mcpdsssdtof->GetYaxis()->SetTitleOffset(1.4);
//	h_mcpdsssdtof->SetMarkerStyle(20);

/*	
	TH1F *h_septof5 = new TH1F("h_septof5","Separator ToF",2000,-10000,10000);
	t5->Draw("xtofh>>h_septof5",pulse + dsssd2 + bgo2,"same");
	h_septof5->SetLineColor(2);
	h_septof5->SetFillColor(2);

	TH1F *h_septof1 = new TH1F("h_septof1","Separator ToF",75,2500,5500);
	t5->Draw("xtofh>>h_septof1",pulse + dsssd + bgo1,"same");
	//TH1F *h_septof2 = new TH1F("h_septof2","Separator ToF",75,2500,5500);
	//t5->Draw("xtofh>>h_septof2",pulse + dsssd + bgo2,"same");
	//TH1F *h_septof3 = new TH1F("h_septof3","Separator ToF",75,2500,5500);
	//t5->Draw("xtofh>>h_septof3",pulse + dsssd + bgo3,"same");
	h_septof4->SetTitle("Separator Time of Flight (E_{c.m.} = 181 keV)");
	h_septof4->GetYaxis()->SetTitle("Counts [40 ns / bin]");
	h_septof4->GetXaxis()->SetTitle("Time [ns]");
	h_septof4->GetYaxis()->SetTitleOffset(1.4);
	
	h_septof1->SetLineColor(1);
	//h_septof2->SetLineColor(4);
	//h_septof3->SetLineColor(2);

	Double_t par[4];
	TF1 *l1 = new TF1 ("l1","pol0",3000,5000);
	TF1 *g1 = new TF1 ("g1","gausn",3950,4050);

	l1->SetParLimits(0,10,60);
	g1->SetParLimits(0,10,200);
	g1->SetParLimits(1,3800,4200);
	g1->SetParLimits(2,20,80); 	

	h_septof1->Fit(l1,"R");
   	h_septof1->Fit(g1,"R");
    
   	g1->GetParameters(&par[0]);
   	l1->GetParameters(&par[3]);

   	TF1 *f1 = new TF1 ("f1","pol0(0)+gaus(1)",3000,5000);
   	f1->SetParameters(par);
	f1->SetParLimits(0,10,60);
	f1->SetParLimits(1,0,200);
	f1->SetParLimits(2,3800,4200);
	f1->SetParLimits(3,20,80); 
   	h_septof1->Fit(f1,"R");
   	f1->SetLineColor(2);

	leg = new TLegend(0.1,0.7,0.48,0.9);
	leg->SetHeader("Legend");
	leg->AddEntry(h_septof1,"E_{#gamma0} > 2.0 MeV");
	leg->AddEntry(h_septof2,"E_{#gamma0} > 2.5 MeV");
	leg->AddEntry(h_septof3,"E_{#gamma0} > 3.0 MeV");
	leg->Draw();
	
	TLine *l1 = new TLine(2000,0.1,2000,2800);
	TLine *l2 = new TLine(2300,0.1,2300,2800);

	l1->SetLineWidth(2);
	l2->SetLineWidth(2);

	l1->SetLineStyle(2);
	l2->SetLineStyle(2);
	
	l1->Draw();
	l2->Draw();

	h_septof->GetYaxis()->SetRangeUser(0.1,2800);
	h_septof->GetXaxis()->SetRangeUser(1800,2400);

	TCanvas *C8 = new TCanvas();
	TH2F *h_dsssdvRF1 = new TH2F("h_dsssdvRF1","DSSSD vs RF ToF",600,0,12,500,0,500);
	t3->Draw("rftof1:dsssd.efront>>h_dsssdvRF1","!(tail.io32.trigger_latch & 128)","colz");

	h_dsssdvRF1->SetTitle("484keV July");
	h_dsssdvRF1->GetYaxis()->SetTitle("RF ToF (ns)");
	h_dsssdvRF1->GetXaxis()->SetTitle("DSSSD Front Energy (MeV)");

	//TH2F *h_dsssdvRF2 = new TH2F("h_dsssdvRF2","DSSSD vs RF ToF",600,0,12,150,100,250);
	//t3->Draw("rftof1:dsssd.efront>>h_dsssdvRF2"," ","colz");
	//h_dsssdvRF2->SetMarkerColor(6);

	h15->SetTitle("484keV Run 5718");
	h15->GetYaxis()->SetTitle("RF ToF (ns)");
	h15->GetXaxis()->SetTitle("DSSSD Front Energy (MeV)");	

	TCanvas *C10 = new TCanvas();
	TH2F *h16 = new TH2F("h16","DSSSD vs RF ToF",600,0,12,150,100,250);
	t5->Draw("rftof:t.dsssd.efront>>h16",pulse + septof,"colz");

	h15->SetTitle("484keV Run 5718");
	h15->GetYaxis()->SetTitle("RF ToF (ns)");
	h15->GetXaxis()->SetTitle("DSSSD Front Energy (MeV)");	
*/
}
