/**************************************************************************
 * Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id$ */

/////////////////////////////////////////////////////////////
//
//
//
// Authors:
// Andrea Festanti andrea.festanti@cern.ch
// Fabrizio Grosa  fabrizio.grosa@cern.ch
// Gian Michele Innocenti gian.michele.innocenti@cern.ch
// L.V.R. van Doremalen, lennart.van.doremalen@cern.ch
////////////////////////////////////////////////////////////

#include <Riostream.h>
#include <TClonesArray.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TList.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TDatabasePDG.h>
#include <THnSparse.h>
#include <AliAnalysisDataSlot.h>
#include <AliAnalysisDataContainer.h>
#include "TChain.h"
#include "AliVertexingHFUtils.h"
#include "AliAnalysisManager.h"
#include "AliESDtrack.h"
#include "AliVertexerTracks.h"
#include "AliAODHandler.h"
#include "AliAODEvent.h"
#include "AliAODVertex.h"
#include "AliAODTrack.h"
#include "AliAODMCHeader.h"
#include "AliAODMCParticle.h"
#include "AliAODRecoDecay.h"
#include "AliAODRecoDecayHF2Prong.h"
#include "AliAODRecoDecayHF3Prong.h"
#include "AliAODRecoCascadeHF.h"
#include "AliAODPidHF.h"
#include "AliAnalysisVertexingHF.h"
#include "AliNormalizationCounter.h"
#include "AliAnalysisTaskSE.h"
#include "AliAnalysisTaskSEHFTreeCreator.h"

using std::cout;
using std::endl;

/// \cond CLASSIMP
ClassImp(AliAnalysisTaskSEHFTreeCreator);
/// \endcond

//________________________________________________________________________
AliAnalysisTaskSEHFTreeCreator::AliAnalysisTaskSEHFTreeCreator():
    AliAnalysisTaskSE(),
    fNentries(0x0),
    fListCuts(0x0),
    fFiltCutsD0toKpi(0x0),
    fFiltCutsDstoKKpi(0x0),
    fFiltCutsDplustoKpipi(0x0),
    fFiltCutsBplustoD0pi(0x0),
    fCutsD0toKpi(0x0),
    fCutsDstoKKpi(0x0),
    fCutsDplustoKpipi(0x0),
    fCutsBplustoD0pi(0x0),
    fReadMC(0),
    fListCounter(0x0),
    fCounter(0x0),
    fUseSelectionBit(kTRUE),
    fSys(0),
    fAODProtection(1),
    fListTree(0),
    fWriteVariableTreeD0(0),
    fWriteVariableTreeDs(0),
    fWriteVariableTreeDplus(0),
    fWriteVariableTreeBplus(0),
    fVariablesTreeD0(0x0),
    fVariablesTreeDs(0x0),
    fVariablesTreeDplus(0x0),
    fVariablesTreeBplus(0x0),
    fWriteOnlySignal(kFALSE),
    fTreeHandlerD0(0x0),
    fTreeHandlerDs(0x0),
    fTreeHandlerDplus(0x0),
    fTreeHandlerBplus(0x0),
    fPIDoptD0(AliHFCutOptTreeHandler::kNsigmaPIDchar),
    fPIDoptDs(AliHFCutOptTreeHandler::kNsigmaPIDchar),
    fPIDoptDplus(AliHFCutOptTreeHandler::kNsigmaPIDchar),
    fPIDoptBplus(AliHFCutOptTreeHandler::kNsigmaPIDchar)
{

/// Default constructor

}
//________________________________________________________________________
AliAnalysisTaskSEHFTreeCreator::AliAnalysisTaskSEHFTreeCreator(const char *name, TList *cutsList):
    AliAnalysisTaskSE(name),
    fNentries(0x0),
    fListCuts(0x0),
    fFiltCutsD0toKpi(0x0),
    fFiltCutsDstoKKpi(0x0),
    fFiltCutsDplustoKpipi(0x0),
    fFiltCutsBplustoD0pi(0x0),
    fCutsD0toKpi(0x0),
    fCutsDstoKKpi(0x0),
    fCutsDplustoKpipi(0x0),
    fCutsBplustoD0pi(0x0),
    fReadMC(0),
    fListCounter(0x0),
    fCounter(0x0),
    fUseSelectionBit(kTRUE),
    fSys(0),
    fAODProtection(1),
    fListTree(0),
    fWriteVariableTreeD0(0),
    fWriteVariableTreeDs(0),
    fWriteVariableTreeDplus(0),
    fWriteVariableTreeBplus(0),
    fVariablesTreeD0(0x0),
    fVariablesTreeDs(0x0),
    fVariablesTreeDplus(0x0),
    fVariablesTreeBplus(0x0),
    fWriteOnlySignal(kFALSE),
    fTreeHandlerD0(0x0),
    fTreeHandlerDs(0x0),
    fTreeHandlerDplus(0x0),
    fTreeHandlerBplus(0x0),
    fPIDoptD0(AliHFCutOptTreeHandler::kNsigmaPIDchar),
    fPIDoptDs(AliHFCutOptTreeHandler::kNsigmaPIDchar),
    fPIDoptDplus(AliHFCutOptTreeHandler::kNsigmaPIDchar),
    fPIDoptBplus(AliHFCutOptTreeHandler::kNsigmaPIDchar)
{
    /// Standard constructor


    if (fFiltCutsD0toKpi) {
        delete fFiltCutsD0toKpi; fFiltCutsD0toKpi = NULL;
    }
    if (fFiltCutsDstoKKpi) {
        delete fFiltCutsDstoKKpi; fFiltCutsDstoKKpi = NULL;
    }
    if (fFiltCutsDplustoKpipi) {
        delete fFiltCutsDplustoKpipi; fFiltCutsDplustoKpipi = NULL;
    }
    if (fFiltCutsBplusD0pi) {
        delete fFiltCutsBplusD0pi; fFiltCutsBplusD0pi = NULL;
    }
    if (fCutsD0toKpi) {
        delete fCutsD0toKpi; fCutsD0toKpi = NULL;
    }
    if (fCutsDstoKKpi) {
        delete fCutsDstoKKpi; fCutsDstoKKpi = NULL;
    }
    if (fCutsDplustoKpipi) {
        delete fCutsDplustoKpipi; fCutsDplustoKpipi = NULL;
    }
    if (fCutsBplustoD0pi) {
        delete fCutsBplustoD0pi; fCutsBplustoD0pi = NULL;
    }

    fListCuts = cutsList;

    fFiltCutsD0toKpi     = (AliRDHFCutsD0toKpi*)fListCuts->FindObject("D0toKpiFilteringCuts");
    fFiltCutsDstoKKpi    = (AliRDHFCutsDstoKKpi*)fListCuts->FindObject("DstoKKpiFilteringCuts");
    fFiltCutsDplustoKpipi = (AliRDHFCutsDplustoKpipi*)fListCuts->FindObject("DplustoKpipiFilteringCuts");
    fFiltCutsBplustoD0pi = (AliRDHFCutsBPlustoD0Pi*)fListCuts->FindObject("BplustoD0piFilteringCuts");
    fCutsD0toKpi         = (AliRDHFCutsD0toKpi*)fListCuts->FindObject("D0toKpiAnalysisCuts");
    fCutsDstoKKpi        = (AliRDHFCutsDstoKKpi*)fListCuts->FindObject("DstoKKpiAnalysisCuts");
    fCutsDplustoKpipi    = (AliRDHFCutsDplustoKpipi*)fListCuts->FindObject("DplustoKpipiAnalysisCuts");
    fCutsBplustoD0pi     = (AliRDHFCutsBPlustoD0Pi*)fListCuts->FindObject("BplustoD0piAnalysisCuts");


    DefineInput(0, TChain::Class());
    // Output slot #1 writes into a TH1F container (number of events)
    DefineOutput(1, TH1F::Class());
    // Output slot #2 writes into a TList container (cuts)
    DefineOutput(2, TList::Class());
    // Output slot #3 writes Normalization Counter
    DefineOutput(3, TList::Class());
    // Output slot #4 stores the tree of the candidate variables after track selection
    DefineOutput(4, TList::Class());

}
//________________________________________________________________________
AliAnalysisTaskSEHFTreeCreator::~AliAnalysisTaskSEHFTreeCreator()
{
    if (fListCuts) {
        delete fListCuts;
        fListCuts = 0;
    }
    if (fFiltCutsD0toKpi) {
        delete fFiltCutsD0toKpi;
        fFiltCutsD0toKpi = 0;
    }
    if (fFiltCutsDstoKKpi) {
        delete fFiltCutsDstoKKpi;
        fFiltCutsDstoKKpi = 0;
    }
    if (fFiltCutsDplustoKpipi) {
        delete fFiltCutsDplustoKpipi;
        fFiltCutsDplustoKpipi = 0;
    }
    if (fFiltCutsBplusD0pi) {
        delete fFiltCutsBplusD0pi;
        fFiltCutsBplusD0pi = 0;
    }
    if (fCutsD0toKpi) {
        delete fCutsD0toKpi;
        fCutsD0toKpi = 0;
    }
    if (fCutsDstoKKpi) {
        delete fCutsDstoKKpi;
        fCutsDstoKKpi = 0;
    }
    if (fCutsDplustoKpipi) {
        delete fCutsDplustoKpipi;
        fCutsDplustoKpipi = 0;
    }
    if (fCutsBplustoD0pi) {
        delete fCutsBplustoD0pi;
        fCutsBplustoD0pi = 0;
    }
    if (fNentries) {
        delete fNentries;
        fNentries = 0;
    }
    if (fListCounter) {
        delete fListCounter;
        fListCounter = 0;
    }
    if (fCounter) {
        delete fCounter;
        fCounter = 0;
    }
    if (fListTree) {
        delete fListTree;
        fListTree = 0;
    }
    if (fTreeHandlerD0) {
        delete fTreeHandlerD0;
        fTreeHandlerD0 = 0;
    }
    if (fTreeHandlerDs) {
        delete fTreeHandlerDs;
        fTreeHandlerDs = 0;
    }
    if (fTreeHandlerDplus) {
        delete fTreeHandlerDplus;
        fTreeHandlerDplus = 0;
    }
    if (fTreeHandlerBplus) {
        delete fTreeHandlerBplus;
        fTreeHandlerBplus = 0;
    }
}

//________________________________________________________________________
void AliAnalysisTaskSEHFTreeCreator::Init()
{
    /// Initialization

    if (fDebug > 1) printf("AliAnalysisTaskSEHFTreeCreator::Init() \n");

    PostData(2, fListCuts);

    return;
}

//________________________________________________________________________
void AliAnalysisTaskSEHFTreeCreator::UserCreateOutputObjects()
{

    /// Create the output container
    //
    if (fDebug > 1) printf("AliAnalysisTaskSEHFTreeCreator::UserCreateOutputObjects() \n");

    const char* nameoutput = GetOutputSlot(1)->GetContainer()->GetName();
    fNentries = new TH1F(nameoutput, "Number of events", 27, -0.5, 26.5);
    fNentries->GetXaxis()->SetBinLabel(1, "n. evt. read");
    fNentries->GetXaxis()->SetBinLabel(2, "n. evt. matched dAOD");
    fNentries->GetXaxis()->SetBinLabel(3, "n. evt. mismatched dAOD");
    fNentries->GetXaxis()->SetBinLabel(4, "n. evt. analised");
    fNentries->GetXaxis()->SetBinLabel(5, "n. evt. passing IsEvSelected");
    fNentries->GetXaxis()->SetBinLabel(6, "n. evt. rejected due to trigger");
    fNentries->GetXaxis()->SetBinLabel(7, "n. evt. rejected due to not reco vertex");
    fNentries->GetXaxis()->SetBinLabel(8, "n. evt. rejected for contr vertex");
    fNentries->GetXaxis()->SetBinLabel(9, "n. evt. rejected for vertex out of accept");
    fNentries->GetXaxis()->SetBinLabel(10, "n. evt. rejected for pileup events");
    fNentries->GetXaxis()->SetBinLabel(11, "n. evt. of out centrality events");
    fNentries->GetXaxis()->SetBinLabel(12, "n. of 2 prong candidates");
    fNentries->GetXaxis()->SetBinLabel(13, "n. D0 after filtering");
    fNentries->GetXaxis()->SetBinLabel(14, "n. D0 after selection");
    fNentries->GetXaxis()->SetBinLabel(15, "n. of not on-the-fly rec D0");
    fNentries->GetXaxis()->SetBinLabel(16, "n. of 3 prong candidates");
    fNentries->GetXaxis()->SetBinLabel(17, "n. Ds after filtering");
    fNentries->GetXaxis()->SetBinLabel(18, "n. Ds after selection");
    fNentries->GetXaxis()->SetBinLabel(19, "n. of not on-the-fly rec Ds");
    fNentries->GetXaxis()->SetBinLabel(20, "n. Dplus after filtering");
    fNentries->GetXaxis()->SetBinLabel(21, "n. Dplus after selection");
    fNentries->GetXaxis()->SetBinLabel(22, "n. of not on-the-fly rec Dplus");
    fNentries->GetXaxis()->SetBinLabel(23, "n. of 2 prong candidates for Bplus");
    fNentries->GetXaxis()->SetBinLabel(24, "n. Bplus after filtering");
    fNentries->GetXaxis()->SetBinLabel(25, "n. Bplus after selection");
    fNentries->GetXaxis()->SetBinLabel(26, "n. of not on-the-fly rec Bplus");


    fListCounter = new TList();
    fListCounter->SetOwner(kTRUE);
    fListCounter->SetName("NormCounter");
    fCounter = new AliNormalizationCounter("norm_counter");
    fCounter->Init();
    fListCounter->Add(fCounter);


    //
    // Output slot 4 : list of trees of the candidate variables after track selection
    //

    fListTree = new TList();
    fListTree->SetOwner(kTRUE);
    fListTree->SetName("Trees");

    if (fWriteVariableTreeD0) {
        TString nameoutput = "tree_D0";
        fTreeHandlerD0 = new AliHFCutOptTreeHandler(AliHFCutOptTreeHandler::kD0toKpi, fPIDoptD0, fReadMC);
        if (fSys > 0) fTreeHandlerD0->SetUseCentrality();
        if (fReadMC && fWriteOnlySignal) fTreeHandlerD0->SetFillOnlySignal();
        fTreeHandlerD0->SetIsMC(fReadMC);
        fTreeHandlerD0->SetUseSelectedStdFlag(kTRUE);
        fVariablesTreeD0 = (TTree*)fTreeHandlerD0->BuildTree(nameoutput, nameoutput);
        fListTree->Add(fVariablesTreeD0);
    }
    if (fWriteVariableTreeDs) {
        TString nameoutput = "tree_Ds";
        fTreeHandlerDs = new AliHFCutOptTreeHandler(AliHFCutOptTreeHandler::kDstoKKpi, fPIDoptDs, fReadMC);
        if (fSys > 0) fTreeHandlerDs->SetUseCentrality();
        if (fReadMC && fWriteOnlySignal) fTreeHandlerDs->SetFillOnlySignal();
        fTreeHandlerDs->SetIsMC(fReadMC);
        fTreeHandlerDs->SetUseSelectedStdFlag(kTRUE);
        fVariablesTreeDs = (TTree*)fTreeHandlerDs->BuildTree(nameoutput, nameoutput);
        fListTree->Add(fVariablesTreeDs);
    }
    if (fWriteVariableTreeDplus) {
        TString nameoutput = "tree_Dplus";
        fTreeHandlerDplus = new AliHFCutOptTreeHandler(AliHFCutOptTreeHandler::kDplustoKpipi, fPIDoptDplus, fReadMC);
        if (fSys > 0) fTreeHandlerDplus->SetUseCentrality();
        if (fReadMC && fWriteOnlySignal) fTreeHandlerDplus->SetFillOnlySignal();
        fTreeHandlerDplus->SetIsMC(fReadMC);
        fTreeHandlerDplus->SetUseSelectedStdFlag(kTRUE);
        fVariablesTreeDplus = (TTree*)fTreeHandlerDplus->BuildTree(nameoutput, nameoutput);
        fListTree->Add(fVariablesTreeDplus);
    }
    if (fWriteVariableTreeBplus) {
        TString nameoutput = "tree_Bplus";
        fTreeHandlerBplus = new AliHFCutOptTreeHandler(AliHFCutOptTreeHandler::kBplustoD0pi, fPIDoptBplus, fReadMC);
        if (fSys > 0) fTreeHandlerBplus->SetUseCentrality();
        if (fReadMC && fWriteOnlySignal) fTreeHandlerBplus->SetFillOnlySignal();
        fTreeHandlerBplus->SetIsMC(fReadMC);
        fTreeHandlerBplus->SetUseSelectedStdFlag(kTRUE);
        fVariablesTreeBplus = (TTree*)fTreeHandlerBplus->BuildTree(nameoutput, nameoutput);
        fListTree->Add(fVariablesTreeBplus);
    }

    // Post the data
    PostData(1, fNentries);
    PostData(3, fListCounter);
    PostData(4, fListTree);


    return;
}

//________________________________________________________________________
void AliAnalysisTaskSEHFTreeCreator::UserExec(Option_t */*option*/)

{
    /// Execute analysis for current event:

    AliAODEvent *aod = dynamic_cast<AliAODEvent*> (InputEvent());

    fNentries->Fill(0); // all events
    if (fAODProtection >= 0) {
        //   Protection against different number of events in the AOD and deltaAOD
        //   In case of discrepancy the event is rejected.
        Int_t matchingAODdeltaAODlevel = AliRDHFCuts::CheckMatchingAODdeltaAODevents();
        if (matchingAODdeltaAODlevel < 0 || (matchingAODdeltaAODlevel == 0 && fAODProtection == 1)) {
            // AOD/deltaAOD trees have different number of entries || TProcessID do not match while it was required
            fNentries->Fill(2);
            return;
        }
        fNentries->Fill(1);
    }


    TString candidates2prongArrayName = "D0toKpi";
    TString candidates3prongArrayName = "Charm3Prong";
    TClonesArray *array2prong = 0;
    TClonesArray *array3Prong = 0;

    if (!aod && AODEvent() && IsStandardAOD()) {
        // In case there is an AOD handler writing a standard AOD, use the AOD
        // event in memory rather than the input (ESD) event.
        aod = dynamic_cast<AliAODEvent*> (AODEvent());
        // in this case the braches in the deltaAOD (AliAOD.VertexingHF.root)
        // have to taken from the AOD event hold by the AliAODExtension
        AliAODHandler* aodHandler = (AliAODHandler*)
                                    ((AliAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());

        if (aodHandler->GetExtensions()) {
            AliAODExtension *ext = (AliAODExtension*)aodHandler->GetExtensions()->FindObject("AliAOD.VertexingHF.root");
            AliAODEvent* aodFromExt = ext->GetAOD();
            array2prong = (TClonesArray*)aodFromExt->GetList()->FindObject(candidates2prongArrayName.Data());
            array3Prong = (TClonesArray*)aodFromExt->GetList()->FindObject(candidates3prongArrayName.Data());
        }
    } else if (aod) {
        array2prong = (TClonesArray*)aod->GetList()->FindObject(candidates2prongArrayName.Data());
        array3Prong = (TClonesArray*)aod->GetList()->FindObject(candidates3prongArrayName.Data());
    }

    if (!array2prong || !array3Prong || !aod) {
        printf("AliAnalysisTaskSEHFTreeCreator::UserExec: input branches not found!\n");
        return;
    }
    // fix for temporary bug in ESDfilter
    // the AODs with null vertex pointer didn't pass the PhysSel
    if (!aod->GetPrimaryVertex() || TMath::Abs(aod->GetMagneticField()) < 0.001) return;

    fNentries->Fill(3); // count events

    if (fFiltCutsD0toKpi->IsEventRejectedDueToTrigger())fNentries->Fill(5);
    if (fFiltCutsD0toKpi->IsEventRejectedDueToNotRecoVertex())fNentries->Fill(6);
    if (fFiltCutsD0toKpi->IsEventRejectedDueToVertexContributors())fNentries->Fill(7);
    if (fFiltCutsD0toKpi->IsEventRejectedDueToZVertexOutsideFiducialRegion())fNentries->Fill(8);
    if (fFiltCutsD0toKpi->IsEventRejectedDueToPileup())fNentries->Fill(9);
    if (fFiltCutsD0toKpi->IsEventRejectedDueToCentrality())fNentries->Fill(10);

    TClonesArray *mcArray = 0;
    AliAODMCHeader *mcHeader = 0;

    if (fReadMC) {
        // load MC particles
        mcArray = (TClonesArray*)aod->GetList()->FindObject(AliAODMCParticle::StdBranchName());
        if (!mcArray) {
            printf("AliAnalysisTaskSEHFTreeCreator::UserExec: MC particles branch not found!\n");
            return;
        }

        // load MC header
        mcHeader = (AliAODMCHeader*)aod->GetList()->FindObject(AliAODMCHeader::StdBranchName());
        if (!mcHeader) {
            printf("AliAnalysisTaskSEHFTreeCreator::UserExec: MC header branch not found!\n");
            return;
        }
    }
    //Printf("CHECKING EVENT SELECTION");
    Bool_t isSameEvSelD0 = !((fFiltCutsD0toKpi->IsEventSelected(aod) && !fCutsD0toKpi->IsEventSelected(aod)) || (!fFiltCutsD0toKpi->IsEventSelected(aod) && fCutsD0toKpi->IsEventSelected(aod)));
    Bool_t isSameEvSelDs = !((fFiltCutsDstoKKpi->IsEventSelected(aod) && !fCutsDstoKKpi->IsEventSelected(aod)) || (!fFiltCutsDstoKKpi->IsEventSelected(aod) && fCutsDstoKKpi->IsEventSelected(aod)));
    Bool_t isSameEvSelDplus = !((fFiltCutsDplustoKpipi->IsEventSelected(aod) && !fCutsDplustoKpipi->IsEventSelected(aod)) || (!fFiltCutsDplustoKpipi->IsEventSelected(aod) && fCutsDplustoKpipi->IsEventSelected(aod)));
    Bool_t isSameEvSelBplus = !((fFiltCutsBplustoD0pi->IsEventSelected(aod) && !fCutsBplustoD0pi->IsEventSelected(aod)) || (!fFiltCutsBplustoD0pi->IsEventSelected(aod) && fCutsBplustoD0pi->IsEventSelected(aod)));
    Bool_t isSameEvSel = isSameEvSelD0 && isSameEvSelDs && isSameEvSelDplus && isSameEvSelBplus;
    if (!isSameEvSel) {
        Printf("AliAnalysisTaskSEHFTreeCreator::UserExec: differences in the event selection cuts same meson");
        return;
    }
    if ((fFiltCutsD0toKpi->IsEventSelected(aod) != fFiltCutsDstoKKpi->IsEventSelected(aod)) || (fFiltCutsD0toKpi->IsEventSelected(aod) != fFiltCutsDplustoKpipi->IsEventSelected(aod)) || (fFiltCutsD0toKpi->IsEventSelected(aod) != fFiltCutsBplustoD0pi->IsEventSelected(aod))) {
        Printf("AliAnalysisTaskSEHFTreeCreator::UserExec: differences in the event selection cuts different meson");
        return;
    }

    fCounter->StoreEvent(aod, fFiltCutsD0toKpi, fReadMC);
    Bool_t isEvSel = fFiltCutsD0toKpi->IsEventSelected(aod);
    if (!isEvSel)return;
    fNentries->Fill(4);

    Float_t ntracks = aod->GetNumberOfTracks();
    Float_t evCentr = fFiltCutsD0toKpi->GetCentrality(aod);
    Int_t runNumber = aod->GetRunNumber();

    //Printf("Process2Prong");
    if (fWriteVariableTreeD0 || fWriteVariableTreeBplus) Process2Prong(array2prong, aod, mcArray, evCentr);

    //Printf("Process3Prong");
    if (fWriteVariableTreeDs || fWriteVariableTreeDplus) Process3Prong(array3Prong, aod, mcArray, evCentr);


    // Post the data
    PostData(1, fNentries);
    PostData(3, fListCounter);
    PostData(4, fListTree);

    return;
}
//________________________________________________________________________
void AliAnalysisTaskSEHFTreeCreator::Terminate(Option_t */*option*/)
{
    /// Terminate analysis
    //
    if (fDebug > 1) printf("AliAnalysisTaskSEHFTreeCreator: Terminate() \n");


    fNentries = dynamic_cast<TH1F*>(GetOutputData(1));
    if (!fNentries) {
        printf("ERROR: fNEntries not available\n");
        return;
    }
    fListCuts = dynamic_cast<TList*>(GetOutputData(2));
    if (!fListCuts) {
        printf("ERROR: fListCuts not available\n");
        return;
    }
    fListCounter = dynamic_cast<TList*>(GetOutputData(3));
    if (!fListCounter) {
        printf("ERROR: fListCounter not available\n");
        return;
    }
    fListTree = dynamic_cast<TList*>(GetOutputData(4));
    if (!fListTree) {
        printf("ERROR: fListTree not available\n");
        return;
    }
    return;
}
//--------------------------------------------------------
void AliAnalysisTaskSEHFTreeCreator::Process2Prong(TClonesArray *array2prong, AliAODEvent *aod, TClonesArray *arrMC, Float_t centrality) {

    Int_t n2prong = array2prong->GetEntriesFast();
    if (fDebug > 2) printf("Number of D0->Kpi: %d\n", n2prong);

    Int_t nSelectedD0 = 0;
    Int_t nFilteredD0 = 0;

    Int_t nSelectedBplus = 0;
    Int_t nFilteredBplus = 0;

    AliAODPidHF* pidHFD0 = fCutsD0toKpi->GetPidHF();
    if (!pidHFD0) pidHFD0 = 0x0;
    AliAODPidHF* pidHFBplus = fCutsBplustoD0pi->GetPidHF();
    if (!pidHFBplus) pidHFBplus = 0x0;


    // vHF object is needed to call the method that refills the missing info of the candidates
    // if they have been deleted in dAOD reconstruction phase
    // in order to reduce the size of the file
    AliAnalysisVertexingHF *vHF = new AliAnalysisVertexingHF();

    for (Int_t i2prong = 0; i2prong < n2prong; i2prong++) {
        AliAODRecoDecayHF2Prong *d = (AliAODRecoDecayHF2Prong*)array2prong->UncheckedAt(i2prong);
        fNentries->Fill(11);

        if (fUseSelectionBit && d->GetSelectionMap()) if (!d->HasSelectionBit(AliRDHFCuts::kD0toKpiCuts)) {
                continue;
            }
        fNentries->Fill(12);
        nFilteredD0++;
        if (!(vHF->FillRecoCand(aod, d))) { //Fill the data members of the candidate only if they are empty.
            fNentries->Fill(14); //monitor how often this fails
            continue;
        }

        //fiducial acceptance
        Bool_t isFidAcc = fFiltCutsD0toKpi->IsInFiducialAcceptance(d->Pt(), d->Y(421));

        if (isFidAcc) {
            Int_t isSelectedFilt     = fFiltCutsD0toKpi->IsSelected(d, AliRDHFCuts::kAll, aod); //selected
            Int_t isSelectedAnalysis = fCutsD0toKpi->IsSelected(d, AliRDHFCuts::kAll, aod); //selected
            if (isSelectedFilt) {
                fNentries->Fill(13);
                nSelectedD0++;
            }
            if (!isSelectedFilt) continue;

            Int_t pdgDgD0toKpi[2] = {321, 211};
            Int_t labD0 = -1;
            if (fReadMC) labD0 = d->MatchToMC(421, arrMC, 2, pdgDgD0toKpi); //return MC particle label if the array corresponds to a D0, -1 if not (cf. AliAODRecoDecay.cxx)

            Bool_t issignal = kFALSE;
            Bool_t isprompt = kFALSE;
            Bool_t isrefl = kFALSE;
            Int_t masshypo = 0;


            if (isSelectedFilt == 1 || isSelectedFilt == 3) { //D0
                if (fWriteVariableTreeD0 && fSys > 0) fTreeHandlerD0->SetCentrality((Char_t)centrality);
                masshypo = 0;
                if (fReadMC) {
                    if (labD0 >= 0) {
                        AliAODMCParticle *partD0 = (AliAODMCParticle*)arrMC->At(labD0);
                        Int_t pdgD0 = partD0->GetPdgCode();
                        Int_t origin = AliVertexingHFUtils::CheckOrigin(arrMC, partD0, kTRUE);
                        if (origin == 4) isprompt = kTRUE;
                        else if (origin == 5) isprompt = kFALSE;

                        if (pdgD0 == 421) {
                            issignal = kTRUE;
                            isrefl = kFALSE;
                        }
                        else {
                            issignal = kTRUE;
                            isrefl = kTRUE;
                        }
                    }//end labD0check
                    else { //background
                        issignal = kFALSE;
                        isrefl = kFALSE;
                    }
                    if (fWriteVariableTreeD0) {
                        //Printf("Before filling tree D0 SIG");
                        fTreeHandlerD0->SetCandidateType(issignal, isprompt, isrefl);
                        fTreeHandlerD0->SetIsSelectedStd(isSelectedAnalysis);
                        // Printf("after caniddate type tree D0 BKG");
                        fTreeHandlerD0->SetVariables(d, masshypo, pidHF, 0x0);
                        // Printf("after setting variables tree D0 BKG");
                        fTreeHandlerD0->FillTree();
                        // Printf("after filling tree D0 SIG");
                    }
                }//end read MC
                else {
                    issignal = kFALSE;
                    isrefl = kFALSE;
                    if (fWriteVariableTreeD0) {
                        //Printf("Before filling tree D0 BKG");
                        fTreeHandlerD0->SetIsSelectedStd(isSelectedAnalysis);
                        fTreeHandlerD0->SetVariables(d, masshypo, pidHF, 0x0);
                        // Printf("after setting variables tree D0 BKG");
                        fTreeHandlerD0->FillTree();
                        // Printf("After filling tree D0 BKG");
                    }
                }

            }//end D0
            if (isSelectedFilt > 1) { //D0bar
                if (fWriteVariableTreeD0 && fSys > 0) fTreeHandlerD0->SetCentrality((Char_t)centrality);
                masshypo = 1;
                if (fReadMC) {
                    if (labD0 >= 0) {
                        AliAODMCParticle *partD0 = (AliAODMCParticle*)arrMC->At(labD0);
                        Int_t pdgD0 = partD0->GetPdgCode();
                        Int_t origin = AliVertexingHFUtils::CheckOrigin(arrMC, partD0, kTRUE);
                        if (origin == 4) isprompt = kTRUE;
                        else if (origin == 5) isprompt = kFALSE;

                        if (pdgD0 == -421) {
                            issignal = kTRUE;
                            isrefl = kFALSE;
                        }
                        else {
                            issignal = kTRUE;
                            isrefl = kTRUE;
                        }
                    } //end label check
                    else { //background MC
                        issignal = kFALSE;
                        isrefl = kFALSE;
                    }
                    if (fWriteVariableTreeD0) {
                        //Printf("Before filling tree D0bar SIG");
                        fTreeHandlerD0->SetCandidateType(issignal, isprompt, isrefl);
                        fTreeHandlerD0->SetIsSelectedStd(isSelectedAnalysis);
                        fTreeHandlerD0->SetVariables(d, masshypo, pidHF, 0x0);
                        fTreeHandlerD0->FillTree();
                        //Printf("After filling tree D0bar SIG");
                    }
                }//end readMC
                else {
                    issignal = kFALSE;
                    isrefl = kFALSE;
                    if (fWriteVariableTreeD0) {
                        //Printf("Before filling tree D0bar BKG");
                        fTreeHandlerD0->SetIsSelectedStd(isSelectedAnalysis);
                        fTreeHandlerD0->SetVariables(d, masshypo, pidHF, 0x0);
                        fTreeHandlerD0->FillTree();
                        //Printf("After filling tree D0bar BKG");
                    }
                }
            }//end D0bar
        }//end is fid acc

    }//end loop on candidates

    for (Int_t i2prong = 0; i2prong < n2prong; i2prong++) {
        AliAODRecoDecayHF2Prong *d = (AliAODRecoDecayHF2Prong*)array2prong->UncheckedAt(i2prong);
        fNentries->Fill(22);

        if (fUseSelectionBit && d->GetSelectionMap()) if (!d->HasSelectionBit(AliRDHFCuts::kBplustoD0piCuts)) {
                continue;
            }

        fNentries->Fill(23);
        nFilteredBplus++;
        if (!(vHF->FillRecoCand(aod, d))) { //Fill the data members of the candidate only if they are empty.
            fNentries->Fill(25); //monitor how often this fails
            continue;
        }

        // here D0 selection on first daughter
        AliAODTrack * trackFirstDaughter = (AliAODTrack*)(d->GetDaughter(0));
        if (trackFirstDaughter->GetITSNcls() < 1) continue;
        if (trackFirstDaughter->GetTPCNcls() < 1) continue;
        if (trackFirstDaughter->GetStatus()&AliESDtrack::kITSpureSA) continue;
        if (!(trackFirstDaughter->GetStatus()&AliESDtrack::kITSin)) continue;
        if (trackFirstDaughter->GetID() < 0) continue;
        Double_t covtest[21];
        if (!trackFirstDaughter->GetCovarianceXYZPxPyPz(covtest)) continue;

        if (trackFirstDaughter->GetITSNcls() < fCutsBplustoD0pi->GetMinITSNclsD0FirstDaughter()) continue;
        if (trackFirstDaughter->GetTPCNcls() < fCutsBplustoD0pi->GetMinTPCNclsD0FirstDaughter()) continue;
        if (fCutsBplustoD0pi->UseITSRefitD0FirstDaughter() == kTRUE) {if (!(trackFirstDaughter->GetStatus()&AliESDtrack::kITSrefit)) {continue;}}
        if (fCutsBplustoD0pi->UseTPCRefitD0FirstDaughter() == kTRUE) {if ((!(trackFirstDaughter->GetStatus()&AliESDtrack::kTPCrefit))) {continue;}}
        if (fCutsBplustoD0pi->UseFilterBitD0FirstDaughter() == kTRUE) {if (!(trackFirstDaughter->TestFilterMask(BIT(fCutsBplustoD0pi->GetFilterBitD0FirstDaughter())))) {continue;}}
        if (trackFirstDaughter->Pt() < fCutsBplustoD0pi->GetMinPtD0FirstDaughter()) continue;
        if (TMath::Abs(d0[0]) < fCutsBplustoD0pi->GetMind0D0FirstDaughter()) continue;
        if (TMath::Abs(trackFirstDaughter->Eta()) > fCutsBplustoD0pi->GetMaxAbsEtaD0FirstDaughter()) continue;

        Bool_t bHardSelectionArrayITSFirstDaughter[7] = {kFALSE};
        fCutsBplustoD0pi->GetHardSelectionArrayITSD0FirstDaughter(bHardSelectionArrayITSFirstDaughter);
        Bool_t bSoftSelectionArrayITSFirstDaughter[7] = {kFALSE};
        fCutsBplustoD0pi->GetSoftSelectionArrayITSD0FirstDaughter(bSoftSelectionArrayITSFirstDaughter);

        Int_t nCounterSoftSelectionFirstDaughter = 0;
        Bool_t bSoftITSPassFirstDaughter = kTRUE;
        Bool_t bHardITSPassFirstDaughter = kTRUE;
        for (Int_t j = 0; j < 7; ++j)
        {
            if (bHardSelectionArrayITSFirstDaughter[j])
            {
                if (!trackFirstDaughter->HasPointOnITSLayer(j)) bHardITSPassFirstDaughter = kFALSE;
            }
            if (bSoftSelectionArrayITSFirstDaughter[j])
            {
                if (trackFirstDaughter->HasPointOnITSLayer(j)) nCounterSoftSelectionFirstDaughter++;
            }
        }
        if (nCounterSoftSelectionFirstDaughter < fCutsBplustoD0pi->GetNSoftITSCutD0FirstDaughter()) bSoftITSPassFirstDaughter = kFALSE;
        if (!bHardITSPassFirstDaughter) continue;
        if (!bSoftITSPassFirstDaughter) continue;

        // here D0 selection on second daughter
        AliAODTrack * trackSecondDaughter = (AliAODTrack*)(d->GetDaughter(1));
        if (trackSecondDaughter->GetITSNcls() < 1) continue;
        if (trackSecondDaughter->GetTPCNcls() < 1) continue;
        if (trackSecondDaughter->GetStatus()&AliESDtrack::kITSpureSA) continue;
        if (!(trackSecondDaughter->GetStatus()&AliESDtrack::kITSin)) continue;
        if (trackSecondDaughter->GetID() < 0) continue;
        Double_t covtest[21];
        if (!trackSecondDaughter->GetCovarianceXYZPxPyPz(covtest)) continue;

        if (trackSecondDaughter->GetITSNcls() < fCutsBplustoD0pi->GetMinITSNclsD0SecondDaughter()) continue;
        if (trackSecondDaughter->GetTPCNcls() < fCutsBplustoD0pi->GetMinTPCNclsD0SecondDaughter()) continue;
        if (fCutsBplustoD0pi->UseITSRefitD0SecondDaughter() == kTRUE) {if (!(trackSecondDaughter->GetStatus()&AliESDtrack::kITSrefit)) {continue;}}
        if (fCutsBplustoD0pi->UseTPCRefitD0SecondDaughter() == kTRUE) {if ((!(trackSecondDaughter->GetStatus()&AliESDtrack::kTPCrefit))) {continue;}}
        if (fCutsBplustoD0pi->UseFilterBitD0SecondDaughter() == kTRUE) {if (!(trackSecondDaughter->TestFilterMask(BIT(fCutsBplustoD0pi->GetFilterBitD0SecondDaughter())))) {continue;}}
        if (trackSecondDaughter->Pt() < fCutsBplustoD0pi->GetMinPtD0SecondDaughter()) continue;
        if (TMath::Abs(d0[0]) < fCutsBplustoD0pi->GetMind0D0SecondDaughter()) continue;
        if (TMath::Abs(trackSecondDaughter->Eta()) > fCutsBplustoD0pi->GetMaxAbsEtaD0SecondDaughter()) continue;

        Bool_t bHardSelectionArrayITSSecondDaughter[7] = {kFALSE};
        fCutsBplustoD0pi->GetHardSelectionArrayITSD0SecondDaughter(bHardSelectionArrayITSSecondDaughter);
        Bool_t bSoftSelectionArrayITSSecondDaughter[7] = {kFALSE};
        fCutsBplustoD0pi->GetSoftSelectionArrayITSD0SecondDaughter(bSoftSelectionArrayITSSecondDaughter);

        Int_t nCounterSoftSelectionSecondDaughter = 0;
        Bool_t bSoftITSPassSecondDaughter = kTRUE;
        Bool_t bHardITSPassSecondDaughter = kTRUE;
        for (Int_t j = 0; j < 7; ++j)
        {
            if (bHardSelectionArrayITSSecondDaughter[j])
            {
                if (!trackSecondDaughter->HasPointOnITSLayer(j)) bHardITSPassSecondDaughter = kFALSE;
            }
            if (bSoftSelectionArrayITSSecondDaughter[j])
            {
                if (trackSecondDaughter->HasPointOnITSLayer(j)) nCounterSoftSelectionSecondDaughter++;
            }
        }
        if (nCounterSoftSelectionSecondDaughter < fCutsBplustoD0pi->GetNSoftITSCutD0SecondDaughter()) bSoftITSPassSecondDaughter = kFALSE;
        if (!bHardITSPassSecondDaughter) continue;
        if (!bSoftITSPassSecondDaughter) continue;

        // here selection on D0 track 
        Bool_t bCutArrayD0[29] = {0};
        Int_t cutReturnValueD0 = fFiltCutsBplustoD0pi->IsD0forD0ptbinSelected(d, 0, aod, bCutArrayD0); //filt cuts
        if (cutReturnValueD0 == -1) continue;
        if (cutReturnValueD0 == 0) continue;

        Bool_t isSelectedAnalysisD0 = kTRUE;
        Int_t cutSelectReturnValueD0 = fCutsBplustoD0pi->IsD0forD0ptbinSelected(d, 0, aod, bCutArrayD0); //std cuts
        if (cutSelectReturnValueD0 == -1) isSelectedAnalysisD0 = kFALSE;
        if (cutSelectReturnValueD0 == 0) isSelectedAnalysisD0 = kFALSE;

        //loop over pion track
        for (Int_t iTrack = 0; iTrack < aod->GetNumberOfTracks(); ++iTrack)
        {   
            // Get pion track and apply track cuts 
            AliAODTrack* pionTrack = dynamic_cast<AliAODTrack*>(aod->GetTrack(iTrack));
            if (!pionTrack) AliFatal("Not a standard AOD");

            AliAODVertex *primaryVertex = (AliAODVertex*)aod->GetPrimaryVertex();
            if (!primaryVertex) continue;

            if (pionTrack->GetITSNcls() < 1) continue;
            if (pionTrack->GetTPCNcls() < 1) continue;
            if (pionTrack->GetStatus()&AliESDtrack::kITSpureSA) continue;
            if (!(pionTrack->GetStatus()&AliESDtrack::kITSin)) continue;
            if (pionTrack->GetID() < 0) continue;
            Double_t covtest[21];
            if (!pionTrack->GetCovarianceXYZPxPyPz(covtest)) continue;

            Double_t pos[3], cov[6];
            primaryVertex->GetXYZ(pos);
            primaryVertex->GetCovarianceMatrix(cov);
            const AliESDVertex vESD(pos, cov, 100., 100);
            if (!fCutsBplustoD0pi->IsDaughterSelected(pionTrack, &vESD, fCutsBplustoD0pi->GetTrackCuts(), aod)) continue;

            if (pionTrack->GetITSNcls() < fCutsBplustoD0pi->GetMinITSNclsBPlusPion()) continue;
            if (pionTrack->GetTPCNcls() < fCutsBplustoD0pi->GetMinTPCNclsBPlusPion()) continue;
            if (fCutsBplustoD0pi->UseITSRefitBPlusPion() == kTRUE) {if (!(pionTrack->GetStatus()&AliESDtrack::kITSrefit)) {continue;}}
            if (fCutsBplustoD0pi->UseTPCRefitBPlusPion() == kTRUE) {if ((!(pionTrack->GetStatus()&AliESDtrack::kTPCrefit))) {continue;}}
            if (fCutsBplustoD0pi->UseFilterBitBPlusPion() == kTRUE) {if (!(pionTrack->TestFilterMask(BIT(fCutsBplustoD0pi->GetFilterBitBPlusPion())))) {continue;}}
            if (pionTrack->Pt() < fCutsBplustoD0pi->GetMinPtBPlusPion()) continue;
            if (TMath::Abs(d0[0]) < fCutsBplustoD0pi->GetMind0BPlusPion()) continue;
            if (TMath::Abs(pionTrack->Eta()) > fCutsBplustoD0pi->GetMaxAbsEtaBPlusPion()) continue;

            Bool_t bHardSelectionArrayITSpionTrack[7] = {kFALSE};
            fCutsBplustoD0pi->GetHardSelectionArrayITSBPlusPion(bHardSelectionArrayITSpionTrack);
            Bool_t bSoftSelectionArrayITSpionTrack[7] = {kFALSE};
            fCutsBplustoD0pi->GetSoftSelectionArrayITSBPlusPion(bSoftSelectionArrayITSpionTrack);

            Int_t nCounterSoftSelectionpionTrack = 0;
            Bool_t bSoftITSPasspionTrack = kTRUE;
            Bool_t bHardITSPasspionTrack = kTRUE;
            for (Int_t j = 0; j < 7; ++j)
            {
                if (bHardSelectionArrayITSpionTrack[j])
                {
                    if (!pionTrack->HasPointOnITSLayer(j)) bHardITSPasspionTrack = kFALSE;
                }
                if (bSoftSelectionArrayITSpionTrack[j])
                {
                    if (pionTrack->HasPointOnITSLayer(j)) nCounterSoftSelectionpionTrack++;
                }
            }
            if (nCounterSoftSelectionpionTrack < fCutsBplustoD0pi->GetNSoftITSCutBPlusPion()) bSoftITSPasspionTrack = kFALSE;
            if (!bHardITSPasspionTrack) continue;
            if (!bSoftITSPasspionTrack) continue;

            //we check if the IDs of the tracks are different
            AliAODTrack* twoProngdaughter0 = (AliAODTrack*)trackD0->GetDaughter(0);
            AliAODTrack* twoProngdaughter1 = (AliAODTrack*)trackD0->GetDaughter(1);
            UShort_t idProng0 = twoProngdaughter0->GetID();
            UShort_t idProng1 = twoProngdaughter1->GetID();
            if (pionTrack->GetID() == idProng0 || pionTrack->GetID() == idProng1) continue;

            // cuts on D0 inv mass using Bplus info
            UInt_t prongsD0[2];
            prongsD0[0] = 211;
            prongsD0[1] = 321;


            UInt_t prongsD02[2];
            prongsD02[1] = 211;
            prongsD02[0] = 321;

            // D0 window - invariant mass cut
            Double_t invariantMassD0 = trackD0->InvMass(2, prongsD0);
            Double_t invariantMassD02 = trackD0->InvMass(2, prongsD02);

            Double_t pdgMassD0 = TDatabasePDG::Instance()->GetParticle(421)->Mass();
            Double_t massWindowD0 = 0.06; //GeV/c^2  //hardset cut - has to be changed at some point 
            Int_t pdgD0 = 421;

            if (pionTrack->Charge() == -1)
            {
            if (TMath::Abs(invariantMassD0 - pdgMassD0) > massWindowD0) continue;

            } else if (pionTrack->Charge() == 1) {
            if (TMath::Abs(invariantMassD02 - pdgMassD0) > massWindowD0) continue;
            }

            //we use the BPlus pion and D0 tracks to reconstruct the vertex for the BPlus
            AliExternalTrackParam firstTrack;
            firstTrack.CopyFromVTrack(pionTrack);
            AliExternalTrackParam secondTrack;
            secondTrack.CopyFromVTrack(trackD0);

            UInt_t prongs[2];
            prongs[1] = 421;
            prongs[0] = 211;

            // we calculate the vertex of the mother candidate
            TObjArray daughterTracks;

            daughterTracks.Add(&firstTrack);
            daughterTracks.Add(&secondTrack);
            Double_t dispersion = 0;
            AliAODVertex *vertexMother = RecalculateVertex(primaryVertex, &daughterTracks, bz, dispersion);
            if (!vertexMother)
            {
              delete vertexMother; vertexMother = nullptr;
              continue;
            }

            //use the new vertex to create the BPlus candidate
            Double_t xdummy = 0., ydummy = 0., dca;
            Double_t d0z0[2], covd0z0[3], d0[2], d0err[2];


            firstTrack.PropagateToDCA(vertexMother, bz, 100., d0z0, covd0z0);
            secondTrack.PropagateToDCA(vertexMother, bz, 100., d0z0, covd0z0);

            //we reconstruct the mother decay prong
            Double_t px[2], py[2], pz[2];
            px[0] = firstTrack.Px();
            py[0] = firstTrack.Py();
            pz[0] = firstTrack.Pz();
            px[1] = secondTrack.Px();
            py[1] = secondTrack.Py();
            pz[1] = secondTrack.Pz();

            UShort_t id[2];
            id[0] = firstTrack.GetID();
            id[1] = secondTrack.GetID();

            firstTrack.PropagateToDCA(primaryVertex, bz, 100., d0z0, covd0z0);
            d0[0] = d0z0[0];
            d0err[0] = TMath::Sqrt(covd0z0[0]);
            secondTrack.PropagateToDCA(primaryVertex, bz, 100., d0z0, covd0z0);
            d0[1] = d0z0[0];
            d0err[1] = TMath::Sqrt(covd0z0[0]);

            dca = secondTrack.GetDCA(&firstTrack, bz, xdummy, ydummy);

            Short_t chargeMother = trackD0->Charge() + pionTrack->Charge();

            AliAODRecoDecayHF2Prong trackBPlus(vertexMother, px, py, pz, d0, d0err, dca);

            trackBPlus.SetCharge(chargeMother);
            trackBPlus.GetSecondaryVtx()->AddDaughter(pionTrack);
            trackBPlus.GetSecondaryVtx()->AddDaughter(trackD0);
            trackBPlus.SetPrimaryVtxRef((AliAODVertex*)aodEvent->GetPrimaryVertex());
            trackBPlus.SetProngIDs(2, id);

            // Fiducial cut
            if (TMath::Abs(trackBPlus.Y(521)) > 0.8) { //will be moved to cutfile at a later point
              delete vertexMother; vertexMother = nullptr;
              continue;
            }

            //apply cuts on BPlus and D0 
            Bool_t bCutArrayBPlus[68] = {0};
            Int_t cutReturnValueBPlus = fFiltCutsBplustoD0pi->IsSelected(&trackBPlus, 0, aod, bCutArrayBPlus); // filt cuts
            if (cutReturnValueBPlus == -1 || cutReturnValueBPlus == 0) {               
                delete vertexMother; vertexMother = nullptr;
                continue;
            }

            Bool_t isSelectedAnalysis = isSelectedAnalysisD0;
            Int_t cutReturnValueBPlus = fCutsBplustoD0pi->IsSelected(&trackBPlus, 0, aod, bCutArrayBPlus); // also do filt cuts
            if (cutReturnValueBPlus == -1 || cutReturnValueBPlus == 0) {               
                isSelectedAnalysis = kFALSE;
            }

            fNentries->Fill(24);
            nSelectedBplus++;

            Int_t labBplus = -1;
            if (fReadMC) labBplus = MatchBPlusCandidateToMonteCarlo(521, trackBPlus, arrMC); //return MC particle label if the array corresponds to a BPlus, -1 if not

            // add Bplus match to mc

            Bool_t issignal = kFALSE;
            Int_t masshypo = 0;
            if(labBplus >= 0) issignal = kTRUE;

            if (fWriteVariableTreeBplus && fSys > 0) fTreeHandlerBplus->SetCentrality((Char_t)centrality);
            masshypo = 0;
            if (fReadMC) {
                if (fWriteVariableTreeBplus) {
                    //Printf("Before filling tree Bplus SIG");
                    fTreeHandlerBplus->SetCandidateType(issignal, kFALSE, kFALSE);
                    fTreeHandlerBplus->SetIsSelectedStd(isSelectedAnalysis);
                    // Printf("after caniddate type tree Bplus BKG");
                    fTreeHandlerBplus->SetVariables(&trackBPlus, masshypo, pidHFBplus, 0x0);
                    // Printf("after setting variables tree Bplus BKG");
                    fTreeHandlerBplus->FillTree();
                    // Printf("after filling tree D0 SIG");
                }
            }//end read MC
            else {
                if (fWriteVariableTreeBplus) {
                    //Printf("Before filling tree Bplus BKG");
                    fTreeHandlerBplus->SetIsSelectedStd(isSelectedAnalysis);
                    fTreeHandlerBplus->SetVariables(&trackBPlus, masshypo, pidHFBplus, 0x0);
                    // Printf("after setting variables tree Bplus BKG");
                    fTreeHandlerBplus->FillTree();
                    // Printf("After filling tree Bplus BKG");
                }
            }
            delete vertexMother; vertexMother = nullptr;
        } // end loop over pion track
    }//end loop on candidates

    delete vHF;
    return;
}

//--------------------------------------------------------
void AliAnalysisTaskSEHFTreeCreator::Process3Prong(TClonesArray * array3Prong, AliAODEvent * aod, TClonesArray * arrMC, Float_t centrality) {

    AliAODVertex *vtx1 = (AliAODVertex*)aod->GetPrimaryVertex();

    Int_t n3prong = array3Prong->GetEntriesFast();
    if (fDebug > 1) printf("Number of Ds->KKpi: %d\n", n3prong);

    Int_t pdgDstoKKpi[3] = {321, 321, 211};
    Int_t nSelectedDs = 0;
    Int_t nFilteredDs = 0;
    Double_t massPhi = TDatabasePDG::Instance()->GetParticle(333)->Mass();

    AliAODPidHF* pidHFDs = fFiltCutsDstoKKpi->GetPidHF();
    if (!pidHFDs) pidHFDs = 0x0;
    AliAODPidHF* pidHFDplus = fFiltCutsDplustoKpipi->GetPidHF();
    if (!pidHFDplus) pidHFDplus = 0x0;

    Int_t pdgDgDplustoKpipi[3] = {321, 211, 211};
    Int_t nSelectedDplus = 0;
    Int_t nFilteredDplus = 0;


    // vHF object is needed to call the method that refills the missing info of the candidates
    // if they have been deleted in dAOD reconstruction phase
    // in order to reduce the size of the file
    AliAnalysisVertexingHF *vHF = new AliAnalysisVertexingHF();

    for (Int_t i3prong = 0; i3prong < n3prong; i3prong++) {
        fNentries->Fill(15);

        //Ds
        Bool_t isDstagged = kTRUE;
        AliAODRecoDecayHF3Prong *ds    = (AliAODRecoDecayHF3Prong*)array3Prong->UncheckedAt(i3prong);
        if (fUseSelectionBit && !(ds->HasSelectionBit(AliRDHFCuts::kDsCuts))) {
            isDstagged = kFALSE;
        }

        if (isDstagged) {
            fNentries->Fill(16);
            nFilteredDs++;
            if ((vHF->FillRecoCand(aod, ds))) { ////Fill the data members of the candidate only if they are empty.

                Bool_t isFidAcc = fFiltCutsDstoKKpi->IsInFiducialAcceptance(ds->Pt(), ds->YDs());
                if (isFidAcc) {
                    //Printf("Ds ");
                    Int_t isSelectedFilt = fFiltCutsDstoKKpi->IsSelected(ds, AliRDHFCuts::kAll, aod);
                    //Printf("Filtering %d",i3prong);
                    Int_t isSelectedAnalysis = fCutsDstoKKpi->IsSelected(ds, AliRDHFCuts::kAll, aod);
                    //Printf("Analysis %d",i3prong);
                    Int_t isSelectedCandidateNoRes = isSelectedFilt;
                    Bool_t origRes = fFiltCutsDstoKKpi->IsCutOnResonancesApplied();
                    if (origRes) {
                        fFiltCutsDstoKKpi->ApplyCutOnResonances(kFALSE);
                        isSelectedCandidateNoRes = fFiltCutsDstoKKpi->IsSelected(ds, AliRDHFCuts::kAll, aod);
                        fFiltCutsDstoKKpi->ApplyCutOnResonances(origRes);
                    }

                    Int_t isKKpi = isSelectedFilt & 1;
                    Int_t ispiKK = isSelectedFilt & 2;
                    Int_t isPhiKKpi = isSelectedFilt & 4;
                    Int_t isPhipiKK = isSelectedFilt & 8;
                    Int_t isK0starKKpi = isSelectedFilt & 16;
                    Int_t isK0starpiKK = isSelectedFilt & 32;

                    if (isSelectedFilt > 0) {

                        fNentries->Fill(17);
                        nSelectedDs++;

                        Bool_t unsetvtx = kFALSE;
                        if (!ds->GetOwnPrimaryVtx()) {
                            ds->SetOwnPrimaryVtx(vtx1);
                            unsetvtx = kTRUE;
                            // NOTE: the ow primary vertex should be unset, otherwise there is a memory leak
                            // Pay attention if you use continue inside this loop!!!
                        }
                        Bool_t recVtx = kFALSE;
                        AliAODVertex *origownvtx = 0x0;
                        if (fFiltCutsDstoKKpi->GetIsPrimaryWithoutDaughters()) {
                            if (ds->GetOwnPrimaryVtx()) origownvtx = new AliAODVertex(*ds->GetOwnPrimaryVtx());
                            if (fFiltCutsDstoKKpi->RecalcOwnPrimaryVtx(ds, aod))recVtx = kTRUE;
                            else fFiltCutsDstoKKpi->CleanOwnPrimaryVtx(ds, aod, origownvtx);
                        }

                        Int_t labDs = -1;
                        Int_t labDplus = -1;
                        Int_t pdgCode0 = -999;
                        Int_t orig = 0;

                        if (fReadMC) {

                            AliAODMCParticle *partDs = 0x0;


                            labDs = ds->MatchToMC(431, arrMC, 3, pdgDstoKKpi);
                            labDplus = ds->MatchToMC(411, arrMC, 3, pdgDstoKKpi);

                            if (labDs >= 0) {
                                Int_t labDau0 = ((AliAODTrack*)ds->GetDaughter(0))->GetLabel();
                                AliAODMCParticle* p = (AliAODMCParticle*)arrMC->UncheckedAt(TMath::Abs(labDau0));
                                pdgCode0 = TMath::Abs(p->GetPdgCode());

                                partDs = (AliAODMCParticle*)arrMC->At(labDs);

                            }
                            else {
                                if (labDplus >= 0) {
                                    Int_t labDau0 = ((AliAODTrack*)ds->GetDaughter(0))->GetLabel();
                                    AliAODMCParticle* p = (AliAODMCParticle*)arrMC->UncheckedAt(TMath::Abs(labDau0));
                                    pdgCode0 = TMath::Abs(p->GetPdgCode());

                                    partDs = (AliAODMCParticle*)arrMC->At(labDplus);
                                }
                            }
                            if (partDs) orig = AliVertexingHFUtils::CheckOrigin(arrMC, partDs, kTRUE);
                        }

                        if (fWriteVariableTreeDs > 0 && fWriteVariableTreeDs < 4) {
                            if ((fWriteVariableTreeDs == 1 && (isPhiKKpi || isPhipiKK)) || (fWriteVariableTreeDs == 2 && (isK0starKKpi || isK0starpiKK)) || (fWriteVariableTreeDs == 3 && (isKKpi || ispiKK))) {
                                if (fSys > 0) fTreeHandlerDs->SetCentrality((Char_t)centrality);

                                Bool_t issignal = kFALSE;
                                Bool_t isprompt = kFALSE;
                                Bool_t isrefl = kFALSE;

                                if (isKKpi || isPhiKKpi || isK0starKKpi) {
                                    if (fReadMC) {
                                        if (labDs >= 0) issignal = kTRUE;
                                        else issignal = kFALSE;

                                        if (orig == 4) isprompt = kTRUE;
                                        else if (orig == 5) isprompt = kFALSE;

                                        if (pdgCode0 == 211) isrefl = kTRUE;
                                        else isrefl = kFALSE;

                                        fTreeHandlerDs->SetIsSelectedStd(isSelectedAnalysis);
                                        fTreeHandlerDs->SetCandidateType(issignal, isprompt, isrefl);
                                        fTreeHandlerDs->SetVariables(ds, 0, pidHFDs, 0x0);
                                        fTreeHandlerDs->FillTree();
                                    }
                                    else { //real data
                                        fTreeHandlerDs->SetVariables(ds, 0, pidHFDs, 0x0);
                                        fTreeHandlerDs->FillTree();
                                    }
                                }
                                if (ispiKK || isPhipiKK || isK0starpiKK) {
                                    if (fReadMC) {
                                        if (labDs >= 0) issignal = kTRUE;
                                        else issignal = kFALSE;

                                        if (orig == 4) isprompt = kTRUE;
                                        else if (orig == 5) isprompt = kFALSE;

                                        if (pdgCode0 == 321) isrefl = kTRUE;
                                        else isrefl = kFALSE;

                                        fTreeHandlerDs->SetIsSelectedStd(isSelectedAnalysis);
                                        fTreeHandlerDs->SetCandidateType(issignal, isprompt, isrefl);
                                        fTreeHandlerDs->SetVariables(ds, 1, pidHFDs, 0x0);
                                        fTreeHandlerDs->FillTree();
                                    }
                                    else { //real data
                                        fTreeHandlerDs->SetIsSelectedStd(isSelectedAnalysis);
                                        fTreeHandlerDs->SetVariables(ds, 1, pidHFDs, 0x0);
                                        fTreeHandlerDs->FillTree();
                                    }
                                }
                            }
                        }//end fill tree
                        if (recVtx)fFiltCutsDstoKKpi->CleanOwnPrimaryVtx(ds, aod, origownvtx);
                        if (unsetvtx) ds->UnsetOwnPrimaryVtx();
                    }//end is selected
                }//fid acc
            }
            else {
                fNentries->Fill(18); //monitor how often this fails
            }
        }//end Ds


        //*************************************
        //Dplus
        Bool_t isDplustagged = kTRUE;
        AliAODRecoDecayHF3Prong *dplus = (AliAODRecoDecayHF3Prong*)array3Prong->UncheckedAt(i3prong);
        if (fUseSelectionBit && !(dplus->HasSelectionBit(AliRDHFCuts::kDplusCuts))) {
            isDplustagged = kFALSE;
        }
        if (isDplustagged) {
            nFilteredDplus++;
            fNentries->Fill(19);
            if ((vHF->FillRecoCand(aod, dplus))) { ////Fill the data members of the candidate only if they are empty.

                Bool_t isFidAcc = fFiltCutsDplustoKpipi->IsInFiducialAcceptance(dplus->Pt(), dplus->YDplus());
                if (isFidAcc) { //is fid acc

                    Int_t isSelectedFilt    = fFiltCutsDplustoKpipi->IsSelected(dplus, AliRDHFCuts::kAll, aod);
                    Int_t isSelectedAnalysis = fCutsDplustoKpipi->IsSelected(dplus, AliRDHFCuts::kAll, aod);
                    if (isSelectedFilt) {
                        nSelectedDplus++;

                        Bool_t unsetvtx = kFALSE;
                        if (!dplus->GetOwnPrimaryVtx()) {
                            dplus->SetOwnPrimaryVtx(vtx1);
                            unsetvtx = kTRUE;
                        }
                        Bool_t recVtx = kFALSE;
                        AliAODVertex *origownvtx = 0x0;
                        if (fFiltCutsDplustoKpipi->GetIsPrimaryWithoutDaughters()) {
                            if (dplus->GetOwnPrimaryVtx()) origownvtx = new AliAODVertex(*dplus->GetOwnPrimaryVtx());
                            if (fFiltCutsDplustoKpipi->RecalcOwnPrimaryVtx(dplus, aod))recVtx = kTRUE;
                            else fFiltCutsDplustoKpipi->CleanOwnPrimaryVtx(dplus, aod, origownvtx);
                        }

                        Int_t labDp = -1;
                        Bool_t isPrimary = kFALSE;
                        Bool_t isFeeddown = kFALSE;
                        Int_t pdgCode = -2;
                        //read MC
                        if (fReadMC) {
                            labDp = dplus->MatchToMC(411, arrMC, 3, pdgDgDplustoKpipi);
                            if (labDp >= 0) {
                                AliAODMCParticle *partDp = (AliAODMCParticle*)arrMC->At(labDp);
                                Int_t orig = AliVertexingHFUtils::CheckOrigin(arrMC, partDp, kTRUE); //Prompt = 4, FeedDown = 5
                                pdgCode = TMath::Abs(partDp->GetPdgCode());
                                if (orig == 4) {
                                    isPrimary = kTRUE;
                                    isFeeddown = kFALSE;
                                } else if (orig == 5) {
                                    isPrimary = kFALSE;
                                    isFeeddown = kTRUE;
                                } else {
                                    pdgCode = -3;
                                }
                            } else {
                                pdgCode = -1;
                            }
                        } //end read MC

                        // fill tree
                        if (fWriteVariableTreeDplus == 1) {
                            Bool_t issignal = kFALSE;
                            if (labDp >= 0) issignal = kTRUE;
                            fTreeHandlerDplus->SetIsSelectedStd(isSelectedAnalysis);
                            fTreeHandlerDplus->SetCandidateType(issignal, isPrimary, kFALSE);
                            fTreeHandlerDplus->SetVariables(dplus, 0, pidHFDplus, 0x0);
                            if (fSys > 0) fTreeHandlerDplus->SetCentrality(centrality);
                            fTreeHandlerDplus->FillTree();
                        }//end fill tree

                        if (recVtx)fFiltCutsDplustoKpipi->CleanOwnPrimaryVtx(dplus, aod, origownvtx);
                        if (unsetvtx) dplus->UnsetOwnPrimaryVtx();
                    } //end topol and PID cuts


                }//end is fid acc

            }//end ok fill reco cand
            else {
                fNentries->Fill(21); //monitor how often this fails
            }
        }//end Dplus


    }//end loop on cadidates


    delete vHF;
    return;

}

//-------------------------------------------------------------------------------------
Int_t AliAnalysisTaskSEHFTreeCreator::MatchBPlusCandidateToMonteCarlo(Int_t pdgabs, AliAODRecoDecayHF2Prong * candidate, TClonesArray *mcArray) const
{
  //
  // Check if this candidate is matched to a MC signal
  // If no, return -1
  // If yes, return label (>=0) of the AliAODMCParticle


  // Check number of daughters
  Int_t ndg = candidate->GetNDaughters();
  if (!ndg) { AliError("No daughters available"); return -1;}
  if (ndg != 2) return -1;

  // loop on daughters and write the labels
  Int_t dgLabels[2] = { -1};
  Int_t pdgDg[2] = {0};
  if (pdgabs == 421)
  {
    AliAODTrack *trk0 = (AliAODTrack*)candidate->GetDaughter(0);
    dgLabels[0] = trk0->GetLabel();
    AliAODTrack *trk1 = (AliAODTrack*)candidate->GetDaughter(1);
    dgLabels[1] = trk1->GetLabel();
    pdgDg[0] = 211; pdgDg[1] = 321;
  }
  else if (pdgabs == 521)
  {
    AliAODTrack *trk0 = (AliAODTrack*)candidate->GetDaughter(0);
    dgLabels[0] = trk0->GetLabel();
    dgLabels[1] = MatchBPlusCandidateToMonteCarlo(421, (AliAODRecoDecayHF2Prong*)candidate->GetDaughter(1), mcArray);
    pdgDg[0] = 211; pdgDg[1] = 421;
  }
  else
  {
    std::cout << "Wrong pdg supplied for function to match candidate to monte carlo signal." << std::endl;
    return -1;
  }
  if (dgLabels[0] == -1) return -1;
  if (dgLabels[1] == -1) return -1;


  Int_t labMom[2] = {0, 0};
  Int_t i, j, lab, labMother, pdgMother, pdgPart;
  AliAODMCParticle *part = 0;
  AliAODMCParticle *mother = 0;
  Double_t pxSumDgs = 0., pySumDgs = 0., pzSumDgs = 0.;
  Bool_t pdgUsed[2] = {kFALSE, kFALSE};

  // loop on daughter labels
  for (i = 0; i < ndg; i++)
  {
    labMom[i] = -1;
    lab = TMath::Abs(dgLabels[i]);
    if (lab < 0)
    {
      printf("daughter with negative label %d\n", lab);
      return -1;
    }
    part = (AliAODMCParticle*)mcArray->At(lab);
    if (!part)
    {
      printf("no MC particle\n");
      return -1;
    }

    // check the PDG of the daughter
    pdgPart = TMath::Abs(part->GetPdgCode());
    for (j = 0; j < ndg; j++)
    {
      if (!pdgUsed[j] && pdgPart == pdgDg[j])
      {
        pdgUsed[j] = kTRUE;
        break;
      }
    }


    mother = part;
    while (mother->GetMother() >= 0)
    {
      labMother = mother->GetMother();
      mother = (AliAODMCParticle*)mcArray->At(labMother);
      if (!mother)
      {
        printf("no MC mother particle\n");
        break;
      }
      pdgMother = TMath::Abs(mother->GetPdgCode());
      if (pdgMother == pdgabs)
      {
        labMom[i] = labMother;
        // keep sum of daughters' momenta, to check for mom conservation
        pxSumDgs += part->Px();
        pySumDgs += part->Py();
        pzSumDgs += part->Pz();
        break;
      }
      else break;
    }
    if (labMom[i] == -1) return -1; // mother PDG not ok for this daughter
  } // end loop on daughters

  // check if the candidate is signal
  labMother = labMom[0];
  // all labels have to be the same and !=-1
  for (i = 0; i < ndg; i++)
  {
    if (labMom[i] == -1)        return -1;
    if (labMom[i] != labMother) return -1;
  }

  // check that all daughter PDGs are matched
  for (i = 0; i < ndg; i++)
  {
    if (pdgUsed[i] == kFALSE) return -1;
  }

  // Check for mom conservation
  mother = (AliAODMCParticle*)mcArray->At(labMother);
  Double_t pxMother = mother->Px();
  Double_t pyMother = mother->Py();
  Double_t pzMother = mother->Pz();


  // check the number of daughters (we are not looking at resonant decay)
  if(mother->GetNDaughters() != 2) return -1;

  // if momentum conservation is not within 0.5%, show warning. This can be due to large propagation distance through magnetic field.
  if ((TMath::Abs(pxMother - pxSumDgs) / (TMath::Abs(pxMother) + 1.e-13)) > 0.005 ||
      (TMath::Abs(pyMother - pySumDgs) / (TMath::Abs(pyMother) + 1.e-13)) > 0.005 ||
      (TMath::Abs(pzMother - pzSumDgs) / (TMath::Abs(pzMother) + 1.e-13)) > 0.005)
  {
    std::cout << std::endl << " Momentum difference for decay pdgabs = " << pdgabs << "daughters = " << mother->GetNDaughters() << std::endl;
    std::cout << "pxMother = " << pxMother << "pyMother = " << pyMother << "pzMother = " << pzMother << std::endl;
    std::cout << "pxSumDgs = " << pxSumDgs << "pySumDgs = " << pySumDgs << "pzSumDgs = " << pzSumDgs << std::endl;
  }

  return labMother;
}
//-------------------------------------------------------------------------------------
AliAODVertex* AliAnalysisTaskSEHFTreeCreator::RecalculateVertex(const AliVVertex *primary, TObjArray *tracks, Double_t bField, Double_t dispersion) {
  //
  // Helper function to recalculate a vertex.
  //

  AliESDVertex *vertexESD = 0;
  AliAODVertex *vertexAOD = 0;

  AliVertexerTracks vertexer;
  vertexer.SetFieldkG(bField);

  vertexer.SetVtxStart((AliESDVertex*)primary); //primary vertex
  vertexESD = (AliESDVertex*)vertexer.VertexForSelectedESDTracks(tracks);

  // delete vertexer; vertexer=NULL;

  if (!vertexESD) return vertexAOD;


  if (vertexESD->GetNContributors() != tracks->GetEntriesFast())
  {
    delete vertexESD; vertexESD = nullptr;
    return vertexAOD;
  }

  // convert to AliAODVertex
  Double_t pos[3], cov[6], chi2perNDF;
  for (Int_t a = 0; a < 3; a++)pos[a] = 0.;
  for (Int_t b = 0; b < 6; b++)cov[b] = 0.;
  chi2perNDF = 0;

  vertexESD->GetXYZ(pos); // position
  vertexESD->GetCovMatrix(cov); //covariance matrix


  Double_t vertRadius2 = pos[0] * pos[0] + pos[1] * pos[1];
  if (vertRadius2 > 8.) //(2.82)^2 radius beam pipe
  {
    delete vertexESD; vertexESD = nullptr;
    return vertexAOD;
  }

  chi2perNDF = vertexESD->GetChi2toNDF();
  dispersion = vertexESD->GetDispersion();
  delete vertexESD; vertexESD = nullptr;
  Int_t nprongs = 2; //tracks->GetEntriesFast();
  vertexAOD = new AliAODVertex(pos, cov, chi2perNDF, 0x0, -1, AliAODVertex::kUndef, nprongs);

  return vertexAOD;
}