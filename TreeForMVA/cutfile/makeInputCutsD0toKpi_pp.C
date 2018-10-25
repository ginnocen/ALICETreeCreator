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

AliRDHFCutsD0toKpi *makeInputCutsD0toKpi_pp(Int_t whichCuts=0, TString nameCuts="D0toKpiFilteringCuts", Float_t minc=0.,Float_t maxc=20.)
{
  AliRDHFCutsD0toKpi* cutsD0toKpi=new AliRDHFCutsD0toKpi();
  cutsD0toKpi->SetName(nameCuts.Data());
  cutsD0toKpi->SetTitle(nameCuts.Data());
  
  AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
  esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
  //default
  esdTrackCuts->SetRequireTPCRefit(kTRUE);
  esdTrackCuts->SetRequireITSRefit(kTRUE);
  if(whichCuts==0) esdTrackCuts->SetMinNClustersTPC(50);
  esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);
  esdTrackCuts->SetEtaRange(-0.8,0.8);
  esdTrackCuts->SetMinDCAToVertexXY(0.);
  esdTrackCuts->SetPtRange(0.3,1.e10);
  esdTrackCuts->SetMaxDCAToVertexXY(1.e6);
//  if(whichCuts==0)esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
//  if(whichCuts==1)esdTrackCuts->SetMinDCAToVertexXYPtDep("0.005*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
  cutsD0toKpi->AddTrackCuts(esdTrackCuts);
  
  if(whichCuts==0){
    const Int_t nptbinsD0=1;
    Float_t ptlimitsD0[nptbinsD0+1]={0.,1000000.};
    Float_t cutsArrayD0toKpi[11]={0.4,999999.,1.1,0.,0.,999999.,999999.,999999.,0.,-1,0.};
    cutsD0toKpi->AddTrackCuts(esdTrackCuts);

    cutsD0toKpi->SetMinPtCandidate(0.);
    cutsD0toKpi->SetUsePID(kFALSE);
    cutsD0toKpi->SetUseTrackSelectionWithFilterBits(kFALSE);
    cutsD0toKpi->SetUseSpecialCuts(kFALSE);
    cutsD0toKpi->SetPtBins(nptbinsD0+1,ptlimitsD0);
    cutsD0toKpi->SetCuts(11,cutsArrayD0toKpi);
  }
  else if(whichCuts==1){
    
    cutsD0toKpi->SetSelectCandTrackSPDFirst(kTRUE, 3);
    const Int_t nvars=11;
    
    const Int_t nptbins=17;
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    ptbins[0]=0.;
    ptbins[1]=0.5;
    ptbins[2]=1.;
    ptbins[3]=2.;
    ptbins[4]=3.;
    ptbins[5]=4.;
    ptbins[6]=5.;
    ptbins[7]=6.;
    ptbins[8]=7.;
    ptbins[9]=8.;
    ptbins[10]=10.;
    ptbins[11]=12.;
    ptbins[12]=16.;
    ptbins[13]=20.;
    ptbins[14]=24.;
    ptbins[15]=36.;
    ptbins[16]=50.;
    ptbins[17]=9999.;
    
    cutsD0toKpi->SetPtBins(nptbins+1,ptbins);
    cutsD0toKpi->SetGlobalIndex(nvars,nptbins);
    cutsD0toKpi->SetPtBins(nptbins+1,ptbins);
    //m    dca      cost*  ptk ptpi  d0k          d0pi       d0d0          cosp  cosxy normdxy
    Float_t cutsMatrixD0toKpiStand[nptbins][nvars]=  {{0.400,350.*1E-4, 0.8, 0.5, 0.5, 1000.*1E-4, 1000.*1E-4, -2000. *1E-8, 0.7,  0.,0.},/* pt<0.5*/
      {0.400,350.*1E-4, 0.8, 0.5, 0.5, 1000.*1E-4, 1000.*1E-4, -2000. *1E-8, 0.7,  0.,0.},/* 0.5<pt<1*/
      {0.400,300.*1E-4, 0.8, 0.4, 0.4, 1000.*1E-4, 1000.*1E-4, -25000.*1E-8, 0.8,  0.,0.},/* 1<pt<2 */
      {0.400,300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -20000.*1E-8, 0.9,   0.,0.},/* 2<pt<3 *///d0d0 e cosp
      {0.400,300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -12000.*1E-8, 0.9,   0.,0.},/* 3<pt<4 */
      {0.400,300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -8000. *1E-8, 0.85,  0.,0.},/* 4<pt<5 */
      {0.400,300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -8000. *1E-8, 0.85,  0.,0.},/* 5<pt<6 */
      {0.400,300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -8000. *1E-8, 0.85,  0.,0.},/* 6<pt<7 */
      {0.400,300.*1E-4, 0.8, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -7000. *1E-8, 0.85,  0.,0.},/* 7<pt<8 */
      {0.400,300.*1E-4, 0.9, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -5000. *1E-8, 0.85,  0.,0.},/* 8<pt<10 */
      {0.400,300.*1E-4, 0.9, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, -5000. *1E-8, 0.85,  0.,0.},/* 10<pt<12 */
      {0.400,300.*1E-4, 1.0, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, 10000. *1E-8, 0.85,  0.,0.},/* 12<pt<16 */
      {0.400,300.*1E-4, 1.0, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, 999999.*1E-8, 0.85,  0.,0.},/* 16<pt<20 */
      {0.400,300.*1E-4, 1.0, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, 999999.*1E-8, 0.85,  0.,0.},/* 20<pt<24 */
      {0.400,300.*1E-4, 1.0, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, 999999.*1E-8, 0.85,  0.,0.},/* 24<pt<36 */
      {0.400,300.*1E-4, 1.0, 0.7, 0.7, 1000.*1E-4, 1000.*1E-4, 999999.*1E-8, 0.85,  0.,0.},/* 36<pt<50 */
      {0.400,300.*1E-4, 1.0, 0.6, 0.6, 1000.*1E-4, 1000.*1E-4, 999999.*1E-8, 0.8,   0.,0.}};/* pt>50 */
    
    
    
    //setting my cut values
    //cuts order
    //       printf("    |M-MD0| [GeV]    < %f\n",fD0toKpiCuts[0]);
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
    cutsD0toKpi->SetRemoveDaughtersFromPrim(kTRUE);
    for(Int_t iv=0;iv<nvars;iv++) delete [] cutsMatrixTransposeStand[iv];
    delete [] cutsMatrixTransposeStand;
    cutsMatrixTransposeStand=NULL;
        
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
    if(pidflag) cout<<"PID is used"<<endl;
    else cout<<"PID is not used"<<endl;
  }
  
  //event selection
  cutsD0toKpi->SetUsePhysicsSelection(kTRUE);
  cutsD0toKpi->SetTriggerClass("");
  cutsD0toKpi->SetTriggerMask(AliVEvent::kINT7);
  cutsD0toKpi->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent);
  cutsD0toKpi->ConfigurePileupCuts(5,0.8);
  cutsD0toKpi->SetMaxVtxZ(10.);
  cutsD0toKpi->SetCutOnzVertexSPD(3);
  cutsD0toKpi->SetMinVtxContr(1);

  cout<<"This is the object I'm going to save:"<<endl;
  cutsD0toKpi->SetName(nameCuts.Data());
  cutsD0toKpi->SetTitle(nameCuts.Data());
  cutsD0toKpi->PrintAll();
  
  return cutsD0toKpi;
  
}
