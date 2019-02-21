#!/bin/bash

START=$(date +%s)

myfile=$1
#isMC=$2             #to be added
doDplusFromEvt=$3   #to be added
doDsFromEvt=$4      #to be added
doDzeroFromEvt=$5   #to be added
doDstarFromEvt=$6   #to be added
doLcpKpiFromEvt=$7  #to be added
doLcpK0sFromEvt=$8
#doBplusFromEvt=$9   #to be added
#doPID=$10           #to be added


if [ $doLctopK0sFromEvt -eq 1 ]
then

DataTree="tree_Lc2V0bachelor;*"

g++ includeRemove/removeTree.C $(root-config --cflags --libs) -g -o removeTree.exe
while IFS='' read -r line || [[ -n "$line" ]]; do
   ./removeTree.exe "${line}" "$DataTree" &

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
echo "Removing LctopK0s = All done"
rm -rf removeTree.exe removeTree.exe.dSYM


while IFS='' read -r line || [[ -n "$line" ]]; do
   ./includeRemove/reduceSize.sh "${line%.*}_small.root" "${line}" &

  #1) Submit 50 jobs in parallel. 2) Reached 50? Wait till all are finished. 3) Delete big Keep submitting till again 50. 4) Go back to 2)
  #Not ideal solution, as the oldest one can get stuck/very big, so be there for a long long time. Worse situation, only this job is running at one point
  #Take job a bit further? The third one by '| head -3 | tail -1'
  if [ $(jobs -r | wc -l) -ge 50 ]; then
    wait $(jobs -r -p | head -3 | tail -1)
  fi
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Reducing size LctopK0s = All done"

fi


END=$(date +%s)
DIFF=$(( $END - $START ))
echo "Removing took $DIFF seconds"

