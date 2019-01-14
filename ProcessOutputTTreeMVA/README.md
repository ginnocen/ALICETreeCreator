# Getting and processing TTreeCreator output

Instructions to download the output from the LEGO train, skim for specific mesons, and merge the files. The instructions assume you are an user of  the new aliceml server. With some small changes, the instructions are valid for each system though.

## 1) Setup

Start by logging in
```
ssh -X lxplus7.cern.ch
ssh -X username@aliceml
```

### a) Installing aliBuild
The prerequisites of aliBuild are already installed. One should however add the following lines in ~/.bashrc
```
export PYTHONUSERBASE="$HOME/user_python"
export PATH="$PYTHONUSERBASE/bin:$PATH"
```
and source your .bashrc
```
source ~/.bashrc
```
> One can test if it succeeded by doing: "echo $PYTHONUSERBASE". You should see /home/username/user_python

Install aliBuild
```
pip install alibuild --upgrade --user
```
> One can test if it succeeded by doing: "type aliBuild". You should see "aliBuild is /home/username/user_python/bin/aliBuild"

and add in you ~/.bashrc
```
export ALIBUILD_WORK_DIR="$HOME/alice/sw"
eval "`alienv shell-helper`"
```

### b) Building JAliEn

```
mkdir -p ~/alice
cd ~/alice/
git clone https://github.com/alisw/alidist
aliBuild build JAliEn --defaults jalien -z jalien
```

### c) Getting these scripts

```
cd ~/alice/
git clone https://github.com/ginnocen/MachineLearningHF
cd MachineLearningHF/ProcessOutputTTreeMVA
```

### d) Getting ROOT (optional)

Before building ROOT, check if there is not already a version installed as root. If so, just source it in your ~/.bashrc using
```
source path-to-general-root-build/bin/thisroot.sh
```
if this is not yet the case, please follow the instructions below. **Please note that these instructions don't build against a specific python version.**
```
git clone http://github.com/root-project/root.git
cd root
git checkout -b v6-14-04 v6-14-04    #Or any other version listed by 'git tag -l'
mkdir -p ../rootbuild
cd ../rootbuild
cmake ../root
```
Please check before building ROOT with the **top** command if the server is empty. If so, build with 40 cores (building ROOT should take less than 10 minutes).
```
cmake --build . -- -j40
```
When ready, source ROOT in your ~/.bashrc
```
source $HOME/alice/rootbuild/bin/thisroot.sh    #change directory if needed
```
and source your .bashrc
```
source ~/.bashrc
```

## 2) Download, Skim, and Merge the train output

One needs to enter the jAliEn environment to download the output.
```
alienv enter JAliEn/latest-jalien-jalien
jalien
#Enter Grid Certificate password
exit
```
> **NB:** If you get the error: "**JBox isn't running, so we won't start JSh.**" on lxplus, your grid certificates probably don't have the right permissions. Correct them in *~/.globus/* using: *"chmod 0440 usercert.pem"* and *"chmod 0400 userkey.pem"*. It seems that JALiEn needs slightly different permissions than usual.

The scripts are saved in *~/alice/MachineLearningHF/ProcessOutputTTreeMVA*. There are two ways to run the DownloadSkimMerge.sh script:
1) Run ./DownloadSkimMerge.sh without arguments. The script will ask you for the required input.
2) Run ./DownloadSkimMerge.sh with arguments: *./DownloadSkimMerge.sh trainname dataset outputdirectory gridmergingstage*

The first argument is the trainname, which has the format: *trainnumber_date-time*. Secondly the dataset should be given, for now only *LHC17pq* (pp@5TeV data) and *LHC18a4a2* (HF-enriched MC for pp@5TeV) are implemented. The outputdirectory for aliceml is */data/HeavyFlavour/DmesonsLambdac_pp_2017_5TeV/\**, although for testing one can better use your local folder. The last argument is the GRID merging stage, which should be in the format Stage_#. If this argument is empty, JAliEn will download the unmerged files from GRID. 
> For now downloading the unmerged files is the preferred way, as it is not yet possible to distinguish between processed and unprocessed (in case of Stage_1 merging failure) jobs.

If (some of the) arguments are empty, the script will ask for your input:
1) The script will always ask for confirmation of the hardcoded variables (see below). Please read them carefully when submitting a job over full statistics.
2) You will be asked to fill in the trainname (if not provided).
3) You will be asked to fill in the dataset (if not provided).
4) Confirmation will be asked to save output in "./" (if no outputdirectory was provided). Output path can be changed if one doesn't answer 'y' or 'Y'
5) A warning will be shown that the script will download unmerged GRID files if the fourth argument was empty. This was however already checked in 1).

**Finally, note that the skimming and merging part of this script will be performed in sequential. So it will fill up the/your system. So please use with care!**

### Hardcoded values

A few variables are hardcoded in *DownloadSkimMerge.sh*:
1) The number of files to download from GRID. By default all files will be downloaded: **nfiles="/*/"**. For test runs, one can add some zeros ("/000*/", assuming 1000 < jobs < 9999) to download less files.
2) The file to be downloaded is by default: **AnalysisResults.root**.
3) The different particles can be turned on/off. By default all (D+, Ds, D0, D*, and Lc) are turned **on**.
4) The number of skimmed files to merge is set to **150**. This number is based on the output of train 308, and should be reconsidered for different trains.

### The screen program

If one will download and process full statistics, the *screen* program is very usefull. This program allows you to keep programs running on the remote computer even if you disconnect from it. To use it
```
ssh -X lxplus7.cern.ch
screen    #A empty terminal will pop up
#Do everything till the DownloadSkimMerge.sh is running
```
When the script is running, you can detach from it by pressing **Ctrl-a d**. You will find yourself back in the previous terminal. Before quiting, there is some information you should remember:
```
screen -list     #Should print something like: "There is a screen on: 32693.pts-30.lxplus008    (Detached)"
hostname         #Should print something like: "lxplus008.cern.ch"
```
Save this information somewhere, exit lxplus, and start doing something else.

When the script is ready, or you want to check the progress, just do:
```
ssh lxplus008.cern.ch     #Change to your situation
screen -list
screen -rD 32693.pts-30.lxplus008       #Change to your situation
```
Is the script ready? Exit the *screen* program with **Ctrl-d**. Is the script still running, detach again with **Ctrl-a d**.

## In case of problems:

For problems luuk.vermunt@cern.ch
