#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsDstoKKpi.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="DstoKKpiFilteringCuts"
 whichCuts=1, nameCuts="DstoKKpiAnalysisCuts"
 */

AliRDHFCutsDstoKKpi *makeInputCutsDstoKKpi_pp(Int_t whichCuts=0, TString nameCuts="DstoKKpiFilteringCuts", Float_t minc=0.,Float_t maxc=100.)
{
  
  AliRDHFCutsDstoKKpi* cuts=new AliRDHFCutsDstoKKpi();
  cuts->SetName(nameCuts.Data());
  cuts->SetTitle(nameCuts.Data());
  
  //UPDATE 21/06/19, use the same track quality cuts for filtering and analysis cuts
  //single track cuts
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
  esdTrackCuts->SetMaxDCAToVertexXY(1.e6);
  cuts->AddTrackCuts(esdTrackCuts);
  cuts->SetUseTrackSelectionWithFilterBits();
  
  if(whichCuts==0){
    
    Int_t nptbins=2;
    Float_t ptlimits[2]={1.,1000000.};
    Float_t cutsArrayDstoKKpi[20]={0.3,0.3,0.3,0.,0.,0.005,0.06,0.,0.,0.7,0.,1000.,0.02,0.1,-1.,1.,0.,0.,0.,-1.};
    //Reduce output possibilities even more:
    //  - cosThetaPoint 0.7 -> 0.85 for pT<5
    //  - PID 4sigma to 3sigma
    //  - Turn on decLen (0.02)
    
    cuts->SetMinPtCandidate(0.);
    cuts->SetPtBins(nptbins,ptlimits);
    cuts->SetCuts(20,cutsArrayDstoKKpi);
    
    //UPDATE 21/06/19, enable PID selection to reduce output size
    //UPDATE 03/10/19, set 4 sigma instead of 3 to select more signal
    AliAODPidHF* pidObj=new AliAODPidHF();
    Double_t sigmasBac[5]={4.,4.,4.,4.,4.}; // 0, 1(A), 2(A) -> TPC; 3 -> TOF; 4 -> ITS
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
    
  }
  else if(whichCuts==1){
    
    const Int_t nptbins=10;
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    ptbins[0]=1.;
    ptbins[1]=2.;
    ptbins[2]=3.;
    ptbins[3]=4.;
    ptbins[4]=5.;
    ptbins[5]=6.;
    ptbins[6]=8.;
    ptbins[7]=10.;
    ptbins[8]=12.;
    ptbins[9]=16.;
    ptbins[10]=24.;
    
    const Int_t nvars=20;
    
    Float_t** anacutsval;
    anacutsval=new Float_t*[nvars];
    
    for(Int_t ic=0;ic<nvars;ic++){anacutsval[ic]=new Float_t[nptbins];}
    
    //0 inv. mass [GeV]
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[0][ipt]=0.35; }
    //1 pTK [GeV/c]
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[1][ipt]=0.3; }
    //2 pTPi [GeV/c]
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[2][ipt]=0.3; }
    //3 d0K [cm]
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[3][ipt]=0.; }
    //4 d0pi [cm]
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[4][ipt]=0.; }
    //5 dist12 [cm]
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[5][ipt]=0.; }
    //6 sigmavert [cm]
    anacutsval[6][0]=0.03; //1-2
    anacutsval[6][1]=0.03; //2-3
    anacutsval[6][2]=0.03; //3-4
    anacutsval[6][3]=0.04; //4-5
    anacutsval[6][4]=0.04; //5-6
    anacutsval[6][5]=0.04; //6-8
    anacutsval[6][6]=0.04; //8-10
    anacutsval[6][7]=0.06; //10-12
    anacutsval[6][8]=0.06; //12-16
    anacutsval[6][9]=0.06; //16-24
    //7 decLen [cm]",
    anacutsval[7][0]=0.02;
    anacutsval[7][1]=0.02;
    anacutsval[7][2]=0.02;
    anacutsval[7][3]=0.03;
    anacutsval[7][4]=0.03;
    anacutsval[7][5]=0.03;
    anacutsval[7][6]=0.03;
    anacutsval[7][7]=0.03;
    anacutsval[7][8]=0.05;
    anacutsval[7][9]=0.05;
    //8 ptMax [GeV/c]
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[8][ipt]=0.; }
    //9 cosThetaPoint
    anacutsval[9][0]=0.92;
    anacutsval[9][1]=0.92;
    anacutsval[9][2]=0.92;
    anacutsval[9][3]=0.90;
    anacutsval[9][4]=0.90;
    anacutsval[9][5]=0.90;
    anacutsval[9][6]=0.90;
    anacutsval[9][7]=0.90;
    anacutsval[9][8]=0.90;
    anacutsval[9][9]=0.90;
    //10 Sum d0^2 (cm^2)
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[10][ipt]=0.; }
    //11 dca [cm]
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[11][ipt]=1000.; }
    //12 inv. mass (Mphi-MKK) [GeV]
    anacutsval[12][0]=0.010;
    anacutsval[12][1]=0.010;
    anacutsval[12][2]=0.010;
    anacutsval[12][3]=0.010;
    anacutsval[12][4]=0.010;
    anacutsval[12][5]=0.010;
    anacutsval[12][6]=0.010;
    anacutsval[12][7]=0.010;
    anacutsval[12][8]=0.010;
    anacutsval[12][9]=0.010;
    //13 inv. mass (MKo*-MKpi) [GeV]
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[13][ipt]=0.2; }
    //14 Abs(CosineKpiPhiRFrame)^3
    anacutsval[14][0]=0.10;
    anacutsval[14][1]=0.10;
    anacutsval[14][2]=0.10;
    anacutsval[14][3]=0.05;
    anacutsval[14][4]=0.05;
    anacutsval[14][5]=0.05;
    anacutsval[14][6]=0.00;
    anacutsval[14][7]=0.00;
    anacutsval[14][8]=0.00;
    anacutsval[14][9]=0.00;
    //15 Abs(CosineKpiPhiRFrame)^3
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[15][ipt]=1.; }
    //16 decLenXY [cm]
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[16][ipt]=0.; }
    //17 NormdecLen
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[17][ipt]=0.; }
    //18 NormdecLenXY
    for(Int_t ipt=0;ipt<nptbins;ipt++) { anacutsval[18][ipt]=4.; }
    //19 cosThetaPointXY
    anacutsval[19][0]=0.94;
    anacutsval[19][1]=0.92;
    anacutsval[19][2]=0.92;
    anacutsval[19][3]=0.92;
    anacutsval[19][4]=0.92;
    anacutsval[19][5]=0.90;
    anacutsval[19][6]=0.90;
    anacutsval[19][7]=0.90;
    anacutsval[19][8]=0.90;
    anacutsval[19][9]=0.90;
    //d0
    Float_t *d0cutsval=new Float_t[nptbins];
    d0cutsval[0]=140;
    d0cutsval[1]=140;
    d0cutsval[2]=140;
    d0cutsval[3]=120;
    d0cutsval[4]=120;
    d0cutsval[5]=120;
    d0cutsval[6]=120;
    d0cutsval[7]=120;
    d0cutsval[8]=120;
    d0cutsval[9]=120;
    
    cuts->SetMinPtCandidate(1.);
    cuts->SetMaxPtCandidate(24.);
    cuts->SetPtBins(nptbins+1,ptbins);
    cuts->SetCuts(nvars,nptbins,anacutsval);
    cuts->Setd0Cut(nptbins,d0cutsval);
    
    Bool_t pidflag = kTRUE;
    cuts->SetUsePID(pidflag);
    cuts->SetPidOption(1); //0=kConservative,1=kStrong
    cuts->SetMaxPtStrongPid(6.);
    cuts->SetMaxPtStrongPid(9999.);
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
  
  cuts->SetName(nameCuts.Data());
  cuts->SetTitle(nameCuts.Data());
  cout<<"This is the object I'm going to save:"<<endl;
  cuts->PrintAll();
  
  return cuts;
}


