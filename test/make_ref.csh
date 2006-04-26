#! /bin/csh

setenv DATADIR /uscms_data/d1/elis/data4/CMSSW_0_6_0_pre3/src/IOMC/ParticleGuns/test
setenv ROOTFile simhitoutput.root
 
eval `scramv1 ru -csh`

foreach i (1 2 3 4 5 6 7 8 9 10 11 12)

 echo "===========> Validating Tracker Simhits with 15 GeV Muon eta ......."
 cp  ${DATADIR}/Muon_$i.root   testinput.root
 cmsRun runP_tracker.cfg
 root -b -p -q  macro/EnergyLoss_ref.C\($i\)
# cp  SimHitMuon_$i.root simhitoutput.root  
 root -b -p -q  macro/Position_ref.C\($i\)
 mv  simhitoutput.root   SimHitMuon_$i.root
 if ( -e testinput.root ) /bin/rm testinput.root

end


echo "...Done..."
