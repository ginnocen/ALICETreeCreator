/* Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//*************************************************************************
// \class AliHFTreeHandlerD0toKpi
// \brief helper class to handle a tree for D+ cut optimisation and MVA analyses
// \authors:
// F. Catalano, fabio.catalano@cern.ch
// A. Festanti, andrea.festanti@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
// G. Innocenti, gian.michele.innocenti@cern.ch
// F. Prino, prino@to.infn.it
// L. Vermunt, luuk.vermunt@cern.ch
/////////////////////////////////////////////////////////////

#include <TString.h>
#include "AliHFTreeHandlerD0toKpi.h"
#include "AliAODRecoDecayHF2Prong.h"

/// \cond CLASSIMP
ClassImp(AliHFTreeHandlerD0toKpi);
/// \endcond

//________________________________________________________________
AliHFTreeHandlerD0toKpi::AliHFTreeHandlerD0toKpi():
  AliHFTreeHandler()
{
  //
  // Default constructor
  //

  fNProngs=2; // --> cannot be changed
  for(int iVar=0; iVar<knD0toKpiVars; iVar++) fD0VarVector[iVar] = -999.;
}

//________________________________________________________________
AliHFTreeHandlerD0toKpi::AliHFTreeHandlerD0toKpi(int PIDopt):
  AliHFTreeHandler(PIDopt)
{
  //
  // Standard constructor
  //

  fNProngs=2; // --> cannot be changed
  for(int iVar=0; iVar<knD0toKpiVars; iVar++) fD0VarVector[iVar] = -999.;
}

//________________________________________________________________
AliHFTreeHandlerD0toKpi::AliHFTreeHandlerD0toKpi(const AliHFTreeHandlerD0toKpi& source):
  AliHFTreeHandler(source)
{
  //
  // Copy constructor
  //

  for(int iVar=0; iVar<knD0toKpiVars; iVar++) fD0VarVector[iVar] = source.fD0VarVector[iVar];
}

//________________________________________________________________
AliHFTreeHandlerD0toKpi &AliHFTreeHandlerD0toKpi::operator=(const AliHFTreeHandlerD0toKpi& source)
{
  //
  // assignment operator
  //

  if(&source == this) return *this;
  AliHFTreeHandler::operator=(source); 

  for(int iVar=0; iVar<knD0toKpiVars; iVar++) fD0VarVector[iVar] = source.fD0VarVector[iVar];

  return *this;
}

//________________________________________________________________
AliHFTreeHandlerD0toKpi::~AliHFTreeHandlerD0toKpi()
{
  //
  // Default Destructor
  //
}

//________________________________________________________________
TTree* AliHFTreeHandlerD0toKpi::BuildTree(TString name, TString title) 
{
  if(fTreeVar) {
    delete fTreeVar;
    fTreeVar=0x0;
  }
  fTreeVar = new TTree(name.Data(),title.Data());

  //set common variables
  AddCommonDmesonVarBranches();

  //set D0 variables
  TString varnames[knD0toKpiVars] = {"cos_t_star", "imp_par_prong0", "imp_par_prong1","imp_par_prod"};
  for(int iVar=0; iVar<knD0toKpiVars; iVar++) {
    fTreeVar->Branch(varnames[iVar].Data(),&fD0VarVector[iVar],Form("%s/F",varnames[iVar].Data()));
  }

  if(fEnableCentrality) fTreeVar->Branch("centrality",&fCentrality,"centrality/B");
  if(fEnableNormd0MeasMinusExp) fTreeVar->Branch("normd0d0exp",&fNormd0MeasMinusExp,"normd0d0exp/F");
  
  //set PID variables
  if(fPidOpt!=kNoPID) AddPidBranches(true,true,false,true,true);

  return fTreeVar;
}

//________________________________________________________________
bool AliHFTreeHandlerD0toKpi::SetVariables(AliAODRecoDecayHF* cand, int masshypo, AliAODPidHF* pidHF) 
{
  if(!cand) return false;

  //topological variables
  //common
  fCommonVarVector[1]=cand->Pt();
  fCommonVarVector[2]=cand->DecayLength();
  fCommonVarVector[3]=cand->DecayLengthXY();
  fCommonVarVector[4]=cand->NormalizedDecayLengthXY();
  fCommonVarVector[5]=cand->CosPointingAngle();
  fCommonVarVector[6]=cand->CosPointingAngleXY();
  fCommonVarVector[7]=cand->ImpParXY();
  fCommonVarVector[8]=cand->PtProng(0);
  fCommonVarVector[9]=cand->PtProng(1);
  //D0 -> Kpi variables
  fD0VarVector[1]=((AliAODRecoDecayHF2Prong*)cand)->Getd0Prong(0);
  fD0VarVector[2]=((AliAODRecoDecayHF2Prong*)cand)->Getd0Prong(1);
  fD0VarVector[3]=fD0VarVector[1]*fD0VarVector[2];
    if(masshypo==0) { //D0 -> Kpi
      fCommonVarVector[0]=((AliAODRecoDecayHF2Prong*)cand)->InvMassD0();
      fD0VarVector[0]=((AliAODRecoDecayHF2Prong*)cand)->CosThetaStarD0();
    }
    else if(masshypo==1) { //D0 -> piK
      fCommonVarVector[0]=((AliAODRecoDecayHF2Prong*)cand)->InvMassD0bar();
      fD0VarVector[0]=((AliAODRecoDecayHF2Prong*)cand)->CosThetaStarD0bar();
    }

  //pid variables
  if(fPidOpt==kNoPID) return true;

  AliAODTrack* prongtracks[2];
  for(int iProng=0; iProng<fNProngs; iProng++) prongtracks[iProng] = (AliAODTrack*)cand->GetDaughter(iProng);
  bool setpid = SetPidVars(prongtracks,pidHF,true,true,false,true,true);
  if(!setpid) return false;

  return true;
}