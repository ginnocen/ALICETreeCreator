#if !defined (__CINT__) || defined (__CLING__)
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "AliAnalysisAlien.h"
#include "AliAnalysisManager.h"
#include "AliAODInputHandler.h"

#include "AliPhysicsSelectionTask.h"
#include "AliMultSelectionTask.h"
#include "AliAnalysisTaskPIDResponse.h"
#include "AliAnalysisTaskSECleanupVertexingHF.h"

#include "AliAnalysisTaskSEHFTreeCreator_dev.h"
#endif

using namespace std;
enum system {kpp, kpPb, kPbPb};

//SETTINGS
//************************************

Bool_t runLocal=kFALSE;                                  // flag to run locally on AliAOD.root + AliAOD.VertexingHF.root
TString pathToLocalAODfiles="../AODFiles";               // path to find AOD files when running locally
Bool_t runGridTest=kFALSE;                               // flag to run a grid test: kTRUE (+runLocal=kFALSE). To run job on GRID: runGridTest=kFALSE, runLocal=kFALSE

// Alien output directory
TString gridWorkingDir="testNtupleCreator_LHC16i2a";
TString gridOutputDir="output";

//Task configuration
TString cutFile="./cutfile/D0DsDplusLcBplusCuts.root"; // file containing the cuts for the different mesons
  														  // to generate the cut file: 1) move to cutfile directory
  														  //                           2) .L makeCutsTreeCreator.C
  														  //                           3) makeCutsTreeCreator();
  														  // to run with ROOT5/6 generate the cut file using AliPhysics built on ROOT5/6
//************************************


void runAnalysis(TString configfilename="runAnalysis_config_LHC18a4a2_fast.dat", TString runMode = "full");
Bool_t loadConfigFile(TString configfilename, vector<Int_t> &runs, Bool_t &isRunOnMC, TString &aliPhysVersion, TString &gridDataDir, TString &gridDataPattern, Int_t &System);

void runAnalysis(TString configfilename, TString runMode)
{
    // set if you want to run the analysis locally (kTRUE), or on grid (kFALSE)
    Bool_t local = runLocal;
    // if you run on grid, specify test mode (kTRUE) or full grid model (kFALSE)
    Bool_t gridTest = runGridTest;

    vector<Int_t> runlist;
    Bool_t isRunOnMC;
    Int_t System=-1;
    TString aliPhysVersion = "";
    TString gridDataDir = "";
    TString gridDataPattern = "";
    loadConfigFile(configfilename,runlist,isRunOnMC,aliPhysVersion,gridDataDir,gridDataPattern,System);
    const Int_t nruns = runlist.size();

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
    // from root6, or the Int_terpreter of root5
#if !defined (__CINT__) || defined (__CLING__)

    AliPhysicsSelectionTask *physseltask = reinterpret_cast<AliPhysicsSelectionTask *>(gInterpreter->ProcessLine(Form(".x %s (%d)", gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C"),isRunOnMC)));

    AliAnalysisTaskPIDResponse *pidResp = reinterpret_cast<AliAnalysisTaskPIDResponse *>(gInterpreter->ProcessLine(Form(".x %s (%d)", gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C"),isRunOnMC)));

    if(System==kpPb || System==kPbPb) {
        AliMultSelectionTask *multSel = reinterpret_cast<AliMultSelectionTask *>(gInterpreter->ProcessLine(Form(".x %s", gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C"))));
        if(gridDataDir.Contains("LHC15o") || gridDataDir.Contains("LHC16i2")) multSel->SetAlternateOADBforEstimators("LHC15o-DefaultMC-HIJING");
    }

    gInterpreter->LoadMacro("AliHFTreeHandler_dev.cxx++g");
    gInterpreter->LoadMacro("AliHFTreeHandlerD0toKpi_dev.cxx++g");
    gInterpreter->LoadMacro("AliHFTreeHandlerDplustoKpipi_dev.cxx++g");
    gInterpreter->LoadMacro("AliHFTreeHandlerDstoKKpi_dev.cxx++g");
    gInterpreter->LoadMacro("AliHFTreeHandlerLctopKpi_dev.cxx++g");
    gInterpreter->LoadMacro("AliHFTreeHandlerBplustoD0pi_dev.cxx++g");
    gInterpreter->LoadMacro("AliAnalysisTaskSEHFTreeCreator_dev.cxx++g");
    AliAnalysisTaskSEHFTreeCreator_dev *task = reinterpret_cast<AliAnalysisTaskSEHFTreeCreator_dev*>(gInterpreter->ProcessLine(Form(".x %s (%d,%d,\"%s\",\"%s\",%d,%d,%d,%d,%d,%d,%d,%d)",gSystem->ExpandPathName("AddTaskHFTreeCreator_dev.C"),isRunOnMC, 0, "HFTreeCreator", cutFile.Data(),1,isRunOnMC,isRunOnMC,1,1,1,1,1)));

    if(System==kPbPb) {
        AliAnalysisTaskSECleanupVertexingHF *taskclean =reinterpret_cast<AliAnalysisTaskSECleanupVertexingHF *>(gInterpreter->ProcessLine(Form(".x %s", gSystem->ExpandPathName("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskCleanupVertexingHF.C"))));
    }

#else

    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
    AliPhysicsSelectionTask *physSelTask= AddTaskPhysicsSelection(isRunOnMC);

    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
    AliAnalysisTaskPIDResponse *pidResp = AddTaskPIDResponse(isRunOnMC);

    if(System==kpPb || System==kPbPb) {
        gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
        AliMultSelectionTask *multSel = AddTaskMultSelection();
        if(gridDataDir.Contains("LHC15o") || gridDataDir.Contains("LHC16i2")) multSel->SetAlternateOADBforEstimators("LHC15o-DefaultMC-HIJING");
    }

    gROOT->LoadMacro("AliHFTreeHandler_dev.cxx++g");
    gROOT->LoadMacro("AliHFTreeHandlerD0toKpi_dev.cxx++g");
    gROOT->LoadMacro("AliHFTreeHandlerDplustoKpipi_dev.cxx++g");
    gROOT->LoadMacro("AliHFTreeHandlerDstoKKpi_dev.cxx++g");
    gROOT->LoadMacro("AliHFTreeHandlerLctopKpi_dev.cxx++g");
    gROOT->LoadMacro("AliHFTreeHandlerBplustoD0pi_dev.cxx++g");
    gROOT->LoadMacro("AliAnalysisTaskSEHFTreeCreator_dev.cxx++g");
    gROOT->LoadMacro("AddTaskHFTreeCreator_dev.C");
    AliAnalysisTaskSEHFTreeCreator_dev *task = AddTaskHFTreeCreator_dev(isRunOnMC, 0, "HFTreeCreator", cutFile.Data(),1,isRunOnMC,isRunOnMC,1,1,1,1,1);

    if(System==kPbPb) {
        gROOT->LoadMacro("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskCleanupVertexingHF.C");
        AliAnalysisTaskSECleanupVertexingHF *taskclean = AddTaskCleanupVertexingHF();
    }

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
        alienHandler->SetAdditionalLibs("AliHFTreeHandler_dev.cxx AliHFTreeHandler_dev.h AliHFTreeHandlerD0toKpi_dev.cxx AliHFTreeHandlerD0toKpi_dev.h AliHFTreeHandlerDplustoKpipi_dev.cxx AliHFTreeHandlerDplustoKpipi_dev.h AliHFTreeHandlerDstoKKpi_dev.cxx AliHFTreeHandlerDstoKKpi_dev.h AliHFTreeHandlerBplustoD0pi_dev.cxx AliHFTreeHandlerBplustoD0pi_dev.h AliHFTreeHandlerLctopKpi_dev.cxx AliHFTreeHandlerLctopKpi_dev.h AliAnalysisTaskSEHFTreeCreator_dev.cxx AliAnalysisTaskSEHFTreeCreator_dev.h AliAnalysisTaskSEHFTreeCreator_dev.cxx AliAnalysisTaskSEHFTreeCreator_dev.h");
        alienHandler->SetAnalysisSource("AliHFTreeHandler_dev.cxx AliHFTreeHandlerD0toKpi_dev.cxx AliHFTreeHandlerDplustoKpipi_dev.cxx AliHFTreeHandlerDstoKKpi_dev.cxx AliHFTreeHandlerBplustoD0pi_dev.cxx AliHFTreeHandlerLctopKpi_dev.cxx AliAnalysisTaskSEHFTreeCreator_dev.cxx");

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
        alienHandler->SetMergeViaJDL(kFALSE);

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

Bool_t loadConfigFile(TString configfilename, vector<Int_t> &runs, Bool_t &isRunOnMC, TString &aliPhysVersion, TString &gridDataDir, TString &gridDataPattern, Int_t &System) {
  
    ifstream infile(configfilename.Data());
    if(!infile || !infile.is_open()) {
        cerr<<"Config file not found! Exit."<<endl;
        return kFALSE;
    }
  
    string line;
    TString teststring="#";
    TString strSystem="";
    while(teststring.Contains("#")) {
        getline(infile, line);
        teststring = line;
        strSystem = teststring;
    }
    if(strSystem=="pp") System = kpp;
    else if(strSystem=="pPb") System = kpPb;
    else if(strSystem=="PbPb") System = kPbPb;
    else {
        cerr<<"Wrong colliding system! Exit."<<endl;
        return kFALSE;
    }
    teststring="#";
    while(teststring.Contains("#")) {
        getline(infile, line);
        teststring = line;
        gridDataDir = teststring;
    }
    if(gridDataDir.Contains("sim")) isRunOnMC = kTRUE;
    else isRunOnMC = kFALSE;
    teststring="#";
    while(teststring.Contains("#")) {
        getline(infile, line);
        teststring = line;
        gridDataPattern = line;
    }
    teststring="#";
    while(teststring.Contains("#")) {
        getline(infile, line);
        teststring = line;
        aliPhysVersion = line;
    }

    while(!infile.eof()) {
    
        getline(infile, line);
        teststring = line;
        Int_t run=-1;
        if(!teststring.Contains("#")) {
            stringstream convert(line);
            if( !(convert >> run) ) run = -1;
            runs.push_back(run);
        }
    }
  
    return kTRUE;
}
