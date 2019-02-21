#!/bin/bash

START=$(date +%s)

myfile=$1
isMC=$2
ispp=$3
doDplusFromEvt=$4
doDsFromEvt=$5
doDzeroFromEvt=$6
doDstarFromEvt=$7
doLcFromEvt=$8
doLctopK0sFromEvt=$9



if [ $doDplusFromEvt -eq 1 ]
then

DataTree="tree_Dplus"

g++ includeSkim/skimTreeDplusFromEvt.C $(root-config --cflags --libs) -g -o skimTreeDplusFromEvt.exe
while IFS='' read -r line || [[ -n "$line" ]]; do
   ./skimTreeDplusFromEvt.exe "${line}" "${line%.*}_Dplus_skimmed.root" "$DataTree" "$isMC" "$ispp" &

  #1) Submit 50 jobs in parallel. 2) Reached 50? Wait till oldest one is finished. 3) Keep submitting till again 50. 4) Go back to 2)
  #Not ideal solution, as the oldest one can get stuck/very big, so be there for a long long time. Worse situation, only this job is running at one point
  #Take job a bit further? The third one by '| head -3 | tail -1'
  if [ $(jobs -r | wc -l) -ge 50 ]; then
    wait $(jobs -r -p | head -3 | tail -1)
  fi
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Dplus = All done"
rm -rf skimTreeDplusFromEvt.exe skimTreeDplusFromEvt.exe.dSYM

fi



if [ $doDsFromEvt -eq 1 ]
then

DataTree="tree_Ds"

g++ includeSkim/skimTreeDsFromEvt.C $(root-config --cflags --libs) -g -o skimTreeDsFromEvt.exe
while IFS='' read -r line || [[ -n "$line" ]]; do
  ./skimTreeDsFromEvt.exe "${line}" "${line%.*}_Ds_skimmed.root" "$DataTree" "$isMC" "$ispp" &

  #1) Submit 50 jobs in parallel. 2) Reached 50? Wait till oldest one is finished. 3) Keep submitting till again 50. 4) Go back to 2)
  #Not ideal solution, as the oldest one can get stuck/very big, so be there for a long long time. Worse situation, only this job is running at one point
  #Take job a bit further? The third one by '| head -3 | tail -1'
  if [ $(jobs -r | wc -l) -ge 50 ]; then
    wait $(jobs -r -p | head -3 | tail -1)
  fi
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Ds = All done"
rm -rf skimTreeDsFromEvt.exe skimTreeDsFromEvt.exe.dSYM

fi



if [ $doDzeroFromEvt -eq 1 ]
then

DataTree="tree_D0"

g++ includeSkim/skimTreeDzeroFromEvt.C $(root-config --cflags --libs) -g -o skimTreeDzeroFromEvt.exe
while IFS='' read -r line || [[ -n "$line" ]]; do
  ./skimTreeDzeroFromEvt.exe "${line}" "${line%.*}_Dzero_skimmed.root" "$DataTree" "$isMC" "$ispp" &

  #1) Submit 50 jobs in parallel. 2) Reached 50? Wait till oldest one is finished. 3) Keep submitting till again 50. 4) Go back to 2)
  #Not ideal solution, as the oldest one can get stuck/very big, so be there for a long long time. Worse situation, only this job is running at one point
  #Take job a bit further? The third one by '| head -3 | tail -1'
  if [ $(jobs -r | wc -l) -ge 50 ]; then
    wait $(jobs -r -p | head -3 | tail -1)
  fi
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Dzero = All done"
rm -rf skimTreeDzeroFromEvt.exe skimTreeDzeroFromEvt.exe.dSYM

fi



if [ $doDstarFromEvt -eq 1 ]
then

DataTree="tree_Dstar"

g++ includeSkim/skimTreeDstarFromEvt.C $(root-config --cflags --libs) -g -o skimTreeDstarFromEvt.exe
while IFS='' read -r line || [[ -n "$line" ]]; do
  ./skimTreeDstarFromEvt.exe "${line}" "${line%.*}_Dstar_skimmed.root" "$DataTree" "$isMC" "$ispp" &

  #1) Submit 50 jobs in parallel. 2) Reached 50? Wait till oldest one is finished. 3) Keep submitting till again 50. 4) Go back to 2)
  #Not ideal solution, as the oldest one can get stuck/very big, so be there for a long long time. Worse situation, only this job is running at one point
  #Take job a bit further? The third one by '| head -3 | tail -1'
  if [ $(jobs -r | wc -l) -ge 50 ]; then
    wait $(jobs -r -p | head -3 | tail -1)
  fi
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Dstar = All done"
rm -rf skimTreeDstarFromEvt.exe skimTreeDstarFromEvt.exe.dSYM

fi



if [ $doLcFromEvt -eq 1 ]
then

DataTree="tree_LctopKpi"

g++ includeSkim/skimTreeLcFromEvt.C $(root-config --cflags --libs) -g -o skimTreeLcFromEvt.exe
while IFS='' read -r line || [[ -n "$line" ]]; do
  ./skimTreeLcFromEvt.exe "${line}" "${line%.*}_LctopKpi_skimmed.root" "$DataTree" "$isMC" "$ispp" &

  #1) Submit 50 jobs in parallel. 2) Reached 50? Wait till oldest one is finished. 3) Keep submitting till again 50. 4) Go back to 2)
  #Not ideal solution, as the oldest one can get stuck/very big, so be there for a long long time. Worse situation, only this job is running at one point
  #Take job a bit further? The third one by '| head -3 | tail -1'
  if [ $(jobs -r | wc -l) -ge 50 ]; then
    wait $(jobs -r -p | head -3 | tail -1)
  fi
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Lc->pKpi = All done"
rm -rf skimTreeLcFromEvt.exe skimTreeLcFromEvt.exe.dSYM

fi



if [ $doLctopK0sFromEvt -eq 1 ]
then

DataTree="tree_Lc2V0bachelor"

g++ includeSkim/skimTreeLctopK0sFromEvt.C $(root-config --cflags --libs) -g -o skimTreeLctopK0sFromEvt.exe
while IFS='' read -r line || [[ -n "$line" ]]; do
  ./skimTreeLctopK0sFromEvt.exe "${line}" "${line%.*}_LctopK0s_skimmed.root" "$DataTree" "$isMC" "$ispp" &

  #1) Submit 50 jobs in parallel. 2) Reached 50? Wait till oldest one is finished. 3) Keep submitting till again 50. 4) Go back to 2)
  #Not ideal solution, as the oldest one can get stuck/very big, so be there for a long long time. Worse situation, only this job is running at one point
  #Take job a bit further? The third one by '| head -3 | tail -1'
  if [ $(jobs -r | wc -l) -ge 50 ]; then
    wait $(jobs -r -p | head -3 | tail -1)
  fi
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Lc->pK0s = All done"
rm -rf skimTreeLctopK0sFromEvt.exe skimTreeLctopK0sFromEvt.exe.dSYM

fi



END=$(date +%s)
DIFF=$(( $END - $START ))
echo "Skimming took $DIFF seconds"
