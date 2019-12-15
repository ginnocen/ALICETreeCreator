#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsDplustoKpipi.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="DplustoKpipiFilteringCuts"
 whichCuts=1, nameCuts="DplustoKpipiAnalysisCuts"
 */

AliRDHFCutsDplustoKpipi *makeInputCutsDplustoKpipi_pp(Int_t whichCuts=0, TString nameCuts="DplustoKpipiFilteringCuts", Float_t minc=0.,Float_t maxc=100.)
{
  
  AliRDHFCutsDplustoKpipi* cuts=new AliRDHFCutsDplustoKpipi();
  cuts->SetName(nameCuts.Data());
  cuts->SetTitle(nameCuts.Data());
  
  //UPDATE 21/06/19, use the same track quality cuts for filtering and analysis cuts
  AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
  esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
  //default
  esdTrackCuts->SetRequireTPCRefit(kTRUE);
  esdTrackCuts->SetRequireITSRefit(kTRUE);
  //Should not use SetMinNClustersTPC anymore, not well described in MC
  //Two lines below replace this cut (for value 70)
  //  esdTrackCuts->SetMinNClustersTPC(70);
  esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
  esdTrackCuts->SetMinNCrossedRowsTPC(70);
  esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);
  esdTrackCuts->SetEtaRange(-0.8,0.8);
  esdTrackCuts->SetMinDCAToVertexXY(0.);
  esdTrackCuts->SetPtRange(0.3,1.e10);
  cuts->AddTrackCuts(esdTrackCuts);
  
  if(whichCuts==0){
    Int_t nptbins=2;
    Float_t ptlimits[2]={0.,1000000.};
    Float_t cutsArrayDplustoKpipi[14]={0.2,0.3,0.3,0.,0.,0.01,0.06,0.02,0.,0.7,0.,10000000000.,0.,-1.};
    
    cuts->SetMinPtCandidate(0.);
    cuts->SetPtBins(nptbins,ptlimits);
    cuts->SetCuts(14,cutsArrayDplustoKpipi);
    
    //UPDATE 21/06/19, enable PID selection to reduce output size
    AliAODPidHF* pidObj=new AliAODPidHF();
    Double_t sigmasBac[5]={3.,3.,3.,3.,3.}; // 0, 1(A), 2(A) -> TPC; 3 -> TOF; 4 -> ITS
    pidObj->SetSigma(sigmasBac);
    pidObj->SetAsym(kFALSE);
    pidObj->SetMatch(1);
    pidObj->SetTPC(kTRUE);
    pidObj->SetTOF(kTRUE);
    
    cuts->SetPidHF(pidObj);
    Bool_t pidflag=kTRUE;
    cuts->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for filtering cuts"<<endl;
    else cout<<"PID is not used for filtering cuts"<<endl;
  } else if(whichCuts==1){
    
    const Int_t nptbins=11;
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    
    ptbins[0]=0.;
    ptbins[1]=1.;
    ptbins[2]=1.5;
    ptbins[3]=2.;
    ptbins[4]=3.;
    ptbins[5]=4.;
    ptbins[6]=8.;
    ptbins[7]=10.;
    ptbins[8]=12.;
    ptbins[9]=24.;
    ptbins[10]=36.;
    ptbins[11]=50.;
    
    const Int_t nvars=14;
    Float_t** anacutsval;
    anacutsval=new Float_t*[nvars];
    for(Int_t ic=0;ic<nvars;ic++){anacutsval[ic]=new Float_t[nptbins];}
    
    Int_t ic=0;//minv
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.2;
    }
    
    ic=1;//ptK
    anacutsval[ic][0]=0.4;//0.0-1.0
    anacutsval[ic][1]=0.4;//1.0-1.5
    anacutsval[ic][2]=0.4;//1.5-2.0
    anacutsval[ic][3]=0.3;//2.0-3.0
    anacutsval[ic][4]=0.3;//3.0-4.0
    anacutsval[ic][5]=0.3;//4.0-8.0
    anacutsval[ic][6]=0.3;//8.0-10.0
    anacutsval[ic][7]=0.3;//10.0-12.0
    anacutsval[ic][8]=0.3;//12.0-24.0
    anacutsval[ic][9]=0.3;//24.0-36.0
    anacutsval[ic][10]=0.3;//36.0-50.0
    
    ic=2;//ptPi
    anacutsval[ic][0]=0.4;//0.0-1.0
    anacutsval[ic][1]=0.4;//1.0-1.5
    anacutsval[ic][2]=0.4;//1.5-2.0
    anacutsval[ic][3]=0.3;//2.0-3.0
    anacutsval[ic][4]=0.3;//3.0-4.0
    anacutsval[ic][5]=0.3;//4.0-8.0
    anacutsval[ic][6]=0.3;//8.0-10.0
    anacutsval[ic][7]=0.3;//10.0-12.0
    anacutsval[ic][8]=0.3;//12.0-24.0
    anacutsval[ic][9]=0.3;//24.0-36.0
    anacutsval[ic][10]=0.3;//36.0-50.0
    
    ic=3;//d0K
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.;
    }
    ic=4;//d0Pi
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.;
    }
    ic=5;//dist12
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.01;
    }
    
    ic=6;//sigvert
    anacutsval[ic][0]=0.030;//0.0-1.0
    anacutsval[ic][1]=0.030;//1.0-1.5
    anacutsval[ic][2]=0.030;//1.5-2.0
    anacutsval[ic][3]=0.030;//2.0-3.0
    anacutsval[ic][4]=0.030;//3.0-4.0
    anacutsval[ic][5]=0.030;//4.0-8.0
    anacutsval[ic][6]=0.030;//8.0-10.0
    anacutsval[ic][7]=0.027;//10.0-12.0
    anacutsval[ic][8]=0.030;//12.0-24.0
    anacutsval[ic][9]=0.030;//24.0-36.0
    anacutsval[ic][10]=0.030;//36.0-50.0
    
    ic=7;//declen
    anacutsval[ic][0]=0.07;//0.0-1.0
    anacutsval[ic][1]=0.07;//1.0-1.5
    anacutsval[ic][2]=0.07;//1.5-2.0
    anacutsval[ic][3]=0.07;//2.0-3.0
    anacutsval[ic][4]=0.10;//3.0-4.0
    anacutsval[ic][5]=0.10;//4.0-8.0
    anacutsval[ic][6]=0.12;//8.0-10.0
    anacutsval[ic][7]=0.12;//10.0-12.0
    anacutsval[ic][8]=0.12;//12.0-24.0
    anacutsval[ic][9]=0.12;//24.0-36.0
    anacutsval[ic][10]=0.12;//36.0-50.0
    
    ic=8;//pM
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.0;
    }
    
    //cosp
    ic=9;
    anacutsval[ic][0]=0.97;//0.0-1.0
    anacutsval[ic][1]=0.97;//1.0-1.5
    anacutsval[ic][2]=0.97;//1.5-2.0
    anacutsval[ic][3]=0.96;//2.0-3.0
    anacutsval[ic][4]=0.96;//3.0-4.0
    anacutsval[ic][5]=0.96;//4.0-8.0
    anacutsval[ic][6]=0.96;//8.0-10.0
    anacutsval[ic][7]=0.96;//10.0-12.0
    anacutsval[ic][8]=0.94;//12.0-24.0
    anacutsval[ic][9]=0.94;//24.0-36.0
    anacutsval[ic][10]=0.94;//36.0-50.0
    
    ic=10;//sumd02
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.0;
    }
    
    ic=11;//dca
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=10000000000.;
    }
    
    ic=12;//ndlXY
    anacutsval[ic][0]=7.;//0.0-1.0
    anacutsval[ic][1]=7.;//1.0-1.5
    anacutsval[ic][2]=7.;//1.5-2.0
    anacutsval[ic][3]=5.;//2.0-3.0
    anacutsval[ic][4]=5.;//3.0-4.0
    anacutsval[ic][5]=5.;//4.0-8.0
    anacutsval[ic][6]=5.;//8.0-10.0
    anacutsval[ic][7]=5.;//10.0-12.0
    anacutsval[ic][8]=5.;//12.0-24.0
    anacutsval[ic][9]=5.;//24.0-36.0
    anacutsval[ic][10]=5.;//36.0-50.0
    
    ic=13;//cospXY
    anacutsval[ic][0]=0.985;//0.0-1.0
    anacutsval[ic][1]=0.985;//1.0-1.5
    anacutsval[ic][2]=0.985;//1.5-2.0
    anacutsval[ic][3]=0.98;//2.0-3.0
    anacutsval[ic][4]=0.98;//3.0-4.0
    anacutsval[ic][5]=0.;//4.0-8.0
    anacutsval[ic][6]=0.;//8.0-10.0
    anacutsval[ic][7]=0.;//10.0-12.0
    anacutsval[ic][8]=0.;//12.0-24.0
    anacutsval[ic][9]=0.;//24.0-36.0
    anacutsval[ic][10]=0.;//36.0-50.0
    
    Float_t *d0d0expcutsval=new Float_t[nptbins];
    d0d0expcutsval[0]=1.5;//0.0-1.0
    d0d0expcutsval[1]=1.5;//1.0-1.5
    d0d0expcutsval[2]=2.5;//1.5-2.0
    d0d0expcutsval[3]=2.5;//2.0-3.0
    d0d0expcutsval[4]=2.5;//3.0-4.0
    d0d0expcutsval[5]=2.5;//4.0-8.0
    d0d0expcutsval[6]=2.5;//8.0-10.0
    d0d0expcutsval[7]=2.5;//10.0-12.0
    d0d0expcutsval[8]=2.5;//12.0-24.0
    d0d0expcutsval[9]=2.5;//24.0-36.0
    d0d0expcutsval[10]=2.5;//36.0-50.0
    
    cuts->SetMinPtCandidate(0.);
    cuts->SetMaxPtCandidate(50.);
    cuts->SetPtBins(nptbins+1,ptbins);
    cuts->SetCuts(nvars,nptbins,anacutsval);
    cuts->Setd0MeasMinusExpCut(nptbins,d0d0expcutsval);
    cuts->SetScaleNormDLxyBypOverPt(kFALSE);
    cuts->SetUseImpParProdCorrCut(kFALSE);
    
    Bool_t pidflag=kTRUE;
    cuts->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for analysis cuts"<<endl;
    else cout<<"PID is not used for analysis cuts"<<endl;
  }
  
  cuts->SetRemoveDaughtersFromPrim(kTRUE); //activate for pp
  
  //event selection
  cuts->SetUsePhysicsSelection(kTRUE);
  cuts->SetTriggerClass("");
  //Since X/08/19 we set the triggers in wagon code/runAnalysis script if needed
  cuts->SetTriggerMask(AliVEvent::kAny);
  cuts->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent);
  cuts->SetMinContribPileupMV(5);
  cuts->SetMaxVtxZ(10.);
  cuts->SetCutOnzVertexSPD(3);
  cuts->SetMinVtxContr(1);
  
  cout<<"This is the object I'm going to save:"<<endl;
  cuts->SetName(nameCuts.Data());
  cuts->SetTitle(nameCuts.Data());
  cuts->PrintAll();
  
  return cuts;
}
