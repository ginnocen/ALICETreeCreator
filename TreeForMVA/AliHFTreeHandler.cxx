/* Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//*************************************************************************
// \class AliHFTreeHandler
// \brief helper class to handle a tree for cut optimisation and MVA analyses
// \authors:
// F. Catalano, fabio.catalano@cern.ch
// A. Festanti, andrea.festanti@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
// G. Innocenti, gian.michele.innocenti@cern.ch
// F. Prino, prino@to.infn.it
// L. Vermunt, luuk.vermunt@cern.ch
/////////////////////////////////////////////////////////////

#include <cmath>
#include <limits>
#include "AliHFTreeHandler.h"
#include "AliPID.h"
#include "AliAODRecoDecayHF.h"
#include "AliPIDResponse.h"

/// \cond CLASSIMP
ClassImp(AliHFTreeHandler);
/// \endcond

//________________________________________________________________
AliHFTreeHandler::AliHFTreeHandler():
  TObject(),
  fTreeVar(0x0),
  fNProngs(),
  fNCandidates(0),
  fCandTypeMap(0),
  fCandType(),
  fInvMass(),
  fPt(),
  fY(),
  fEta(),
  fPhi(),
  fDecayLength(),
  fDecayLengthXY(),
  fNormDecayLengthXY(),
  fCosP(),
  fCosPXY(),
  fImpParXY(),
  fNormd0MeasMinusExp(),
  fPProng(),
  fTPCPProng(),
  fPtProng(),
  fEtaProng(),
  fImpParProng(),
  fPhiProng(),
  fNTPCclsProng(),
  fNTPCclsPidProng(),
  fNTPCCrossedRowProng(),
  fChi2perNDFProng(),
  fNITSclsProng(),
  fITSclsMapProng(),
  fPIDVarVector(),
  fPIDVarIntVector(),
  fPidOpt(kNsigmaPID),
  fFillOnlySignal(false)
{
  //
  // Default constructor
  //
}

//________________________________________________________________
AliHFTreeHandler::AliHFTreeHandler(int PIDopt):
  TObject(),
  fTreeVar(0x0),
  fNProngs(),
  fNCandidates(0),
  fCandTypeMap(0),
  fCandType(),
  fInvMass(),
  fPt(),
  fY(),
  fEta(),
  fPhi(),
  fDecayLength(),
  fDecayLengthXY(),
  fNormDecayLengthXY(),
  fCosP(),
  fCosPXY(),
  fImpParXY(),
  fNormd0MeasMinusExp(),
  fPProng(),
  fTPCPProng(),
  fPtProng(),
  fEtaProng(),
  fImpParProng(),
  fPhiProng(),
  fNTPCclsProng(),
  fNTPCclsPidProng(),
  fNTPCCrossedRowProng(),
  fChi2perNDFProng(),
  fNITSclsProng(),
  fITSclsMapProng(),
  fPIDVarVector(),
  fPIDVarIntVector(),
  fPidOpt(PIDopt),
  fFillOnlySignal(false)
{
  //
  // Standard constructor
  //
}

//________________________________________________________________
AliHFTreeHandler::~AliHFTreeHandler()
{
  //
  // Destructor
  //

  if(fTreeVar) delete fTreeVar;
}

//________________________________________________________________
void AliHFTreeHandler::SetCandidateType(bool issignal, bool isbkg, bool isprompt, bool isFD, bool isreflected) 
{  
  if(issignal) fCandTypeMap |= kSignal;
  else fCandTypeMap &= ~kSignal;
  if(isbkg) fCandTypeMap |= kBkg;
  else fCandTypeMap &= ~kBkg;
  if(isprompt) fCandTypeMap |= kPrompt;
  else fCandTypeMap &= ~kPrompt;
  if(isFD) fCandTypeMap |= kFD;
  else fCandTypeMap &= ~kFD;
  if(isreflected) fCandTypeMap |= kRefl;
  else fCandTypeMap &= ~kRefl;
}

//________________________________________________________________
void AliHFTreeHandler::AddCommonDmesonVarBranches() {

  fTreeVar->Branch("n_cand",&fNCandidates);
  fTreeVar->Branch("cand_type",&fCandType);
  fTreeVar->Branch("inv_mass",&fInvMass);
  fTreeVar->Branch("pt_cand",&fPt);
  fTreeVar->Branch("y_cand",&fY);
  fTreeVar->Branch("eta_cand",&fEta);
  fTreeVar->Branch("phi_cand",&fPhi);
  fTreeVar->Branch("d_len",&fDecayLength);
  fTreeVar->Branch("d_len_xy",&fDecayLengthXY);
  fTreeVar->Branch("norm_dl_xy",&fNormDecayLengthXY);
  fTreeVar->Branch("cos_p",&fCosP);
  fTreeVar->Branch("cos_p_xy",&fCosPXY);
  fTreeVar->Branch("imp_par_xy",&fImpParXY);
  fTreeVar->Branch("max_norm_d0d0exp",&fNormd0MeasMinusExp);
} 

//________________________________________________________________
void AliHFTreeHandler::AddSingleTrackBranches() {

  for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
    fTreeVar->Branch(Form("p_prong%d",iProng),&fPProng[iProng]);
    if(fPidOpt==kRawPID) fTreeVar->Branch(Form("pTPC_prong%d",iProng),&fTPCPProng[iProng]);
    fTreeVar->Branch(Form("pt_prong%d",iProng),&fPtProng[iProng]);
    fTreeVar->Branch(Form("eta_prong%d",iProng),&fEtaProng[iProng]);
    fTreeVar->Branch(Form("imp_par_prong%d",iProng),&fImpParProng[iProng]);
    fTreeVar->Branch(Form("phi_prong%d",iProng),&fPhiProng[iProng]);
    fTreeVar->Branch(Form("nTPCcls_prong%d",iProng),&fNTPCclsProng[iProng]);
    fTreeVar->Branch(Form("nTPCclspid_prong%d",iProng),&fNTPCclsPidProng[iProng]);
    fTreeVar->Branch(Form("nTPCcrossrow_prong%d",iProng),&fNTPCCrossedRowProng[iProng]);
    fTreeVar->Branch(Form("chi2perndf_prong%d",iProng),&fChi2perNDFProng[iProng]);
    fTreeVar->Branch(Form("nITScls_prong%d",iProng),&fNITSclsProng[iProng]);
    fTreeVar->Branch(Form("ITSclsmap_prong%d",iProng),&fITSclsMapProng[iProng]);
  }
}

//________________________________________________________________
void AliHFTreeHandler::AddPidBranches(bool usePionHypo, bool useKaonHypo, bool useProtonHypo, bool useTPC, bool useTOF) 
{

  if(fPidOpt==kNoPID) return;
  if(fPidOpt>kRawPID) {
    AliWarning("Wrong PID setting!");
    return;
  }

  bool useHypo[knMaxHypo4Pid] = {usePionHypo,useKaonHypo,useProtonHypo};
  bool useDet[knMaxDet4Pid] = {useTPC,useTOF};
  TString partHypoName[knMaxHypo4Pid] = {"Pi","K","Pr"};
  TString detName[knMaxDet4Pid] = {"TPC","TOF"};
  TString rawPidName[knMaxDet4Pid] = {"dEdxTPC","ToF"};

  for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
    if(fPidOpt>=kNsigmaPID && fPidOpt<=kNsigmaPIDfloatandint) {
      for(unsigned int iDet=0; iDet<knMaxDet4Pid; iDet++) {
        if(!useDet[iDet]) continue;
        for(unsigned int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
          if(!useHypo[iPartHypo]) continue;
          if(fPidOpt==kNsigmaPID) fTreeVar->Branch(Form("nsig%s_%s_%d",detName[iDet].Data(),partHypoName[iPartHypo].Data(),iProng),&fPIDVarVector[iProng][iDet][iPartHypo]);
          else if(fPidOpt==kNsigmaPIDint) fTreeVar->Branch(Form("nsig%s_%s_%d",detName[iDet].Data(),partHypoName[iPartHypo].Data(),iProng),&fPIDVarIntVector[iProng][iDet][iPartHypo]);
          else if(fPidOpt==kNsigmaPIDfloatandint) {
            fTreeVar->Branch(Form("nsig%s_%s_%d",detName[iDet].Data(),partHypoName[iPartHypo].Data(),iProng),&fPIDVarVector[iProng][iDet][iPartHypo]);
            fTreeVar->Branch(Form("int_nsig%s_%s_%d",detName[iDet].Data(),partHypoName[iPartHypo].Data(),iProng),&fPIDVarIntVector[iProng][iDet][iPartHypo]);
          }
        }
      }
    }
    else if(fPidOpt>=kNsigmaCombPID && fPidOpt<=kNsigmaCombPIDfloatandint) {
      for(unsigned int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
        if(!useHypo[iPartHypo]) continue;
        if(fPidOpt==kNsigmaCombPID) fTreeVar->Branch(Form("nsigComb_%s_%d",partHypoName[iPartHypo].Data(),iProng),&fPIDVarVector[iProng][0][iPartHypo]);
        else if(fPidOpt==kNsigmaCombPIDint) fTreeVar->Branch(Form("nsigComb_%s_%d",partHypoName[iPartHypo].Data(),iProng),&fPIDVarIntVector[iProng][0][iPartHypo]);
        else if(fPidOpt==kNsigmaCombPIDfloatandint) {
          fTreeVar->Branch(Form("nsigComb_%s_%d",partHypoName[iPartHypo].Data(),iProng),&fPIDVarVector[iProng][0][iPartHypo]);
          fTreeVar->Branch(Form("int_nsigComb_%s_%d",partHypoName[iPartHypo].Data(),iProng),&fPIDVarIntVector[iProng][0][iPartHypo]);
        }
      }
    }
    else if(fPidOpt==kRawPID) {
      for(unsigned int iDet=0; iDet<knMaxDet4Pid; iDet++) {
        if(!useDet[iDet]) continue;
        fTreeVar->Branch(Form("%s_%d",rawPidName[iDet].Data(),iProng),&fPIDVarVector[iProng][iDet][0]);
      }
    }
  }
}

//________________________________________________________________
bool AliHFTreeHandler::SetSingleTrackVars(AliAODTrack* prongtracks[], AliAODRecoDecayHF* cand) {

  for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
    if(!prongtracks[iProng]) {
      AliWarning("Prong track not found!");
      return false;
    }
  }

  for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
    fPProng[iProng].push_back(cand->PProng(iProng));
    if(fPidOpt==kRawPID) fTPCPProng[iProng].push_back(prongtracks[iProng]->GetTPCmomentum());
    fPtProng[iProng].push_back(cand->PtProng(iProng));
    fEtaProng[iProng].push_back(cand->EtaProng(iProng));
    fImpParProng[iProng].push_back(cand->PhiProng(iProng));
    fPhiProng[iProng].push_back(cand->PProng(iProng));
    fNTPCclsProng[iProng].push_back(prongtracks[iProng]->GetTPCNcls());
    fNTPCclsPidProng[iProng].push_back(prongtracks[iProng]->GetTPCsignalN());
    fNTPCCrossedRowProng[iProng].push_back(prongtracks[iProng]->GetTPCNCrossedRows());
    fChi2perNDFProng[iProng].push_back(prongtracks[iProng]->Chi2perNDF());
    fNITSclsProng[iProng].push_back(prongtracks[iProng]->GetITSNcls());
    fITSclsMapProng[iProng].push_back(static_cast<int>(prongtracks[iProng]->GetITSClusterMap()));
  }

  return true;
}

//________________________________________________________________
bool AliHFTreeHandler::SetPidVars(AliAODTrack* prongtracks[], AliAODPidHF* pidHF, bool usePionHypo, bool useKaonHypo, bool useProtonHypo, bool useTPC, bool useTOF) 
{
  if(!pidHF) return false;
  for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
    if(!prongtracks[iProng]) {
      AliWarning("Prong track not found!");
      return false;
    }
  }

  //PID variables
  double sig[knMaxProngs][knMaxDet4Pid][knMaxHypo4Pid];
  double sigComb[knMaxProngs][knMaxHypo4Pid];
  double rawPID[knMaxProngs][knMaxDet4Pid];
  bool useHypo[knMaxHypo4Pid] = {usePionHypo,useKaonHypo,useProtonHypo};
  AliPID::EParticleType parthypo[knMaxHypo4Pid] = {AliPID::kPion,AliPID::kKaon,AliPID::kProton};

  //compute PID variables for different options
  for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
    if(fPidOpt>=kNsigmaPID && fPidOpt<=kNsigmaCombPIDfloatandint) {
      for(unsigned int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
        if(useHypo[iPartHypo]) {
          if(useTPC) pidHF->GetnSigmaTPC(prongtracks[iProng],parthypo[iPartHypo],sig[iProng][kTPC][iPartHypo]);
          if(useTOF) pidHF->GetnSigmaTOF(prongtracks[iProng],parthypo[iPartHypo],sig[iProng][kTOF][iPartHypo]);
          if(fPidOpt>=kNsigmaCombPID && fPidOpt<=kNsigmaCombPIDfloatandint) {
            sigComb[iProng][iPartHypo] = CombineNsigmaDiffDet(sig[iProng][kTPC][iPartHypo],sig[iProng][kTOF][iPartHypo]);
          }
        }
      }
    }
    else if(fPidOpt==kRawPID) {
      AliPIDResponse* pidrespo = (AliPIDResponse*)pidHF->GetPidResponse();
      rawPID[iProng][kTPC] = prongtracks[iProng]->GetTPCsignal();
      rawPID[iProng][kTOF] = prongtracks[iProng]->GetTOFsignal();
      float time0 = pidrespo->GetTOFResponse().GetStartTime(prongtracks[iProng]->P());
      rawPID[iProng][kTOF] -= time0;
    }
  }

  //fill PID arrays for different options
  switch(fPidOpt) {
    case 1: //kNsigmaPID
      for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
        for(int iDet=kTPC; iDet<=kTOF; iDet++) {
          for(unsigned int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
            fPIDVarVector[iProng][iDet][iPartHypo].push_back(sig[iProng][iDet][iPartHypo]);
          }
        }
      }
    break;
    case 2: //kNsigmaPIDint
      for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
        for(int iDet=kTPC; iDet<=kTOF; iDet++) {
          for(unsigned int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
            fPIDVarIntVector[iProng][iDet][iPartHypo].push_back(RoundFloatToInt(sig[iProng][iDet][iPartHypo]*100));
          }
        }
      }
    break;
    case 3: //kNsigmaPIDfloatandint
      for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
        for(int iDet=kTPC; iDet<=kTOF; iDet++) {
          for(unsigned int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
            fPIDVarVector[iProng][iDet][iPartHypo].push_back(sig[iProng][iDet][iPartHypo]*100);  
            fPIDVarIntVector[iProng][iDet][iPartHypo].push_back(RoundFloatToInt(sig[iProng][iDet][iPartHypo]*100));
          }
        }
      }
    break;
    case 4: //kNsigmaCombPID
      for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
        for(unsigned int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
          fPIDVarVector[iProng][0][iPartHypo].push_back(sigComb[iProng][iPartHypo]);        
        }
      }
    break;
    case 5: //kNsigmaCombPIDint
      for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
        for(unsigned int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
          fPIDVarIntVector[iProng][0][iPartHypo].push_back(RoundFloatToInt(sigComb[iProng][iPartHypo]*100));
        }
      }
    break;
    case 6: //kNsigmaCombPIDfloatandint
      for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
        for(unsigned int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
          fPIDVarVector[iProng][0][iPartHypo].push_back(sigComb[iProng][iPartHypo]*100);     
          fPIDVarIntVector[iProng][0][iPartHypo].push_back(RoundFloatToInt(sigComb[iProng][iPartHypo]*100));
        }
      }
    break;
    case 7: //kRawPID
      for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
        for(int iDet=kTPC; iDet<=kTOF; iDet++) {
          fPIDVarVector[iProng][iDet][0].push_back(rawPID[iProng][iDet]);     
        }
      }
    break;
    default:
      AliWarning("Wrong PID setting!");
      return false;
    break;
  }

  return true;
}

//________________________________________________________________
void AliHFTreeHandler::ResetDmesonCommonVarVectors() {

  fCandType.clear();
  fInvMass.clear();
  fPt.clear();
  fY.clear();
  fEta.clear();
  fPhi.clear();
  fDecayLength.clear();
  fDecayLengthXY.clear();
  fNormDecayLengthXY.clear();
  fCosP.clear();
  fCosPXY.clear();
  fImpParXY.clear();
  fNormd0MeasMinusExp.clear();
}

//________________________________________________________________
void AliHFTreeHandler::ResetSingleTrackVarVectors() {
  for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
    fPProng[iProng].clear();
    fTPCPProng[iProng].clear();
    fPtProng[iProng].clear();
    fEtaProng[iProng].clear();
    fImpParProng[iProng].clear();
    fPhiProng[iProng].clear();
    fNTPCclsProng[iProng].clear();
    fNTPCclsPidProng[iProng].clear();
    fNTPCCrossedRowProng[iProng].clear();
    fChi2perNDFProng[iProng].clear();
    fNITSclsProng[iProng].clear();
    fITSclsMapProng[iProng].clear();
  }
}

//________________________________________________________________
void AliHFTreeHandler::ResetPidVarVectors() {

  for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
    for(unsigned int iDet=0; iDet<knMaxDet4Pid; iDet++) {
      for(unsigned int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
        fPIDVarVector[iProng][iDet][iPartHypo].clear();
        fPIDVarIntVector[iProng][iDet][iPartHypo].clear();
      }
    }
  }
}

//________________________________________________________________
double AliHFTreeHandler::CombineNsigmaDiffDet(double nsigmaTPC, double nsigmaTOF)
{
  if(nsigmaTPC > -998. && nsigmaTOF > -998.) return TMath::Sqrt((nsigmaTPC*nsigmaTPC+nsigmaTOF*nsigmaTOF)/2);
  else if(nsigmaTPC > -998. && nsigmaTOF < -998.) return nsigmaTPC;
  else if(nsigmaTPC > -998. && nsigmaTOF < -998.) return nsigmaTPC;
  else return -999.;
}

//________________________________________________________________
int AliHFTreeHandler::RoundFloatToInt(double num) 
{
  if(num>=static_cast<double>(numeric_limits<int>::max())) return numeric_limits<int>::max();
  else if(num<=static_cast<double>(numeric_limits<int>::min())) return numeric_limits<int>::min();
 
  return static_cast<int>(num);
}

//________________________________________________________________
float AliHFTreeHandler::ComputeMaxd0MeasMinusExp(AliAODRecoDecayHF* cand, float bfield) 
{
  float dd0max=0;
  for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
    double d0diff, errd0diff;
    cand->Getd0MeasMinusExpProng(iProng,bfield,d0diff,errd0diff);
    float normdd0 = d0diff/errd0diff;
    if(iProng==0) dd0max=normdd0;
    else if(TMath::Abs(normdd0)>TMath::Abs(dd0max)) dd0max=normdd0;
  }
  return dd0max;
}

