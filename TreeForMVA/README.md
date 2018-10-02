Instructions to run HFTreeCreator task



*********************************************************************************************
Step 1: Generate the file containing the cut objects for D0, Ds, Dplus

1) cd cutfile (directory where the macros are stored)
2) aliroot / .L makeCutsTreeCreator.C / makeCutsTreeCreator()

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

    1) download one AliAOD.root and the corresponding AliAOD.VertexingHF.root files
    2) set flag runLocal=kTRUE
    3) set pathToLocalAODfiles equal to the local path where the AOD files are stored
    4) set isRunOnMC=kTRUE or =kFALSE to run either on Monte Carlo or data

- to run in test mode

    1) set flag runLocal=kFALSE
    2) set flag runGridTest=kTRUE;
    3) set gridDataDir and gridDataPattern:
       - to run on data: gridDataDir="/alice/data/2015/LHC15o/"; gridDataPattern="/pass1/AOD194";
       - to run on MC: gridDataDir="/alice/sim/2016/LHC16i2a/"; gridDataPattern="/AOD198";
    4) define the run list: nruns = n; runlist[nruns] = {runNumber, runNumber, ...};
    5) set isRunOnMC=kTRUE or =kFALSE to run either on Monte Carlo or data

- to run in full mode

    1) set flag runLocal=kFALSE
    2) set flag runGridTest=kFALSE;
    3) set runMode="full"
    4) set gridDataDir and gridDataPattern:
      - to run on data: gridDataDir="/alice/data/2015/LHC15o/"; gridDataPattern="/pass1/AOD194";
      - to run on MC: gridDataDir="/alice/sim/2016/LHC16i2a/"; gridDataPattern="/AOD198";
    5) set aliPhysVersion
    6) set gridWorkingDir="testNtupleCreator"; and gridOutputDir="output";
    7) define the run list: nruns = n; runlist[nruns] = {runNumber0, runNumber1, ...};
    8) set isRunOnMC=kTRUE or =kFALSE to run either on Monte Carlo or data


Common to the 3 running modes:

Set the cut file to be used by the task: cutFile="./cutfile/D0DsDplusCuts.root";
*********************************************************************************************
*********************************************************************************************
Step 3: run the task

1) .L runAnalysis_treeCreator.C
2) runAnalysis()
*********************************************************************************************
*********************************************************************************************
Step 4: run the merging on Grid (only if runMode="full")

1) once the jobs are DONE, set runMode="terminate" (all the other settings remain the same)
2) .L runAnalysis_treeCreator.C
3) runAnalysis()
4) repeat 2) and 3) for each merging stage (3)
*********************************************************************************************

