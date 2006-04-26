#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"

const int nsys = 6;
TH1F *ref[nsys]; 
TH1F *tst[nsys]; 

// Test file
char filename[] ="$ROOTFile_test";     // The Name of Root File.
//char filename[] ="SimHitMuon_1.root";     // The Name of Root File.
char treename[] ="Events";            // The Name of Tree.
TString brnchname = "PTrackerSimHit_thp_TrkHits.obj";
TBranch *srcbrnch;

// Reference file
char filename_ref[] ="$ROOTFile_ref";     // The Name of reference File.
//char filename_ref[] ="position_ref_1.root";     // The Name of reference File.
TFile * rfile;


// systems 
const char* si[] = {"_1","_2","_3","_4","_5","_6"};
const char *SysName[] = {"TIB","TOB","TID","TEC","PIB","PEC"};
// limits
const float dx_high[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
const float dy_high[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
const float dz_high[] = {0.05, 0.06, 0.05, 0.06, 0.05, 0.05};
const float lx_high[] = {3.2, 5.0, 5.5, 6.2, 0.85, 0.4};   
const float ly_high[] = {6.0, 10., 5.6, 10.5, 3.4, 0.42};
const float zero[] = {0.,0.,0.,0.,0.,0.};
int nbin = 10000;
float xmin[nsys], xmax[nsys];
Long64_t entry;
Double_t ks[nsys];
float mean_eta;
string range;
int n_test[nsys];  
int n_ref[nsys];
float s_test[nsys];  
float s_ref[nsys];

gROOT ->Reset(); 

void Position_compare(int arg, int arg2)
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
 
 TH1F *heta = (TH1F*)rfile->Get("primary_eta");
 mean_eta = heta->GetMean();  
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

 if (arg == 1) test(arg, dx_high, dx_high, stat);
 if (arg == 2) test(arg, dy_high, dy_high, stat);
 if (arg == 3) test(arg, zero, dz_high, stat);
 if (arg == 4) test(arg, lx_high, lx_high, stat);
 if (arg == 5) test(arg, ly_high, ly_high, stat);

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
  
 string si_cname = "SiTrk_" + name;
 string px_cname = "PxTrk_" + name;
   
 TCanvas * SiTrk = new TCanvas(si_cname.c_str(),si_cname.c_str(),600,800);
 SiTrk->Divide(2,2);
 TCanvas * PxTrk = new TCanvas(px_cname.c_str(),px_cname.c_str(),600,600);
 PxTrk->Divide(1,2);
 
 double norm_r, norm_t;
 float max;
 char prob[24];
 
 if (scale == 1) {
  for(int i=0; i<4; i++) {
   SiTrk->cd(i+1);
   gPad->SetLogy();
  }
  for(int i=0; i<2; i++) {
   PxTrk->cd(i+1);
   gPad->SetLogy();
  }
 }
 
 for(int i=0; i<4; i++) {
  if (n_test[i] == 0 || n_ref[i] == 0) continue;
  SiTrk->cd(i+1);
  if (stype == "KS") {
   ref[i]->Rebin(100);
   tst[i]->Rebin(100);
  }
   
  if (s_ref[i] > 0.00000001) ref[i]->Scale(1.0/s_ref[i]); 
  if (s_test[i] > 0.00000001) tst[i]->Scale(1.0/s_test[i]); 
  float max_r = ref[i]->GetMaximum();
  float max_t = tst[i]->GetMaximum();
  if (max_r>max_t) max = 1.3*max_r;
  else max = 1.3*max_t;
  ref[i]->SetMaximum(max);
  ref[i]->SetLineColor(2);
  
  tst[i]->SetMarkerStyle(20);
  tst[i]->SetMarkerSize(0.5);
  tst[i]->SetMarkerColor(1);
  
  ref[i]->Draw("h");
  tst[i]->Draw("pesame");
  if (stype == "KS") sprintf (prob,"KSprob  %2.3f",ks[i]);
  if (stype == "chi2") sprintf (prob,"chi2prob  %2.3f",ks[i]);
  tx = new TLatex (xmin[i]*0.9,0.5*max,prob);
  tx->SetTextColor(1);
  tx->SetTextSize(0.04);
  tx->Draw("same");
//  gPad->RedrawAxis();
 }
 
 for(int i=0; i<2; i++) {
  if (n_test[i+4] == 0 || n_ref[i+4] == 0) continue;
  PxTrk->cd(i+1);
  if (stype == "KS") {
   ref[i+4]->Rebin(100);
   tst[i+4]->Rebin(100);
  }
  
  if (s_ref[i+4] > 0.00000001) ref[i+4]->Scale(1.0/s_ref[i+4]); 
  if (s_test[i+4] > 0.00000001) tst[i+4]->Scale(1.0/s_test[i+4]);     
  float max_r = ref[i+4]->GetMaximum();
  float max_t = tst[i+4]->GetMaximum();
  if (max_r>max_t) max = 1.3*max_r;
  else max = 1.3*max_t;
  ref[i+4]->SetMaximum(max);
  ref[i+4]->SetLineColor(2);
  
  tst[i+4]->SetMarkerStyle(20);
  tst[i+4]->SetMarkerSize(0.5);
  tst[i+4]->SetMarkerColor(1);
  
  ref[i+4]->Draw("h");
  tst[i+4]->Draw("pesame");
  if (stype == "KS") sprintf (prob,"KSprob  %2.3f",ks[i+4]);
  if (stype == "chi2") sprintf (prob,"chi2prob  %2.3f",ks[i+4]);
  tx = new TLatex (xmin[i]*0.9,0.5*max,prob);
  tx->SetTextColor(1);
  tx->SetTextSize(0.04);
  tx->Draw("same");
//  gPad->RedrawAxis();
 }
 string strips = "pos_" + name + "_" + stype + "_" + range + "_si.eps";
 string pixel  = "pos_" + name + "_" + stype + "_" + range + "_px.eps";
 SiTrk->Print(strips.c_str());
 PxTrk->Print(pixel.c_str());
}

void Compare_KS()
{
 cout << "KS in Root" << endl;  
   
 for(int i=0; i<nsys; i++) { 
   n_test[i] = tst[i]->GetEntries();
   n_ref[i]  = ref[i]->GetEntries();
   s_test[i] = tst[i]->Integral();
   s_ref[i]  = ref[i]->Integral();
   
   if (n_test[i] == 0 && n_ref[i] == 0) {
    cout << "[OVAL]: Reference and test histograms are empty" << endl;
    continue;
   } 
   if (s_test[i] < 0.00000001 && s_ref[i] < 0.00000001) {
    cout << "[OVAL]: Reference and test histograms integral is zero" << endl;
    continue;
   } 
   if ((n_test[i] != 0 && n_ref[i] == 0) || (n_test[i] == 0 && n_ref[i] != 0)) {
    cout << "[OVAL]: # of hits in test histogram " << n_test[i] << 
            " # of hits in reference histogram " << n_ref[i] << endl;
    continue;
   }
   if ((s_test[i] > 0.00000001 && s_ref[i] < 0.00000001) || (s_test[i] < 0.00000001 && s_ref[i] > 0.00000001)) {
    cout << "[OVAL]: integral of test histogram " << s_test[i] << 
            " # integral of reference histogram " << s_ref[i] << endl;
    continue;
   }
       
   tst[i]->Sumw2();
   ref[i]->Sumw2();
   ks[i] = tst[i]->KolmogorovTest(ref[i]);  
   cout<<"[OVAL]: mean eta "<< mean_eta << " number of hits/event in test "<< n_test[i]/float(entry) <<
         " number of hits/event in ref "<< n_ref[i]/1000. << " KSprob = " << ks[i] << endl;	 
 }
}

void Compare_chi2()
{
 cout << "Chi2Test in Root" << endl;  
 for(int i=0; i<nsys; i++) { 
   n_test[i] = tst[i]->GetEntries();
   n_ref[i]  = ref[i]->GetEntries();
   s_test[i] = tst[i]->Integral();
   s_ref[i]  = ref[i]->Integral();
   
   if (n_test[i] == 0 && n_ref[i] == 0) {
    cout << "[OVAL]: Reference and test histograms are empty" << endl;
    continue;
   } 
   if (s_test[i] < 0.00000001 && s_ref[i] < 0.00000001) {
    cout << "[OVAL]: Reference and test histograms integral is zero" << endl;
    continue;
   } 
   if ((n_test[i] != 0 && n_ref[i] == 0) || (n_test[i] == 0 && n_ref[i] != 0)) {
    cout << "[OVAL]: # of hits in test histogram " << n_test[i] << 
            " # of hits in reference histogram " << n_ref[i] << endl;
    continue;
   }    
   if ((s_test[i] > 0.00000001 && s_ref[i] < 0.00000001) || (s_test[i] < 0.00000001 && s_ref[i] > 0.00000001)) {
    cout << "[OVAL]: integral of test histogram " << s_test[i] << 
            " # integral of reference histogram " << s_ref[i] << endl;
    continue;
   }
   tst[i]->Sumw2();
   ref[i]->Sumw2();
   tst[i]->Rebin(100);
   ref[i]->Rebin(100);
   ks[i] = tst[i]->Chi2Test(ref[i],"OU");  
   cout<<"[OVAL]: mean eta "<< mean_eta <<" number of hits/event in test "<< n_test[i]/float(entry) <<
         " number of hits/event in ref "<< n_ref[i]/1000. << " chi2prob = " << ks[i] << endl;	 
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
	if (iopt == 1) tst[id]->Fill(Hits[i]._enx - Hits[i]._exx);	
	if (iopt == 2) tst[id]->Fill(Hits[i]._eny - Hits[i]._exy);	  
	if (iopt == 3) tst[id]->Fill(fabs(Hits[i]._enz - Hits[i]._exz));
	if (iopt == 4) tst[id]->Fill(Hits[i]._lpx);	  
	if (iopt == 5) tst[id]->Fill(Hits[i]._lpy);	  
		
	}	
 }
  for(int i=0; i<nsys; i++) {
   n_test[i] = tst[i]->GetEntries();
   n_ref[i]  = ref[i]->GetEntries();
 }  
 
}

void  GetReference(string sname) {
 TH1F *h[nsys];
 string ss;
 for(int i=0; i<nsys; i++) {
  ss = sname + si[i];
  char* hname = ss.c_str();
  cout << "GetReference to " << hname << endl;  
  h[i]   = (TH1F*) rfile->Get(hname);	
  ref[i] = (TH1F*) h[i]->Clone();
 }
} 

void  BookTestHistos(string sname, int nbin, float *xmin, float *xmax) {
 TH1F *ht[nsys];
 string ss, st, sa;
 sa = sname + " (cm)";
 for(int i=0; i<nsys; i++) {
  ss = "t_" + sname + si[i];
  st = sname + " in " + SysName[i];
  char* hname = ss.c_str();   	
  char* htitle = st.c_str();   	
  ht[i] = new TH1F(hname, htitle, nbin , xmin[i], xmax[i]);
  ht[i]->SetXTitle(sa.c_str());
  tst[i] = (TH1F*)ht[i]->Clone();
  cout <<"Book histo "<<hname<<" "<<htitle<<" "<<nbin<<" "<<xmin[i]<<" "<<xmax[i]<<endl; 
 }
}
