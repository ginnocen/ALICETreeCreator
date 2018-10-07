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



*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
Instructions to use the HFTreeHandler and to define derived classes

AliHFTreeHandler is a mother class for the definition of classes that create trees useful for cut optimisation / MVA studies of HF particles. 

*********************************************************************************************
The main methods are:

1) virtual TTree* BuildTree(TString name, TString title)
2) virtual bool SetVariables(AliAODRecoDecayHF* cand, int masshypo, AliAODPidHF* pidHF)
3) bool FillTree()

The 1) and 2) methods must be implemented in the derived classes and are used to 1) set the branch addresses to the variables that one wants to add to the tree and 2) set the variables that will be used to fill the tree.
The method 1) must be called right after the HFTreeHandler is constructed (and after all the additional branches are enabled) and returns the tree that will be filled. The tree is delete by the HFTreeHandler, so it should not be deleted in the task.
The 2) and 3) methods have to be called for each entry of the tree (first set variables and then fill the tree).

*********************************************************************************************
In the mother class there are two methods that can be useful for the implementation of 1) and 2) methods. In particular:

4) void AddCommonDmesonVarBranches()
5) void AddPidBranches(bool usePionHypo, bool useKaonHypo, bool useProtonHypo, bool useTPC, bool useTOF)

can be used in the implementation of BuildTree in a derived class to set the branch addresses of the common D-meson topological variables and the PID variables for each daughter track, respectively. The number of daughter tracks (fNProngs) has to be setted in the derived class. In addition

6) bool SetPidVars(AliAODTrack* prongtracks[], AliAODPidHF* pidHF, bool usePionHypo, bool useKaonHypo, bool useProtonHypo, bool useTPC, bool useTOF);

Can be used in the implementation of method 2) to set the values of the PID variables. 

*********************************************************************************************
Several options for the PID variables are implemented:

kNoPID --> no PID variables in the tree
kNsigmaPID --> number of sigma for each enabled detector (TPC, TOF) and for each particle hypothesis enabled (pi,K,pr)
kNsigmaPIDchar --> same as kNsigmaPID, but nsigma stored in a char (to save space)
kNsigmaPIDfloatandchar --> both kNsigmaPID and kNsigmaPIDchar together (to test performance of float/char)
kNsigmaCombPID --> combination of number of sigma of TPC and TOF for each particle hypothesis enabled (pi,K,pr)
kNsigmaCombPIDchar  --> same as kNsigmaCombPID, but stored in a char (to save space)
kNsigmaCombPIDfloatandchar --> both kNsigmaCombPID and kNsigmaCombPIDchar together (to test performance of float/char)
kRawPID --> PID signal each enabled detector (TPC --> dE/dx, TOF --> tof)

*********************************************************************************************
Flags to keep the information of the "type" of candidates are stored in a single branch via a bitmap:

kSelected = BIT(0)
kSignal   = BIT(1)
kBkg      = BIT(2)
kPrompt   = BIT(3)
kFD       = BIT(4)
kRefl     = BIT(5)

the first bit can be activated for both data and MC using:

7) void SetIsSelectedStd(bool isselected) 

while the other ones can be enabled in case of MC truth using:

8) void SetCandidateType(bool issignal, bool isbkg, bool isprompt, bool isFD, bool isreflected)

each flag enables one bit (i.e. if issignal=false it is not automatically set isbkg=true). When reading the entries of the tree, each bit can be tested using the following static methods:

9) static bool IsSignal(unsigned char candtype)
10) static bool IsBkg(unsigned char candtype)
11) static bool IsPrompt(unsigned char candtype)
12) static bool IsFD(unsigned char candtype)
13) static bool IsRefl(unsigned char candtype)

To directly project the tree for a particular type of candidates, one can ask directly the bit to be enabled. For example:

- if one wants signal+prompt has to require (cand_type == AliHFTreeHandler::kSignal | AliHFTreeHandler::kPrompt)
- if one wants signal (both prompt and feed-down) has to require ( (cand_type == AliHFTreeHandler::kSignal | AliHFTreeHandler::kPrompt) || (cand_type == AliHFTreeHandler::kSignal | AliHFTreeHandler::kFD) )

*********************************************************************************************
Additional branches can be enabled BEFORE calling AliHFTreeHandler::BuildTree, using:

14) void EnableCentralityBranch(bool enablecent=true)
15) void EnableMaxNormd0MeasMinusExpBranch(bool enablenormd0measminusexp=true) 

and then setting the corresponding variables BEFORE calling AliHFTreeHandler::FillTree, using:

16) void SetCentrality(char centrality)
17) void SetMaxNormd0MeasMinusExp(float normd0measminuexp)

Many more variables can be added both in the mother class (if useful for different particles) or in a dedicated derived class.

*********************************************************************************************
A first version of three derived classes has already been implemented:

1) AliHFTreeHandlerD0toKpi
2) AliHFTreeHandlerDplustoKpipi
3) AliHFTreeHandlerDstoKKpi
