#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsD0toKpi.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="BplustoD0piFilteringCuts"
 whichCuts=1, nameCuts="BplustoD0piAnalysisCuts"
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

AliRDHFCutsD0toKpi *makeInputCutsBplustoD0pi_pp(Int_t whichCuts=0, TString nameCuts="BplustoD0piFilteringCuts", Float_t minc=0.,Float_t maxc=100.)
{
  AliRDHFCutsD0toKpi* cuts=new AliRDHFCutsD0toKpi();
  cuts->SetName(nameCuts.Data());
  cuts->SetTitle(nameCuts.Data());
  
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
  esdTrackCuts->SetPtRange(0.5,1.e10);
  cuts->AddTrackCuts(esdTrackCuts);
  cuts->SetUseTrackSelectionWithFilterBits();
  
  if(whichCuts==0){
    
    const Int_t nptbinsD0=1;
    Float_t ptlimitsD0[nptbinsD0+1]={0.,1000000.};
    Float_t cutsArrayD0toKpi[11]={0.15,0.05,1.1,0.5,0.5,0.5,0.5,0.1,0.6,-1,0.};
    //Used values for D0 analysis taken at 28/08/19 but tightened InvMass cut (0.25->0.15)

    cuts->SetMinPtCandidate(1.);
    cuts->SetPtBins(nptbinsD0+1,ptlimitsD0);
    cuts->SetCuts(11,cutsArrayD0toKpi);
    
    cuts->SetUseSpecialCuts(kFALSE);
    
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
  }
  else if(whichCuts==1){
    
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
    
    //Used values for D0 analysis taken at 28/08/19
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
    
    //CREATE TRANSPOSE MATRIX...REVERSE INDICES as required by AliRDHFCuts
    Float_t **cutsMatrixTransposeStand=new Float_t*[nvars];
    for(Int_t iv=0;iv<nvars;iv++)cutsMatrixTransposeStand[iv]=new Float_t[nptbins];
    
    for (Int_t ibin=0;ibin<nptbins;ibin++){
      for (Int_t ivar = 0; ivar<nvars; ivar++){
        cutsMatrixTransposeStand[ivar][ibin]=cutsMatrixD0toKpiStand[ibin][ivar];
      }
    }
    
    cuts->SetPtBins(nptbins+1,ptbins);
    cuts->SetGlobalIndex(nvars,nptbins);
    cuts->SetMinPtCandidate(1.);
    cuts->SetCuts(nvars,nptbins,cutsMatrixTransposeStand);
    
    cuts->SetUseSpecialCuts(kTRUE);
    
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
    cuts->SetPidHF(pidObj);
    
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
