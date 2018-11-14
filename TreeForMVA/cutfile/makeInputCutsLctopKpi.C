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



AliRDHFCutsLctopKpi *makeInputCutsLctopKpi(Int_t whichCuts=0, TString nameCuts="DplustoKpipiFilteringCuts", Float_t minc=0.,Float_t maxc=20.)
{
    
    AliRDHFCutsLctopKpi* cuts=new AliRDHFCutsLctopKpi();
    cuts->SetName(nameCuts.Data());
    cuts->SetTitle(nameCuts.Data());
    
    AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
    esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
    //default
    esdTrackCuts->SetRequireTPCRefit(kTRUE);
    esdTrackCuts->SetRequireITSRefit(kTRUE);
    esdTrackCuts->SetMinNClustersTPC(70);
    esdTrackCuts->SetMinNClustersITS(2); // default for run 2
    esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);
    esdTrackCuts->SetEtaRange(-0.8,0.8);
    esdTrackCuts->SetPtRange(0.3,1.e10);
    esdTrackCuts->SetMinDCAToVertexXY(0.);
    
    cuts->AddTrackCuts(esdTrackCuts);

    // cuts
    Int_t nptbins=2; Float_t ptlimits[2]={0.,1000000.};
//    cuts->SetStandardCutsPbPb2011();
    cuts->SetUseTrackSelectionWithFilterBits(kFALSE);
    Float_t cutsArrayLctopKpi[13]={0.18,0.3,0.3,0.,0.,0.,0.06,0.,0.,-1.,0.,0.05,0.3};
    cuts->SetPtBins(nptbins,ptlimits);
    cuts->SetCuts(13,cutsArrayLctopKpi);
    cuts->AddTrackCuts(esdTrackCuts);
    //cuts->SetMinPtCandidate(1.);
    
    // PID not added for now, to do
//    if(whichCuts==0 ){
    cuts->SetUsePID(kFALSE);
//    }
//    else if(whichCuts==1){
//      // PID
//      cuts->SetUsePID(kTRUE);
//      AliAODPidHF* pidObjp=new AliAODPidHF();
//      AliAODPidHF* pidObjK=new AliAODPidHF();
//      AliAODPidHF* pidObjpi=new AliAODPidHF();
//      pidObjp->SetTPC(kTRUE);
//      pidObjK->SetTPC(kTRUE);
//      pidObjpi->SetTPC(kTRUE);
//      pidObjp->SetTOF(kTRUE);
//      pidObjK->SetTOF(kTRUE);
//      pidObjpi->SetTOF(kTRUE);
//      cuts->SetPidprot(pidObjp);
//      cuts->SetPidHF(pidObjK);
//      cuts->SetPidpion(pidObjpi);
//
//      SetupCombinedPID(cuts,0.);
//      cuts->SetPIDStrategy(AliRDHFCutsLctopKpi::kCombinedpPb);
//    }
    //event selection
    cuts->SetTriggerClass("");
    cuts->SetTriggerMask(AliVEvent::kINT7);
    cuts->SetUseCentrality(AliRDHFCuts::kCentOff); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,k CentInvalid
		cuts->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent);
    cuts->SetMaxVtxZ(10.);
    cuts->SetCutOnzVertexSPD(2);
    cuts->SetKinkRejection(kTRUE);
    
    cout<<"This is the object I'm going to save:"<<endl;
    cuts->SetName(nameCuts.Data());
    cuts->SetTitle(nameCuts.Data());
    cuts->PrintAll();
    
    return cuts;
    
}


