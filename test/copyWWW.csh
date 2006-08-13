#! /bin/csh
setenv RELEASE 080pre4
setenv WWWDIR /afs/fnal.gov/files/home/room2/elis/public_html/Validation/$RELEASE

mkdir $WWWDIR
mkdir $WWWDIR/TK-strips
mkdir $WWWDIR/TK-strips/eloss
mkdir $WWWDIR/TK-strips/Localx
mkdir $WWWDIR/TK-strips/Localy
mkdir $WWWDIR/TK-strips/Entryx-Exitx
mkdir $WWWDIR/TK-strips/Entryy-Exity
mkdir $WWWDIR/TK-strips/Entryz-Exitz
mkdir $WWWDIR/TK-pixels
mkdir $WWWDIR/TK-pixels/eloss
mkdir $WWWDIR/TK-pixels/Localx
mkdir $WWWDIR/TK-pixels/Localy
mkdir $WWWDIR/TK-pixels/Entryx-Exitx
mkdir $WWWDIR/TK-pixels/Entryy-Exity
mkdir $WWWDIR/TK-pixels/Entryz-Exitz

echo "...Copying..."
cp plots/muon/eloss_KS*st* $WWWDIR/TK-strips/eloss
cp plots/muon/pos_Entryx-Exitx_KS*st* $WWWDIR/TK-strips/Entryx-Exitx
cp plots/muon/pos_Entryy-Exity_KS*st* $WWWDIR/TK-strips/Entryy-Exity
cp plots/muon/pos_Entryz-Exitz_KS*st* $WWWDIR/TK-strips/Entryz-Exitz
cp plots/muon/pos_Localy_KS*st* $WWWDIR/TK-strips/Localy
cp plots/muon/pos_Localx_KS*st* $WWWDIR/TK-strips/Localx

cp plots/muon/eloss_KS*px* $WWWDIR/TK-pixels/eloss
cp plots/muon/pos_Entryx-Exitx_KS*px* $WWWDIR/TK-pixels/Entryx-Exitx
cp plots/muon/pos_Entryy-Exity_KS*px* $WWWDIR/TK-pixels/Entryy-Exity
cp plots/muon/pos_Entryz-Exitz_KS*px* $WWWDIR/TK-pixels/Entryz-Exitz
cp plots/muon/pos_Localy_KS*px* $WWWDIR/TK-pixels/Localy
cp plots/muon/pos_Localx_KS*px* $WWWDIR/TK-pixels/Localx
echo "...Done..."
