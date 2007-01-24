#!/bin/csh
echo "************************************************"
echo "***     Energy Loss Analysis       ***"
echo "************************************************"

eval `scramv1 ru -csh`

setenv MACRODIR $PWD/macro
setenv REFDIR $PWD/refHisto/muon
setenv ROOTFile_test SimHitMuon.root

setenv ROOTFile_ref  $REFDIR/eloss_ref.root
if ( ! -e plots ) mkdir plots

echo "************************************************"
echo running macro from directory $MACRODIR on file $ROOTFile_test
echo reference directory $REFDIR , file $ROOTFile_ref
echo "************************************************"
# KS eloss
echo "********** Start ROOT **********"
root -b -p -q $MACRODIR/EnergyLoss_compare_all.C\(1\)
if ( ! -e plots/muon ) mkdir plots/muon
/bin/mv eloss*.eps plots/muon
/bin/mv eloss*.gif plots/muon

# chi2 eloss
#root -b -p -q $MACRODIR/EnergyLoss_compare_all.C\(2\)
#if ( ! -e plots/muon ) mkdir plots/muon
#/bin/mv eloss*.eps plots/muon
#/bin/mv eloss*.gif plots/muon

setenv ROOTFile_ref  $REFDIR/position_ref.root

echo "************************************************"
echo running macro from directory $MACRODIR on file $ROOTFile_test
echo reference directory $REFDIR , file $ROOTFile_ref
echo "************************************************"
# KS position tests
  root -b -p -q $MACRODIR/Position_compare_all.C\(1\)
 if ( ! -e plots/muon ) mkdir plots/muon
 /bin/mv pos*.eps plots/muon
 /bin/mv pos*.gif plots/muon
 
# chi2 position tests
#  root -b -p -q $MACRODIR/Position_compare_all.C\(2\)
# if ( ! -e plots/muon ) mkdir plots/muon
# /bin/mv pos*.eps plots/muon
# /bin/mv pos*.gif plots/muon

 if ( ! -e plots/muon ) mkdir plots/muon
 /bin/mv summary*.eps plots/muon
 /bin/mv summary*.gif plots/muon

