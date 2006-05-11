#! /bin/csh

#setenv DATADIR /uscms_data/d1/elis/data4/CMSSW_0_6_0_pre3/src/IOMC/ParticleGuns/test
setenv DATADIR $PWD/data
setenv ROOTFile simhitoutput.root
 
eval `scramv1 ru -csh`
SealPluginRefresh

foreach i (1 2 3 4 5 6 7 8 9 10 11 12)

 echo "===========> Validating Tracker Simhits with 15 GeV Muon eta ......."
# cp  ${DATADIR}/Muon_$i.root   testinput.root
 cp /afs/cern.ch/cms/data/CMSSW/Validation/TrackerHits/data/Muon_$i.root testinput.root
 cmsRun runP_tracker.cfg >& output_$i.log
 /bin/rm output_$i.log

 mv  simhitoutput.root SimHitMuon_$i.root
 source Compare_muon.csh $i

 if ( -e testinput.root ) /bin/rm testinput.root
  
end


echo "...Done..."
