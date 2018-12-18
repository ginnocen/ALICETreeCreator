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
#include "AliHFTreeHandlerDstartoKpipi.h"
#include "AliAODRecoCascadeHF.h"

/// \cond CLASSIMP
ClassImp(AliHFTreeHandlerDstartoKpipi);
/// \endcond

//________________________________________________________________
AliHFTreeHandlerDstartoKpipi::AliHFTreeHandlerDstartoKpipi():
  AliHFTreeHandler(),
  fCosThetaStar(),
  fImpParProd(),
  fSoftPiPt()
{
  //
  // Default constructor
  //

  fNProngs=2; // --> cannot be changed
}

//________________________________________________________________
AliHFTreeHandlerDstartoKpipi::AliHFTreeHandlerDstartoKpipi(int PIDopt):
  AliHFTreeHandler(PIDopt),
  fCosThetaStar(),
  fImpParProd(),
  fSoftPiPt()
{
  //
  // Standard constructor
  //

  fNProngs=2; // --> cannot be changed
}

//________________________________________________________________
AliHFTreeHandlerDstartoKpipi::~AliHFTreeHandlerDstartoKpipi()
{
  //
  // Default Destructor
  //
}

//________________________________________________________________
TTree* AliHFTreeHandlerDstartoKpipi::BuildTree(TString name, TString title) 
{
  fIsMCGenTree=false;

  if(fTreeVar) {
    delete fTreeVar;
    fTreeVar=0x0;
  }
  fTreeVar = new TTree(name.Data(),title.Data());

  //set common variables
  AddCommonDmesonVarBranches();

  //set Dstar variables
  fTreeVar->Branch("cos_t_star",&fCosThetaStar);
  fTreeVar->Branch("imp_par_prod",&fImpParProd);
  fTreeVar->Branch("pt_soft",&fSoftPiPt);  
  //set single-track variables
  AddSingleTrackBranches();

  //set PID variables
  if(fPidOpt!=kNoPID) AddPidBranches(true,true,false,true,true);

  return fTreeVar;
}

//________________________________________________________________
bool AliHFTreeHandlerDstartoKpipi::SetVariables(AliAODRecoDecayHF* cand, float bfield, int masshypo, AliAODPidHF* pidHF) 
{
  fIsMCGenTree=false;

  if(!cand) return false;
  if(fFillOnlySignal) { //if fill only signal and not signal candidate, do not store
    if(!(fCandTypeMap&kSignal)) return true;
  }
  fNCandidates++;

  AliAODRecoDecayHF2Prong *d0 = ((AliAODRecoCascadeHF*)cand)->Get2Prong();

  //topological variables
  //common
  fCandType.push_back(fCandTypeMap);
  fPt.push_back(((AliAODRecoCascadeHF*)cand)->Pt());
  fY.push_back(((AliAODRecoCascadeHF*)cand)->YDstar());
  fEta.push_back(cand->Eta());
  fPhi.push_back(cand->Phi());
  fDecayLength.push_back(d0->DecayLength());
  fDecayLengthXY.push_back(d0->DecayLengthXY());
  fNormDecayLengthXY.push_back(d0->NormalizedDecayLengthXY()*(d0->P()/d0->Pt()));
  fCosP.push_back(d0->CosPointingAngle());
  fCosPXY.push_back(d0->CosPointingAngleXY());
  fImpParXY.push_back(d0->ImpParXY());
  fNormd0MeasMinusExp.push_back(ComputeMaxd0MeasMinusExp(d0,bfield)); //not sure this is needed for the d*
  
  //D* -> D0 pi variables
  fImpParProd.push_back(d0->Prodd0d0());
  fInvMass.push_back(((AliAODRecoCascadeHF*)cand)->DeltaInvMass());
  if( (((AliAODRecoCascadeHF*)cand)->Charge()) >0)   fCosThetaStar.push_back(d0->CosThetaStarD0());
  else   fCosThetaStar.push_back(d0->CosThetaStarD0bar());
  AliAODTrack *softPion = (AliAODTrack*)((AliAODRecoCascadeHF*)cand)->GetBachelor();   
  fSoftPiPt.push_back(softPion->Pt());

  //single track variables
  AliAODTrack* prongtracks[2];

  for(unsigned int iProng=0; iProng<fNProngs; iProng++) prongtracks[iProng] = (AliAODTrack*)d0->GetDaughter(iProng);
  bool setsingletrack = SetSingleTrackVars(prongtracks,cand);  
  if(!setsingletrack) return false;

  //pid variables
  if(fPidOpt==kNoPID) return true;

  bool setpid = SetPidVars(prongtracks,pidHF,true,true,false,true,true);
  if(!setpid) return false;

  return true;
}

//________________________________________________________________
void AliHFTreeHandlerDstartoKpipi::FillTree() {
  fTreeVar->Fill();
  
  //VERY IMPORTANT: CLEAR ALL VECTORS
  if(!fIsMCGenTree) {
    ResetDmesonCommonVarVectors();
    fCosThetaStar.clear();
    fImpParProd.clear();
    fSoftPiPt.clear();
    ResetSingleTrackVarVectors();
    if(fPidOpt!=kNoPID) ResetPidVarVectors();
  } else {
    ResetMCGenVectors();
  }
  fCandTypeMap=0;
  fNCandidates=0;
}
