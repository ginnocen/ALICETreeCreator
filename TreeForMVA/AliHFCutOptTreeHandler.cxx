/* Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//*************************************************************************
// Class AliHFCutOptTreeHandler
// helper class to handle a tree for cut optimisation and MVA analyses
// Authors:
// F. Catalano, fabio.catalano@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
// G. Innocenti, gian.michele.innocenti@cern.ch
// F. Prino, prino@to.infn.it
/////////////////////////////////////////////////////////////

#include "AliHFCutOptTreeHandler.h"

#include <TDatabasePDG.h>
#include <TMath.h>
#include "AliAODTrack.h"
#include "AliAODRecoDecayHF.h"
#include "AliAODRecoDecayHF2Prong.h"
#include "AliAODRecoDecayHF3Prong.h"
#include "AliAODRecoCascadeHF.h"

/// \cond CLASSIMP
ClassImp(AliHFCutOptTreeHandler);
/// \endcond

AliHFCutOptTreeHandler::AliHFCutOptTreeHandler():
  TObject(),
  fTreeTopolVar(0x0),
  fDecayChannel(kD0toKpi),
  fCandType(kBkg),
  fUseCentrality(false),
  fCentrality(0),
  fPidOpt(kNsigmaPID)
{
  // Default constructor
  for(int iVar=0; iVar<15; iVar++) {
    fTopolVarVector[iVar] = -999.;
    if(iVar<6) {
      fPIDnSigmaVector[iVar]=-1;
      fPIDnSigmaCharVector[iVar]=-1;
    }
  } 
}

AliHFCutOptTreeHandler::AliHFCutOptTreeHandler(int decay, int PIDopt):
  TObject(),
  fTreeTopolVar(0x0),
  fDecayChannel(decay),
  fCandType(kBkg),
  fUseCentrality(false),
  fCentrality(0),
  fPidOpt(PIDopt)
{
  // Standard constructor
  for(int iVar=0; iVar<15; iVar++) {
    fTopolVarVector[iVar] = -999.;
    if(iVar<6) {
      fPIDnSigmaVector[iVar]=-1;
      fPIDnSigmaCharVector[iVar]=-1;
    }
  }
}
  
AliHFCutOptTreeHandler::~AliHFCutOptTreeHandler()
{
  // Destructor
  if(fTreeTopolVar) delete fTreeTopolVar;
}

bool AliHFCutOptTreeHandler::SetVariables(AliAODRecoDecayHF* d, AliAODPidHF* pidHF, int masshypo, bool issig, bool isprompt, bool isrefl) 
{
  //candidate type
  if(!issig) fCandType=kBkg;
  else {
    if(!isrefl && isprompt) fCandType=kPromptSig;
    if(!isrefl && !isprompt) fCandType=kFDSig;
    if(isrefl && isprompt) fCandType=kPromptRefl;
    if(isrefl && !isprompt) fCandType=kFDRefl;
  }

  std::cout << fCandType-1 << std::endl;
  //topological variables
  AliAODRecoDecayHF* dcorr = 0x0;

  fTopolVarVector[1]=d->Pt();
  fTopolVarVector[2]=d->DecayLength();
  fTopolVarVector[3]=d->DecayLengthXY();
  fTopolVarVector[4]=d->NormalizedDecayLengthXY();
  fTopolVarVector[5]=d->CosPointingAngle();
  fTopolVarVector[6]=d->CosPointingAngleXY();
  fTopolVarVector[7]=d->ImpParXY();
  fTopolVarVector[8]=d->PtProng(0);
  fTopolVarVector[9]=d->PtProng(1);
  if(fDecayChannel!=kD0toKpi) { // common to 3prongs
    fTopolVarVector[10]=((AliAODRecoDecayHF3Prong*)d)->PtProng(2);
    fTopolVarVector[11]=((AliAODRecoDecayHF3Prong*)d)->GetSigmaVert();
  }

  if(fDecayChannel==kDplustoKpipi) fTopolVarVector[0]=((AliAODRecoDecayHF3Prong*)d)->InvMassDplus();
  if(fDecayChannel==kDstoKKpi) {
    float massPhi = TDatabasePDG::Instance()->GetParticle(333)->Mass();
    if(masshypo==0){ //phiKKpi
      fTopolVarVector[0]=((AliAODRecoDecayHF3Prong*)d)->InvMassDsKKpi();
      fTopolVarVector[12]=TMath::Abs(((AliAODRecoDecayHF3Prong*)d)->InvMass2Prongs(0,1,321,321)-massPhi);
      fTopolVarVector[13]=((AliAODRecoDecayHF3Prong*)d)->CosPiDsLabFrameKKpi();
      fTopolVarVector[14]=((AliAODRecoDecayHF3Prong*)d)->CosPiKPhiRFrameKKpi();
    }
    if(masshypo==1){ //phipiKK
      fTopolVarVector[0]=((AliAODRecoDecayHF3Prong*)d)->InvMassDspiKK();
      fTopolVarVector[12]=TMath::Abs(((AliAODRecoDecayHF3Prong*)d)->InvMass2Prongs(1,2,321,321)-massPhi);
      fTopolVarVector[13]=((AliAODRecoDecayHF3Prong*)d)->CosPiDsLabFramepiKK();
      fTopolVarVector[14]=((AliAODRecoDecayHF3Prong*)d)->CosPiKPhiRFramepiKK();
    }
    fTopolVarVector[14] = TMath::Abs(fTopolVarVector[14]*fTopolVarVector[14]*fTopolVarVector[14]);
  }

  if(fPidOpt==kNoPID) return true;

  //PID variables
  double sigTPC_K_0=-999.;
  double sigTPC_K_1=-999.;
  double sigTPC_Pi_0=-999.;
  double sigTPC_Pi_1=-999.;
  double sigTOF_K_0=-999.;
  double sigTOF_K_1=-999.;
  double sigTOF_Pi_0=-999.;
  double sigTOF_Pi_1=-999.;
  double sigTPC_K_2=-999.;
  double sigTPC_Pi_2=-999.;
  double sigTOF_K_2=-999.;
  double sigTOF_Pi_2=-999.;

  AliAODTrack *track0=(AliAODTrack*)d->GetDaughter(0);
  AliAODTrack *track1=(AliAODTrack*)d->GetDaughter(1);
  AliAODTrack *track2=0x0;

  pidHF->GetnSigmaTPC(track0,3,sigTPC_K_0);
  pidHF->GetnSigmaTPC(track1,3,sigTPC_K_1);
  pidHF->GetnSigmaTPC(track0,2,sigTPC_Pi_0);
  pidHF->GetnSigmaTPC(track1,2,sigTPC_Pi_1);

  pidHF->GetnSigmaTOF(track0,3,sigTOF_K_0);
  pidHF->GetnSigmaTOF(track1,3,sigTOF_K_1);
  pidHF->GetnSigmaTOF(track0,2,sigTOF_Pi_0);
  pidHF->GetnSigmaTOF(track1,2,sigTOF_Pi_1);

  if(fDecayChannel!=kD0toKpi) {
    track2 = (AliAODTrack*)d->GetDaughter(2);
    pidHF->GetnSigmaTPC(track2,3,sigTPC_K_2);
    pidHF->GetnSigmaTPC(track2,2,sigTPC_Pi_2);
    pidHF->GetnSigmaTOF(track2,3,sigTOF_K_2);
    pidHF->GetnSigmaTOF(track2,2,sigTOF_Pi_2);
  }

  //PID TPC and TOF squared sum, normalized to sqrt(2)
  float sigComb_K_0 = -1.;
  float sigComb_K_1 = -1.;
  float sigComb_K_2 = -1.;
  float sigComb_Pi_0 = -1.;
  float sigComb_Pi_1 = -1.;
  float sigComb_Pi_2 = -1.;

  if(sigTPC_K_0 > -998.){
    if(sigTOF_K_0 > -998.)
      sigComb_K_0 = TMath::Sqrt((sigTPC_K_0*sigTPC_K_0 + sigTOF_K_0*sigTOF_K_0) / 2.);
    else
      sigComb_K_0 = TMath::Abs(sigTPC_K_0);
  }

  if(sigTPC_K_1 > -998.){
    if(sigTOF_K_1 > -998.)
      sigComb_K_1 = TMath::Sqrt((sigTPC_K_1*sigTPC_K_1 + sigTOF_K_1*sigTOF_K_1) / 2.);
    else
      sigComb_K_1 = TMath::Abs(sigTPC_K_1);
  }

  if(sigTPC_Pi_0 > -998.){
    if(sigTOF_Pi_0 > -998.)
      sigComb_Pi_0 = TMath::Sqrt((sigTPC_Pi_0*sigTPC_Pi_0 + sigTOF_Pi_0*sigTOF_Pi_0) / 2.);
    else
      sigComb_Pi_0 = TMath::Abs(sigTPC_Pi_0);
  }

  if(sigTPC_Pi_1 > -998.){
    if(sigTOF_Pi_1 > -998.)
      sigComb_Pi_1 = TMath::Sqrt((sigTPC_Pi_1*sigTPC_Pi_1 + sigTOF_Pi_1*sigTOF_Pi_1) / 2.);
    else
      sigComb_Pi_1 = TMath::Abs(sigTPC_Pi_1);
  }

  if(fDecayChannel!=kD0toKpi) {

    if(sigTPC_Pi_2 > -998.){
      if(sigTOF_Pi_2 > -998.)
        sigComb_Pi_2 = TMath::Sqrt((sigTPC_Pi_2*sigTPC_Pi_2 + sigTOF_Pi_2*sigTOF_Pi_2) / 2.);
      else
        sigComb_Pi_2 = TMath::Abs(sigTPC_Pi_2);
    }
    if(sigTPC_K_2 > -998.){
      if(sigTOF_K_2 > -998.)
        sigComb_K_2 = TMath::Sqrt((sigTPC_K_2*sigTPC_K_2 + sigTOF_K_2*sigTOF_K_2) / 2.);
      else
        sigComb_K_2 = TMath::Abs(sigTPC_K_2);
    }
  }
  
  fPIDnSigmaVector[0]=sigComb_Pi_0*10;
  fPIDnSigmaVector[1]=sigComb_K_0*10;
  fPIDnSigmaVector[2]=sigComb_Pi_1*10;
  fPIDnSigmaVector[3]=sigComb_K_1*10;

  sigComb_Pi_0<12.7 ? fPIDnSigmaCharVector[0]=sigComb_Pi_0*10 : -1;
  sigComb_K_0<12.7 ? fPIDnSigmaCharVector[1]=sigComb_K_0*10 : -1;
  sigComb_Pi_1<12.7 ? fPIDnSigmaCharVector[2]=sigComb_Pi_1*10 : -1;
  sigComb_K_1<12.7 ? fPIDnSigmaCharVector[3]=sigComb_K_1*10 : -1;

  if(fDecayChannel!=kD0toKpi) {

    fPIDnSigmaVector[4]=sigComb_Pi_2*10;
    fPIDnSigmaVector[5]=sigComb_K_2*10;

    sigComb_Pi_2<12.7 ? fPIDnSigmaCharVector[4]=sigComb_Pi_2*10 : -1;
    sigComb_K_2<12.7 ? fPIDnSigmaCharVector[5]=sigComb_K_2*10 : -1;
  }

  return true;
}

TTree* AliHFCutOptTreeHandler::BuildTree(TString name, TString title) 
{
  if(fTreeTopolVar) delete fTreeTopolVar;
  fTreeTopolVar=0x0;
  fTreeTopolVar = new TTree(name.Data(),title.Data());

  TString topolvarnames[15] = {"inv_mass","pt_cand","d_len","d_len_xy","norm_dl_xy","cos_p","cos_p_xy","imp_par_xy","pt_prong0","pt_prong1","pt_prong2","sig_vert","delta_mass_KK","cos_PiDs","cos_PiKPhi_3"};
  TString PIDvarnames[6] = {"sigComb_Pi_0","sigComb_K_0","sigComb_Pi_1","sigComb_K_1","sigComb_Pi_2","sigComb_K_2"};

  for(int iVar=0; iVar<15; iVar++) {
    if((iVar==10 || iVar==11) && fDecayChannel==kD0toKpi) continue; 
    if(iVar>11 && fDecayChannel!=kDstoKKpi) continue; 
    fTreeTopolVar->Branch(topolvarnames[iVar].Data(),&fTopolVarVector[iVar],Form("%s/F",topolvarnames[iVar].Data()));
  }

  switch(fPidOpt) {
    case 0: 
      return fTreeTopolVar;
    break;
    case 1:
      for(int iVar=0; iVar<6; iVar++) {
        if(iVar>3 && fDecayChannel==kD0toKpi) continue; 
        fTreeTopolVar->Branch(PIDvarnames[iVar].Data(),&fPIDnSigmaVector[iVar],Form("%s/F",PIDvarnames[iVar].Data()));
      }
    break;
    case 2:
    for(int iVar=0; iVar<6; iVar++) {
      if(iVar>3 && fDecayChannel==kD0toKpi) continue; 
      fTreeTopolVar->Branch(PIDvarnames[iVar].Data(),&fPIDnSigmaCharVector[iVar],Form("%s/B",PIDvarnames[iVar].Data()));
    }
    break;
    case 3:
    for(int iVar=0; iVar<6; iVar++) {
      if(iVar>3 && fDecayChannel==kD0toKpi) continue; 
      fTreeTopolVar->Branch(PIDvarnames[iVar].Data(),&fPIDnSigmaVector[iVar],Form("%s/F",PIDvarnames[iVar].Data()));
      fTreeTopolVar->Branch(Form("%s_char",PIDvarnames[iVar].Data()),&fPIDnSigmaCharVector[iVar],Form("%s_char/B",PIDvarnames[iVar].Data()));
    }
    break;
  }

  fTreeTopolVar->Branch("cand_type",&fCandType,"cand_type/B");
  if(fUseCentrality) fTreeTopolVar->Branch("centrality",&fCentrality,"centrality/B");

  return fTreeTopolVar;
}
