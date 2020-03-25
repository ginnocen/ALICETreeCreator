#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsD0toKpi.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="D0toKpiFilteringCuts"
 whichCuts=1, nameCuts="D0toKpiAnalysisCuts"
 */

//setting my cut values
//cuts order
//     printf("    |M-MD0| [GeV]    < %f\n",fD0toKpiCuts[0]);
//     printf("    dca    [cm]  < %f\n",fD0toKpiCuts[1]);
//     printf("    cosThetaStar     < %f\n",fD0toKpiCuts[2]);
//     printf("    pTK     [GeV/c]    > %f\n",fD0toKpiCuts[3]);
//     printf("    pTpi    [GeV/c]    > %f\n",fD0toKpiCuts[4]);
//     printf("    |d0K|  [cm]  < %f\n",fD0toKpiCuts[5]);
//     printf("    |d0pi| [cm]  < %f\n",fD0toKpiCuts[6]);
//     printf("    d0d0  [cm^2] < %f\n",fD0toKpiCuts[7]);
//     printf("    cosThetaPoint    > %f\n",fD0toKpiCuts[8]);
//     printf("    |cosThetaPointXY| < %f\n",fD0toKpiCuts[9]);
//     printf("    NormDecayLenghtXY    > %f\n",fD0toKpiCuts[10]);

AliRDHFCutsD0toKpi *makeInputCutsD0toKpi_pp(Int_t whichCuts=0, TString nameCuts="D0toKpiFilteringCuts", Float_t minc=0.,Float_t maxc=100.)
{
  AliRDHFCutsD0toKpi* cutsD0toKpi=new AliRDHFCutsD0toKpi();
  cutsD0toKpi->SetName(nameCuts.Data());
  cutsD0toKpi->SetTitle(nameCuts.Data());
  
  //UPDATE 21/06/19, use the same track quality cuts for filtering and analysis cuts
  //single track cuts
  AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
  esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
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
  cutsD0toKpi->AddTrackCuts(esdTrackCuts);
  cutsD0toKpi->SetSelectCandTrackSPDFirst(kTRUE, 3);
  cutsD0toKpi->SetUseTrackSelectionWithFilterBits();
  
  if(whichCuts==0){
    
    const Int_t nptbinsD0=1;
    Float_t ptlimitsD0[nptbinsD0+1]={0.,1000000.};
    Float_t cutsArrayD0toKpi[11]={0.25,0.05,1.1,0.3,0.3,0.5,0.5,0.1,0.6,-1,0.};
    
    cutsD0toKpi->SetMinPtCandidate(1.);
    cutsD0toKpi->SetPtBins(nptbinsD0+1,ptlimitsD0);
    cutsD0toKpi->SetCuts(11,cutsArrayD0toKpi);
    
    cutsD0toKpi->SetUseSpecialCuts(kFALSE);
    
    //UPDATE 21/06/19, enable PID selection to reduce output size
    //UPDATE 03/10/19, set std PID selection as it is looser
    AliAODPidHF* pidObj=new AliAODPidHF();
    //pidObj->SetName("pid4D0");
    const Int_t nlims=2;
    Double_t plims[nlims]={0.6,0.8}; //TPC limits in momentum [GeV/c]
    Double_t sigmas[5]={2.,1.,0.,3.,0.}; //to be checked and to be modified with new implementation of setters by Rossella
    pidObj->SetAsym(kTRUE);// if you want to use the asymmetric bands in TPC
    pidObj->SetMatch(1);
    pidObj->SetPLimit(plims,nlims);
    pidObj->SetSigma(sigmas);
    pidObj->SetCompat(kTRUE);
    pidObj->SetTPC(kTRUE);
    pidObj->SetTOF(kTRUE);
    pidObj->SetPCompatTOF(1.5);
    pidObj->SetSigmaForTPCCompat(3.);
    pidObj->SetSigmaForTOFCompat(3.);
    pidObj->SetOldPid(kFALSE);
    
    cutsD0toKpi->SetPidHF(pidObj);
    Bool_t pidflag=kTRUE;
    cutsD0toKpi->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for filtering cuts"<<endl;
    else cout<<"PID is not used for filtering cuts"<<endl;
  } else if(whichCuts==1){
    //UPDATE 25/03/20, use the same topological cuts as STD 13TeV analysis of Cristina.
    //Note: PID selection is NOT changed. Also kFirst requirement for pT < 3 not added

    const Int_t nvars=11;
    const Int_t nptbins=25;
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    ptbins[0]=0.;
    ptbins[1]=0.5;
    ptbins[2]=1.0;
    ptbins[3]=1.5;
    ptbins[4]=2.0;
    ptbins[5]=2.5;
    ptbins[6]=3.0;
    ptbins[7]=3.5;
    ptbins[8]=4.0;
    ptbins[9]=4.5;
    ptbins[10]=5.0;
    ptbins[11]=5.5;
    ptbins[12]=6.0;
    ptbins[13]=6.5;
    ptbins[14]=7.0;
    ptbins[15]=7.5;
    ptbins[16]=8.;
    ptbins[17]=9.;
    ptbins[18]=10.;
    ptbins[19]=12.;
    ptbins[20]=16.;
    ptbins[21]=20.;
    ptbins[22]=24.;
    ptbins[23]=36.;
    ptbins[24]=50.;
    ptbins[25]=99999.;

    cutsD0toKpi->SetGlobalIndex(nvars,nptbins);
    cutsD0toKpi->SetPtBins(nptbins+1,ptbins);

    Float_t** fCutsRD;
    fCutsRD=new Float_t*[nvars];
    for(Int_t iv=0;iv<nvars;iv++){
      fCutsRD[iv]=new Float_t[nptbins];
    }

    for(int ic = 0; ic < 25; ic++) fCutsRD[0][ic] = 0.4;

    fCutsRD[1][0] = 0.035;
    fCutsRD[1][1] = 0.035;
    for(int ic = 2; ic < 25; ic++) fCutsRD[1][ic] = 0.03;

    for(int ic = 0; ic < 16; ic++) fCutsRD[2][ic] = 0.8;
    for(int ic = 16; ic < 19; ic++) fCutsRD[2][ic] = 0.9;
    for(int ic = 19; ic < 25; ic++) fCutsRD[2][ic] = 1.0;

    fCutsRD[3][0] = 0.5;
    fCutsRD[3][1] = 0.5;
    fCutsRD[3][2] = 0.4;
    fCutsRD[3][3] = 0.4;
    for(int ic = 4; ic < 24; ic++) fCutsRD[3][ic] = 0.7;
    fCutsRD[3][24] = 0.6;

    fCutsRD[4][0] = 0.5;
    fCutsRD[4][1] = 0.5;
    fCutsRD[4][2] = 0.4;
    fCutsRD[4][3] = 0.4;
    for(int ic = 4; ic < 24; ic++) fCutsRD[4][ic] = 0.7;
    fCutsRD[4][24] = 0.6;

    for(int ic = 0; ic < 25; ic++) fCutsRD[5][ic] = 0.1;

    for(int ic = 0; ic < 25; ic++) fCutsRD[6][ic] = 0.1;

    fCutsRD[7][0] = -5e-05;
    fCutsRD[7][1] = -5e-05;
    fCutsRD[7][2] = -0.00025;
    fCutsRD[7][3] = -0.00025;
    fCutsRD[7][4] = -0.0002;
    fCutsRD[7][5] = -0.0002;
    fCutsRD[7][6] = -0.00012;
    fCutsRD[7][7] = -0.00012;
    fCutsRD[7][8] = -8e-05;
    fCutsRD[7][9] = -8e-05;
    fCutsRD[7][10] = -8e-05;
    fCutsRD[7][11] = -8e-05;
    fCutsRD[7][12] = -8e-05;
    fCutsRD[7][13] = -8e-05;
    fCutsRD[7][14] = -7e-05;
    fCutsRD[7][15] = -7e-05;
    fCutsRD[7][16] = -5e-05;
    fCutsRD[7][17] = -5e-05;
    fCutsRD[7][18] = -5e-05;
    fCutsRD[7][19] = 0.0001;
    fCutsRD[7][20] = 0.00999999;
    fCutsRD[7][21] = 0.00999999;
    fCutsRD[7][22] = 0.00999999;
    fCutsRD[7][23] = 0.00999999;
    fCutsRD[7][24] = 0.00999999;

    fCutsRD[8][0] = 0.8;
    fCutsRD[8][1] = 0.8;
    fCutsRD[8][2] = 0.8;
    fCutsRD[8][3] = 0.8;
    fCutsRD[8][4] = 0.9;
    fCutsRD[8][5] = 0.9;
    for(int ic = 6; ic < 24; ic++) fCutsRD[8][ic] = 0.85;
    fCutsRD[8][24] = 0.8;

    for(int ic = 0; ic < 25; ic++) fCutsRD[9][ic] = 0.;

    for(int ic = 0; ic < 25; ic++) fCutsRD[10][ic] = 0.;

    cutsD0toKpi->SetCuts(nvars,nptbins,fCutsRD);
    
    cutsD0toKpi->SetUseSpecialCuts(kTRUE);
    
    //pid settings
    AliAODPidHF* pidObj=new AliAODPidHF();
    //pidObj->SetName("pid4D0");
    Int_t mode=1;
    const Int_t nlims=2;
    Double_t plims[nlims]={0.6,0.8}; //TPC limits in momentum [GeV/c]
    Bool_t compat=kTRUE; //effective only for this mode
    Bool_t asym=kTRUE;
    Double_t sigmas[5]={2.,1.,0.,3.,0.}; //to be checked and to be modified with new implementation of setters by Rossella
    pidObj->SetAsym(asym);// if you want to use the asymmetric bands in TPC
    pidObj->SetMatch(mode);
    pidObj->SetPLimit(plims,nlims);
    pidObj->SetSigma(sigmas);
    pidObj->SetCompat(compat);
    pidObj->SetTPC(kTRUE);
    pidObj->SetTOF(kTRUE);
    pidObj->SetPCompatTOF(1.5);
    pidObj->SetSigmaForTPCCompat(3.);
    pidObj->SetSigmaForTOFCompat(3.);
    pidObj->SetOldPid(kFALSE);
    cutsD0toKpi->SetPidHF(pidObj);
    
    Bool_t pidflag=kTRUE;
    cutsD0toKpi->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for analysis cuts"<<endl;
    else cout<<"PID is not used for analysis cuts"<<endl;
  }
  
  cutsD0toKpi->SetRemoveDaughtersFromPrim(kTRUE); //activate for pp
  
  //event selection
  cutsD0toKpi->SetUsePhysicsSelection(kTRUE);
  cutsD0toKpi->SetTriggerClass("");
  //Since X/08/19 we set the triggers in wagon code/runAnalysis script if needed
  cutsD0toKpi->SetTriggerMask(AliVEvent::kAny);
  cutsD0toKpi->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent);
  cutsD0toKpi->SetMinContribPileupMV(5);
  cutsD0toKpi->SetMaxVtxZ(10.);
  cutsD0toKpi->SetCutOnzVertexSPD(3);
  cutsD0toKpi->SetMinVtxContr(1);
  
  cout<<"This is the object I'm going to save:"<<endl;
  cutsD0toKpi->SetName(nameCuts.Data());
  cutsD0toKpi->SetTitle(nameCuts.Data());
  cutsD0toKpi->PrintAll();
  
  return cutsD0toKpi;
  
}
