#ifndef ALIHFTREEHANDLERDSTOKKPI_H
#define ALIHFTREEHANDLERDSTOKKPI_H

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

#include "AliHFTreeHandler.h"

class AliHFTreeHandlerDstoKKpi : public AliHFTreeHandler
{
  public:

    enum massKKopt {kMassKK,kDeltaMassKKPhi};

    AliHFTreeHandlerDstoKKpi();
    AliHFTreeHandlerDstoKKpi(int PIDopt);
    AliHFTreeHandlerDstoKKpi(const AliHFTreeHandlerDstoKKpi& source);
    AliHFTreeHandlerDstoKKpi& operator=(const AliHFTreeHandlerDstoKKpi& source);

    virtual ~AliHFTreeHandlerDstoKKpi();

    virtual TTree* BuildTree(TString name="tree", TString title="tree");
    virtual bool SetVariables(AliAODRecoDecayHF* cand, int masshypo=0, AliAODPidHF* pidHF=0x0);

    void SetMassKKOption(int opt) {fMassKKOpt=opt;}
  
  private:

    static const int knDstoKKpiVars = 5;

    float fDsVarVector[knDstoKKpiVars]; /// array with variables 
    int fMassKKOpt; /// option for massKK variable (mass or delta mass wrt phi)

    /// \cond CLASSIMP
    ClassDef(AliHFTreeHandlerDstoKKpi,1); /// 
    /// \endcond
};
#endif
