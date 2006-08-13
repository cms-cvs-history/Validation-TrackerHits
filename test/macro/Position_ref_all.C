#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"

void Position_ref_all()
{	
/*=======================================================*/
/* This Macros is used to plot the Energy deposited      */
/* by a particle in different Tracker subsystems         */
/*=======================================================*/

 char filename[] ="$ROOTFile";     // The Name of Root File.
// char filename[] = "simhitoutput.root";     // The Name of Root File.
 char treename[] ="Events";            // The Name of Tree.
 TString brnchname = "PTrackerSimHit_thp_TrkHits_TrackerHits.obj";

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
 const char *SysName[] = {"TIB","TOB","TID","TEC","BPIX","FPIX"};
 const char *Region[] = {"005","051","115","152","225","253",
                       "-050","-105","-151","-215","-252","-325"};  
 
 TH1F *h1_tib[12],*h2_tib[12],*h3_tib[12],*h4_tib[12],*h5_tib[12];
 TH1F *h1_tob[12],*h2_tob[12],*h3_tob[12],*h4_tob[12],*h5_tob[12];
 TH1F *h1_tid[12],*h2_tid[12],*h3_tid[12],*h4_tid[12],*h5_tid[12];
 TH1F *h1_tec[12],*h2_tec[12],*h3_tec[12],*h4_tec[12],*h5_tec[12];
 TH1F *h1_bpix[12],*h2_bpix[12],*h3_bpix[12],*h4_bpix[12],*h5_bpix[12];
 TH1F *h1_fpix[12],*h2_fpix[12],*h3_fpix[12],*h4_fpix[12],*h5_fpix[12];
//TIB
 char hn1_tib[30], ht1_tib[40];
 char hn2_tib[30], ht2_tib[40];
 char hn3_tib[30], ht3_tib[40];
 char hn4_tib[30], ht4_tib[40];
 char hn5_tib[30], ht5_tib[40];
//tob
 char hn1_tob[30], ht1_tob[40];
 char hn2_tob[30], ht2_tob[40];
 char hn3_tob[30], ht3_tob[40];
 char hn4_tob[30], ht4_tob[40];
 char hn5_tob[30], ht5_tob[40];
//tid
 char hn1_tid[30], ht1_tid[40];
 char hn2_tid[30], ht2_tid[40];
 char hn3_tid[30], ht3_tid[40];
 char hn4_tid[30], ht4_tid[40];
 char hn5_tid[30], ht5_tid[40];
//tec
 char hn1_tec[30], ht1_tec[40];
 char hn2_tec[30], ht2_tec[40];
 char hn3_tec[30], ht3_tec[40];
 char hn4_tec[30], ht4_tec[40];
 char hn5_tec[30], ht5_tec[40];
//bpix
 char hn1_bpix[30], ht1_bpix[40];
 char hn2_bpix[30], ht2_bpix[40];
 char hn3_bpix[30], ht3_bpix[40];
 char hn4_bpix[30], ht4_bpix[40];
 char hn5_bpix[30], ht5_bpix[40];
//fpix
 char hn1_fpix[30], ht1_fpix[40];
 char hn2_fpix[30], ht2_fpix[40];
 char hn3_fpix[30], ht3_fpix[40];
 char hn4_fpix[30], ht4_fpix[40];
 char hn5_fpix[30], ht5_fpix[40];
 
 const float lx_high[] = {3.2, 5.0, 5.5, 6.2, 0.85, 0.4};   
 const float ly_high[] = {6.0, 10., 5.6, 10.5, 3.4, 0.42};
 const float dx_high[] = {0.03, 0.03, 0.02, 0.03, 0.03, 0.03};
 const float dy_high[] = {0.03, 0.03, 0.02, 0.03, 0.03, 0.03};
 const float dz_high[] = {0.05, 0.06, 0.05, 0.06, 0.05, 0.05};
   
 for(int i=0; i<12; i++) {
  sprintf (ht1_tib,"Entryx-Exitx in TIB %s", Region[i]);
  sprintf (ht1_tob,"Entryx-Exitx in TOB %s", Region[i]);
  sprintf (ht1_tid,"Entryx-Exitx in TID %s", Region[i]);
  sprintf (ht1_tec,"Entryx-Exitx in TEC %s", Region[i]);
  sprintf (ht1_bpix,"Entryx-Exitx in BPIX %s", Region[i]);
  sprintf (ht1_fpix,"Entryx-Exitx in FPIX %s", Region[i]);
    
  sprintf (ht2_tib,"Entryy-Exity in TIB %s", Region[i]);
  sprintf (ht2_tob,"Entryy-Exity in TOB %s", Region[i]);
  sprintf (ht2_tid,"Entryy-Exity in TID %s", Region[i]);
  sprintf (ht2_tec,"Entryy-Exity in TEC %s", Region[i]);
  sprintf (ht2_bpix,"Entryy-Exity in BPIX %s", Region[i]);
  sprintf (ht2_fpix,"Entryy-Exity in FPIX %s", Region[i]);
  
  sprintf (ht3_tib,"abs(Entryz-Exitz) in TIB %s", Region[i]);
  sprintf (ht3_tob,"abs(Entryz-Exitz) in TOB %s", Region[i]);
  sprintf (ht3_tid,"abs(Entryz-Exitz) in TID %s", Region[i]);
  sprintf (ht3_tec,"abs(Entryz-Exitz) in TEC %s", Region[i]);
  sprintf (ht3_bpix,"abs(Entryz-Exitz) in BPIX %s", Region[i]);
  sprintf (ht3_fpix,"abs(Entryz-Exitz) in FPIX %s", Region[i]);
  
  sprintf (ht4_tib,"Localx in TIB %s", Region[i]);
  sprintf (ht4_tob,"Localx in TOB %s", Region[i]);
  sprintf (ht4_tid,"Localx in TID %s", Region[i]);
  sprintf (ht4_tec,"Localx in TEC %s", Region[i]);
  sprintf (ht4_bpix,"Localx in BPIX %s", Region[i]);
  sprintf (ht4_fpix,"Localx in FPIX %s", Region[i]);
  
  sprintf (ht5_tib,"Localy in TIB %s", Region[i]);
  sprintf (ht5_tob,"Localy in TOB %s", Region[i]);
  sprintf (ht5_tid,"Localy in TID %s", Region[i]);
  sprintf (ht5_tec,"Localy in TEC %s", Region[i]);
  sprintf (ht5_bpix,"Localy in BPIX %s", Region[i]);
  sprintf (ht5_fpix,"Localy in FPIX %s", Region[i]);
    
  sprintf (hn1_tib,"Entryx-Exitx_TIB_%i",i+1);
  sprintf (hn1_tob,"Entryx-Exitx_TOB_%i",i+1);
  sprintf (hn1_tid,"Entryx-Exitx_TID_%i",i+1);
  sprintf (hn1_tec,"Entryx-Exitx_TEC_%i",i+1);
  sprintf (hn1_bpix,"Entryx-Exitx_BPIX_%i",i+1);
  sprintf (hn1_fpix,"Entryx-Exitx_FPIX_%i",i+1);
  
  sprintf (hn2_tib,"Entryy-Exity_TIB_%i",i+1);
  sprintf (hn2_tob,"Entryy-Exity_TOB_%i",i+1);
  sprintf (hn2_tid,"Entryy-Exity_TID_%i",i+1);
  sprintf (hn2_tec,"Entryy-Exity_TEC_%i",i+1);
  sprintf (hn2_bpix,"Entryy-Exity_BPIX_%i",i+1);
  sprintf (hn2_fpix,"Entryy-Exity_FPIX_%i",i+1);
  
  sprintf (hn3_tib,"abs(Entryz-Exitz)_TIB_%i",i+1);
  sprintf (hn3_tob,"abs(Entryz-Exitz)_TOB_%i",i+1);
  sprintf (hn3_tid,"abs(Entryz-Exitz)_TID_%i",i+1);
  sprintf (hn3_tec,"abs(Entryz-Exitz)_TEC_%i",i+1);
  sprintf (hn3_bpix,"abs(Entryz-Exitz)_BPIX_%i",i+1);
  sprintf (hn3_fpix,"abs(Entryz-Exitz)_FPIX_%i",i+1);
  
  sprintf (hn4_tib,"Localx_TIB_%i",i+1);
  sprintf (hn4_tob,"Localx_TOB_%i",i+1);
  sprintf (hn4_tid,"Localx_TID_%i",i+1);
  sprintf (hn4_tec,"Localx_TEC_%i",i+1);
  sprintf (hn4_bpix,"Localx_BPIX_%i",i+1);
  sprintf (hn4_fpix,"Localx_FPIX_%i",i+1);
  
  sprintf (hn5_tib,"Localy_TIB_%i",i+1);
  sprintf (hn5_tob,"Localy_TOB_%i",i+1);
  sprintf (hn5_tid,"Localy_TID_%i",i+1);
  sprintf (hn5_tec,"Localy_TEC_%i",i+1);
  sprintf (hn5_bpix,"Localy_BPIX_%i",i+1);
  sprintf (hn5_fpix,"Localy_FPIX_%i",i+1);
  
  h1_tib[i]  = new TH1F(hn1_tib, ht1_tib, 10000 , -dx_high[0], dx_high[0]);
  h1_tob[i]  = new TH1F(hn1_tob, ht1_tob, 10000 , -dx_high[1], dx_high[1]);
  h1_tid[i]  = new TH1F(hn1_tid, ht1_tid, 10000 , -dx_high[2], dx_high[2]);
  h1_tec[i]  = new TH1F(hn1_tec, ht1_tec, 10000 , -dx_high[3], dx_high[3]);
  h1_bpix[i]  = new TH1F(hn1_bpix, ht1_bpix, 10000 , -dx_high[4], dx_high[4]);
  h1_fpix[i]  = new TH1F(hn1_fpix, ht1_fpix, 10000 , -dx_high[5], dx_high[5]);

  h2_tib[i]  = new TH1F(hn2_tib, ht2_tib, 10000 , -dy_high[0], dy_high[0]);
  h2_tob[i]  = new TH1F(hn2_tob, ht2_tob, 10000 , -dy_high[1], dy_high[1]);
  h2_tid[i]  = new TH1F(hn2_tid, ht2_tid, 10000 , -dy_high[2], dy_high[2]);
  h2_tec[i]  = new TH1F(hn2_tec, ht2_tec, 10000 , -dy_high[3], dy_high[3]);
  h2_bpix[i]  = new TH1F(hn2_bpix, ht2_bpix, 10000 , -dy_high[4], dy_high[4]);
  h2_fpix[i]  = new TH1F(hn2_fpix, ht2_fpix, 10000 , -dy_high[5], dy_high[5]);

  h3_tib[i]  = new TH1F(hn3_tib, ht3_tib, 10000 , 0., dz_high[0]);
  h3_tob[i]  = new TH1F(hn3_tob, ht3_tob, 10000 , 0., dz_high[1]);
  h3_tid[i]  = new TH1F(hn3_tid, ht3_tid, 10000 , 0., dz_high[2]);
  h3_tec[i]  = new TH1F(hn3_tec, ht3_tec, 10000 , 0., dz_high[3]);
  h3_bpix[i]  = new TH1F(hn3_bpix, ht3_bpix, 10000 , 0., dz_high[4]);
  h3_fpix[i]  = new TH1F(hn3_fpix, ht3_fpix, 10000 , 0., dz_high[5]);

  h4_tib[i]  = new TH1F(hn4_tib, ht4_tib, 10000 , -lx_high[0], lx_high[0]);
  h4_tob[i]  = new TH1F(hn4_tob, ht4_tob, 10000 , -lx_high[1], lx_high[1]);
  h4_tid[i]  = new TH1F(hn4_tid, ht4_tid, 10000 , -lx_high[2], lx_high[2]);
  h4_tec[i]  = new TH1F(hn4_tec, ht4_tec, 10000 , -lx_high[3], lx_high[3]);
  h4_bpix[i]  = new TH1F(hn4_bpix, ht4_bpix, 10000 , -lx_high[4], lx_high[4]);
  h4_fpix[i]  = new TH1F(hn4_fpix, ht4_fpix, 10000 , -lx_high[5], lx_high[5]);

  h5_tib[i]  = new TH1F(hn5_tib, ht5_tib, 10000 , -ly_high[0], ly_high[0]);
  h5_tob[i]  = new TH1F(hn5_tob, ht5_tob, 10000 , -ly_high[1], ly_high[1]);
  h5_tid[i]  = new TH1F(hn5_tid, ht5_tid, 10000 , -ly_high[2], ly_high[2]);
  h5_tec[i]  = new TH1F(hn5_tec, ht5_tec, 10000 , -ly_high[3], ly_high[3]);
  h5_bpix[i]  = new TH1F(hn5_bpix, ht5_bpix, 10000 , -ly_high[4], ly_high[4]);
  h5_fpix[i]  = new TH1F(hn5_fpix, ht5_fpix, 10000 , -ly_high[5], ly_high[5]);

  h1_tib[i]->SetXTitle("Entryx-Exitx (cm)");
  h2_tib[i]->SetXTitle("Entryy-Exity (cm)");
  h3_tib[i]->SetXTitle("abs(Entryz-Exitz) (cm)");
  h4_tib[i]->SetXTitle("Localx (cm)");
  h5_tib[i]->SetXTitle("Localy (cm)");
  
  h1_tob[i]->SetXTitle("Entryx-Exitx (cm)");
  h2_tob[i]->SetXTitle("Entryy-Exity (cm)");
  h3_tob[i]->SetXTitle("abs(Entryz-Exitz) (cm)");
  h4_tob[i]->SetXTitle("Localx (cm)");
  h5_tob[i]->SetXTitle("Localy (cm)");
  
  h1_tid[i]->SetXTitle("Entryx-Exitx (cm)");
  h2_tid[i]->SetXTitle("Entryy-Exity (cm)");
  h3_tid[i]->SetXTitle("abs(Entryz-Exitz) (cm)");
  h4_tid[i]->SetXTitle("Localx (cm)");
  h5_tid[i]->SetXTitle("Localy (cm)");
  
  h1_tec[i]->SetXTitle("Entryx-Exitx (cm)");
  h2_tec[i]->SetXTitle("Entryy-Exity (cm)");
  h3_tec[i]->SetXTitle("abs(Entryz-Exitz) (cm)");
  h4_tec[i]->SetXTitle("Localx (cm)");
  h5_tec[i]->SetXTitle("Localy (cm)");
  
  h1_bpix[i]->SetXTitle("Entryx-Exitx (cm)");
  h2_bpix[i]->SetXTitle("Entryy-Exity (cm)");
  h3_bpix[i]->SetXTitle("abs(Entryz-Exitz) (cm)");
  h4_bpix[i]->SetXTitle("Localx (cm)");
  h5_bpix[i]->SetXTitle("Localy (cm)");
  
  h1_fpix[i]->SetXTitle("Entryx-Exitx (cm)");
  h2_fpix[i]->SetXTitle("Entryy-Exity (cm)");
  h3_fpix[i]->SetXTitle("abs(Entryz-Exitz) (cm)");
  h4_fpix[i]->SetXTitle("Localx (cm)");
  h5_fpix[i]->SetXTitle("Localy (cm)");
 } 
////// end booking histograms
 	

 for ( int j = 0; j <  entry; j++) {
        srcbrnch->GetEntry( j );
	
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
	if (id == 0) {
	 h1_tib[ir-1]->Fill(Hits[i]._enx - Hits[i]._exx);	
	 h2_tib[ir-1]->Fill(Hits[i]._eny - Hits[i]._exy);   
	 h3_tib[ir-1]->Fill(fabs(Hits[i]._enz - Hits[i]._exz));
	 h4_tib[ir-1]->Fill(Hits[i]._lpx);  
	 h5_tib[ir-1]->Fill(Hits[i]._lpy);  
        }
	if (id == 1) {
	 h1_tob[ir-1]->Fill(Hits[i]._enx - Hits[i]._exx);	
	 h2_tob[ir-1]->Fill(Hits[i]._eny - Hits[i]._exy);   
	 h3_tob[ir-1]->Fill(fabs(Hits[i]._enz - Hits[i]._exz));
	 h4_tob[ir-1]->Fill(Hits[i]._lpx);  
	 h5_tob[ir-1]->Fill(Hits[i]._lpy);  
        }
	if (id == 2) {
	 h1_tid[ir-1]->Fill(Hits[i]._enx - Hits[i]._exx);	
	 h2_tid[ir-1]->Fill(Hits[i]._eny - Hits[i]._exy);   
	 h3_tid[ir-1]->Fill(fabs(Hits[i]._enz - Hits[i]._exz));
	 h4_tid[ir-1]->Fill(Hits[i]._lpx);  
	 h5_tid[ir-1]->Fill(Hits[i]._lpy);  
        }
	if (id == 3) {
	 h1_tec[ir-1]->Fill(Hits[i]._enx - Hits[i]._exx);	
	 h2_tec[ir-1]->Fill(Hits[i]._eny - Hits[i]._exy);   
	 h3_tec[ir-1]->Fill(fabs(Hits[i]._enz - Hits[i]._exz));
	 h4_tec[ir-1]->Fill(Hits[i]._lpx);  
	 h5_tec[ir-1]->Fill(Hits[i]._lpy);  
        }
	if (id == 4) {
	 h1_bpix[ir-1]->Fill(Hits[i]._enx - Hits[i]._exx);	
	 h2_bpix[ir-1]->Fill(Hits[i]._eny - Hits[i]._exy);   
	 h3_bpix[ir-1]->Fill(fabs(Hits[i]._enz - Hits[i]._exz));
	 h4_bpix[ir-1]->Fill(Hits[i]._lpx);  
	 h5_bpix[ir-1]->Fill(Hits[i]._lpy);  
        }
	if (id == 5) {
	 h1_fpix[ir-1]->Fill(Hits[i]._enx - Hits[i]._exx);	
	 h2_fpix[ir-1]->Fill(Hits[i]._eny - Hits[i]._exy);   
	 h3_fpix[ir-1]->Fill(fabs(Hits[i]._enz - Hits[i]._exz));
	 h4_fpix[ir-1]->Fill(Hits[i]._lpx);  
	 h5_fpix[ir-1]->Fill(Hits[i]._lpy);  
        }
    }	
 } 
 
 TFile* hout = new TFile("position_ref.root","RECREATE");
 for(int i=0; i<12; i++) {
   h1_tib[i]->Write();
   h2_tib[i]->Write();
   h3_tib[i]->Write();
   h4_tib[i]->Write();
   h5_tib[i]->Write();
   h1_tob[i]->Write();
   h2_tob[i]->Write();
   h3_tob[i]->Write();
   h4_tob[i]->Write();
   h5_tob[i]->Write();
   h1_tid[i]->Write();
   h2_tid[i]->Write();
   h3_tid[i]->Write();
   h4_tid[i]->Write();
   h5_tid[i]->Write();
   h1_tec[i]->Write();
   h2_tec[i]->Write();
   h3_tec[i]->Write();
   h4_tec[i]->Write();
   h5_tec[i]->Write();
   h1_bpix[i]->Write();
   h2_bpix[i]->Write();
   h3_bpix[i]->Write();
   h4_bpix[i]->Write();
   h5_bpix[i]->Write();
   h1_fpix[i]->Write();
   h2_fpix[i]->Write();
   h3_fpix[i]->Write();
   h4_fpix[i]->Write();
   h5_fpix[i]->Write();
 }
 hout->Close();
}
