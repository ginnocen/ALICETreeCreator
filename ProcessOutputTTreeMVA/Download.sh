#!/bin/bash
#IMPORTANT: Before running one should have entered jAliEn environment:
#    alienv enter JAliEn/latest-jalien-jalien
#    jalien
#    <Enter Grid certificate password>
#    exit
#
#Arguments to this bash:
#   $1 is trainname (e.g. 297_20181120-2315_child_1)
#   $2 is dataset (e.g. for pp5TeV LHC17pq or LHC18a4a2)
#   $3 is path to place to save output (e.g. "" or ../ALICEanalysis/MLproductions/)
#   $4 is GRID merging Stage_X (e.g. "" for no merging, or Stage_1)
#
#To set in script (find with "#toset"):
#   NFILES     (/*/ = download all files, /000*/ is 10 files, /00*/ is 100 files, etc)
#   OUTPUTFILE (name of file to download)
#   What mesons to skim
#   Number of files to merge

printf "\n\n\n\e[1m----RUNNING THE DOWNLOADER----\e[0m\n\n"



#----THINGS TO SET----#
nfiles="/000*/" #toset   For testing: "0*", "00*", or "000*" (Assuming 1000 < jobs < 9999)
outputfile="AnalysisResults" #toset


printf "\e[1mYou set the following setters in the script. Please check them carefully before continuing.\e[0m\n"
printf "   Number of files to download from grid: \e[1m$nfiles\e[0m\n"
printf "   Outputfile to be downloaded from grid: \e[1m$outputfile.root\e[0m\n"
if [ -z "$4" ]; then
  printf "   You didn't provide the GRID merging stage as argument. I will download \e[1mnon-merged files\e[0m from GRID\n"
fi

printf "\n\e[1m   Are you okay with these settings [y/n]: \e[0m"
read answer
if [ "$answer" == "y" ]; then
  printf "   Thanks for confirming. Continuing...\n\n"
elif [ "$answer" == "Y" ]; then
  printf "   Thanks for confirming. Continuing...\n\n"
else
  printf "   \e[1;31mERROR: Please correct in script. \e[0m\n\n"
  exit
fi

#----INITIALIZING----#
if [ -z "$1" ]; then
  printf "Please enter train name: "
  read trainname
  printf "  Will download \e[1m$outputfile.root\e[0m output from train: \e[1m$trainname\e[0m \n"
else
  trainname=$1
  printf "Will download \e[1m$outputfile.root\e[0m output from train: \e[1m$trainname\e[0m \n"
fi

if [ -z "$2" ]; then
  printf "\nPlease enter dataset name (LHC17pq, LHC18a4a2, ...): "
  read dataset
  printf "  Chosen dataset: \e[1m$dataset\e[0m\n"
#  printf "  \e[0;31mWarning: For now only the Devel_2 LEGO train is implemented.\e[0m\n"
else
  dataset=$2
  printf "\nChosen dataset: \e[1m$dataset\e[0m\n"
fi

if [ "$dataset" == "LHC17pq" ]; then
  inputpathchild1=/alice/data/2017/LHC17p/000282341/pass1_FAST/PWGZZ/Devel_2
  inputpathchild2=/alice/data/2017/LHC17p/000282341/pass1_CENT_wSDD/PWGZZ/Devel_2
  inputpathchild3=/alice/data/2017/LHC17q/000282366/pass1_FAST/PWGZZ/Devel_2
  inputpathchild4=/alice/data/2017/LHC17q/000282366/pass1_CENT_wSDD/PWGZZ/Devel_2
  ninput=4
  isMC=0
  ispp=1
elif [ "$dataset" == "LHC18a4a2" ]; then
  inputpathchild1=/alice/sim/2018/LHC18a4a2_fast/282341/PWGZZ/Devel_2
  inputpathchild2=/alice/sim/2018/LHC18a4a2_fast/282366/PWGZZ/Devel_2
  inputpathchild3=/alice/sim/2018/LHC18a4a2_cent/282341/PWGZZ/Devel_2
  inputpathchild4=/alice/sim/2018/LHC18a4a2_cent/282366/PWGZZ/Devel_2
  ninput=4
  isMC=1
  ispp=1
else
  printf "\e[1;31mError: Dataset not yet implemented. Returning...\e[0m\n\n"
  exit
fi

if [ -z "$3" ]; then
printf "\n\e[0;31mWarning: No output directory was given as argument. Files will be saved in \"./\".\e[0m\n  \e[1mWas this intended? [y/n]:\e[0m "
  read answer
  if [ "$answer" == "y" ]; then
    placetosave=$(pwd)
  elif [ "$answer" == "Y" ]; then
    placetosave=$(pwd)
  else
    printf "  Please enter output directory: "
    read placetosave
  fi
  printf "  Output will be saved in: \e[1m$placetosave\e[0m \n"
else
  placetosave=$3
  printf "\nOutput will be saved in: \e[1m$placetosave\e[0m \n"
fi

if [ -z "$4" ]; then
  printf "\n\e[0;31mWarning: No GRID merging stage was entered. I will download non-merged files\e[0m\n"
else
  stage=$4
  printf "\nI will download files from GRID merging: \e[1m$stage\e[0m    (if not in format Stage_#, download will fail)\n"
fi

datestamp="$(date +"%d-%m-%Y")"
mkdir -p -m 777 $placetosave/$datestamp
  if [ $? -ne 0 ]; then
    printf "\n\e[1;31mError: Could not create output directory. Is $placetosave writable? Returning... \e[0m\n\n"
  exit
else
  printf "\nCreated directory: \e[1m$placetosave/$datestamp\e[0m \n"
fi
placetosave=$placetosave/$datestamp
mkdir -p -m 777 $placetosave/$trainname
if [ $? -ne 0 ]; then
  printf "\n\e[1;31mError: Could not create output directory. Is $placetosave writable? Returning... \e[0m\n\n"
  exit
else
   printf "Created directory: \e[1m$placetosave/$trainname\e[0m \n"
fi

timestamp="$(date +"%H-%M-%S")"
if [ -z "$4" ]; then
  stdoutputfile=$(printf "D_%s_stdout_%s-%s.txt" $trainname $datestamp $timestamp)
  stderrorfile=$(printf "D_%s_stderr_%s-%s.txt" $trainname $datestamp $timestamp)
else
  stdoutputfile=$(printf "D_%s_%s_stdout_%s-%s.txt" $trainname $stage $datestamp $timestamp)
  stderrorfile=$(printf "D_%s_%s_stderr_%s-%s.txt" $trainname $stage $datestamp $timestamp)
fi



#----RUNNING THE DOWNLOADER----#
printf "\n\n\e[1m----RUNNING THE DOWNLOADER----\e[0m\n\n"
printf "  Output of downloaders stored in:            \e[1m%s\e[0m\n  Warnings/Errors of downloader stored in:    \e[1m%s\e[0m\n" $i $stdoutputfile $stderrorfile
rundownloader="sh ./downloader.sh"

printf "\n\n\n\nOutput downloading starts here\n\n" > "$stdoutputfile"
printf "\n\n\n\nErrors downloading starts here\n\n" > "$stderrorfile"

if [ $ninput -eq 1 ]; then
  sh ./run_downloader $rundownloader $inputpathchild1 1 "$nfiles" $outputfile $placetosave $trainname $stage >> "$stdoutputfile" 2>> "$stderrorfile"
elif [ $ninput -eq 2 ]; then
  sh ./run_downloader $rundownloader $inputpathchild1 1 "$nfiles" $outputfile $placetosave $trainname $stage >> "$stdoutputfile" 2>> "$stderrorfile"
  sh ./run_downloader $rundownloader $inputpathchild2 2 "$nfiles" $outputfile $placetosave $trainname $stage >> "$stdoutputfile" 2>> "$stderrorfile"
elif [ $ninput -eq 3 ]; then
  sh ./run_downloader $rundownloader $inputpathchild1 1 "$nfiles" $outputfile $placetosave $trainname $stage >> "$stdoutputfile" 2>> "$stderrorfile"
  sh ./run_downloader $rundownloader $inputpathchild2 2 "$nfiles" $outputfile $placetosave $trainname $stage >> "$stdoutputfile" 2>> "$stderrorfile"
  sh ./run_downloader $rundownloader $inputpathchild3 3 "$nfiles" $outputfile $placetosave $trainname $stage >> "$stdoutputfile" 2>> "$stderrorfile"
elif [ $ninput -eq 4 ]; then
  sh ./run_downloader $rundownloader $inputpathchild1 1 "$nfiles" $outputfile $placetosave $trainname $stage >> "$stdoutputfile" 2>> "$stderrorfile"
  sh ./run_downloader $rundownloader $inputpathchild2 2 "$nfiles" $outputfile $placetosave $trainname $stage >> "$stdoutputfile" 2>> "$stderrorfile"
  sh ./run_downloader $rundownloader $inputpathchild3 3 "$nfiles" $outputfile $placetosave $trainname $stage >> "$stdoutputfile" 2>> "$stderrorfile"
  sh ./run_downloader $rundownloader $inputpathchild4 4 "$nfiles" $outputfile $placetosave $trainname $stage >> "$stdoutputfile" 2>> "$stderrorfile"
else
  printf "ERROR: More than 4 childs not yet supported, please implement. Returning..."
  exit
fi

if grep -q "jalien\|command not found" "$stderrorfile"
then
  printf "\e[1;31m  Warning: The 'jalien' command was not found, so no new files where downloaded. Did you already connect to JAliEn? Check log if this was not intended!\e[0m\n\n"
fi

mv $stdoutputfile $placetosave/$trainname/
mv $stderrorfile $placetosave/$trainname/

printf "\n\e[1mMoved log files to $placetosave/$trainname/\e[0m\n"
printf "\n\e[1m----DOWNLOADER FINISHED----\e[0m\n\n"
