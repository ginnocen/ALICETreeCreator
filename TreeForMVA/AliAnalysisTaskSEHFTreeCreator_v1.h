#ifndef AliAnalysisTaskSEHFTreeCreator_v1_H
#define AliAnalysisTaskSEHFTreeCreator_v1_H

/* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

///*************************************************************************
/// \class AliAnalysisTaskSEHFTreeCreator_v1
/// 
// \authors:
// F. Catalano, fabio.catalano@cern.ch
// A. Festanti, andrea.festanti@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
// G. Innocenti, gian.michele.innocenti@cern.ch
// F. Prino, prino@to.infn.it
// L. Vermunt, luuk.vermunt@cern.ch
///*************************************************************************

#include <TROOT.h>
#include <TSystem.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>

#include "AliAnalysisTaskSE.h"
#include "AliRDHFCutsD0toKpi.h"
#include "AliRDHFCutsDstoKKpi.h"
#include "AliRDHFCutsDplustoKpipi.h"
#include "AliNormalizationCounter.h"
#include "AliHFTreeHandler.h"
#include "AliHFTreeHandlerD0toKpi.h"
#include "AliHFTreeHandlerDplustoKpipi.h"
#include "AliHFTreeHandlerDstoKKpi.h"


class AliAODEvent;

class AliAnalysisTaskSEHFTreeCreator_v1 : public AliAnalysisTaskSE
{
public:
    
    AliAnalysisTaskSEHFTreeCreator_v1();
    AliAnalysisTaskSEHFTreeCreator_v1(const char *name,TList *cutsList);
    virtual ~AliAnalysisTaskSEHFTreeCreator_v1();
    
    
    /// Implementation of interface methods
    virtual void UserCreateOutputObjects();
    virtual void Init();
    virtual void LocalInit() {Init();}
    virtual void UserExec(Option_t *option);
    virtual void Terminate(Option_t *option);
    
    
    void SetReadMC(Bool_t opt=kFALSE){fReadMC=opt;}
    void SetSystem(Int_t opt){fSys=opt;}
    void SetAODMismatchProtection(Int_t opt=1) {fAODProtection=opt;}
    void SetWriteOnlySignalTree(Bool_t opt){fWriteOnlySignal=opt;}
    void SetFillD0Tree(Int_t opt){fWriteVariableTreeD0=opt;}
    void SetFillDsTree(Int_t opt){fWriteVariableTreeDs=opt;}
    void SetFillDplusTree(Int_t opt){fWriteVariableTreeDplus=opt;}
    void SetPIDoptD0Tree(Int_t opt){fPIDoptD0=opt;}
    void SetPIDoptDsTree(Int_t opt){fPIDoptDs=opt;}
    void SetPIDoptDplusTree(Int_t opt){fPIDoptDplus=opt;}
    void SetEnableFillCent(Int_t opt){fEnableFillCent=opt;}
    void SetEnableFillNormd0MeasMinusExp(Int_t opt){fEnableFillNormd0MeasMinusExp=opt;}
        
    
    Int_t  GetSystem() const {return fSys;}
    Bool_t GetWriteOnlySignalTree() const {return fWriteOnlySignal;}
    
    void Process2Prong(TClonesArray *array2prong, AliAODEvent *aod, TClonesArray *arrMC, Float_t centrality);
    void Process3Prong(TClonesArray *array3Prong, AliAODEvent *aod, TClonesArray *arrMC, Float_t centrality);
    
private:
    
    AliAnalysisTaskSEHFTreeCreator_v1(const AliAnalysisTaskSEHFTreeCreator_v1&);
    AliAnalysisTaskSEHFTreeCreator_v1& operator=(const AliAnalysisTaskSEHFTreeCreator_v1&);
    
    
    
    TH1F                    *fNentries;                  //!<!   histogram with number of events on output slot 1
    TList                   *fListCuts;                  //      list of cuts sent to output slot 2
    AliRDHFCutsD0toKpi      *fFiltCutsD0toKpi;           //      D0toKpi filtering (or loose) cuts
    AliRDHFCutsDstoKKpi     *fFiltCutsDstoKKpi;          //      DstoKKpi filtering (or loose) cuts
    AliRDHFCutsDplustoKpipi *fFiltCutsDplustoKpipi;      //      DplustoKpipi filtering (or loose) cuts 
    AliRDHFCutsD0toKpi      *fCutsD0toKpi;               //      D0toKpi analysis cuts
    AliRDHFCutsDstoKKpi     *fCutsDstoKKpi;              //      DstoKKpi analysis cuts
    AliRDHFCutsDplustoKpipi *fCutsDplustoKpipi;          //      DplustoKpipi analysis cuts
    Bool_t                  fReadMC;                     //     flag for MC array: kTRUE = read it, kFALSE = do not read it
    TList                   *fListCounter;               //!<!   list for normalization counter on output slot 3
    AliNormalizationCounter *fCounter;                   //!<!   AliNormalizationCounter
    Bool_t                  fUseSelectionBit;
    Int_t                   fSys;                        // fSys=0 -> p-p; fSys=1 ->PbPb
    Int_t                   fAODProtection;              // flag to activate protection against AOD-dAOD mismatch.
                                                         // -1: no protection,  0: check AOD/dAOD nEvents only,  1: check AOD/dAOD nEvents + TProcessID names
    Int_t                  fWriteVariableTreeD0;         // flag to decide whether to write the candidate variables on a tree variables
                                                         // 0 don't fill
    													 // 1 fill standard tree
    Int_t                  fWriteVariableTreeDs;         // flag to decide whether to write the candidate variables on a tree variables
 														 // 0 don't fill
   														 // 1 fill standard tree
    Int_t                  fWriteVariableTreeDplus;      // flag to decide whether to write the candidate variables on a tree variables
    													 // 0 don't fill
                                                         // 1 fill standard tree
    TList                   *fListTree;                  //!<!
    TTree                   *fVariablesTreeD0;           //!<! tree of the candidate variables
    TTree                   *fVariablesTreeDs;           //!<! tree of the candidate variables
    TTree                   *fVariablesTreeDplus;        //!<! tree of the candidate variables
    bool                    fWriteOnlySignal;
    AliHFTreeHandlerD0toKpi        *fTreeHandlerD0;             //!<! handler object for the tree with topological variables
    AliHFTreeHandlerDstoKKpi       *fTreeHandlerDs;             //!<! handler object for the tree with topological variables
    AliHFTreeHandlerDplustoKpipi   *fTreeHandlerDplus;          //!<! handler object for the tree with topological variables
    int                     fPIDoptD0;
    int                     fPIDoptDs;
    int                     fPIDoptDplus;
    bool                    fEnableFillCent;
    bool                    fEnableFillNormd0MeasMinusExp;
    
    
    
    /// \cond CLASSIMP
    ClassDef(AliAnalysisTaskSEHFTreeCreator_v1,2);
    /// \endcond
};

#endif

