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
// Class AliAnalysisTaskSingleTrack4PID
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
#include "AliAnalysisTaskSingleTrack4PID.h"
#include "AliAnalysisUtils.h"

/// \cond CLASSIMP
ClassImp(AliAnalysisTaskSingleTrack4PID);
/// \endcond

//________________________________________________________________________
AliAnalysisTaskSingleTrack4PID::AliAnalysisTaskSingleTrack4PID():
  AliAnalysisTaskSE("PID4tracks"),
  fOutput(0x0),
  fHistNEvents(0x0),
  fTrackTree(0x0),
  fTreeVarFloat(0x0),
  fTreeVarInt(0x0),
  fFilterMask(16),
  fTrCuts(0x0),
  fOptForTOF(1),
  fReadMC(kFALSE),
  fUsePhysSel(kFALSE),
  fUsePileupCut(kFALSE),
  fTriggerMask(AliVEvent::kAnyINT),
  fGoUpToQuark(kTRUE),
  fKeepNegID(kFALSE)
{
  /// default constructor

  DefineInput(0, TChain::Class());
  DefineOutput(1,TList::Class());
  DefineOutput(2, TTree::Class());
}

//________________________________________________________________________
AliAnalysisTaskSingleTrack4PID::~AliAnalysisTaskSingleTrack4PID()
{
  //
  /// Destructor
  //
  if(fOutput && !fOutput->IsOwner()){
    delete fHistNEvents;
    delete fTrackTree;
  }

  delete fOutput;
  delete fTrCuts;
  delete [] fTreeVarFloat;
  delete [] fTreeVarInt;

}

//________________________________________________________________________
void AliAnalysisTaskSingleTrack4PID::UserCreateOutputObjects()
{
  /// Create the output container
  //
  if(fDebug > 1) printf("AnalysisTaskSingleTrack4PID::UserCreateOutputObjects() \n");
  
  fOutput = new TList();
  fOutput->SetOwner();
  fOutput->SetName("OutputHistos");
  
  fHistNEvents = new TH1F("hNEvents", "Number of processed events",15,-0.5,14.5);
  fHistNEvents->GetXaxis()->SetBinLabel(1,"All events");
  fHistNEvents->GetXaxis()->SetBinLabel(2,"PhysSel");
  fHistNEvents->GetXaxis()->SetBinLabel(3,"Good vertex");
  fHistNEvents->GetXaxis()->SetBinLabel(4,"Pass zSPD-zTrk vert sel");
  fHistNEvents->GetXaxis()->SetBinLabel(5,"|zvert|<10");
  fHistNEvents->GetXaxis()->SetBinLabel(6,"Pileup cut");
  fHistNEvents->SetMinimum(0);
  fOutput->Add(fHistNEvents);
  
  fTrackTree = new TTree("trackTree", "Tree for analysis");
  TString floatVarName[kNumOfFloatVar];
  fTreeVarFloat = new Float_t[kNumOfFloatVar];
  floatVarName[0]="p";          // momentum
  floatVarName[1]="eta";        // pseudorap
  floatVarName[2]="pt";         // pt
  floatVarName[3]="sig1pt";     // sigma 1/pt (relative)
  floatVarName[4]="dca";        // imp par
  floatVarName[5]="sigdca";     // imp par unc
  floatVarName[6]="chisq";      // First daughter chi2 in TPC
  floatVarName[7]="dedx";       // dE/dx
  floatVarName[8]="nsigpiTPC";  // n sigma pion (TPC)
  floatVarName[9]="nsigkTPC";   // n sigma kaon (TPC)
  floatVarName[10]="nsigprTPC"; // n sigma proton (TPC)
  floatVarName[11]="tof";       // TOF
  floatVarName[12]="nsigpiTOF"; // n sigma pion (TOF)
  floatVarName[13]="nsigkTOF";  // n sigma kaon (TOF)
  floatVarName[14]="nsigprTOF"; // n sigma proton (TOF)

  for(Int_t ivar=0; ivar<kNumOfFloatVar; ivar++){
    fTrackTree->Branch(floatVarName[ivar].Data(),&fTreeVarFloat[ivar],Form("%s/F",floatVarName[ivar].Data()));
  }
  TString intVarName[kNumOfIntVar];
  fTreeVarInt = new Int_t[kNumOfIntVar];
  intVarName[0]="pdg"; // PDG code
  intVarName[1]="itscl"; // ITS clusters
  intVarName[2]="tpccl"; // TPC clusters
  for(Int_t ivar=0; ivar<kNumOfIntVar; ivar++){
    fTrackTree->Branch(intVarName[ivar].Data(),&fTreeVarInt[ivar],Form("%s/I",intVarName[ivar].Data()));
  }

  PostData(1,fOutput);
  PostData(2,fTrackTree);
}

//________________________________________________________________________
void AliAnalysisTaskSingleTrack4PID::UserExec(Option_t */*option*/){
  /// Build the 3-track combinatorics (+-+ and -+-) for D+->Kpipi decays
  
  AliAODEvent *aod = dynamic_cast<AliAODEvent*> (InputEvent());
  if(!aod && AODEvent() && IsStandardAOD()) {
    // In case there is an AOD handler writing a standard AOD, use the AOD
    // event in memory rather than the input (ESD) event.
    aod = dynamic_cast<AliAODEvent*> (AODEvent());
  }
  if(!aod){
    printf("AliAnalysisTaskSingleTrack4PID::UserExec: AOD not found!\n");
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
  
  TClonesArray *arrayMC=0;
  AliAODMCHeader *mcHeader=0;
  if(fReadMC){
    arrayMC =  (TClonesArray*)aod->GetList()->FindObject(AliAODMCParticle::StdBranchName());
    if(!arrayMC) {
      printf("AliAnalysisTaskSingleTrack4PID::UserExec: MC particles branch not found!\n");
      return;
    }
    
    // load MC header
    mcHeader =  (AliAODMCHeader*)aod->GetList()->FindObject(AliAODMCHeader::StdBranchName());
    if(!mcHeader) {
      printf("AliAnalysisTaskSingleTrack4PID::UserExec: MC header branch not found!\n");
      return;
    }
  }

  fHistNEvents->Fill(0); // count event
  if(fUsePhysSel){
    Bool_t isPhysSel = (((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected() & fTriggerMask);
    if(!isPhysSel) return;
  }
  fHistNEvents->Fill(1);

  const AliVVertex* vtTrc = aod->GetPrimaryVertex();
  const AliVVertex* vtSPD = aod->GetPrimaryVertexSPD();
  TString titTrc=vtTrc->GetTitle();
  if(titTrc.IsNull() || titTrc=="vertexer: 3D" || titTrc=="vertexer: Z") return;
  if (vtSPD->GetNContributors()<1) return;
  fHistNEvents->Fill(2);

  double covTrc[6],covSPD[6];
  vtTrc->GetCovarianceMatrix(covTrc);
  vtSPD->GetCovarianceMatrix(covSPD);
  double dz = vtTrc->GetZ()-vtSPD->GetZ();
  double errTot = TMath::Sqrt(covTrc[5]+covSPD[5]);
  double errTrc = TMath::Sqrt(covTrc[5]);
  double nsigTot = TMath::Abs(dz)/errTot, nsigTrc = TMath::Abs(dz)/errTrc;
  if (TMath::Abs(dz)>0.2 || nsigTot>10 || nsigTrc>20) return; // bad vertexing
  fHistNEvents->Fill(3);

  Float_t xvert=vtTrc->GetX();
  Float_t yvert=vtTrc->GetY();
  Float_t zvert=vtTrc->GetZ();
  if(TMath::Abs(zvert)>10) return;
  fHistNEvents->Fill(4);

  if(fUsePileupCut){
    AliAnalysisUtils utils;
    utils.SetMinPlpContribMV(5);
    utils.SetMaxPlpChi2MV(5.);
    utils.SetMinWDistMV(15.);
    utils.SetCheckPlpFromDifferentBCMV(kTRUE);
    Bool_t isPUMV = utils.IsPileUpMV(aod);
    if(isPUMV) return;
    fHistNEvents->Fill(5);
  }

  // Post the data already here
  PostData(1,fOutput);
  PostData(2,fTrackTree);

  Int_t ntracks=aod->GetNumberOfTracks();

  for (Int_t iTrack=0; iTrack < ntracks; iTrack++) {
    AliAODTrack * track = (AliAODTrack*)aod->GetTrack(iTrack);
    if (!track) continue;
    if(!IsTrackSelected(track)) continue;
    AliExternalTrackParam etp;
    etp.CopyFromVTrack(track);
    Double_t curvrelerr = TMath::Sqrt(etp.GetSigma1Pt2())/etp.OneOverPt();
    Float_t ip[2], ipCov[3];
    track->GetImpactParameters(ip,ipCov);
    Int_t trlabel=track->GetLabel();
    Int_t pdgCode=0;
    if(fReadMC){
      AliAODMCParticle* part = dynamic_cast<AliAODMCParticle*>(arrayMC->At(TMath::Abs(trlabel)));
      if(part) pdgCode = part->GetPdgCode();
    }

    fTreeVarFloat[0] = track->P();
    fTreeVarFloat[1] = track->Eta();
    fTreeVarFloat[2] = track->Pt();
    fTreeVarFloat[3] = curvrelerr;
    fTreeVarFloat[4] = ip[0];
    fTreeVarFloat[5] = ipCov[0];
    fTreeVarFloat[6] = track->GetTPCchi2();
    fTreeVarFloat[7] = track->GetTPCsignal();
    fTreeVarFloat[8] = pidResp->NumberOfSigmasTPC(track,AliPID::kPion);
    fTreeVarFloat[9] = pidResp->NumberOfSigmasTPC(track,AliPID::kKaon);
    fTreeVarFloat[10] = pidResp->NumberOfSigmasTPC(track,AliPID::kProton);
    fTreeVarFloat[11] = GetTimeOfFlight(track, pidResp);
    fTreeVarFloat[12] = pidResp->NumberOfSigmasTOF(track,AliPID::kPion);
    fTreeVarFloat[13] = pidResp->NumberOfSigmasTOF(track,AliPID::kKaon);
    fTreeVarFloat[14] = pidResp->NumberOfSigmasTOF(track,AliPID::kProton);

    fTreeVarInt[0] = pdgCode;
    fTreeVarInt[1] = track->GetITSClusterMap();
    fTreeVarInt[2] = track->GetTPCncls();
    fTrackTree->Fill();
  }
  
  PostData(1,fOutput);
  PostData(2,fTrackTree);

  return;
}

//________________________________________________________________________
Bool_t AliAnalysisTaskSingleTrack4PID::IsTrackSelected(AliAODTrack* track){
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
Double_t AliAnalysisTaskSingleTrack4PID::GetTimeOfFlight(AliAODTrack* track, AliPIDResponse *pidResp){
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
void AliAnalysisTaskSingleTrack4PID::Terminate(Option_t */*option*/)
{
  /// Terminate analysis
  //
  if(fDebug > 1) printf("AliAnalysisTaskSingleTrack4PID: Terminate() \n");
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
