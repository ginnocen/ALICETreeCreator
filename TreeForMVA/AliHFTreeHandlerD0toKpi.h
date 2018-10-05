#ifndef ALIHFTREEHANDLERD0TOKPI_H
#define ALIHFTREEHANDLERD0TOKPI_H

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

class AliHFTreeHandlerD0toKpi : public AliHFTreeHandler
{
  public:
    AliHFTreeHandlerD0toKpi();
    AliHFTreeHandlerD0toKpi(int PIDopt);
    AliHFTreeHandlerD0toKpi(const AliHFTreeHandlerD0toKpi& source);
    AliHFTreeHandlerD0toKpi& operator=(const AliHFTreeHandlerD0toKpi& source);

    virtual ~AliHFTreeHandlerD0toKpi();

    virtual TTree* BuildTree(TString name="tree", TString title="tree");
    virtual bool SetVariables(AliAODRecoDecayHF* cand, int masshypo=0, AliAODPidHF* pidHF=0x0);

  private:

    static const int knD0toKpiVars = 4;

    float fD0VarVector[knD0toKpiVars]; /// array with variables 

    /// \cond CLASSIMP
    ClassDef(AliHFTreeHandlerD0toKpi,1); /// 
    /// \endcond
};
#endif