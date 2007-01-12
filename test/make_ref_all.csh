#! /bin/csh

setenv DATADIR /afs/cern.ch/cms/data/CMSSW/Validation/TrackerHits/data
setenv ROOTFile SimHitMuon.root
 
eval `scramv1 ru -csh`

 echo "===========> Validating Tracker Simhits with 15 GeV Muon eta ......."
 cp  ${DATADIR}/Muon.root  .
 cmsRun TrackerHitValid.cfg
 root -b -p -q  macro/EnergyLoss_ref_all.C
 root -b -p -q  macro/Position_ref_all.C
 mv  simhitoutput.root SimHitMuon.root
 if ( -e Muon.root ) /bin/rm Muon.root

echo "...Done..."
