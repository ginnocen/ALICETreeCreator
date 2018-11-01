/* Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//*************************************************************************
// \class AliHFTreeHandlerBplustoD0pi
// \brief helper class to handle a tree for B+ cut optimisation and MVA analyses
// \authors:
// F. Catalano, fabio.catalano@cern.ch
// A. Festanti, andrea.festanti@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
// G. Innocenti, gian.michele.innocenti@cern.ch
// F. Prino, prino@to.infn.it
// L. Vermunt, luuk.vermunt@cern.ch
// L.V.R. van Doremalen, lennart.van.doremalen@cern.ch
/////////////////////////////////////////////////////////////

#include <TString.h>
#include "AliHFTreeHandlerBplustoD0pi.h"
#include "AliAODRecoDecayHF2Prong.h"
#include "AliAODRecoDecayHF.h"
#include <bitset>

/// \cond CLASSIMP
ClassImp(AliHFTreeHandlerBplustoD0pi);
/// \endcond

//________________________________________________________________
AliHFTreeHandlerBplustoD0pi::AliHFTreeHandlerBplustoD0pi():
  AliHFTreeHandler(),
  fCosThetaStar(),
  fImpParProd(),
  fDCA(),
  // fDecayTime(),
  // fPseudoProperDecayTime(),
  fAngleBoth(),
  fInvMassD0(),
  fPtD0(),
  fDecayLengthD0(),
  fDecayLengthXYD0(),
  fNormDecayLengthXYD0(),
  fCosPD0(),
  fCosPXYD0(),
  fImpParXYD0(),
  fCosThetaStarD0(),
  fImpParProdD0(),
  fDCAD0(),
  // fDecayTimeD0(),
  // fPseudoProperDecayTimeD0(),
  fAngleBothD0(),
  fImpParBplusPion(),
  fImpParD0Pion(),
  fImpParD0Kaon()
{
  //
  // Default constructor
  //

  fNProngs=3; // --> cannot be changed
}

//________________________________________________________________
AliHFTreeHandlerBplustoD0pi::AliHFTreeHandlerBplustoD0pi(int PIDopt):
  AliHFTreeHandler(PIDopt),
  fCosThetaStar(),
  fImpParProd(),
  fDCA(),
  // fDecayTime(),
  // fPseudoProperDecayTime(),
  fAngleBoth(),
  fInvMassD0(),
  fPtD0(),
  fDecayLengthD0(),
  fDecayLengthXYD0(),
  fNormDecayLengthXYD0(),
  fCosPD0(),
  fCosPXYD0(),
  fImpParXYD0(),
  fCosThetaStarD0(),
  fImpParProdD0(),
  fDCAD0(),
  // fDecayTimeD0(),
  // fPseudoProperDecayTimeD0(),
  fAngleBothD0(),
  fImpParBplusPion(),
  fImpParD0Pion(),
  fImpParD0Kaon()
{
  //
  // Standard constructor
  //

  fNProngs=3; // --> cannot be changed
}

//________________________________________________________________
AliHFTreeHandlerBplustoD0pi::~AliHFTreeHandlerBplustoD0pi()
{
  //
  // Default Destructor
  //
}

//________________________________________________________________
TTree* AliHFTreeHandlerBplustoD0pi::BuildTree(TString name, TString title) 
{
  fIsMCGenTree=false;
  
  if(fTreeVar) {
    delete fTreeVar;
    fTreeVar=0x0;
  }
  fTreeVar = new TTree(name.Data(),title.Data());

  //set common variables
  AddCommonDmesonVarBranches();

  //set D0 variables
  fTreeVar->Branch("cos_t_star",&fCosThetaStar);
  fTreeVar->Branch("imp_par_prod",&fImpParProd);
  fTreeVar->Branch("dca",&fDCA);
  // fTreeVar->Branch("decay_time",&fDecayTime);
  // fTreeVar->Branch("pseudo_proper_decay_time",&fPseudoProperDecayTime);
  fTreeVar->Branch("angle_between_both_daughters",&fAngleBoth);

  fTreeVar->Branch("inv_mass_D0",&fInvMassD0);
  fTreeVar->Branch("pt_cand_D0",&fPtD0);
  fTreeVar->Branch("d_len_D0",&fDecayLengthD0);
  fTreeVar->Branch("d_len_xy_D0",&fDecayLengthXYD0);
  fTreeVar->Branch("norm_dl_xy_D0",&fNormDecayLengthXYD0);
  fTreeVar->Branch("cos_p_D0",&fCosPD0);
  fTreeVar->Branch("cos_p_xy_D0",&fCosPXYD0);
  fTreeVar->Branch("imp_par_xy_D0",&fImpParXYD0);
  fTreeVar->Branch("cos_t_star_D0",&fCosThetaStarD0);
  fTreeVar->Branch("imp_par_prod_D0",&fImpParProdD0);
  fTreeVar->Branch("dca_D0",&fDCAD0);
  // fTreeVar->Branch("decay_time_D0",&fDecayTimeD0);
  // fTreeVar->Branch("pseudo_proper_decay_time_D0",&fPseudoProperDecayTimeD0);
  fTreeVar->Branch("angle_between_both_daughters_D0",&fAngleBothD0);

  fTreeVar->Branch("imp_par_Bplus_pion",&fImpParBplusPion);
  fTreeVar->Branch("imp_par_D0_pion",&fImpParD0Pion);
  fTreeVar->Branch("imp_par_D0_kaon",&fImpParD0Kaon);

  //set single-track variables
  AddSingleTrackBranches();

  //set PID variables
  if(fPidOpt!=kNoPID) AddPidBranches(true,true,false,true,true);

  return fTreeVar;
}

//________________________________________________________________
bool AliHFTreeHandlerBplustoD0pi::SetVariables(AliAODRecoDecayHF* cand, float bfield, int masshypo, AliAODPidHF* pidHF) 
{
  fIsMCGenTree=false;

  if(!cand) return false;
  if(fFillOnlySignal) { //if fill only signal and not signal candidate, do not store
    if(!(fCandTypeMap&kSignal || fCandTypeMap&kRefl)) return true;
  }
  fNCandidates++;

  cout << bitset<32>(fCandTypeMap) << endl;

  AliAODTrack* candidateBplusPion = (AliAODTrack*)cand->GetDaughter(0);  
  AliAODRecoDecayHF2Prong* candidateD0 = (AliAODRecoDecayHF2Prong*)cand->GetDaughter(1);  
  AliAODTrack *candidateFirstDaughter = (AliAODTrack*)candidateD0->GetDaughter(0);
  AliAODTrack *candidateSecondDaughter = (AliAODTrack*)candidateD0->GetDaughter(1);
  // AliAODVertex * vertexBPlus = candidateBPlus->GetSecondaryVtx();
  // AliAODVertex * vertexD0 = candidateD0->GetSecondaryVtx();
  // AliAODVertex * primaryVertex = aod->GetPrimaryVertex();


  Double_t angleBetweenBothDaughters  = (candidateD0->Px() * candidateBplusPion->Px() + candidateD0->Py() * candidateBplusPion->Py() + candidateD0->Pz() * candidateBplusPion->Pz()) /(candidateD0->P() * candidateBplusPion->P());
  Double_t angleBetweenBothDaughtersD0  = (candidateSecondDaughter->Px() * candidateFirstDaughter->Px() + candidateSecondDaughter->Py() * candidateFirstDaughter->Py() + candidateSecondDaughter->Pz() * candidateFirstDaughter->Pz()) /(candidateSecondDaughter->P() * candidateFirstDaughter->P());

  // Double_t pseudoProperDecayLengthD0 = ((vertexD0->GetX() - primaryVertex->GetX()) * candidateD0->Px() / TMath::Abs(candidateD0->Pt())) + ((vertexD0->GetY() - primaryVertex->GetY()) * candidateD0->Py() / TMath::Abs(candidateD0->Pt()));
  // Double_t pseudoProperDecayTimeD0 = pseudoProperDecayLengthD0 * pdgMassMother/ptMother;
  // Double_t decayTimeD0 = vertexDistanceD0 / (299792458 * TMath::Sqrt(1/((pdgMassMother*pdgMassMother/(momentumMother*momentumMother)) + 1)));

  //topological variables
  //common
  fCandType.push_back(fCandTypeMap);
  fPt.push_back(((AliAODRecoDecayHF2Prong*)cand)->Pt());
  fY.push_back(((AliAODRecoDecayHF2Prong*)cand)->Y(521));
  fEta.push_back(((AliAODRecoDecayHF2Prong*)cand)->Eta());
  fPhi.push_back(((AliAODRecoDecayHF2Prong*)cand)->Phi());
  fDecayLength.push_back(((AliAODRecoDecayHF2Prong*)cand)->DecayLength());
  fDecayLengthXY.push_back(((AliAODRecoDecayHF2Prong*)cand)->DecayLengthXY());
  fNormDecayLengthXY.push_back(((AliAODRecoDecayHF2Prong*)cand)->NormalizedDecayLengthXY());
  fCosP.push_back(((AliAODRecoDecayHF2Prong*)cand)->CosPointingAngle());
  fCosPXY.push_back(((AliAODRecoDecayHF2Prong*)cand)->CosPointingAngleXY());
  fImpParXY.push_back(((AliAODRecoDecayHF2Prong*)cand)->ImpParXY());

  fNormd0MeasMinusExp.push_back(0.0);
  
  UInt_t prongs[2];
  prongs[0] = 211; prongs[1] = 421;
  fInvMass.push_back(((AliAODRecoDecayHF2Prong*)cand)->InvMass(2,prongs));
  fCosThetaStar.push_back(((AliAODRecoDecayHF2Prong*)cand)->CosThetaStar(0,521,211,421));
  fImpParProd.push_back(((AliAODRecoDecayHF2Prong*)cand)->Prodd0d0());

  fDCA.push_back(((AliAODRecoDecayHF2Prong*)cand)->GetDCA());
  // fDecayTime.push_back();
  // fPseudoProperDecayTime.push_back();
  fAngleBoth.push_back(angleBetweenBothDaughters);
  fImpParBplusPion.push_back(((AliAODRecoDecayHF2Prong*)cand)->Getd0Prong(0));

  //single track variables
  AliAODTrack* prongtracks[3];

  prongtracks[0] = (AliAODTrack*)cand->GetDaughter(0);

  if(((AliAODRecoDecayHF2Prong*)cand)->Charge()==-1) {
    fInvMassD0.push_back(candidateD0->InvMassD0());
    fPtD0.push_back(candidateD0->Pt());
    fDecayLengthD0.push_back(candidateD0->DecayLength());
    fDecayLengthXYD0.push_back(candidateD0->DecayLengthXY());
    fNormDecayLengthXYD0.push_back(candidateD0->NormalizedDecayLengthXY());
    fCosPD0.push_back(candidateD0->CosPointingAngle());
    fCosPXYD0.push_back(candidateD0->CosPointingAngleXY());
    fImpParXYD0.push_back(candidateD0->ImpParXY());
    fCosThetaStarD0.push_back(candidateD0->CosThetaStarD0());
    fImpParProdD0.push_back(candidateD0->Prodd0d0());
    fDCAD0.push_back(candidateD0->GetDCA());
    // fDecayTimeD0.push_back();
    // fPseudoProperDecayTimeD0.push_back();
    fAngleBothD0.push_back(angleBetweenBothDaughtersD0);
    fImpParD0Pion.push_back(candidateD0->Getd0Prong(0));
    fImpParD0Kaon.push_back(candidateD0->Getd0Prong(1));

    prongtracks[1] = (AliAODTrack*)candidateD0->GetDaughter(0);
    prongtracks[2] = (AliAODTrack*)candidateD0->GetDaughter(1);
  }
  else {
    fInvMassD0.push_back(candidateD0->InvMassD0bar());
    fPtD0.push_back(candidateD0->Pt());
    fDecayLengthD0.push_back(candidateD0->DecayLength());
    fDecayLengthXYD0.push_back(candidateD0->DecayLengthXY());
    fNormDecayLengthXYD0.push_back(candidateD0->NormalizedDecayLengthXY());
    fCosPD0.push_back(candidateD0->CosPointingAngle());
    fCosPXYD0.push_back(candidateD0->CosPointingAngleXY());
    fImpParXYD0.push_back(candidateD0->ImpParXY());
    fCosThetaStarD0.push_back(candidateD0->CosThetaStarD0bar());
    fImpParProdD0.push_back(candidateD0->Prodd0d0());
    fDCAD0.push_back(candidateD0->GetDCA());
    // fDecayTimeD0.push_back();
    // fPseudoProperDecayTimeD0.push_back();
    fAngleBothD0.push_back(angleBetweenBothDaughtersD0);
    fImpParD0Pion.push_back(candidateD0->Getd0Prong(1));
    fImpParD0Kaon.push_back(candidateD0->Getd0Prong(0));

    prongtracks[1] = (AliAODTrack*)candidateD0->GetDaughter(1);
    prongtracks[2] = (AliAODTrack*)candidateD0->GetDaughter(0);
  }


  bool setsingletrack = SetSingleTrackVars(prongtracks,cand);  
  if(!setsingletrack) return false;

  //pid variables
  if(fPidOpt==kNoPID) return true;

  bool setpid = SetPidVars(prongtracks,pidHF,true,true,false,true,true);
  if(!setpid) return false;

  return true;
}

//________________________________________________________________
void AliHFTreeHandlerBplustoD0pi::FillTree() {
  fTreeVar->Fill();
  
  //VERY IMPORTANT: CLEAR ALL VECTORS
  ResetDmesonCommonVarVectors();
  if(!fIsMCGenTree) {
    fCosThetaStar.clear();
    fImpParProd.clear();
    fDCA.clear();
    // fDecayTime.clear();
    // fPseudoProperDecayTime.clear();
    fAngleBoth.clear();
    fInvMassD0.clear();
    fPtD0.clear();
    fDecayLengthD0.clear();
    fDecayLengthXYD0.clear();
    fNormDecayLengthXYD0.clear();
    fCosPD0.clear();
    fCosPXYD0.clear();
    fImpParXYD0.clear();
    fCosThetaStarD0.clear();
    fImpParProdD0.clear();
    fDCAD0.clear();
    // fDecayTimeD0.clear();
    // fPseudoProperDecayTimeD0.clear();
    fAngleBothD0.clear();
    fImpParBplusPion.clear();
    fImpParD0Pion.clear();
    fImpParD0Kaon.clear();
    ResetSingleTrackVarVectors();
    if(fPidOpt!=kNoPID) ResetPidVarVectors();
  }
  fCandTypeMap=0;
  fNCandidates=0;
}