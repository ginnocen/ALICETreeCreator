#include <Riostream.h>
#include <TFile.h>
#include "AliRDHFCutsBPlustoD0Pi.h"
#include <TClonesArray.h>
#include <TParameter.h>
#include <ctime>
#include <time.h>
#include <TSystem.h>

void setFilterBplusCuts(AliRDHFCutsBPlustoD0Pi* RDHFBPlustoD0Pi);
void setAnalysisBplusCuts(AliRDHFCutsBPlustoD0Pi* RDHFBPlustoD0Pi);

/*
 whichCuts=0, nameCuts="D0toKpiFilteringCuts"
 whichCuts=1, nameCuts="D0toKpiAnalysisCuts"
 */

AliRDHFCutsBPlustoD0Pi * makeInputCutsBplustoD0pi(Int_t whichCuts=0, TString nameCuts="BplustoD0piFilteringCuts", Float_t minc=0.,Float_t maxc=100.){
    
    AliRDHFCutsBPlustoD0Pi* RDHFBPlustoD0Pi = new AliRDHFCutsBPlustoD0Pi();
    RDHFBPlustoD0Pi->SetName(nameCuts.Data());
    RDHFBPlustoD0Pi->SetTitle(nameCuts.Data());
    
    AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
    esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
    esdTrackCuts->SetRequireTPCRefit(kTRUE);
    esdTrackCuts->SetRequireITSRefit(kTRUE);
    if(whichCuts==0)esdTrackCuts->SetMinNClustersTPC(50);
    if(whichCuts==1)esdTrackCuts->SetMinNClustersTPC(70);
    esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kAny);
    esdTrackCuts->SetEtaRange(-0.8,0.8);
    esdTrackCuts->SetMinDCAToVertexXY(0.);
    esdTrackCuts->SetPtRange(0.5,1.e10);
    esdTrackCuts->SetMaxDCAToVertexXY(1.);
    esdTrackCuts->SetMaxDCAToVertexZ(1.);
    if(whichCuts==1)esdTrackCuts->SetMinDCAToVertexXY(0.);
    if(whichCuts==0)esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    if(whichCuts==1)esdTrackCuts->SetMinDCAToVertexXYPtDep("0.005*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");

    AliESDtrackCuts* esdTrackCutsBplusPion=new AliESDtrackCuts();
    esdTrackCutsBplusPion->SetRequireSigmaToVertex(kFALSE);
    esdTrackCutsBplusPion->SetRequireTPCRefit(kTRUE);
    esdTrackCutsBplusPion->SetRequireITSRefit(kTRUE);
    if(whichCuts==0)esdTrackCutsBplusPion->SetMinNClustersTPC(50);
    if(whichCuts==1)esdTrackCutsBplusPion->SetMinNClustersTPC(70);
    esdTrackCutsBplusPion->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kAny);
    esdTrackCutsBplusPion->SetEtaRange(-0.8,0.8);
    esdTrackCutsBplusPion->SetMinDCAToVertexXY(0.);
    esdTrackCutsBplusPion->SetPtRange(0.5,1.e10);
    esdTrackCutsBplusPion->SetMaxDCAToVertexXY(1.);
    esdTrackCutsBplusPion->SetMaxDCAToVertexZ(1.);
    if(whichCuts==1)esdTrackCutsBplusPion->SetMinDCAToVertexXY(0.);
    if(whichCuts==0)esdTrackCutsBplusPion->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    if(whichCuts==1)esdTrackCutsBplusPion->SetMinDCAToVertexXYPtDep("0.005*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    
    RDHFBPlustoD0Pi->AddTrackCuts(esdTrackCuts);
    RDHFBPlustoD0Pi->AddTrackCutsSoftPi(esdTrackCutsBplusPion);

    if(whichCuts==0){
        //Bplus pt bins
        const Int_t nptbinsBplus=1;
        Float_t* ptlimitsBplus;
        ptlimitsBplus=new Float_t[nptbinsBplus+1];
        ptlimitsBplus[0]=0.;
        ptlimitsBplus[1]=1000000.;
        RDHFBPlustoD0Pi->SetPtBins(nptbinsBplus+1,ptlimitsBplus);
        
        // D0 with D0 pt bins
        const Int_t nptbinsD0forD0ptbin=1;
        Float_t* ptbinsD0forD0ptbin;
        ptbinsD0forD0ptbin =new Float_t[nptbinsD0forD0ptbin+1];
        ptbinsD0forD0ptbin[0]=0.;
        ptbinsD0forD0ptbin[1]=9999999.;
        RDHFBPlustoD0Pi->SetNPtBinsD0forD0ptbin(nptbinsD0forD0ptbin);
        RDHFBPlustoD0Pi->SetPtBinsD0forD0ptbin(nptbinsD0forD0ptbin+1,ptbinsD0forD0ptbin);
        
        //Ds sets cuts->SetStandardCutsPbPb2010();, TODO: Check what is set
        
        RDHFBPlustoD0Pi->SetUsePID(kFALSE);
        RDHFBPlustoD0Pi->SetMinPtCandidate(ptlimitsBplus[0]);
        RDHFBPlustoD0Pi->SetUseTrackSelectionWithFilterBits(kFALSE);
        
        setFilterBplusCuts(RDHFBPlustoD0Pi);
    } else if(whichCuts==1){
        
        //main BPlus pt bins
        const Int_t nptbins=31;
        Float_t* ptbins;
        ptbins=new Float_t[nptbins+1];
        ptbins[0]=0.;
        ptbins[1]=1.;
        ptbins[2]=2.;
        ptbins[3]=3.;
        ptbins[4]=4.;
        ptbins[5]=5.;
        ptbins[6]=6.;
        ptbins[7]=7.;
        ptbins[8]=8.;
        ptbins[9]=9.;
        ptbins[10]=10.;
        ptbins[11]=11.;
        ptbins[12]=12.;
        ptbins[13]=13.;
        ptbins[14]=14.;
        ptbins[15]=15.;
        ptbins[16]=16.;
        ptbins[17]=17.;
        ptbins[18]=18.;
        ptbins[19]=19.;
        ptbins[20]=20.;
        ptbins[21]=21.;
        ptbins[22]=22.;
        ptbins[23]=23.;
        ptbins[24]=24.;
        ptbins[25]=25.;
        ptbins[26]=26.;
        ptbins[27]=27.;
        ptbins[28]=28.;
        ptbins[29]=29.;
        ptbins[30]=30.;
        ptbins[31]=9999999.;
        RDHFBPlustoD0Pi->SetPtBins(nptbins+1,ptbins);
        
        // D0 with D0 pt bins
        const Int_t nptbinsD0forD0ptbin=1;
        Float_t* ptbinsD0forD0ptbin;
        ptbinsD0forD0ptbin =new Float_t[nptbinsD0forD0ptbin+1];
        ptbinsD0forD0ptbin[0]=0.;
        ptbinsD0forD0ptbin[1]=9999999.;
        RDHFBPlustoD0Pi->SetNPtBinsD0forD0ptbin(nptbinsD0forD0ptbin);
        RDHFBPlustoD0Pi->SetPtBinsD0forD0ptbin(nptbinsD0forD0ptbin+1,ptbinsD0forD0ptbin);
        
        RDHFBPlustoD0Pi->SetUsePID(kTRUE);
        RDHFBPlustoD0Pi->SetMinPtCandidate(ptbins[0]);
        RDHFBPlustoD0Pi->SetMaxPtCandidate(ptbins[nptbins]);
        
        setAnalysisBplusCuts(RDHFBPlustoD0Pi);
    }
    
    //event selection
    RDHFBPlustoD0Pi->SetTriggerClass("");
    RDHFBPlustoD0Pi->SetTriggerMask(AliVEvent::kINT7);
    RDHFBPlustoD0Pi->SetMinCentrality(minc);
    RDHFBPlustoD0Pi->SetMaxCentrality(maxc);
    RDHFBPlustoD0Pi->SetUseCentrality(AliRDHFCuts::kCentV0M); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,kCentInvalid
    RDHFBPlustoD0Pi->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
    RDHFBPlustoD0Pi->SetMaxVtxZ(10.);
    RDHFBPlustoD0Pi->SetCutOnzVertexSPD(3);
    
    cout<<"This is the object I'm going to save:"<<endl;
    RDHFBPlustoD0Pi->SetName(nameCuts.Data());
    RDHFBPlustoD0Pi->SetTitle(nameCuts.Data());
    RDHFBPlustoD0Pi->PrintAll();
    
    return RDHFBPlustoD0Pi;
}

void setFilterBplusCuts(AliRDHFCutsBPlustoD0Pi* RDHFBPlustoD0Pi){
    //Partly changed Lennarts filtering cuts (commented ones), to be discussed with him
    //For now same as *_pp.C

    //---------------------------------------------------------------
    //
    //  SET BASIC DAUGHTER CUTS
    //
    // --------------------------------------------------------------
    
    RDHFBPlustoD0Pi->SetMaxAbsEtaD0FirstDaughter(0.8);
    Bool_t bHardSelectionArrayITSD0FirstDaughter[7] = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    RDHFBPlustoD0Pi->SetHardSelectionArrayITSD0FirstDaughter(bHardSelectionArrayITSD0FirstDaughter);
    Bool_t bSoftSelectionArrayITSD0FirstDaughter[7] = {kTRUE,kTRUE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    RDHFBPlustoD0Pi->SetSoftSelectionArrayITSD0FirstDaughter(bSoftSelectionArrayITSD0FirstDaughter);
    RDHFBPlustoD0Pi->SetNSoftITSCutD0FirstDaughter(1);
    
    //RDHFBPlustoD0Pi->SetMinITSNclsD0FirstDaughter(2);
    RDHFBPlustoD0Pi->SetMinTPCNclsD0FirstDaughter(50);//70);
    RDHFBPlustoD0Pi->SetUseITSRefitD0FirstDaughter(kTRUE);
    RDHFBPlustoD0Pi->SetUseTPCRefitD0FirstDaughter(kTRUE);
    RDHFBPlustoD0Pi->SetUseFilterBitD0FirstDaughter(kFALSE);
    RDHFBPlustoD0Pi->SetFilterBitD0FirstDaughter(0);
    RDHFBPlustoD0Pi->SetMinPtD0FirstDaughter(0.3);
    
    RDHFBPlustoD0Pi->SetMaxAbsEtaD0SecondDaughter(0.8);
    Bool_t bHardSelectionArrayITSD0SecondDaughter[7] = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    RDHFBPlustoD0Pi->SetHardSelectionArrayITSD0SecondDaughter(bHardSelectionArrayITSD0SecondDaughter);
    Bool_t bSoftSelectionArrayITSD0SecondDaughter[7] = {kTRUE,kTRUE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    RDHFBPlustoD0Pi->SetSoftSelectionArrayITSD0SecondDaughter(bSoftSelectionArrayITSD0SecondDaughter);
    RDHFBPlustoD0Pi->SetNSoftITSCutD0SecondDaughter(1);
    
    //RDHFBPlustoD0Pi->SetMinITSNclsD0SecondDaughter(2);
    RDHFBPlustoD0Pi->SetMinTPCNclsD0SecondDaughter(50);//70);
    RDHFBPlustoD0Pi->SetUseITSRefitD0SecondDaughter(kTRUE);
    RDHFBPlustoD0Pi->SetUseTPCRefitD0SecondDaughter(kTRUE);
    RDHFBPlustoD0Pi->SetUseFilterBitD0SecondDaughter(kFALSE);
    RDHFBPlustoD0Pi->SetFilterBitD0SecondDaughter(0);
    RDHFBPlustoD0Pi->SetMinPtD0SecondDaughter(0.3);
    
    RDHFBPlustoD0Pi->SetMaxAbsEtaBPlusPion(0.8);
    Bool_t bHardSelectionArrayITSBPlusPion[7] = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    RDHFBPlustoD0Pi->SetHardSelectionArrayITSBPlusPion(bHardSelectionArrayITSBPlusPion);
    Bool_t bSoftSelectionArrayITSBPlusPion[7] = {kTRUE,kTRUE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    RDHFBPlustoD0Pi->SetSoftSelectionArrayITSBPlusPion(bSoftSelectionArrayITSBPlusPion);
    RDHFBPlustoD0Pi->SetNSoftITSCutBPlusPion(1);
    
    //RDHFBPlustoD0Pi->SetMinITSNclsBPlusPion(2);
    RDHFBPlustoD0Pi->SetMinTPCNclsBPlusPion(50);//70);
    RDHFBPlustoD0Pi->SetUseITSRefitBPlusPion(kTRUE);
    RDHFBPlustoD0Pi->SetUseTPCRefitBPlusPion(kTRUE);
    RDHFBPlustoD0Pi->SetUseFilterBitBPlusPion(kFALSE);
    RDHFBPlustoD0Pi->SetFilterBitBPlusPion(0);
    RDHFBPlustoD0Pi->SetMinPtBPlusPion(0.3);
    
    //RDHFBPlustoD0Pi->SetMind0D0FirstDaughter(0.0); //is off, default=0
    //RDHFBPlustoD0Pi->SetMind0D0SecondDaughter(0.0); //is off, default=0
    //RDHFBPlustoD0Pi->SetMind0BPlusPion(0.0); //is off, default=0
    
    //---------------------------------------------------------------
    //
    //  SET CUTS
    //
    // --------------------------------------------------------------
    
    // we initialize the cuts by creating a cut array of which all cuts are set to zero and set to unused.
    RDHFBPlustoD0Pi->InitializeCuts();
    Int_t nptbins = RDHFBPlustoD0Pi->GetNPtBins();
    Int_t nptbinsD0forD0ptbin = RDHFBPlustoD0Pi->GetNPtBinsD0forD0ptbin();
    // We set cuts for all candidates
    // These can have a large effect on the CPU time of the analysis
    
    for (int i = 0; i < nptbins; ++i){
        RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.06); // 71 - "inv. mass width[GeV] D0";
        //Needed to delete the hardcoded invariant mass window
        RDHFBPlustoD0Pi->SetCut(39, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 1.1);  // 39 - "inv. mass width[GeV]" (Bplus);
        //        RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.92); // 70 - "pointing angle [Cos(theta)]";
        //        RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.05); // 71 - "dca [cm]";
        //        RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.02); // 75 - "d0 BPlus [cm]";
        //        RDHFBPlustoD0Pi->SetCut(49, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0); // 78 - "d0d0 [cm^2]";
        //        RDHFBPlustoD0Pi->SetCut(56, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 85 - "pseudoProperDecayTime";
        //        RDHFBPlustoD0Pi->SetCut(64, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.96); // 93 - "pointing angle XY [Cos(theta)]";
    }
    
    // We set cuts for all D0 candidates
    // These can have a large effect on the CPU time of the analysis
    
    for (int i = 0; i < nptbinsD0forD0ptbin; ++i){
        //A bit looser (0.07 vs 0.06) than when D0/D0bar issue is solved, to be on the safe side.
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.07); // 00 - "inv. mass width[GeV]" (D0);
        //        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(2, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.7);  // 02 - "pointing angle [Cos(theta)]";
        //        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(3, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.02); // 03 - "dca [cm]";
        //        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(4, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 2.5);  // 04 - "Pt D0 [GeV/c]";
        //        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(12, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.5);  // 12 "angle between both daughters";
        //        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(17, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.0);  // 17 - "pseudoProperDecayTime";
    }
    //TODO: decide wrt to what D0 is doing for filtering:
    // 00 - "inv. mass width[GeV]"         --> 0.4 instead of 0.7
    // 02 - "pointing angle [Cos(theta)]"; --> 0. instead of 0.7
    // 03 - "dca [cm]";                    --> 999999. instead of 0.02
    // 04 - "Pt D0 [GeV/c]";               --> 0. instead of 2.5
    // 12 "angle between both daughters";  --> 0. instead of 0.5
    // 17 - "pseudoProperDecayTime";       --> No cut instead of 0.0 (can be negative?)
    
    Bool_t pidflag=kFALSE;//kTRUE;
    RDHFBPlustoD0Pi->SetUsePID(pidflag);
    
    //AliAODPidHF* pidObj = new AliAODPidHF();
    //Int_t mode=1;
    //Double_t priors[5]={0.01,0.001,0.3,0.3,0.3};
    //pidObj->SetPriors(priors,5);
    //pidObj->SetMatch(mode);
    //pidObj->SetSigma(0,3); // TPC
    //pidObj->SetSigma(3,3); // TOF
    //pidObj->SetTPC(kTRUE);
    //pidObj->SetTOF(kTRUE);
    //pidObj->SetMaxTrackMomForCombinedPID(3.);
    //RDHFBPlustoD0Pi->SetPidHF(pidObj);
    
    //RDHFBPlustoD0Pi->SetOffHighPtPIDinTPC(4);
    
    return;
}

void setAnalysisBplusCuts(AliRDHFCutsBPlustoD0Pi* RDHFBPlustoD0Pi){
    //Cuts coming from makeTFile4CutsBPlustoD0Pi_Cut_0024.C
    //For now same as *_pp.C
    
    //---------------------------------------------------------------
    //
    //  SET BASIC DAUGHTER CUTS
    //
    // --------------------------------------------------------------
    
    RDHFBPlustoD0Pi->SetMaxAbsEtaD0FirstDaughter(0.8);
    Bool_t bHardSelectionArrayITSD0FirstDaughter[7] = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    RDHFBPlustoD0Pi->SetHardSelectionArrayITSD0FirstDaughter(bHardSelectionArrayITSD0FirstDaughter);
    Bool_t bSoftSelectionArrayITSD0FirstDaughter[7] = {kTRUE,kTRUE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    RDHFBPlustoD0Pi->SetSoftSelectionArrayITSD0FirstDaughter(bSoftSelectionArrayITSD0FirstDaughter);
    RDHFBPlustoD0Pi->SetNSoftITSCutD0FirstDaughter(1);
    
    RDHFBPlustoD0Pi->SetMinITSNclsD0FirstDaughter(2);
    RDHFBPlustoD0Pi->SetMinTPCNclsD0FirstDaughter(70);
    RDHFBPlustoD0Pi->SetUseITSRefitD0FirstDaughter(kTRUE);
    RDHFBPlustoD0Pi->SetUseTPCRefitD0FirstDaughter(kTRUE);
    RDHFBPlustoD0Pi->SetUseFilterBitD0FirstDaughter(kFALSE);
    RDHFBPlustoD0Pi->SetFilterBitD0FirstDaughter(0);
    RDHFBPlustoD0Pi->SetMinPtD0FirstDaughter(1.0);
    
    RDHFBPlustoD0Pi->SetMaxAbsEtaD0SecondDaughter(0.8);
    Bool_t bHardSelectionArrayITSD0SecondDaughter[7] = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    RDHFBPlustoD0Pi->SetHardSelectionArrayITSD0SecondDaughter(bHardSelectionArrayITSD0SecondDaughter);
    Bool_t bSoftSelectionArrayITSD0SecondDaughter[7] = {kTRUE,kTRUE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    RDHFBPlustoD0Pi->SetSoftSelectionArrayITSD0SecondDaughter(bSoftSelectionArrayITSD0SecondDaughter);
    RDHFBPlustoD0Pi->SetNSoftITSCutD0SecondDaughter(1);
    
    RDHFBPlustoD0Pi->SetMinITSNclsD0SecondDaughter(2);
    RDHFBPlustoD0Pi->SetMinTPCNclsD0SecondDaughter(70);
    RDHFBPlustoD0Pi->SetUseITSRefitD0SecondDaughter(kTRUE);
    RDHFBPlustoD0Pi->SetUseTPCRefitD0SecondDaughter(kTRUE);
    RDHFBPlustoD0Pi->SetUseFilterBitD0SecondDaughter(kFALSE);
    RDHFBPlustoD0Pi->SetFilterBitD0SecondDaughter(0);
    RDHFBPlustoD0Pi->SetMinPtD0SecondDaughter(1.0);
    
    RDHFBPlustoD0Pi->SetMaxAbsEtaBPlusPion(0.8);
    Bool_t bHardSelectionArrayITSBPlusPion[7] = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    RDHFBPlustoD0Pi->SetHardSelectionArrayITSBPlusPion(bHardSelectionArrayITSBPlusPion);
    Bool_t bSoftSelectionArrayITSBPlusPion[7] = {kTRUE,kTRUE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    RDHFBPlustoD0Pi->SetSoftSelectionArrayITSBPlusPion(bSoftSelectionArrayITSBPlusPion);
    RDHFBPlustoD0Pi->SetNSoftITSCutBPlusPion(1);
    
    RDHFBPlustoD0Pi->SetMinITSNclsBPlusPion(2);
    RDHFBPlustoD0Pi->SetMinTPCNclsBPlusPion(70);
    RDHFBPlustoD0Pi->SetUseITSRefitBPlusPion(kTRUE);
    RDHFBPlustoD0Pi->SetUseTPCRefitBPlusPion(kTRUE);
    RDHFBPlustoD0Pi->SetUseFilterBitBPlusPion(kFALSE);
    RDHFBPlustoD0Pi->SetFilterBitBPlusPion(0);
    RDHFBPlustoD0Pi->SetMinPtBPlusPion(1.0);
    
    RDHFBPlustoD0Pi->SetMind0D0FirstDaughter(0.002);
    RDHFBPlustoD0Pi->SetMind0D0SecondDaughter(0.002);
    RDHFBPlustoD0Pi->SetMind0BPlusPion(0.006);
    
    //---------------------------------------------------------------
    //
    //  SET CUTS
    //
    // --------------------------------------------------------------
    
    // we initialize the cuts by creating a cut array of which all cuts are set to zero and set to unused.
    RDHFBPlustoD0Pi->InitializeCuts();
    Int_t nptbins = RDHFBPlustoD0Pi->GetNPtBins();
    Int_t nptbinsD0forD0ptbin = RDHFBPlustoD0Pi->GetNPtBinsD0forD0ptbin();
    
    // We set cuts for all candidates
    // These can have a large effect on the CPU time of the analysis
    
    for (int i = 0; i < nptbins; ++i){
        RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.92); // 70 - "pointing angle [Cos(theta)]";
        RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.015); // 75 - "d0 BPlus [cm]";
        RDHFBPlustoD0Pi->SetCut(49, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0); // 78 - "d0d0 [cm^2]";
        
        RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.01); // 84 - "vertexDistance";
        RDHFBPlustoD0Pi->SetCut(56, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0); // 85 - "pseudoProperDecayTime";
        RDHFBPlustoD0Pi->SetCut(59, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 2.0); // 88 - "normDecayLength";
        
        RDHFBPlustoD0Pi->SetCut(64, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.99); // 93 - "pointing angle XY [Cos(theta)]";
    }
    for (int i = 0; i < 3; ++i){
        RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.04); // 71 - "inv. mass width[GeV] D0";
        RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
        RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";
        RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.006); // 71 - "d0 D0 [cm]";
        RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 first daughter D0";
        RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 second daughter D0";
        
        RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.92); // 70 - "pointing angle [Cos(theta)]";
        RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.03); // 71 - "dca [cm]";
        RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
        RDHFBPlustoD0Pi->SetCut(59, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 3.5); // 88 - "normDecayLength";
    }
    for (int i = 3; i < 6; ++i){
        RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.045); // 71 - "inv. mass width[GeV] D0";
        RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
        RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";
        RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.006); // 71 - "d0 D0 [cm]";
        RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 first daughter D0";
        RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 second daughter D0";
        RDHFBPlustoD0Pi->SetCut(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.07); // 84 - "vertexDistance D0";
        
        
        RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.92); // 70 - "pointing angle [Cos(theta)]";
        RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.025); // 71 - "dca [cm]";
        RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
        RDHFBPlustoD0Pi->SetCut(59, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 3.5); // 88 - "normDecayLength";
    }
    for (int i = 6; i < 10; ++i){
        RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.05); // 71 - "inv. mass width[GeV] D0";
        RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
        RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";
        RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.006); // 71 - "d0 D0 [cm]";
        RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 first daughter D0";
        RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 second daughter D0";
        RDHFBPlustoD0Pi->SetCut(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.07); // 84 - "vertexDistance D0";
        
        
        RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.94); // 70 - "pointing angle [Cos(theta)]";
        RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.025); // 71 - "dca [cm]";
        RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
        RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistance";
        RDHFBPlustoD0Pi->SetCut(59, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 3.5); // 88 - "normDecayLength";
        RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistanceXY";
    }
    for (int i = 10; i < 18; ++i){
        RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.06); // 71 - "inv. mass width[GeV] D0";
        RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
        RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";
        RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.004); // 71 - "d0 D0 [cm]";
        RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 first daughter D0";
        RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 71 - "d0 second daughter D0";
        RDHFBPlustoD0Pi->SetCut(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.07); // 84 - "vertexDistance D0";
        
        
        RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.94); // 70 - "pointing angle [Cos(theta)]";
        RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.02); // 71 - "dca [cm]";
        RDHFBPlustoD0Pi->SetCut(45, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.5); // 84 - "pt pion B0";
        RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
        RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistance";
        RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistanceXY";
    }
    for (int i = 18; i < 30; ++i){
        RDHFBPlustoD0Pi->SetCut(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.07); // 71 - "inv. mass width[GeV] D0";
        RDHFBPlustoD0Pi->SetCut(2, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.94); // 71 - "pointing angle [Cos(theta)] D0";
        RDHFBPlustoD0Pi->SetCut(5, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt first daughter D0";
        RDHFBPlustoD0Pi->SetCut(6, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.0); // 71 - "pt second daughter D0";
        RDHFBPlustoD0Pi->SetCut(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.004); // 71 - "d0 D0 [cm]";
        RDHFBPlustoD0Pi->SetCut(8, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.004); // 71 - "d0 first daughter D0";
        RDHFBPlustoD0Pi->SetCut(9, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.004); // 71 - "d0 second daughter D0";
        RDHFBPlustoD0Pi->SetCut(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.07); // 84 - "vertexDistance D0";
        
        
        RDHFBPlustoD0Pi->SetCut(41, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.98); // 70 - "pointing angle [Cos(theta)]";
        RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.02); // 71 - "dca [cm]";
        RDHFBPlustoD0Pi->SetCut(45, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 1.5); // 84 - "pt pion B0";
        RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.01); // 75 - "d0 BPlus [cm]";
        RDHFBPlustoD0Pi->SetCut(55, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistance";
        RDHFBPlustoD0Pi->SetCut(65, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.03); // 84 - "vertexDistanceXY";
    }
    for (int i = 30; i < 31; ++i){
        RDHFBPlustoD0Pi->SetCut(42, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.05); // 71 - "dca [cm]";
        RDHFBPlustoD0Pi->SetCut(46, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.03); // 75 - "d0 BPlus [cm]";
    }
    
    // We set cuts for all D0 candidates
    // These can have a large effect on the CPU time of the analysis
    
    for (int i = 0; i < nptbinsD0forD0ptbin; ++i)
    {
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(0, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.07);   // 00 - "inv. mass width[GeV]";
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(2, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.8);    // 02 - "pointing angle [Cos(theta)]";
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(3, i, AliRDHFCutsBPlustoD0Pi::kCutAboveValue, 0.02);   // 03 - "dca [cm]";
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(4, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 2.5);    // 04 - "Pt D0 [GeV/c]";
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(7, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.002); // 07 - "d0 D0 [cm]";
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(12, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.005);    // 11 "d0d0 XY [cm^2]";
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(12, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.5);    // 12 "angle between both daughters";
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(16, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.05);   // 16 - "vertexDistance";
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(17, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.0);    // 17 - "pseudoProperDecayTime";
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(18, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.1e-9);    // 18 - "DecayTime";
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(26, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 0.05);    // 26 - "vertexDistanceXY";
        RDHFBPlustoD0Pi->SetCutD0forD0ptbin(27, i, AliRDHFCutsBPlustoD0Pi::kCutBelowValue, 4.0);    // 27 - "normDecayLength XY";
    }
    
    Bool_t pidflag=kTRUE;
    RDHFBPlustoD0Pi->SetUsePID(pidflag);
    
    AliAODPidHF* pidObj = new AliAODPidHF();
    Int_t mode=1;
    Double_t priors[5]={0.01,0.001,0.3,0.3,0.3};
    pidObj->SetPriors(priors,5);
    pidObj->SetMatch(mode);
    pidObj->SetSigma(0,3); // TPC
    pidObj->SetSigma(3,3); // TOF
    pidObj->SetTPC(kTRUE);
    pidObj->SetTOF(kTRUE);
    pidObj->SetMaxTrackMomForCombinedPID(3.);
    RDHFBPlustoD0Pi->SetPidHF(pidObj);
    
    RDHFBPlustoD0Pi->SetOffHighPtPIDinTPC(4);
    
    return;
}
