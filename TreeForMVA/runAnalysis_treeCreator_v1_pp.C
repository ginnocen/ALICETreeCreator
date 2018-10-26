#if !defined (__CINT__) || defined (__CLING__)
#include "AliAnalysisAlien.h"
#include "AliAnalysisManager.h"
#include "AliAODInputHandler.h"

#include "AliPhysicsSelectionTask.h"
#include "AliMultSelectionTask.h"
#include "AliAnalysisTaskPIDResponse.h"

#include "AliAnalysisTaskSEDs.h"
#include "AliAnalysisTaskSED0Mass.h"

#include "AliAnalysisTaskSEHFTreeCreator_v1.h"
#endif

//SETTINGS
//************************************

Bool_t runLocal=kFALSE;                                  // flag to run locally on AliAOD.root + AliAOD.VertexingHF.root
TString pathToLocalAODfiles="../AODFiles";               // path to find AOD files when running locally
Bool_t runGridTest=kFALSE;                               // flag to run a grid test: kTRUE (+runLocal=kFALSE). To run job on GRID: runGridTest=kFALSE, runLocal=kFALSE
TString runMode="full";                             // sets the run grid mode: "full", "terminate"

TString aliPhysVersion="vAN-20181021-1";

Bool_t isRunOnMC=kFALSE;                                 // set to kTRUE to run on Mone Carlo and uncomment/comment accordingly the following lines about paths on Alien
//paths on Alien
// Monte Carlo
//TString gridDataDir="/alice/sim/2018/LHC18a4a2_fast/";
//TString gridDataPattern="AOD";
// Data
TString gridDataDir="/alice/data/2017/LHC17p/";
TString gridDataPattern="/pass1_FAST/AOD";


// Alien output directory
TString gridWorkingDir="testNtupleCreator_LHC17p_FAST_run282343";
TString gridOutputDir="output";

//run numbers
const Int_t nruns = 1;
Int_t runlist[nruns] = {282343};// 282342, 282341, 282340, 282314, 282313, 282312, 282309, 282307, 282306, 282305, 282304, 282303, 282302, 282247, 282230, 282229, 282227, 282224, 282206, 282189, 282147, 282146, 282127, 282126, 282125, 282123, 282122, 282120, 282119, 282118, 282099, 282098, 282078, 282051, 282050, 282031, 282025, 282021, 282016, 282008, 282367, 282366, 282365};

//Task configuration
TString cutFile="./cutfile/D0DsDplusCuts_pp.root";          // file containing the cuts for the different mesons
  														 // to generate the cut file: 1) move to cutfile directory
  														 //                           2) .L makeCutsTreeCreator.C
  														 //                           3) makeCutsTreeCreator();
  														 // to run with ROOT5/6 generate the cut file using AliPhysics built on ROOT5/6
//************************************


void runAnalysis()
{
    // set if you want to run the analysis locally (kTRUE), or on grid (kFALSE)
    Bool_t local = runLocal;
    // if you run on grid, specify test mode (kTRUE) or full grid model (kFALSE)
    Bool_t gridTest = runGridTest;

    // since we will compile a class, tell root where to look for headers
#if !defined (__CINT__) || defined (__CLING__)
    gInterpreter->ProcessLine(".include $ROOTSYS/include");
    gInterpreter->ProcessLine(".include $ALICE_ROOT/include");
#else
    gROOT->ProcessLine(".include $ROOTSYS/include");
    gROOT->ProcessLine(".include $ALICE_ROOT/include");
#endif

    // create the analysis manager
    AliAnalysisManager *mgr = new AliAnalysisManager("AnalysisTaskExample");
    AliAODInputHandler *aodH = new AliAODInputHandler();
    mgr->SetInputEventHandler(aodH);


    // compile the class and load the add task macro
    // here we have to differentiate between using the just-in-time compiler
    // from root6, or the interpreter of root5
#if !defined (__CINT__) || defined (__CLING__)

    AliPhysicsSelectionTask *physseltask = reinterpret_cast<AliPhysicsSelectionTask *>(gInterpreter->ProcessLine(Form(".x %s (%d)", gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C"),isRunOnMC)));

    AliAnalysisTaskPIDResponse *pidResp = reinterpret_cast<AliAnalysisTaskPIDResponse *>(gInterpreter->ProcessLine(Form(".x %s (%d)", gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C"),isRunOnMC)));

//    AliMultSelectionTask *multSel = reinterpret_cast<AliMultSelectionTask *>(gInterpreter->ProcessLine(Form(".x %s", gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C"))));
//    //multSel->SetAlternateOADBforEstimators("LHC15o-DefaultMC-HIJING");

    gInterpreter->LoadMacro("AliHFTreeHandler.cxx++g");
    gInterpreter->LoadMacro("AliHFTreeHandlerD0toKpi.cxx++g");
    gInterpreter->LoadMacro("AliHFTreeHandlerDplustoKpipi.cxx++g");
    gInterpreter->LoadMacro("AliHFTreeHandlerDstoKKpi.cxx++g");
    gInterpreter->LoadMacro("AliAnalysisTaskSEHFTreeCreator_v1.cxx++g");
    AliAnalysisTaskSEHFTreeCreator_v1 *task = reinterpret_cast<AliAnalysisTaskSEHFTreeCreator_v1*>(gInterpreter->ProcessLine(Form(".x %s (%d,%d,\"%s\",\"%s\",%d,%d,%d,%d,%d)",gSystem->ExpandPathName("AddTaskHFTreeCreator_v1.C"),isRunOnMC, 0, "HFTreeCreator", cutFile.Data(),1,kFALSE,1,1,1)));

#else

    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
    AliPhysicsSelectionTask *physSelTask= AddTaskPhysicsSelection(isRunOnMC);

    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
    AliAnalysisTaskPIDResponse *pidResp = AddTaskPIDResponse(isRunOnMC);

//    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
//    AliMultSelectionTask *multSel = AddTaskMultSelection();
    //multSel->SetAlternateOADBforEstimators("LHC15o-DefaultMC-HIJING");

    gROOT->LoadMacro("AliHFTreeHandler.cxx++g");
    gROOT->LoadMacro("AliHFTreeHandlerD0toKpi.cxx++g");
    gROOT->LoadMacro("AliHFTreeHandlerDplustoKpipi.cxx++g");
    gROOT->LoadMacro("AliHFTreeHandlerDstoKKpi.cxx++g");
    gROOT->LoadMacro("AliAnalysisTaskSEHFTreeCreator_v1.cxx++g");
    gROOT->LoadMacro("AddTaskHFTreeCreator_v1.C");
    AliAnalysisTaskSEHFTreeCreator_v1 *task = AddTaskHFTreeCreator_v1(isRunOnMC, 0, "HFTreeCreator", cutFile.Data(),1,kFALSE,1,1,1);

#endif

    if(!mgr->InitAnalysis()) return;
    mgr->SetDebugLevel(2);
    mgr->PrintStatus();
    mgr->SetUseProgressBar(1, 25);

    if(local) {

        // if you want to run locally, we need to define some input
        TChain* chainAOD = new TChain("aodTree");
        TChain *chainAODfriend = new TChain("aodTree");

        // add a few files to the chain (change this so that your local files are added)
        chainAOD->Add(Form("%s/AliAOD.root",pathToLocalAODfiles.Data()));
        chainAODfriend->Add(Form("%s/AliAOD.VertexingHF.root",pathToLocalAODfiles.Data()));

        chainAOD->AddFriend(chainAODfriend);


        // start the analysis locally, reading the events from the tchain
        mgr->StartAnalysis("local", chainAOD);



    } else {

        // if we want to run on grid, we create and configure the plugin
        AliAnalysisAlien *alienHandler = new AliAnalysisAlien();

        // also specify the include (header) paths on grid
        alienHandler->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");

        // make sure your source files get copied to grid
        //alienHandler->SetAdditionalLibs("AliHFCutOptTreeHandler.cxx AliHFCutOptTreeHandler.h");
        alienHandler->SetAdditionalLibs("AliHFTreeHandler.cxx AliHFTreeHandler.h AliHFTreeHandlerD0toKpi.cxx AliHFTreeHandlerD0toKpi.h AliHFTreeHandlerDplustoKpipi.cxx AliHFTreeHandlerDplustoKpipi.h AliHFTreeHandlerDstoKKpi.cxx AliHFTreeHandlerDstoKKpi.h AliAnalysisTaskSEHFTreeCreator_v1.cxx AliAnalysisTaskSEHFTreeCreator_v1.h");
        alienHandler->SetAnalysisSource("AliHFTreeHandler.cxx AliHFTreeHandlerD0toKpi.cxx AliHFTreeHandlerDplustoKpipi.cxx AliHFTreeHandlerDstoKKpi.cxx AliAnalysisTaskSEHFTreeCreator_v1.cxx");

        // select the aliphysics version. all other packages
        // are LOADED AUTOMATICALLY!
        alienHandler->SetAliPhysicsVersion(aliPhysVersion.Data());

        // set the Alien API version
        alienHandler->SetAPIVersion("V1.1x");

        // select the input data
        alienHandler->SetGridDataDir(gridDataDir.Data());
        alienHandler->SetDataPattern(Form("%s/*AliAOD.root",gridDataPattern.Data()));
        alienHandler->SetFriendChainName("AliAOD.VertexingHF.root");

        // MC has no prefix, data has prefix 000
        if(!isRunOnMC)alienHandler->SetRunPrefix("000");


        for(Int_t k=0; k<nruns; k++){
            alienHandler->AddRunNumber(runlist[k]);
        }
        alienHandler->SetNrunsPerMaster(nruns);

        // number of files per subjob
        alienHandler->SetSplitMaxInputFileNumber(1);
        alienHandler->SetExecutable("myTask.sh");

        // specify how many seconds your job may take
        alienHandler->SetTTL(10000);
        alienHandler->SetJDLName("myTask.jdl");

        alienHandler->SetOutputToRunNo(kTRUE);
        alienHandler->SetKeepLogs(kTRUE);

        // merging: run with kTRUE to merge on grid
        // after re-running the jobs in SetRunMode("terminate")
        // (see below) mode, set SetMergeViaJDL(kFALSE)
        // to collect final results
        alienHandler->SetMaxMergeStages(3); //2, 3
        alienHandler->SetMergeViaJDL(kTRUE);

        // define the output folders
        alienHandler->SetGridWorkingDir(gridWorkingDir.Data());
        alienHandler->SetGridOutputDir(gridOutputDir.Data());

        // connect the alien plugin to the manager
        mgr->SetGridHandler(alienHandler);

        if(gridTest) {

            // speficy on how many files you want to run
            alienHandler->SetNtestFiles(1);
            // and launch the analysis
            alienHandler->SetRunMode("test");
            mgr->StartAnalysis("grid");
        }
        else {
            // else launch the full grid analysis
            alienHandler->SetRunMode(runMode.Data()); //terminate
            mgr->StartAnalysis("grid");
        }
    }
}
