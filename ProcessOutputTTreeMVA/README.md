# Getting and processing TTreeCreator output

Instructions to download the output from the LEGO train, skim for specific mesons, and merge the files. The instructions assume you are an user of the new aliceml server. With some small changes, the instructions are valid for each system though.

## 1) Setup your environment

Start by logging in
```
ssh -X username@lxplus.cern.ch
ssh -X username@aliceml
```
> Please have a look at section 3) if you want to use these script on a local system or different server,  as some packages are already pre-installed at aliceml which you might need to install yourself first.

### a) Building and loading the virtual environment

While logged in at the server, one should (create/)load your personal virtual environment.
```
ml-create-virtualenv     #only once to create the environment
ml-activate-virtualenv   #start (and enable python) in virtual environment
ml-activate-root         #Enable system-wide ROOT installation
```
and clone this git repository
```
git clone https://github.com/ginnocen/MachineLearningHF
cd MachineLearningHF/ProcessOutputTTreeMVA
```

## 2) Download, Skim, and Merge the train output

Before downloading, one has to enter the jAliEn environment manually. Please make sure your GRID certificates are copied to the server.
```
jalien
#Enter Grid Certificate password
exit
```
> **NB:** If you get the error: "**JBox isn't running, so we won't start JSh.**", your grid certificates probably don't have the right permissions. Correct them in *~/.globus/* using: *"chmod 0440 usercert.pem"* and *"chmod 0400 userkey.pem"*. It seems that JALiEn needs slightly different permissions than usual.
 
 
The scripts are saved in *~/MachineLearningHF/ProcessOutputTTreeMVA*. There are two ways to run the DownloadSkimMerge.sh script:
1) Run ./DownloadSkimMerge.sh **without** arguments. The script will ask you for the required input.
2) Run ./DownloadSkimMerge.sh **with** arguments: *./DownloadSkimMerge.sh trainname dataset outputdirectory gridmergingstage*
> In this way, all three steps will run after each other. Please note that this can be a heavy process, so it is also possible to run the different steps separate (using './Download.sh', './Skim.sh', './Merge.sh', or './SkimMerge.sh').
 
 
The first argument is the trainname, which has the format: *trainnumber_date-time*. Secondly the dataset should be given, for now only *LHC17pq* (pp@5TeV data) and *LHC18a4a2* (HF-enriched MC for pp@5TeV) are implemented, but it is easy to add more. The outputdirectory, the third argument, is */data/HeavyFlavour/DmesonsLambdac_pp_2017_5TeV/\** for the aliceml server, although for testing it is better to use your local folder. The last argument is the GRID merging stage, which should be in the format Stage_#. If this argument is empty, JAliEn will download the unmerged files from GRID. 
> For now downloading the unmerged files is the preferred way, as it is not yet possible to distinguish between processed and unprocessed (in case of Stage_1 merging failure) jobs.
 
 
If (some of the) arguments are empty, the script will ask for your input:
1) The script will always ask for confirmation of the hardcoded variables (see below). Please read them carefully when submitting a job over full statistics.
2) You will be asked to fill in the trainname (if not provided).
3) You will be asked to fill in the dataset (if not provided).
4) Confirmation will be asked to save output in "./" (if no outputdirectory was provided). Output path can be changed if one doesn't answer 'y' or 'Y'
5) A warning will be shown that the script will download unmerged GRID files if the fourth argument was empty. This should however already have been checked in 1).
 
 
**Finally, note that the skimming and merging part of this script will be performed in parallel. So it will fill up the/your system. So please use with care!**

### a) Hardcoded values

A few variables are hardcoded in *DownloadSkimMerge.sh*:
1) The number of files to download from GRID. By default all files will be downloaded: **nfiles="/*/"**. For test runs, one can add some zeros ("/000*/", assuming 1000 < jobs < 9999) to download less files.
2) The file to be downloaded is by default: **AnalysisResults.root**.
3) The different particles can be turned on/off, those currently implemented are: D+, Ds, D0, D*  and Lc. By default D+ is turned **on**. 
4) The number of skimmed files to merge is set to **150**, which will give merged files of ~8GB. This number is based on the output of train 308, and should be reconsidered for different trains.

### b) The screen program

If one will download and process the full statistics, the *screen* program can be very usefull. This program allows you to keep programs running on the remote computer even if you disconnect from it. To use it, one should do:
```
ssh -X username@lxplus.cern.ch
screen    #A empty terminal will pop up
#Do everything till the DownloadSkimMerge.sh script is running
```
When the script is running, you can detach from it by pressing **Ctrl-a d**. You will find yourself back in the previous terminal. Before quiting, there is some information you should remember:
```
screen -list     #Should print something like: "There is a screen on: 32693.pts-30.lxplus008    (Detached)"
hostname         #Should print something like: "lxplus008.cern.ch"
```
Save this information somewhere, disconnect from lxplus, and start doing something else.

When the script is ready, or you want to check the progress, just do:
```
ssh username@lxplus008.cern.ch          #Change to your situation
screen -list
screen -rD 32693.pts-30.lxplus008       #Change to your situation
```
Is the script ready? Exit the *screen* program with **Ctrl-d**. Is the script still running, detach again with **Ctrl-a d**.


## 3) Installation on a local system/different server

The JAliEn tool is needed for downloading from the GRID. An installation of alibuild is needed (follow https://alice-doc.github.io/alice-analysis-tutorial/building/). Afterwards, one can build JAliEn (the installation should take a few minutes only).
```
mkdir -p ~/alice
cd ~/alice/
git clone https://github.com/alisw/alidist
aliBuild build JAliEn --defaults jalien -z jalien
```
> JAliEn is already installed at lxplus. To enter the environment, do '/cvmfs/alice.cern.ch/bin/alienv enter JAliEn'

You may need to edit the hard-coded jalien path (`/opt/jalien/src/jalien/jalien`) in `downloader.sh` to 
something appropriate for your system, e.g. simply `jalien`. 

ROOT is needed for the skimming and merging of the files. If this is not yet installed, please follow the instructions below. **Please note that these instructions don't build against a specific python version, which you might need for ML studies.**
```
git clone http://github.com/root-project/root.git
cd root
git checkout -b v6-14-04 v6-14-04    #Or any other version listed by 'git tag -l'
mkdir -p ../rootbuild
cd ../rootbuild
cmake ../root
```
Please change **N** into the number of cores to be used.
```
cmake --build . -- -jN
```
When ready, source ROOT in your ~/.bashrc
```
source $HOME/alice/rootbuild/bin/thisroot.sh    #change directory if needed
```
and source your .bashrc
```
source ~/.bashrc
```

## In case of problems:

For problems luuk.vermunt@cern.ch
