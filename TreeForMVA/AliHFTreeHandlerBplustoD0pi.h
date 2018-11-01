#ifndef ALIHFTREEHANDLERBplusTOD0PI_H
#define ALIHFTREEHANDLERBplusTOD0PI_H

/* Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//*************************************************************************
// \class AliHFTreeHandlerBplustoD0pi
// \brief helper class to handle a tree for D+ cut optimisation and MVA analyses
// \authors:
// F. Catalano, fabio.catalano@cern.ch
// A. Festanti, andrea.festanti@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
// G. Innocenti, gian.michele.innocenti@cern.ch
// F. Prino, prino@to.infn.it
// L. Vermunt, luuk.vermunt@cern.ch
// L.V.R. van Doremalen, lennart.van.doremalen@cern.ch
/////////////////////////////////////////////////////////////

#include "AliHFTreeHandler.h"
#include "AliAODRecoDecayHF.h"


using std::vector;

class AliHFTreeHandlerBplustoD0pi : public AliHFTreeHandler
{
  public:
    AliHFTreeHandlerBplustoD0pi();
    AliHFTreeHandlerBplustoD0pi(int PIDopt);

    virtual ~AliHFTreeHandlerBplustoD0pi();

    virtual TTree* BuildTree(TString name="tree", TString title="tree");
    virtual bool SetVariables(AliAODRecoDecayHF* cand, float bfiled, int masshypo=0, AliAODPidHF* pidHF=0x0);
    virtual void FillTree(); //to be called for each event, not each candidate!

  private:

    vector<float> fCosThetaStar;                 /// 
    vector<float> fImpParProd;                   /// 
    vector<float> fDCA;                          ///
    // vector<float> fDecayTime;                    ///
    // vector<float> fPseudoProperDecayTime;        ///
    vector<float> fAngleBoth;                    ///
    vector<float> fInvMassD0;                    ///
    vector<float> fPtD0;                         ///
    vector<float> fDecayLengthD0;                ///
    vector<float> fDecayLengthXYD0;              ///
    vector<float> fNormDecayLengthXYD0;          ///
    vector<float> fCosPD0;                       ///
    vector<float> fCosPXYD0;                     ///
    vector<float> fImpParXYD0;                   ///
    vector<float> fCosThetaStarD0;               ///
    vector<float> fImpParProdD0;                 ///
    vector<float> fDCAD0;                        ///
    // vector<float> fDecayTimeD0;                  ///
    // vector<float> fPseudoProperDecayTimeD0;      ///
    vector<float> fAngleBothD0;                  ///
    vector<float> fImpParBplusPion;              ///
    vector<float> fImpParD0Pion;                 ///
    vector<float> fImpParD0Kaon;                 ///
    /// \cond CLASSIMP
    ClassDef(AliHFTreeHandlerBplustoD0pi,1); /// 
    /// \endcond
};
#endif