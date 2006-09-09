#!/bin/csh

eval `scramv1 ru -csh`

setenv MACRODIR $PWD/macro
setenv REFDIR $PWD/refHisto/muon
setenv ROOTFile_test SimHitMuon.root

setenv ROOTFile_ref  $REFDIR/position_ref.root

echo "************************************************"
echo running macro from directory $MACRODIR on file $ROOTFile_test
echo reference directory $REFDIR , file $ROOTFile_ref
echo "************************************************"
# KS position tests
  root -b -p -q $MACRODIR/Position_compare_all.C\(1\)
 if ( ! -e plots/muon ) mkdir plots/muon
 /bin/mv pos*.eps plots/muon
 
# chi2 position tests
#  root -b -p -q $MACRODIR/Position_compare_all.C\(2\)
# if ( ! -e plots/muon ) mkdir plots/muon
# /bin/mv pos*.eps plots/muon

 if ( ! -e plots/muon ) mkdir plots/muon
 /bin/mv pos*.eps     plots/muon
 /bin/mv summary*.eps plots/muon

