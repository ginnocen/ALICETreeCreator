#ifndef ALIHFTREEHANDLERDSTARTOKPIPI_H
#define ALIHFTREEHANDLERDSTARTOKPIPI_H

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

#include "AliHFTreeHandler.h"

using std::vector;

class AliHFTreeHandlerDstartoKpipi : public AliHFTreeHandler
{
  public:
    AliHFTreeHandlerDstartoKpipi();
    AliHFTreeHandlerDstartoKpipi(int PIDopt);

    virtual ~AliHFTreeHandlerDstartoKpipi();

    virtual TTree* BuildTree(TString name="tree", TString title="tree");
    virtual bool SetVariables(AliAODRecoDecayHF* cand, float bfiled, int masshypo=0, AliAODPidHF* pidHF=0x0);
    virtual void FillTree(); //to be called for each event, not each candidate!

  private:

    vector<float> fCosThetaStar; /// 
    vector<float> fImpParProd; /// 
    vector<float> fSoftPiPt; ///
    /// \cond CLASSIMP
    ClassDef(AliHFTreeHandlerDstartoKpipi,1); /// 
    /// \endcond
};
#endif
