#!/bin/bash
#Arguments to this bash:
#   $1 is trainname (e.g. 297_20181120-2315)
#   $2 is dataset (e.g. for pp5TeV LHC17pq or LHC18a4a2)
#   $3 is path to place to save output (e.g. "" or ../ALICEanalysis/MLproductions/)
#   $4 is GRID merging Stage_X (e.g. "" for no merging, or Stage_1)
#
#To set in script (find with "#toset"):
#   OUTPUTFILE (name of file to download)
#   What mesons to skim
#   Number of files to merge

printf "\n\n\n\e[1m----RUNNING THE SKIMMER-MERGER----\e[0m\n\n"



#----THINGS TO SET----#
outputfile="AnalysisResults" #toset

doDplus=1       #toset (skimmers)
doDs=0          #toset (skimmers)
doDzero=0       #toset (skimmers)
doDstar=0       #toset (skimmers)
doLcpKpi=0      #toset (skimmers)
doLcpK0s=0      #toset (skimmers)

filestomerge=150


#Confirm with user if hardcoded values are what he/she wants
printf "\e[1mYou set the following setters in the script. Please check them carefully before continuing.\e[0m\n"
printf "   Outputfile that was downloaded from grid: \e[1m$outputfile.root\e[0m\n"
printf "   Number of skimmed files to be merged:  \e[1m$filestomerge\e[0m\n       \033[0;37m(NB: for pp data '100' is usually used, for pp MC '200')\e[0m\n"
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
  ninput=4 #LHC17pq for Devel_2 has 4 childs
  isMC=0
  ispp=1
elif [ "$dataset" == "LHC18a4a2" ]; then
  ninput=4 #LHC18a4a2 for Devel_2 has 4 childs
  isMC=1
  ispp=1
elif [ "$dataset" == "LHC16i2a" ]; then
  ninput=1
  isMC=1
  ispp=0
elif [ "$dataset" == "LHC18r" ]; then
  ninput=1
  isMC=0
  ispp=0
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

#User should provide the date when downloading was started (because of folder name)
printf "Please enter date when files where downloaded (dd-mm-yyyy): "
read datestamp
printf "Will look for the folder: \e[1m$datestamp\e[0m \n"

#Check if folders exist, otherwise exit with warning
if [ ! -d "$placetosave/$datestamp" ]; then
  printf "\n\e[1;31mError: Could not find input directory: $placetosave/$datestamp. Are you sure you put the right path? Returning... \e[0m\n\n"
  exit
else
  printf "\nInput directory exist. Processing: $placetosave/$datestamp\n"
  placetosave=$placetosave/$datestamp
fi

if [ ! -d "$placetosave/$trainname" ]; then
  printf "\n\e[1;31mError: Could not find input directory: $placetosave/$trainname. Are you sure you put the right trainname? Returning... \e[0m\n\n"
  exit
else
  printf "Input directory exist. Processing: $placetosave/$trainname\n"
fi

if [ ! -d "$placetosave/$trainname/unmerged" ]; then
  printf "\n\e[1;31mError: Could not find input directory: $placetosave/$trainname/unmerged. Is the data downloaded? Returning... \e[0m\n\n"
  exit
else
  printf "Input directory exist. Processing: $placetosave/$trainname/unmerged\n"

  #Saving the existing childs in bitmap, so macro will not look for child that was not downloaded
  childs=0
  for ((i=$ninput; i>=1; i--))
  do
    if [ ! -d "$placetosave/$trainname/unmerged/child_$i" ]; then
      printf "\e[1;31mWarning: Could not find input directory: $placetosave/$trainname/unmerged/child_$i. \e[0m\n"
      childs=$((childs<<1))
    else
      printf "Input directory exist. Processing: $placetosave/$trainname/unmerged/child_$i\n"

      mkdir -p -m 777 $placetosave/$trainname/merged/child_$i
      if [ $? -ne 0 ]; then
        printf "\n\e[1;31mError: Could not create output directory. Is $placetosave/$trainname/merged writable? Returning... \e[0m\n\n"
        exit
      else
        printf "Created directory: \e[1m$placetosave/$trainname/merged/child_$i\e[0m \n"
      fi

      childs=$((childs<<1))
      childs=$((childs+1))
    fi
  done
fi

#Log filess (first letters are for macro) + date and timestamp
datestamp2="$(date +"%d-%m-%Y")"
timestamp="$(date +"%H-%M-%S")"
if [ -z "$4" ]; then
  stdoutputfile=$(printf "SM_%s_stdout_%s-%s.txt" $trainname $datestamp2 $timestamp)
  stderrorfile=$(printf "SM_%s_stderr_%s-%s.txt" $trainname $datestamp2 $timestamp)
else
  stdoutputfile=$(printf "SM_%s_%s_stdout_%s-%s.txt" $trainname $stage $datestamp2 $timestamp)
  stderrorfile=$(printf "SM_%s_%s_stderr_%s-%s.txt" $trainname $stage $datestamp2 $timestamp)
fi



#----RUNNING THE SKIMMER----#
printf "\n\n\e[1m----RUNNING THE SKIMMER----\e[0m\n\n"
printf "Skimming for: Dplus (%s), Ds (%s), Dzero (%s), Dstar (%s), Lc->pKpi (%s), Lc->pK0s (%s)\n" $doDplus $doDs $doDzero $doDstar $doLcpKpi $doLcpK0s

for ((i=1; i<=$ninput; i++))
do

  #Checking bitmap, continue only when child was found before
  if (((childs&0x01)>0)) && ((i==1)); then
    foundchild=1
  elif (((childs&0x02)>0)) && ((i==2)); then
    foundchild=1
  elif (((childs&0x04)>0)) && ((i==3)); then
    foundchild=1
  elif (((childs&0x08)>0)) && ((i==4)); then
    foundchild=1
  elif (((childs&0x10)>0)) && ((i==5)); then
    foundchild=1
  elif (((childs&0x20)>0)) && ((i==6)); then
    foundchild=1
  elif (((childs&0x40)>0)) && ((i==7)); then
    foundchild=1
  elif (((childs&0x80)>0)) && ((i==8)); then
    foundchild=1
  else
    foundchild=0
  fi

  if [ "$foundchild" == "1" ]; then
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
    sh ./utils/run_skimmer $runskimmer $outputlist $isMC $ispp $doDplus $doDs $doDzero $doDstar $doLcpKpi $doLcpK0s "0" >> "$skimmeroutputfile" 2>> "$skimmererrorfile"

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
  fi

done


#For safety, wait till skimming is finished
wait


#----RUNNING THE MERGER----#
printf "\n\e[1m----RUNNING THE MERGER----\e[0m\n\n"
printf "Merging for: Dplus (%s), Ds (%s), Dzero (%s), Dstar (%s), Lc->pKpi (%s), Lc->pK0s (%s) \n" $doDplus $doDs $doDzero $doDstar $doLcpKpi $doLcpK0s

for ((i=1; i<=$ninput; i++))
do

  #Checking bitmap, continue only when child was found before
  if (((childs&0x01)>0)) && ((i==1)); then
    foundchild=1
  elif (((childs&0x02)>0)) && ((i==2)); then
    foundchild=1
  elif (((childs&0x04)>0)) && ((i==3)); then
    foundchild=1
  elif (((childs&0x08)>0)) && ((i==4)); then
    foundchild=1
  elif (((childs&0x10)>0)) && ((i==5)); then
    foundchild=1
  elif (((childs&0x20)>0)) && ((i==6)); then
    foundchild=1
  elif (((childs&0x40)>0)) && ((i==7)); then
    foundchild=1
  elif (((childs&0x80)>0)) && ((i==8)); then
    foundchild=1
  else
    foundchild=0
  fi

  if [ "$foundchild" == "1" ]; then

    #First save logs in separate file, so macro could search for "error"-related words
    mergeroutputfile="merger_stdout.txt"
    mergererrorfile="merger_stderr.txt"
    printf "  Output of merger (child_%s) stored in:  \e[1m%s\e[0m\n  Warnings/Errors of merger stored in:   \e[1m%s\e[0m\n" $i $stdoutputfile $stderrorfile
    runmerger="sh ./utils/merger.sh"

    printf "\n\n\n\nMerging child_$i starts here\n\n" > "$mergeroutputfile"
    printf "\n\n\n\nMerging child_$i starts here\n\n" > "$mergererrorfile"

    #run mergers + progress bar
    if [ "$doDplus" == "1" ]; then
      sh ./utils/run_merger $runmerger $trainname $placetosave $i $filestomerge "Dplus" $stage >> "$mergeroutputfile" 2>> "$mergererrorfile"
    fi
    if [ "$doDs" == "1" ]; then
      sh ./utils/run_merger $runmerger $trainname $placetosave $i $filestomerge "Ds" $stage >> "$mergeroutputfile" 2>> "$mergererrorfile"
    fi
    if [ "$doDzero" == "1" ]; then
      sh ./utils/run_merger $runmerger $trainname $placetosave $i $filestomerge "Dzero" $stage >> "$mergeroutputfile" 2>> "$mergererrorfile"
    fi
    if [ "$doDstar" == "1" ]; then
      sh ./utils/run_merger $runmerger $trainname $placetosave $i $filestomerge "Dstar" $stage >> "$mergeroutputfile" 2>> "$mergererrorfile"
    fi
    if [ "$doLcpKpi" == "1" ]; then
      sh ./utils/run_merger $runmerger $trainname $placetosave $i $filestomerge "LctopKpi" $stage >> "$mergeroutputfile" 2>> "$mergererrorfile"
    fi
    if [ "$doLcpK0s" == "1" ]; then
      sh ./utils/run_merger $runmerger $trainname $placetosave $i $filestomerge "LctopK0s" $stage >> "$mergeroutputfile" 2>> "$mergererrorfile"
    fi

    #Copy log in general log, and empty for next child
    cat "$mergeroutputfile" >> "$stdoutputfile"
    cat "$mergererrorfile" >> "$stderrorfile"
    rm "$mergeroutputfile" "$mergererrorfile"
  fi

done


#Saving log files in output directory
mv $stdoutputfile $placetosave/$trainname/
mv $stderrorfile $placetosave/$trainname/

printf "\n\e[1mMoved log files to $placetosave/$trainname/\e[0m\n"
printf "\n\e[1m----SKIMMER-MERGER FINISHED----\e[0m\n\n"
