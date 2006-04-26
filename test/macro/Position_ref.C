#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"

void Position_ref(int k)
{	
/*=======================================================*/
/* This Macros is used to plot the Energy deposited      */
/* by a particle in different Tracker subsystems         */
/*=======================================================*/

 char filename[] ="$ROOTFile";     // The Name of Root File.
// char filename[] = "simhitoutput.root";     // The Name of Root File.
 char treename[] ="Events";            // The Name of Tree.
 TString brnchname = "PTrackerSimHit_thp_TrkHits.obj";

 gROOT ->Reset();
 gStyle->SetNdivisions(504,"XYZ");
 gStyle->SetStatH(0.18);
 gStyle->SetStatW(0.35);

 // clear memory of file name
 delete gROOT->GetListOfFiles()->FindObject(filename);
 cout << "opening " << filename << endl;

 // open source file
 TFile * file = new TFile (filename);      //Open Root File.

 // get tree from file
 TTree *tree = dynamic_cast<TTree*>(file->Get(treename));
 assert(tree != 0);
 
 // get branch from tree
 TBranch *srcbrnch = tree->GetBranch(brnchname);
 assert(srcbrnch != 0);
 
 // get number of events
 Int_t entry = tree->GetEntries();
 cout << "number of events = " << entry << endl;
  
 
 float ent_x[1000],ent_y[1000],ent_z[1000];
 float ext_x[1000],ext_y[1000],ext_z[1000];
 float loc_x[1000],loc_y[1000];
 int ids[1000];
 float eta[1500];
 int nhit, nprim;

 // set access to branch as proper object
 PTrackerSimHit TrkSimHits;
 srcbrnch->SetAddress(&TrkSimHits);
 
 cout << "Setting branch address" << endl;

////// booking histograms
 const char *SysName[] = {"TIB","TOB","TID","TEC","PIB","PEC"};
 
 TH1F *h1[6],*h2[6],*h3[6],*h4[6],*h5[6];
 TH1F *heta;
 char hname1[30], htitle1[40];
 char hname2[30], htitle2[40];
 char hname3[30], htitle3[40];
 char hname4[30], htitle4[40];
 char hname5[30], htitle5[40];
 const float lx_high[] = {3.2, 5.0, 5.5, 6.2, 0.85, 0.4};   
 const float ly_high[] = {6.0, 10., 5.6, 10.5, 3.4, 0.42};
 const float dx_high[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
 const float dy_high[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
 const float dz_high[] = {0.05, 0.06, 0.05, 0.06, 0.05, 0.05};
 
 heta  = new TH1F("primary_eta", "Primary eta", 120 , -3.0 , 3.0);
   
 for(int i=0; i<6; i++) {
  sprintf (htitle1,"Entryx-Exitx in %s", SysName[i]);
  sprintf (htitle2,"Entryy-Exity in %s", SysName[i]);
  sprintf (htitle3,"abs(Entryz-Exitz) in %s", SysName[i]);
  sprintf (htitle4,"Localx in %s", SysName[i]);
  sprintf (htitle5,"Localy in %s", SysName[i]);
  cout << htitle3 << endl;
  
  sprintf (hname1,"Entryx-Exitx_%i",i+1);
  sprintf (hname2,"Entryy-Exity_%i",i+1);
  sprintf (hname3,"abs(Entryz-Exitz)_%i",i+1);
  sprintf (hname4,"Localx_%i",i+1);
  sprintf (hname5,"Localy_%i",i+1);
  cout << "limit " << dz_high[i] << endl; 
  
  h1[i]  = new TH1F(hname1, htitle1, 10000 , -dx_high[i], dx_high[i]);
  h2[i]  = new TH1F(hname2, htitle2, 10000 , -dy_high[i], dy_high[i]);
  h3[i]  = new TH1F(hname3, htitle3, 10000 ,  0.,  dz_high[i]);
  h4[i]  = new TH1F(hname4, htitle4, 10000 , -lx_high[i], lx_high[i]);
  h5[i]  = new TH1F(hname5, htitle5, 10000 , -ly_high[i], ly_high[i]);
  
  h1[i]->SetXTitle("Entryx-Exitx (cm)");
  h2[i]->SetXTitle("Entryy-Exity (cm)");
  h3[i]->SetXTitle("abs(Entryz-Exitz) (cm)");
  h4[i]->SetXTitle("Localx (cm)");
  h5[i]->SetXTitle("Localy (cm)");
 } 
////// end booking histograms
 	

 for ( int j = 0; j <  entry; j++) {
        srcbrnch->GetEntry( j );
	
    // get G4Track info
    std::vector<PTrackerSimHit::Trk> G4Trk = TrkSimHits.getG4Trk();
    for (Int_t i = 0; i < G4Trk.size(); ++i) {
//      cout << " eta = " << G4Trk[i].eta << endl; 
      heta->Fill(G4Trk[i].eta);
    }
	
//	cout << "Number of hits " << nhit << endl;
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
//	cout << Hits[i]._sysID << " " << id << endl;
	h1[id]->Fill(Hits[i]._enx - Hits[i]._exx);	
	h2[id]->Fill(Hits[i]._eny - Hits[i]._exy);	
	h3[id]->Fill(fabs(Hits[i]._enz - Hits[i]._exz));
	h4[id]->Fill(Hits[i]._lpx);	
	h5[id]->Fill(Hits[i]._lpy);	
    }	
 } 
 char hnout[30];
 sprintf (hnout,"position_ref_%i.root",k);
 TFile* hout = new TFile(hnout,"RECREATE");
   heta->Write();
 for(int i=0; i<6; i++) {
   h1[i]->Write();
   h2[i]->Write();
   h3[i]->Write();
   h4[i]->Write();
   h5[i]->Write();
 }
 hout->Close();
}
