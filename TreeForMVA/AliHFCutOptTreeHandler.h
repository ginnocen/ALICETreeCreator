#ifndef ALIHFCUTOPTTREEHANDLER_H
#define ALIHFCUTOPTTREEHANDLER_H

/* Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//*************************************************************************
// Class AliHFCutOptTreeHandler
// helper class to handle a tree for cut optimisation and MVA analyses
// Authors:
// F. Catalano, fabio.catalano@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
// G. Innocenti, gian.michele.innocenti@cern.ch
// F. Prino, prino@to.infn.it
/////////////////////////////////////////////////////////////

#include <TTree.h>
#include "AliAODRecoDecayHF.h"
#include "AliAODPidHF.h"

class AliHFCutOptTreeHandler : public TObject
{
  public:
    enum decaychannel {
      kD0toKpi,
      kDplustoKpipi,
      kDstoKKpi
    };

    enum candtype {
      kBkg,
      kPromptSig,
      kFDSig,
      kPromptRefl,
      kFDRefl
    };

    enum optpid {
      kNoPID,
      kNsigmaPID,
      kNsigmaPIDchar,
      kNsigmaPIDfloatandchar //--> to test
    };

    AliHFCutOptTreeHandler();
    AliHFCutOptTreeHandler(int decay, int PIDopt);
    virtual ~AliHFCutOptTreeHandler();

    void SetDecayChannel(int decay=kD0toKpi) {fDecayChannel=decay;}
    void SetOptPID(int PIDopt) {fPidOpt=PIDopt;}
    bool SetVariables(AliAODRecoDecayHF* d, AliAODPidHF* pidHF, int masshypo, bool issig=false, bool isprompt=false, bool isrefl=false);
    void SetCentrality(char centrality) {fCentrality=centrality;}
    void SetUseCentrality() {fUseCentrality=true;}

    TTree* BuildTree(TString name="tree", TString title="tree");
    bool FillTree() {
      if(!fTreeTopolVar) return false;
      fTreeTopolVar->Fill();
      return true;
    }

  private:
    TTree* fTreeTopolVar; /// tree for cut optimisation
    int fDecayChannel; /// decay channel
    float fTopolVarVector[15]; /// vector with topological variables 
    float fPIDnSigmaVector[6]; /// vector with nSigma PID
    float fPIDnSigmaCharVector[6]; /// vector with nSigma PID
    int fPidOpt; /// option for PID variables
    char fCandType; ///flag for candidate type (bkg, prompt signal, FD signal, prompt refl, FD refl)
    bool fUseCentrality; ///flag to enable centrality
    char fCentrality; ///centrality in case of p-Pb or Pb-Pb
  
  /// \cond CLASSIMP
  ClassDef(AliHFCutOptTreeHandler,1); /// 
  /// \endcond
};

#endif
