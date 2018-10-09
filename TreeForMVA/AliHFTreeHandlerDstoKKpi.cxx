/* Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//*************************************************************************
// \class AliHFTreeHandlerDstoKKpi
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
#include <TDatabasePDG.h>
#include "AliHFTreeHandlerDstoKKpi.h"
#include "AliAODRecoDecayHF3Prong.h"

/// \cond CLASSIMP
ClassImp(AliHFTreeHandlerDstoKKpi);
/// \endcond

//________________________________________________________________
AliHFTreeHandlerDstoKKpi::AliHFTreeHandlerDstoKKpi():
  AliHFTreeHandler(),
  fMassKKOpt(kDeltaMassKKPhi)
{
  //
  // Default constructor
  //

  fNProngs=3; // --> cannot be changed
  for(int iVar=0; iVar<knDstoKKpiVars; iVar++) fDsVarVector[iVar] = -999.;
}

//________________________________________________________________
AliHFTreeHandlerDstoKKpi::AliHFTreeHandlerDstoKKpi(int PIDopt):
  AliHFTreeHandler(PIDopt),
  fMassKKOpt(kDeltaMassKKPhi)
{
  //
  // Standard constructor
  //

  fNProngs=3; // --> cannot be changed
  for(int iVar=0; iVar<knDstoKKpiVars; iVar++) fDsVarVector[iVar] = -999.;
}

//________________________________________________________________
AliHFTreeHandlerDstoKKpi::AliHFTreeHandlerDstoKKpi(const AliHFTreeHandlerDstoKKpi& source):
  AliHFTreeHandler(source),
  fMassKKOpt(source.fMassKKOpt)
{
  //
  // Copy constructor
  //

  for(int iVar=0; iVar<knDstoKKpiVars; iVar++) fDsVarVector[iVar] = source.fDsVarVector[iVar];
}

//________________________________________________________________
AliHFTreeHandlerDstoKKpi &AliHFTreeHandlerDstoKKpi::operator=(const AliHFTreeHandlerDstoKKpi& source)
{
  //
  // assignment operator
  //

  if(&source == this) return *this;
  AliHFTreeHandler::operator=(source); 
  fMassKKOpt = source.fMassKKOpt;

  for(int iVar=0; iVar<knDstoKKpiVars; iVar++) fDsVarVector[iVar] = source.fDsVarVector[iVar];

  return *this;
}

//________________________________________________________________
AliHFTreeHandlerDstoKKpi::~AliHFTreeHandlerDstoKKpi()
{
  //
  // Default Destructor
  //
}

//________________________________________________________________
TTree* AliHFTreeHandlerDstoKKpi::BuildTree(TString name, TString title) 
{
  if(fTreeVar) {
    delete fTreeVar;
    fTreeVar=0x0;
  }
  fTreeVar = new TTree(name.Data(),title.Data());

  //set common variables
  AddCommonDmesonVarBranches();

  //set Ds variables
  TString varnames[knDstoKKpiVars] = {"pt_prong2","sig_vert","","cos_PiDs","cos_PiKPhi_3"};
  if(fMassKKOpt==kMassKK) varnames[2] = "mass_KK";
  else if(fMassKKOpt==kDeltaMassKKPhi) varnames[2] = "delta_mass_KK";
  for(int iVar=0; iVar<knDstoKKpiVars; iVar++) {
    fTreeVar->Branch(varnames[iVar].Data(),&fDsVarVector[iVar],Form("%s/F",varnames[iVar].Data()));
  }

  if(fEnableCentrality) fTreeVar->Branch("centrality",&fCentrality,"centrality/B");
  if(fEnableNormd0MeasMinusExp) fTreeVar->Branch("normd0d0exp",&fNormd0MeasMinusExp,"normd0d0exp/F");

  //sed pid variables
  if(fPidOpt!=kNoPID) AddPidBranches(true,true,false,true,true);

  return fTreeVar;
}

//________________________________________________________________
bool AliHFTreeHandlerDstoKKpi::SetVariables(AliAODRecoDecayHF* cand, int masshypo, AliAODPidHF* pidHF) 
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
  //Ds+ -> KKpi variables
  fDsVarVector[0]=((AliAODRecoDecayHF3Prong*)cand)->PtProng(2);
  fDsVarVector[1]=((AliAODRecoDecayHF3Prong*)cand)->GetSigmaVert();
  float massPhi = 0;
  if(fMassKKOpt==kDeltaMassKKPhi) massPhi = TDatabasePDG::Instance()->GetParticle(333)->Mass();
  if(masshypo==0){ //phiKKpi
    fCommonVarVector[0]=((AliAODRecoDecayHF3Prong*)cand)->InvMassDsKKpi();
    fDsVarVector[2]=TMath::Abs(((AliAODRecoDecayHF3Prong*)cand)->InvMass2Prongs(0,1,321,321)-massPhi);
    fDsVarVector[3]=((AliAODRecoDecayHF3Prong*)cand)->CosPiDsLabFrameKKpi();
    fDsVarVector[4]=((AliAODRecoDecayHF3Prong*)cand)->CosPiKPhiRFrameKKpi();
  }
  else if(masshypo==1){ //phipiKK
    fCommonVarVector[0]=((AliAODRecoDecayHF3Prong*)cand)->InvMassDspiKK();
    fDsVarVector[2]=TMath::Abs(((AliAODRecoDecayHF3Prong*)cand)->InvMass2Prongs(1,2,321,321)-massPhi);
    fDsVarVector[3]=((AliAODRecoDecayHF3Prong*)cand)->CosPiDsLabFramepiKK();
    fDsVarVector[4]=((AliAODRecoDecayHF3Prong*)cand)->CosPiKPhiRFramepiKK();
  }
  fDsVarVector[4] = TMath::Abs(fDsVarVector[4]*fDsVarVector[4]*fDsVarVector[4]);

  //pid variables
  if(fPidOpt==kNoPID) return true;

  AliAODTrack* prongtracks[3];
  for(int iProng=0; iProng<fNProngs; iProng++) prongtracks[iProng] = (AliAODTrack*)cand->GetDaughter(iProng);
  bool setpid = SetPidVars(prongtracks,pidHF,true,true,false,true,true);
  if(!setpid) return false;

  return true;
}