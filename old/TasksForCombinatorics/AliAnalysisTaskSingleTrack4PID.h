#ifndef ALIANALYSISTASKSINGLETRACK4PID_H
#define ALIANALYSISTASKSINGLETRACK4PID_H

/* Copyright(c) 1998-2018, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id: $ */

///*************************************************************************
/// \class Class AliAnalysisTaskSingleTrack4PID
//////////////////////////////////////////////////////////////

class TTree;
class TH1F;
class AliAODTrack;

#include "AliAnalysisTaskSE.h"
#include "AliRDHFCuts.h"

class AliAnalysisTaskSingleTrack4PID : public AliAnalysisTaskSE
{
public:
  
  AliAnalysisTaskSingleTrack4PID();
  virtual ~AliAnalysisTaskSingleTrack4PID();
  
  virtual void UserCreateOutputObjects();
  virtual void Init(){};
  virtual void LocalInit() {Init();}
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *option);
  
  void SetUseTimeOfFlight(){fOptForTOF=0;}
  void SetUseTOFnSigma(){fOptForTOF=1;}

  void SetReadMC(Bool_t read){fReadMC=read;}
  void SetKeepNegIDtracks(Bool_t nid){fKeepNegID=nid;}//set it to kTRUE only if you know what you are doing
  void SetTrackCuts(AliESDtrackCuts* cuts){
    if(fTrCuts) delete fTrCuts;
    fTrCuts=new AliESDtrackCuts(*cuts);
  }
  void SetFilterMask(UInt_t mask=16){fFilterMask=mask;}
  
  void SetUsePhysicsSelection(Bool_t opt=kTRUE){
    fUsePhysSel=opt;
  }
  void SetTriggerMask(Int_t mask){
    fTriggerMask=mask;
  }
  void SetUsePileupCut(Bool_t opt=kTRUE){
    fUsePileupCut=kTRUE;
  }

  Bool_t IsTrackSelected(AliAODTrack* track);
  Double_t GetTimeOfFlight(AliAODTrack* track, AliPIDResponse* pidResp);

  enum EMesonSpecies {kDzero, kDplus, kDstar, kDs, kLc};
  
private:
  
  enum EVarsTree {kNumOfIntVar=3, kNumOfFloatVar=15};

  AliAnalysisTaskSingleTrack4PID(const AliAnalysisTaskSingleTrack4PID &source);
  AliAnalysisTaskSingleTrack4PID& operator=(const AliAnalysisTaskSingleTrack4PID& source);
  void FillTree(Int_t pdgD,Int_t nProngs, AliAODRecoDecay* tmpRD, Double_t* px, Double_t* py, Double_t* pz, UInt_t *pdgdau, TClonesArray *arrayMC, Int_t* dgLabels);

  TList   *fOutput;           //!<! list send on output slot 0
  TH1F* fHistNEvents;         //!<! histo with number of events
  TTree*   fTrackTree;        //!<! output tree
  Float_t* fTreeVarFloat;     //!<! variables to be written to the tree
  Int_t*   fTreeVarInt;       //!<! variables to be written to the tree
  UInt_t fFilterMask;         /// FilterMask
  AliESDtrackCuts* fTrCuts;   /// track selection
  Int_t  fOptForTOF;          /// TOF info: 0=tof, 1=nsigma
  Bool_t fReadMC;             ///  flag for access to MC
  Bool_t fUsePhysSel;         /// flag use/not use phys sel
  Bool_t fUsePileupCut;       /// flag use/not use phys pileup cut
  Int_t  fTriggerMask;        /// mask used in physics selection
  Bool_t fGoUpToQuark;        /// flag for definition of c,b origin
  Bool_t fKeepNegID;          /// flag to keep also track with negative ID (default kFALSE, change it only if you know what you are doing)

  /// \cond CLASSIMP
  ClassDef(AliAnalysisTaskSingleTrack4PID,1); /// D0D+ task from AOD tracks
  /// \endcond
};

#endif
