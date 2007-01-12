#! /bin/csh
setenv DATADIR /afs/cern.ch/cms/data/CMSSW/Validation/TrackerHits/data
setenv ROOTFile SimHitMuon.root

eval `scramv1 ru -csh`
SealPluginRefresh

foreach i (1 2 3 4 5 6 7 8 9 10 11 12)

 echo "===========> Validating Tracker Simhits with 15 GeV Muon eta ......."
 cp  ${DATADIR}/Muon_$i.root   Muon.root
 cmsRun TrackerHitValid.cfg >& output_$i.log
 /bin/rm output_$i.log

 mv  SimHitMuon.root SimHitMuon_$i.root
 source Compare_muon.csh $i

 if ( -e Muon.root ) /bin/rm Muon.root
  
end


echo "...Done..."
