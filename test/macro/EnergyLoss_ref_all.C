#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"

void EnergyLoss_ref_all()
{	
/*================================================================*/
/* This Macros is used to produce reference plots of the          */
/* Energy deposited by a particle in different Tracker subsystems */
/*================================================================*/
// const float E2NEL = 2.778E+8;
 const float E2NEL = 1.;
 
  // setup names
 char filename[] ="$ROOTFile";     // The Name of Root File.
// char filename[] ="SimHitMuon_0-0.5.root";     // The Name of Root File.
 char treename[] ="Events";            // The Name of Tree.
 TString brnchname = "PTrackerSimHit_trackerHitsValid_TrkHits_TrackerValidation.obj";

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

 // set access to branch as proper object
 PTrackerSimHit TrkSimHits;
 srcbrnch->SetAddress(&TrkSimHits);
 
 cout << "Setting branch address" << endl;

 float de[1000];
 int ids[100000];
 int nhit;
 int nprim;
 
////// booking histograms
 const char *SysName[] = {"TIB","TOB","TID","TEC","BPIX","FPIX"};
 const char *Region[] = {"005","051","115","152","225","253",
                       "-050","-105","-151","-215","-252","-325"};  

 TH1F *h1[12],*h2[12],*h3[12],*h4[12],*h5[12], *h6[12];
 char hname1[30], htitle1[40];
 char hname2[30], htitle2[40];
 char hname3[30], htitle3[40];
 char hname4[30], htitle4[40];
 char hname5[30], htitle5[40];
 char hname6[30], htitle6[40];

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
  
 for (int ev=1; ev<=entry; ev++) {
   srcbrnch->GetEntry( ev );
   int ir = 0;   
    // get G4Track info
   std::vector<PTrackerSimHit::Trk> G4Trk = TrkSimHits.getG4Trk();
   if (G4Trk[0].eta>0.0 && G4Trk[0].eta<=0.5) ir = 1;
   if (G4Trk[0].eta>0.5 && G4Trk[0].eta<=1.0) ir = 2;
   if (G4Trk[0].eta>1.0 && G4Trk[0].eta<=1.5) ir = 3;
   if (G4Trk[0].eta>1.5 && G4Trk[0].eta<=2.0) ir = 4;
   if (G4Trk[0].eta>2.0 && G4Trk[0].eta<=2.5) ir = 5;
   if (G4Trk[0].eta>2.5 ) ir = 6;
   if (G4Trk[0].eta>-0.5 && G4Trk[0].eta<=0.0) ir = 7;
   if (G4Trk[0].eta>-1.0 && G4Trk[0].eta<=-0.5) ir = 8;
   if (G4Trk[0].eta>-1.5 && G4Trk[0].eta<=-1.0) ir = 9;
   if (G4Trk[0].eta>-2.0 && G4Trk[0].eta<=-1.5) ir = 10;
   if (G4Trk[0].eta>-2.5 && G4Trk[0].eta<=-2.0) ir = 11;
   if (G4Trk[0].eta<=-2.5) ir = 12;
//   cout << " eta = " << G4Trk[0].eta << " ir " << ir << endl; 
    
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

 char hnout[30];
 TFile* hout = new TFile("eloss_ref.root","RECREATE");
 for(int i=0; i<12; i++) {
   h1[i]->Write();
   h2[i]->Write();
   h3[i]->Write();
   h4[i]->Write();
   h5[i]->Write();
   h6[i]->Write();
 }
 hout->Close();
}
