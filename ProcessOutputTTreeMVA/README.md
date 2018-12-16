# Getting and processing TTreeCreator output

Instructions to download the output from the LEGO train, merge the files, and skim for specific mesons.

## 1) Setup

The following scripts works out of the box on lxplus, but with the disadvantage that one has to download them another time when the output is needed on local system/server. 

To be able to run the processing scripts on a local system one has to use the following instead (assuming aliBuild is installed. If this is not the case, follow: https://alice-doc.github.io/alice-analysis-tutorial/building/build.html):
```
aliBuild build AliPhysics --defaults user-root6 -z aliroot6
aliBuild build AliPhysics --defaults jalien-root6 -z jalienroot6
```

## 2) Download train output

One needs to enter the jAliEn environment to download the output. On lxplus, use
```
/cvmfs/alice.cern.ch/bin/alienv enter JAliEn
```
On a local system:
```
alienv enter AliPhysics/latest-jalienroot6-jalien-root6
```
The first time of a session, one needs to "login" to the GRID, so the download script can access the files on the GRID:
```
jalien
#Enter Grid Certificate password
exit
```
Now it is only a matter of waiting till everything is downloaded using the following script:
```
./downloadOutputTrain.sh $TRAINNAME $PLACETOSAVEOUTPUT $STAGE
```
where 
* $TRAINNAME = 297_20181120-2315_child_1 (example for pp@5TeV containing D0, D+ and Ds candidates)
* $PLACETOSAVEOUTPUT = "" for the current directory or ../ALICEanalysis/MLproductions for example
* $STAGE = "" or "Stage_1" to specify when the merging on the GRID was succesful.

Four train-specific variables can be set in the script (highlighted with "#toset"):
* OUTPUTPATH       (output of train, default is pp@5TeV for the Devel_2 LEGO train)
* NFILES       (number of files to download, default is /&#42;/, for tests one can use /000&#42;/ to download only 10 files, /00&#42;/ for 100 files, etc)
* OUTPUTFILE       (name of file to download, default is AnalysisResults.root)

## 3) Merging

Exit jAliEn environment (if needed), and load normal AliPhysics. For lxplus one uses:
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
where $TRAINNAME, $PLACETOSAVEOUTPUT, and $STAGE = "Stage_#" should be the same as for the downloadOutputTrain script. In addition one has
* $NFILESFORMERGING   (the amount of files to be merged using hadd, default is 4)

## 4) Skimming

Enable the mesons you want to skim in the macro, and run:
```
./submitSkimJobs.sh /path/to/outputdir/mergeSkimOutputDir_$NFILESFORMERGING/lsOutputMergedList_$TRAINNAME$STAGE.txt
```
The txt file is saved by the mergefiles.sh script. The variables should therefore be the same as used before. 

If no merging was applied, one has to tweak a bit the output of the downloading stage.

## In case of problems:

For problems luuk.vermunt@cern.ch
