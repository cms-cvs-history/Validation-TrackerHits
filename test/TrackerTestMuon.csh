#! /bin/csh

setenv DATADIR $PWD/data
setenv ROOTFile simhitoutput.root
 
eval `scramv1 ru -csh`
SealPluginRefresh

 echo "===========> Validating Tracker Simhits with 15 GeV Muon eta ......."
 cp  ${DATADIR}/Muon.root   testinput.root
# cp /afs/cern.ch/cms/data/CMSSW/Validation/TrackerHits/data/Muon.root testinput.root
 cmsRun runP_tracker.cfg >& output.log
 /bin/rm output.log

 mv  simhitoutput.root SimHitMuon.root
 source Compare_muon_all.csh 

 if ( -e testinput.root ) /bin/rm testinput.root
  


echo "...Done..."
