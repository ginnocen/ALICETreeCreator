Instructions to run HFTreeCreator task



*********************************************************************************************
Step 1: Generate the file containing the cut objects for D0, Ds, Dplus

i) cd cutfile (directory where the macros are stored)
ii) aliroot / .L makeCutsTreeCreator.C / makeCutsTreeCreator()

makeCutsTreeCreator.C loads the macros where the cuts for D0, Ds and Dplus are defined and uses the methods defined there to create the cut objects for each meson and store them into a file .root. The cuts for each meson are defined in:
makeInputCutsD0toKpi.C
makeInputCutsDstoKKpi.C
makeInputCutsDplustoKpipi.C
The methods defined in each macro can be used to generate "filtering cuts" (loose cuts) or "analysis cuts" (optimised for the analysis in central PbPb events). The centrality class is set directly into makeCutsTreeCreator.C. Note that the names of the cut objects have to be as those defined in the current version of makeCutsTreeCreator.C. "D...FilteringCuts" are the cuts applied to fill the trees, "D...AnalysisCuts" are the cuts used to flag each candidate that passes the "D...FilteringCuts".

NB: generate the cut file with AliPhysics built on ROTT5/6 according to the ROOT version you have locally or used to build the AliPhyiscs version you want to use on Grid.
*********************************************************************************************
*********************************************************************************************
Step 2: configure the runAnalysis_treeCreator.C
The relevant settings have to be defined at the beginning of the runAnalysis macro.

- to run local tests:
i)  download one AliAOD.root and the corresponding AliAOD.VertexingHF.root files
ii) set flag runLocal=kTRUE
iii) set pathToLocalAODfiles equal to the local path where the AOD files are stored
iiii) set isRunOnMC=kTRUE or =kFALSE to run either on Monte Carlo or data

- to run in test mode
i) set flag runLocal=kFALSE
ii) set flag runGridTest=kTRUE;
iii) set gridDataDir and gridDataPattern:
. to run on data: gridDataDir="/alice/data/2015/LHC15o/"; gridDataPattern="/pass1/AOD194";
. ton run on MC: gridDataDir="/alice/sim/2016/LHC16i2a/"; gridDataPattern="/AOD198";
iiii) define the run list: nruns = n; runlist[nruns] = {runNumber, runNumber, ...};
iiiii) set isRunOnMC=kTRUE or =kFALSE to run either on Monte Carlo or data

- to run in full mode
i) set flag runLocal=kFALSE
ii) set flag runGridTest=kFALSE;
iii) set runMode="full"
iiii) set gridDataDir and gridDataPattern:
. to run on data: gridDataDir="/alice/data/2015/LHC15o/"; gridDataPattern="/pass1/AOD194";
. to run on MC: gridDataDir="/alice/sim/2016/LHC16i2a/"; gridDataPattern="/AOD198";
iiiii) set aliPhysVersion
iiiiii) set gridWorkingDir="testNtupleCreator"; and gridOutputDir="output";
iiiiiii) define the run list: nruns = n; runlist[nruns] = {runNumber0, runNumber1, ...};
iiiiiiii) set isRunOnMC=kTRUE or =kFALSE to run either on Monte Carlo or data


Common to the 3 running modes:
Set the cut file to be used by the task: cutFile="./cutfile/D0DsDplusCuts.root";
*********************************************************************************************
*********************************************************************************************
Step 3: run the task
i) .L runAnalysis_treeCreator.C
ii) runAnalysis()
*********************************************************************************************
*********************************************************************************************
Step 4: run the merging on Grid (only if runMode="full")
i) once the jobs are DONE, set runMode="terminate" (all the other settings remain the same)
ii) .L runAnalysis_treeCreator.C
iii) runAnalysis()
iiii) repeat ii) and iii) for each merging stage (3)
*********************************************************************************************

