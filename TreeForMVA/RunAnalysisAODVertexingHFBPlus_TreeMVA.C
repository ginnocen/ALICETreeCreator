//#R__LOAD_LIBRARY(./AliRDHFCutsBPlustoD0Pi_cxx.so);
//#R__LOAD_LIBRARY(./AliAnalysisTaskSEBPlustoD0Pi_TreeMVA_cxx.so);

// AliAnalysisGrid* CreateAlienHandler(TString pluginmode="test",Bool_t useParFiles=kFALSE, TString testfileslistWithPlugin="");

class AliAnalysisGrid;
class AliAnalysisAlien;
class AliAnalysisManager;
class AliAODInputHandler;
class AliAnalysisTask;
class AliAnalysisTaskPIDResponse;
class AliAnalysisTaskSEBPlustoD0Pi_TreeMVA;
class AliRDHFCutsBPlustoD0Pi;
// class AliAnalysisTaskSEHFQA;

// #include <ctime>
// #include <iostream>

void RunAnalysisAODVertexingHFBPlus_TreeMVA()
{
  //
  // Test macro for AliAnalysisTaskSE's for heavy-flavour candidates
  // It has the structure of an Analysis Train:
  // - in this macro, change things related to running mode
  //   and input preparation 
  // - add your task using a AddTaskXXX macro 
  //
  // A.Dainese, andrea.dainese@lnl.infn.it
  // "grid" mode added by R.Bala, bala@to.infn.it
  //


  gSystem->SetIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS -I$ALICE_PHYSICS/include -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/CONTAINERS -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/TRD -I$ALICE_ROOT/macros -I$ALICE_ROOT/ANALYSIS  -I$ALICE_PHYSICS/OADB -I$ALICE_PHYSICS/PWGHF -I$ALICE_PHYSICS/PWGHF/base -I$ALICE_PHYSICS/PWGHF/vertexingHF -I$ALICE_PHYSICS/PWG/FLOW/Base -I$ALICE_PHYSICS/PWG/FLOW/Tasks -I$ALICE_PHYSICS/PWG -g");
  
  //
  TString trainName = "D2H";
  TString analysisMode = "grid"; // "local", "grid", or "proof"
  TString inputMode    = "list"; // "list", "xml", or "dataset"
  Long64_t nentries=123567890,firstentry=0;
  Bool_t useParFiles=kFALSE;
  Bool_t useAlienPlugin=kTRUE;
  TString pluginmode="test";
  TString testfileslistWithPlugin="";
  Bool_t saveProofToAlien=kFALSE;
  TString proofOutdir = "";
  //TString loadMacroPath="$ALICE_PHYSICS/PWGHF/vertexingHF/macros/";
  TString loadMacroPath="./"; // this is normally needed for CAF

  if(analysisMode=="grid") {
    // Connect to AliEn
    TGrid::Connect("alien://");
  } 


  // AliRoot libraries
  if(analysisMode=="local" || analysisMode=="grid") {
    
#ifdef __CLING__
    gInterpreter->ProcessLine(Form(".x %s/PWGHF/vertexingHF/macros/LoadLibraries.C", gSystem->Getenv("ALICE_PHYSICS")));
#else
    gROOT->LoadMacro(gSystem->ExpandPathName("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/LoadLibraries.C"));
    LoadLibraries(useParFiles);
#endif

    gSystem->Load("libGui");
    gSystem->Load("libRAWDatabase");
    gSystem->Load("libCDB");
    gSystem->Load("libSTEER");
    gSystem->Load("libTRDbase");
    gSystem->Load("libPWGTRD");
  } //"proof" if-statement deleted because incompatible with ROOT6


  // Create Alien plugin, if requested
  AliAnalysisGrid *alienHandler;
  if(useAlienPlugin) {  
    //    if(analysisMode!="grid") {printf("Analysis mode must be grid, to use alien plugin\n"); return;}
    alienHandler = CreateAlienHandler(pluginmode,useParFiles,testfileslistWithPlugin);  
    if(!alienHandler) return;
  }
  alienHandler->SetMergeExcludes("EventStat_temp.root");


  //-------------------------------------------------------------------
  // Prepare input
  TChain *chainAOD = 0;
  TString dataset; // for proof

  if(!useAlienPlugin) {
    TString makeAODInputChain="../MakeAODInputChain.C"; makeAODInputChain.Prepend(loadMacroPath.Data());
    if(inputMode=="list") {
      // Local files
#ifdef __CLING__
      
       chainAOD = reinterpret_cast<TChain *>(gInterpreter->ProcessLine(Form(".x %s/PWGHF/vertexingHF/macros/MakeAODInputChain.C", gSystem->Getenv("ALICE_PHYSICS"))));
#else
       gROOT->LoadMacro(gSystem->ExpandPathName("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/MakeAODInputChain.C"));
       chainAOD = MakeAODInputChain();
#endif
      printf("ENTRIES %d\n",(Int_t)chainAOD->GetEntries());
    } else if(inputMode=="xml") {
      // xml
#ifdef __CLING__
        chainAOD = reinterpret_cast<TChain *>(gInterpreter->ProcessLine(Form(".x %s/PWGHF/vertexingHF/macros/MakeAODInputChain.C(\"collection_aod.xml\",\"collection_aodHF.xml\")", gSystem->Getenv("ALICE_PHYSICS"))));
#else
        gROOT->LoadMacro(gSystem->ExpandPathName("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/MakeAODInputChain.C"));
        chainAOD = MakeAODInputChain("collection_aod.xml","collection_aodHF.xml");
#endif
    } else if(inputMode=="dataset") {
      // CAF dataset
      //gProof->ShowDataSets();
      dataset="/ITS/dainesea/AODVertexingHF_LHC08x_180100";
    }
  }

  // Create the analysis manager
  AliAnalysisManager *mgr  = new AliAnalysisManager("My Manager","My Manager");
  mgr->SetDebugLevel(1);
  // Connect plug-in to the analysis manager
  if(useAlienPlugin) mgr->SetGridHandler(alienHandler);

  // Input
  AliAODInputHandler *inputHandler = new AliAODInputHandler("handler","handler for D2H");
  //"proof" if-statement deleted because incompatible with ROOT6
  mgr->SetInputEventHandler(inputHandler);
  //-------------------------------------------------------------------

  
  //-------------------------------------------------------------------
  // Analysis tasks (wagons of the train)   
  //
  // First add the task for the PID response setting
#ifdef __CLING__
  gInterpreter->ProcessLine(Form(".x %s/OADB/macros/AddTaskPhysicsSelection.C(kTRUE,kTRUE)", gSystem->Getenv("ALICE_PHYSICS")));
#else
    gROOT->LoadMacro(gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C"));
    AddTaskPhysicsSelection(kFALSE,kTRUE);
#endif
    
#ifdef __CLING__
    AliAnalysisTask *setupTask = reinterpret_cast<AliAnalysisTask *>(gInterpreter->ProcessLine(Form(".x %s/ANALYSIS/macros/AddTaskPIDResponse.C(kFALSE,kTRUE,kTRUE,1)", gSystem->Getenv("ALICE_ROOT"))));
#else
    gROOT->LoadMacro(gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C"));
    AliAnalysisTask *setupTask = AddTaskPIDResponse(kFALSE,kTRUE);
#endif
    
    
#ifdef __CLING__

    // gInterpreter->ProcessLine(".x AliRDHFCutsBPlustoD0Pi.cxx+");
    // gInterpreter->ProcessLine(".x AliAnalysisTaskSEBPlustoD0Pi_TreeMVA.cxx+");
    // AliAnalysisTaskSEBPlustoD0Pi_TreeMVA *BPlusTask = reinterpret_cast<AliAnalysisTaskSEBPlustoD0Pi_TreeMVA *>(gInterpreter->ProcessLine(".x ./AddTaskBPlustoD0Pi.C(\"./BPlustoDStarPiCuts.root\",\"doremalen\",kTRUE, kFALSE, kTRUE, kTRUE, kFALSE)"));
#else

    // gROOT->LoadMacro("./AliRDHFCutsBPlustoD0Pi.cxx++"); 
    gROOT->LoadMacro("./AliHFCutOptTreeHandler.cxx+g");
    gROOT->LoadMacro("./AliAnalysisTaskSEBPlustoD0Pi_TreeMVA.cxx++");
    gROOT->LoadMacro("./AddTaskBPlustoD0Pi_TreeMVA.C");
    AliAnalysisTaskSEBPlustoD0Pi_TreeMVA *BPlusTask = AddTaskBPlustoD0Pi_TreeMVA(
     "./BPlustoD0PiCuts.root","doremalen",kFALSE, 0, kTRUE, kTRUE, kTRUE, kFALSE, kTRUE, 0, 0, 0, 0, 0.125,kFALSE,1,1);//3, 60);

  // AliAnalysisTaskSEHFQA * mytask = AddTaskHFQA( AliAnalysisTaskSEHFQA::kD0toKpi,"./d03.root",kTRUE, kFALSE, 1, "_0100", kTRUE, kTRUE, kTRUE, kTRUE, kFALSE, kTRUE);
#endif
    //-------------------------------------------------------------------
    
    //
    // Run the analysis
    //
    if(chainAOD) printf("CHAIN HAS %d ENTRIES\n",(Int_t)chainAOD->GetEntries());
    
    if(!mgr->InitAnalysis()) return;
    mgr->PrintStatus();
    if(analysisMode=="grid" && !useAlienPlugin) analysisMode="local";
    if(analysisMode!="proof") {
        mgr->StartAnalysis(analysisMode.Data(),chainAOD,nentries,firstentry);
    } else {
        // proof
        mgr->StartAnalysis(analysisMode.Data(),dataset.Data(),nentries,firstentry);
    }
    
    return;
}
//_____________________________________________________________________________
//
AliAnalysisGrid* CreateAlienHandler(TString pluginmode="test",Bool_t useParFiles=kFALSE, TString testfileslistWithPlugin="")
{
    // Check if user has a valid token, otherwise make one. This has limitations.
    // One can always follow the standard procedure of calling alien-token-init then
    //   source /tmp/gclient_env_$UID in the current shell.
    AliAnalysisAlien *plugin = new AliAnalysisAlien();
    // Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
    plugin->SetRunMode(pluginmode.Data());
    plugin->SetUser("lvandore");
    // Set versions of used packages
    plugin->SetAPIVersion("V1.1x");
    // plugin->SetROOTVersion();
    // plugin->SetROOTVersion("v5-34-30-alice7-11");
    // plugin->SetAliROOTVersion("v5-08-00-1");
    plugin->SetAliPhysicsVersion("vAN-20180618-1");
    //plugin->SetAliPhysicsVersion("vAN-20171101-1");
    plugin->SetNtestFiles(1);
    gROOT->LoadMacro("./AddGoodRuns.C");
    
    // Declare input data to be processed.
    //************************************************
    // Set data file list to test on local mode
    //************************************************
    plugin->SetFileForTestMode(testfileslistWithPlugin.Data());
    
    //************************************************
    // Set data search pattern for DATA
    //************************************************
    // Method 1: To create automatically xml through plugin
    
    //plugin->SetGridDataDir("/alice/data/2015/LHC15n/");
    //plugin->SetDataPattern("/pass2/AOD183/*AliAOD.root"); // specify reco pass and AOD set
    // plugin->SetGridDataDir("/alice/sim/2018/LHC18a4a2_cent/");
    // plugin->SetDataPattern("/AOD/*AliAOD.root"); // specify reco pass and AOD set
    // plugin->SetFriendChainName("./AliAOD.VertexingHF.root");
    
    // // OR plugin->SetFriendChainName("deltas/AliAOD.VertexingHF.root");
    // // Adds only the good runs from the Monalisa Run Condition Table
    // // More than one period can be added but the period name has to be removed from GridDataDir (to be tested)
    // Int_t totruns=0;
    
    // Int_t nruns=1;
    // Int_t ngoodruns = 0;
    // Int_t runlist[1]={282341};
    // for(Int_t k=0;k<nruns;k++){
    //     plugin->AddRunNumber(runlist[k]);
    //     ngoodruns++;
    // }
    // plugin->SetNrunsPerMaster(ngoodruns);

    //LHC16d2
//     plugin->SetGridDataDir("/alice/sim/2018/LHC18e1/"); // specify LHC period
//     plugin->SetDataPattern("/AOD/*/AliAOD.root"); // specify reco pass and AOD set
//     plugin->SetFriendChainName("./AliAOD.VertexingHF.root");
//     Int_t totruns=151;
//     // totruns += AddGoodRuns(plugin,"LHC14j5_new","LHC14j5_new"); // specify LHC period
//     // totruns += AddGoodRuns(plugin,"LHC14j5_new_onerun","LHC14j5_new_onerun"); // specify LHC period

//     Int_t  runlist[151]= {244918, 244975, 244980, 244982, 244983, 245061, 245064, 245066, 245068, 245145, 245146, 245148, 245151, 245152, 245231, 245232, 245259, 245343, 245345, 245346, 245347, 245349, 245353, 245396, 245397, 245401, 245407, 245409, 245410, 245411, 245439, 245441, 245446, 245450, 245452, 245454, 245496, 245497, 245501, 245504, 245505, 245507, 245535, 245540, 245542, 245543, 245544, 245545, 245554, 245683, 245692, 245700, 245702, 245705, 245729, 245731, 245738, 245752, 245759, 245766, 245775, 245785, 245793, 245829, 245831, 245833, 245923, 245949, 245952, 245954, 245963, 246001, 246003, 246012, 246036, 246037, 246042, 246048, 246049, 246052, 246053, 246087, 246089, 246113, 246115, 246148, 246151, 246152, 246153, 246178, 246180, 246181, 246182, 246185, 246217, 246222, 246225, 246271, 246272, 246275, 246276, 246390, 246391, 246392, 246424, 246428, 246431, 246434, 246487, 246488, 246493, 246495, 246553, 246575, 246583, 246648, 246675, 246676, 246750, 246751, 246757, 246758, 246759, 246760, 246763, 246765, 246766, 246804, 246805, 246807, 246808, 246809, 246810, 246844, 246845, 246846, 246847, 246851, 246865, 246867, 246870, 246871, 246928, 246945, 246948, 246980, 246982, 246984, 246989, 246991, 246994
// };

//     for(Int_t k=0;k<totruns;k++){
//         plugin->AddRunNumber(runlist[k]);
//     }


  // plugin->SetGridDataDir("/alice/sim/2015/LHC15a2a"); // specify LHC period
  //  plugin->SetDataPattern("AOD/*AliAOD.root"); // specify reco pass and AOD set
  //  plugin->SetFriendChainName("./AliAOD.VertexingHF.root");
  //  Int_t totruns=0;
  //  totruns += AddGoodRuns(plugin,"LHC15a2a","LHC15a2a"); // specify LHC period
   // totruns += AddGoodRuns(plugin,"LHC15a2a_one_run","LHC15a2a_one_run"); // specify LHC period
   // totruns += AddGoodRuns(plugin,"LHC15a2a_fifty_run",


   // LHC14j5_new
   // plugin->SetGridDataDir("/alice/sim/2014/LHC14j5_new"); // specify LHC period
   // plugin->SetDataPattern("/*AliAOD.root"); // specify reco pass and AOD set
   // plugin->SetFriendChainName("./AliAOD.VertexingHF.root");
   // Int_t totruns=0;
   // // totruns += AddGoodRuns(plugin,"LHC14j5_new","LHC14j5_new"); // specify LHC period
   // totruns += AddGoodRuns(plugin,"LHC14j5_new_onerun","LHC14j5_new_onerun"); // specify LHC period

   plugin->SetGridDataDir("/alice/data/2010/LHC10c"); // specify LHC period
   plugin->SetDataPattern("/AOD/*AliAOD.root"); // specify reco pass and AOD set
   plugin->SetFriendChainName("./AliAOD.VertexingHF.root");
   // OR plugin->SetFriendChainName("deltas/AliAOD.VertexingHF.root");
   // Adds only the good runs from the Monalisa Run Condition Table
   // More than one period can be added but the period name has to be removed from GridDataDir (to be tested)
   Int_t totruns=0;
   // totruns += AddGoodRuns(plugin,"LHC10b"); // specify LHC period
   totruns += AddGoodRuns(plugin,"LHC10c"); // specify LHC period
   // totruns += AddGoodRuns(plugin,"LHC10d"); // specify LHC period
   // totruns += AddGoodRuns(plugin,"LHC10e_pass4"); // specify LHC period
   plugin->SetNrunsPerMaster(totruns);

    //LHC14j5_new
 //    plugin->SetGridDataDir("/alice/sim/2013/LHC13d19"); // specify LHC period
 //    plugin->SetDataPattern("/*AliAOD.root"); // specify reco pass and AOD set
 //    plugin->SetFriendChainName("./AliAOD.VertexingHF.root");
 //    Int_t totruns=10;
 // //    // totruns += AddGoodRuns(plugin,"LHC14j5_new","LHC14j5_new"); // specify LHC period
 //    totruns += AddGoodRuns(plugin,"LHC14j5_new_onerun","LHC14j5_new_onerun"); // specify LHC period

  // Int_t  runlist[10]= {
 //       137161, 137162, 137230, 137231, 137232, 137236, 137243, 137366, 137430, 137431};

 //    for(Int_t k=0;k<totruns;k++){
 //        plugin->AddRunNumber(runlist[k]);
 //    }


 //    plugin->SetGridDataDir("/alice/sim/2018/LHC18d4b"); // specify LHC period
 //    plugin->SetDataPattern("/*AliAOD.root"); // specify reco pass and AOD set
 //    plugin->SetFriendChainName("./AliAOD.VertexingHF.root");
 //    Int_t totruns=1;
 //    // totruns += AddGoodRuns(plugin,"LHC14j5_new","LHC14j5_new"); // specify LHC period
 //    // totruns += AddGoodRuns(plugin,"LHC14j5_new_onerun","LHC14j5_new_onerun"); // specify LHC period

  // Int_t  runlist[1]= {280235};

 //    for(Int_t k=0;k<totruns;k++){
 //        plugin->AddRunNumber(runlist[k]);
 //    }

// new Sim: "/alice/sim/2018/LHC18d4a" -> run=246392
// new Sim: "/alice/sim/2018/LHC18d4b" -> run=280235

    // Method 2: Declare existing data files (e.g xml collections)
    
    // plugin->AddDataFile("/alice/cern.ch/user/r/rbala/000168068_000170593.xml");
    // plugin->SetDataPattern("*AliAOD.root");
    // plugin->SetFriendChainName("./AliAOD.VertexingHF.root");
    
    //************************************************
    // Set data search pattern for MONTECARLO
    //************************************************
    
    // OR plugin->SetFriendChainName("deltas/AliAOD.VertexingHF.root");
    // Adds only the good runs from the Monalisa Run Condition Table
    // More than one period can be added!
    // Int_t totruns=0;
    // totruns += AddGoodRuns(plugin,"LH10d"); // specify LHC period for anchor runs; and the name of the MC production
    // totruns += AddGoodRuns(plugin,"LHC10c","LHC10f7"); // specify LHC period for anchor runs;  and the name of the MC production
    // totruns += AddGoodRuns(plugin,"LHC10d","LHC10f7"); // specify LHC period for anchor runs;  and the name of the MC production
    plugin->SetNrunsPerMaster(1);
    
    //
    // Define alien work directory where all files will be copied. Relative to alien $HOME.
    plugin->SetGridWorkingDir("BPluscommittest");
    // Name of executable
    plugin->SetExecutable("BPluscommittest.sh");
    // Declare alien output directory. Relative to working directory.
    plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
    // plugin->SetGridOutputDir(job_name + "/output_" + job_name + "/000/Stage_1"); 
   // Declare the analysis source files names separated by blancs. To be compiled runtime
   // using ACLiC on the worker nodes.
   plugin->SetAnalysisSource("AliRDHFCutsBPlustoD0Pi.cxx AliAnalysisTaskSEBPlustoD0Pi_TreeMVA.cxx");
   // Declare all libraries (other than the default ones for the framework. These will be
   // loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.
   plugin->SetAdditionalLibs("AliRDHFCutsBPlustoD0Pi.cxx AliRDHFCutsBPlustoD0Pi.h AliAnalysisTaskSEBPlustoD0Pi_TreeMVA.cxx AliAnalysisTaskSEBPlustoD0Pi_TreeMVA.h");
    // use par files
    if(useParFiles) {
        plugin->EnablePackage("STEERBase.par");
        plugin->EnablePackage("ESD.par");
        plugin->EnablePackage("AOD.par");
        plugin->EnablePackage("ANALYSIS.par");
        plugin->EnablePackage("OADB.par");
        plugin->EnablePackage("ANALYSISalice.par");
        plugin->EnablePackage("CORRFW.par");
        plugin->EnablePackage("PWGHFbase.par");
        plugin->EnablePackage("PWGHFvertexingHF.par");
    }
    plugin->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS -I$ALICE_PHYSICS/include -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/CONTAINERS -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/TRD -I$ALICE_ROOT/macros -I$ALICE_ROOT/ANALYSIS  -I$ALICE_PHYSICS/OADB -I$ALICE_PHYSICS/PWGHF -I$ALICE_PHYSICS/PWGHF/base -I$ALICE_PHYSICS/PWGHF/vertexingHF -I$ALICE_PHYSICS/PWG/FLOW/Base -I$ALICE_PHYSICS/PWG/FLOW/Tasks -I$ALICE_PHYSICS/PWG -g");
    plugin->SetMergeExcludes("EventStat_temp.root");
    // plugin->SetNrunsPerMaster(1);
    plugin->SetDefaultOutputs(kTRUE);
    plugin->SetSplitMaxInputFileNumber(40);
    plugin->SetTTL(24.0*60*60);

    // merging via jdl
    plugin->SetMergeViaJDL(kTRUE); 
    plugin->SetOneStageMerging(kFALSE);
    plugin->SetMaxMergeStages(2);
    
    // Optionally set a name for the generated analysis macro (default MyAnalysis.C)
    plugin->SetAnalysisMacro("AnalysisHF.C");
    // Optionally set maximum number of input files/subjob (default 100, put 0 to ignore)
    // Optionally modify the name of the generated JDL (default analysis.jdl)
    plugin->SetJDLName("TaskHF.jdl");
    
    return plugin;
}
