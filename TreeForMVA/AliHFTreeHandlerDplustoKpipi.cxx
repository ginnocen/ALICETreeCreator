/* Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//*************************************************************************
// \class AliHFTreeHandlerDplustoKpipi
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
#include "AliHFTreeHandlerDplustoKpipi.h"
#include "AliAODRecoDecayHF3Prong.h"

/// \cond CLASSIMP
ClassImp(AliHFTreeHandlerDplustoKpipi);
/// \endcond

//________________________________________________________________
AliHFTreeHandlerDplustoKpipi::AliHFTreeHandlerDplustoKpipi():
  AliHFTreeHandler()
{
  //
  // Default constructor
  //

  fNProngs=3; // --> cannot be changed
  for(int iVar=0; iVar<knDplustoKpipiVars; iVar++) fDplusVarVector[iVar] = -999.;
}

//________________________________________________________________
AliHFTreeHandlerDplustoKpipi::AliHFTreeHandlerDplustoKpipi(int PIDopt):
  AliHFTreeHandler(PIDopt)
{
  //
  // Standard constructor
  //

  fNProngs=3; // --> cannot be changed
  for(int iVar=0; iVar<knDplustoKpipiVars; iVar++) fDplusVarVector[iVar] = -999.;
}

//________________________________________________________________
AliHFTreeHandlerDplustoKpipi::AliHFTreeHandlerDplustoKpipi(const AliHFTreeHandlerDplustoKpipi& source):
  AliHFTreeHandler(source)
{
  //
  // Copy constructor
  //

  for(int iVar=0; iVar<knDplustoKpipiVars; iVar++) fDplusVarVector[iVar] = source.fDplusVarVector[iVar];
}

//________________________________________________________________
AliHFTreeHandlerDplustoKpipi &AliHFTreeHandlerDplustoKpipi::operator=(const AliHFTreeHandlerDplustoKpipi& source)
{
  //
  // assignment operator
  //

  if(&source == this) return *this;
  AliHFTreeHandler::operator=(source); 

  for(int iVar=0; iVar<knDplustoKpipiVars; iVar++) fDplusVarVector[iVar] = source.fDplusVarVector[iVar];

  return *this;
}

//________________________________________________________________
AliHFTreeHandlerDplustoKpipi::~AliHFTreeHandlerDplustoKpipi()
{
  //
  // Default Destructor
  //
}

//________________________________________________________________
TTree* AliHFTreeHandlerDplustoKpipi::BuildTree(TString name, TString title) 
{
  if(fTreeVar) {
    delete fTreeVar;
    fTreeVar=0x0;
  }
  fTreeVar = new TTree(name.Data(),title.Data());

  //set common variables
  AddCommonDmesonVarBranches();

  //Set D+ variables
  TString varnames[knDplustoKpipiVars] = {"pt_prong2","sig_vert"};
  for(int iVar=0; iVar<knDplustoKpipiVars; iVar++) {
    fTreeVar->Branch(varnames[iVar].Data(),&fDplusVarVector[iVar],Form("%s/F",varnames[iVar].Data()));
  }

  if(fEnableCentrality) fTreeVar->Branch("centrality",&fCentrality,"centrality/B");
  if(fEnableNormd0MeasMinusExp) fTreeVar->Branch("normd0d0exp",&fNormd0MeasMinusExp,"normd0d0exp/F");

  //Set PID variables
  if(fPidOpt!=kNoPID) AddPidBranches(true,true,false,true,true);

  return fTreeVar;
}

//________________________________________________________________
bool AliHFTreeHandlerDplustoKpipi::SetVariables(AliAODRecoDecayHF* cand, int masshypo, AliAODPidHF* pidHF) 
{
  if(!cand) return false;
  fCandType &= ~kRefl; //protection --> D+ ->Kpipi cannot be reflected

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
  //D+ -> Kpipi variables
  fCommonVarVector[0]=((AliAODRecoDecayHF3Prong*)cand)->InvMassDplus();
  fDplusVarVector[0]=((AliAODRecoDecayHF3Prong*)cand)->PtProng(2);
  fDplusVarVector[1]=((AliAODRecoDecayHF3Prong*)cand)->GetSigmaVert();

  //pid variables
  if(fPidOpt==kNoPID) return true;

  AliAODTrack* prongtracks[3];
  for(int iProng=0; iProng<fNProngs; iProng++) prongtracks[iProng] = (AliAODTrack*)cand->GetDaughter(iProng);
  bool setpid = SetPidVars(prongtracks,pidHF,true,true,false,true,true);
  if(!setpid) return false;

  return true;
}