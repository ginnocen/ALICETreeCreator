#!/bin/bash
#Arguments to this bash:
#   $1 is trainname (e.g. 297_20181120-2315_child_1)
#   $2 is dataset (e.g. for pp5TeV LHC17pq or LHC18a4a2)
#   $3 is path to place to save output (e.g. "" or ../ALICEanalysis/MLproductions/)
#   $4 is GRID merging Stage_X (e.g. "" for no merging, or Stage_1)
#
#To set in script (find with "#toset"):
#   OUTPUTFILE (name of file to download)
#   What mesons to skim

printf "\n\n\n\e[1m----RUNNING THE SKIMMER DIRECTLY FROM GRID----\e[0m\n\n"



#----THINGS TO SET----#
outputfile="AnalysisResults" #toset
nfiles="/000*/" #toset   For testing: "0*", "00*", or "000*" (Assuming 1000 < jobs < 9999)

doDplus=0       #toset (skimmers)
doDs=0          #toset (skimmers)
doDzero=1       #toset (skimmers)
doDstar=0       #toset (skimmers)
doLcpKpi=0      #toset (skimmers)
doLcpK0s=0      #toset (skimmers)

#Confirm with user if hardcoded values are what he/she wants
printf "\e[1mYou set the following setters in the script. Please check them carefully before continuing.\e[0m\n"
printf "   Outputfile to be downloaded from grid: \e[1m$outputfile.root\e[0m\n"
printf "   Number of files to download from grid: \e[1m$nfiles\e[0m\n"
printf "   Particles that are enabled: Dplus \e[1m(%s)\e[0m, Ds \e[1m(%s)\e[0m, Dzero \e[1m(%s)\e[0m, Dstar \e[1m(%s)\e[0m, Lc->pKpi \e[1m(%s)\e[0m, Lc->pK0s \e[1m(%s)\e[0m\n" $doDplus $doDs $doDzero $doDstar $doLcpKpi $doLcpK0s
if [ -z "$4" ]; then
  printf "   You didn't provide the GRID merging stage as argument. I will look for \e[1mnon-merged files\e[0m from GRID\n"
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
#When arguments are not given, user should provide them while macro is started
#Checking argument 1, trainname
if [ -z "$1" ]; then
  printf "Please enter train name: "
  read trainname
  printf "  Will look for the downloaded output from train: \e[1m$trainname\e[0m \n"
else
  trainname=$1
  printf "Will look for the downloaded output from train: \e[1m$trainname\e[0m \n"
fi

#Checking argument 2, dataset
if [ -z "$2" ]; then
  printf "\nPlease enter dataset name (LHC17pq, LHC18a4a2, ...): "
  read dataset
  printf "  Chosen dataset: \e[1m$dataset\e[0m\n"
#  printf "  \e[0;31mWarning: For now only the Devel_2 LEGO train is implemented.\e[0m\n"
else
  dataset=$2
  printf "\nChosen dataset: \e[1m$dataset\e[0m\n"
fi

#Hardcoded information about dataset
if [ "$dataset" == "LHC17pq" ]; then
  inputpathchild[0]=alien:///alice/data/2017/LHC17p/000282341/pass1_FAST/PWGZZ/Devel_2
  inputpathchild[1]=alien:///alice/data/2017/LHC17p/000282341/pass1_CENT_wSDD/PWGZZ/Devel_2
  inputpathchild[2]=alien:///alice/data/2017/LHC17q/000282366/pass1_FAST/PWGZZ/Devel_2
  inputpathchild[3]=alien:///alice/data/2017/LHC17q/000282366/pass1_CENT_wSDD/PWGZZ/Devel_2
  ninput=4
  isMC=0
  ispp=1
  datasetwithchilds=1
elif [ "$dataset" == "LHC18a4a2" ]; then
  inputpathchild[0]=alien:///alice/sim/2018/LHC18a4a2_fast/282341/PWGZZ/Devel_2
  inputpathchild[1]=alien:///alice/sim/2018/LHC18a4a2_fast/282366/PWGZZ/Devel_2
  inputpathchild[2]=alien:///alice/sim/2018/LHC18a4a2_cent/282341/PWGZZ/Devel_2
  inputpathchild[3]=alien:///alice/sim/2018/LHC18a4a2_cent/282366/PWGZZ/Devel_2
  ninput=4
  isMC=1
  ispp=1
  datasetwithchilds=1
elif [ "$dataset" == "LHC16i2a" ]; then
  inputpathchild[0]=alien:///alice/sim/2016/LHC16i2a/246087/AOD198/PWGZZ/Devel_2
  ninput=1
  isMC=1
  ispp=0
  datasetwithchilds=0
elif [ "$dataset" == "LHC18r" ]; then
  inputpathchild[0]=alien:///alice/data/2018/LHC18r/000296932/pass1/PWGZZ/Devel_2
  ninput=1
  isMC=0
  ispp=0
  datasetwithchilds=0
elif [ "$dataset" == "LHC18r_test" ]; then
  inputpathchild[0]=alien:///alice/data/2018/LHC18r/000296787/pass1/PWGZZ/Devel_2
  ninput=1
  isMC=0
  ispp=0
  datasetwithchilds=0
else
  printf "\e[1;31mError: Dataset not yet implemented. Returning...\e[0m\n\n"
  exit
fi

#Checking argument 3, output directoy
if [ -z "$3" ]; then
  printf "\n\e[0;31mWarning: No output directory was given as argument. I will be working from \"./\".\e[0m\n  \e[1mWas this intended? [y/n]:\e[0m "
  read answer
  if [ "$answer" == "y" ]; then
    placetosave=$(pwd)
  elif [ "$answer" == "Y" ]; then
    placetosave=$(pwd)
  else
    printf "  Please enter output directory: "
    read placetosave
  fi
  printf "  Will be working from: \e[1m$placetosave\e[0m \n"
else
  placetosave=$3
  printf "\nWill be working from: \e[1m$placetosave\e[0m \n"
fi

#Checking argument 4, GRID merging stage
if [ -z "$4" ]; then
  printf "\n\e[0;31mWarning: No GRID merging stage was entered. I will look for non-merged files\e[0m\n"
else
  stage=$4
  printf "\nI will look for files from GRID merging: \e[1m$stage\e[0m    (if not in format Stage_#, this will fail)\n"
fi

#Creating output directory, and checking if user has writing permission. + dir with datestamp, so it is easier to see which production should be used/can be deleted
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
mkdir -p -m 777 $placetosave/$trainname/unmerged
if [ $? -ne 0 ]; then
  printf "\n\e[1;31mError: Could not create output directory. Is $placetosave/$trainname writable? Returning... \e[0m\n\n"
  exit
else
   printf "Created directory: \e[1m$placetosave/$trainname/unmerged\e[0m \n"
fi
for ((i=$ninput; i>=1; i--))
do
  mkdir -p -m 777 $placetosave/$trainname/unmerged/child_$i
  if [ $? -ne 0 ]; then
    printf "\n\e[1;31mError: Could not create output directory. Is $placetosave/$trainname/merged writable? Returning... \e[0m\n\n"
    exit
  else
    printf "Created directory: \e[1m$placetosave/$trainname/merged/child_$i\e[0m \n"
  fi
done


###TODO. THIS PART DOESN'T WORK YET. WE NEED A WAY TO PRINT ALL PRODUCED TRAIN FILES IN A .TXT FILES

#for ((i=$ninput; i>=1; i--))
#do
#  if [ $datasetwithchilds -eq 1 ]; then
#    DWNLDOUTPUTPATH=$(printf "%s/%s_child_%s/%s" ${inputpathchild[i-1]} $trainname $i $stage)
#  else
#    DWNLDOUTPUTPATH=$(printf "%s/%s/%s" ${inputpathchild[i-1]} $trainname $stage)
#  fi
#  printf "Reading LEGO train files from: %s\n" $DWNLDOUTPUTPATH
#
#  cmd1=$(printf "find %s %s > listfiles_%s_child_%s%s.txt\n" $DWNLDOUTPUTPATH $outputfile $trainname $i $stage)
#  if [ -z "$stage" ]; then
#    cmd2=$(printf "cp -T 32 listfiles_%s_child_%s%s.txt  file:%s/%s/unmerged/child_%s/%s/listfiles_%s_child_%s%s.txt\n" $trainname $i $stage $placetosave $trainname $i $stage $trainname $i $stage)
#  else
#    cmd2=$(printf "cp -T 32 listfiles_%s_child_%s.txt  file:%s/%s/unmerged/child_%s/listfiles_%s_child_%s.txt\n" $trainname $i $placetosave $trainname $i $trainname $i)
#  fi
#  cmd3=$(printf "rm listfiles_%s_child_%s%s.txt\n" $trainname $i $stage)
#
#  /opt/jalien/src/jalien/jalien << EOF
#  $cmd1
#  $cmd2
#  $cmd3
#  exit
#  EOF
#done

#TODO: If not already there, put 'alien://' in front of path (Note that this is already in inputchild[] at the moment
#TODO: Check if not Stage_1 files are stored in the .txt file, if so delete
#From now on, the script assumes there is a filled .txt file in each child_X directory
#with the name 'listfiles_$trainname_child_X.txt' or 'listfiles_$trainname_child_XStage_1.txt'

#Log files (first letters are for macro) + date and timestamp
datestamp2="$(date +"%d-%m-%Y")"
timestamp="$(date +"%H-%M-%S")"
if [ -z "$4" ]; then
  stdoutputfile=$(printf "SDG_%s_stdout_%s-%s.txt" $trainname $datestamp2 $timestamp)
  stderrorfile=$(printf "SDG_%s_stderr_%s-%s.txt" $trainname $datestamp2 $timestamp)
else
  stdoutputfile=$(printf "SDG_%s_%s_stdout_%s-%s.txt" $trainname $stage $datestamp2 $timestamp)
  stderrorfile=$(printf "SDG_%s_%s_stderr_%s-%s.txt" $trainname $stage $datestamp2 $timestamp)
fi



#----RUNNING THE SKIMMER DIRECTLY FROM GRID----#
printf "\n\n\e[1m----RUNNING THE SKIMMER DIRECTLY FROM GRID----\e[0m\n\n"
printf "Skimming for: Dplus (%s), Ds (%s), Dzero (%s), Dstar (%s), Lc->pKpi (%s), Lc->pK0s (%s)\n" $doDplus $doDs $doDzero $doDstar $doLcpKpi $doLcpK0s

for ((i=1; i<=$ninput; i++))
do

  #Load the .txt in which downloader saved list of files
  if [ -z "$stage" ]; then
    outputlist=$(printf "%s/%s/unmerged/child_%s/listfiles_%s_child_%s.txt" $placetosave $trainname $i $trainname $i)
  else
    outputlist=$(printf "%s/%s/unmerged/child_%s/%s/listfiles_%s_child_%s%s.txt" $placetosave $trainname $i $stage $trainname $i $stage)
  fi

  #First save logs in separate file, so macro can search for "error"-related words
  skimmeroutputfile="skimmer_stdout.txt"
  skimmererrorfile="skimmer_stderr.txt"
  printf "  Output of skimmer (child_%s) stored in:  \e[1m%s\e[0m\n  Warnings/Errors of skimmer stored in:   \e[1m%s\e[0m\n" $i $stdoutputfile $stderrorfile
  runskimmer="sh ./utils/skimmer.sh"

  printf "\n\n\n\nSkimming child_$i starts here\n\n" > "$skimmeroutputfile"
  printf "\n\n\n\nSkimming child_$i starts here\n\n" > "$skimmererrorfile"

  #run skimmer + progress bar
  sh ./utils/run_skimmer $runskimmer $outputlist $isMC $ispp $doDplus $doDs $doDzero $doDstar $doLcpKpi $doLcpK0s "1" >> "$skimmeroutputfile" 2>> "$skimmererrorfile"

  #Look for errors in logfile, and print warning if the case
  if grep -q "Error\|ERROR\|error\|segmentation\|Segmentation\|SEGMENTATION\|fault\|No such file or directory" "$skimmererrorfile"
  then
    printf "\e[1;31mwith errors, check log!\e[0m\n\n"
  else
    printf "\e[1;32mwithout errors\e[0m\n\n"
  fi

  #Copy log in general log, and empty for next child
  cat "$skimmeroutputfile" >> "$stdoutputfile"
  cat "$skimmererrorfile" >> "$stderrorfile"
  rm "$skimmeroutputfile" "$skimmererrorfile"

done

mv $stdoutputfile $placetosave/$trainname/
mv $stderrorfile $placetosave/$trainname/

printf "\n\e[1mMoved log files to $placetosave/$trainname/\e[0m\n"
printf "\n\e[1m----SKIMMER FINISHED----\e[0m\n\n"
