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
#doBplusFromEvt=$9   #to be added
#doPID=$10           #to be added



if [ $doDplusFromEvt -eq 1 ]
then

DataTree="tree_Dplus"

g++ includeSkim/skimTreeDplusFromEvt.C $(root-config --cflags --libs) -g -o skimTreeDplusFromEvt.exe
while IFS='' read -r line || [[ -n "$line" ]]; do
   ./skimTreeDplusFromEvt.exe "${line}" "${line%.*}_Dplus_skimmed.root" "$DataTree" "$isMC" "$ispp" &
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
  ./skimTreeLcFromEvt.exe "${line}" "${line%.*}_Lc_skimmed.root" "$DataTree" "$isMC" "$ispp" &
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Lc = All done"
rm -rf skimTreeLcFromEvt.exe skimTreeLcFromEvt.exe.dSYM

fi



#if [ $doBplusFromEvt -eq 1 ]
#then
#
#DataTree="tree_Bplus"
#
#g++ includeSkim/skimTreeBplusFromEvt.C $(root-config --cflags --libs) -g -o skimTreeBplusFromEvt.exe
#while IFS='' read -r line || [[ -n "$line" ]]; do
#  ./skimTreeBplusFromEvt.exe "${line}" "${line%.*}_Bplus_skimmed.root" "$DataTree" &
#done < "$myfile" &
#
### would wait until those are completed
### before displaying all done message
#wait
#echo "Bplus = All done"
#rm -rf skimTreeBplusFromEvt.exe skimTreeBplusFromEvt.exe.dSYM
#
#fi



END=$(date +%s)
DIFF=$(( $END - $START ))
echo "Skimming took $DIFF seconds"
