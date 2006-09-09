#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include <iostream>
#include <fstream>

// Test file
char filename[] ="$ROOTFile_test";     // The Name of Root File.
//char filename[] ="SimHitMuon_1.root";     // The Name of Root File.
char treename[] ="Events";            // The Name of Tree.
TString brnchname = "PTrackerSimHit_thp_TrkHits_TrackerHits.obj";
TBranch *srcbrnch;
ofstream outfile("LowKS_pos_list.dat");

// Reference file
char filename_ref[] ="$ROOTFile_ref";     // The Name of reference File.
//char filename_ref[] ="position_ref_1.root";     // The Name of reference File.
TFile * rfile;

const int neta = 12;
const int nsys = 6;
TH1F *ref[nsys][neta];
TH1F *tst[nsys][neta];

// systems 
const char* si[] = {"_1","_2","_3","_4","_5","_6"};
const char *SysName[] = {"TIB","TOB","TID","TEC","BPIX","FPIX"};
// eta bins
const char* se[] =
          {"_1","_2","_3","_4","_5","_6","_7","_8","_9","_10","_11","_12"};
const char *Region[] = {"005","051","115","152","225","253",
                       "-050","-105","-151","-215","-252","-325"};  
// limits
const float dx_high[] = {0.03, 0.03, 0.02, 0.03, 0.03, 0.03};
const float dy_high[] = {0.03, 0.03, 0.02, 0.03, 0.03, 0.03};
const float dz_high[] = {0.05, 0.06, 0.05, 0.06, 0.05, 0.05};
const float lx_high[] = {3.2, 5.0, 5.5, 6.2, 0.85, 0.4};   
const float ly_high[] = {6.0, 10., 5.6, 10.5, 3.4, 0.42};
const float zero[] = {0.,0.,0.,0.,0.,0.};
int nbin = 10000;
float xmin[nsys], xmax[nsys];
Long64_t entry;
Double_t ks[nsys][neta];
int n_test[nsys][neta];  
int n_ref[nsys][neta];
float s_test[nsys][neta];  
float s_ref[nsys][neta];

gROOT ->Reset(); 

void Position_compare_all(int arg2)
{
 
 rfile = new TFile (filename_ref);   //Open reference histo File. 

// Open test file and get tree 
 TFile *file = new TFile (filename);      //Open Root File.
 TTree *tree = dynamic_cast<TTree*>(file->Get(treename));  //Fecth The Tree.
 assert(tree != 0);
 
 // get branch from tree
 srcbrnch = tree->GetBranch(brnchname);
 assert(srcbrnch != 0);

 entry = tree->GetEntries();
 cout << "number of events = " << entry << endl;

 string stat;
 if (arg2 == 1) stat = "KS";
 if (arg2 == 2) stat = "chi2";
 
 TH1F *hsum_st, *hsum_px;
 TH1F *hsum_TIB;
 TH1F *hsum_TOB;
 TH1F *hsum_TID;
 TH1F *hsum_TEC;
 TH1F *hsum_BPIX;
 TH1F *hsum_FPIX;
 
 hsum_TIB = new TH1F("ks_TIB", "KS summary TIB", 22 , -0.05 , 1.05);
 hsum_TOB = new TH1F("ks_TOB", "KS summary TOB", 22 , -0.05 , 1.05);
 hsum_TID = new TH1F("ks_TID", "KS summary TID", 22 , -0.05 , 1.05);
 hsum_TEC = new TH1F("ks_TEC", "KS summary TEC", 22 , -0.05 , 1.05);
 hsum_BPIX = new TH1F("ks_BPIX", "KS summary BPIX", 22 , -0.05 , 1.05);
 hsum_FPIX = new TH1F("ks_FPIX", "KS summary FPIX", 22 , -0.05 , 1.05);
 hsum_st = new TH1F("ks_st", "KS summary STRIPS", 22 , -0.05 , 1.05);
 hsum_px = new TH1F("ks_px", "KS summary PIXELS", 22 , -0.05 , 1.05);

// Run all tests 
 test(1, dx_high, dx_high, stat);
 for(int j=0; j<12; j++) { 
  hsum_TIB->Fill(ks[0][j]);
  hsum_TOB->Fill(ks[1][j]);
  hsum_TID->Fill(ks[2][j]);
  hsum_TEC->Fill(ks[3][j]);
  hsum_BPIX->Fill(ks[4][j]);
  hsum_FPIX->Fill(ks[5][j]);
  for (int i=0; i<4; i++) hsum_st->Fill(ks[i][j]);
  for (int i=4; i<6; i++) hsum_px->Fill(ks[i][j]);
  for (int i=0; i<6; i++) 
  if (ks[i][j] < 0.1) outfile << SysName[i] << " Entryx-Exitx test in eta region "<<Region[j] <<" KS probability = "<< ks[i][j] <<endl;
 }
 
 test(2, dy_high, dy_high, stat);
 for(int j=0; j<12; j++) { 
  hsum_TIB->Fill(ks[0][j]);
  hsum_TOB->Fill(ks[1][j]);
  hsum_TID->Fill(ks[2][j]);
  hsum_TEC->Fill(ks[3][j]);
  hsum_BPIX->Fill(ks[4][j]);
  hsum_FPIX->Fill(ks[5][j]);
  for (int i=0; i<4; i++) hsum_st->Fill(ks[i][j]);
  for (int i=4; i<6; i++) hsum_px->Fill(ks[i][j]);
  for (int i=0; i<6; i++) 
  if (ks[i][j] < 0.1) outfile << SysName[i] << " Entryy-Exity test in eta region "<<Region[j] <<" KS probability = "<< ks[i][j] <<endl;
 }

 test(3, zero, dz_high, stat);
 for(int j=0; j<12; j++) { 
  hsum_TIB->Fill(ks[0][j]);
  hsum_TOB->Fill(ks[1][j]);
  hsum_TID->Fill(ks[2][j]);
  hsum_TEC->Fill(ks[3][j]);
  hsum_BPIX->Fill(ks[4][j]);
  hsum_FPIX->Fill(ks[5][j]);
  for (int i=0; i<4; i++) hsum_st->Fill(ks[i][j]);
  for (int i=4; i<6; i++) hsum_px->Fill(ks[i][j]);
  for (int i=0; i<6; i++) 
  if (ks[i][j] < 0.1) outfile << SysName[i] << " abs(Entryz-Exitz) test in eta region "<<Region[j] <<" KS probability = "<< ks[i][j] <<endl;
 }
 
 test(4, lx_high, lx_high, stat);
 for(int j=0; j<12; j++) { 
  hsum_TIB->Fill(ks[0][j]);
  hsum_TOB->Fill(ks[1][j]);
  hsum_TID->Fill(ks[2][j]);
  hsum_TEC->Fill(ks[3][j]);
  hsum_BPIX->Fill(ks[4][j]);
  hsum_FPIX->Fill(ks[5][j]);
  for (int i=0; i<4; i++) hsum_st->Fill(ks[i][j]);
  for (int i=4; i<6; i++) hsum_px->Fill(ks[i][j]);
  for (int i=0; i<6; i++) 
  if (ks[i][j] < 0.1) outfile << SysName[i] << " Localx test in eta region "<<Region[j] <<" KS probability = "<< ks[i][j] <<endl;
 }

 test(5, ly_high, ly_high, stat);
 for(int j=0; j<12; j++) { 
  hsum_TIB->Fill(ks[0][j]);
  hsum_TOB->Fill(ks[1][j]);
  hsum_TID->Fill(ks[2][j]);
  hsum_TEC->Fill(ks[3][j]);
  hsum_BPIX->Fill(ks[4][j]);
  hsum_FPIX->Fill(ks[5][j]);
  for (int i=0; i<4; i++) hsum_st->Fill(ks[i][j]);
  for (int i=4; i<6; i++) hsum_px->Fill(ks[i][j]);
  for (int i=0; i<6; i++) 
  if (ks[i][j] < 0.1) outfile << SysName[i] << " Localy test in eta region "<<Region[j] <<" KS probability = "<< ks[i][j] <<endl;
 }
 
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
 
 s->Print("summary_pos_KS.eps");
}

void test(int iopt, float *low, float *high, string stype)
{ 
// Get reference histo
 string name, hname;
 if (iopt == 1) hname = "Entryx-Exitx"; 
 if (iopt == 2) hname = "Entryy-Exity"; 
 if (iopt == 3) hname = "abs(Entryz-Exitz)"; 
 if (iopt == 4) hname = "Localx"; 
 if (iopt == 5) hname = "Localy";
 
 name = hname;
 if (iopt == 3) name = "Entryz-Exitz"; 
 cout << hname << " " << name << endl;
 
 GetReference(hname);
 cout << "Reference found" << endl;

 for(int i=0; i<nsys; i++) { 
   xmax[i] = high[i];
   xmin[i] = -low[i];
 }
 
 BookTestHistos (hname, nbin, xmin, xmax);
 FillTestHistos (iopt);
 if (stype == "KS")   Compare_KS(); 
 if (stype == "chi2") Compare_chi2();
 cout << "Now plot " << endl; 
 if (iopt == 4 || iopt == 5) Plot(name,stype,0);
 else Plot(name,stype,1);
}


void Plot(string name, string stype, int scale) {

 gStyle->SetNdivisions(504,"XYZ");
 gStyle->SetStatH(0.18);
 gStyle->SetStatW(0.35);  
    
 double norm_r, norm_t;
 float max;
 char prob[24];
 
 
for(int i=0; i<6; i++) {
 
 TCanvas * Si = new TCanvas("si","si",600,800);
 Si->Divide(3,4);
 if (scale == 1) {
//  for(int j=0; j<12; j++) {
//   Si->cd(j+1);
//   gPad->SetLogy();
//  }
 }
 
 for(int j=0; j<12; j++) {
  if (n_test[i][j] == 0 || n_ref[i][j] == 0) continue;
  Si->cd(j+1);
  if (stype == "KS") {
   ref[i][j]->Rebin(100);
   tst[i][j]->Rebin(100);
  }
   
  if (s_ref[i][j] > 0.00000001) ref[i][j]->Scale(1.0/s_ref[i][j]); 
  if (s_test[i][j] > 0.00000001) tst[i][j]->Scale(1.0/s_test[i][j]); 
  float max_r = ref[i][j]->GetMaximum();
  float max_t = tst[i][j]->GetMaximum();
  if (max_r>max_t) max = 1.3*max_r;
  else max = 1.3*max_t;
  ref[i][j]->SetMaximum(max);
  ref[i][j]->SetLineColor(2);
  
  tst[i][j]->SetMarkerStyle(20);
  tst[i][j]->SetMarkerSize(0.5);
  tst[i][j]->SetMarkerColor(1);
  
  ref[i][j]->Draw("h");
  tst[i][j]->Draw("pesame");
  if (stype == "KS") sprintf (prob,"KSprob  %2.3f",ks[i][j]);
  if (stype == "chi2") sprintf (prob,"chi2prob  %2.3f",ks[i][j]);
  if (xmin[i]<0.) tx = new TLatex (xmin[i]*0.9,0.9*max,prob);
  if (xmin[i]>=0.) tx = new TLatex (xmin[i]+0.005,0.9*max,prob);
  tx->SetTextColor(1);
  tx->SetTextSize(0.04);
  tx->Draw("same");
//  gPad->RedrawAxis();
 }
 
 string tmp = "pos_" + name + "_" + SysName[i] + "_" + stype + ".eps";
 Si->Print(tmp.c_str()); 
}
 
}

void Compare_KS()
{
 cout << "KS in Root" << endl;  
   
for(int i=0; i<nsys; i++) { 
 for(int j=0; j<neta; j++) { 
   n_test[i][j] = tst[i][j]->GetEntries();
   n_ref[i][j]  = ref[i][j]->GetEntries();
   s_test[i][j] = tst[i][j]->Integral();
   s_ref[i][j]  = ref[i][j]->Integral();
   
   if (n_test[i][j] == 0 && n_ref[i][j] == 0) {
    cout << "[OVAL]: Reference and test histograms are empty" << endl;
    continue;
   } 
   if (s_test[i][j] < 0.00000001 && s_ref[i][j] < 0.00000001) {
    cout << "[OVAL]: Reference and test histograms integral is zero" << endl;
    continue;
   } 
   if ((n_test[i][j] != 0 && n_ref[i][j] == 0) || (n_test[i][j] == 0 && n_ref[i][j] != 0)) {
    cout << "[OVAL]: # of hits in test histogram " << n_test[i][j] << 
            " # of hits in reference histogram " << n_ref[i][j] << endl;
    continue;
   }
   if ((s_test[i][j] > 0.00000001 && s_ref[i][j] < 0.00000001) || (s_test[i][j] < 0.00000001 && s_ref[i][j] > 0.00000001)) {
    cout << "[OVAL]: integral of test histogram " << s_test[i][j] << 
            " # integral of reference histogram " << s_ref[i][j] << endl;
    continue;
   }
       
   tst[i][j]->Sumw2();
   ref[i][j]->Sumw2();
   ks[i][j] = tst[i][j]->KolmogorovTest(ref[i][j]);  
   cout<<"[OVAL]: number of hits/event in test "<< n_test[i][j]/float(entry) <<
         " number of hits/event in ref "<< n_ref[i][j]/18000. << " KSprob = " << ks[i][j] << endl;   
 }
}
}

void Compare_chi2()
{
 cout << "Chi2Test in Root" << endl;  
for(int i=0; i<nsys; i++) { 
 for(int j=0; j<neta; j++) { 
   n_test[i][j] = tst[i][j]->GetEntries();
   n_ref[i][j]  = ref[i][j]->GetEntries();
   s_test[i][j] = tst[i][j]->Integral();
   s_ref[i][j]  = ref[i][j]->Integral();
   
   if (n_test[i][j] == 0 && n_ref[i][j] == 0) {
    cout << "[OVAL]: Reference and test histograms are empty" << endl;
    continue;
   } 
   if (s_test[i][j] < 0.00000001 && s_ref[i][j] < 0.00000001) {
    cout << "[OVAL]: Reference and test histograms integral is zero" << endl;
    continue;
   } 
   if ((n_test[i][j] != 0 && n_ref[i][j] == 0) || (n_test[i][j] == 0 && n_ref[i][j] != 0)) {
    cout << "[OVAL]: # of hits in test histogram " << n_test[i][j] << 
            " # of hits in reference histogram " << n_ref[i][j] << endl;
    continue;
   }    
   if ((s_test[i][j] > 0.00000001 && s_ref[i][j] < 0.00000001) || (s_test[i][j] < 0.00000001 && s_ref[i][j] > 0.00000001)) {
    cout << "[OVAL]: integral of test histogram " << s_test[i][j] << 
            " # integral of reference histogram " << s_ref[i][j] << endl;
    continue;
   }
   tst[i][j]->Sumw2();
   ref[i][j]->Sumw2();
   tst[i][j]->Rebin(100);
   ref[i][j]->Rebin(100);
   ks[i][j] = tst[i][j]->Chi2Test(ref[i][j],"OU");  
   cout<<"[OVAL]: number of hits/event in test "<< n_test[i][j]/float(entry) <<
         " number of hits/event in ref "<< n_ref[i][j]/18000. << " chi2prob = " << ks[i][j] << endl;     
 }
}
}
void  FillTestHistos(int iopt) 
{
 int ids[1000];
 
 // set access to branch as proper object
 PTrackerSimHit TrkSimHits;
 srcbrnch->SetAddress(&TrkSimHits);
    
 for ( int j = 0; j <  entry; j++) {
   srcbrnch->GetEntry(j);
   int ir = 0;   
    // get G4Track info
   std::vector<PTrackerSimHit::Trk> G4Trk = TrkSimHits.getG4Trk();
   if (G4Trk[0].eta>0.0 && G4Trk[0].eta<=0.5) ir = 1;
   if (G4Trk[0].eta>0.5 && G4Trk[0].eta<=1.0) ir = 2;
   if (G4Trk[0].eta>1.0 && G4Trk[0].eta<=1.5) ir = 3;
   if (G4Trk[0].eta>1.5 && G4Trk[0].eta<=2.0) ir = 4;
   if (G4Trk[0].eta>2.0 && G4Trk[0].eta<=2.5) ir = 5;
   if (G4Trk[0].eta>2.5 && G4Trk[0].eta<=3.0) ir = 6;
   if (G4Trk[0].eta>-0.5 && G4Trk[0].eta<=0.0) ir = 7;
   if (G4Trk[0].eta>-1.0 && G4Trk[0].eta<=-0.5) ir = 8;
   if (G4Trk[0].eta>-1.5 && G4Trk[0].eta<=-1.0) ir = 9;
   if (G4Trk[0].eta>-2.0 && G4Trk[0].eta<=-1.5) ir = 10;
   if (G4Trk[0].eta>-2.5 && G4Trk[0].eta<=-2.0) ir = 11;
   if (G4Trk[0].eta>-3.0 && G4Trk[0].eta<=-2.5) ir = 12;
    
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
    if (iopt == 1) tst[id][ir-1]->Fill(Hits[i]._enx - Hits[i]._exx);  
    if (iopt == 2) tst[id][ir-1]->Fill(Hits[i]._eny - Hits[i]._exy);   
    if (iopt == 3) tst[id][ir-1]->Fill(fabs(Hits[i]._enz - Hits[i]._exz));
    if (iopt == 4) tst[id][ir-1]->Fill(Hits[i]._lpx);  
    if (iopt == 5) tst[id][ir-1]->Fill(Hits[i]._lpy);       
      } 
 }
  for(int i=0; i<nsys; i++) {
  for(int j=0; j<neta; j++) {
   n_test[i][j] = tst[i][j]->GetEntries();
   n_ref[i][j]  = ref[i][j]->GetEntries();
 }  
 }
}

void  GetReference(string sname) {
 TH1F *h[nsys][neta];
 string ss;
 for(int i=0; i<nsys; i++) {
  for(int j=0; j<neta; j++) {
   ss = sname +"_"+ SysName[i]+se[j];
   char* hname = ss.c_str();
//   cout << "GetReference to " << hname << endl;  
   h[i][j]   = (TH1F*) rfile->Get(hname);   
   ref[i][j] = (TH1F*) h[i][j]->Clone();
  }
 }
} 

void  BookTestHistos(string sname, int nbin, float *xmin, float *xmax) {
 TH1F *ht[nsys][neta];
 string ss, st, sa;
 sa = sname + " (cm)";
 for(int i=0; i<nsys; i++) {
  for(int j=0; j<neta; j++) {
   ss = "t_" + sname + si[i] + se[j];
   st = sname + " in " + SysName[i] + " " + Region[j];
   char* hname = ss.c_str();    
   char* htitle = st.c_str();       
   ht[i][j] = new TH1F(hname, htitle, nbin , xmin[i], xmax[i]);
   ht[i][j]->SetXTitle(sa.c_str());
   tst[i][j] = (TH1F*)ht[i][j]->Clone();
//   cout <<"Book histo "<<hname<<" "<<htitle<<" "<<nbin<<" "<<xmin[i]<<" "<<xmax[i]<<endl; 
  }
 }
}












