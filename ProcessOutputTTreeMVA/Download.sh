#!/bin/bash
#IMPORTANT: Before running one should have entered jAliEn environment:
#    jalien
#    <Enter Grid certificate password>
#    exit
#
#Arguments to this bash:
#   $1 is trainname (e.g. 297_20181120-2315)
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
nfiles="/*/" #toset   For testing: "0*", "00*", or "000*" (Assuming 1000 < jobs < 9999)
outputfile="AnalysisResults" #toset

#Confirm with user if hardcoded values are what he/she wants
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
  printf "\nPlease enter dataset name (LHC17pq_woSDD, LHC2018_pp, "
  printf "\n                           LHC2016[17][18]_pass1[2]_MC_pp, "
  printf "\n                           or dataset name like 'LHC18r': "
  read dataset
  printf "  Chosen dataset: \e[1m$dataset\e[0m\n"
else
  dataset=$2
  printf "\nChosen dataset: \e[1m$dataset\e[0m\n"
fi

#Hardcoded information about dataset
if [ "$dataset" == "LHC17pq_woSDD" ]; then
  inputpaths=(/alice/data/2017/LHC17p/000282341/pass1_FAST/PWGHF/HF_TreeCreator
              /alice/data/2017/LHC17q/000282366/pass1_FAST/PWGHF/HF_TreeCreator
              /alice/data/2017/LHC17p/000282341/pass1_CENT_woSDD/PWGHF/HF_TreeCreator
              /alice/data/2017/LHC17q/000282366/pass1_CENT_woSDD/PWGHF/HF_TreeCreator)
  isMC=0
  ispp=1
  datasetwithchilds=1
elif [ "$dataset" == "LHC18a4a2" ]; then
  inputpaths=(/alice/sim/2018/LHC18a4a2_fast/282341/PWGHF/HF_TreeCreator
              /alice/sim/2018/LHC18a4a2_fast/282366/PWGHF/HF_TreeCreator
              /alice/sim/2018/LHC18a4a2_cent/282341/PWGHF/HF_TreeCreator
              /alice/sim/2018/LHC18a4a2_cent/282366/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=1
  datasetwithchilds=1
elif [ "$dataset" == "LHC2018_pp" ]; then
  inputpaths=(/alice/data/2018/LHC18b/000285064/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18d/000286313/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18e/000286653/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18f/000287784/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18g/000288750/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18h/000288806/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18i/000288908/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18j/000288943/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18k/000289177/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18l/000289931/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18m/000292430/pass1_withTRDtracking/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18n/000293359/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18o/000293741/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18p/000294011/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18f/000287208/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18l/000289444/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18m/000291397/pass1_withTRDtracking/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18m/000292701/pass1_withTRDtracking/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18m/000292430/pass1_withTRDtracking/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18p/000294925/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18p/000294152/pass1/PWGHF/HF_TreeCreator)
  isMC=0
  ispp=1
  datasetwithchilds=1
elif [ "$dataset" == "LHC2016_pass1_MC_pp" ]; then
  inputpaths=(/alice/sim/2017/LHC17h8a/253529/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=1
  datasetwithchilds=0
elif [ "$dataset" == "LHC2016_pass2_MC_pp" ]; then
  inputpaths=(/alice/sim/2018/LHC18f4a/257630/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=1
  datasetwithchilds=0
elif [ "$dataset" == "LHC2017_MC_pp" ]; then
  inputpaths=(/alice/sim/2018/LHC18l4a/272123/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=1
  datasetwithchilds=0
elif [ "$dataset" == "LHC2018_MC_pp" ]; then
  inputpaths=(/alice/sim/2018/LHC18l4b/285064/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=1
  datasetwithchilds=0
elif [ "$dataset" == "LHC17j4_adeghjop_MC_pp" ]; then
  inputpaths=(/alice/sim/2017/LHC17j4a/253529/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=1
  datasetwithchilds=0
elif [ "$dataset" == "LHC17j4_kl_MC_pp" ]; then
  inputpaths=(/alice/sim/2017/LHC17j4a/257630/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=1
  datasetwithchilds=0
elif [ "$dataset" == "LHC17j4d2" ]; then
  #Lc->pK0s enriched pPb MC (used for pp for now)
  inputpaths=(/alice/sim/2017/LHC17j4d2_fast/265343/PWGHF/HF_TreeCreator
              /alice/sim/2017/LHC17j4d2_fast/267163/PWGHF/HF_TreeCreator
              /alice/sim/2017/LHC17j4d2_cent_wSDD/265343/PWGHF/HF_TreeCreator
              /alice/sim/2017/LHC17j4d2_cent_wSDD/267163/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=1
  datasetwithchilds=1
elif [ "$dataset" == "LHC19c2b_all_q" ]; then
  inputpaths=(/alice/sim/2019/LHC19c2b/296433/PWGHF/HF_TreeCreator
              /alice/sim/2019/LHC19c2b2/296433/PWGHF/HF_TreeCreator
              /alice/sim/2019/LHC19c2b_extra/296433/PWGHF/HF_TreeCreator
              /alice/sim/2019/LHC19c2b2_extra/296433/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=0
  datasetwithchilds=1
elif [ "$dataset" == "LHC19c2b_all_r" ]; then
  inputpaths=(/alice/sim/2019/LHC19c2b/297481/PWGHF/HF_TreeCreator
              /alice/sim/2019/LHC19c2b2/297481/PWGHF/HF_TreeCreator
              /alice/sim/2019/LHC19c2b_extra/297481/PWGHF/HF_TreeCreator
              /alice/sim/2019/LHC19c2b2_extra/297481/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=0
  datasetwithchilds=1
elif [ "$dataset" == "LHC19c2a_all_q" ]; then
  inputpaths=(/alice/sim/2019/LHC19c2a/296433/PWGHF/HF_TreeCreator
              /alice/sim/2019/LHC19c2a2/296244/PWGHF/HF_TreeCreator
              /alice/sim/2019/LHC19c2a_extra/296433/PWGHF/HF_TreeCreator
              /alice/sim/2019/LHC19c2a2_extra/296244/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=0
  datasetwithchilds=1
elif [ "$dataset" == "LHC19c2a_all_r" ]; then
  inputpaths=(/alice/sim/2019/LHC19c2a/297481/PWGHF/HF_TreeCreator
              /alice/sim/2019/LHC19c2a2/297481/PWGHF/HF_TreeCreator
              /alice/sim/2019/LHC19c2a_extra/297481/PWGHF/HF_TreeCreator
              /alice/sim/2019/LHC19c2a2_extra/297481/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=0
  datasetwithchilds=1
elif [ "$dataset" == "LHC18l8b2_q" ]; then
  inputpaths=(/alice/sim/2018/LHC18l8b2/296433/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=0
  datasetwithchilds=0
elif [ "$dataset" == "LHC18l8b2_r" ]; then
  inputpaths=(/alice/sim/2018/LHC18l8b2/297481/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=0
  datasetwithchilds=0
elif [ "$dataset" == "LHC18l8c2_q" ]; then
  inputpaths=(/alice/sim/2018/LHC18l8c2/296433/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=0
  datasetwithchilds=0
elif [ "$dataset" == "LHC18l8c2_r" ]; then
  inputpaths=(/alice/sim/2018/LHC18l8c2/297481/PWGHF/HF_TreeCreator)
  isMC=1
  ispp=0
  datasetwithchilds=0
elif [ "$dataset" == "LHC18r" ]; then
  inputpaths=(/alice/data/2018/LHC18r/000296894/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000297218/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000297366/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000297481/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000297193/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000297590/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000296899/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000297415/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000297479/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000296799/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000297588/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000297512/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000296851/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18r/000297413/pass1/PWGHF/HF_TreeCreator)
  isMC=0
  ispp=0
  datasetwithchilds=0
elif [ "$dataset" == "LHC18q" ]; then
  inputpaths=(/alice/data/2018/LHC18q/000296415/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18q/000296549/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18q/000296377/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18q/000296197/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18q/000296244/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18q/000295831/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18q/000296550/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18q/000296433/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18q/000296623/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18q/000295826/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18q/000295854/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18q/000296511/pass1/PWGHF/HF_TreeCreator
              /alice/data/2018/LHC18q/000296424/pass1/PWGHF/HF_TreeCreator)
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
  stdoutputfile=$(printf "D_%s_stdout_%s-%s.txt" $trainname $datestamp $timestamp)
  stderrorfile=$(printf "D_%s_stderr_%s-%s.txt" $trainname $datestamp $timestamp)
else
  stdoutputfile=$(printf "D_%s_%s_stdout_%s-%s.txt" $trainname $stage $datestamp $timestamp)
  stderrorfile=$(printf "D_%s_%s_stderr_%s-%s.txt" $trainname $stage $datestamp $timestamp)
fi



#----RUNNING THE DOWNLOADER----#
printf "\n\n\e[1m----RUNNING THE DOWNLOADER----\e[0m\n\n"
printf "  Output of downloaders stored in:            \e[1m%s\e[0m\n  Warnings/Errors of downloader stored in:    \e[1m%s\e[0m\n" $i $stdoutputfile $stderrorfile
rundownloader="sh ./utils/downloader.sh"

printf "\n\n\n\nOutput downloading starts here\n\n" > "$stdoutputfile"
printf "\n\n\n\nErrors downloading starts here\n\n" > "$stderrorfile"

for input_index in ${!inputpaths[*]}
do
  ithinput=$(($input_index+1))
  localchild=$(($input_index+1))
  #--TO BE WRITTEN IN A NICER WAY--
  if [ "$dataset" == "LHC2018_pp" ]; then
    if [ $input_index -eq 14 ]; then
      ithinput=4
    elif [ $input_index -eq 15 ]; then
      ithinput=10
    elif [ $input_index -eq 16 ]; then
      ithinput=11
    elif [ $input_index -eq 17 ]; then
      ithinput=11
    elif [ $input_index -eq 18 ]; then
      ithinput=11
    elif [ $input_index -eq 19 ]; then
      ithinput=14
    elif [ $input_index -eq 20 ]; then
      ithinput=14
    fi
  fi
  sh ./utils/run_downloader $rundownloader ${inputpaths[$input_index]} $ithinput "$nfiles" $outputfile $placetosave $trainname $datasetwithchilds $localchild $stage >> "$stdoutputfile" 2>> "$stderrorfile"
done


#give all permissions to all directories downloaded from the GRID
chmod -R 777 $placetosave/$trainname/unmerged/

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

#Saving log files in output directory
mv $stdoutputfile $placetosave/$trainname/
mv $stderrorfile $placetosave/$trainname/

printf "\n\e[1mMoved log files to $placetosave/$trainname/\e[0m\n"
printf "\n\e[1m----DOWNLOADER FINISHED----\e[0m\n\n"
