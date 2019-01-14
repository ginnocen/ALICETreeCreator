#!/bin/bash
#Arguments to this bash:
#   $1 is trainname (e.g. 297_20181120-2315_child_1)
#   $2 is path where input is saved (e.g. "" or ../ALICEanalysis/MLproductions/ -> Should be the as used for downloadOutputTrain.sh)
#   $3 is GRID merging Stage_X (e.g. "" for no merging, or Stage_1)
#   $4 is how many files to merge into one
#   $5 particle name (options: Dzero, Dplus, Ds, Dstar, Lc, Bplus)

BASEDIR=$2
if [ -z "$BASEDIR" ]; then
  BASEDIR=$(pwd)
else
  BASEDIR=$(pwd)/$BASEDIR
fi
TRAINNAME=$1
STAGE=$3

inputfile=$(printf "%s/%s/%s/listfilesMerging_%s%s.txt" $BASEDIR $TRAINNAME $STAGE $TRAINNAME $STAGE)
if [ -z "$STAGE" ]; then
  inputfile=$(printf "%s/%s/listfilesMerging_%s.txt" $BASEDIR $TRAINNAME $TRAINNAME)
fi
printf "\nReading $inputfile for files to merge\n"

nfilesformerging=$4
if [ -z "$nfilesformerging" ]; then
  nfilesformerging=4
fi
printf "\nMerging with $nfilesformerging inputfiles\n"

PARTICLE=$5
printf "\nMerging output for particle: $PARTICLE \n"

nameoutput=$(printf "%s/%s/%s/mergeSkimOutputDir_%s" $BASEDIR $TRAINNAME "$STAGE" $PARTICLE)
nameoutput=${nameoutput}_${nfilesformerging}
printf "\nSaving merged output in directory: $nameoutput\n"
nameoutputlist=$(printf "lsOutputMergedList_%s%s.txt" $TRAINNAME $STAGE)
printf "\nWriting merged output in: $nameoutputlist\n"

rm -rf $nameoutput
mkdir $nameoutput
split -l $nfilesformerging $inputfile $nameoutput/merged-file
sed -i -e "s|.root|_${PARTICLE}_skimmed.root|g" $nameoutput/merged-file
ls $nameoutput/merged-file*> $nameoutputlist

while IFS='' read -r line || [[ -n "$line" ]]; do
echo $line
#sed 's/$/.root /g' "${line}" > "${line}_rootflag"
#mv "${line}_rootflag" "${line}"
hadd "${line}_${PARTICLE}.root" @"$line"
done < "$nameoutputlist"

mv $nameoutputlist $nameoutput/
