#!/bin/bash
#Arguments to this bash:
#   $1 is trainname (e.g. 297_20181120-2315_child_1)
#   $2 is path where input is saved (e.g. "" or ../ALICEanalysis/MLproductions/ -> Should be the as used for downloadOutputTrain.sh)
#   $3 is GRID merging Stage_X (e.g. "" for no merging, or Stage_1)
#   $4 is how many files to merge into one
#   $5 particle name (options: Dzero, Dplus, Ds, Dstar, Lc, Bplus)
#   $6 child

TRAINNAME=$1
BASEDIR=$2
CHILD=$3
nfilesformerging=$4
PARTICLE=$5
STAGE=$6

inputfile=$(printf "%s/%s/unmerged/child_%s/%s/listfiles_%s_child_%s%s.txt" $BASEDIR $TRAINNAME $CHILD $STAGE $TRAINNAME $CHILD $STAGE)
inputfileskim=$(printf "%s/%s/unmerged/child_%s/%s/listfiles_Skimmed%s_%s_child_%s%s.txt" $BASEDIR $TRAINNAME $CHILD $STAGE $PARTICLE $TRAINNAME $CHILD $STAGE)
nameoutput=$(printf "%s/%s/merged/child_%s/%s" $BASEDIR $TRAINNAME $CHILD $STAGE)
nameoutputlist=$(printf "listfiles_Merged%d%s_%s_child_%s%s.txt" $nfilesformerging $PARTICLE $TRAINNAME $CHILD $STAGE)
if [ -z "$STAGE" ]; then
  inputfile=$(printf "%s/%s/unmerged/child_%s/listfiles_%s_child_%s.txt" $BASEDIR $TRAINNAME $CHILD $TRAINNAME $CHILD)
  inputfileskim=$(printf "%s/%s/unmerged/child_%s/listfiles_Skimmed%s_%s_child_%s.txt" $BASEDIR $TRAINNAME $CHILD $PARTICLE $TRAINNAME $CHILD)
  nameoutput=$(printf "%s/%s/merged/child_%s" $BASEDIR $TRAINNAME $CHILD)
  nameoutputlist=$(printf "listfiles_Merged%d%s_%s_child_%s.txt" $nfilesformerging $PARTICLE $TRAINNAME $CHILD)
fi

printf "\n\n\nMerging output for particle: $PARTICLE \n"
printf "Reading $inputfileskim for files to merge\n"
printf "Merging with $nfilesformerging inputfiles\n"
printf "Saving merged output in directory: $nameoutput\n"
printf "Writing merged output in: $nameoutputlist\n"

cp "$inputfile" "$inputfileskim"

#Ubuntu:
sed -i -e "s|.root|_${PARTICLE}_skimmed.root|g" $inputfileskim
firstline=$(head -n 1 $inputfileskim)
skimmedname=${firstline##*/}
skimmedname=${skimmedname/.root/_}
mergedname="${skimmedname}Merged$nfilesformerging"

#macOS, I don't know why.. TO CHECK
#sed -i "" "s|.root|_${PARTICLE}_skimmed.root|g" $inputfileskim
#split -l $nfilesformerging $inputfileskim $nameoutput/merged_skimmedFile_
awk -v lines=$nfilesformerging -v outdir="$nameoutput/%04d" -v splitfile="$nameoutput/%04d/$mergedname" '{if((NR-1)%lines==0)system("mkdir -p \"" sprintf(outdir,1+int((NR-1)/lines)) "\""); print>sprintf(splitfile,1+int((NR-1)/lines))}' "$inputfileskim"

rm ${nameoutput}/*/${mergedname}.root
ls $nameoutput/*/$mergedname > $nameoutputlist

while IFS='' read -r line || [[ -n "$line" ]]; do
  echo $line
  hadd "${line}.root" @"$line" &
done < "$nameoutputlist"

## wait till all merging jobs are finished
wait

#Ubuntu:
sed -i -e "s|$|.root|g" $nameoutputlist
#macOS, I don't know why.. TO CHECK
#sed -i "" "s|$|_${PARTICLE}.root|g" $nameoutputlist
mv $nameoutputlist $nameoutput/
