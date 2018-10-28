/* Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//*************************************************************************
// Class AliHFCutOptTreeHandler
// helper class to handle a tree for cut optimisation and MVA analyses
// Authors:
// F. Catalano, fabio.catalano@cern.ch
// A. Festanti, andrea.festanti@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
// G. Innocenti, gian.michele.innocenti@cern.ch
// F. Prino, prino@to.infn.it
// L. Vermunt, luuk.vermunt@cern.ch
// L.V.R. van Doremalen, lennart.van.doremalen@cern.ch
/////////////////////////////////////////////////////////////

#include "AliHFCutOptTreeHandler.h"

#include <TDatabasePDG.h>
#include <TMath.h>
#include "AliAODTrack.h"
#include "AliAODRecoDecayHF.h"
#include "AliAODRecoDecayHF2Prong.h"
#include "AliAODRecoDecayHF3Prong.h"
#include "AliAODRecoCascadeHF.h"
#include "AliAODMCParticle.h"
#include "AliVertexingHFUtils.h"

/// \cond CLASSIMP
ClassImp(AliHFCutOptTreeHandler);
/// \endcond

//________________________________________________________________
AliHFCutOptTreeHandler::AliHFCutOptTreeHandler():
  TObject(),
  fTreeTopolVar(0x0),
  fDecayChannel(kD0toKpi),
  fPidOpt(kNsigmaPID),
  fCandType(kBkg),
  fUseCentrality(false),
  fCentrality(0),
  fFillOnlySignal(false),
  fIsMC(false),
  fIsSignal(-999),
  fIsPrompt(-999),
  fIsRefl(-999),
  fIsSelStd(false),
  fUseSelFlag(true)
{
  // Default constructor
  SetPdgCodes();

  for(int iVar=0; iVar<knTopolVars; iVar++) fTopolVarVector[iVar] = -999.;
  for(int iVar=0; iVar<knPidVars; iVar++) {
    fPIDnSigmaVector[iVar]=-1;
    fPIDnSigmaCharVector[iVar]=-1;
  }
}

//________________________________________________________________
AliHFCutOptTreeHandler::AliHFCutOptTreeHandler(int decay, int PIDopt, bool isMC):
  TObject(),
  fTreeTopolVar(0x0),
  fDecayChannel(decay),
  fPidOpt(PIDopt),
  fCandType(kBkg),
  fUseCentrality(false),
  fCentrality(0),
  fFillOnlySignal(false),
  fIsMC(isMC),
  fIsSignal(-999),
  fIsPrompt(-999),
  fIsRefl(-999),
  fIsSelStd(false),
  fUseSelFlag(true)
{
  // Standard constructor
  SetPdgCodes();
  for(int iVar=0; iVar<knTopolVars; iVar++) fTopolVarVector[iVar] = -999.;
  for(int iVar=0; iVar<knPidVars; iVar++) {
    fPIDnSigmaVector[iVar]=-1;
    fPIDnSigmaCharVector[iVar]=-1;
  }
}
  
//________________________________________________________________
AliHFCutOptTreeHandler::~AliHFCutOptTreeHandler()
{
  // Destructor
  if(fTreeTopolVar) delete fTreeTopolVar;
}

//________________________________________________________________
bool AliHFCutOptTreeHandler::SetVariables(AliAODRecoDecayHF* d, int masshypo, AliAODPidHF* pidHF, TClonesArray* arrayMC) 
{

  if(!d) return false;

  //candidate type
  if(fIsMC) {
    if(fIsSignal==-999 || fIsPrompt==-999 || fIsRefl==-999) { // if not setted, recompute 
      if(!arrayMC) fIsSignal=0;
      else {
        int labD=-1;
        int orig=-1;
        int labDau0=-1;
        int pdgdau0=-1;
        bool isrefl=false;
        if(fDecayChannel==kDplustoKpipi || fDecayChannel==kDstoKKpi) labD = d->MatchToMC(fPdgCode,arrayMC,3,fPdgCodeProngs);
        else if (fDecayChannel==kD0toKpi) labD = d->MatchToMC(fPdgCode,arrayMC,2,fPdgCodeProngs);
        else if (fDecayChannel==kBplustoD0pi) labD = MatchBPlusCandidateToMonteCarlo(521,d,arrayMC);
        if(labD<0) fIsSignal=0;
        else {
          fIsSignal=1;

          AliAODMCParticle *partD = (AliAODMCParticle*)arrayMC->At(labD);
          if(partD || fDecayChannel!=kBplustoD0pi) orig = AliVertexingHFUtils::CheckOrigin(arrayMC,partD,kTRUE); //4 --> prompt, 5 --> FD

          if(fDecayChannel==kBplustoD0pi) fIsPrompt=1; //the use of promt/feeddown is not needed for B decays
          else if(orig==4) fIsPrompt=1;
          else if(orig==5) fIsPrompt=0;
          else fIsSignal=0;

          if(fDecayChannel==kDplustoKpipi || fDecayChannel==kBplustoD0pi) fIsRefl=0; // no reflected signal for D+ -> Kpipi and Bplus -> D0pi
          else { 
            labDau0=((AliAODTrack*)d->GetDaughter(0))->GetLabel();
            AliAODMCParticle* dau0=(AliAODMCParticle*)arrayMC->UncheckedAt(TMath::Abs(labDau0));
            if(dau0) pdgdau0=TMath::Abs(dau0->GetPdgCode());
            if((masshypo==0 && pdgdau0!=321) || (masshypo==1 && pdgdau0!=211)) fIsRefl=1;
            else fIsRefl=0;
          }
        }
      }
    }
    if(fIsSignal==0) fCandType=kBkg;
    else {
      if(fIsPrompt==1) {
        if(fIsRefl==0) fCandType=kPromptSig;
        else fCandType=kPromptRefl;
      }
      else {
        if(fIsRefl==0) fCandType=kFDSig;
        else fCandType=kFDRefl;
      }
    }
  }
  else { //if data not possible to tag
    fCandType=kBkg;
  }

  if(fCandType==kBkg && fFillOnlySignal) return true;

  //reset candidate flags
  fIsSignal=-999;
  fIsPrompt=-999;
  fIsRefl=-999;

  //topological variables
  fTopolVarVector[1]=d->Pt();
  fTopolVarVector[2]=d->DecayLength();
  fTopolVarVector[3]=d->DecayLengthXY();
  fTopolVarVector[4]=d->NormalizedDecayLengthXY();
  fTopolVarVector[5]=d->CosPointingAngle();
  fTopolVarVector[6]=d->CosPointingAngleXY();
  fTopolVarVector[7]=d->ImpParXY();
  fTopolVarVector[8]=d->PtProng(0);
  fTopolVarVector[9]=d->PtProng(1);

  switch(fDecayChannel) {
    case 0: //D0 -> Kpi
    fTopolVarVector[11]=((AliAODRecoDecayHF2Prong*)d)->Getd0Prong(0);
    fTopolVarVector[12]=((AliAODRecoDecayHF2Prong*)d)->Getd0Prong(1);
    fTopolVarVector[13]=fTopolVarVector[11]*fTopolVarVector[12];
      if(masshypo==0) {
        fTopolVarVector[0]=((AliAODRecoDecayHF2Prong*)d)->InvMassD0();
        fTopolVarVector[10]=((AliAODRecoDecayHF2Prong*)d)->CosThetaStarD0();
      }
      else {
        fTopolVarVector[0]=((AliAODRecoDecayHF2Prong*)d)->InvMassD0bar();
        fTopolVarVector[10]=((AliAODRecoDecayHF2Prong*)d)->CosThetaStarD0bar();
      }
    break;
    case 1: //D+ -> Kpipi
      fTopolVarVector[0]=((AliAODRecoDecayHF3Prong*)d)->InvMassDplus();
      fTopolVarVector[10]=((AliAODRecoDecayHF3Prong*)d)->PtProng(2);
      fTopolVarVector[11]=((AliAODRecoDecayHF3Prong*)d)->GetSigmaVert();  
    break;
    case 2: //Ds+ -> KKpi
      fTopolVarVector[10]=((AliAODRecoDecayHF3Prong*)d)->PtProng(2);
      fTopolVarVector[11]=((AliAODRecoDecayHF3Prong*)d)->GetSigmaVert();
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
    break;
    case 3: //Bplus -> D0pi
    UInt_t prongs[2];
    prongs[0] = 211; prongs[1] = 421;
    fTopolVarVector[0]=d->InvMass(2,prongs);
    fTopolVarVector[10]=d->CosThetaStar(0,521,211,421);
    fTopolVarVector[11]=((AliAODRecoDecayHF2Prong*)d)->Getd0Prong(0);
    fTopolVarVector[12]=((AliAODRecoDecayHF2Prong*)d)->Getd0Prong(1);
    fTopolVarVector[13]=fTopolVarVector[11]*fTopolVarVector[12];

    AliAODRecoDecayHF2Prong* candidateD0 = (AliAODRecoDecayHF2Prong*)d->GetDaughter(1);  
      if(d->charge()==-1) {
        fTopolVarVector[14]=candidateD0->InvMassD0();
        fTopolVarVector[15]=d->Pt();
        fTopolVarVector[16]=d->DecayLength();
        fTopolVarVector[17]=d->DecayLengthXY();
        fTopolVarVector[18]=d->NormalizedDecayLengthXY();
        fTopolVarVector[19]=d->CosPointingAngle();
        fTopolVarVector[20]=d->CosPointingAngleXY();
        fTopolVarVector[21]=d->ImpParXY();
        fTopolVarVector[22]=d->PtProng(0);
        fTopolVarVector[23]=d->PtProng(1);
        fTopolVarVector[24]=candidateD0->CosThetaStarD0();
        fTopolVarVector[25]=((AliAODRecoDecayHF2Prong*)d)->Getd0Prong(0);
        fTopolVarVector[26]=((AliAODRecoDecayHF2Prong*)d)->Getd0Prong(1);
        fTopolVarVector[27]=fTopolVarVector[25]*fTopolVarVector[26];      }
      else {
        fTopolVarVector[14]=candidateD0->InvMassD0bar();
        fTopolVarVector[15]=d->Pt();
        fTopolVarVector[16]=d->DecayLength();
        fTopolVarVector[17]=d->DecayLengthXY();
        fTopolVarVector[18]=d->NormalizedDecayLengthXY();
        fTopolVarVector[19]=d->CosPointingAngle();
        fTopolVarVector[20]=d->CosPointingAngleXY();
        fTopolVarVector[21]=d->ImpParXY();
        fTopolVarVector[22]=d->PtProng(0);
        fTopolVarVector[23]=d->PtProng(1);
        fTopolVarVector[24]=candidateD0->CosThetaStarD0bar();
        fTopolVarVector[25]=((AliAODRecoDecayHF2Prong*)d)->Getd0Prong(0);
        fTopolVarVector[26]=((AliAODRecoDecayHF2Prong*)d)->Getd0Prong(1);
        fTopolVarVector[27]=fTopolVarVector[25]*fTopolVarVector[26];  
      }
    break;
  }

  if(fPidOpt==kNoPID || !pidHF) return true; //if no PID, return before

  SetPidVars(d,pidHF);
  return true;
}

//________________________________________________________________
TTree* AliHFCutOptTreeHandler::BuildTree(TString name, TString title) 
{
  if(fTreeTopolVar) delete fTreeTopolVar;
  fTreeTopolVar=0x0;
  fTreeTopolVar = new TTree(name.Data(),title.Data());

  TString topolvarnamesCommon[knTopolVarsCommon] = {"inv_mass","pt_cand","d_len","d_len_xy","norm_dl_xy","cos_p","cos_p_xy","imp_par_xy","pt_prong0","pt_prong1"};
  TString topolvarNamesDzero[knTopolVarsDzero] = {"cos_t_star", "imp_par_prong0", "imp_par_prong1","imp_par_prod"};
  TString topolvarNamesDs[knTopolVarsDs] = {"pt_prong2","sig_vert","delta_mass_KK","cos_PiDs","cos_PiKPhi_3"};
  TString topolvarNamesDplus[knTopolVarsDplus] = {"pt_prong2","sig_vert"};
  TString topolvarNamesBplus[knTopolVarsBplus] = {"cos_t_star", "imp_par_prong0", "imp_par_prong1","imp_par_prod","inv_mass_D0","pt_cand_D0","d_len_D0","d_len_xy_D0","norm_dl_xy_D0","cos_p_D0","cos_p_xy_D0","imp_par_xy_D0","pt_prong0_D0","pt_prong1_D0","cos_t_star_D0", "imp_par_prong0_D0", "imp_par_prong1_D0","imp_par_prod_D0"};
  TString PIDvarnamesNsigma[knPidVars] = {"nsigTPC_Pi_0","nsigTPC_K_0","nsigTOF_Pi_0","nsigTOF_K_0","nsigTPC_Pi_1","nsigTPC_K_1","nsigTOF_Pi_1","nsigTOF_K_1","nsigTPC_Pi_2","nsigTPC_K_2","nsigTOF_Pi_2","nsigTOF_K_2"};
  TString PIDvarnamesNsigmaComb[knPidVars] = {"nsigComb_Pi_0","nsigComb_K_0","nsigComb_Pi_1","nsigComb_K_1","nsigComb_Pi_2","nsigComb_K_2","","","","","",""};

  for(int iVar=0; iVar<knTopolVarsCommon; iVar++) {
    fTreeTopolVar->Branch(topolvarnamesCommon[iVar].Data(),&fTopolVarVector[iVar],Form("%s/F",topolvarnamesCommon[iVar].Data()));
  }
  switch(fDecayChannel){
    case 0: //D0 -> Kpi
      for(int iVar=0; iVar<knTopolVarsDzero; iVar++){
        fTreeTopolVar->Branch(topolvarNamesDzero[iVar].Data(),&fTopolVarVector[knTopolVarsCommon+iVar],Form("%s/F",topolvarNamesDzero[iVar].Data()));
      }
    break;
    case 1: //D+ -> Kpipi
      for(int iVar=0; iVar<knTopolVarsDplus; iVar++){
        fTreeTopolVar->Branch(topolvarNamesDplus[iVar].Data(),&fTopolVarVector[knTopolVarsCommon+iVar],Form("%s/F",topolvarNamesDplus[iVar].Data()));
      }
    break;
    case 2: //Ds -> KKpi
      for(int iVar=0; iVar<knTopolVarsDs; iVar++){
        fTreeTopolVar->Branch(topolvarNamesDs[iVar].Data(),&fTopolVarVector[knTopolVarsCommon+iVar],Form("%s/F",topolvarNamesDs[iVar].Data()));
      }
    break;
    case 3: //Bplus -> D0pi
      for(int iVar=0; iVar<knTopolVarsBplus; iVar++){
        fTreeTopolVar->Branch(topolvarNamesBplus[iVar].Data(),&fTopolVarVector[knTopolVarsCommon+iVar],Form("%s/F",topolvarNamesBplus[iVar].Data()));
      }
    break;
  }

  fTreeTopolVar->Branch("cand_type",&fCandType,"cand_type/B");
  if(fUseCentrality) fTreeTopolVar->Branch("centrality",&fCentrality,"centrality/B");
  if(fUseSelFlag) fTreeTopolVar->Branch("isselectedstd",&fIsSelStd,"isselectedstd/O");

  switch(fPidOpt) {
    case 0: //no PID
      return fTreeTopolVar;
    break;
    case 1: // nsigmaPID
      for(int iVar=0; iVar<knPidVars; iVar++) {
        if(iVar>7 && fDecayChannel==kD0toKpi) continue; 
        fTreeTopolVar->Branch(PIDvarnamesNsigma[iVar].Data(),&fPIDnSigmaVector[iVar],Form("%s/F",PIDvarnamesNsigma[iVar].Data()));
      }
    break;
    case 2: //nsigmaPIDchar
    for(int iVar=0; iVar<knPidVars; iVar++) {
      if(iVar>7 && fDecayChannel==kD0toKpi) continue; 
      fTreeTopolVar->Branch(PIDvarnamesNsigma[iVar].Data(),&fPIDnSigmaCharVector[iVar],Form("%s/B",PIDvarnamesNsigma[iVar].Data()));
    }
    break;
    case 3: //nsigmaPIDfloatandchar
    for(int iVar=0; iVar<knPidVars; iVar++) {
      if(iVar>7 && fDecayChannel==kD0toKpi) continue; 
      fTreeTopolVar->Branch(PIDvarnamesNsigma[iVar].Data(),&fPIDnSigmaVector[iVar],Form("%s/F",PIDvarnamesNsigma[iVar].Data()));
      fTreeTopolVar->Branch(Form("%s_char",PIDvarnamesNsigma[iVar].Data()),&fPIDnSigmaCharVector[iVar],Form("%s_char/B",PIDvarnamesNsigma[iVar].Data()));
    }
    break;
    case 4: // nsigmaCombPID
      for(int iVar=0; iVar<knPidVars/2; iVar++) {
        if(iVar>3 && fDecayChannel==kD0toKpi) continue; 
        fTreeTopolVar->Branch(PIDvarnamesNsigmaComb[iVar].Data(),&fPIDnSigmaVector[iVar],Form("%s/F",PIDvarnamesNsigmaComb[iVar].Data()));
      }
    break;
    case 5: //nsigmaCombPIDchar
    for(int iVar=0; iVar<knPidVars/2; iVar++) {
      if(iVar>3 && fDecayChannel==kD0toKpi) continue; 
      fTreeTopolVar->Branch(PIDvarnamesNsigmaComb[iVar].Data(),&fPIDnSigmaCharVector[iVar],Form("%s/B",PIDvarnamesNsigmaComb[iVar].Data()));
    }
    break;
    case 6: //nsigmaCombPIDfloatandchar
    for(int iVar=0; iVar<knPidVars/2; iVar++) {
      if(iVar>3 && fDecayChannel==kD0toKpi) continue; 
      fTreeTopolVar->Branch(PIDvarnamesNsigmaComb[iVar].Data(),&fPIDnSigmaVector[iVar],Form("%s/F",PIDvarnamesNsigmaComb[iVar].Data()));
      fTreeTopolVar->Branch(Form("%s_char",PIDvarnamesNsigmaComb[iVar].Data()),&fPIDnSigmaCharVector[iVar],Form("%s_char/B",PIDvarnamesNsigmaComb[iVar].Data()));
    }
    break;
    default: //no PID
      return fTreeTopolVar;
    break;
  }

  return fTreeTopolVar;
}

//________________________________________________________________
void AliHFCutOptTreeHandler::SetPidVars(AliAODRecoDecayHF* d, AliAODPidHF* pidHF) {
  //PID variables
  double sigTPC_K[knMaxProngs]={-999.,-999.,-999.};
  double sigTPC_Pi[knMaxProngs]={-999.,-999.,-999.};
  double sigTOF_K[knMaxProngs]={-999.,-999.,-999.};
  double sigTOF_Pi[knMaxProngs]={-999.,-999.,-999.};

  float sigComb_K[knMaxProngs] = {-1.,-1.,-1.};
  float sigComb_Pi[knMaxProngs] = {-1.,-1.,-1.};

  AliAODTrack *track[knMaxProngs] = {0x0,0x0,0x0};

  for(int iProng=0; iProng<knMaxProngs; iProng++) {
    track[iProng]=(AliAODTrack*)d->GetDaughter(0); // Remark Lennart: Is GetDaughter(0) correct? shouldn't this be iProng? // code for Bplus should take the daughters from the D0 and the pion of the Bplus for PID

    pidHF->GetnSigmaTPC(track[iProng],3,sigTPC_K[iProng]);
    pidHF->GetnSigmaTPC(track[iProng],2,sigTPC_Pi[iProng]);

    pidHF->GetnSigmaTOF(track[iProng],3,sigTOF_K[iProng]);
    pidHF->GetnSigmaTOF(track[iProng],2,sigTPC_Pi[iProng]);

    if(fDecayChannel==kD0toKpi && iProng>1) continue; //D0 -> Kpi only 2 prongs
    // if(fDecayChannel==kBplustoD0pi && iProng != 1) continue; //Bplus -> D0pi only 2 prongs of which only the pion is a final track

    if(fPidOpt>kNoPID && fPidOpt<=kNsigmaPIDfloatandchar) {
      fPIDnSigmaVector[4*iProng]=sigTPC_Pi[iProng]*10;
      fPIDnSigmaVector[4*iProng+1]=sigTPC_K[iProng]*10;
      fPIDnSigmaVector[4*iProng+2]=sigTPC_Pi[iProng]*10;
      fPIDnSigmaVector[4*iProng+3]=sigTOF_K[iProng]*10;

      //to be changed below
      if(sigTPC_Pi[iProng]>0) {
        if(sigTPC_Pi[iProng]<12.7) fPIDnSigmaCharVector[4*iProng]=sigTPC_Pi[iProng]*10;
        else fPIDnSigmaCharVector[4*iProng]=127;
      }
      else {
        if(sigTPC_Pi[iProng]>-12.7) fPIDnSigmaCharVector[4*iProng]=sigTPC_Pi[iProng]*10;
        else fPIDnSigmaCharVector[4*iProng]=-127;
      }
      if(sigTPC_K[iProng]>0) {
        if(sigTPC_K[iProng]<12.7) fPIDnSigmaCharVector[4*iProng+1]=sigTPC_K[iProng]*10;
        else fPIDnSigmaCharVector[4*iProng+1]=127;
      }
      else {
        if(sigTPC_K[iProng]>-12.7) fPIDnSigmaCharVector[4*iProng+1]=sigTPC_K[iProng]*10;
        else fPIDnSigmaCharVector[4*iProng+1]=-127;
      }
      if(sigTOF_Pi[iProng]>0) {
        if(sigTOF_Pi[iProng]<12.7) fPIDnSigmaCharVector[4*iProng+2]=sigTOF_Pi[iProng]*10;
        else fPIDnSigmaCharVector[4*iProng+2]=127;
      }
      else {
        if(sigTOF_Pi[iProng]>-12.7) fPIDnSigmaCharVector[4*iProng+2]=sigTOF_Pi[iProng]*10;
        else fPIDnSigmaCharVector[4*iProng+2]=-127;
      }
      if(sigTOF_K[iProng]>0) {
        if(sigTOF_K[iProng]<12.7) fPIDnSigmaCharVector[4*iProng+3]=sigTOF_K[iProng]*10;
        else fPIDnSigmaCharVector[4*iProng+3]=127;
      }
      else {
        if(sigTOF_K[iProng]>-12.7) fPIDnSigmaCharVector[4*iProng+3]=sigTOF_K[iProng]*10;
        else fPIDnSigmaCharVector[4*iProng+3]=-127;
      }
    }
    else if(fPidOpt>kNsigmaPIDfloatandchar && fPidOpt<=kNsigmaCombPIDfloatandchar) {

      //PID TPC and TOF squared sum, normalized to sqrt(2)      
      if(sigTPC_K[iProng] > -998.){
        if(sigTOF_K[iProng] > -998.)
          sigComb_K[iProng] = TMath::Sqrt((sigTPC_K[iProng]*sigTPC_K[iProng] + sigTOF_K[iProng]*sigTOF_K[iProng]) / 2.);
        else
          sigComb_K[iProng] = TMath::Abs(sigTPC_K[iProng]);
      }

      if(sigTPC_Pi[iProng] > -998.){
        if(sigTOF_Pi[iProng] > -998.)
          sigComb_Pi[iProng] = TMath::Sqrt((sigTPC_Pi[iProng]*sigTPC_Pi[iProng] + sigTOF_Pi[iProng]*sigTOF_Pi[iProng]) / 2.);
        else
          sigComb_Pi[iProng] = TMath::Abs(sigTPC_Pi[iProng]);
      }

      fPIDnSigmaVector[2*iProng]=sigComb_Pi[iProng]*10;
      fPIDnSigmaVector[2*iProng+1]=sigComb_K[iProng]*10;

      sigComb_Pi[iProng]<12.7 ? fPIDnSigmaCharVector[2*iProng]=sigComb_Pi[iProng]*10 : 127; //set last bin
      sigComb_K[iProng]<12.7 ? fPIDnSigmaCharVector[2*iProng+1]=sigComb_K[iProng]*10 : 127; //set last bin
    }
  }
}

//________________________________________________________________
void AliHFCutOptTreeHandler::SetPdgCodes() {
  
  switch(fDecayChannel) {
    case 0:
      fPdgCode=421;
      fPdgCodeProngs[0]=321;
      fPdgCodeProngs[1]=211;
      fPdgCodeProngs[2]=-1;
    break;
    case 1:
      fPdgCode=411;
      fPdgCodeProngs[0]=321;
      fPdgCodeProngs[1]=211;
      fPdgCodeProngs[2]=211;
    break;
    case 2:
      fPdgCode=431;
      fPdgCodeProngs[0]=321;
      fPdgCodeProngs[1]=321;
      fPdgCodeProngs[2]=211;
    break;
    case 3:
      fPdgCode=521;
      fPdgCodeProngs[0]=211;
      fPdgCodeProngs[1]=421;
      fPdgCodeProngs[2]=-1;
    break;
    default:
      fPdgCode=-1;
      fPdgCodeProngs[0]=-1;
      fPdgCodeProngs[1]=-1;
      fPdgCodeProngs[2]=-1;
    break;
  }
}
//-------------------------------------------------------------------------------------
Int_t AliHFCutOptTreeHandler::MatchBPlusCandidateToMonteCarlo(Int_t pdgabs, AliAODRecoDecayHF2Prong * candidate, TClonesArray *mcArray) const
{
  //
  // Check if this candidate is matched to a MC signal
  // If no, return -1
  // If yes, return label (>=0) of the AliAODMCParticle


  // Check number of daughters
  Int_t ndg = candidate->GetNDaughters();
  if (!ndg) { AliError("No daughters available"); return -1;}
  if (ndg != 2) return -1;

  // loop on daughters and write the labels
  Int_t dgLabels[2] = { -1};
  Int_t pdgDg[2] = {0};
  if (pdgabs == 421)
  {
    AliAODTrack *trk0 = (AliAODTrack*)candidate->GetDaughter(0);
    dgLabels[0] = trk0->GetLabel();
    AliAODTrack *trk1 = (AliAODTrack*)candidate->GetDaughter(1);
    dgLabels[1] = trk1->GetLabel();
    pdgDg[0] = 211; pdgDg[1] = 321;
  }
  else if (pdgabs == 521)
  {
    AliAODTrack *trk0 = (AliAODTrack*)candidate->GetDaughter(0);
    dgLabels[0] = trk0->GetLabel();
    dgLabels[1] = MatchBPlusCandidateToMonteCarlo(421, (AliAODRecoDecayHF2Prong*)candidate->GetDaughter(1), mcArray);
    pdgDg[0] = 211; pdgDg[1] = 421;
  }
  else
  {
    std::cout << "Wrong pdg supplied for function to match candidate to monte carlo signal." << std::endl;
    return -1;
  }
  if (dgLabels[0] == -1) return -1;
  if (dgLabels[1] == -1) return -1;


  Int_t labMom[2] = {0, 0};
  Int_t i, j, lab, labMother, pdgMother, pdgPart;
  AliAODMCParticle *part = 0;
  AliAODMCParticle *mother = 0;
  Double_t pxSumDgs = 0., pySumDgs = 0., pzSumDgs = 0.;
  Bool_t pdgUsed[2] = {kFALSE, kFALSE};

  // loop on daughter labels
  for (i = 0; i < ndg; i++)
  {
    labMom[i] = -1;
    lab = TMath::Abs(dgLabels[i]);
    if (lab < 0)
    {
      printf("daughter with negative label %d\n", lab);
      return -1;
    }
    part = (AliAODMCParticle*)mcArray->At(lab);
    if (!part)
    {
      printf("no MC particle\n");
      return -1;
    }

    // check the PDG of the daughter
    pdgPart = TMath::Abs(part->GetPdgCode());
    for (j = 0; j < ndg; j++)
    {
      if (!pdgUsed[j] && pdgPart == pdgDg[j])
      {
        pdgUsed[j] = kTRUE;
        break;
      }
    }


    mother = part;
    while (mother->GetMother() >= 0)
    {
      labMother = mother->GetMother();
      mother = (AliAODMCParticle*)mcArray->At(labMother);
      if (!mother)
      {
        printf("no MC mother particle\n");
        break;
      }
      pdgMother = TMath::Abs(mother->GetPdgCode());
      if (pdgMother == pdgabs)
      {
        labMom[i] = labMother;
        // keep sum of daughters' momenta, to check for mom conservation
        pxSumDgs += part->Px();
        pySumDgs += part->Py();
        pzSumDgs += part->Pz();
        break;
      }
      else break;
    }
    if (labMom[i] == -1) return -1; // mother PDG not ok for this daughter
  } // end loop on daughters

  // check if the candidate is signal
  labMother = labMom[0];
  // all labels have to be the same and !=-1
  for (i = 0; i < ndg; i++)
  {
    if (labMom[i] == -1)        return -1;
    if (labMom[i] != labMother) return -1;
  }

  // check that all daughter PDGs are matched
  for (i = 0; i < ndg; i++)
  {
    if (pdgUsed[i] == kFALSE) return -1;
  }

  // Check for mom conservation
  mother = (AliAODMCParticle*)mcArray->At(labMother);
  Double_t pxMother = mother->Px();
  Double_t pyMother = mother->Py();
  Double_t pzMother = mother->Pz();


  // check the number of daughters (we are not looking at resonant decay)
  if(mother->GetNDaughters() != 2) return -1;

  // if momentum conservation is not within 0.5%, show warning. This can be due to large propagation distance through magnetic field.
  if ((TMath::Abs(pxMother - pxSumDgs) / (TMath::Abs(pxMother) + 1.e-13)) > 0.005 ||
      (TMath::Abs(pyMother - pySumDgs) / (TMath::Abs(pyMother) + 1.e-13)) > 0.005 ||
      (TMath::Abs(pzMother - pzSumDgs) / (TMath::Abs(pzMother) + 1.e-13)) > 0.005)
  {
    std::cout << std::endl << " Momentum difference for decay pdgabs = " << pdgabs << "daughters = " << mother->GetNDaughters() << std::endl;
    std::cout << "pxMother = " << pxMother << "pyMother = " << pyMother << "pzMother = " << pzMother << std::endl;
    std::cout << "pxSumDgs = " << pxSumDgs << "pySumDgs = " << pySumDgs << "pzSumDgs = " << pzSumDgs << std::endl;
  }

  return labMother;
}