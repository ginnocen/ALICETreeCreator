class AliAnalysisGrid;
class AliAnalysisAlien;

//_________________________________________________________________________________________________________
//data runs
const Int_t runs17p[] = {282343, 282342, 282341, 282340, 282314, 282313, 282312, 282309, 282307, 282306, 282305, 282304, 282303, 282302, 282247, 282230, 282229, 282227, 282224, 282206, 282189, 282147, 282146, 282127, 282126, 282125, 282123, 282122, 282120, 282119, 282118, 282099, 282098, 282078, 282051, 282050, 282031, 282030, 282025, 282021, 282016, 282008};
const Int_t runs17q[] = {282367, 282366, 282365};

//MC runs
const Int_t runsMCen[] = {282343};//, 282342, 282341, 282340, 282314, 282313, 282312, 282309, 282307, 282306, 282305, 282304, 282303, 282302, 282247, 282230, 282229, 282227, 282224, 282206, 282189, 282147, 282146, 282127, 282126, 282125, 282123, 282122, 282120, 282119, 282118, 282099, 282098, 282078, 282051, 282050, 282031, 282030, 282025, 282021, 282016, 282008, 282367, 282366, 282365};

//_________________________________________________________________________________________________________
//prototypes
void RunAnalysisAODVertexingHFDs_TreeMVA_ROOT5(TString pluginmode = "full", Bool_t fIsMC=kFALSE, TString period = "LHC17p"/*"LHC18a4a2_fast"*/, TString recopass = "pass1_FAST"/*""*/);
AliAnalysisGrid* CreateAlienHandler(TString pluginmode, TString testfileslistWithPlugin, Bool_t fIsMC, TString period, TString recopass);

//_________________________________________________________________________________________________________
//impelementation
void RunAnalysisAODVertexingHFDs_TreeMVA_ROOT5(TString pluginmode, Bool_t fIsMC, TString period, TString recopass) {
  
  gSystem->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_PHYSICS/include -g");

  TString trainName = "D2H";
  TString analysisMode = "grid"; // "local", "grid"
  TString inputMode    = "list"; // "list", "xml", or "dataset"
  Long64_t nentries=123567890,firstentry=0;
  TString testfileslistWithPlugin="input_data.txt";

  if(analysisMode=="grid") {
    // Connect to AliEn
    TGrid::Connect("alien://");
  }

  // AliRoot libraries
  gSystem->Load("libGui");
  gSystem->Load("libRAWDatabase");
  gSystem->Load("libCDB");
  gSystem->Load("libSTEER");
  gSystem->Load("libTRDbase");
  gSystem->Load("libPWGTRD");
  
  // Create Alien plugin, if requested
  AliAnalysisGrid *alienHandler = CreateAlienHandler(pluginmode,testfileslistWithPlugin,fIsMC,period,recopass);
  if(!alienHandler) return;

  //-------------------------------------------------------------------
  // Prepare input
  TChain *chainAOD = 0;

  // Create the analysis manager
  AliAnalysisManager *mgr  = new AliAnalysisManager("My Manager","My Manager");
  mgr->SetDebugLevel(10);
  // Connect plug-in to the analysis manager
  mgr->SetGridHandler(alienHandler);

  // Input
  AliAODInputHandler *inputHandler = new AliAODInputHandler("handler","handler for D2H");
  mgr->SetInputEventHandler(inputHandler);

  //-------------------------------------------------------------------
  // Analysis tasks (wagons of the train)

  //PID response task
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
  AliAnalysisTaskSE *setupTask = AddTaskPIDResponse(fIsMC,fIsMC);

  //Physics selection task
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
  if(pluginmode!="terminate") AliPhysicsSelectionTask* physseltask = AddTaskPhysicsSelection(fIsMC);

  //Improver task (only in case of MC --> correct distribution with smearing)
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskImproveITS.C");
  AliAnalysisTaskSEImproveITS *imptask = 0x0;
  if(period.Contains("fast")) {imptask = AddTaskImproveITS(kFALSE,"LHC17pq_G3MC_fast","central",0);}
  else if(period.Contains("cent")) {imptask = AddTaskImproveITS(kFALSE,"LHC17pq_G3MC_cent","central",0);}
  
  //Ds task
  gROOT->LoadMacro("AliHFTreeHandler.cxx+g");
  gROOT->LoadMacro("AliHFTreeHandlerDstoKKpi.cxx+g");
  gROOT->LoadMacro("AliAnalysisTaskSEDs_TreeMVA.cxx+g");
  gROOT->LoadMacro("AddTaskDs_TreeMVA.C");
  TString cutfilename_noPID="alien:///alice/cern.ch/user/f/fgrosa/DstoKKpiCuts_pp_loose_noPID_kINT7.root";
  TString suffix_noPID_MVA="_MB_Loose_noPID_MVA_kINT7";
  Int_t sparseopt=0; //enables THnSparses
  Int_t treeopt=1; //enables TTree
  AliAnalysisTaskSEDs_TreeMVA* taskDs_noPID_MVA = AddTaskDs_TreeMVA(0,treeopt,sparseopt,sparseopt,fIsMC,cutfilename_noPID.Data(),suffix_noPID_MVA.Data(),0,1,0,0,0,0,0,0);
  if(fIsMC) taskDs_noPID_MVA->SetWriteOnlySignalInTree(); // writes only signal
  
  //-------------------------------------------------------------------
  // Run the analysis
  if(chainAOD) printf("CHAIN HAS %d ENTRIES\n",(Int_t)chainAOD->GetEntries());

  if(!mgr->InitAnalysis()) return;
  mgr->PrintStatus();
  mgr->StartAnalysis(analysisMode.Data(),chainAOD,nentries,firstentry);

  return;
}

//_____________________________________________________________________________
//
AliAnalysisGrid* CreateAlienHandler(TString pluginmode, TString testfileslistWithPlugin, Bool_t fIsMC, TString period, TString recopass) {
  
  // Check if user has a valid token, otherwise make one. This has limitations.
  // One can always follow the standard procedure of calling alien-token-init then
   AliAnalysisAlien *plugin = new AliAnalysisAlien();
   // Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
   plugin->SetRunMode(pluginmode.Data());
   plugin->SetUser("fgrosa");
   // Set versions of used packages
   plugin->SetAPIVersion("V1.1x");
   plugin->SetAliPhysicsVersion("vAN-20181004-1"); //if you want ROOT6: vAN-20180918_ROOT6-1
   plugin->SetNtestFiles(1);
   plugin->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_PHYSICS/include -g");

   // Declare input data to be processed.
   // Set data file list to test on local mode
   plugin->SetFileForTestMode(testfileslistWithPlugin.Data());

   // Set data search pattern for DATA
   if(!fIsMC) {
     plugin->SetGridDataDir(Form("/alice/data/2017/%s",period.Data())); // specify LHC period
     plugin->SetDataPattern(Form("%s/AOD/*/AliAOD.root",recopass.Data())); // specify reco pass and AOD set
   }
   else {
     plugin->SetGridDataDir(Form("/alice/sim/2018/%s",period.Data())); // specify LHC period
     plugin->SetDataPattern("AOD/*/AliAOD.root"); // specify reco pass and AOD set
   }

  TString friendchainname = "./AliAOD.VertexingHF.root";
   plugin->SetFriendChainName(friendchainname.Data());
   // Adds only the good runs from the Monalisa Run Condition Table
   // More than one period can be added but the period name has to be removed from GridDataDir (to be tested)
    if(!fIsMC) {
      plugin->SetRunPrefix("000"); // for data only
      if(period.Contains("17p")) {
        Int_t nruns = sizeof(runs17p)/sizeof(runs17p[0]);
        for(Int_t iRun=0; iRun<nruns; iRun++) {
          plugin->AddRunNumber(runs17p[iRun]);
        }
        plugin->SetNrunsPerMaster(nruns);
      }
      else if(period.Contains("17q")) {
        Int_t nruns = sizeof(runs17q)/sizeof(runs17q[0]);
        for(Int_t iRun=0; iRun<nruns; iRun++) {
          plugin->AddRunNumber(runs17q[iRun]);
        }
        plugin->SetNrunsPerMaster(nruns);
      }
    }
    else {
      Int_t nruns = sizeof(runsMCen)/sizeof(runsMCen[0]);
      for(Int_t iRun=0; iRun<nruns; iRun++) {
        plugin->AddRunNumber(runsMCen[iRun]);
      }
      plugin->SetNrunsPerMaster(nruns);
    }

   // Define alien work directory where all files will be copied. Relative to alien $HOME.
   plugin->SetGridWorkingDir(Form("Ds_pp_5TeV_%s_%s",period.Data(),recopass.Data()));
   // Name of executable
   plugin->SetExecutable(Form("Ds_pp_5TeV_%s_%s.sh",period.Data(),recopass.Data()));
   // Declare alien output directory. Relative to working directory.
   plugin->SetGridOutputDir("output");
   // Declare the analysis source files names separated by blancs. To be compiled runtime
   gROOT->ProcessLine(".L AliHFTreeHandler.cxx++g");
   gROOT->ProcessLine(".L AliHFTreeHandlerDstoKKpi.cxx++g");
   gROOT->ProcessLine(".L AliAnalysisTaskSEDs_TreeMVA.cxx+g");
   plugin->SetAnalysisSource("AliHFTreeHandler.cxx AliHFTreeHandlerDstoKKpi.cxx AliAnalysisTaskSEDs_TreeMVA.cxx");

   // Declare all libraries (other than the default ones for the framework. These will be
   // loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.
   plugin->SetAdditionalLibs("libPWGflowBase.so libPWGflowTasks.so libPWGHFbase.so libPWGHFvertexingHF.so AliHFTreeHandler.h AliHFTreeHandler.cxx AliHFTreeHandlerDstoKKpi.h AliHFTreeHandlerDstoKKpi.cxx AliAnalysisTaskSEDs_TreeMVA.h AliAnalysisTaskSEDs_TreeMVA.cxx");

   plugin->SetDefaultOutputs(kTRUE);
   // merging via jdl
   plugin->SetMergeViaJDL(kFALSE);
   plugin->SetOneStageMerging(kFALSE);
   plugin->SetMaxMergeStages(3);
   plugin->SetSplitMaxInputFileNumber(8);
   plugin->SetTTL(30000);

   // Optionally set a name for the generated analysis macro (default MyAnalysis.C)
   plugin->SetAnalysisMacro("AnalysisHF.C");
   // Optionally set maximum number of input files/subjob (default 100, put 0 to ignore)
   // Optionally modify the name of the generated JDL (default analysis.jdl)
   plugin->SetJDLName("TaskHF.jdl");

   return plugin;
}
