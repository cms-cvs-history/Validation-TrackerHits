#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include <iostream>
#include <fstream>

const float E2NEL = 1.; 

// Reference file
char filename_ref[] ="$ROOTFile_ref";     // The Name of reference File.
TFile * rfile = new TFile (filename_ref);      //Open reference histo File. 
ofstream outfile("LowKS_eloss_list.dat");

char ref_name1[20];
char ref_name2[20];
char ref_name3[20];
char ref_name4[20];
char ref_name5[20];
char ref_name6[20];

TH1F *eloss_ref_TIB[12], *ref1[12]; 
TH1F *eloss_ref_TOB[12], *ref2[12]; 
TH1F *eloss_ref_TID[12], *ref3[12]; 
TH1F *eloss_ref_TEC[12], *ref4[12]; 
TH1F *eloss_ref_BPIX[12], *ref5[12]; 
TH1F *eloss_ref_FPIX[12], *ref6[12]; 
  
TH1F *hsum_st, *hsum_px;
TH1F *hsum_TIB;
TH1F *hsum_TOB;
TH1F *hsum_TID;
TH1F *hsum_TEC;
TH1F *hsum_BPIX;
TH1F *hsum_FPIX; 

// Test file
char filename[] ="$ROOTFile_test";     // The Name of Root File.
char treename[] ="Events";            // The Name of Tree.
TString brnchname = "PTrackerSimHit_trackerHitsValid_TrkHits_TrackerValidation.obj";
  
float de[1000000];
int ids[1000000];
int nhit;
////// booking histograms
const char *SysName[] = {"TIB","TOB","TID","TEC","BPIX","FPIX"};
const char *Region[] = {"005","051","115","152","225","253","-050","-105","-151","-215","-252","-325"};

TH1F *h1[12];
TH1F *h2[12];
TH1F *h3[12];
TH1F *h4[12];
TH1F *h5[12];
TH1F *h6[12];

char hname1[50], htitle1[80];
char hname2[50], htitle2[80];
char hname3[50], htitle3[80];
char hname4[50], htitle4[80];
char hname5[50], htitle5[80];
char hname6[50], htitle6[80];

string stat;
 
gROOT ->Reset();

void EnergyLoss_compare_all(int arg)
{
  
  /*=======================================================*/
  /* This Macros is used to plot the Energy deposited      */
  /* by a particle in different Tracker subsystems         */
  /*=======================================================*/
  
  int arg == 1;
  if (arg == 1) stat = "KS";
  if (arg == 2) stat = "chi2";
  
  
  hsum_TIB = new TH1F("ks_TIB", "KS summary eloss TIB", 22 , -0.05 , 1.05);
  hsum_TOB = new TH1F("ks_TOB", "KS summary eloss TOB", 22 , -0.05 , 1.05);
  hsum_TID = new TH1F("ks_TID", "KS summary eloss TID", 22 , -0.05 , 1.05);
  hsum_TEC = new TH1F("ks_TEC", "KS summary eloss TEC", 22 , -0.05 , 1.05);
  hsum_BPIX = new TH1F("ks_BPIX", "KS summary eloss BPIX", 22 , -0.05 , 1.05);
  hsum_FPIX = new TH1F("ks_FPIX", "KS summary eloss FPIX", 22 , -0.05 , 1.05);
  hsum_st = new TH1F("ks_st", "KS summary eloss STRIPS", 22 , -0.05 , 1.05);
  hsum_px = new TH1F("ks_px", "KS summary eloss PIXELS", 22 , -0.05 , 1.05);
  
  for(int i=0; i<12; i++) {
    sprintf (ref_name1,"Eloss_TIB_%i",i+1);
    sprintf (ref_name2,"Eloss_TOB_%i",i+1);
    sprintf (ref_name3,"Eloss_TID_%i",i+1);
    sprintf (ref_name4,"Eloss_TEC_%i",i+1);
    sprintf (ref_name5,"Eloss_BPIX_%i",i+1);
    sprintf (ref_name6,"Eloss_FPIX_%i",i+1);
    
    eloss_ref_TIB[i] = (TH1F*)rfile->Get(ref_name1);
    eloss_ref_TOB[i] = (TH1F*)rfile->Get(ref_name2);
    eloss_ref_TID[i] = (TH1F*)rfile->Get(ref_name3);
    eloss_ref_TEC[i] = (TH1F*)rfile->Get(ref_name4);
    eloss_ref_BPIX[i] = (TH1F*)rfile->Get(ref_name5);
    eloss_ref_FPIX[i] = (TH1F*)rfile->Get(ref_name6);
    
    ref1[i]       = (TH1F*)eloss_ref_TIB[i]->Clone();
    ref2[i]       = (TH1F*)eloss_ref_TOB[i]->Clone();
    ref3[i]       = (TH1F*)eloss_ref_TID[i]->Clone();
    ref4[i]       = (TH1F*)eloss_ref_TEC[i]->Clone();
    ref5[i]       = (TH1F*)eloss_ref_BPIX[i]->Clone();
    ref6[i]       = (TH1F*)eloss_ref_FPIX[i]->Clone();
  }
 
  gStyle->SetNdivisions(504,"XYZ");
  gStyle->SetStatH(0.18);
  gStyle->SetStatW(0.35);
  
  TFile * file = new TFile (filename);      //Open Root File.
  
  TTree *tree = dynamic_cast<TTree*>(file->Get(treename));
  assert(tree != 0);
  
  // get branch from tree
  TBranch *srcbrnch = tree->GetBranch(brnchname);
  assert(srcbrnch != 0);
  
  // set access to branch as proper object
  PTrackerSimHit TrkSimHits;
  srcbrnch->SetAddress(&TrkSimHits);
  
  
  for(int i=0; i<12; i++) {
    sprintf (htitle1,"Energy loss in TIB %s", Region[i]);
    sprintf (htitle2,"Energy loss in TOB %s", Region[i]);
    sprintf (htitle3,"Energy loss in TID %s", Region[i]);
    sprintf (htitle4,"Energy loss in TEC %s", Region[i]);
    sprintf (htitle5,"Energy loss in BPIX %s", Region[i]);
    sprintf (htitle6,"Energy loss in FPIX %s", Region[i]);
    
    sprintf (hname1,"Eloss_TIB_%i",i+1);
    sprintf (hname2,"Eloss_TOB_%i",i+1);
    sprintf (hname3,"Eloss_TID_%i",i+1);
    sprintf (hname4,"Eloss_TEC_%i",i+1);
    sprintf (hname5,"Eloss_BPIX_%i",i+1);
    sprintf (hname6,"Eloss_FPIX_%i",i+1);
    
    h1[i]  = new TH1F(hname1, htitle1, 10000 , 0.0 , 0.001*E2NEL);
    h2[i]  = new TH1F(hname2, htitle2, 10000 , 0.0 , 0.001*E2NEL);
    h3[i]  = new TH1F(hname3, htitle3, 10000 , 0.0 , 0.001*E2NEL);
    h4[i]  = new TH1F(hname4, htitle4, 10000 , 0.0 , 0.001*E2NEL);
    h5[i]  = new TH1F(hname5, htitle5, 10000 , 0.0 , 0.001*E2NEL);
    h6[i]  = new TH1F(hname6, htitle6, 10000 , 0.0 , 0.001*E2NEL);
    
    h1[i]->SetXTitle("Hit Energy (GeV)"); 
    h2[i]->SetXTitle("Hit Energy (GeV)"); 
    h3[i]->SetXTitle("Hit Energy (GeV)"); 
    h4[i]->SetXTitle("Hit Energy (GeV)"); 
    h5[i]->SetXTitle("Hit Energy (GeV)"); 
    h6[i]->SetXTitle("Hit Energy (GeV)"); 
  } 
  
 ////// end booking histograms
  
  Long64_t entry = tree->GetEntries();
  cout << "[OVAL]: number of events in test file = " << entry << endl;
  
  for ( int ev = 1; ev <=entry; ev++) {
    srcbrnch->GetEntry(ev);
    int ir = 0;   
    // get G4Track info
    int ntrack = TrkSimHits.getnG4Trk();
    if (ntrack>0) std::vector<PTrackerSimHit::Trk> G4Trk = TrkSimHits.getG4Trk(); 
    else {
      cout << "No tracks are found associated with a hit!!!" << endl;
      continue;
    }
    if (G4Trk[0].eta>0.0 && G4Trk[0].eta<=0.5) ir = 1;
    if (G4Trk[0].eta>0.5 && G4Trk[0].eta<=1.0) ir = 2;
    if (G4Trk[0].eta>1.0 && G4Trk[0].eta<=1.5) ir = 3;
    if (G4Trk[0].eta>1.5 && G4Trk[0].eta<=2.0) ir = 4;
    if (G4Trk[0].eta>2.0 && G4Trk[0].eta<=2.5) ir = 5;
    if (G4Trk[0].eta>2.5) ir = 6;
    
    if (G4Trk[0].eta>-0.5 && G4Trk[0].eta<=0.0) ir = 7;
    if (G4Trk[0].eta>-1.0 && G4Trk[0].eta<=-0.5) ir = 8;
    if (G4Trk[0].eta>-1.5 && G4Trk[0].eta<=-1.0) ir = 9;
    if (G4Trk[0].eta>-2.0 && G4Trk[0].eta<=-1.5) ir = 10;
    if (G4Trk[0].eta>-2.5 && G4Trk[0].eta<=-2.0) ir = 11;
    if (G4Trk[0].eta<=-2.5) ir = 12;
    
    // get Strip Barrel info
    std::vector<PTrackerSimHit::Hit> Hits = TrkSimHits.getHits();
    for (Int_t i = 0; i < Hits.size(); ++i) {
      int id = 0;
      if (Hits[i]._sysID < 10 || Hits[i]._sysID > 400) { 
	cout << " Undefined system" << endl;
	continue;
      }
      if (Hits[i]._sysID < 100) id = Hits[i]._sysID/10 - 1;
      else id = Hits[i]._sysID/100 + 7;
      id = int(id/2);
      if (id == 0) h1[ir-1]->Fill(Hits[i]._eloss*E2NEL);
      if (id == 1) h2[ir-1]->Fill(Hits[i]._eloss*E2NEL);  
      if (id == 2) h3[ir-1]->Fill(Hits[i]._eloss*E2NEL);  
      if (id == 3) h4[ir-1]->Fill(Hits[i]._eloss*E2NEL);  
      if (id == 4) h5[ir-1]->Fill(Hits[i]._eloss*E2NEL);  
      if (id == 5) h6[ir-1]->Fill(Hits[i]._eloss*E2NEL);	    
    }
  } 
  
  Double_t ks1[12],ks2[12],ks3[12],ks4[12],ks5[12],ks6[12];
  int n_test1[12],n_test2[12],n_test3[12],n_test4[12],n_test5[12],n_test6[12];  
  int n_ref1[12],n_ref2[12],n_ref3[12],n_ref4[12],n_ref5[12],n_ref6[12];  
  float s_test1[12],s_test2[12],s_test3[12],s_test4[12],s_test5[12],s_test6[12];  
  float s_ref1[12],s_ref2[12],s_ref3[12],s_ref4[12],s_ref5[12],s_ref6[12];
  
 //////////////////  TIB
  
 for(int i=0; i<12; i++) {
   n_test1[i] = h1[i]->GetEntries();
   n_ref1[i] = ref1[i]->GetEntries();
   s_test1[i] = h1[i]->Integral();
   s_ref1[i] = ref1[i]->Integral();
   
   if (n_test1[i] == 0 && n_ref1[i] == 0) {
     cout << "[OVAL]: Reference and test histograms are empty for TIB" << endl;
     continue;
   } 
   if (s_test1[i] < 0.00000001 && s_ref1[i] < 0.00000001) {
     cout << "[OVAL]: Reference and test histograms have zero integral for TIB" << endl;
     continue;
   } 
   if ((n_test1[i] != 0 && n_ref1[i] == 0) || (n_test1[i] == 0 && n_ref1[i] != 0)) {
     cout << "[OVAL]: # of hits in test TIB histogram " << n_test1[i] << 
       " # of hits in reference TIB histogram " << n_ref1[i] << endl;
   } 		    
   if ((s_test1[i] > 0.00000001 && s_ref1[i] < 0.00000001) || 
       (s_test1[i] < 0.00000001 && s_ref1[i] > 0.00000001)) {
     cout << "[OVAL]: integral of test TIB histogram " << s_test1[i] << 
       " integral of reference TIB histogram " << s_ref1[i] << endl;
     continue;
   }    
   h1[i]->Sumw2();
   ref1[i]->Sumw2();
   if (stat == "KS") ks1[i] = h1[i]->KolmogorovTest(ref1[i]);
   if (stat == "chi2") {
     ref1[i]->Rebin(50);
     h1[i]->Rebin(50);     
     ks1[i] = h1[i]->Chi2Test(ref1[i],"OU");
   }
   h1[i]->SetMarkerStyle(20);
   h1[i]->SetMarkerSize(0.5);
   h1[i]->SetMarkerColor(1);
   ref1[i]->SetLineColor(2);
   string statp = stat+"prob";
   cout<<
     "[OVAL]: eta region "<< Region[i] << " TIB: number of hits/event in test "<<
     n_test1[i]/float(entry) <<
     " number of hits/event in ref "<< n_ref1[i]/18000. << " " << statp <<" = " <<
     ks1[i] << endl;
   if (ks1[i] < 0.1) outfile << "TIB eloss: eta region "<< Region[i] <<" KS probability = "<< ks1[i] <<" "<<endl;
 }
 
 //////////////////  end TIB
 
 //////////////////  TOB
 
 for(int i=0; i<12; i++) {
   n_test2[i] = h2[i]->GetEntries();
   n_ref2[i] = ref2[i]->GetEntries();
   s_test2[i] = h2[i]->Integral();
   s_ref2[i] = ref2[i]->Integral();
   
   if (n_test2[i] == 0 && n_ref2[i] == 0) {
     cout << "[OVAL]: Reference and test histograms are empty for TOB" << endl;
     continue;
   } 
   if (s_test2[i] < 0.00000001 && s_ref2[i] < 0.00000001) {
     cout << "[OVAL]: Reference and test histograms have zero integral for TOB" << endl;
     continue;
   } 
   if ((n_test2[i] != 0 && n_ref2[i] == 0) || (n_test2[i] == 0 && n_ref2[i] != 0)) {
     cout << "[OVAL]: # of hits in test TOB histogram " << n_test2[i] << 
       " # of hits in reference TOB histogram " << n_ref2[i] << endl;
   } 		    
   if ((s_test2[i] > 0.00000001 && s_ref2[i] < 0.00000001) || 
       (s_test2[i] < 0.00000001 && s_ref2[i] > 0.00000001)) {
     cout << "[OVAL]: integral of test TOB histogram " << s_test2[i] << 
       " integral of reference TOB histogram " << s_ref2[i] << endl;
    continue;
   }    
   h2[i]->Sumw2();
   ref2[i]->Sumw2();
   if (stat == "KS") ks2[i] = h2[i]->KolmogorovTest(ref2[i]);
   if (stat == "chi2") {
    ref2[i]->Rebin(50);
    h2[i]->Rebin(50);     
    ks2[i] = h2[i]->Chi2Test(ref2[i],"OU");
   }
   h2[i]->SetMarkerStyle(20);
   h2[i]->SetMarkerSize(0.5);
   h2[i]->SetMarkerColor(1);
   ref2[i]->SetLineColor(2);
   string statp = stat+"prob";
   cout<<
     "[OVAL]: eta region "<< Region[i] << " TOB: number of hits/event in test "<<
   n_test2[i]/float(entry) <<
     " number of hits/event in ref "<< n_ref2[i]/18000. << " " << statp <<" = " <<
     ks2[i] << endl;
   if (ks2[i] < 0.1) outfile << "TOB eloss: eta region "<< Region[i] <<" KS probability = "<< ks2[i] <<" "<<endl;
 }
 
 //////////////////  end TOB
 
 //////////////////  TID
 
 for(int i=0; i<12; i++) {
   n_test3[i] = h3[i]->GetEntries();
   n_ref3[i] = ref3[i]->GetEntries();
   s_test3[i] = h3[i]->Integral();
   s_ref3[i] = ref3[i]->Integral();
   
   if (n_test3[i] == 0 && n_ref3[i] == 0) {
     cout << "[OVAL]: Reference and test histograms are empty for TID" << endl;
     continue;
   } 
   if (s_test3[i] < 0.00000001 && s_ref3[i] < 0.00000001) {
     cout << "[OVAL]: Reference and test histograms have zero integral for TID" << endl;
     continue;
   } 
   if ((n_test3[i] != 0 && n_ref3[i] == 0) || (n_test3[i] == 0 && n_ref3[i] != 0)) {
     cout << "[OVAL]: # of hits in test TID histogram " << n_test3[i] << 
       " # of hits in reference TID histogram " << n_ref3[i] << endl;
   } 		    
   if ((s_test3[i] > 0.00000001 && s_ref3[i] < 0.00000001) || 
       (s_test3[i] < 0.00000001 && s_ref3[i] > 0.00000001)) {
     cout << "[OVAL]: integral of test TID histogram " << s_test3[i] << 
       " integral of reference TID histogram " << s_ref3[i] << endl;
     continue;
   }    
   h3[i]->Sumw2();
   ref3[i]->Sumw2();
   if (stat == "KS") ks3[i] = h3[i]->KolmogorovTest(ref3[i]);
   if (stat == "chi2") {
     ref3[i]->Rebin(50);
     h3[i]->Rebin(50);     
     ks3[i] = h3[i]->Chi2Test(ref3[i],"OU");
   }
   h3[i]->SetMarkerStyle(20);
   h3[i]->SetMarkerSize(0.5);
   h3[i]->SetMarkerColor(1);
   ref3[i]->SetLineColor(2);
   string statp = stat+"prob";
   cout<<
     "[OVAL]: eta region "<< Region[i] << " TID: number of hits/event in test "<<
     n_test3[i]/float(entry) <<
     " number of hits/event in ref "<< n_ref3[i]/18000. << " " << statp <<" = " <<
     ks3[i] << endl;
   if (ks3[i] < 0.1) outfile << "TID eloss: eta region "<< Region[i] <<" KS probability = "<< ks3[i] <<" "<<endl;
 }
 
 //////////////////  end TID
 
 //////////////////  TEC
 
 for(int i=0; i<12; i++) {
   n_test4[i] = h4[i]->GetEntries();
   n_ref4[i] = ref4[i]->GetEntries();
   s_test4[i] = h4[i]->Integral();
   s_ref4[i] = ref4[i]->Integral();
   
   if (n_test4[i] == 0 && n_ref4[i] == 0) {
     cout << "[OVAL]: Reference and test histograms are empty for TEC" << endl;
     continue;
   } 
   if (s_test4[i] < 0.00000001 && s_ref4[i] < 0.00000001) {
     cout << "[OVAL]: Reference and test histograms have zero integral for TEC" << endl;
     continue;
   } 
   if ((n_test4[i] != 0 && n_ref4[i] == 0) || (n_test4[i] == 0 && n_ref4[i] != 0)) {
     cout << "[OVAL]: # of hits in test TEC histogram " << n_test4[i] << 
       " # of hits in reference TEC histogram " << n_ref4[i] << endl;
   } 		    
   if ((s_test4[i] > 0.00000001 && s_ref4[i] < 0.00000001) || 
       (s_test4[i] < 0.00000001 && s_ref4[i] > 0.00000001)) {
     cout << "[OVAL]: integral of test TEC histogram " << s_test4[i] << 
       " integral of reference TEC histogram " << s_ref4[i] << endl;
     continue;
   }    
   h4[i]->Sumw2();
   ref4[i]->Sumw2();
   if (stat == "KS") ks4[i] = h4[i]->KolmogorovTest(ref4[i]);
   if (stat == "chi2") {
     ref4[i]->Rebin(50);
     h4[i]->Rebin(50);     
     ks4[i] = h4[i]->Chi2Test(ref4[i],"OU");
   }
   h4[i]->SetMarkerStyle(20);
   h4[i]->SetMarkerSize(0.5);
   h4[i]->SetMarkerColor(1);
   ref4[i]->SetLineColor(2);
   string statp = stat+"prob";
   cout<<
     "[OVAL]: eta region "<< Region[i] << " TEC: number of hits/event in test "<<
     n_test4[i]/float(entry) <<
     " number of hits/event in ref "<< n_ref4[i]/18000. << " " << statp <<" = " <<
     ks4[i] << endl;
   if (ks4[i] < 0.1) outfile << "TEC eloss: eta region "<< Region[i] <<" KS probability = "<< ks4[i] <<" "<<endl;
 }
 
 //////////////////  end TEC 
 
 //////////////////  BPIX
 
 for(int i=0; i<12; i++) {
   n_test5[i] = h5[i]->GetEntries();
   n_ref5[i] = ref5[i]->GetEntries();
   s_test5[i] = h5[i]->Integral();
   s_ref5[i] = ref5[i]->Integral();
   
   if (n_test5[i] == 0 && n_ref5[i] == 0) {
     cout << "[OVAL]: Reference and test histograms are empty for BPIX" << endl;
     continue;
   } 
   if (s_test5[i] < 0.00000001 && s_ref5[i] < 0.00000001) {
     cout << "[OVAL]: Reference and test histograms have zero integral for BPIX" << endl;
     continue;
   } 
   if ((n_test5[i] != 0 && n_ref5[i] == 0) || (n_test5[i] == 0 && n_ref5[i] != 0)) {
     cout << "[OVAL]: # of hits in test BPIX histogram " << n_test5[i] << 
       " # of hits in reference BPIX histogram " << n_ref5[i] << endl;
   } 		    
   if ((s_test5[i] > 0.00000001 && s_ref5[i] < 0.00000001) || 
       (s_test5[i] < 0.00000001 && s_ref5[i] > 0.00000001)) {
     cout << "[OVAL]: integral of test BPIX histogram " << s_test5[i] << 
       " integral of reference BPIX histogram " << s_ref5[i] << endl;
     continue;
   }    
   h5[i]->Sumw2();
   ref5[i]->Sumw2();
   if (stat == "KS") ks5[i] = h5[i]->KolmogorovTest(ref5[i]);
   if (stat == "chi2") {
     ref5[i]->Rebin(50);
     h5[i]->Rebin(50);     
     ks5[i] = h5[i]->Chi2Test(ref5[i],"OU");
   }
   h5[i]->SetMarkerStyle(20);
   h5[i]->SetMarkerSize(0.5);
   h5[i]->SetMarkerColor(1);
   ref5[i]->SetLineColor(2);
   string statp = stat+"prob";
   cout<<
     "[OVAL]: eta region "<< Region[i] << " BPIX: number of hits/event in test "<<
     n_test5[i]/float(entry) <<
     " number of hits/event in ref "<< n_ref5[i]/18000. << " " << statp <<" = " <<
     ks5[i] << endl;
   if (ks5[i] < 0.1) outfile << "BPIX eloss: eta region "<< Region[i] <<" KS probability = "<< ks5[i] <<" "<<endl;
 }
 
 //////////////////  end BPIX 
 
 //////////////////  FPIX
 
 for(int i=0; i<12; i++) {
   n_test6[i] = h6[i]->GetEntries();
   n_ref6[i] = ref6[i]->GetEntries();
   s_test6[i] = h6[i]->Integral();
   s_ref6[i] = ref6[i]->Integral();
   
   if (n_test6[i] == 0 && n_ref6[i] == 0) {
     cout << "[OVAL]: Reference and test histograms are empty for FPIX" << endl;
     continue;
   } 
   if (s_test6[i] < 0.00000001 && s_ref6[i] < 0.00000001) {
     cout << "[OVAL]: Reference and test histograms have zero integral for FPIX" << endl;
     continue;
   } 
   if ((n_test6[i] != 0 && n_ref6[i] == 0) || (n_test6[i] == 0 && n_ref6[i] != 0)) {
     cout << "[OVAL]: # of hits in test FPIX histogram " << n_test6[i] << 
       " # of hits in reference FPIX histogram " << n_ref6[i] << endl;
   } 		    
   if ((s_test6[i] > 0.00000001 && s_ref6[i] < 0.00000001) || 
       (s_test6[i] < 0.00000001 && s_ref6[i] > 0.00000001)) {
     cout << "[OVAL]: integral of test FPIX histogram " << s_test6[i] << 
       " integral of reference FPIX histogram " << s_ref6[i] << endl;
     continue;
   }    
   h6[i]->Sumw2();
   ref6[i]->Sumw2();
   if (stat == "KS") ks6[i] = h6[i]->KolmogorovTest(ref6[i]);
   if (stat == "chi2") {
     ref6[i]->Rebin(50);
     h6[i]->Rebin(50);     
     ks6[i] = h6[i]->Chi2Test(ref6[i],"OU");
   }
   h6[i]->SetMarkerStyle(20);
   h6[i]->SetMarkerSize(0.5);
   h6[i]->SetMarkerColor(1);
   ref6[i]->SetLineColor(2);
   string statp = stat+"prob";
   cout<<
     "[OVAL]: eta region "<< Region[i] << " FPIX: number of hits/event in test "<<
     n_test6[i]/float(entry) <<
     " number of hits/event in ref "<< n_ref6[i]/18000. << " " << statp <<" = " <<
     ks6[i] << endl;
   if (ks6[i] < 0.1) outfile << "FPIX eloss: eta region "<< Region[i] <<" KS probability = "<< ks6[i] <<" "<<endl;
 }
 
 //////////////////  end FPIX 
 
 char prob[24];
 float max;
 
 TCanvas * TIB = new TCanvas("TIB","TIB",600,800);
 TIB->Divide(3,4);
 TCanvas * TOB = new TCanvas("TOB","TOB",600,800);
 TOB->Divide(3,4);
 TCanvas * TID = new TCanvas("TID","TID",600,800);
 TID->Divide(3,4);
 TCanvas * TEC = new TCanvas("TEC","TEC",600,800);
 TEC->Divide(3,4);
 TCanvas * BPIX = new TCanvas("BPIX","BPIX",600,800);
 BPIX->Divide(3,4);
 TCanvas * FPIX = new TCanvas("FPIX","FPIX",600,800);
 FPIX->Divide(3,4);
 
 
 //////////////////  TIB
 for(int i=0; i<12; i++) {
   if (n_test1[i] == 0 || n_ref1[i] == 0) continue;
   TIB->cd(i+1);
   if (stat == "KS") { 
     ref1[i]->Rebin(100);
     h1[i]->Rebin(100);
   }  
   if (s_ref1[i] > 0.00000001) ref1[i]->Scale(1.0/s_ref1[i]); 
   if (s_test1[i] > 0.00000001) h1[i]->Scale(1.0/s_test1[i]); 
   float max_r = ref1[i]->GetMaximum();
   float max_t = h1[i]->GetMaximum();
   if (max_r>max_t) max = 1.1*max_r;
   else max = 1.1*max_t;
   ref1[i]->SetMaximum(max);
   ref1[i]->SetLineColor(2);
   ref1[i]->Draw("h");
   h1[i]->Draw("pesame");
   if (stat == "KS") sprintf (prob,"KSprob  %2.3f",ks1[i]);
   if (stat == "chi2") sprintf (prob,"chi2prob  %2.3f",ks1[i]);
   tx = new TLatex (0.0003,0.9*max,prob);
   tx->SetTextColor(1);
   tx->SetTextSize(0.04);
   tx->Draw("same");
 }
 string tmp_eps = "eloss_TIB_" + stat + ".eps";
 TIB->Print(tmp_eps.c_str());
 //////////////////  end TIB
 //////////////////  TOB
 for(int i=0; i<12; i++) {
   if (n_test2[i] == 0 || n_ref2[i] == 0) continue;
   TOB->cd(i+1);
   if (stat == "KS") { 
     ref2[i]->Rebin(100);
     h2[i]->Rebin(100);
   }  
   if (s_ref2[i] > 0.00000001) ref2[i]->Scale(1.0/s_ref2[i]); 
   if (s_test2[i] > 0.00000001) h2[i]->Scale(1.0/s_test2[i]); 
   float max_r = ref2[i]->GetMaximum();
   float max_t = h2[i]->GetMaximum();
   if (max_r>max_t) max = 1.1*max_r;
   else max = 1.1*max_t;
   ref2[i]->SetMaximum(max);
   ref2[i]->SetLineColor(2);
   ref2[i]->Draw("h");
   h2[i]->Draw("pesame");
   if (stat == "KS") sprintf (prob,"KSprob  %2.3f",ks2[i]);
   if (stat == "chi2") sprintf (prob,"chi2prob  %2.3f",ks2[i]);
   tx = new TLatex (0.0003,0.9*max,prob);
   tx->SetTextColor(1);
   tx->SetTextSize(0.04);
   tx->Draw("same");
 }
 string tmp_eps = "eloss_TOB_" + stat + ".eps";
 TOB->Print(tmp_eps.c_str());
 //////////////////  end TOB
 //////////////////  TID
 for(int i=0; i<12; i++) {
   if (n_test3[i] == 0 || n_ref3[i] == 0) continue;
   TID->cd(i+1);
   if (stat == "KS") { 
     ref3[i]->Rebin(100);
     h3[i]->Rebin(100);
   }  
   if (s_ref3[i] > 0.00000001) ref3[i]->Scale(1.0/s_ref3[i]); 
   if (s_test3[i] > 0.00000001) h3[i]->Scale(1.0/s_test3[i]); 
   float max_r = ref3[i]->GetMaximum();
   float max_t = h3[i]->GetMaximum();
   if (max_r>max_t) max = 1.1*max_r;
   else max = 1.1*max_t;
   ref3[i]->SetMaximum(max);
   ref3[i]->SetLineColor(2);
   ref3[i]->Draw("h");
   h3[i]->Draw("pesame");
   if (stat == "KS") sprintf (prob,"KSprob  %2.3f",ks3[i]);
   if (stat == "chi2") sprintf (prob,"chi2prob  %2.3f",ks3[i]);
   tx = new TLatex (0.0003,0.9*max,prob);
   tx->SetTextColor(1);
   tx->SetTextSize(0.04);
   tx->Draw("same");
 }
 string tmp_eps = "eloss_TID_" + stat + ".eps";
 TID->Print(tmp_eps.c_str());
 //////////////////  end TID
 //////////////////  TEC
 for(int i=0; i<12; i++) {
   if (n_test4[i] == 0 || n_ref4[i] == 0) continue;
   TEC->cd(i+1);
   if (stat == "KS") { 
     ref4[i]->Rebin(100);
     h4[i]->Rebin(100);
   }  
   if (s_ref4[i] > 0.00000001) ref4[i]->Scale(1.0/s_ref4[i]); 
   if (s_test4[i] > 0.00000001) h4[i]->Scale(1.0/s_test4[i]); 
   float max_r = ref4[i]->GetMaximum();
   float max_t = h4[i]->GetMaximum();
   if (max_r>max_t) max = 1.1*max_r;
   else max = 1.1*max_t;
   ref4[i]->SetMaximum(max);
   ref4[i]->SetLineColor(2);
   ref4[i]->Draw("h");
   h4[i]->Draw("pesame");
   if (stat == "KS") sprintf (prob,"KSprob  %2.3f",ks4[i]);
   if (stat == "chi2") sprintf (prob,"chi2prob  %2.3f",ks4[i]);
   tx = new TLatex (0.0003,0.9*max,prob);
   tx->SetTextColor(1);
   tx->SetTextSize(0.04);
   tx->Draw("same");
 }
 string tmp_eps = "eloss_TEC_" + stat + ".eps";
 TEC->Print(tmp_eps.c_str());
 cout << "TEC plots finished " << endl;  
 //////////////////  end TEC
 //////////////////  BPIX
 for(int i=0; i<12; i++) {
   if (n_test5[i] == 0 || n_ref5[i] == 0) continue;
   BPIX->cd(i+1);
   if (stat == "KS") { 
     ref5[i]->Rebin(100);
     h5[i]->Rebin(100);
   }  
   if (s_ref5[i] > 0.00000001) ref5[i]->Scale(1.0/s_ref5[i]); 
   if (s_test5[i] > 0.00000001) h5[i]->Scale(1.0/s_test5[i]); 
   float max_r = ref5[i]->GetMaximum();
   float max_t = h5[i]->GetMaximum();
   if (max_r>max_t) max = 1.1*max_r;
   else max = 1.1*max_t;
   ref5[i]->SetMaximum(max);
   ref5[i]->SetLineColor(2);
   ref5[i]->Draw("h");
   h5[i]->Draw("pesame");
   if (stat == "KS") sprintf (prob,"KSprob  %2.3f",ks5[i]);
   if (stat == "chi2") sprintf (prob,"chi2prob  %2.3f",ks5[i]);
   tx = new TLatex (0.0003,0.9*max,prob);
   tx->SetTextColor(1);
   tx->SetTextSize(0.04);
   tx->Draw("same");
 }
 string tmp_eps = "eloss_BPIX_" + stat + ".eps";
 BPIX->Print(tmp_eps.c_str());
 //////////////////  end BPIX
 cout << "BPIX plots finished " << endl;  
 
 //////////////////  FPIX
 for(int i=0; i<12; i++) {
   if (n_test6[i] == 0 || n_ref6[i] == 0) continue;
   FPIX->cd(i+1);
   if (stat == "KS") { 
     ref6[i]->Rebin(100);
     h6[i]->Rebin(100);
   }  
   if (s_ref6[i] > 0.00000001) ref6[i]->Scale(1.0/s_ref6[i]); 
   if (s_test6[i] > 0.00000001) h6[i]->Scale(1.0/s_test6[i]); 
   float max_r = ref6[i]->GetMaximum();
   float max_t = h6[i]->GetMaximum();
   if (max_r>max_t) max = 1.1*max_r;
   else max = 1.1*max_t;
   ref6[i]->SetMaximum(max);
   ref6[i]->SetLineColor(2);
   ref6[i]->Draw("h");
   h6[i]->Draw("pesame");
   if (stat == "KS") sprintf (prob,"KSprob  %2.3f",ks6[i]);
   if (stat == "chi2") sprintf (prob,"chi2prob  %2.3f",ks6[i]);
   tx = new TLatex (0.0003,0.9*max,prob);
   tx->SetTextColor(1);
   tx->SetTextSize(0.04);
   tx->Draw("same");
 }
 string tmp_eps = "eloss_FPIX_" + stat + ".eps";
 FPIX->Print(tmp_eps.c_str());
 //////////////////  end FPIX
 
 //////////  summary 
 for(int j=0; j<12; j++) { 
   hsum_TIB->Fill(ks1[j]);
   hsum_TOB->Fill(ks2[j]);
   hsum_TID->Fill(ks3[j]);
   hsum_TEC->Fill(ks4[j]);
   hsum_BPIX->Fill(ks5[j]);
   hsum_FPIX->Fill(ks6[j]);
 }
 hsum_st -> Add (hsum_TIB);
 hsum_st -> Add (hsum_TOB);
 hsum_st -> Add (hsum_TID);
 hsum_st -> Add (hsum_TEC); 
 hsum_px -> Add (hsum_BPIX);
 hsum_px -> Add (hsum_FPIX);
 
 TCanvas * s = new TCanvas("s","s",600,800);
 s->Divide(2,4);
 
 s->cd (1);
 hsum_TIB -> Draw();
 s->cd (2);
 hsum_TOB -> Draw();
 s->cd (3);
 hsum_TID -> Draw();
 s->cd (4);
 hsum_TEC -> Draw();
 s->cd (5);
 hsum_BPIX -> Draw();
 s->cd (6);
 hsum_FPIX -> Draw();
 s->cd (7);
 hsum_st -> Draw();
 s->cd (8);
 hsum_px -> Draw();
 
 s->Print("summary_eloss_KS.eps");  
 
}


