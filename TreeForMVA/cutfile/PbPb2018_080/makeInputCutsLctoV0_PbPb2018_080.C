#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsLctoV0.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="LctoV0FilteringCuts"
 whichCuts=1, nameCuts="LctoV0AnalysisCuts"
 */

const Int_t nvars=23;
//        Lc inv. mass if K0S [GeV/c2]
//        Lc inv. mass if Lambda [GeV/c2]
//        K0S inv. mass [GeV/c2]
//        Lambda/LambdaBar inv. mass[GeV/c2]
//        pT min bachelor track [GeV/c]
//        pT min V0-positive track [GeV/c]
//        pT min V0-negative track [GeV/c]
//        dca cascade (prong-to-prong) cut [cm]
//        dca V0 (prong-to-prong) cut [number of sigmas]
//        V0 cosPA min wrt PV
//        d0 max bachelor wrt PV [cm]
//        d0 max V0 wrt PV [cm]
//        mass K0S veto [GeV/c2]
//        mass Lambda/LambdaBar veto [GeV/c2]
//        mass Gamma veto [GeV/c2]
//        pT min V0 track [GeV/c]
//        Max Proton emission angle in Lc CMS
//        Min Proton emission angle in Lc CMS
//        Resigned d0
//        V0 qT/|alpha|
//        V0 min radius
//        V0 max radius
//        V0 type

AliRDHFCutsLctoV0 *makeInputCutsLctoV0(Int_t whichCuts=0, TString nameCuts="LctoV0FilteringCuts", Float_t minc=0., Float_t maxc=80., Bool_t isMC=kFALSE, Int_t OptPreSelect = 1, Int_t TPCClsPID = 50, Bool_t PIDcorrection=kTRUE, Double_t minpt = 4, Bool_t spdkAny=kTRUE, Bool_t ispass1 = kFALSE)
{
  
  AliRDHFCutsLctoV0* cutsLctoV0=new AliRDHFCutsLctoV0();
  cutsLctoV0->SetName(nameCuts.Data());
  cutsLctoV0->SetTitle(nameCuts.Data());
  
  //UPDATE 07/05/19, use the same track quality cuts for filtering and analysis cuts
  //single track cuts
  AliESDtrackCuts *esdTrackCuts = new AliESDtrackCuts("AliESDtrackCuts","default");
  esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
  esdTrackCuts->SetRequireTPCRefit(kTRUE);
  esdTrackCuts->SetRequireITSRefit(kTRUE);
  //Should not use SetMinNClustersTPC anymore, not well described in MC
  //Two lines below replace this cut (for value 70)
  //  esdTrackCuts->SetMinNClustersTPC(70);
  esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
  esdTrackCuts->SetMinNCrossedRowsTPC(70);
  //UPDATE 08/06/20, make kAny default. With the spdhits_prong# variables
  //we can always do the kFirst selection in MLHEP
  if(spdkAny) esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kAny);
  else        esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kFirst);
  //NB: No cut is placed for cent<20 for SetTrackFilterBachelor, so doesn't matter
  esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))"); //filtering
  esdTrackCuts->SetMinDCAToVertexXY(0.);
  esdTrackCuts->SetMaxDCAToVertexXY(1.);
  esdTrackCuts->SetMaxDCAToVertexZ(1.);
  //NB: Minimum in filtering is 0.5 for all centralities (see SetTrackFilterBachelor)
  esdTrackCuts->SetPtRange(0.5,1.e10);
  esdTrackCuts->SetEtaRange(-0.8,+0.8);
  esdTrackCuts->SetAcceptKinkDaughters(kFALSE);
  //UPDATE 04/11/10, set chi2 per TPC cluster to 2.5 instead of 4
  if(!ispass1) esdTrackCuts->SetMaxChi2PerClusterTPC(2.5);

  // V0 daughters cuts
  AliESDtrackCuts* esdTrackCutsV0daughters=new AliESDtrackCuts();
  esdTrackCutsV0daughters->SetRequireSigmaToVertex(kFALSE);
  esdTrackCutsV0daughters->SetRequireTPCRefit(kTRUE);
  esdTrackCutsV0daughters->SetRequireITSRefit(kFALSE);
  //Should not use SetMinNClustersTPC anymore, not well described in MC
  //Two lines below replace this cut (for value 70)
  //  esdTrackCuts->SetMinNClustersTPC(70);
  esdTrackCutsV0daughters->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
  esdTrackCutsV0daughters->SetMinNCrossedRowsTPC(70);
  esdTrackCutsV0daughters->SetMinDCAToVertexXY(0.);
  esdTrackCutsV0daughters->SetPtRange(0.,1.e10);
  esdTrackCutsV0daughters->SetEtaRange(-0.8,+0.8);
  esdTrackCutsV0daughters->SetAcceptKinkDaughters(kFALSE);
  //UPDATE 04/11/10, set chi2 per TPC cluster to 2.5 instead of 4
  if(!ispass1) esdTrackCutsV0daughters->SetMaxChi2PerClusterTPC(2.5);

  cutsLctoV0->AddTrackCuts(esdTrackCuts);
  cutsLctoV0->AddTrackCutsV0daughters(esdTrackCutsV0daughters);
  cutsLctoV0->SetKinkRejection(!esdTrackCuts->GetAcceptKinkDaughters());
  //UPDATE 08/06/20, set to kTRUE as should be done for all other HF hadrons (pK0s was true, others false)
  cutsLctoV0->SetUseTrackSelectionWithFilterBits(kTRUE);
  //UPDATE 08/06/20, Add cut on TPC clusters for PID (similar to geometrical cut)
  cutsLctoV0->SetMinNumTPCClsForPID(TPCClsPID);
  
  if(whichCuts==0){
    const Int_t nptbins=3;
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    ptbins[0]=0.;
    ptbins[1]=2.;
    ptbins[2]=4.;
    ptbins[3]=999.;
    cutsLctoV0->SetPtBins(nptbins+1,ptbins);
    //Update 22/02/21: Same cuts used as 0-10%
    Float_t cuts[nptbins][nvars]={
      0.2, 0.0, 0.01, 0.05, 0.0, 0.0, 0.0, 1000., 0.4, 0.9998, 3., 1.5, 0., 0.005, 0.1, 0.5, 0.0, -9999., -9999., 0.15, 0. ,1000., 1,
      0.2, 0.0, 0.01, 0.05, 0.0, 0.0, 0.0, 1000., 0.4, 0.9998, 3., 1.5, 0., 0.005, 0.1, 0.5, 0.5, -9999., -9999., 0.15, 0., 1000., 1,
      0.2, 0.0, 0.01, 0.05, 0.0, 0.0, 0.0, 1000., 0.4, 0.998, 3., 1.5, 0., 0.005, 0.1, 0.5, 9999., -9999., -9999., 0.15, 0., 1000., 1
    };
    //Difference 15o cuts
    //[1] 1000 -> 0.0   //fine
    //[3] 9999 -> 0.05  //fine
    //[4] 1 -> 0        //fine
    //[5] 0.1 -> 0      //fine
    //[6] 0.1 -> 0      //fine
    //[7] 1 -> 1000     //fine
    //[10] 0.1 -> 3     //fine
    //[15] 1 -> 0.5      //fine
    //[16] 0.5 -> 0/0.5/-9999 //fine
    
    Float_t** prodcutsval;
    prodcutsval=new Float_t*[nvars];
    for(Int_t ic=0;ic<nvars;ic++){prodcutsval[ic]=new Float_t[nptbins];}
    for(Int_t ipt=0; ipt<nptbins; ipt++){
      for(Int_t ic=0; ic<nvars; ic++){
        prodcutsval[ic][ipt]=cuts[ipt][ic];
      }
    }
    
    cutsLctoV0->SetMinPtCandidate(minpt);
    cutsLctoV0->SetCuts(nvars,nptbins,prodcutsval);
    
    //UPDATE 05/11/20: Update PID strategy 11 -> 13 (= 3sigma TPC && TOF, of TPC when no TOF)
    cutsLctoV0->SetPidSelectionFlag(13);
    AliAODPidHF* pidObjBachelor = new AliAODPidHF();
    cutsLctoV0->SetPidHF(pidObjBachelor);
    Bool_t pidflag=kTRUE;
    cutsLctoV0->SetUsePID(pidflag);
    
  }
  else if(whichCuts==1){
    
    cutsLctoV0->SetHighPtCut(999.);
    cutsLctoV0->SetLowPtCut(2.0);
    cutsLctoV0->SetPidSelectionFlag(10);
    
    const Int_t nptbins=5;
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    ptbins[0]=2.;
    ptbins[1]=4.;
    ptbins[2]=6.;
    ptbins[3]=12.;
    ptbins[4]=25.;
    ptbins[5]=35.;
    cutsLctoV0->SetPtBins(nptbins+1,ptbins);
    
    Float_t cuts[nptbins][nvars]={
      0.2, 1000., 0.01, 9999., 1., 0.1, 0.1, 1., 0.4, 0.9998, 0.1, 1.5, 0., 0.005, 0.1, 1., 0.5, -1., 0.003, 0.15, 0., 1000., 1.,
      0.2, 1000., 0.01, 9999., 1., 0.1, 0.1, 1., 0.4, 0.9998, 0.1, 1.5, 0., 0.005, 0.1, 1., 0.5, -1., 0.003, 0.15, 0., 1000., 1.,
      0.2, 1000., 0.01, 9999., 1., 0.1, 0.1, 1., 0.4, 0.9998, 0.1, 1.5, 0., 0.005, 0.1, 1., 0.5, -1., 0.003, 0.15, 0., 1000., 1.,
      0.2, 1000., 0.01, 9999., 1., 0.1, 0.1, 1., 0.4, 0.9998, 0.1, 1.5, 0., 0.005, 0.1, 1., 0.5, -1., 0.003, 0.15, 0., 1000., 1.,
      0.2, 1000., 0.01, 9999., 1., 0.1, 0.1, 1., 0.4, 0.9998, 0.1, 1.5, 0., 0.005, 0.1, 1., 0.5, -1., 0.003, 0.15, 0., 1000., 1.
    };
    
    Float_t** prodcutsval;
    prodcutsval=new Float_t*[nvars];
    for(Int_t ic=0;ic<nvars;ic++){prodcutsval[ic]=new Float_t[nptbins];}
    for(Int_t ipt=0; ipt<nptbins; ipt++){
      for(Int_t ic=0; ic<nvars; ic++){
        prodcutsval[ic][ipt]=cuts[ipt][ic];
      }
    }
    cutsLctoV0->SetCuts(nvars,nptbins,prodcutsval);
    
    //pid settings
    //UPDATE 22/02/21: Update PID strategy to Yosukes one (case 10)
    cutsLctoV0->SetPidSelectionFlag(10);
    AliAODPidHF* pidObjBachelor = new AliAODPidHF();
    cutsLctoV0->SetPidHF(pidObjBachelor);
    Bool_t pidflag=kTRUE;
    cutsLctoV0->SetUsePID(pidflag);
    cutsLctoV0->SetLowPtCut(3);
    
    cutsLctoV0->SetMinPtCandidate(1.0);
    cutsLctoV0->SetMaxPtCandidate(50.);
    
  }
  
  //UPDATE 08/06/20: PreSelect, acting before FillRecoCasc.
  //NOTE: actual function not implemented for all HF hadrons yet (please check)
  cutsLctoV0->SetUsePreSelect(OptPreSelect);

  //Do not recalculate the vertex
  cutsLctoV0->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
  
  //Temporary PID fix for 2018 PbPb (only to be used on data)
  //NOT READY FOR 0-80!
  if(!isMC && PIDcorrection) cutsLctoV0->EnableNsigmaDataDrivenCorrection(kTRUE, AliAODPidHF::kPbPb010);
  
  //event selection
  cutsLctoV0->SetUsePhysicsSelection(kTRUE);
  cutsLctoV0->SetTriggerClass("");
  if(!isMC) cutsLctoV0->SetTriggerMask(AliVEvent::kINT7 | AliVEvent::kSemiCentral | AliVEvent::kCentral);
  else      cutsLctoV0->SetTriggerMask(AliVEvent::kAny);
  cutsLctoV0->SetMinCentrality(minc);
  cutsLctoV0->SetMaxCentrality(maxc);
  cutsLctoV0->SetUseCentrality(AliRDHFCuts::kCentV0M); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,kCentInvalid
  cutsLctoV0->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
  cutsLctoV0->SetMaxVtxZ(10.);
  cutsLctoV0->SetCutOnzVertexSPD(3);
  
  cout<<"This is the object I'm going to save:"<<endl;
  cutsLctoV0->SetName(nameCuts.Data());
  cutsLctoV0->SetTitle(nameCuts.Data());
  cutsLctoV0->PrintAll();
  
  return cutsLctoV0;
  
}
