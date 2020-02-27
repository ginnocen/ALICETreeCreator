#include "AliAnalysisAlien.h"
#include "AliAnalysisManager.h"
#include "AliAODInputHandler.h"

#include "AliPhysicsSelectionTask.h"
#include "AliMultSelectionTask.h"
#include "AliAnalysisTaskPIDResponse.h"

#include "AliAnalysisTaskSEHFTreeCreator.h"
#include "AliAnalysisTaskSED0Mass.h"

using namespace std;
enum system {kpp, kpPb, kPbPb};

Bool_t runLocal=kTRUE;    
TString pathToLocalAODfiles;
Bool_t runGridTest=kFALSE; 
TString runMode="full";   

TString aliPhysVersion="vAN-20190701_ROOT6-1";
TString gridDataDir;
TString gridDataPattern="/pass1/AOD194";
TString gridWorkingDir="testNtupleCreator";
TString gridOutputDir="output";

const Int_t nruns = 1;
Int_t runlist[nruns] = {246994};


TString cutFile="../TreeForMVA/cutfile/pp/D0DsDplusDstarLcBplusBsLbCuts_pp.root";


void runAnalysis_treeCreator_localserver(Bool_t isRunOnMC=kFALSE)
{
  TGrid::Connect("alien://");
  
  Int_t System=-1;
  if (isRunOnMC == kTRUE) {
    pathToLocalAODfiles = "/data2/highmultppAOD/mc/LHC18f4a";
    gridDataDir="/alice/sim/2018/LHC18f4a";
    System=kpp;
    std::cout<<"Running on MC"<<std::endl;
  }
  if (isRunOnMC == kFALSE){
    pathToLocalAODfiles = "/data/AOD208/AOD208/0009";
    gridDataDir="/alice/data/2018/LHC18f";
    System=kpp;
  }
  Bool_t local = runLocal;
  Bool_t gridTest = runGridTest;
  
  gInterpreter->ProcessLine(".include $ROOTSYS/include");
  gInterpreter->ProcessLine(".include $ALICE_ROOT/include");
  
  AliAnalysisManager *mgr = new AliAnalysisManager("AnalysisTaskExample");
  AliAODInputHandler *aodH = new AliAODInputHandler();
  mgr->SetInputEventHandler(aodH);
  
  
  AliPhysicsSelectionTask *physseltask = reinterpret_cast<AliPhysicsSelectionTask *>(gInterpreter->ProcessLine(Form(".x %s(%d, %d)",
				  gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C"),isRunOnMC, kTRUE)));
  
  AliAnalysisTaskPIDResponse *pidResp = reinterpret_cast<AliAnalysisTaskPIDResponse *>(gInterpreter->ProcessLine(Form(".x %s(%d)",
				  gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C"),isRunOnMC)));
  
  if(System==kpPb || System==kPbPb) {
    AliMultSelectionTask *multSel = reinterpret_cast<AliMultSelectionTask *>(gInterpreter->ProcessLine(Form(".x %s",
				    gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C"))));
    multSel->SetAlternateOADBFullManualBypassMC("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/data/OADB-LHC18q-DefaultMC-HIJING.root");
  }
  
  AliAnalysisTaskSEHFTreeCreator *task = reinterpret_cast<AliAnalysisTaskSEHFTreeCreator*>(gInterpreter->ProcessLine(Form(".x %s(%d,%d,\"%s\",\"%s\", %d,%d,%d)",
   				  gSystem->ExpandPathName("$ALICE_PHYSICS/PWGHF/treeHF/macros/AddTaskHFTreeCreator.C"),
  				  isRunOnMC, 0, "HFTreeCreator", cutFile.Data(), 1, kTRUE, kTRUE)));

  AliAnalysisTaskSEDvsMultiplicity  *tasklcvsmult = reinterpret_cast<AliAnalysisTaskSEDvsMultiplicity *>(gInterpreter->ProcessLine(Form(".x %s(%d,%d,%d,%d,\"%s\",\"%s\",\"%s\",\"%s\",%f,%d,%d,%d,%d,%d,%d,%d)",
  				  gSystem->ExpandPathName("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskDvsMultiplicity.C"),
				  0,kFALSE,0,4122,"LcpK0s_vsMult18","Lc2pK0SCuts.root","LctoV0AnalysisCuts","alien:///alice/cern.ch/user/l/ldellost/LcvsMult/ntrackl18.root",12.25,
				  kTRUE,kFALSE, AliAnalysisTaskSEDvsMultiplicity::kNtrk10,AliAnalysisTaskSEDvsMultiplicity::kEta10,kFALSE,18,kTRUE)));
  if(System==kPbPb) {
    AliAnalysisTaskSECleanupVertexingHF *taskclean =reinterpret_cast<AliAnalysisTaskSECleanupVertexingHF *>(gInterpreter->ProcessLine(Form(".x %s",
				    gSystem->ExpandPathName("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskCleanupVertexingHF.C"))));
  }
  
  
  if(!mgr->InitAnalysis()) return;
  mgr->SetDebugLevel(2);
  mgr->PrintStatus();
  mgr->SetUseProgressBar(1, 25);
  
    
  TChain* chainAOD = new TChain("aodTree");
  TChain *chainAODfriend = new TChain("aodTree");
  chainAOD->Add(Form("%s/AliAOD.root",pathToLocalAODfiles.Data()));
  chainAODfriend->Add(Form("%s/AliAOD.VertexingHF.root",pathToLocalAODfiles.Data()));
  chainAOD->AddFriend(chainAODfriend);
  mgr->StartAnalysis("local", chainAOD, 9999, 0);
    
}
