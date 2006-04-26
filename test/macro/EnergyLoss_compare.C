#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"

void EnergyLoss_compare(int arg)
{	
/*=======================================================*/
/* This Macros is used to plot the Energy deposited      */
/* by a particle in different Tracker subsystems         */
/*=======================================================*/
// const float E2NEL = 2.778E+8;
 const float E2NEL = 1.;
 
// Reference file
 char filename_ref[] ="$ROOTFile_ref";     // The Name of reference File.
// char filename_ref[] ="eloss_ref_1.root";     // The Name of reference File.
 TFile * rfile = new TFile (filename_ref);      //Open reference histo File. 
 TH1F *eloss_ref[6], *ref[6]; 
 char ref_name[10];
  
 string stat;
 if (arg == 1) stat = "KS";
 if (arg == 2) stat = "chi2";
 
 for(int i=0; i<6; i++) {
   sprintf (ref_name,"Eloss_%i",i+1);
   eloss_ref[i] = (TH1F*)rfile->Get(ref_name);
   ref[i]       = (TH1F*)eloss_ref[i]->Clone();
 }

   TH1F *heta = (TH1F*)rfile->Get("primary_eta");
   float mean_eta = heta->GetMean();  
   string range;
   if (mean_eta <0.5 && mean_eta >0.0) range = "0+0.5"; 
   if (mean_eta <1.0 && mean_eta >0.5) range = "0.5+1"; 
   if (mean_eta <1.5 && mean_eta >1.0) range = "1+1.5"; 
   if (mean_eta <2.0 && mean_eta >1.5) range = "1.5+2"; 
   if (mean_eta <2.5 && mean_eta >2.0) range = "2+2.5"; 
   if (mean_eta <3.0 && mean_eta >2.5) range = "2.5+3"; 
   if (-0.5< mean_eta && mean_eta <0.0 ) range = "-0.5-0"; 
   if (-1.0< mean_eta && mean_eta <-0.5 ) range = "-1-0.5"; 
   if (-1.5< mean_eta && mean_eta <-1.0 ) range = "-1.5-1"; 
   if (-2.0< mean_eta && mean_eta <-1.5 ) range = "-2-1.5"; 
   if (-2.5< mean_eta && mean_eta <-2.0 ) range = "-2.5-2"; 
   if (-3.0< mean_eta && mean_eta <-2.5 ) range = "-3-2.5"; 
// Statistical test
   cout << "[OVAL]: Performing "<< stat << " test" << " in eta range " << range 
   << endl;

// Test file
 char filename[] ="$ROOTFile_test";     // The Name of Root File.
// char filename[] ="SimHitMuon_0-0.5.root";     // The Name of Root File.
 char treename[] ="Events";            // The Name of Tree.
 TString brnchname = "PTrackerSimHit_thp_TrkHits.obj";

 gROOT ->Reset();
 gStyle->SetNdivisions(504,"XYZ");
 gStyle->SetStatH(0.18);
 gStyle->SetStatW(0.35);

// delete gROOT->GetListOfFiles()->FindObject(filename);

 TCanvas * SiTrk = new TCanvas("SiTrk","SiTrk",600,800);
 SiTrk->Divide(2,2);
 TCanvas * PxTrk = new TCanvas("PxTrk","PxTrk",600,600);
 PxTrk->Divide(1,2);

 TFile * file = new TFile (filename);      //Open Root File.
 // get tree from file
 TTree *tree = dynamic_cast<TTree*>(file->Get(treename));
 assert(tree != 0);
 
 // get branch from tree
 TBranch *srcbrnch = tree->GetBranch(brnchname);
 assert(srcbrnch != 0);

 // set access to branch as proper object
 PTrackerSimHit TrkSimHits;
 srcbrnch->SetAddress(&TrkSimHits);
 
 cout << "Setting branch address" << endl;
 
 float de[1000];
 int ids[1000];
 int nhit;
 	
////// booking histograms
 const char *SysName[] = {"TIB","TOB","TID","TEC","PIB","PEC"};
 
 TH1F *eloss[6];
 char hname[10], htitle[40];  
 for(int i=0; i<6; i++) {
  sprintf (htitle,"Energy loss in %s", SysName[i]);
  sprintf (hname,"Eloss_%i",i+1);
  eloss[i]  = new TH1F(hname, htitle, 10000 , 0.0 , 0.002*E2NEL);
  eloss[i]->SetXTitle("Hit Energy (GeV)");
 } 
////// end booking histograms
 	
 Long64_t entry = tree->GetEntries();
 cout << "[OVAL]: number of events in test file = " << entry << endl;

 for ( int ev = 1; ev <=entry; ev++) {
	srcbrnch->GetEntry(ev);
	
// get Strip Barrel info
        std::vector<PTrackerSimHit::Hit> Hits = TrkSimHits.getHits();
//	cout << "Number of hits " << Hits.size() << endl;
  	for (Int_t i = 0; i < Hits.size(); ++i) {
  	    int id = 0;
  	    if (Hits[i]._sysID < 10 || Hits[i]._sysID > 400) { 
  	       cout << " Undefined system" << endl;
  	       continue;
  	    }
  	    if (Hits[i]._sysID < 100) id = Hits[i]._sysID/10 - 1;
  	    else id = Hits[i]._sysID/100 + 7;
  	    id = int(id/2);
//  	    cout << Hits[i]._sysID << " " << id << endl;
  	    eloss[id]->Fill(Hits[i]._eloss*E2NEL);  
  	}
 } 

 Double_t ks[6];
 int n_test[6];  
 int n_ref[6];  
 float s_test[6];  
 float s_ref[6];
   
 for(int i=0; i<6; i++) {
   n_test[i] = eloss[i]->GetEntries();
   n_ref[i] = ref[i]->GetEntries();
   s_test[i] = eloss[i]->Integral();
   s_ref[i] = ref[i]->Integral();
   
   if (n_test[i] == 0 && n_ref[i] == 0) {
    cout << "[OVAL]: Reference and test histograms are empty" << endl;
    continue;
   } 
   if (s_test[i] < 0.00000001 && s_ref[i] < 0.00000001) {
    cout << "[OVAL]: Reference and test histograms have zero integral" << endl;
    continue;
   } 
   if ((n_test[i] != 0 && n_ref[i] == 0) || (n_test[i] == 0 && n_ref[i] != 0)) {
    cout << "[OVAL]: # of hits in test histogram " << n_test[i] << 
            " # of hits in reference histogram " << n_ref[i] << endl;
   } 		    
   if ((s_test[i] > 0.00000001 && s_ref[i] < 0.00000001) || 
       (s_test[i] < 0.00000001 && s_ref[i] > 0.00000001)) {
    cout << "[OVAL]: integral of test histogram " << s_test[i] << 
            " integral of reference histogram " << s_ref[i] << endl;
    continue;
   }    
   eloss[i]->Sumw2();
   ref[i]->Sumw2();
   if (stat == "KS") ks[i] = eloss[i]->KolmogorovTest(ref[i]);
   if (stat == "chi2") {
    ref[i]->Rebin(50);
    eloss[i]->Rebin(50);     
    ks[i] = eloss[i]->Chi2Test(ref[i],"OU");
   }
   eloss[i]->SetMarkerStyle(20);
   eloss[i]->SetMarkerSize(0.5);
   eloss[i]->SetMarkerColor(1);
   ref[i]->SetLineColor(2);
   string statp = stat+"prob";
   cout<<"[OVAL]: mean eta "<< mean_eta << " number of hits/event in test "<< n_test[i]/float(entry) <<
         " number of hits/event in ref "<< n_ref[i]/1000. << " " << statp <<" = " << ks[i] << endl;
 }
 
 char prob[24];
 float max;
 for(int i=0; i<4; i++) {
  if (n_test[i] == 0 || n_ref[i] == 0) continue;
  SiTrk->cd(i+1);
  if (stat == "KS") { 
   ref[i]->Rebin(100);
   eloss[i]->Rebin(100);
  }  
  if (s_ref[i] > 0.00000001) ref[i]->Scale(1.0/s_ref[i]); 
  if (s_test[i] > 0.00000001) eloss[i]->Scale(1.0/s_test[i]); 
  float max_r = ref[i]->GetMaximum();
  float max_t = eloss[i]->GetMaximum();
  if (max_r>max_t) max = 1.1*max_r;
  else max = 1.1*max_t;
  ref[i]->SetMaximum(max);
  ref[i]->SetLineColor(2);
  ref[i]->Draw("h");
  eloss[i]->Draw("pesame");
  if (stat == "KS") sprintf (prob,"KSprob  %2.3f",ks[i]);
  if (stat == "chi2") sprintf (prob,"chi2prob  %2.3f",ks[i]);
  tx = new TLatex (0.0003,0.9*max,prob);
  tx->SetTextColor(1);
  tx->SetTextSize(0.04);
  tx->Draw("same");
 }
 
 for(int i=0; i<2; i++) {
  if (n_test[i+4] == 0 || n_ref[i+4] == 0) continue;
  PxTrk->cd(i+1);
  if (stat == "KS") {
   ref[i+4]->Rebin(100);
   eloss[i+4]->Rebin(100);
  }
  if (s_ref[i+4] > 0.00000001) ref[i+4]->Scale(1.0/s_ref[i+4]); 
  if (s_test[i+4] > 0.00000001) eloss[i+4]->Scale(1.0/s_test[i+4]);     
  float max_r = ref[i+4]->GetMaximum();
  float max_t = eloss[i+4]->GetMaximum();
  if (max_r>max_t) max = 1.1*max_r;
  else max = 1.1*max_t;
  ref[i+4]->SetMaximum(max);
  ref[i+4]->SetLineColor(2);
  ref[i+4]->Draw("h");
  eloss[i+4]->Draw("pesame");
  if (stat == "KS") sprintf (prob,"KSprob  %2.3f",ks[i+4]);
  if (stat == "chi2") sprintf (prob,"chi2prob  %2.3f",ks[i+4]);
  tx = new TLatex (0.0003,0.9*max,prob);
  tx->SetTextColor(1);
  tx->SetTextSize(0.04);
  tx->Draw("same");
 }
 string tmp1 = "eloss_" + stat + "_" + range + "_si.eps";
 string tmp2 = "eloss_" + stat + "_" + range + "_px.eps";
 SiTrk->Print(tmp1.c_str());
 PxTrk->Print(tmp2.c_str());
}
