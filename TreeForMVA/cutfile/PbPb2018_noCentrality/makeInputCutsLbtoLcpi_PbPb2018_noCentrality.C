#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsLctopKpi.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="LbtoLcpiFilteringCuts"
 whichCuts=1, nameCuts="LbtoLcpiAnalysisCuts"
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

AliRDHFCutsLctopKpi *makeInputCutsLbtoLcpi(Int_t whichCuts=0, TString nameCuts="LctoKpipiFilteringCuts", Float_t minc=0.,Float_t maxc=100.,Bool_t usePID=kTRUE)
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
  //  esdTrackCuts->SetMinNClustersTPC(50);
  esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
  esdTrackCuts->SetMinNCrossedRowsTPC(70);
  esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);
  esdTrackCuts->SetEtaRange(-0.8,0.8);
  esdTrackCuts->SetPtRange(0.3,1.e10);
  esdTrackCuts->SetMaxDCAToVertexXY(1.);
  esdTrackCuts->SetMaxDCAToVertexZ(1.);
  esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0015*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
  
  cuts->SetStandardCutsPbPb2010();
  cuts->AddTrackCuts(esdTrackCuts);
  cuts->SetUseTrackSelectionWithFilterBits(kFALSE);
  
  // cuts
  const Int_t nvars=13;
  const Int_t nptbinsLc=1;
  Float_t* ptbins;
  ptbins=new Float_t[nptbinsLc+1];
  ptbins[0]=0.;
  ptbins[1]=999.;
  
  cuts->SetPtBins(nptbinsLc+1,ptbins);
  
  Float_t** rdcutsvalmine;
  rdcutsvalmine=new Float_t*[nvars];
  for(Int_t iv=0;iv<nvars;iv++){
    rdcutsvalmine[iv]=new Float_t[nptbinsLc];
  }

  //0-999
  rdcutsvalmine[0][0]=0.07;   //inv mass window
  rdcutsvalmine[1][0]=0.3;    // pTK
  rdcutsvalmine[2][0]=0.3;    // pTP
  rdcutsvalmine[3][0]=0.0;    // d0K
  rdcutsvalmine[4][0]=0.0;    // d0Pi
  rdcutsvalmine[5][0]=0.0;  // dist12
  rdcutsvalmine[6][0]=0.06;  // sigmavert
  rdcutsvalmine[7][0]=0.0;// dist prim-sec
  rdcutsvalmine[8][0]=0.0;    // pM=Max{pT1,pT2,pT3}
  rdcutsvalmine[9][0]=0.0;    // cosThetaPoint
  rdcutsvalmine[10][0]=0.0;   // Sum d0^2
  rdcutsvalmine[11][0]=0.05; // dca cut
  rdcutsvalmine[12][0]=0.3;   // cut on pTpion [GeV/c]

  cuts->SetCuts(nvars,nptbinsLc,rdcutsvalmine);
  cuts->SetMinPtCandidate(1.);
  
  AliAODPidHF* pidObjp=new AliAODPidHF();
  AliAODPidHF* pidObjK=new AliAODPidHF();
  AliAODPidHF* pidObjpi=new AliAODPidHF();
  
  if(whichCuts==0 ){
    // PID
    // Set here since no default PIDHF object created in RDHF
    // 1. kaon
    Double_t sigmasK[5]={3.,3.,3.,3.,3.};
    pidObjK->SetSigma(sigmasK);
    pidObjK->SetAsym(kFALSE);
    pidObjK->SetMatch(1);
    pidObjK->SetTPC(kTRUE);
    pidObjK->SetTOF(kTRUE);
    //Double_t plimK[2]={0.5,0.8};
    //pidObjK->SetPLimit(plimK,2);
    pidObjK->SetTOFdecide(kFALSE);
    
    //2. pion
    AliAODPidHF* pidObjpi=new AliAODPidHF();
    Double_t sigmaspi[5]={3.,3.,3.,3.,3.};
    pidObjpi->SetSigma(sigmaspi);
    pidObjpi->SetAsym(kFALSE);
    pidObjpi->SetMatch(1);
    pidObjpi->SetTPC(kTRUE);
    pidObjpi->SetTOF(kTRUE);
    pidObjpi->SetTOFdecide(kTRUE);
    
    // 3. proton
    AliAODPidHF* pidObjp=new AliAODPidHF();
    Double_t sigmasp[5]={3.,3.,3.,3.,3.};
    pidObjp->SetSigma(sigmasp);
    pidObjp->SetAsym(kFALSE);
    pidObjp->SetMatch(1);
    pidObjp->SetTPC(kTRUE);
    pidObjp->SetTOF(kTRUE);
    //Double_t plimp[2]={1.,2.};
    //pidObjp->SetPLimit(plimp,2);
    pidObjp->SetTOFdecide(kFALSE);
    
    cuts->SetPidprot(pidObjp);
    cuts->SetPidHF(pidObjK);
    cuts->SetPidpion(pidObjpi);
    
    Bool_t pidflag=usePID;
    cuts->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for filtering cuts"<<endl;
    else cout<<"PID is not used for filtering cuts"<<endl;

    cuts->AddTrackCuts(esdTrackCuts);
  } else if(whichCuts==1){
    // PID
    AliAODPidHF* pidObjp=new AliAODPidHF();
    AliAODPidHF* pidObjK=new AliAODPidHF();
    AliAODPidHF* pidObjpi=new AliAODPidHF();
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
    
    Bool_t pidflag=usePID;
    cuts->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for analysis cuts"<<endl;
    else cout<<"PID is not used for analysis cuts"<<endl;
  }
  
  //Do not recalculate the vertex
  cuts->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
  
  //event selection
  cuts->SetUsePhysicsSelection(kFALSE);
  cuts->SetTriggerClass("");
  cuts->SetTriggerMask(AliVEvent::kAny);
  cuts->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
  cuts->SetMaxVtxZ(10.);
  cuts->SetCutOnzVertexSPD(3);
  
  cout<<"This is the object I'm going to save:"<<endl;
  cuts->SetName(nameCuts.Data());
  cuts->SetTitle(nameCuts.Data());
  cuts->PrintAll();
  
  return cuts;
}


