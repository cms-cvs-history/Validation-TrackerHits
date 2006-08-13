#! /bin/csh

setenv DATADIR /uscms_data/d1/elis/data4/CMSSW_0_9_0_pre3/src/Validation/TrackerHits/test/data
setenv ROOTFile simhitoutput.root
 
eval `scramv1 ru -csh`

 echo "===========> Validating Tracker Simhits with 15 GeV Muon eta ......."
 cp  ${DATADIR}/Muon.root   testinput.root
 cmsRun runP_tracker.cfg
# cp  SimHitMuon.root simhitoutput.root
 root -b -p -q  macro/EnergyLoss_ref_all.C
# cp  SimHitMuon.root simhitoutput.root  
 root -b -p -q  macro/Position_ref_all.C
 mv  simhitoutput.root SimHitMuon.root
 if ( -e testinput.root ) /bin/rm testinput.root



echo "...Done..."
