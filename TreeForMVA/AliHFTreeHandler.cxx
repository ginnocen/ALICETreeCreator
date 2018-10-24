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
  fPdgCode(),
  fNProngs(),
  fPidOpt(kNsigmaPID),
  fCandType(0),
  fEnableCentrality(false),
  fCentrality(-1),
  fEnableNormd0MeasMinusExp(false),
  fNormd0MeasMinusExp(-999.),
  fFillOnlySignal(false)
{
  //
  // Default constructor
  //

  for(int iVar=0; iVar<knCommonDmesonVars; iVar++) fCommonVarVector[iVar] = -999.;
  for(int iProng=0; iProng<fNProngs; iProng++) {
    for(int iDet=0; iDet<knMaxDet4Pid; iDet++) {
      for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
        fPIDVarVector[iProng][iDet][iPartHypo]=-999;
        fPIDVarIntVector[iProng][iDet][iPartHypo]=-127;
      }
    }
  }
}

//________________________________________________________________
AliHFTreeHandler::AliHFTreeHandler(int PIDopt):
  TObject(),
  fTreeVar(0x0),
  fPdgCode(),
  fNProngs(),
  fPidOpt(PIDopt),
  fCandType(0),
  fEnableCentrality(false),
  fCentrality(-1),
  fEnableNormd0MeasMinusExp(false),
  fNormd0MeasMinusExp(-999.),
  fFillOnlySignal(false)
{
  //
  // Standard constructor
  //

  for(int iVar=0; iVar<knCommonDmesonVars; iVar++) fCommonVarVector[iVar] = -999.;
  for(int iProng=0; iProng<fNProngs; iProng++) {
    for(int iDet=0; iDet<knMaxDet4Pid; iDet++) {
      for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
        fPIDVarVector[iProng][iDet][iPartHypo]=-999;
        fPIDVarIntVector[iProng][iDet][iPartHypo]=-127;
      }
    }
  }
}

//________________________________________________________________
AliHFTreeHandler::AliHFTreeHandler(const AliHFTreeHandler &source) :
  TObject(),
  fTreeVar(source.fTreeVar),
  fPdgCode(source.fPdgCode),
  fNProngs(source.fNProngs),
  fPidOpt(source.fPidOpt),
  fCandType(source.fCandType),
  fEnableCentrality(source.fEnableCentrality),
  fCentrality(source.fCentrality),
  fEnableNormd0MeasMinusExp(source.fEnableNormd0MeasMinusExp),
  fNormd0MeasMinusExp(source.fNormd0MeasMinusExp),
  fFillOnlySignal(source.fFillOnlySignal)
{
  //
  // Copy constructor
  //

  for(int iVar=0; iVar<knCommonDmesonVars; iVar++) fCommonVarVector[iVar] = source.fCommonVarVector[iVar];
  for(int iProng=0; iProng<fNProngs; iProng++) {
    for(int iDet=0; iDet<knMaxDet4Pid; iDet++) {
      for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
        fPIDVarVector[iProng][iDet][iPartHypo]=source.fPIDVarVector[iProng][iDet][iPartHypo];
        fPIDVarIntVector[iProng][iDet][iPartHypo]=source.fPIDVarIntVector[iProng][iDet][iPartHypo];
      }
    }
  }
}

//________________________________________________________________
AliHFTreeHandler &AliHFTreeHandler::operator=(const AliHFTreeHandler &source)
{
  //
  // assignment operator
  //

  if(&source == this) return *this;
 
  fTreeVar                  = source.fTreeVar;
  fPdgCode                  = source.fPdgCode;
  fNProngs                  = source.fNProngs;
  fPidOpt                   = source.fPidOpt;
  fCandType                 = source.fCandType;
  fEnableCentrality         = source.fEnableCentrality;
  fCentrality               = source.fCentrality;
  fEnableNormd0MeasMinusExp = source.fEnableNormd0MeasMinusExp;
  fNormd0MeasMinusExp       = source.fNormd0MeasMinusExp;
  fFillOnlySignal           = source.fFillOnlySignal;

  for(int iVar=0; iVar<knCommonDmesonVars; iVar++) fCommonVarVector[iVar] = source.fCommonVarVector[iVar];
  for(int iProng=0; iProng<fNProngs; iProng++) {
    for(int iDet=0; iDet<knMaxDet4Pid; iDet++) {
      for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
        fPIDVarVector[iProng][iDet][iPartHypo]=source.fPIDVarVector[iProng][iDet][iPartHypo];
        fPIDVarIntVector[iProng][iDet][iPartHypo]=source.fPIDVarIntVector[iProng][iDet][iPartHypo];
      }
    }
  }

  return *this;
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
  if(issignal) fCandType |= kSignal;
  else fCandType &= ~kSignal;
  if(isbkg) fCandType |= kBkg;
  else fCandType &= ~kBkg;
  if(isprompt) fCandType |= kPrompt;
  else fCandType &= ~kPrompt;
  if(isFD) fCandType |= kFD;
  else fCandType &= ~kFD;
  if(isreflected) fCandType |= kRefl;
  else fCandType &= ~kRefl;
}

//________________________________________________________________
void AliHFTreeHandler::AddCommonDmesonVarBranches() {

  fTreeVar->Branch("cand_type",&fCandType,"cand_type/I");
  for(int iVar=0; iVar<knCommonDmesonVars; iVar++) {
    fTreeVar->Branch(commonDvarnames[iVar].Data(),&fCommonVarVector[iVar],Form("%s/F",commonDvarnames[iVar].Data()));
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

  for(int iProng=0; iProng<fNProngs; iProng++) {
    if(fPidOpt>=kNsigmaPID && fPidOpt<=kNsigmaPIDfloatandint) {
      for(int iDet=0; iDet<knMaxDet4Pid; iDet++) {
        if(!useDet[iDet]) continue;
        for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
          if(!useHypo[iPartHypo]) continue;
          if(fPidOpt==kNsigmaPID) fTreeVar->Branch(Form("nsig%s_%s_%d",detName[iDet].Data(),partHypoName[iPartHypo].Data(),iProng),&fPIDVarVector[iProng][iDet][iPartHypo],Form("nsig%s_%s_%d/F",detName[iDet].Data(),partHypoName[iPartHypo].Data(),iProng));
          else if(fPidOpt==kNsigmaPIDint) fTreeVar->Branch(Form("nsig%s_%s_%d",detName[iDet].Data(),partHypoName[iPartHypo].Data(),iProng),&fPIDVarIntVector[iProng][iDet][iPartHypo],Form("nsig%s_%s_%d/I",detName[iDet].Data(),partHypoName[iPartHypo].Data(),iProng));
          else if(fPidOpt==kNsigmaPIDfloatandint) {
            fTreeVar->Branch(Form("nsig%s_%s_%d",detName[iDet].Data(),partHypoName[iPartHypo].Data(),iProng),&fPIDVarVector[iProng][iDet][iPartHypo],Form("nsig%s_%s_%d/F",detName[iDet].Data(),partHypoName[iPartHypo].Data(),iProng));
            fTreeVar->Branch(Form("int_nsig%s_%s_%d",detName[iDet].Data(),partHypoName[iPartHypo].Data(),iProng),&fPIDVarIntVector[iProng][iDet][iPartHypo],Form("int_nsig%s_%s_%d/I",detName[iDet].Data(),partHypoName[iPartHypo].Data(),iProng));
          }
        }
      }
    }
    else if(fPidOpt>=kNsigmaCombPID && fPidOpt<=kNsigmaCombPIDfloatandint) {
      for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
        if(!useHypo[iPartHypo]) continue;
        if(fPidOpt==kNsigmaCombPID) fTreeVar->Branch(Form("nsigComb_%s_%d",partHypoName[iPartHypo].Data(),iProng),&fPIDVarVector[iProng][0][iPartHypo],Form("nsigComb_%s_%d/F",partHypoName[iPartHypo].Data(),iProng));
        else if(fPidOpt==kNsigmaCombPIDint) fTreeVar->Branch(Form("nsigComb_%s_%d",partHypoName[iPartHypo].Data(),iProng),&fPIDVarIntVector[iProng][0][iPartHypo],Form("nsigComb_%s_%d/I",partHypoName[iPartHypo].Data(),iProng));
        else if(fPidOpt==kNsigmaCombPIDfloatandint) {
          fTreeVar->Branch(Form("nsigComb_%s_%d",partHypoName[iPartHypo].Data(),iProng),&fPIDVarVector[iProng][0][iPartHypo],Form("nsigComb_%s_%d/F",partHypoName[iPartHypo].Data(),iProng));
          fTreeVar->Branch(Form("int_nsigComb_%s_%d",partHypoName[iPartHypo].Data(),iProng),&fPIDVarIntVector[iProng][0][iPartHypo],Form("int_nsigComb_%s_%d/I",partHypoName[iPartHypo].Data(),iProng));
        }
      }
    }
    else if(fPidOpt==kRawPID) {
      for(int iDet=0; iDet<knMaxDet4Pid; iDet++) {
        if(!useDet[iDet]) continue;
        fTreeVar->Branch(Form("%s_%d",rawPidName[iDet].Data(),iProng),&fPIDVarVector[iProng][iDet][0],Form("%s_%d/F",detName[iDet].Data(),iProng));
      }
    }
  }
}

//________________________________________________________________
bool AliHFTreeHandler::SetPidVars(AliAODTrack* prongtracks[], AliAODPidHF* pidHF, bool usePionHypo, bool useKaonHypo, bool useProtonHypo, bool useTPC, bool useTOF) 
{
  if(!pidHF) return false;
  for(int iProng=0; iProng<fNProngs; iProng++) {
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
  for(int iProng=0; iProng<fNProngs; iProng++) {
    if(fPidOpt>=kNsigmaPID && fPidOpt<=kNsigmaCombPIDfloatandint) {
      for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
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
      rawPID[iProng][kTPC] = pidrespo->GetTPCsignalTunedOnData(prongtracks[iProng]);
      rawPID[iProng][kTOF] = pidrespo->GetTOFsignalTunedOnData(prongtracks[iProng]);
      float time0 = pidrespo->GetTOFResponse().GetStartTime(prongtracks[iProng]->P());
      rawPID[iProng][kTOF] -= time0;
    }
  }

  //fill PID arrays for different options
  switch(fPidOpt) {
    case 1: //kNsigmaPID
      for(int iProng=0; iProng<fNProngs; iProng++) {
        for(int iDet=kTPC; iDet<=kTOF; iDet++) {
          for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
            fPIDVarVector[iProng][iDet][iPartHypo] = sig[iProng][iDet][iPartHypo];
          }
        }
      }
    break;
    case 2: //kNsigmaPIDint
      for(int iProng=0; iProng<fNProngs; iProng++) {
        for(int iDet=kTPC; iDet<=kTOF; iDet++) {
          for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
            fPIDVarIntVector[iProng][iDet][iPartHypo] = RoundFloatToInt(sig[iProng][iDet][iPartHypo]*100);
          }
        }
      }
    break;
    case 3: //kNsigmaPIDfloatandint
      for(int iProng=0; iProng<fNProngs; iProng++) {
        for(int iDet=kTPC; iDet<=kTOF; iDet++) {
          for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
            fPIDVarVector[iProng][iDet][iPartHypo] = sig[iProng][iDet][iPartHypo]*100;  
            fPIDVarIntVector[iProng][iDet][iPartHypo] = RoundFloatToInt(fPIDVarVector[iProng][iDet][iPartHypo]*100);
          }
        }
      }
    break;
    case 4: //kNsigmaCombPID
      for(int iProng=0; iProng<fNProngs; iProng++) {
        for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
          fPIDVarVector[iProng][0][iPartHypo] = sigComb[iProng][iPartHypo];        
        }
      }
    break;
    case 5: //kNsigmaCombPIDint
      for(int iProng=0; iProng<fNProngs; iProng++) {
        for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
          fPIDVarIntVector[iProng][0][iPartHypo] = RoundFloatToInt(sigComb[iProng][iPartHypo]*100);
        }
      }
    break;
    case 6: //kNsigmaCombPIDfloatandint
      for(int iProng=0; iProng<fNProngs; iProng++) {
        for(int iPartHypo=0; iPartHypo<knMaxHypo4Pid; iPartHypo++) {
          fPIDVarVector[iProng][0][iPartHypo] = sigComb[iProng][iPartHypo]*100;     
          fPIDVarIntVector[iProng][0][iPartHypo] = RoundFloatToInt(fPIDVarVector[iProng][0][iPartHypo]*100);
        }
      }
    break;
    case 7: //kRawPID
      for(int iProng=0; iProng<fNProngs; iProng++) {
        for(int iDet=kTPC; iDet<=kTOF; iDet++) {
          fPIDVarVector[iProng][iDet][0] = rawPID[iProng][iDet];     
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
