#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsDstoKKpi.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="DstoKKpiFilteringCuts"
 whichCuts=1, nameCuts="DstoKKpiAnalysisCuts"
 */


AliRDHFCutsDstoKKpi *makeInputCutsDstoKKpi(Int_t whichCuts=0, TString nameCuts="DstoKKpiFilteringCuts", Float_t minc=0.,Float_t maxc=20.)
{
    
    AliRDHFCutsDstoKKpi* cuts=new AliRDHFCutsDstoKKpi();
    cuts->SetName(nameCuts.Data());
    cuts->SetTitle(nameCuts.Data());
    
    AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
    esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
    //default
    esdTrackCuts->SetRequireTPCRefit(kTRUE);
    esdTrackCuts->SetRequireITSRefit(kTRUE);
    if(whichCuts==0)esdTrackCuts->SetMinNClustersTPC(50);
    if(whichCuts==1)esdTrackCuts->SetMinNClustersTPC(70);
    if(whichCuts==1)esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
    esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);
    esdTrackCuts->SetEtaRange(-0.8,0.8);
    esdTrackCuts->SetMinDCAToVertexXY(0.);
    //esdTrackCuts->SetPtRange(0.6,1.e10);
    esdTrackCuts->SetPtRange(0.3,1.e10);
    esdTrackCuts->SetMaxDCAToVertexXY(1.);
    esdTrackCuts->SetMaxDCAToVertexZ(1.);
    esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0060*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    //if(whichCuts==1)esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0060*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    
    cuts->AddTrackCuts(esdTrackCuts);
    
    
    if(whichCuts==0){
        Int_t nptbins=2; Float_t ptlimits[2]={0.,1000000.};
        
        cuts->SetStandardCutsPbPb2010();
        cuts->SetUseTrackSelectionWithFilterBits(kFALSE);
        cuts->SetUsePID(kFALSE);
        Float_t cutsArrayDstoKKpi[20]={0.3,0.3,0.3,0.,0.,0.,0.06,0.02,0.,0.9,0.,100000.,0.03,0.0001,-1.,1.,0.,0.,0.,-1.};
        cuts->SetPtBins(nptbins,ptlimits);
        cuts->SetCuts(20,cutsArrayDstoKKpi);
        cuts->AddTrackCuts(esdTrackCuts);
        cuts->SetMinPtCandidate(2.);
    }
    
    else if(whichCuts==1){
        
        const Int_t nptbins=4;
        Float_t* ptbins;
        ptbins=new Float_t[nptbins+1];
        ptbins[0]=4.;
        ptbins[1]=6.;
        ptbins[2]=8.;
        ptbins[3]=12.;
        ptbins[4]=16.;
        
        const Int_t nvars=20;
        
        Float_t** anacutsval;
        anacutsval=new Float_t*[nvars];
        
        for(Int_t ic=0;ic<nvars;ic++){anacutsval[ic]=new Float_t[nptbins];}
        for(Int_t ipt=0;ipt<nptbins;ipt++){
            
            
            anacutsval[0][ipt]=0.2;
            anacutsval[1][ipt]=0.3;
            anacutsval[2][ipt]=0.3;
            anacutsval[3][ipt]=0.;
            anacutsval[4][ipt]=0.;
            anacutsval[5][ipt]=0.005;
            anacutsval[6][ipt]=0.06;
            anacutsval[7][ipt]=0.02;
            anacutsval[8][ipt]=0.;
            anacutsval[9][ipt]=0.94;
            anacutsval[10][ipt]=0.;
            anacutsval[11][ipt]=1000.0;
            anacutsval[12][ipt]=0.015;
            anacutsval[13][ipt]=0.001;
            anacutsval[14][ipt]=0.;
            anacutsval[15][ipt]=1.;
            anacutsval[16][ipt]=0.;
            anacutsval[17][ipt]=0.;
            anacutsval[18][ipt]=0.;
            anacutsval[19][ipt]=0.94;
            
        }
        
        anacutsval[6][0]=0.025;   //sigmavert
        anacutsval[7][0]=0.05;   //decay length
        anacutsval[9][0]=0.995;   //cosP
        anacutsval[12][0]=0.006; //Mass Phi
        anacutsval[14][0]=0.2;  //Abs(CosineKpiPhiRFrame)^3
        anacutsval[15][0]=0.8;  //CosP labFrame
        anacutsval[16][0]=0.05;  //decayXY
        anacutsval[17][0]=0.;    //normdecay
        anacutsval[18][0]=9.0;   //normdecayXY
        anacutsval[19][0]=0.995;  //CosPXY
        anacutsval[1][0]=0.9;
        anacutsval[2][0]=0.9;
        
        anacutsval[6][1]=0.030;   //sigmavert
        anacutsval[7][1]=0.05;   //decay length
        anacutsval[9][1]=0.99;   //cosP
        anacutsval[12][1]=0.006; //Mass Phi
        anacutsval[14][1]=0.1;  //Abs(CosineKpiPhiRFrame)^3
        anacutsval[15][1]=1.0;  //CosP labFrame
        anacutsval[16][1]=0.05;  //decayXY
        anacutsval[17][1]=0.;    //normdecay
        anacutsval[18][1]=9.0;   //normdecayXY
        anacutsval[19][1]=0.99;  //CosPXY
        
        anacutsval[6][2]=0.025;   //sigmavert
        anacutsval[7][2]=0.04;   //decay length
        anacutsval[9][2]=0.98;   //cosP
        anacutsval[12][2]=0.004; //Mass Phi
        anacutsval[14][2]=0.2;   //Abs(CosineKpiPhiRFrame)^3
        anacutsval[15][2]=0.8;   //CosP labFrame
        anacutsval[16][2]=0.04;  //decayXY
        anacutsval[17][2]=0.;    //normdecay
        anacutsval[18][2]=6.0;   //normdecayXY
        anacutsval[19][2]=0.98;  //CosPXY
        
        anacutsval[6][3]=0.025;   //sigmavert
        anacutsval[7][3]=0.04;   //decay length
        anacutsval[9][3]=0.99;   //cosP
        anacutsval[12][3]=0.004; //Mass Phi
        anacutsval[14][3]=0.1;   //Abs(CosineKpiPhiRFrame)^3
        anacutsval[15][3]=0.9;  //CosP labFrame
        anacutsval[16][3]=0.04;   //decayXY
        anacutsval[17][3]=0.;    //normdecay
        anacutsval[18][3]=8.0;   //normdecayXY
        anacutsval[19][3]=0.99;  //CosPXY
        
        Float_t topomCuts[nptbins] = {1.,2,2,2.5}; //topomatic
        
        cuts->SetPtBins(nptbins+1,ptbins);
        cuts->SetCuts(nvars,nptbins,anacutsval);
        cuts->AddTrackCuts(esdTrackCuts);
        cuts->Setd0MeasMinusExpCut(nptbins,topomCuts);
        
        cuts->SetUsePID(kTRUE);
        cuts->SetPidOption(1); //0=kConservative,1=kStrong
        
        cuts->SetMinPtCandidate(4.);
        cuts->SetMaxPtCandidate(16.);
        cuts->SetMaxPtStrongPid(9999.);
        
        //Do not recalculate the vertex
        cuts->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
        
    }
    //event selection
    cuts->SetTriggerClass("");
    cuts->SetTriggerMask(AliVEvent::kINT7);
    cuts->SetMinCentrality(minc);
    cuts->SetMaxCentrality(maxc);
    cuts->SetUseCentrality(AliRDHFCuts::kCentV0M); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,kCentInvalid
    cuts->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
    cuts->SetMaxVtxZ(10.);
    cuts->SetCutOnzVertexSPD(3);
    
    cuts->SetName(nameCuts.Data());
    cuts->SetTitle(nameCuts.Data());
    cout<<"This is the object I'm going to save:"<<endl;
    cuts->PrintAll();
    
    return cuts;
    
}


