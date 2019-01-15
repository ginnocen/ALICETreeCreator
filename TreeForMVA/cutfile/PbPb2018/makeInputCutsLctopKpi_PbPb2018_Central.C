#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsDplustoKpipi.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="DplustoKpipiFilteringCuts"
 whichCuts=1, nameCuts="DplustoKpipiAnalysisCuts"
 */

void SetupCombinedPID(AliRDHFCutsLctopKpi *cutsObj,Double_t threshold);

void SetupCombinedPID(AliRDHFCutsLctopKpi *cutsObj,Double_t threshold) {

  cutsObj->GetPidHF()->SetCombDetectors(AliAODPidHF::kTPCTOF);
  for (Int_t ispecies=0;ispecies<AliPID::kSPECIES;++ispecies)
    cutsObj->SetPIDThreshold(static_cast<AliPID::EParticleType>(ispecies),threshold);
  cutsObj->GetPidHF()->SetUseCombined(kTRUE);
  cutsObj->GetPidHF()->SetUpCombinedPID();
  return;
}

AliRDHFCutsLctopKpi *makeInputCutsLctopKpi(Int_t whichCuts=0, TString nameCuts="LctoKpipiFilteringCuts", Float_t minc=0.,Float_t maxc=20.)
{
    
    AliRDHFCutsLctopKpi* cuts=new AliRDHFCutsLctopKpi();
    cuts->SetName(nameCuts.Data());
    cuts->SetTitle(nameCuts.Data());
    
    AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
    esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
    //default
    esdTrackCuts->SetRequireTPCRefit(kTRUE);
    esdTrackCuts->SetRequireITSRefit(kTRUE);
    if(whichCuts==0)esdTrackCuts->SetMinNClustersTPC(50);
    if(whichCuts==1)esdTrackCuts->SetMinNClustersTPC(70);
    if(whichCuts==1)esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
    esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);
    esdTrackCuts->SetEtaRange(-0.8,0.8);
    esdTrackCuts->SetMinDCAToVertexXY(0.);
    esdTrackCuts->SetPtRange(0.5,1.e10);
    //esdTrackCuts->SetPtRange(0.3,1.e10);
    esdTrackCuts->SetMaxDCAToVertexXY(1.);
    esdTrackCuts->SetMaxDCAToVertexZ(1.);
    esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0060*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    cuts->AddTrackCuts(esdTrackCuts);

    // cuts
    Int_t nptbins=2; Float_t ptlimits[2]={0.,1000000.};
    //cuts->SetStandardCutsPbPb2011();
    cuts->SetUseTrackSelectionWithFilterBits(kFALSE);
    Float_t cutsArrayLctopKpi[13]={0.13,0.5,0.625,0.,0.,0.0125,0.045,0.00625,0.8,0.25,0.,0.0375,0.5};
    cuts->SetPtBins(nptbins,ptlimits);
    cuts->SetCuts(13,cutsArrayLctopKpi);
    cuts->SetMinPtCandidate(4.);
    
  AliAODPidHF* pidObjp=new AliAODPidHF();
  AliAODPidHF* pidObjK=new AliAODPidHF();
  AliAODPidHF* pidObjpi=new AliAODPidHF();
  if(whichCuts==0 ){
    // PID
    // Set here since no default PIDHF object created in RDHF
    // 1. kaon
    Double_t sigmasK[5]={3.,1.,1.,3.,2.};
    pidObjK->SetSigma(sigmasK);
    pidObjK->SetAsym(kTRUE);
    pidObjK->SetMatch(1);
    pidObjK->SetTPC(kTRUE);
    pidObjK->SetTOF(kTRUE);
    Double_t plimK[2]={0.5,0.8};
    pidObjK->SetPLimit(plimK,2);
    pidObjK->SetTOFdecide(kTRUE);
    
    //2. pion
    AliAODPidHF* pidObjpi=new AliAODPidHF();
    pidObjpi->SetTPC(kTRUE);
    Double_t sigmaspi[5]={3.,0.,0.,0.,0.};
    pidObjpi->SetSigma(sigmaspi);
    //  pidObjpi->SetTOFdecide(kTRUE);
    
    // 3. proton
    AliAODPidHF* pidObjp=new AliAODPidHF();
    Double_t sigmasp[5]={3.,1.,1.,3.,2.};
    pidObjp->SetSigma(sigmasp);
    pidObjp->SetAsym(kTRUE);
    pidObjp->SetMatch(1);
    pidObjp->SetTPC(kTRUE);
    pidObjp->SetTOF(kTRUE);
    Double_t plimp[2]={1.,2.};
    pidObjp->SetPLimit(plimp,2);
    pidObjp->SetTOFdecide(kTRUE);
    
    cuts->SetPidprot(pidObjp);
    cuts->SetPidHF(pidObjK);
    cuts->SetPidpion(pidObjpi);
    
    cuts->SetUsePID(kFALSE);
  }
  else if(whichCuts==1){
    // PID
    cuts->SetUsePID(kTRUE);
    AliAODPidHF* pidObjp=new AliAODPidHF();
    AliAODPidHF* pidObjK=new AliAODPidHF();
    AliAODPidHF* pidObjpi=new AliAODPidHF();
    pidObjp->SetTPC(kTRUE);
    pidObjK->SetTPC(kTRUE);
    pidObjpi->SetTPC(kTRUE);
    pidObjp->SetTOF(kTRUE);
    pidObjK->SetTOF(kTRUE);
    pidObjpi->SetTOF(kTRUE);
    cuts->SetPidprot(pidObjp);
    cuts->SetPidHF(pidObjK);
    cuts->SetPidpion(pidObjpi);
    
    SetupCombinedPID(cuts,0.);
    cuts->SetPIDStrategy(AliRDHFCutsLctopKpi::kCombinedpPb);
  }



    //event selection
    cuts->SetTriggerClass("");
    cuts->SetTriggerMask(AliVEvent::kINT7 | AliVEvent::kCentral);
    cuts->SetMinCentrality(minc);
    cuts->SetMaxCentrality(maxc);
    cuts->SetUseCentrality(AliRDHFCuts::kCentV0M); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,k CentInvalid
	cuts->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
    cuts->SetMaxVtxZ(10.);
    cuts->SetCutOnzVertexSPD(3);
    cuts->SetKinkRejection(kTRUE);

    cout<<"This is the object I'm going to save:"<<endl;
    cuts->SetName(nameCuts.Data());
    cuts->SetTitle(nameCuts.Data());
    cuts->PrintAll();
    
    return cuts;
    
}


