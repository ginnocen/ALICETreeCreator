#!/bin/bash

OUTPUTPATH=$1
CHILD=$2
NFILES=$3
OUTPUTFILE=$4
BASEDIR=$5
TRAINNAME=$6
STAGE=$7

SAVEDIR=$(printf "%s/%s/%s/child_%s" $BASEDIR $TRAINNAME $STAGE $CHILD)
mkdir -p -m 777 $SAVEDIR

OUTPUTPATH=$(printf "%s/%s_child_%s/%s" $OUTPUTPATH $TRAINNAME $CHILD $STAGE)
echo "Downloading LEGO train files from: $OUTPUTPATH\n"

cmd=$(printf "cp -T 32 %s/%s/%s.root file:%s/\n" $OUTPUTPATH "$NFILES" $OUTPUTFILE $SAVEDIR)

jalien << EOF
$cmd
exit
EOF

nameoutputlist=$(printf "listfilesMerging_%s%s.txt" $TRAINNAME $STAGE)
find $BASEDIR/$TRAINNAME/$STAGE/*/ -maxdepth 1 -not -type d> $nameoutputlist

mv $nameoutputlist $BASEDIR/$TRAINNAME/$STAGE/
