#!/bin/csh
echo "************************************************"
echo "***     Energy Loss Analysis       ***"
echo "************************************************"

setenv MACRODIR $PWD/macro
setenv REFDIR $PWD/refHisto/muon
setenv ROOTFile_test SimHitMuon_$i.root

setenv ROOTFile_ref  $REFDIR/eloss_ref_$1.root
if ( ! -e plots ) mkdir plots

echo "************************************************"
echo running macro from directory $MACRODIR on file $ROOTFile_test
echo reference directory $REFDIR , file $ROOTFile_ref
echo "************************************************"
# KS eloss
root -b -p -q $MACRODIR/EnergyLoss_compare.C\(1\)
if ( ! -e plots/muon ) mkdir plots/muon
/bin/mv eloss*.eps plots/muon
# chi2 eloss
root -b -p -q $MACRODIR/EnergyLoss_compare.C\(2\)
if ( ! -e plots/muon ) mkdir plots/muon
/bin/mv eloss*.eps plots/muon

setenv ROOTFile_ref  $REFDIR/position_ref_$1.root

echo "************************************************"
echo running macro from directory $MACRODIR on file $ROOTFile_test
echo reference directory $REFDIR , file $ROOTFile_ref
echo "************************************************"
# KS position tests
foreach i ("Entryx-Exitx" "Entryy-Exity" "Entryz-Exitz" "Localx" "Localy")
 if ($i == "Entryx-Exitx") then 
  root -b -p -q $MACRODIR/Position_compare.C\(1,1\)
 else if ($i == "Entryy-Exity") then 
  root -b -p -q $MACRODIR/Position_compare.C\(2,1\)
 else if ($i == "Entryz-Exitz") then 
  root -b -p -q $MACRODIR/Position_compare.C\(3,1\)
 else if ($i == "Localx") then 
  root -b -p -q $MACRODIR/Position_compare.C\(4,1\)
 else if ($i == "Localy") then 
  root -b -p -q $MACRODIR/Position_compare.C\(5,1\)
 else 
  echo ------------->>> Test is not available!
 endif
 if ( ! -e plots/muon ) mkdir plots/muon
 /bin/mv pos*.eps plots/muon
end

# chi2 position tests
foreach i ("Entryx-Exitx" "Entryy-Exity" "Entryz-Exitz" "Localx" "Localy")
 if ($i == "Entryx-Exitx") then 
  root -b -p -q $MACRODIR/Position_compare.C\(1,2\)
 else if ($i == "Entryy-Exity") then 
  root -b -p -q $MACRODIR/Position_compare.C\(2,2\)
 else if ($i == "Entryz-Exitz") then 
  root -b -p -q $MACRODIR/Position_compare.C\(3,2\)
 else if ($i == "Localx") then 
  root -b -p -q $MACRODIR/Position_compare.C\(4,2\)
 else if ($i == "Localy") then 
  root -b -p -q $MACRODIR/Position_compare.C\(5,2\)
 else 
  echo ------------->>> Test is not available!
 endif
 if ( ! -e plots/muon ) mkdir plots/muon
 /bin/mv pos*.eps plots/muon
end

