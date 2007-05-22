#! /bin/csh

setenv DATADIR  /afs/cern.ch/cms/data/CMSSW/Validation/TrackerHits/data
setenv ROOTFile SimHitMuon.root

eval `scramv1 ru -csh`

 echo "===========> Validating Tracker Simhits with 15 GeV Muon eta ......."
 cp  ${DATADIR}/Muon.root   .

 cmsRun  TrackerHitValid.cfg >& output.log
 /bin/rm output.log

 source Compare_muon_all.csh 

 if ( -e Muon.root ) /bin/rm Muon.root
  
echo "...Done..."
