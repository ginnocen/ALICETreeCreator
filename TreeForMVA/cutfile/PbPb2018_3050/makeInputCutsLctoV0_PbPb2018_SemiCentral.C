#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsLctoV0.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="LctoV0FilteringCuts"
 whichCuts=1, nameCuts="LctoV0AnalysisCuts"
 */

const Int_t nvars=21;
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
//        V0 type

AliRDHFCutsLctoV0 *makeInputCutsLctoV0(Int_t whichCuts=0, TString nameCuts="LctoV0FilteringCuts", Float_t minc=30.,Float_t maxc=50.,Bool_t isMC=kFALSE)
{
  
  AliRDHFCutsLctoV0* cutsLctoV0=new AliRDHFCutsLctoV0();
  cutsLctoV0->SetName(nameCuts.Data());
  cutsLctoV0->SetTitle(nameCuts.Data());
  
  //UPDATE 07/05/19, use the same track quality cuts for filtering and analysis cuts
  //single track cuts
  AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
  esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
  esdTrackCuts->SetRequireTPCRefit(kTRUE);
  esdTrackCuts->SetRequireITSRefit(kTRUE);
  //Should not use SetMinNClustersTPC anymore, not well described in MC
  //Two lines below replace this cut (for value 70)
  //  esdTrackCuts->SetMinNClustersTPC(50);
  esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
  esdTrackCuts->SetMinNCrossedRowsTPC(70);
  esdTrackCuts->SetMinNClustersITS(0);
  esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kFirst);
  esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))"); //filtering
  esdTrackCuts->SetMinDCAToVertexXY(0.);
  esdTrackCuts->SetMaxDCAToVertexXY(1.);
  esdTrackCuts->SetMaxDCAToVertexZ(1.);
  esdTrackCuts->SetPtRange(0.3,1.e10);    //Changed from 0.5->0.3 for bin 2-4, for pT>4 effect countered with topo cuts
  esdTrackCuts->SetEtaRange(-0.8,+0.8);
  esdTrackCuts->SetAcceptKinkDaughters(kFALSE);
  
  // V0 daughters cuts
  AliESDtrackCuts* esdTrackCutsV0daughters=new AliESDtrackCuts();
  esdTrackCutsV0daughters->SetRequireSigmaToVertex(kFALSE);
  esdTrackCutsV0daughters->SetRequireTPCRefit(kTRUE);
  esdTrackCutsV0daughters->SetRequireITSRefit(kFALSE);//(kTRUE);
  esdTrackCutsV0daughters->SetMinNClustersITS(0);//(4); // default is 5
  //Should not use SetMinNClustersTPC anymore, not well described in MC
  //Two lines below replace this cut (for value 70)
  //  esdTrackCuts->SetMinNClustersTPC(70);
  esdTrackCutsV0daughters->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
  esdTrackCutsV0daughters->SetMinNCrossedRowsTPC(70);
  esdTrackCutsV0daughters->SetMinDCAToVertexXY(0.);
  esdTrackCutsV0daughters->SetPtRange(0.,1.e10);
  esdTrackCutsV0daughters->SetEtaRange(-0.8,+0.8);
  esdTrackCutsV0daughters->SetAcceptKinkDaughters(kFALSE);
  
  cutsLctoV0->AddTrackCuts(esdTrackCuts);
  cutsLctoV0->AddTrackCutsV0daughters(esdTrackCutsV0daughters);
  cutsLctoV0->SetKinkRejection(!esdTrackCuts->GetAcceptKinkDaughters());
  cutsLctoV0->SetUseTrackSelectionWithFilterBits(kTRUE);
  
  if(whichCuts==0){
    const Int_t nptbins=2;
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    ptbins[0]=2.;
    ptbins[1]=4.;
    ptbins[2]=999.;
    cutsLctoV0->SetPtBins(nptbins+1,ptbins);
    Float_t cuts[nptbins][nvars]={
      0.2,0.,0.03,0.05,0.3,0.0,0.0,1000.,1000.,0.998,3.,1000.,0.,0.,0.,0.5,9999.,-9999.,-9999.,-9999.,1,
      0.2,0.,0.03,0.05,0.5,0.0,0.0,1000.,1000.,0.998,3.,1000.,0.,0.,0.,0.5,9999.,-9999.,-9999.,-9999.,1
    };
    
    Float_t** prodcutsval;
    prodcutsval=new Float_t*[nvars];
    for(Int_t ic=0;ic<nvars;ic++){prodcutsval[ic]=new Float_t[nptbins];}
    for(Int_t ipt=0; ipt<nptbins; ipt++){
      for(Int_t ic=0; ic<nvars; ic++){
        prodcutsval[ic][ipt]=cuts[ipt][ic];
      }
    }
    
    //Bin [1-2] can still be added, but need some optimisation wrt size
    cutsLctoV0->SetMinPtCandidate(2.);
    cutsLctoV0->SetCuts(nvars,nptbins,prodcutsval);
    
    cutsLctoV0->SetPidSelectionFlag(11);
    //pid settings
    //1. bachelor: default one
    AliAODPidHF* pidObjBachelor = new AliAODPidHF();
    Double_t sigmasBac[5]={4.,4.,4.,4.,4.}; // 0, 1(A), 2(A) -> TPC; 3 -> TOF; 4 -> ITS
    pidObjBachelor->SetSigma(sigmasBac);
    pidObjBachelor->SetAsym(kFALSE);
    pidObjBachelor->SetMatch(1);
    pidObjBachelor->SetTPC(kTRUE);
    pidObjBachelor->SetTOF(kTRUE);
    pidObjBachelor->SetTOFdecide(kFALSE);
    
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
      0.2, 1000., 0.01, 9999., 1., 0.1, 0.1, 1., 0.4, 0.9998, 0.1, 1.5, 0., 0.005, 0.1, 1., 0.5, -1., 0.003, 0.15, 1.,
      0.2, 1000., 0.01, 9999., 1., 0.1, 0.1, 1., 0.4, 0.9998, 0.1, 1.5, 0., 0.005, 0.1, 1., 0.5, -1., 0.003, 0.15, 1.,
      0.2, 1000., 0.01, 9999., 1., 0.1, 0.1, 1., 0.4, 0.9998, 0.1, 1.5, 0., 0.005, 0.1, 1., 0.5, -1., 0.003, 0.15, 1.,
      0.2, 1000., 0.01, 9999., 1., 0.1, 0.1, 1., 0.4, 0.9998, 0.1, 1.5, 0., 0.005, 0.1, 1., 0.5, -1., 0.003, 0.15, 1.,
      0.2, 1000., 0.01, 9999., 1., 0.1, 0.1, 1., 0.4, 0.9998, 0.1, 1.5, 0., 0.005, 0.1, 1., 0.5, -1., 0.003, 0.15, 1.
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
    //1. bachelor: default one
    AliAODPidHF* pidObjBachelor = new AliAODPidHF();
    Double_t sigmasBac[5]={3.,3.,3.,3.,3.}; // 0, 1(A), 2(A) -> TPC; 3 -> TOF; 4 -> ITS
    pidObjBachelor->SetSigma(sigmasBac);
    pidObjBachelor->SetAsym(kFALSE);
    pidObjBachelor->SetMatch(1);
    pidObjBachelor->SetTPC(kTRUE);
    pidObjBachelor->SetTOF(kTRUE);
    pidObjBachelor->SetTOFdecide(kFALSE);
    
    cutsLctoV0->SetPidHF(pidObjBachelor);
    Bool_t pidflag=kTRUE;
    cutsLctoV0->SetUsePID(pidflag);
    
    cutsLctoV0->SetMinPtCandidate(1.0);
    cutsLctoV0->SetMaxPtCandidate(50.);
    
  }
  
  //Do not recalculate the vertex
  cutsLctoV0->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp

  //Temporary PID fix for 2018 PbPb (only to be used on data)
  if(!isMC) cutsLctoV0->EnableNsigmaDataDrivenCorrection(kTRUE, AliAODPidHF::kPbPb3050);

  //event selection
  cutsLctoV0->SetUsePhysicsSelection(kTRUE);
  cutsLctoV0->SetTriggerClass("");
  if(!isMC) cutsLctoV0->SetTriggerMask(AliVEvent::kINT7 | AliVEvent::kSemiCentral);
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
