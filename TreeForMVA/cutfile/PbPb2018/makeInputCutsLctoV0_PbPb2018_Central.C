#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsLctoV0.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="LctoV0FilteringCuts"
 whichCuts=1, nameCuts="LctoV0AnalysisCuts"
 */

AliRDHFCutsLctoV0 *makeInputCutsLctoV0(Int_t whichCuts=0, TString nameCuts="LctoV0FilteringCuts", Float_t minc=0.,Float_t maxc=20.)
{
    
    AliRDHFCutsLctoV0* cutsLctoV0=new AliRDHFCutsLctoV0();
    cutsLctoV0->SetName(nameCuts.Data());
    cutsLctoV0->SetTitle(nameCuts.Data());

    AliESDtrackCuts *esdTrackCuts = new AliESDtrackCuts("AliESDtrackCuts","default");
    esdTrackCuts->SetRequireTPCRefit(kTRUE);
    if(whichCuts==0)esdTrackCuts->SetMinNClustersTPC(50);
    esdTrackCuts->SetRequireITSRefit(kTRUE);
    //esdTrackCuts->SetMinNClustersITS(4);
    esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kAny);
    // |d0|>25 micron for pt<2GeV, no cut above 2
    if(whichCuts==1)esdTrackCuts->SetMinDCAToVertexXY(0.);
    if(whichCuts==0)esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    if(whichCuts==1)esdTrackCuts->SetMinDCAToVertexXYPtDep("0.005*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    esdTrackCuts->SetMaxDCAToVertexXY(1.);
    esdTrackCuts->SetMaxDCAToVertexZ(1.);
    esdTrackCuts->SetPtRange(0.4,1.e10);
    esdTrackCuts->SetEtaRange(-0.8,+0.8);
    cutsLctoV0->AddTrackCuts(esdTrackCuts);
    
    if(whichCuts==0){
        const Int_t nptbinsLctoV0=1;
        Float_t ptlimitsLctoV0[nptbinsLctoV0+1]={0.,1000000.};
        Float_t** cutsArrayLctoV0;
        cutsArrayLctoV0=new Float_t*[21];
        for(Int_t iv=0;iv<21;iv++){
            cutsArrayLctoV0[iv]=new Float_t[nptbinsLctoV0];
        }
       
	cutsArrayLctoV0[0][0]=0.2;    // inv. mass if K0S [GeV/c2]
        cutsArrayLctoV0[1][0]=0.;    // inv. mass if Lambda [GeV/c2]
        cutsArrayLctoV0[2][0]=0.05;  // inv. mass V0 if K0S [GeV/c2]
        cutsArrayLctoV0[3][0]=0.05;  // inv. mass V0 if Lambda [GeV/c2]
        cutsArrayLctoV0[4][0]=0.5;   // pT min bachelor track [GeV/c] // AOD by construction
        cutsArrayLctoV0[5][0]=0.;    // pT min V0-positive track [GeV/c]
        cutsArrayLctoV0[6][0]=0.;    // pT min V0-negative track [GeV/c]
        cutsArrayLctoV0[7][0]=1000.; // dca cascade cut [cm]
        cutsArrayLctoV0[8][0]=1000.;   // dca V0 cut [nSigma] // it's 1.5 x offline V0s
        cutsArrayLctoV0[9][0]=0.99;   // cosPA V0 cut // it's 0.90 x offline V0s at reconstruction level, 0.99 at filtering level
        cutsArrayLctoV0[10][0]=3.;   // d0 max bachelor wrt PV [cm]
        cutsArrayLctoV0[11][0]=1000.;// d0 max V0 wrt PV [cm]
        cutsArrayLctoV0[12][0]=0.;   // mass K0S veto [GeV/c2]
        cutsArrayLctoV0[13][0]=0.;   // mass Lambda/LambdaBar veto [GeV/c2]
        cutsArrayLctoV0[14][0]=0.;   // mass Gamma veto [GeV/c2]
        cutsArrayLctoV0[15][0]=0.5;   // pT min V0 track [GeV/c]
        cutsArrayLctoV0[16][0]=9999.;// max cos (Proton emission angle) cut 
        cutsArrayLctoV0[17][0]=-9999.;// min cos (Proton emission angle) cut 
        cutsArrayLctoV0[18][0]=-9999.;// Re-signed d0 [cm]
        cutsArrayLctoV0[19][0]=-9999.;// V0 armenteros qT/|alpha|
        cutsArrayLctoV0[20][0]=1.;   // V0 type cut

        cutsLctoV0->SetStandardCutsPbPb2010();
        cutsLctoV0->SetMinPtCandidate(0.);
        cutsLctoV0->SetUsePID(kFALSE);
        cutsLctoV0->SetUseTrackSelectionWithFilterBits(kFALSE);
        //cutsLctoV0->SetUseSpecialCuts(kFALSE);
        //cutsLctoV0->SetUsePhysicsSelection(kFALSE);
        //cutsLctoV0->SetMaxCentrality(101.);//90.);
        //cutsLctoV0->SetMaxVtxZ(1.e6);
        cutsLctoV0->SetPtBins(nptbinsLctoV0+1,ptlimitsLctoV0);
        cutsLctoV0->SetCuts(21,nptbinsLctoV0,cutsArrayLctoV0);

        Bool_t pidflag=kTRUE;
        cutsLctoV0->SetUsePID(pidflag);
        if(pidflag) cout<<"PID is used"<<endl;
        else cout<<"PID is not used"<<endl;
        
        //pid settings
        AliAODPidHF* pidObj=new AliAODPidHF();
        //pidObj->SetName("pid4LctoV0");;
	//const Int_t nlims=2;
        //Double_t plims[nlims]={0.6,0.8}; //TPC limits in momentum [GeV/c]
        //Bool_t compat=kTRUE; //effective only for this mode
        //Bool_t asym=kTRUE;
        Double_t sigmas[5]={2.,1.,0.,3.,0.}; //to be checked and to be modified with new implementation of setters by Rossella
        pidObj->SetAsym(kFALSE);// if you want to use the asymmetric bands in TPC
        pidObj->SetMatch(1);
        //pidObj->SetPLimit(plims,nlims);
        pidObj->SetSigma(sigmas);
        //pidObj->SetCompat(compat);
        pidObj->SetTPC(kTRUE);
        pidObj->SetTOF(kTRUE);

	AliESDtrackCuts *esdV0daughterTrackCuts = new AliESDtrackCuts("AliESDtrackCutsForV0D","default cuts for V0 daughters");
        esdV0daughterTrackCuts->SetRequireTPCRefit(kTRUE);
        esdV0daughterTrackCuts->SetMinNClustersTPC(30);
        esdV0daughterTrackCuts->SetRequireITSRefit(kFALSE);
        esdV0daughterTrackCuts->SetMinDCAToVertexXY(0.);
        esdV0daughterTrackCuts->SetPtRange(0.05,1.e10);
        esdV0daughterTrackCuts->SetEtaRange(-1.1,+1.1);
        esdV0daughterTrackCuts->SetAcceptKinkDaughters(kTRUE);
        esdV0daughterTrackCuts->SetRequireSigmaToVertex(kFALSE);
        cutsLctoV0->AddTrackCutsV0daughters(esdV0daughterTrackCuts);
//        vHF->SetCutsLctoV0(cutsLctoV0);
      

        //pidObj->SetPCompatTOF(2.);
        //pidObj->SetSigmaForTPCCompat(3.);
        //pidObj->SetSigmaForTOFCompat(3.);
        
        //pidObj->SetOldPid(kFALSE);
        
        //cutsLctoV0->SetLowPt(kFALSE);
        //cutsLctoV0->SetUseDefaultPID(kFALSE); //to use the AliAODPidHF
        
        cutsLctoV0->SetPidHF(pidObj);
    }
    else if(whichCuts==1){
        
        
    }
    
    //event selection
    cutsLctoV0->SetTriggerClass("");
    cutsLctoV0->SetTriggerMask(AliVEvent::kINT7 | AliVEvent::kCentral);
    cutsLctoV0->SetMinCentrality(minc);
    cutsLctoV0->SetMaxCentrality(maxc);
    cutsLctoV0->SetUseCentrality(AliRDHFCuts::kCentV0M); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,kCentInvalid
    cutsLctoV0->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
    cutsLctoV0->SetMaxVtxZ(10.);
    cutsLctoV0->SetCutOnzVertexSPD(3);
    
    cout<<"This is the object I'm going to save:"<<endl;
    cutsLctoV0->SetName(nameCuts.Data());
    cutsLctoV0->SetTitle(nameCuts.Data());
    cutsLctoV0->PrintAll();
    
    return cutsLctoV0;
    
}


