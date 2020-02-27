#!/bin/bash

DORUNDATA=1
index=0
LISTNAME=list.txt
rm $LISTNAME
sudo chmod -R 777  /home/vkucera/download/alice/data/2017/LHC17j/000274657/pass1/AOD208
ls -d /home/vkucera/download/alice/data/2017/LHC17j/000274657/pass1/AOD208/* > $LISTNAME

rm -rf folder_*
if [ $DORUNDATA -eq 1 ]; then
  while read F  ; do
  echo "$F"
  foldername="folder_$index"
  mkdir $foldername
  cd $foldername
  cp ../runAnalysis_treeCreator_localserver.C .
  root -q -l "runAnalysis_treeCreator_localserver.C(\"$F\")" &
  echo $index
  cd ..
  index=$((index+1))
  done <"$LISTNAME"

  wait
fi

rm AnalysisResults.root
hadd AnalysisResults.root folder_*/AnalysisResults.root
nevents=99999999999
root -q -l "debug.C($nevents)"
