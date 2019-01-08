# Getting and processing TTreeCreator output

Instructions to download the output from the LEGO train, merge the files, and skim for specific mesons.

## 1) Setup

The following scripts works out of the box on lxplus (if you have copied your GRID certificates to ~/.globus/), but with the disadvantage that one has a storage limitation and the files needs to be downloaded another time when the output is needed on local system/server. 

To be able to run the processing scripts on a local system instead, one has to setup the following (assuming aliBuild is installed. If this is not the case, follow https://alice-doc.github.io/alice-analysis-tutorial/building/build.html first):
```
aliBuild build AliPhysics --defaults user-root6 -z aliroot6
aliBuild build AliPhysics --defaults jalien-root6 -z jalienroot6
```
> **NB1** jAliEn doesn't work (for the moment) within alidock, so one should still use the custom alibuild installation!

> **NB2** At the time of writing (08/01/19) there are three small bugs in alidist, causing the build of jalien-root6 to fail. Experts are notified. For the moment one can tweak a bit your local folder by:
> * Delete the **-lXrdSec** and **-lXrdOfs** flags in the makefiles of https://github.com/alisw/xalienfs. A dirty fix is by looking for (and deleting) these flags in *client/gclientlib/Makefile.am*, *client/gclientlib/Makefile.in*, *server/Makefile.am, server/Makefile.in* in xalienfs.sh in your local alidist folder by doing for example: *"sed  -i 's/-lXrdSec //g' client/gclientlib/Makefile.am"* before the " ./bootstrap.sh"-line
> * (only needed on macOS) Add **.git** at the end of line https://github.com/alisw/alidist/blob/a5d9bce7e19203aa6fb74276138ba88027c413b4/jalien-root.sh#L4

## 2) Download train output

One needs to enter the jAliEn environment to download the output. On lxplus, use
```
/cvmfs/alice.cern.ch/bin/alienv enter JAliEn
```
On a local system:
```
alienv enter AliPhysics/latest-jalienroot6-jalien-root6
```
The first time of a session, one needs to "login" to the GRID, so the download script can access the files:
```
jalien
#Enter Grid Certificate password
exit
```
> If you get: **JBox isn't running, so we won't start JSh.** on lxplus, your grid certificates probably don't have the right permissions. Correct them using: "chmod 0440 usercert.pem" and "chmod 0400 userkey.pem"

Now it is only a matter of waiting till everything is downloaded using the following script:
```
./downloadOutputTrain.sh $TRAINNAME $PLACETOSAVEOUTPUT $STAGE
```
where 
* $TRAINNAME = 297_20181120-2315_child_1 (example for pp@5TeV containing D0, D+ and Ds candidates, change accordingly)
* $PLACETOSAVEOUTPUT = "" for the current directory or ../ALICEanalysis/MLproductions for example
* $STAGE = can be specified when the merging on the GRID was succesful (e.g. "Stage_1").

Three train-specific variables can be set in the script (highlighted in the code by "#toset"):
* OUTPUTPATH       (output of train, default is pp@5TeV for the Devel_2 LEGO train)
* NFILES       (number of files to download, default is /&#42;/. For tests one can use /000&#42;/ to download only 10 files, /00&#42;/ for 100 files, etc)
* OUTPUTFILE       (name of file to download, default is AnalysisResults.root)

## 3) Merging

Exit jAliEn environment (if needed), and load AliPhysics. For lxplus one uses:
```
/cvmfs/alice.cern.ch/bin/alienv enter VO_ALICE@AliPhysics::vAN-20181208-1 #Change to recent date
```
Locally one should enter:
```
alienv enter AliPhysics/latest-aliroot6-user-root6
```
Run the merging script
```
./mergefiles.sh $TRAINNAME $PLACETOSAVEOUTPUT $STAGE $NFILESFORMERGING
```
where $TRAINNAME, $PLACETOSAVEOUTPUT, and $STAGE should be the same as for the downloadOutputTrain script. In addition one has
* $NFILESFORMERGING   (the amount of files to be merged using hadd, default is 4)

## 4) Skimming

Enable the mesons you want to skim in the submitSkimJobs.sh macro, specify if the output is Data or MC, pp or pPb/PbPb, and run:
```
./submitSkimJobs.sh /path/to/mergeSkimOutputDir_$NFILESFORMERGING/lsOutputMergedList_$TRAINNAME$STAGE.txt
```
The txt file is saved by the mergefiles.sh script. The variables should therefore be the same as used before. 

If no merging was applied (not recommended), one has to tweak a bit the listfilesMerging_%s%s.txt output of the downloading stage.

## In case of problems:

For problems luuk.vermunt@cern.ch
