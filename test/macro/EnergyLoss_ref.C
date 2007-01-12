#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"

void EnergyLoss_ref(int k)
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
 int ids[1000];
 int nhit;
 int nprim;
 
////// booking histograms
 const char *SysName[] = {"TIB","TOB","TID","TEC","BPIX","FPIX"};
 
 TH1F *eloss[6];
 TH1F *heta;
 char hname[10], htitle[40];  
  heta  = new TH1F("primary_eta", "Primary eta", 120 , -3.0 , 3.0);
 for(int i=0; i<6; i++) {
  sprintf (htitle,"Energy loss in %s", SysName[i]);
  sprintf (hname,"Eloss_%i",i+1);
  eloss[i]  = new TH1F(hname, htitle, 10000 , 0.0 , 0.001*E2NEL);
  eloss[i]->SetXTitle("Hit Energy (GeV)");
 } 
////// end booking histograms
  
 for (int ev=1; ev<=entry; ev++) {
   srcbrnch->GetEntry( ev );
   
    // get G4Track info
    std::vector<PTrackerSimHit::Trk> G4Trk = TrkSimHits.getG4Trk();
    for (Int_t i = 0; i < G4Trk.size(); ++i) {
//      cout << " eta = " << G4Trk[i].eta << endl; 
      heta->Fill(G4Trk[i].eta);
    }

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
	eloss[id]->Fill(Hits[i]._eloss*E2NEL);	
    }
} 	

 char hnout[30];
 sprintf (hnout,"eloss_ref_%i.root",k);
 TFile* hout = new TFile(hnout,"RECREATE");
 heta->Write();
 for(int i=0; i<6; i++) {
   eloss[i]->Write();
 }
 hout->Close();
}
