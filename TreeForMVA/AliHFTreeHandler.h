#ifndef ALIHFTREEHANDLER_H
#define ALIHFTREEHANDLER_H

/* Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//*************************************************************************
// \class AliHFTreeHandler
// \brief helper class to handle a tree for cut optimisation and MVA analyses
// \authors:
// F. Catalano, fabio.catalano@cern.ch
// A. Festanti, andrea.festanti@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
// G. Innocenti, gian.michele.innocenti@cern.ch
// F. Prino, prino@to.infn.it
// L. Vermunt, luuk.vermunt@cern.ch
/////////////////////////////////////////////////////////////

#include <TTree.h>
#include "AliAODTrack.h"
#include "AliAODPidHF.h"
#include "AliAODRecoDecayHF.h"

class AliHFTreeHandler : public TObject
{
  public:
  
    enum candtype {
      kSelected = BIT(0),
      kSignal   = BIT(1),
      kBkg      = BIT(2),
      kPrompt   = BIT(3),
      kFD       = BIT(4),
      kRefl     = BIT(5)
    };

    enum optpid {
      kNoPID,
      kNsigmaPID,
      kNsigmaPIDint,
      kNsigmaPIDfloatandint, //--> to test
      kNsigmaCombPID,
      kNsigmaCombPIDint,
      kNsigmaCombPIDfloatandint, //--> to test
      kRawPID
    };

    enum piddet {
      kTPC,
      kTOF
    };

    AliHFTreeHandler();
    AliHFTreeHandler(int PIDopt);
    AliHFTreeHandler(const AliHFTreeHandler& source);
    AliHFTreeHandler& operator=(const AliHFTreeHandler& source);

    virtual ~AliHFTreeHandler();

    //core methods --> implemented in each derived class
    virtual TTree* BuildTree(TString name, TString title) = 0;
    virtual bool SetVariables(AliAODRecoDecayHF* cand, int masshypo, AliAODPidHF* pidHF) = 0;
    
    //common methods
    bool FillTree() {
      if(!fTreeVar) return false;
      if(!fFillOnlySignal) fTreeVar->Fill();
      else {
        if(fCandType & kSignal) 
          fTreeVar->Fill();
      }
      fCandType=0; //reset candtype after filling tree 
      return true;
    }

    void SetOptPID(int PIDopt) {fPidOpt=PIDopt;}
    void EnableCentralityBranch(bool enablecent=true) {fEnableCentrality=enablecent;}
    void EnableMaxNormd0MeasMinusExpBranch(bool enablenormd0measminusexp=true) {fEnableNormd0MeasMinusExp=enablenormd0measminusexp;}
    void SetFillOnlySignal(bool fillopt=true) {fFillOnlySignal=fillopt;}

    void SetCentrality(int centrality) {fCentrality=centrality;}
    void SetMaxNormd0MeasMinusExp(float normd0measminuexp) {fNormd0MeasMinusExp=normd0measminuexp;}
    void SetCandidateType(bool issignal, bool isbkg, bool isprompt, bool isFD, bool isreflected);
    void SetIsSelectedStd(bool isselected) {
      if(isselected) fCandType |= kSelected;
      else fCandType &= ~kSelected;
    }

    static bool IsSelectedStd(int candtype) {
      if(candtype&1) return true;
      return false;
    }
    static bool IsSignal(int candtype) {
      if(candtype>>1&1) return true;
      return false;
    }
    static bool IsBkg(int candtype) {
      if(candtype>>2&1) return true;
      return false;
    }
    static bool IsPrompt(int candtype) {
      if(candtype>>3&1) return true;
      return false;
    }
    static bool IsFD(int candtype) {
      if(candtype>>4&1) return true;
      return false;
    }
    static bool IsRefl(int candtype) {
      if(candtype>>5&1) return true;
      return false;
    }

  protected:  
    //constant variables
    static const int knCommonDmesonVars  = 10;
    static const int knMaxProngs   = 3;
    static const int knMaxDet4Pid  = 2;
    static const int knMaxHypo4Pid = 3;

    const TString commonDvarnames[knCommonDmesonVars] = {"inv_mass","pt_cand","d_len","d_len_xy","norm_dl_xy","cos_p","cos_p_xy","imp_par_xy","pt_prong0","pt_prong1"};

    //helper methods for derived clases (to be used in BuildTree and SetVariables functions)
    void AddCommonDmesonVarBranches();
    void AddPidBranches(bool usePionHypo, bool useKaonHypo, bool useProtonHypo, bool useTPC, bool useTOF);
    bool SetPidVars(AliAODTrack* prongtracks[], AliAODPidHF* pidHF, bool usePionHypo, bool useKaonHypo, bool useProtonHypo, bool useTPC, bool useTOF);

    //utils methods
    double CombineNsigmaDiffDet(double nsigmaTPC, double nsigmaTOF);
    int RoundFloatToInt(double num);

    TTree* fTreeVar; /// tree with variables
    int fPdgCode; /// absolute value of pdg code of the particle of interest
    int fNProngs; /// number of prongs
    int fPdgCodeProngs[knMaxProngs]; ///absolute values of pdg codes of the prongs
    float fCommonVarVector[knCommonDmesonVars]; /// array with common variables 
    float fPIDVarVector[knMaxProngs][knMaxDet4Pid][knMaxHypo4Pid]; /// array with common PID variables
    int fPIDVarIntVector[knMaxProngs][knMaxDet4Pid][knMaxHypo4Pid]; /// array with common PID variables (int)
    int fPidOpt; /// option for PID variables
    int fCandType; ///flag for candidate type (bit map above)
    bool fEnableCentrality; ///flag to enable centrality branch
    int fCentrality; ///centrality in case of p-Pb or Pb-Pb
    bool fEnableNormd0MeasMinusExp; ///flag to enable max normalised single-track imp-par residual
    float fNormd0MeasMinusExp; /// topomatic variable
    bool fFillOnlySignal; ///flag to enable only signal filling

  /// \cond CLASSIMP
  ClassDef(AliHFTreeHandler,1); /// 
  /// \endcond
};

#endif
