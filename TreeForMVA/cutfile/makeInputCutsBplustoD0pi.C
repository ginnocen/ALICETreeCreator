

// gSystem->SetIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS -I$ALICE_PHYSICS/include -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/CONTAINERS -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/TRD -I$ALICE_ROOT/macros -I$ALICE_ROOT/ANALYSIS  -I$ALICE_PHYSICS/OADB -I$ALICE_PHYSICS/PWGHF -I$ALICE_PHYSICS/PWGHF/base -I$ALICE_PHYSICS/PWGHF/vertexingHF -I$ALICE_PHYSICS/PWG/FLOW/Base -I$ALICE_PHYSICS/PWG/FLOW/Tasks -I$ALICE_PHYSICS/PWG -g");
// .L AliRDHFCutsBPlustoD0Pi.cxx++
// .L makeTFile4CutsBPlustoD0Pi.C++
// makeInputAliAnalysisTaskSEBPlustoD0Pi()

#include <Riostream.h>
#include <TFile.h>
#include "AliRDHFCutsBPlustoD0Pi.h"
#include <TClonesArray.h>
#include <TParameter.h>
#include <ctime>
#include <time.h>
#include <TSystem.h>


// 
// Comment
// 
// test for cut optimization
// 


//Use:

// root[] .L makeTFile4CutsBPlustoD0Pi.C++
// root[] makeInputAliAnalysisTaskSEBPlustoD0Pi() // for MC
// root[] makeInputAliAnalysisTaskSEBPlustoD0Pi(kTRUE)  // for Data

//Author: Lennart van Doremalen, l.v.r.vandoremalen@uu.nl


//macro to make a .root file which contains an AliRDHFCutsBPlustoD0Pi for AliAnalysisTaskSEDStarSpectra task
AliRDHFCutsBPlustoD0Pi * makeInputCutsBplustoD0pi(Int_t whichCuts=0, TString nameCuts="BplustoD0piFilteringCuts", Float_t minc=0.,Float_t maxc=100.){

  // cout << " test 1 " << endl;

  AliRDHFCutsBPlustoD0Pi* RDHFBPlustoD0Pi = new AliRDHFCutsBPlustoD0Pi();
  // RDHFBPlustoD0Pi->SetName("BPlustoD0PiCuts");
  // RDHFBPlustoD0Pi->SetTitle("Cuts for BPlus analysis");
  RDHFBPlustoD0Pi->SetName(nameCuts.Data());
  RDHFBPlustoD0Pi->SetTitle(nameCuts.Data());
  // TString cutNumber = "Cut_0030opt";
  // if(!bIsData) cutNumber+= "MC";
  //Centrality selection
  RDHFBPlustoD0Pi->SetUseCentrality(kFALSE);
  RDHFBPlustoD0Pi->SetMinCentrality(minc);
  RDHFBPlustoD0Pi->SetMaxCentrality(maxc);
  
  //triggers
  // RDHFBPlustoD0Pi->SetTriggerClass("");
  // if(bIsData) RDHFBPlustoD0Pi->SetTriggerMask(AliVEvent::kINT7);
  // if(!bIsData) RDHFBPlustoD0Pi->SetTriggerMask(AliVEvent::kMB);
  // RDHFBPlustoD0Pi->SetTriggerMask(AliVEvent::kAny);
  RDHFBPlustoD0Pi->SetTriggerClass("");
  RDHFBPlustoD0Pi->SetTriggerMask(AliVEvent::kINT7);
  //for upgrade:
  // RDHFBPlustoD0Pi->SetUsePhysicsSelection(kFALSE);
  // RDHFBPlustoD0Pi->SetCutOnzVertexSPD(0);

  AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts(); //not yet implemented as ESD track cuts
  esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
  esdTrackCuts->SetRequireTPCRefit(kTRUE);
  esdTrackCuts->SetRequireITSRefit(kTRUE);
  esdTrackCuts->SetMinNClustersTPC(70);
  esdTrackCuts->SetMinNClustersITS(2);
  // esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kAny); 
  esdTrackCuts->SetMinDCAToVertexXY(0.);
  esdTrackCuts->SetPtRange(0.3,1.e10);
  RDHFBPlustoD0Pi->AddTrackCuts(esdTrackCuts);

  // cout << " test 2 " << endl;
  // set pre selections
  // RDHFBPlustoD0Pi->AddTrackCuts(esdTrackCuts);

  //--------------------------------------------------------------- 
  // 
  //  SET BASIC DAUGHTER CUTS
  // 
  // --------------------------------------------------------------

  RDHFBPlustoD0Pi->SetMaxAbsEtaD0FirstDaughter(0.8);
  Bool_t bHardSelectionArrayITSD0FirstDaughter[7] = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
  RDHFBPlustoD0Pi->SetHardSelectionArrayITSD0FirstDaughter(bHardSelectionArrayITSD0FirstDaughter);
  Bool_t bSoftSelectionArrayITSD0FirstDaughter[7] = {kTRUE,kTRUE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
  RDHFBPlustoD0Pi->SetSoftSelectionArrayITSD0FirstDaughter(bSoftSelectionArrayITSD0FirstDaughter);
  RDHFBPlustoD0Pi->SetNSoftITSCutD0FirstDaughter(1);

  RDHFBPlustoD0Pi->SetMinITSNclsD0FirstDaughter(2); 
  RDHFBPlustoD0Pi->SetMinTPCNclsD0FirstDaughter(70);
  RDHFBPlustoD0Pi->SetUseITSRefitD0FirstDaughter(kTRUE);
  RDHFBPlustoD0Pi->SetUseTPCRefitD0FirstDaughter(kTRUE);
  RDHFBPlustoD0Pi->SetUseFilterBitD0FirstDaughter(kFALSE);
  RDHFBPlustoD0Pi->SetFilterBitD0FirstDaughter(0);
  RDHFBPlustoD0Pi->SetMinPtD0FirstDaughter(0.5);

  RDHFBPlustoD0Pi->SetMaxAbsEtaD0SecondDaughter(0.8);
  Bool_t bHardSelectionArrayITSD0SecondDaughter[7] = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
  RDHFBPlustoD0Pi->SetHardSelectionArrayITSD0SecondDaughter(bHardSelectionArrayITSD0SecondDaughter);
  Bool_t bSoftSelectionArrayITSD0SecondDaughter[7] = {kTRUE,kTRUE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
  RDHFBPlustoD0Pi->SetSoftSelectionArrayITSD0SecondDaughter(bSoftSelectionArrayITSD0SecondDaughter);
  RDHFBPlustoD0Pi->SetNSoftITSCutD0SecondDaughter(1);

  RDHFBPlustoD0Pi->SetMinITSNclsD0SecondDaughter(2);
  RDHFBPlustoD0Pi->SetMinTPCNclsD0SecondDaughter(70);
  RDHFBPlustoD0Pi->SetUseITSRefitD0SecondDaughter(kTRUE);
  RDHFBPlustoD0Pi->SetUseTPCRefitD0SecondDaughter(kTRUE);
  RDHFBPlustoD0Pi->SetUseFilterBitD0SecondDaughter(kFALSE);
  RDHFBPlustoD0Pi->SetFilterBitD0SecondDaughter(0);
  RDHFBPlustoD0Pi->SetMinPtD0SecondDaughter(0.5);

  RDHFBPlustoD0Pi->SetMaxAbsEtaBPlusPion(0.8);
  Bool_t bHardSelectionArrayITSBPlusPion[7] = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
  RDHFBPlustoD0Pi->SetHardSelectionArrayITSBPlusPion(bHardSelectionArrayITSBPlusPion);
  Bool_t bSoftSelectionArrayITSBPlusPion[7] = {kTRUE,kTRUE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
  RDHFBPlustoD0Pi->SetSoftSelectionArrayITSBPlusPion(bSoftSelectionArrayITSBPlusPion);
  RDHFBPlustoD0Pi->SetNSoftITSCutBPlusPion(1);

  RDHFBPlustoD0Pi->SetMinITSNclsBPlusPion(2);
  RDHFBPlustoD0Pi->SetMinTPCNclsBPlusPion(70);
  RDHFBPlustoD0Pi->SetUseITSRefitBPlusPion(kTRUE);
  RDHFBPlustoD0Pi->SetUseTPCRefitBPlusPion(kTRUE);
  RDHFBPlustoD0Pi->SetUseFilterBitBPlusPion(kFALSE);
  RDHFBPlustoD0Pi->SetFilterBitBPlusPion(0);
  RDHFBPlustoD0Pi->SetMinPtBPlusPion(0.5);

  RDHFBPlustoD0Pi->SetMind0D0FirstDaughter(0.0);
  RDHFBPlustoD0Pi->SetMind0D0SecondDaughter(0.0);
  RDHFBPlustoD0Pi->SetMind0BPlusPion(0.0);

  //--------------------------------------------------------------- 
  // 
  //  SET PT BINS
  // 
  // --------------------------------------------------------------

  //main BPlus pt bins
  const Int_t nptbins=6;
  
  Float_t* ptbins;
  ptbins=new Float_t[nptbins+1];
  ptbins[0]=0.;
  ptbins[1]=3.;
  ptbins[2]=6.;
  ptbins[3]=10.;
  ptbins[4]=18.;
  ptbins[5]=30.;
  ptbins[6]=9999999.;

  // RDHFBPlustoD0Pi->SetNPtBins(nptbins);
  RDHFBPlustoD0Pi->SetPtBins(nptbins+1,ptbins);
  
  // D0 with D0 pt bins
  const Int_t nptbinsD0forD0ptbin=1;
  const Int_t nptbinsD0forD0ptbinlimits=2; //nptbinsD0forD0ptbin +1
  
  Float_t* ptbinsD0forD0ptbin;
  ptbinsD0forD0ptbin =new Float_t[nptbinsD0forD0ptbinlimits];
  ptbinsD0forD0ptbin[0]=0.;
  // ptbinsD0forD0ptbin[1]=2.;
  // ptbinsD0forD0ptbin[2]=5.;
  // ptbinsD0forD0ptbin[3]=10.;
  // ptbinsD0forD0ptbin[4]=16.;
  ptbinsD0forD0ptbin[1]=9999999.;

  RDHFBPlustoD0Pi->SetNPtBinsD0forD0ptbin(nptbinsD0forD0ptbin);
  RDHFBPlustoD0Pi->SetPtBinsD0forD0ptbin(nptbinsD0forD0ptbinlimits,ptbinsD0forD0ptbin);


  //--------------------------------------------------------------- 
  // 
  //  SET CUTS
  // 
  // --------------------------------------------------------------

  // we initialize the cuts by creating a cut array of which all cuts are set to zero and set to unused.
  RDHFBPlustoD0Pi->InitializeCuts();
  Int_t ptBin = 0;
  // AliRDHFCutsBPlustoD0Pi::EUpperCut AliRDHFCutsBPlustoD0Pi::kCutBelowValue;
  // AliRDHFCutsBPlustoD0Pi::EUpperCut AliRDHFCutsBPlustoD0Pi::kCutAboveValue;

  // we set the cuts for BPlus pt bins.
  // the cut direction of a variable should be the same for all pt bins.


  //loop over all pt bins for pt integrated cuts

  // We set cuts for all candidates
  // These can have a large effect on the CPU time of the analysis

  // for (int i = 0; i < nptbins; ++i)
  // {
  //   // RDHFBPlustoD0Pi->SetCut(36, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, -0.01);    // 36 - "pseudoProperDecayTimeToBPlus";  
  //   // RDHFBPlustoD0Pi->SetCut(39, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0); // 68 - "inv. mass width[GeV]";    
  //   // RDHFBPlustoD0Pi->SetCut(40, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0); // 69 - "delta mass width  [GeV]"; 
  //   RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.92); // 70 - "pointing angle [Cos(theta)]";
  //   // RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.025); // 71 - "dca [cm]";                   
  //   // RDHFBPlustoD0Pi->SetCut(43, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 72 - "Pt BPlus [GeV/c]";
  //   // RDHFBPlustoD0Pi->SetCut(44, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 73 - "Pt D0 [GeV/c]";
  //   // RDHFBPlustoD0Pi->SetCut(45, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 74 - "Pt Pion [GeV/c]";
  //   RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.02); // 75 - "d0 BPlus [cm]";
  //   // RDHFBPlustoD0Pi->SetCut(47, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 76 - "d0 D0 [cm]";                
  //   // RDHFBPlustoD0Pi->SetCut(48, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 77 - "d0 Pion [cm]";                
  //   RDHFBPlustoD0Pi->SetCut(49, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0); // 78 - "d0d0 [cm^2]";
  //   // RDHFBPlustoD0Pi->SetCut(50, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 79 - "d0d0 XY [cm^2]";

  //   // RDHFBPlustoD0Pi->SetCut(51, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 80 - "angle between both daughters"; 
  //   // RDHFBPlustoD0Pi->SetCut(52, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 81 - "angle mother with first daughter";
  //   // RDHFBPlustoD0Pi->SetCut(53, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 82 - "angle mother with second daughter";
  //   // RDHFBPlustoD0Pi->SetCut(54, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 83 - "cosThetaStar";                
  //   // RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.01); // 84 - "vertexDistance"; 
  //   RDHFBPlustoD0Pi->SetCut(56, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 85 - "pseudoProperDecayTime"; 
  //   // RDHFBPlustoD0Pi->SetCut(57, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 86 - "DecayTime"; 
  //   // RDHFBPlustoD0Pi->SetCut(58, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 87 - "normDecayTime";
  //   // RDHFBPlustoD0Pi->SetCut(59, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 2.0); // 88 - "normDecayLength";
  //   // RDHFBPlustoD0Pi->SetCut(60, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 89 - "topomatic first daughter";
  //   // RDHFBPlustoD0Pi->SetCut(61, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 90 - "topomatic second daughter";
  //   // RDHFBPlustoD0Pi->SetCut(62, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 91 - "topomatic max";
  //   // RDHFBPlustoD0Pi->SetCut(63, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 92 - "topomatic min"; 

  //   RDHFBPlustoD0Pi->SetCut(64, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.98); // 93 - "pointing angle XY [Cos(theta)]";
  //   // RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 94 - "vertex distance XY [cm]";
  //   // RDHFBPlustoD0Pi->SetCut(66, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 95 - "normDecayLength XY";
  //   // RDHFBPlustoD0Pi->SetCut(67, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 96 - "Chi2 per NDF vertex"; 
  // }
  // for (int i = 0; i < 1; ++i)
  // {
  //   RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.04); // 71 - "inv. mass width[GeV] D0"; 
  //   // RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";    
  //   // RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.006); // 71 - "d0 D0 [cm]";    
  //   // RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 first daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 second daughter D0"; 

  //   RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.92); // 70 - "pointing angle [Cos(theta)]";
  //   RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.03); // 71 - "dca [cm]";             
  //   // // RDHFBPlustoD0Pi->SetCut(45, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.5); // 84 - "pt pion B0";       
  //   // RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
  //   // // RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistance"; 
  //   // RDHFBPlustoD0Pi->SetCut(59, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 3.5); // 88 - "normDecayLength";
  //   // RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistanceXY";
  // }
  // for (int i = 1; i < 2; ++i)
  // {
  //   RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.045); // 71 - "inv. mass width[GeV] D0"; 
  //   // RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";    
  //   // RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.006); // 71 - "d0 D0 [cm]";    
  //   // RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 first daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 second daughter D0"; 
  //   // RDHFBPlustoD0Pi->SetCut(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.07); // 84 - "vertexDistance D0";  


  //   RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.92); // 70 - "pointing angle [Cos(theta)]";
  //   RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.025); // 71 - "dca [cm]";             
  //   // // RDHFBPlustoD0Pi->SetCut(45, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.5); // 84 - "pt pion B0";       
  //   // RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
  //   // // RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistance"; 
  //   // RDHFBPlustoD0Pi->SetCut(59, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 3.5); // 88 - "normDecayLength";
  //   // RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistanceXY";
  // }
  // for (int i = 2; i < 3; ++i)
  // {
  //   RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.05); // 71 - "inv. mass width[GeV] D0"; 
  //   // RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";    
  //   // RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.006); // 71 - "d0 D0 [cm]";    
  //   // RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 first daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 second daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.07); // 84 - "vertexDistance D0";    


  //   RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.94); // 70 - "pointing angle [Cos(theta)]";
  //   RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.025); // 71 - "dca [cm]";             
  //   // RDHFBPlustoD0Pi->SetCut(45, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.5); // 84 - "pt pion B0";       
  //   // RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
  //   // RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistance"; 
  //   // RDHFBPlustoD0Pi->SetCut(59, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 3.5); // 88 - "normDecayLength";
  //   // RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistanceXY";

  // }
  // for (int i = 3; i < 4; ++i)
  // {
  //   RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.06); // 71 - "inv. mass width[GeV] D0"; 
  //   // RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";    
  //   // RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.004); // 71 - "d0 D0 [cm]";    
  //   // RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 first daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 second daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.07); // 84 - "vertexDistance D0";    


  //   RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.94); // 70 - "pointing angle [Cos(theta)]";
  //   RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.02); // 71 - "dca [cm]";             
  //   // RDHFBPlustoD0Pi->SetCut(45, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.5); // 84 - "pt pion B0";       
  //   // RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
  //   // RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistance"; 
  //   // RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistanceXY";


  // }
  // for (int i = 4; i < 5; ++i)
  // {
  //   RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.07); // 71 - "inv. mass width[GeV] D0"; 
  //   // RDHFBPlustoD0Pi->SetCut(2, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.94); // 71 - "pointing angle [Cos(theta)] D0"; 
  //   // RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";    
  //   // RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.004); // 71 - "d0 D0 [cm]";    
  //   // RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.004); // 71 - "d0 first daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.004); // 71 - "d0 second daughter D0";
  //   // RDHFBPlustoD0Pi->SetCut(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.07); // 84 - "vertexDistance D0";    


  //   RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.98); // 70 - "pointing angle [Cos(theta)]";
  //   RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.02); // 71 - "dca [cm]";                 
  //   // RDHFBPlustoD0Pi->SetCut(45, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.5); // 84 - "pt pion B0";   
  //   // RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
  //   // RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistance"; 
  //   // RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistanceXY"; 

  // }
  // for (int i = 5; i < 6; ++i)
  // {
  //   RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.05); // 71 - "dca [cm]";                   
  //   // RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.03); // 75 - "d0 BPlus [cm]";
  //   // RDHFBPlustoD0Pi->SetCut(59, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 3.5); // 88 - "normDecayLength";
  // }



  // We set cuts for all D0 candidates
  // These can have a large effect on the CPU time of the analysis

  for (int i = 0; i < nptbinsD0forD0ptbin; ++i)
  {
    // cout << "test s" << endl;
    RDHFBPlustoD0Pi->SetCutD0forD0ptbin(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.07);   // 00 - "inv. mass width[GeV]";    
    // RDHFBPlustoD0Pi->SetCutD0forD0ptbin(2, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.8);    // 02 - "pointing angle [Cos(theta)]";   
    RDHFBPlustoD0Pi->SetCutD0forD0ptbin(3, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.02);   // 03 - "dca [cm]";
    RDHFBPlustoD0Pi->SetCutD0forD0ptbin(4, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 2.5);    // 04 - "Pt D0 [GeV/c]";   
    // RDHFBPlustoD0Pi->SetCutD0forD0ptbin(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 07 - "d0 D0 [cm]";   
    // RDHFBPlustoD0Pi->SetCutD0forD0ptbin(12, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.005);    // 11 "d0d0 XY [cm^2]"; 
    RDHFBPlustoD0Pi->SetCutD0forD0ptbin(12, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.5);    // 12 "angle between both daughters";    
    // RDHFBPlustoD0Pi->SetCutD0forD0ptbin(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.05);   // 16 - "vertexDistance";   
    RDHFBPlustoD0Pi->SetCutD0forD0ptbin(17, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.0);    // 17 - "pseudoProperDecayTime";
    // RDHFBPlustoD0Pi->SetCutD0forD0ptbin(18, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.1e-9);    // 18 - "DecayTime";   
    // RDHFBPlustoD0Pi->SetCutD0forD0ptbin(26, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.05);    // 26 - "vertexDistanceXY";   
    // RDHFBPlustoD0Pi->SetCutD0forD0ptbin(27, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 4.0);    // 27 - "normDecayLength XY";   
  }

  // cout << " test 3 " << endl;
  // Cut optimization - the number of bins used per pt bin is 2*nCutsForOptimization^nVariables. Make sure that this number does not become too big!
  Int_t nCutsForOptimization = 6;
  Int_t nVariables = 6;
  RDHFBPlustoD0Pi->InitializeCutsForCutOptimization(nCutsForOptimization, nVariables);

  // cout << " test 4 " << endl;

  RDHFBPlustoD0Pi->SetSigmaForCutOptimization(0.05, 0);
  RDHFBPlustoD0Pi->SetSigmaForCutOptimization(0.05, 1);
  RDHFBPlustoD0Pi->SetSigmaForCutOptimization(0.05, 2);
  RDHFBPlustoD0Pi->SetSigmaForCutOptimization(0.05, 3);
  RDHFBPlustoD0Pi->SetSigmaForCutOptimization(0.05, 4);
  RDHFBPlustoD0Pi->SetSigmaForCutOptimization(0.05, 5);

  // cout << " test 5 " << endl;
  for (int i = 0; i < 6; ++i)
  {
    Float_t cutValues00[nCutsForOptimization] = {0.1e-9,0.15e-9,0.2e-9,0.25e-9,0.3e-9,0.35e-9};
    Float_t cutValues01[nCutsForOptimization] = {0.01,0.02,0.03,0.04,0.05,0.06};
    Float_t cutValues02[nCutsForOptimization] = {0.01,0.02,0.03,0.04,0.05,0.06};
    Float_t cutValues03[nCutsForOptimization] = {0.000,0.001,0.002,0.003,0.004,0.005};
    Float_t cutValues04[nCutsForOptimization] = {0.000,0.001,0.002,0.003,0.004,0.005};
    Float_t cutValues05[nCutsForOptimization] = {0.000,0.001,0.002,0.003,0.004,0.005};
    // cout << " test 6 " << endl;

    RDHFBPlustoD0Pi->SetCutForCutOptimization(18, 0, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, cutValues00); // "DecayTime";
    RDHFBPlustoD0Pi->SetCutForCutOptimization(55, 1, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, cutValues01); // "vertexDistance"; 
    RDHFBPlustoD0Pi->SetCutForCutOptimization(16, 2, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, cutValues02); // "vertexDistance D0"; 
    RDHFBPlustoD0Pi->SetCutForCutOptimization(7, 3, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, cutValues03); // 07 - "d0 D0 [cm]";   
    RDHFBPlustoD0Pi->SetCutForCutOptimization(8, 4, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, cutValues04); // 71 - "d0 first daughter D0";
    RDHFBPlustoD0Pi->SetCutForCutOptimization(9, 5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, cutValues05); // 71 - "d0 second daughter D0";
  }
  // cout << " test 7 " << endl;
  // for (int i = 1; i < 2; ++i)
  // {
  //   RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.045); // 71 - "inv. mass width[GeV] D0"; 
  //   RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
  //   RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";    
  //   RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.006); // 71 - "d0 D0 [cm]";    
  //   RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 first daughter D0";
  //   RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 second daughter D0"; 
  //   RDHFBPlustoD0Pi->SetCut(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.07); // 84 - "vertexDistance D0";  


  //   RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.92); // 70 - "pointing angle [Cos(theta)]";
  //   RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.025); // 71 - "dca [cm]";             
  //   // RDHFBPlustoD0Pi->SetCut(45, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.5); // 84 - "pt pion B0";       
  //   RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
  //   // RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistance"; 
  //   RDHFBPlustoD0Pi->SetCut(59, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 3.5); // 88 - "normDecayLength";
  //   // RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistanceXY";
  // }
  // for (int i = 2; i < 3; ++i)
  // {
  //   RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.05); // 71 - "inv. mass width[GeV] D0"; 
  //   RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
  //   RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";    
  //   RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.006); // 71 - "d0 D0 [cm]";    
  //   RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 first daughter D0";
  //   RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 second daughter D0";
  //   RDHFBPlustoD0Pi->SetCut(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.07); // 84 - "vertexDistance D0";    


  //   RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.94); // 70 - "pointing angle [Cos(theta)]";
  //   RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.025); // 71 - "dca [cm]";             
  //   // RDHFBPlustoD0Pi->SetCut(45, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.5); // 84 - "pt pion B0";       
  //   RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
  //   RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistance"; 
  //   RDHFBPlustoD0Pi->SetCut(59, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 3.5); // 88 - "normDecayLength";
  //   RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistanceXY";

  // }
  // for (int i = 3; i < 4; ++i)
  // {
  //   RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.06); // 71 - "inv. mass width[GeV] D0"; 
  //   RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
  //   RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";    
  //   RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.004); // 71 - "d0 D0 [cm]";    
  //   RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 first daughter D0";
  //   RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 second daughter D0";
  //   RDHFBPlustoD0Pi->SetCut(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.07); // 84 - "vertexDistance D0";    


  //   RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.94); // 70 - "pointing angle [Cos(theta)]";
  //   RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.02); // 71 - "dca [cm]";             
  //   RDHFBPlustoD0Pi->SetCut(45, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.5); // 84 - "pt pion B0";       
  //   RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
  //   RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistance"; 
  //   RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistanceXY";


  // }
  // for (int i = 4; i < 5; ++i)
  // {
  //   RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.07); // 71 - "inv. mass width[GeV] D0"; 
  //   RDHFBPlustoD0Pi->SetCut(2, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.94); // 71 - "pointing angle [Cos(theta)] D0"; 
  //   RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
  //   RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";    
  //   RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.004); // 71 - "d0 D0 [cm]";    
  //   RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.004); // 71 - "d0 first daughter D0";
  //   RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.004); // 71 - "d0 second daughter D0";
  //   RDHFBPlustoD0Pi->SetCut(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.07); // 84 - "vertexDistance D0";    


  //   RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.98); // 70 - "pointing angle [Cos(theta)]";
  //   RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.02); // 71 - "dca [cm]";                 
  //   RDHFBPlustoD0Pi->SetCut(45, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.5); // 84 - "pt pion B0";   
  //   RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
  //   RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistance"; 
  //   RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistanceXY"; 

  // }
  // for (int i = 5; i < 6; ++i)
  // {
  //   RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.05); // 71 - "dca [cm]";                   
  //   RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.03); // 75 - "d0 BPlus [cm]";
  //   // RDHFBPlustoD0Pi->SetCut(59, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 3.5); // 88 - "normDecayLength";
  // }




  Bool_t pidflag=kTRUE;
  RDHFBPlustoD0Pi->SetUsePID(pidflag);
  if(pidflag) cout<<"PID is used"<<endl;
  else cout<<"PID is not used"<<endl;
  // cout << " test 7b " << endl;
  // PID SETTINGS
  AliAODPidHF* pidObj = new AliAODPidHF();
  // pidObj->SetName("pid4DSatr");
  // cout << " test 7c " << endl;
  Int_t mode=1;
  Double_t priors[5]={0.01,0.001,0.3,0.3,0.3};
  pidObj->SetPriors(priors,5);
  pidObj->SetMatch(mode);
  pidObj->SetSigma(0,3); // TPC
  pidObj->SetSigma(3,3); // TOF
  pidObj->SetTPC(kTRUE);
  pidObj->SetTOF(kTRUE);
  // cout << " test 7d " << endl;
  pidObj->SetMaxTrackMomForCombinedPID(3.);
  RDHFBPlustoD0Pi->SetPidHF(pidObj);
  // cout << " test 8 " << endl;

  RDHFBPlustoD0Pi->SetOffHighPtPIDinTPC(4);

  // if(bIsData){
  //   cout << "Reject pileup" << endl;
  RDHFBPlustoD0Pi->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent);
  RDHFBPlustoD0Pi->ConfigurePileupCuts(5,0.8);
  RDHFBPlustoD0Pi->SetMaxVtxZ(10.);
  RDHFBPlustoD0Pi->SetCutOnzVertexSPD(3);
  RDHFBPlustoD0Pi->SetMinVtxContr(1);
  // } else {
  //   cout << "Don't reject pileup" << endl;
  //   RDHFBPlustoD0Pi->SetOptPileup(0);
  // }

  // RDHFBPlustoD0Pi->SetSelectCandTrackSPDFirst(kTRUE,2);
  // cout << " test 9 " << endl;
  RDHFBPlustoD0Pi->PrintAll();

  TString nameFile = "";
  if(bIsData)  nameFile = "BPlustoD0PiCuts.root";
  // if(!bIsData) nameFile = "BPlustoD0PiCutsMC.root";  
  TFile* fout = new TFile(nameFile.Data(),"recreate"); 
  fout->cd();
  RDHFBPlustoD0Pi->Write();
  fout->Close();

  delete fout; fout = NULL;
  delete pidObj; pidObj = NULL;
  delete RDHFBPlustoD0Pi; RDHFBPlustoD0Pi = NULL;
  delete[] ptbins; ptbins = NULL;
  delete[] ptbinsD0forD0ptbin; ptbinsD0forD0ptbin = NULL;
  // alien_cp  alien:/$BASEDIR/$sub_dir/$file 
  // cout << " test 10 " << endl;
  // char date[1000];
  // std::time_t t = std::time(0);
  // strftime (date, 100, "Date_%Y-%m-%d_Time_%H-%M-%S", localtime (&t));
  // std::cout << date << std::endl;
  // TString command;
  // command = "alien_cp file:./makeTFile4CutsBPlustoD0Pi.C alien:///alice/cern.ch/user/l/lvandore/BPlustoD0Pi_cut_files/makeTFile4CutsBPlustoD0Pi_";
  // // command += date;
  // command += cutNumber;
  // command += ".C";
  // cout << "Copying makeTFile to cut folder: " << command << endl; 
  // gSystem->Exec(command);

  // TString command2;
  // command2 = "alien_cp file:./BPlustoD0PiCuts.root alien:///alice/cern.ch/user/l/lvandore/BPlustoD0Pi_cut_files/BPlustoD0PiCuts_";
  // // command2 += date;
  // command2 += cutNumber;
  // command2 += ".root";
  // cout << "Copying cut file to cut folder: " << command2 << endl; 
  // gSystem->Exec(command2);

  cout<<"This is the object I'm going to save:"<<endl;
  RDHFBPlustoD0Pi->SetName(nameCuts.Data());
  RDHFBPlustoD0Pi->SetTitle(nameCuts.Data());
  RDHFBPlustoD0Pi->PrintAll();
  
  return RDHFBPlustoD0Pi;
}

