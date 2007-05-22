#! /bin/csh

setenv ROOTFile SimHitMuon.root
 
 echo "===========> Validating Tracker Simhits with 15 GeV Muon eta ......."
 root -b -p -q  macro/EnergyLoss_ref_all.C
 root -b -p -q  macro/Position_ref_all.C

echo "...Done..."
