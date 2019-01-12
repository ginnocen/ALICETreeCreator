#!/bin/bash
#IMPORTANT: Before running one should have entered jAliEn environment:
#    alienv enter JAliEn/latest-jalien-jalien
#    jalien
#    <Enter Grid certificate password>
#    exit
#    ./downloadOutputTrain.sh $TRAINNAME $PLACETOSAVEOUTPUT $STAGEGRIDMERGING

#Arguments to this bash:
#   $1 is trainname (e.g. 297_20181120-2315_child_1)
#   $2 is path to place to save output (e.g. "" or ../ALICEanalysis/MLproductions/)
#   $3 is GRID merging Stage_X (e.g. "" for no merging, or Stage_1)
#
#To set in script (find with "#toset"):
#   OUTPUTPATH (output directory of train)
#   NFILES     (/*/ = download all files, /000*/ is 10 files, /00*/ is 100 files, etc)
#   OUTPUTFILE (name of file to download)

nfiles="/000*/" #toset Options: "0* "00*" "000*" #Assumed 1000 < jobs < 9999, if different, change number of zeros
outputfile=AnalysisResults #toset

if [ -z "$1" ]; then
  printf "Please enter train name: "
  read trainname
  printf "\n"
else
  trainname=$1
fi

dataset=$2
if [ -z "$2" ]; then
  printf "Please enter dataset name (LHC17pq, LHC18a4a2, ...): "
  read dataset
  printf "Warning: For now only the Devel_2 LEGO train is implemented. Please change the paths when needed\n\n"
fi

if [ "$dataset" == "LHC17pq" ]; then
  inputpathchild1=/alice/data/2017/LHC17p/000282341/pass1_FAST/PWGZZ/Devel_2
  inputpathchild2=/alice/data/2017/LHC17p/000282341/pass1_CENT_wSDD/PWGZZ/Devel_2
  inputpathchild3=/alice/data/2017/LHC17q/000282366/pass1_FAST/PWGZZ/Devel_2
  inputpathchild4=/alice/data/2017/LHC17q/000282366/pass1_CENT_wSDD/PWGZZ/Devel_2
  ninput=4
elif [ "$dataset" == "LHC18a4a2" ]; then
  inputpathchild1=/alice/sim/2018/LHC18a4a2_fast/282341/PWGZZ/Devel_2
  inputpathchild2=/alice/sim/2018/LHC18a4a2_fast/282366/PWGZZ/Devel_2
  inputpathchild3=/alice/sim/2018/LHC18a4a2_cent/282341/PWGZZ/Devel_2
  inputpathchild4=/alice/sim/2018/LHC18a4a2_cent/282366/PWGZZ/Devel_2
  ninput=4
else
  printf "Error: Dataset not yet implemented. Returning...\n\n"
  exit
fi

placetosave=$3
if [ -z "$3" ]; then
  printf "Warning: No output directory was entered, files will be saved in ./\n\n"
  placetosave=$(pwd)
else
  printf "Output will be saved in: $placetosave \n\n"
fi

stage=$4
if [ -z "$4" ]; then
  printf "Warning: No GRID merging stage was entered. Will download all files\n\n"
fi

mkdir -p -m 777 $placetosave/$trainname
if [ $? -ne 0 ]; then
  printf "Could not create output directory. Is $placetosave writable? Returning... \n\n"
  exit
else
   printf "Created directory: $placetosave/$trainname \n"
fi
mkdir -p -m 777 $placetosave/$trainname/$stage
if [ $? -ne 0 ]; then
  printf "Could not create output directory. Is $placetosave writable? Returning... \n\n"
  exit
else
  printf "Created $placetosave/$trainname/$stage \n\n"
fi


#----RUNNING THE DOWNLOADER----#
if [ $ninput -eq 4 ]; then
  sh ./downloader.sh $inputpathchild1 1 $nfiles $outputfile $placetosave $trainname $stage
  sh ./downloader.sh $inputpathchild2 2 $nfiles $outputfile $placetosave $trainname $stage
  sh ./downloader.sh $inputpathchild3 3 $nfiles $outputfile $placetosave $trainname $stage
  sh ./downloader.sh $inputpathchild4 4 $nfiles $outputfile $placetosave $trainname $stage
fi
