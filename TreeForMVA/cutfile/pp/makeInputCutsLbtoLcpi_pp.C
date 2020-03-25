#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsLctopKpi.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="LcpKpiFilteringCuts"
 whichCuts=1, nameCuts="LcpKpiAnalysisCuts"
 */

void SetupCombinedPID2(AliRDHFCutsLctopKpi *cutsObj,Double_t threshold);

void SetupCombinedPID2(AliRDHFCutsLctopKpi *cutsObj,Double_t threshold) {
  
  cutsObj->GetPidHF()->SetCombDetectors(AliAODPidHF::kTPCTOF);
  for (Int_t ispecies=0;ispecies<AliPID::kSPECIES;++ispecies)
    cutsObj->SetPIDThreshold(static_cast<AliPID::EParticleType>(ispecies),threshold);
  cutsObj->GetPidHF()->SetUseCombined(kTRUE);
  cutsObj->GetPidHF()->SetUpCombinedPID();
  return;
}



AliRDHFCutsLctopKpi *makeInputCutsLbtoLcpi_pp(Int_t whichCuts=0, TString nameCuts="LbtoLcpiFilteringCuts", Float_t minc=0.,Float_t maxc=100.)
{
  
  AliRDHFCutsLctopKpi* cuts=new AliRDHFCutsLctopKpi();
  cuts->SetName(nameCuts.Data());
  cuts->SetTitle(nameCuts.Data());
  
  //UPDATE 21/06/19, use the same track quality cuts for filtering and analysis cuts
  AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
  esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
  //default
  esdTrackCuts->SetRequireTPCRefit(kTRUE);
  esdTrackCuts->SetRequireITSRefit(kTRUE);
  //Should not use SetMinNClustersTPC anymore, not well described in MC
  //Two lines below replace this cut (for value 70)
  //  esdTrackCuts->SetMinNClustersTPC(70);
  esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
  esdTrackCuts->SetMinNCrossedRowsTPC(70);
  esdTrackCuts->SetMinNClustersITS(2); // default for run 2
  esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);
  esdTrackCuts->SetEtaRange(-0.8,0.8);
  esdTrackCuts->SetPtRange(0.3,1.e10);
  esdTrackCuts->SetMinDCAToVertexXY(0.);
  cuts->AddTrackCuts(esdTrackCuts);
  cuts->SetUseTrackSelectionWithFilterBits(kFALSE);
  
  // cuts
  Int_t nptbins=2;
  Float_t ptlimits[2]={0.,1000000.};
  cuts->SetPtBins(nptbins,ptlimits);
  Float_t cutsArrayLctopKpi[13]={0.06,0.3,0.3,0.,0.,0.,0.04,0.,0.,0.8,0.,0.05,0.3};
  //Used values for Lc->pKpi analysis taken at 05/08/19 but tightened InvMass cut to 0.06
  
  cuts->SetMinPtCandidate(1.);
  cuts->SetCuts(13,cutsArrayLctopKpi);
  
  AliAODPidHF* pidObjp=new AliAODPidHF();
  AliAODPidHF* pidObjK=new AliAODPidHF();
  AliAODPidHF* pidObjpi=new AliAODPidHF();
  if(whichCuts==0 ){
    //UPDATE 21/06/19, enable PID selection to reduce output size
    // PID
    // Set here since no default PIDHF object created in RDHF
    // 1. kaon
    Double_t sigmasK[5]={3.,3.,3.,3.,3.};//{3.,1.,1.,3.,2.};
    pidObjK->SetSigma(sigmasK);
    pidObjK->SetAsym(kFALSE);//(kTRUE);
    pidObjK->SetMatch(1);
    pidObjK->SetTPC(kTRUE);
    pidObjK->SetTOF(kTRUE);
    //Double_t plimK[2]={0.5,0.8};
    //pidObjK->SetPLimit(plimK,2);
    pidObjK->SetTOFdecide(kFALSE);//(kTRUE);
    
    //2. pion
    Double_t sigmaspi[5]={3.,3.,3.,3.,3.};//{3.,0.,0.,0.,0.};
    pidObjpi->SetSigma(sigmaspi);
    pidObjpi->SetAsym(kFALSE);
    pidObjpi->SetMatch(1);
    pidObjpi->SetTPC(kTRUE);
    pidObjpi->SetTOF(kTRUE);
    pidObjpi->SetTOFdecide(kFALSE);
    
    // 3. proton
    Double_t sigmasp[5]={3.,3.,3.,3.,3.};//{3.,1.,1.,3.,2.};
    pidObjp->SetSigma(sigmasp);
    pidObjp->SetAsym(kFALSE);//(kTRUE);
    pidObjp->SetMatch(1);
    pidObjp->SetTPC(kTRUE);
    pidObjp->SetTOF(kTRUE);
    //Double_t plimp[2]={1.,2.};
    //pidObjp->SetPLimit(plimp,2);
    pidObjp->SetTOFdecide(kFALSE);//(kTRUE);
    
    cuts->SetPidprot(pidObjp);
    cuts->SetPidHF(pidObjK);
    cuts->SetPidpion(pidObjpi);
    
    Bool_t pidflag=kTRUE;
    cuts->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for filtering cuts"<<endl;
    else cout<<"PID is not used for filtering cuts"<<endl;
    
  } else if(whichCuts==1){
    // PID
    pidObjp->SetMatch(1);
    pidObjK->SetMatch(1);
    pidObjpi->SetMatch(1);
    pidObjp->SetTPC(kTRUE);
    pidObjK->SetTPC(kTRUE);
    pidObjpi->SetTPC(kTRUE);
    pidObjp->SetTOF(kTRUE);
    pidObjK->SetTOF(kTRUE);
    pidObjpi->SetTOF(kTRUE);
    cuts->SetPidprot(pidObjp);
    cuts->SetPidHF(pidObjK);
    cuts->SetPidpion(pidObjpi);
    
    SetupCombinedPID2(cuts,0.);
    cuts->SetPIDStrategy(AliRDHFCutsLctopKpi::kCombinedpPb);
    
    Bool_t pidflag=kTRUE;
    cuts->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for analysis cuts"<<endl;
    else cout<<"PID is not used for analysis cuts"<<endl;
  }
  
  //11/08/19:
  //Not enabled for now because it is missing in AliRDHFCuts
  //Also not sure if this is used for Lc in pp
  cuts->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
  
  //event selection
  cuts->SetUsePhysicsSelection(kTRUE);
  cuts->SetTriggerClass("");
  //Since X/08/19 we set the triggers in wagon code/runAnalysis script if needed
  cuts->SetTriggerMask(AliVEvent::kAny);
  cuts->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent);
  cuts->SetMinContribPileupMV(5);
  cuts->SetMaxVtxZ(10.);
  cuts->SetCutOnzVertexSPD(3);
  cuts->SetMinVtxContr(1);
  
  cout<<"This is the object I'm going to save:"<<endl;
  cuts->SetName(nameCuts.Data());
  cuts->SetTitle(nameCuts.Data());
  cuts->PrintAll();
  
  return cuts;
  
}


