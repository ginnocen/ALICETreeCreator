#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsLctoV0.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="LctoV0FilteringCuts"
 whichCuts=1, nameCuts="LctoV0AnalysisCuts"
 */

const Int_t nvars=21;
//        Lc inv. mass if K0S [GeV/c2]
//        Lc inv. mass if Lambda [GeV/c2]
//        K0S inv. mass [GeV/c2]
//        Lambda/LambdaBar inv. mass[GeV/c2]
//        pT min bachelor track [GeV/c]
//        pT min V0-positive track [GeV/c]
//        pT min V0-negative track [GeV/c]
//        dca cascade (prong-to-prong) cut [cm]
//        dca V0 (prong-to-prong) cut [number of sigmas]
//        V0 cosPA min wrt PV
//        d0 max bachelor wrt PV [cm]
//        d0 max V0 wrt PV [cm]
//        mass K0S veto [GeV/c2]
//        mass Lambda/LambdaBar veto [GeV/c2]
//        mass Gamma veto [GeV/c2]
//        pT min V0 track [GeV/c]
//        Max Proton emission angle in Lc CMS
//        Min Proton emission angle in Lc CMS
//        Resigned d0
//        V0 qT/|alpha|
//        V0 type

AliRDHFCutsLctoV0 *makeInputCutsLctoV0(Int_t whichCuts=0, TString nameCuts="LctoV0FilteringCuts", Float_t minc=0.,Float_t maxc=100.)
{
  
  AliRDHFCutsLctoV0* cutsLctoV0=new AliRDHFCutsLctoV0();
  cutsLctoV0->SetName(nameCuts.Data());
  cutsLctoV0->SetTitle(nameCuts.Data());
  
  //UPDATE 21/06/19, use the same track quality cuts for filtering and analysis cuts
  //single track cuts
  AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
  esdTrackCuts->SetRequireTPCRefit(kTRUE);
  esdTrackCuts->SetRequireITSRefit(kTRUE);
  //Should not use SetMinNClustersTPC anymore, not well described in MC
  //Two lines below replace this cut (for value 70)
  //  esdTrackCuts->SetMinNClustersTPC(70);
  esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
  esdTrackCuts->SetMinNCrossedRowsTPC(70);
  esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,
                                         AliESDtrackCuts::kAny);
  esdTrackCuts->SetMaxDCAToVertexXY(1.e6);
  esdTrackCuts->SetMinDCAToVertexXY(0.);
  esdTrackCuts->SetMinDCAToVertexZ(0.);
  esdTrackCuts->SetPtRange(0.3,1.e10);
  esdTrackCuts->SetEtaRange(-0.8,+0.8);
  esdTrackCuts->SetAcceptKinkDaughters(kFALSE);
  
  AliESDtrackCuts *esdTrackCutsV0daughters = new AliESDtrackCuts("AliESDtrackCutsForV0D","default cuts for V0 daughters");
  esdTrackCutsV0daughters->SetRequireTPCRefit(kTRUE);
  esdTrackCutsV0daughters->SetRequireITSRefit(kFALSE);
  //Should not use SetMinNClustersTPC anymore, not well described in MC
  //Two lines below replace this cut (for value 70)
  //  esdTrackCuts->SetMinNClustersTPC(70);
  esdTrackCutsV0daughters->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
  esdTrackCutsV0daughters->SetMinNCrossedRowsTPC(70);
  esdTrackCutsV0daughters->SetMinDCAToVertexXY(0.);
  esdTrackCutsV0daughters->SetMinDCAToVertexZ(0.);
  esdTrackCutsV0daughters->SetPtRange(0.1,1.e10);
  esdTrackCutsV0daughters->SetEtaRange(-0.8,+0.8);
  esdTrackCutsV0daughters->SetRequireSigmaToVertex(kFALSE);
  
  cutsLctoV0->SetKinkRejection(!esdTrackCuts->GetAcceptKinkDaughters());
  cutsLctoV0->AddTrackCuts(esdTrackCuts);
  cutsLctoV0->AddTrackCutsV0daughters(esdTrackCutsV0daughters);
  cutsLctoV0->SetUseTrackSelectionWithFilterBits(kFALSE);
  
  if(whichCuts==0){
    
    const Int_t nptbins=2;
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    ptbins[0]=0.;
    ptbins[1]=5.;
    ptbins[2]=999.;
    cutsLctoV0->SetPtBins(nptbins+1,ptbins);
    Float_t cuts[nptbins][nvars]={
      0.2,0.,0.03,0.05,0.0,0.0,0.0,1000.,0.8,0.997,3.,1.5,0.,0.,0.,0.,0.9,-0.9,-0.8,1.8,0.0,
      0.2,0.,0.03,0.05,0.0,0.0,0.0,1000.,0.8,0.997,3.,1.5,0.,0.,0.,0.,0.9,-0.9,-0.8,1.8,0.0};
    Float_t** prodcutsval;
    prodcutsval=new Float_t*[nvars];
    for(Int_t ic=0;ic<nvars;ic++){prodcutsval[ic]=new Float_t[nptbins];}
    for(Int_t ipt=0; ipt<nptbins; ipt++){
      for(Int_t ic=0; ic<nvars; ic++){
        prodcutsval[ic][ipt]=cuts[ipt][ic];
      }
    }
    
    cutsLctoV0->SetMinPtCandidate(1.);
    cutsLctoV0->SetCuts(nvars,nptbins,prodcutsval);
    
    //UPDATE 21/06/19, enable PID selection to reduce output size
    //UPDATE 03/10/19, set 4 sigma instead of 3 to select more signal
    cutsLctoV0->SetPidSelectionFlag(11);
    //pid settings
    //1. bachelor: default one
    AliAODPidHF* pidObjBachelor = new AliAODPidHF();
    Double_t sigmasBac[5]={4.,4.,4.,4.,4.}; // 0, 1(A), 2(A) -> TPC; 3 -> TOF; 4 -> ITS
    pidObjBachelor->SetSigma(sigmasBac);
    pidObjBachelor->SetAsym(kFALSE);
    pidObjBachelor->SetMatch(1);
    pidObjBachelor->SetTPC(kTRUE);
    pidObjBachelor->SetTOF(kTRUE);
    pidObjBachelor->SetTOFdecide(kFALSE);
    
    cutsLctoV0->SetPidHF(pidObjBachelor);
    Bool_t pidflag=kTRUE;
    cutsLctoV0->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for filtering cuts"<<endl;
    else cout<<"PID is not used for filtering cuts"<<endl;

  } else if(whichCuts==1){
    //UPDATE 25/03/20, use the same topological cuts as STD 13TeV analysis of Luigi.
    //Note: PID selection is also applied at filtering level. PID_filt + PID_an might be != PID_an
    
    const Int_t nptbins=6;
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    ptbins[0]= 1.;
    ptbins[1]= 2.;
    ptbins[2]= 4.;
    ptbins[3]= 6.;
    ptbins[4]= 8.;
    ptbins[5]= 12.;
    ptbins[6]= 24.;
    cutsLctoV0->SetPtBins(nptbins+1,ptbins);
    
    Float_t** anacutsval;
    anacutsval=new Float_t*[nvars];
    for(Int_t ic=0;ic<nvars;ic++){anacutsval[ic]=new Float_t[nptbins];}
    for(Int_t ipt2=0;ipt2<nptbins;ipt2++){
      anacutsval[0][ipt2]=0.25;   // inv. mass if K0S [GeV/c2]
      anacutsval[1][ipt2]=1.00;   // inv. mass if Lambda [GeV/c2]
      anacutsval[3][ipt2]=0.05;   // inv. mass V0 if Lambda [GeV/c2] ---> WE ARE SEARCHING Lc -> p+K0S, so cut on m(Lambda) has to be leave as it was at filtering level!!!
      anacutsval[7][ipt2]=1000.;  // dca cascade cut [cm]
      anacutsval[8][ipt2]=1.5;    // dca V0 cut [nSigma] // it's 1.5 x offline V0s
      anacutsval[9][ipt2]=0.99;   // cosPA V0 cut // it's 0.90 x offline V0s at reconstruction level, 0.99 at filtering level
      anacutsval[11][ipt2]=999.;  // d0 max V0 wrt PV [cm]
      anacutsval[12][ipt2]=0.;    // mass K0S veto [GeV/c2]
      anacutsval[13][ipt2]=0.005; // mass Lambda/LambdaBar veto [GeV/c2]
      anacutsval[14][ipt2]=0.100; // mass Gamma veto [GeV/c2]
      anacutsval[16][ipt2]=9999;  // max cos proton emission angle in Lc CMS max
      anacutsval[17][ipt2]=-9999; // min cos proton emission angle in Lc CMS max
      anacutsval[18][ipt2]=-9999; // Re-signed d0 min [cm]
      anacutsval[19][ipt2]=-9999; // V0 qT/|alpha| min
      anacutsval[20][ipt2]=0.;    // V0 type cut
    }

    anacutsval[2][0]=0.008; // inv. mass V0 if K0S [GeV/c2]
    anacutsval[2][1]=0.008; // inv. mass V0 if K0S [GeV/c2]
    anacutsval[2][2]=0.008; // inv. mass V0 if K0S [GeV/c2]
    anacutsval[2][3]=0.010; // inv. mass V0 if K0S [GeV/c2]
    anacutsval[2][4]=0.015; // inv. mass V0 if K0S [GeV/c2]
    anacutsval[2][5]=0.020; // inv. mass V0 if K0S [GeV/c2]

    anacutsval[10][0]=0.04; // d0 max bachelor wrt PV [cm]
    anacutsval[10][1]=0.06; // d0 max bachelor wrt PV [cm]
    anacutsval[10][2]=0.08; // d0 max bachelor wrt PV [cm]
    anacutsval[10][3]=0.09; // d0 max bachelor wrt PV [cm]
    anacutsval[10][4]=0.10; // d0 max bachelor wrt PV [cm]
    anacutsval[10][5]=0.20; // d0 max bachelor wrt PV [cm]
    
    anacutsval[4][0]=0.4; // pT min bachelor track [GeV/c] // AOD by construction
    anacutsval[4][1]=0.5; // pT min bachelor track [GeV/c] // AOD by construction
    anacutsval[4][2]=0.8; // pT min bachelor track [GeV/c] // AOD by construction
    anacutsval[4][3]=1.3; // pT min bachelor track [GeV/c] // AOD by construction
    anacutsval[4][4]=2.0; // pT min bachelor track [GeV/c] // AOD by construction
    anacutsval[4][5]=2.5; // pT min bachelor track [GeV/c] // AOD by construction

    anacutsval[5][0]=0.25; // pT min V0-positive track [GeV/c]
    anacutsval[5][1]=0.3;  // pT min V0-positive track [GeV/c]
    anacutsval[5][2]=0.3;  // pT min V0-positive track [GeV/c]
    anacutsval[5][3]=0.4;  // pT min V0-positive track [GeV/c]
    anacutsval[5][4]=0.4;  // pT min V0-positive track [GeV/c]
    anacutsval[5][5]=0.4;  // pT min V0-positive track [GeV/c]

    anacutsval[6][0]=0.25; // pT min V0-negative track [GeV/c]
    anacutsval[6][1]=0.3;  // pT min V0-negative track [GeV/c]
    anacutsval[6][2]=0.3;  // pT min V0-negative track [GeV/c]
    anacutsval[6][3]=0.4;  // pT min V0-negative track [GeV/c]
    anacutsval[6][4]=0.4;  // pT min V0-negative track [GeV/c]
    anacutsval[6][5]=0.4;  // pT min V0-negative track [GeV/c]

    anacutsval[15][0]=0.4; // pT min V0 track [GeV/c]
    anacutsval[15][1]=0.8; // pT min V0 track [GeV/c]
    anacutsval[15][2]=1.3; // pT min V0 track [GeV/c]
    anacutsval[15][3]=1.5; // pT min V0 track [GeV/c]
    anacutsval[15][4]=1.9; // pT min V0 track [GeV/c]
    anacutsval[15][5]=2.5; // pT min V0 track [GeV/c]

    cutsLctoV0->SetMinPtCandidate(1.);
    cutsLctoV0->SetMaxPtCandidate(24.);
    cutsLctoV0->SetCuts(nvars,nptbins,anacutsval);
    
    //pid settings
    cutsLctoV0->SetPidSelectionFlag(9); // 0 -> TOF AND TPC
                                        // 1 -> if (TOF) TOF else TPC w veto
                                        // 2 -> if (p<1) TPC@2s else if (1<=p<2.5) {if (TOF) TOF@3s AND TPC@3s} else (p>=2.5) {if (TOF) -2s<TOF<3s AND TPC@3s}
                                        // 3 -> if (p<1) TPC@2s else if (1<=p<2.5) {if (TOF) TOF@3s AND TPC@3s} else (p>=2.5) {if (TOF) -2s<TOF<3s AND -3s<TPC<2s}
                                        // 4 -> if (p<1) TPC@2s else if (1<=p<2.5) {if (TOF) TOF@3s} else if (p>=2.5) {if (TOF) -2s<TOF<3s}
                                        // 5 -> if (p<1) TPC@2s else if (1<=p<2.5) {if (TOF) TOF@3s} else if (p>=2.5) {if (TOF) -2s<TOF<3s else TPC@3s}
                                        // 6 -> if (p<1) TPC@2s else if (1<=p<2.5) {if (TOF) TOF@3s} else if (p>=2.5) {if (TOF) -2s<TOF<3s else -3s<TPC<2s}
                                        // 7 -> if (p<1) TPC@2s else if (1<=p<2.5) {if (TOF) TOF@3s else TPC@3s} else (p>=2.5) {if (TOF) -2s<TOF<3s else TPC@3s}
                                        // 8 -> if (p<1) TPC@2s else if (1<=p<2.5) {if (TOF) TOF@3s else TPC@3s} else (p>=2.5) {if (TOF) -2s<TOF<3s else -2<TPC<3s}
                                        // 9 -> combined: TOF, TPC
    cutsLctoV0->SetGetNTPCSigmaCutForPreselection(5);
    const Int_t nBachelorPbins=3;
    if(cutsLctoV0->GetPidSelectionFlag()==9){
      Float_t minCombProb[nBachelorPbins];
      minCombProb[0]=0.8;
      minCombProb[1]=0.2;
      minCombProb[2]=0.2;
      cutsLctoV0->SetMinCombinedProbability(nBachelorPbins,minCombProb);
      Float_t bachelorPlimits[nBachelorPbins];
      bachelorPlimits[0]=0.;
      bachelorPlimits[1]=4.;
      bachelorPlimits[2]=999.;
      cutsLctoV0->SetBachelorPLimitsForPID(nBachelorPbins,bachelorPlimits);
    }
    
    //pid settings
    //1. bachelor: default one
    AliAODPidHF* pidObjBachelor = new AliAODPidHF();
    Double_t sigmasBac[5]={3.,1.,1.,3.,3.}; // 0, 1(A), 2(A) -> TPC; 3 -> TOF; 4 -> ITS
    pidObjBachelor->SetSigma(sigmasBac);
    pidObjBachelor->SetAsym(kFALSE);
    pidObjBachelor->SetMatch(1);
    pidObjBachelor->SetTPC(kTRUE);
    pidObjBachelor->SetTOF(kTRUE);
    pidObjBachelor->SetTOFdecide(kFALSE);
    if(cutsLctoV0->GetPidSelectionFlag()==9){
      pidObjBachelor->SetUseCombined();
      pidObjBachelor->SetUseDefaultPriors(kTRUE);
      pidObjBachelor->SetCombDetectors(AliAODPidHF::kTPCTOF);
    }
    
    cutsLctoV0->SetPidHF(pidObjBachelor);
    Bool_t pidflag=kTRUE;
    cutsLctoV0->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for analysis cuts"<<endl;
    else cout<<"PID is not used for analysis cuts"<<endl;
  }
  
  //11/08/19:
  //Not enabled for now because it is missing in AliRDHFCuts
  //Also not sure if this is used for Lc in pp
  cutsLctoV0->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
  
  //event selection
  cutsLctoV0->SetUsePhysicsSelection(kTRUE);
  cutsLctoV0->SetTriggerClass("");
  //Since X/08/19 we set the triggers in wagon code/runAnalysis script if needed
  cutsLctoV0->SetTriggerMask(AliVEvent::kAny);
  cutsLctoV0->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent);
  cutsLctoV0->SetMinContribPileupMV(5);
  cutsLctoV0->SetMaxVtxZ(10.);
  cutsLctoV0->SetCutOnzVertexSPD(3);
  cutsLctoV0->SetMinVtxContr(1);
  
  cout<<"This is the object I'm going to save:"<<endl;
  cutsLctoV0->SetName(nameCuts.Data());
  cutsLctoV0->SetTitle(nameCuts.Data());
  cutsLctoV0->PrintAll();
  
  return cutsLctoV0;
  
}
