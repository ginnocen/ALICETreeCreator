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

AliRDHFCutsLctopKpi *makeInputCutsLbtoLcpi(Int_t whichCuts=0, TString nameCuts="LbtoLcpiFilteringCuts", Float_t minc=30., Float_t maxc=50., Bool_t isMC=kFALSE, Int_t OptPreSelect = 1, Int_t TPCClsPID = 50, Bool_t PIDcorrection=kTRUE)
{
  
  cout << "\n\033[1;31m--Warning (08/06/20)--\033[0m\n";
  cout << "  Don't blindly trust these cuts." << endl;
  cout << "  Relatively old and never tested." << endl;
  cout << "\033[1;31m----------------------\033[0m\n\n";

  AliRDHFCutsLctopKpi* cuts=new AliRDHFCutsLctopKpi();
  cuts->SetName(nameCuts.Data());
  cuts->SetTitle(nameCuts.Data());
  
  //UPDATE 21/06/19, use the same track quality cuts for filtering and analysis cuts
  AliESDtrackCuts *esdTrackCuts = new AliESDtrackCuts();
  esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
  esdTrackCuts->SetRequireTPCRefit(kTRUE);
  esdTrackCuts->SetRequireITSRefit(kTRUE);
  //esdTrackCuts->SetMinNClustersITS(4);
  esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,
                                         AliESDtrackCuts::kAny);
  // |d0|>25 micron for pt<2GeV, no cut above 2
  esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
  esdTrackCuts->SetMaxDCAToVertexXY(1.);
  esdTrackCuts->SetMaxDCAToVertexZ(1.);
  esdTrackCuts->SetPtRange(0.4,1.e10);
  esdTrackCuts->SetEtaRange(-0.8,+0.8);
  //Should not use SetMinNClustersTPC anymore, not well described in MC
  //Two lines below replace this cut (for value 70)
  //  esdTrackCuts->SetMinNClustersTPC(70);
  esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
  esdTrackCuts->SetMinNCrossedRowsTPC(70);

  cuts->AddTrackCuts(esdTrackCuts);
  //UPDATE 08/06/20, set to kTRUE as should be done for all other HF hadrons (pK0s was true, others false)
  cuts->SetUseTrackSelectionWithFilterBits(kTRUE);
  cuts->SetKinkRejection(kTRUE);
  
  //UPDATE 08/06/20, Add cut on TPC clusters for PID (similar to geometrical cut)
  cuts->SetMinNumTPCClsForPID(TPCClsPID);
  
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
  
  rdcutsvalmine[0][0]=0.06;   //inv mass window
  rdcutsvalmine[1][0]=0.5;    // pTK
  rdcutsvalmine[2][0]=0.5;   // pTP
  rdcutsvalmine[3][0]=0.;      // d0K
  rdcutsvalmine[4][0]=0.;      // d0Pi
  rdcutsvalmine[5][0]=0.0;  // dist12
  rdcutsvalmine[6][0]=0.06;   // sigmavert
  rdcutsvalmine[7][0]=0.00; // dist prim-sec
  rdcutsvalmine[8][0]=0.;     // pM=Max{pT1,pT2,pT3}
  rdcutsvalmine[9][0]=0.;    // cosThetaPoint
  rdcutsvalmine[10][0]=0.;     // Sum d0^2
  rdcutsvalmine[11][0]=0.05; // dca cut
  rdcutsvalmine[12][0]=0.5;    // cut on pTpion [GeV/c]
  //Used values for Lc->pKpi analysis taken at 20/08/19 but tightened InvMass cut to 0.06
  
  cuts->SetCuts(nvars,nptbinsLc,rdcutsvalmine);
  cuts->SetMinPtCandidate(2.);
  
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
    
    cuts->SetUsePID(kTRUE);
  }
  else if(whichCuts==1){
    // PID
    cuts->SetUsePID(kTRUE);
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
  }
  
  //UPDATE 08/06/20: PreSelect, acting before FillRecoCasc.
  //NOTE: actual function not implemented for all HF hadrons yet (please check)
  cuts->SetUsePreSelect(OptPreSelect);

  //Do not recalculate the vertex
  cuts->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
  
  //Temporary PID fix for 2018 PbPb (only to be used on data)
  if(!isMC && PIDcorrection) cuts->EnableNsigmaDataDrivenCorrection(kTRUE, AliAODPidHF::kPbPb3050);

  //event selection
  cuts->SetUsePhysicsSelection(kTRUE);
  cuts->SetTriggerClass("");
  if(!isMC) cuts->SetTriggerMask(AliVEvent::kINT7 | AliVEvent::kSemiCentral);
  else      cuts->SetTriggerMask(AliVEvent::kAny);
  cuts->SetMinCentrality(minc);
  cuts->SetMaxCentrality(maxc);
  cuts->SetUseCentrality(AliRDHFCuts::kCentV0M); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,kCentInvalid
  
  cuts->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
  cuts->SetMaxVtxZ(10.);
  cuts->SetCutOnzVertexSPD(3);
  
  cout<<"This is the object I'm going to save:"<<endl;
  cuts->SetName(nameCuts.Data());
  cuts->SetTitle(nameCuts.Data());
  cuts->PrintAll();
  
  return cuts;
}


