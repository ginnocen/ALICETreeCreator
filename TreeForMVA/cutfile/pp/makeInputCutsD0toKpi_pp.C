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
    
    // This is kTRUE by default, set it to kFALSE otherwise it will apply
    // an additional PID selection on candidates < 2GeV (default) resulting
    // in cutting away most of them
    // in this way also comply with the STD analysis done by Cristina
    // with D0Mass task
    cutsD0toKpi->SetLowPt(kFALSE);
    cutsD0toKpi->SetPidHF(pidObj);
    Bool_t pidflag=kTRUE;
    cutsD0toKpi->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for filtering cuts"<<endl;
    else cout<<"PID is not used for filtering cuts"<<endl;
  } else if(whichCuts==1){
    //UPDATE 25/03/20, use the same topological cuts as STD 13TeV analysis of Cristina.

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
                                                      //m     dca      cost*  ptk ptpi  d0k         d0pi        d0d0         cosp  cosxy normdxy
    Float_t cutsMatrixD0toKpiStand[nptbins][nvars]=  {{0.400, 350.*1E-4, 0.8, 0.5, 0.5, 1000.*1E-4, 1000.*1E-4, -5000. *1E-8, 0.80, 0., 0.},/* pt<0.5*/
                                                      {0.400, 350.*1E-4, 0.8, 0.5, 0.5, 1000.*1E-4, 1000.*1E-4, -5000. *1E-8, 0.80, 0., 0.},/* 0.5<pt<1*/
                                                      {0.400, 300.*1E-4, 0.8, 0.4, 0.4, 1000.*1E-4, 1000.*1E-4, -25000.*1E-8, 0.80, 0., 0.},/* 1<pt<1.5 */
                                                      {0.400, 300.*1E-4, 0.8, 0.4, 0.4, 1000.*1E-4, 1000.*1E-4, -25000.*1E-8, 0.80, 0., 0.},/* 1.5<pt<2 */
                                                      {0.400, 300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -20000.*1E-8, 0.90, 0., 0.},/* 2<pt<2.5 *///
                                                      {0.400, 300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -20000.*1E-8, 0.90, 0., 0.},/* 2.5<pt<3 */
                                                      {0.400, 300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -12000.*1E-8, 0.85, 0., 0.},/* 3<pt<3.5 */
                                                      {0.400, 300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -12000.*1E-8, 0.85, 0., 0.},/* 3.5<pt<4 */
                                                      {0.400, 300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -8000. *1E-8, 0.85, 0., 0.},/* 4<pt<4.5 */
                                                      {0.400, 300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -8000. *1E-8, 0.85, 0., 0.},/* 4.5<pt<5 */
                                                      {0.400, 300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -8000. *1E-8, 0.85, 0., 0.},/* 5<pt<5.5 */
                                                      {0.400, 300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -8000. *1E-8, 0.85, 0., 0.},/* 5.5<pt<6 */
                                                      {0.400, 300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -8000. *1E-8, 0.85, 0., 0.},/* 6<pt<6.5 */
                                                      {0.400, 300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -8000. *1E-8, 0.85, 0., 0.},/* 6.5<pt<7 */
                                                      {0.400, 300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -7000. *1E-8, 0.85, 0., 0.},/* 7<pt<7.5 */
                                                      {0.400, 300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -7000. *1E-8, 0.85, 0., 0.},/* 7.5<pt<8 */
                                                      {0.400, 300.*1E-4, 0.9, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -5000. *1E-8, 0.85, 0., 0.},/* 8<pt<9 */
                                                      {0.400, 300.*1E-4, 0.9, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -5000. *1E-8, 0.85, 0., 0.},/* 9<pt<10 */
                                                      {0.400, 300.*1E-4, 0.9, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -5000. *1E-8, 0.85, 0., 0.},/* 10<pt<12 */
                                                      {0.400, 300.*1E-4, 1.0, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, 10000. *1E-8, 0.85, 0., 0.},/* 12<pt<16 */
                                                      {0.400, 300.*1E-4, 1.0, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, 999999.*1E-8, 0.85, 0., 0.},/* 16<pt<20 */
                                                      {0.400, 300.*1E-4, 1.0, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, 999999.*1E-8, 0.85, 0., 0.},/* 20<pt<24 */
                                                      {0.400, 300.*1E-4, 1.0, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, 999999.*1E-8, 0.85, 0., 0.},/* 24<pt<36 */
                                                      {0.400, 300.*1E-4, 1.0, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, 999999.*1E-8, 0.85, 0., 0.},/* 36<pt<50 */
                                                      {0.400, 300.*1E-4, 1.0, 0.6, 0.6, 1000.*1E-4, 1000.*1E-4, 999999.*1E-8, 0.80, 0., 0.}};/* pt>50 */
    
    //CREATE TRANSPOSE MATRIX...REVERSE INDICES as required by AliRDHFCuts
    Float_t **cutsMatrixTransposeStand=new Float_t*[nvars];
    for(Int_t iv=0;iv<nvars;iv++)cutsMatrixTransposeStand[iv]=new Float_t[nptbins];

    for (Int_t ibin=0;ibin<nptbins;ibin++){
      for (Int_t ivar = 0; ivar<nvars; ivar++){
        cutsMatrixTransposeStand[ivar][ibin]=cutsMatrixD0toKpiStand[ibin][ivar];
      }
    }
    cutsD0toKpi->SetCuts(nvars,nptbins,cutsMatrixTransposeStand);
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
    cutsD0toKpi->SetUseDefaultPID(kFALSE);
    cutsD0toKpi->SetLowPt(kFALSE);

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
