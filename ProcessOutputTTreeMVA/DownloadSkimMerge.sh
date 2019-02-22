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

printf "\n\n\n\e[1m----RUNNING THE DOWNLOADER-SKIMMER-MERGER----\e[0m\n\n"



#----THINGS TO SET----#
nfiles="/000*/" #toset   For testing: "0*", "00*", or "000*" (Assuming 1000 < jobs < 9999)
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
printf "   Number of files to download from grid: \e[1m$nfiles\e[0m\n"
printf "   Outputfile to be downloaded from grid: \e[1m$outputfile.root\e[0m\n"
printf "   Number of skimmed files to be merged:  \e[1m$filestomerge\e[0m\n       \033[0;37m(NB: for pp data '100' is usually used, for pp MC '200')\e[0m\n"
printf "   Particles that are enabled: Dplus \e[1m(%s)\e[0m, Ds \e[1m(%s)\e[0m, Dzero \e[1m(%s)\e[0m, Dstar \e[1m(%s)\e[0m, Lc->pKpi \e[1m(%s)\e[0m, Lc->pK0s \e[1m(%s)\e[0m\n" $doDplus $doDs $doDzero $doDstar $doLcpKpi $doLcpK0s
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
#When arguments are not given, user should provide them while macro is started
#Checking argument 1, trainname
if [ -z "$1" ]; then
  printf "Please enter train name: "
  read trainname
  printf "  Will download \e[1m$outputfile.root\e[0m output from train: \e[1m$trainname\e[0m \n"
else
  trainname=$1
  printf "Will download \e[1m$outputfile.root\e[0m output from train: \e[1m$trainname\e[0m \n"
fi

#Checking argument 2, dataset
if [ -z "$2" ]; then
  printf "\nPlease enter dataset name (LHC17pq, LHC18a4a2, LHC16i2a, LHC18r, ...): "
  read dataset
  printf "  Chosen dataset: \e[1m$dataset\e[0m\n"
#  printf "  \e[0;31mWarning: For now only the Devel_2 LEGO train is implemented.\e[0m\n"
else
  dataset=$2
  printf "\nChosen dataset: \e[1m$dataset\e[0m\n"
fi

#Hardcoded information about dataset
if [ "$dataset" == "LHC17pq" ]; then
  inputpathchild1=/alice/data/2017/LHC17p/000282341/pass1_FAST/PWGZZ/Devel_2
  inputpathchild2=/alice/data/2017/LHC17p/000282341/pass1_CENT_wSDD/PWGZZ/Devel_2
  inputpathchild3=/alice/data/2017/LHC17q/000282366/pass1_FAST/PWGZZ/Devel_2
  inputpathchild4=/alice/data/2017/LHC17q/000282366/pass1_CENT_wSDD/PWGZZ/Devel_2
  ninput=4
  isMC=0
  ispp=1
  datasetwithchilds=1
elif [ "$dataset" == "LHC18a4a2" ]; then
  inputpathchild1=/alice/sim/2018/LHC18a4a2_fast/282341/PWGZZ/Devel_2
  inputpathchild2=/alice/sim/2018/LHC18a4a2_fast/282366/PWGZZ/Devel_2
  inputpathchild3=/alice/sim/2018/LHC18a4a2_cent/282341/PWGZZ/Devel_2
  inputpathchild4=/alice/sim/2018/LHC18a4a2_cent/282366/PWGZZ/Devel_2
  ninput=4
  isMC=1
  ispp=1
  datasetwithchilds=1
elif [ "$dataset" == "LHC16i2a" ]; then
  inputpathchild1=/alice/sim/2016/LHC16i2a/246087/AOD198/PWGZZ/Devel_2
  ninput=1
  isMC=1
  ispp=0
  datasetwithchilds=0
elif [ "$dataset" == "LHC18r" ]; then
  inputpathchild1=/alice/data/2018/LHC18r/000296932/pass1/PWGZZ/Devel_2
  ninput=1
  isMC=0
  ispp=0
  datasetwithchilds=0
elif [ "$dataset" == "LHC18r_test" ]; then
  inputpathchild1=/alice/data/2018/LHC18r/000296787/pass1/PWGZZ/Devel_2
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

#Checking argument 4, GRID merging stage
if [ -z "$4" ]; then
  printf "\n\e[0;31mWarning: No GRID merging stage was entered. I will download non-merged files\e[0m\n"
else
  stage=$4
  printf "\nI will download files from GRID merging: \e[1m$stage\e[0m    (if not in format Stage_#, download will fail)\n"
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

#Log filess (first letters are for macro) + date and timestamp
timestamp="$(date +"%H-%M-%S")"
if [ -z "$4" ]; then
  stdoutputfile=$(printf "DSM_%s_stdout_%s-%s.txt" $trainname $datestamp $timestamp)
  stderrorfile=$(printf "DSM_%s_stderr_%s-%s.txt" $trainname $datestamp $timestamp)
else
  stdoutputfile=$(printf "DSM_%s_%s_stdout_%s-%s.txt" $trainname $stage $datestamp $timestamp)
  stderrorfile=$(printf "DSM_%s_%s_stderr_%s-%s.txt" $trainname $stage $datestamp $timestamp)
fi



#----RUNNING THE DOWNLOADER----#
printf "\n\n\e[1m----RUNNING THE DOWNLOADER----\e[0m\n\n"
printf "  Output of downloaders stored in:            \e[1m%s\e[0m\n  Warnings/Errors of downloader stored in:    \e[1m%s\e[0m\n" $i $stdoutputfile $stderrorfile
rundownloader="sh ./utils/downloader.sh"

printf "\n\n\n\nOutput downloading starts here\n\n" > "$stdoutputfile"
printf "\n\n\n\nErrors downloading starts here\n\n" > "$stderrorfile"

#run downloaders + progress bar. Not in parallel as writing time is the limiting factor here
if [ $ninput -eq 1 ]; then
  sh ./utils/run_downloader $rundownloader $inputpathchild1 1 "$nfiles" $outputfile $placetosave $trainname $datasetwithchilds $stage >> "$stdoutputfile" 2>> "$stderrorfile"
elif [ $ninput -eq 2 ]; then
  sh ./utils/run_downloader $rundownloader $inputpathchild1 1 "$nfiles" $outputfile $placetosave $trainname $datasetwithchilds $stage >> "$stdoutputfile" 2>> "$stderrorfile"
  sh ./utils/run_downloader $rundownloader $inputpathchild2 2 "$nfiles" $outputfile $placetosave $trainname $datasetwithchilds $stage >> "$stdoutputfile" 2>> "$stderrorfile"
elif [ $ninput -eq 3 ]; then
  sh ./utils/run_downloader $rundownloader $inputpathchild1 1 "$nfiles" $outputfile $placetosave $trainname $datasetwithchilds $stage >> "$stdoutputfile" 2>> "$stderrorfile"
  sh ./utils/run_downloader $rundownloader $inputpathchild2 2 "$nfiles" $outputfile $placetosave $trainname $datasetwithchilds $stage >> "$stdoutputfile" 2>> "$stderrorfile"
  sh ./utils/run_downloader $rundownloader $inputpathchild3 3 "$nfiles" $outputfile $placetosave $trainname $datasetwithchilds $stage >> "$stdoutputfile" 2>> "$stderrorfile"
elif [ $ninput -eq 4 ]; then
  sh ./utils/run_downloader $rundownloader $inputpathchild1 1 "$nfiles" $outputfile $placetosave $trainname $datasetwithchilds $stage >> "$stdoutputfile" 2>> "$stderrorfile"
  sh ./utils/run_downloader $rundownloader $inputpathchild2 2 "$nfiles" $outputfile $placetosave $trainname $datasetwithchilds $stage >> "$stdoutputfile" 2>> "$stderrorfile"
  sh ./utils/run_downloader $rundownloader $inputpathchild3 3 "$nfiles" $outputfile $placetosave $trainname $datasetwithchilds $stage >> "$stdoutputfile" 2>> "$stderrorfile"
  sh ./utils/run_downloader $rundownloader $inputpathchild4 4 "$nfiles" $outputfile $placetosave $trainname $datasetwithchilds $stage >> "$stdoutputfile" 2>> "$stderrorfile"
else
  printf "ERROR: More than 4 childs/runlist not yet supported, please implement. Returning..."
  exit
fi

#Check logs for the comman 'jalien command not found' error. If this is the case, no files where downloaded.
if grep -q "jalien\|command not found" "$stderrorfile"
then
  printf "\e[1;31m  Warning: The 'jalien' command was not found, so no new files where downloaded. Did you already connect to JAliEn? Check log if this was not intended!\e[0m\n\n"
fi
#Check logs for the comman 'JBox agent could not be started' error. If this is the case, no files where downloaded.
if grep -q "JBox agent\|could not be started" "$stderrorfile"
then
printf "\e[1;31m  Warning: The 'JBox agent' could not be started, so no new files where downloaded. Did you already connect to JAliEn? Check log if this was not intended!\e[0m\n\n"
fi

#For safety, wait till downloading is finished
wait

#----RUNNING THE SKIMMER----#
printf "\n\n\e[1m----RUNNING THE SKIMMER----\e[0m\n\n"
printf "Skimming for: Dplus (%s), Ds (%s), Dzero (%s), Dstar (%s), Lc->pKpi (%s), Lc->pK0s (%s) \n" $doDplus $doDs $doDzero $doDstar $doLcpKpi $doLcpK0s

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
  sh ./utils/run_skimmer $runskimmer $outputlist $isMC $ispp $doDplus $doDs $doDzero $doDstar $doLcpKpi $doLcpK0s >> "$skimmeroutputfile" 2>> "$skimmererrorfile"

  #Look for errors in logfile, and print warning if the case
  if grep -q "Error\|ERROR\|error\|segmentation\|Segmentation\|SEGMENTATION\|fault" "$skimmererrorfile"
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

#For safety, wait till skimming is finished
wait

#----RUNNING THE MERGER----#
printf "\n\e[1m----RUNNING THE MERGER----\e[0m\n\n"
printf "Merging for: Dplus (%s), Ds (%s), Dzero (%s), Dstar (%s), Lc->pKpi (%s), Lc->pK0s (%s)\n" $doDplus $doDs $doDzero $doDstar $doLcpKpi $doLcpK0s

#Create /merged/ dir, in exactly the same structure as downloaded files
mkdir -p -m 777 $placetosave/$trainname/merged
if [ $? -ne 0 ]; then
  printf "\n\e[1;31mError: Could not create output directory. Is $placetosave/$trainname writable? Returning... \e[0m\n\n"
  exit
else
  printf "Created directory: \e[1m$placetosave/$trainname/merged\e[0m \n"
fi

for ((i=1; i<=$ninput; i++))
do

  #Create child_X directories, as exactly the same structure as downloaded files
  mkdir -p -m 777 $placetosave/$trainname/merged/child_$i
  if [ $? -ne 0 ]; then
    printf "\n\e[1;31mError: Could not create output directory. Is $placetosave/$trainname/merged writable? Returning... \e[0m\n\n"
    exit
  else
    printf "Created directory: \e[1m$placetosave/$trainname/merged/child_$i\e[0m \n"
  fi

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

done

#Saving log files in output directory
mv $stdoutputfile $placetosave/$trainname/
mv $stderrorfile $placetosave/$trainname/

printf "\n\e[1mMoved log files to $placetosave/$trainname/\e[0m\n"
printf "\n\e[1m----DOWNLOADER-SKIMMER-MERGER FINISHED----\e[0m\n\n"
