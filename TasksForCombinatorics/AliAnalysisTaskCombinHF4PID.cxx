/**************************************************************************
 * Copyright(c) 1998-2018, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: $ */

//*************************************************************************
// Class AliAnalysisTaskCombinHF4PID
// AliAnalysisTaskSE to build D meson candidates by combining tracks
//  background is computed LS and track rotations is
// Authors: F. Prino, A. Rossi
/////////////////////////////////////////////////////////////

#include <TList.h>
#include <TH1F.h>
#include <TDatabasePDG.h>
#include <TTree.h>
#include <TChain.h>

#include "AliAnalysisManager.h"
#include "AliInputEventHandler.h"
#include "AliAODHandler.h"
#include "AliAODEvent.h"
#include "AliAODMCParticle.h"
#include "AliAODMCHeader.h"
#include "AliAODVertex.h"
#include "AliAODTrack.h"
#include "AliAODRecoDecayHF3Prong.h"
#include "AliAnalysisTaskCombinHF4PID.h"

/// \cond CLASSIMP
ClassImp(AliAnalysisTaskCombinHF4PID);
/// \endcond

//________________________________________________________________________
AliAnalysisTaskCombinHF4PID::AliAnalysisTaskCombinHF4PID():
  AliAnalysisTaskSE(),
  fOutput(0x0),
  fHistNEvents(0x0),
  fCandTree(0x0),
  fTreeVarFloat(0x0),
  fTreeVarInt(0x0),
  fFilterMask(16),
  fTrCuts(0x0),
  fAnalysisCuts(0x0),
  fMinMass(1.720),
  fMaxMass(2.150),
  fMeson(kDzero),
  fOptForTOF(1),
  fReadMC(kFALSE),
  fGoUpToQuark(kTRUE),
  fKeepNegID(kFALSE)
{
  /// default constructor
}

//________________________________________________________________________
AliAnalysisTaskCombinHF4PID::AliAnalysisTaskCombinHF4PID(Int_t meson, AliRDHFCuts* analysiscuts):
  AliAnalysisTaskSE("DmesonCombin"),
  fOutput(0x0),
  fHistNEvents(0x0),
  fCandTree(0x0),
  fTreeVarFloat(0x0),
  fTreeVarInt(0x0),
  fFilterMask(16),
  fTrCuts(0x0),
  fAnalysisCuts(analysiscuts),
  fMinMass(1.720),
  fMaxMass(2.150),
  fMeson(meson),
  fOptForTOF(1),
  fReadMC(kFALSE),
  fGoUpToQuark(kTRUE),
  fKeepNegID(kFALSE)
{
  /// standard constructor

  DefineInput(0, TChain::Class());
  DefineOutput(1,TList::Class());
  DefineOutput(2, TTree::Class());
}

//________________________________________________________________________
AliAnalysisTaskCombinHF4PID::~AliAnalysisTaskCombinHF4PID()
{
  //
  /// Destructor
  //
  if(fOutput && !fOutput->IsOwner()){
    delete fHistNEvents;
    delete fCandTree;
  }

  delete fOutput;
  delete fTrCuts;
  delete fAnalysisCuts;
  delete [] fTreeVarFloat;
  delete [] fTreeVarInt;

}

//________________________________________________________________________
void AliAnalysisTaskCombinHF4PID::UserCreateOutputObjects()
{
  /// Create the output container
  //
  if(fDebug > 1) printf("AnalysisTaskCombinHF4PID::UserCreateOutputObjects() \n");
  
  fOutput = new TList();
  fOutput->SetOwner();
  fOutput->SetName("OutputHistos");
  
  fHistNEvents = new TH1F("hNEvents", "number of events ",9,-0.5,8.5);
  fHistNEvents->GetXaxis()->SetBinLabel(1,"nEventsAnal");
  fHistNEvents->GetXaxis()->SetBinLabel(2,"n. passing IsEvSelected");
  fHistNEvents->GetXaxis()->SetBinLabel(3,"n. rejected due to trigger");
  fHistNEvents->GetXaxis()->SetBinLabel(4,"n. rejected due to phys sel");
  fHistNEvents->GetXaxis()->SetBinLabel(5,"n. rejected due to not reco vertex");
  fHistNEvents->GetXaxis()->SetBinLabel(6,"n. rejected for contr vertex");
  fHistNEvents->GetXaxis()->SetBinLabel(7,"n. rejected for vertex out of accept");
  fHistNEvents->GetXaxis()->SetBinLabel(8,"n. rejected for pileup events");
  fHistNEvents->GetXaxis()->SetBinLabel(9,"no. of out centrality events");
  
  fHistNEvents->GetXaxis()->SetNdivisions(1,kFALSE);
  fHistNEvents->SetMinimum(0);
  fOutput->Add(fHistNEvents);
  
  fCandTree = new TTree("candTree", "Tree for analysis");
  TString floatVarName[kNumOfFloatVar];
  fTreeVarFloat = new Float_t[kNumOfFloatVar];
  floatVarName[0]="ptD";   // D meson candidate pt
  floatVarName[1]="yD";    // D meson candidate rapidity
  floatVarName[2]="massD"; // D meson candidate inv. mass
  floatVarName[3]="pdau0"; // First daughter momentum 
  floatVarName[4]="dedx0"; // First daughter TOF
  floatVarName[5]="tof0"; // First daughter dE/dx
  floatVarName[6]="dca0"; // First daughter imp par
  floatVarName[7]="sigdca0"; // First daughter imp par unc
  floatVarName[8]="chisq0";    // First daughter chi2 in TPC
  floatVarName[9]="pdau1"; // Second daughter momentum 
  floatVarName[10]="dedx1"; // Second daughter dE/dx
  floatVarName[11]="tof1"; // Second daughter TOF
  floatVarName[12]="dca1"; // Second daughter imp par
  floatVarName[13]="sigdca1"; // Second daughter imp par unc
  floatVarName[14]="chisq1";    // Second daughter chi2 in TPC
  floatVarName[15]="pdau2"; // Third daughter momentum 
  floatVarName[16]="dedx2"; // Third daughter dE/dx 
  floatVarName[17]="tof2"; // Third daughter TOF 
  floatVarName[18]="dca2"; // Third daughter imp par
  floatVarName[19]="sigdca2"; // Third daughter imp par unc
  floatVarName[20]="chisq2";    // Third daughter chi2 in TPC

  for(Int_t ivar=0; ivar<kNumOfFloatVar; ivar++){
    fCandTree->Branch(floatVarName[ivar].Data(),&fTreeVarFloat[ivar],Form("%s/F",floatVarName[ivar].Data()));
  }
  TString intVarName[kNumOfIntVar];
  fTreeVarInt = new Int_t[kNumOfIntVar];
  intVarName[0]="pdgD"; // D meson PDG code
  intVarName[1]="pdg0"; // First daughter PDG code
  intVarName[2]="pdg1"; // Third daughter PDG code
  intVarName[3]="pdg2"; // Second daughter PDG code
  intVarName[4]="itscl0"; // First daughter ITS clusters
  intVarName[5]="tpccl0"; // First daughter TPC clusters
  intVarName[6]="itscl1"; // Second daughter ITS clusters
  intVarName[7]="tpccl1"; // Second daughter TPC clusters
  intVarName[8]="itscl2"; // Third daughter ITS clusters
  intVarName[9]="tpccl2"; // Third daughter TPC clusters
  for(Int_t ivar=0; ivar<kNumOfIntVar; ivar++){
    fCandTree->Branch(intVarName[ivar].Data(),&fTreeVarInt[ivar],Form("%s/I",intVarName[ivar].Data()));
  }

  PostData(1,fOutput);
  PostData(2,fCandTree);
}

//________________________________________________________________________
void AliAnalysisTaskCombinHF4PID::UserExec(Option_t */*option*/){
  /// Build the 3-track combinatorics (+-+ and -+-) for D+->Kpipi decays
  
  AliAODEvent *aod = dynamic_cast<AliAODEvent*> (InputEvent());
  if(!aod && AODEvent() && IsStandardAOD()) {
    // In case there is an AOD handler writing a standard AOD, use the AOD
    // event in memory rather than the input (ESD) event.
    aod = dynamic_cast<AliAODEvent*> (AODEvent());
  }
  if(!aod){
    printf("AliAnalysisTaskCombinHF4PID::UserExec: AOD not found!\n");
    return;
  }
  
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  AliInputEventHandler *inputHandler=(AliInputEventHandler*)mgr->GetInputEventHandler();
  AliPIDResponse *pidResp=inputHandler->GetPIDResponse();

  // fix for temporary bug in ESDfilter
  // the AODs with null vertex pointer didn't pass the PhysSel
  if(!aod->GetPrimaryVertex() || TMath::Abs(aod->GetMagneticField())<0.001) return;

  // Reject events with trigger mask 0 of the LHC13d3 production
  // For these events the ITS layers are skipped in the trakcing
  // and the vertex reconstruction efficiency from tracks is biased
  if(fReadMC){
    Int_t runnumber = aod->GetRunNumber();
    if(aod->GetTriggerMask()==0 &&
       (runnumber>=195344 && runnumber<=195677)){
      return;
    }
  }
  
  fHistNEvents->Fill(0); // count event
  // Post the data already here
  PostData(1,fOutput);
  PostData(2,fCandTree);

  Bool_t isEvSel=fAnalysisCuts->IsEventSelected(aod);
  if(fAnalysisCuts->IsEventRejectedDueToTrigger() || fAnalysisCuts->IsEventRejectedDuePhysicsSelection()){
    if(fAnalysisCuts->IsEventRejectedDueToTrigger()) fHistNEvents->Fill(2);
    if(fAnalysisCuts->IsEventRejectedDuePhysicsSelection()) fHistNEvents->Fill(3);
  }else{
    if(fAnalysisCuts->IsEventRejectedDueToCentrality()){
      fHistNEvents->Fill(8);
    }else{
      if(fAnalysisCuts->IsEventRejectedDueToNotRecoVertex() || fAnalysisCuts->IsEventRejectedDueToVertexContributors()){
  	if(fAnalysisCuts->IsEventRejectedDueToNotRecoVertex())fHistNEvents->Fill(4);
  	if(fAnalysisCuts->IsEventRejectedDueToVertexContributors())fHistNEvents->Fill(5);
      }else{
  	if(fAnalysisCuts->IsEventRejectedDueToZVertexOutsideFiducialRegion())fHistNEvents->Fill(6);
  	else if(fAnalysisCuts->IsEventRejectedDueToPileup())fHistNEvents->Fill(7);
      }
    }
  }

  if(fAnalysisCuts->GetUseCentrality()>0 && fAnalysisCuts->IsEventSelectedInCentrality(aod)!=0) return;
  // events not passing the centrality selection can be removed immediately. For the others we must count the generated D mesons

  Int_t ntracks=aod->GetNumberOfTracks();

  TClonesArray *arrayMC=0;
  AliAODMCHeader *mcHeader=0;
  if(fReadMC){
    arrayMC =  (TClonesArray*)aod->GetList()->FindObject(AliAODMCParticle::StdBranchName());
    if(!arrayMC) {
      printf("AliAnalysisTaskCombinHF4PID::UserExec: MC particles branch not found!\n");
      return;
    }
    
    // load MC header
    mcHeader =  (AliAODMCHeader*)aod->GetList()->FindObject(AliAODMCHeader::StdBranchName());
    if(!mcHeader) {
      printf("AliAnalysisTaskCombinHF4PID::UserExec: MC header branch not found!\n");
      return;
    }
  }

  
  if(!isEvSel)return;
  
  fHistNEvents->Fill(1);
  

  // select and flag tracks
  UChar_t* status = new UChar_t[ntracks];
  for(Int_t iTr=0; iTr<ntracks; iTr++){
    status[iTr]=0;
    AliAODTrack* track=dynamic_cast<AliAODTrack*>(aod->GetTrack(iTr));
    if(!track){
      AliWarning("Error in casting track to AOD track. Not a standard AOD?");
      continue;
    }
    if(IsTrackSelected(track)) status[iTr]+=1;
  }
  
  // build the combinatorics
  Double_t dummypos[3]={0.,0.,0.};
  AliAODVertex* v2=new AliAODVertex(dummypos,999.,-1,2);
  AliAODVertex* v3=new AliAODVertex(dummypos,999.,-1,3);
  // dummy values of track impact parameter, needed to build an AliAODRecoDecay object
  Double_t d02[2]={0.,0.};
  Double_t d03[3]={0.,0.,0.};
  AliAODRecoDecay* tmpRD2 = new AliAODRecoDecay(0x0,2,0,d02);
  AliAODRecoDecay* tmpRD3 = new AliAODRecoDecay(0x0,3,1,d03);
  UInt_t pdg0[2]={321,211};
  UInt_t pdgp[3]={321,211,211};
  UInt_t pdgs[3]={321,211,321};
  UInt_t pdgl[3]={321,211,2212};
  UInt_t pdglswap[3]={321,2212,211};
  Double_t tmpp[3];
  Double_t px[3],py[3],pz[3];
  Int_t dgLabels[3];
 
  for(Int_t iTr1=0; iTr1<ntracks; iTr1++){
    AliAODTrack* trK=dynamic_cast<AliAODTrack*>(aod->GetTrack(iTr1));
    if(!trK){
      AliWarning("Error in casting track to AOD track. Not a standard AOD?");
      continue;
    }
    if((status[iTr1] & 1)==0) continue;
    Int_t chargeK=trK->Charge();
    trK->GetPxPyPz(tmpp);
    px[0] = tmpp[0];
    py[0] = tmpp[1];
    pz[0] = tmpp[2];
    dgLabels[0]=trK->GetLabel();
    Float_t ipK[2], ipCovK[3];
    trK->GetImpactParameters(ipK,ipCovK);
    fTreeVarFloat[3] = trK->P();
    fTreeVarFloat[4] = trK->GetTPCsignal();
    if(fOptForTOF==1) fTreeVarFloat[5] = pidResp->NumberOfSigmasTOF(trK,AliPID::kPion);
    else fTreeVarFloat[5] = GetTimeOfFlight(trK, pidResp);
    fTreeVarFloat[6]=ipK[0];
    fTreeVarFloat[7]=ipCovK[0];
    fTreeVarFloat[8]=trK->GetTPCchi2();
    fTreeVarInt[4]=trK->GetITSClusterMap();
    fTreeVarInt[5]=trK->GetTPCncls();
    for(Int_t iTr2=0; iTr2<ntracks; iTr2++){
      if((status[iTr2] & 1)==0) continue;
      if(iTr1==iTr2) continue;
      AliAODTrack* trPi1=dynamic_cast<AliAODTrack*>(aod->GetTrack(iTr2));
      if(!trPi1){
  	AliWarning("Error in casting track to AOD track. Not a standard AOD?");
  	continue;
      }
      Int_t chargePi1=trPi1->Charge();
      trPi1->GetPxPyPz(tmpp);
      px[1] = tmpp[0];
      py[1] = tmpp[1];
      pz[1] = tmpp[2];
      dgLabels[1]=trPi1->GetLabel();
      Float_t ipPi1[2], ipCovPi1[3];
      trPi1->GetImpactParameters(ipPi1,ipCovPi1);
      fTreeVarFloat[9] = trPi1->P();
      fTreeVarFloat[10] = trPi1->GetTPCsignal();
      if(fOptForTOF==1) fTreeVarFloat[11] = pidResp->NumberOfSigmasTOF(trPi1,AliPID::kPion);
      else fTreeVarFloat[11] = GetTimeOfFlight(trPi1, pidResp);
      fTreeVarFloat[12]=ipPi1[0];
      fTreeVarFloat[13]=ipCovPi1[0];
      fTreeVarFloat[14]=trPi1->GetTPCchi2();
      fTreeVarInt[6]=trPi1->GetITSClusterMap();
      fTreeVarInt[7]=trPi1->GetTPCncls();
      if(fMeson==kDzero){
        if(chargePi1!=chargeK){
  	  // OS candidate
  	  v2->AddDaughter(trK);
  	  v2->AddDaughter(trPi1);
  	  tmpRD2->SetSecondaryVtx(v2);
  	  FillTree(421,2,tmpRD2,px,py,pz,pdg0,arrayMC,dgLabels);
  	  v2->RemoveDaughters();
  	}
      }else{
        for(Int_t iTr3=iTr2+1; iTr3<ntracks; iTr3++){
          if((status[iTr3] & 1)==0) continue;
          if(iTr1==iTr3) continue;
          AliAODTrack* trPi2=dynamic_cast<AliAODTrack*>(aod->GetTrack(iTr3));
  	  if(!trPi2){
  	    AliWarning("Error in casting track to AOD track. Not a standard AOD?");
  	    continue;
  	  }
          Int_t chargePi2=trPi2->Charge();
          trPi2->GetPxPyPz(tmpp);
          px[2] = tmpp[0];
          py[2] = tmpp[1];
          pz[2] = tmpp[2];
          dgLabels[2]=trPi2->GetLabel();
	  Float_t ipPi2[2], ipCovPi2[3];
	  trPi2->GetImpactParameters(ipPi2,ipCovPi2);
  	  fTreeVarFloat[15] = trPi2->P();
  	  fTreeVarFloat[16] = trPi2->GetTPCsignal();
	  if(fOptForTOF==1) fTreeVarFloat[17] = pidResp->NumberOfSigmasTOF(trPi2,AliPID::kPion);
	  else fTreeVarFloat[17] = GetTimeOfFlight(trPi2, pidResp);
	  fTreeVarFloat[18]=ipPi2[0];
	  fTreeVarFloat[19]=ipCovPi2[0];
	  fTreeVarFloat[20]=trPi2->GetTPCchi2();
	  fTreeVarInt[8]=trPi1->GetITSClusterMap();
	  fTreeVarInt[9]=trPi1->GetTPCncls();
  	  if(fMeson==kDs){
  	    tmpRD3->SetPxPyPzProngs(3,px,py,pz);
  	  }
  	  if(chargePi1==chargeK && chargePi2==chargeK) continue;
	  v3->AddDaughter(trK);
	  v3->AddDaughter(trPi1);
	  v3->AddDaughter(trPi2);
	  tmpRD3->SetSecondaryVtx(v3);
	  if(fMeson==kDplus){
	    if(chargePi1==chargePi2) FillTree(411,3,tmpRD3,px,py,pz,pdgp,arrayMC,dgLabels);
	  }
	  else if(fMeson==kDs){
	    if(chargeK!=chargePi2) FillTree(431,3,tmpRD3,px,py,pz,pdgs,arrayMC,dgLabels);
	  }
	  else if(fMeson==kLc){
	    if(chargePi1==chargePi2){
	      FillTree(4122,3,tmpRD3,px,py,pz,pdgl,arrayMC,dgLabels);
	      FillTree(4122,3,tmpRD3,px,py,pz,pdglswap,arrayMC,dgLabels);
	    }
	  }
	  v3->RemoveDaughters();
  	}
      }
    }
  }
  
  delete [] status;
  delete v2;
  delete v3;
  delete tmpRD2;
  delete tmpRD3;
  
  PostData(1,fOutput);
  PostData(2,fCandTree);

  return;
}

//________________________________________________________________________
void AliAnalysisTaskCombinHF4PID::FillTree(Int_t pdgD,Int_t nProngs, AliAODRecoDecay* tmpRD, Double_t* px, Double_t* py, Double_t* pz, UInt_t *pdgdau, TClonesArray *arrayMC, Int_t* dgLabels){
  /// Fill histos for candidates with proper charge sign
  
  tmpRD->SetPxPyPzProngs(nProngs,px,py,pz);
  Double_t pt = tmpRD->Pt();
  Double_t minv2 = tmpRD->InvMass2(nProngs,pdgdau);
  Double_t mass=TMath::Sqrt(minv2);
  
  if(minv2>fMinMass*fMinMass && minv2<fMaxMass*fMaxMass){
    Double_t rapid = tmpRD->Y(pdgD);    

    if(fAnalysisCuts->IsInFiducialAcceptance(pt,rapid)){
      fTreeVarFloat[0]=pt;
      fTreeVarFloat[1]=rapid;
      fTreeVarFloat[2]=mass;
      if(fReadMC){
        Int_t signPdg[3]={0,0,0};
        for(Int_t iii=0; iii<nProngs; iii++) signPdg[iii]=pdgdau[iii];
	Int_t signPdgD=0;
        Int_t labD = tmpRD->MatchToMC(pdgD,arrayMC,nProngs,signPdg);
	if(labD>=0){
	  AliAODMCParticle* dmes =  dynamic_cast<AliAODMCParticle*>(arrayMC->At(labD));
	  if(dmes) signPdgD=dmes->GetPdgCode();
	}
	Bool_t isRefl=kFALSE;
  	for(Int_t idau=0; idau<nProngs; idau++){
  	  AliAODMCParticle* part = dynamic_cast<AliAODMCParticle*>(arrayMC->At(TMath::Abs(dgLabels[idau])));
  	  if(part){
	    Int_t pdgDauTrue=part->GetPdgCode();
	    fTreeVarInt[idau+1]=pdgDauTrue;
	    if(TMath::Abs(pdgDauTrue)!=signPdg[idau]) isRefl=kTRUE;
	  }
  	  else fTreeVarInt[idau+1]=0;
  	}
	if(isRefl) signPdgD*=1000;
  	fTreeVarInt[0]=signPdgD;	
      }
      fCandTree->Fill();
    }
  }
  return;
}
//________________________________________________________________________
Bool_t AliAnalysisTaskCombinHF4PID::IsTrackSelected(AliAODTrack* track){
  /// track selection cuts
  
  if(track->Charge()==0) return kFALSE;
  if(track->GetID()<0&&!fKeepNegID)return kFALSE;
  if(fFilterMask>0){
    if(!(track->TestFilterMask(fFilterMask))) return kFALSE;
  }
  if(fTrCuts && !fTrCuts->IsSelected(track)) return kFALSE;
  return kTRUE;
}
//________________________________________________________________________
Double_t AliAnalysisTaskCombinHF4PID::GetTimeOfFlight(AliAODTrack* track, AliPIDResponse *pidResp){
  AliAODPid *pid = track->GetDetPid();
  if(!pid) return -999;
  Double_t prob[AliPID::kSPECIES];
  if(pidResp->ComputeTOFProbability(track,AliPID::kSPECIES,prob)==AliPIDResponse::kDetPidOk){
    Double_t tofTime=pid->GetTOFsignal();
    AliTOFPIDResponse tofResp=pidResp->GetTOFResponse();
    Double_t startTime = tofResp.GetStartTime(track->P());
    tofTime-=startTime;
    return tofTime;
  }else{
    return -999;
  }
}

//_________________________________________________________________
void AliAnalysisTaskCombinHF4PID::Terminate(Option_t */*option*/)
{
  /// Terminate analysis
  //
  if(fDebug > 1) printf("AliAnalysisTaskCombinHF4PID: Terminate() \n");
  fOutput = dynamic_cast<TList*> (GetOutputData(1));
  if (!fOutput) {
    printf("ERROR: fOutput not available\n");
    return;
  }
  fHistNEvents = dynamic_cast<TH1F*>(fOutput->FindObject("hNEvents"));
  if(fHistNEvents){
    printf("Number of analyzed events = %d\n",(Int_t)fHistNEvents->GetBinContent(2));
  }else{
    printf("ERROR: fHistNEvents not available\n");
    return;
  }
  return;
}
