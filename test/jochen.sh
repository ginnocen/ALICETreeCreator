#!/bin/bash

DORUNDATA=1
index=0
LISTNAME=list.txt
rm $LISTNAME
ls /data/TTree/D0DsLckINT7withJets/vAN-20200201_ROOT6-1/pp_2017_data/351_20200202-0239/unmerged/child_5/*/AnalysisResults.root > $LISTNAME

rm -rf folder_*
if [ $DORUNDATA -eq 1 ]; then
  while read F  ; do
  echo "$F"
  root -q -l "debug_.C(\"$F\")" &
  index=$((index+1))
  done <"$LISTNAME"

  wait
fi

rm histomerged.root
hadd histomerged.root histo*.root
