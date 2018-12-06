#ifndef ALIHFTREEHANDLERD0TOKPI_DEV_H
#define ALIHFTREEHANDLERD0TOKPI_DEV_H

/* Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//*************************************************************************
// \class AliHFTreeHandlerD0toKpi_dev
// \brief helper class to handle a tree for D+ cut optimisation and MVA analyses
// \authors:
// F. Catalano, fabio.catalano@cern.ch
// A. Festanti, andrea.festanti@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
// G. Innocenti, gian.michele.innocenti@cern.ch
// F. Prino, prino@to.infn.it
// L. Vermunt, luuk.vermunt@cern.ch
// L. van Doremalen, lennart.van.doremalen@cern.ch
// J. Norman, jaime.norman@cern.ch
/////////////////////////////////////////////////////////////

#include "AliHFTreeHandler_dev.h"

using std::vector;

class AliHFTreeHandlerD0toKpi_dev : public AliHFTreeHandler_dev
{
  public:
    AliHFTreeHandlerD0toKpi_dev();
    AliHFTreeHandlerD0toKpi_dev(int PIDopt);

    virtual ~AliHFTreeHandlerD0toKpi_dev();

    virtual TTree* BuildTree(TString name="tree", TString title="tree");
    virtual bool SetVariables(AliAODRecoDecayHF* cand, float bfiled, int masshypo=0, AliAODPidHF* pidHF=0x0);
    virtual void FillTree(); //to be called for each event, not each candidate!

  private:

    vector<float> fImpParProng[knMaxProngs]; ///vectors of prong impact parameter
    vector<float> fCosThetaStar; /// vector of candidate costhetastar
    vector<float> fImpParProd; /// vector of daughter impact-parameter product
    vector<float> fNormd0MeasMinusExp; ///vector of candidate topomatic variable

    /// \cond CLASSIMP
    ClassDef(AliHFTreeHandlerD0toKpi_dev,1); /// 
    /// \endcond
};
#endif
