#!/bin/bash

OUTPUTPATH=$1
CHILD=$2
NFILES=$3
OUTPUTFILE=4
BASEDIR=$5
TRAINNAME=$6
STAGE=$7

OUTPUTPATH=$OUTPUTPATH/$TRAINNAME_child_$CHILD/$STAGE
echo "Downloading LEGO train files from: $OUTPUTPATH\n"

cmd=$(printf "cp -T 32 %s/%s/%s.root file:%s/%s/%s/\n" $OUTPUTPATH "$NFILES" $OUTPUTFILE $BASEDIR $TRAINNAME $STAGE)

jalien << EOF
$cmd
exit
EOF

nameoutputlist=$(printf "listfilesMerging_%s%s.txt" $TRAINNAME $STAGE)
find $BASEDIR/$TRAINNAME/$STAGE/*/ -maxdepth 1 -not -type d> $nameoutputlist

mv $nameoutputlist $BASEDIR/$TRAINNAME/$STAGE/
