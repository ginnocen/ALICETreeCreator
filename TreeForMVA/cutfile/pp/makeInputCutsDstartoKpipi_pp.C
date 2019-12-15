#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsDStartoKpipi.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="DstartoKpipiFilteringCuts"
 whichCuts=1, nameCuts="DstartoKpipiAnalysisCuts"
 */


//setting my cut values
//cuts order
//printf(    "inv. mass [GeV]" < %f\n",fCutsRD[0]);   
//printf(    "dca [cm]", < %f\n",fCutsRD[1]);
//printf(    "cosThetaStar", < %f\n",fCutsRD[2]);
//printf(    "pTK [GeV/c]", > %f\n",fCutsRD[3]);
//printf(    "pTPi [GeV/c]", > %f\n",fCutsRD[4]);
//printf(    "d0K [cm]", < %f\n",fCutsRD[5]);
//printf(    "d0Pi [cm]", < %f\n",fCutsRD[6]);
//printf(    "d0d0 [cm^2]", < %f\n",fCutsRD[7]);
//printf(    "cosThetaPoint", > %f\n",fCutsRD[8]);
//printf(    "inv. mass half width of D* [GeV]", < %f\n",fCutsRD[9]);
//printf(    "half width of (M_Kpipi-M_D0) [GeV]", < %f\n",fCutsRD[10]);
//printf(    "PtMin of pi_s [GeV/c]", < %f\n",fCutsRD[11]);
//printf(    "PtMax of pi_s [GeV/c]", < %f\n",fCutsRD[12]);
//printf(    "theta, angle between the pi_s and decay plane of the D0 [rad]", > %f\n",fCutsRD[13]);
//printf(    "|cosThetaPointXY|", > %f\n",fCutsRD[14]);
//printf(    "NormDecayLenghtXY" > %f\n",fCutsRD[15]);


AliRDHFCutsDStartoKpipi *makeInputCutsDstartoKpipi_pp(Int_t whichCuts=0, TString nameCuts="DstartoKpipiFilteringCuts", Float_t minc=0.,Float_t maxc=100.)
{
  
  AliRDHFCutsDStartoKpipi* cutsDstartoKpipi=new AliRDHFCutsDStartoKpipi();
  cutsDstartoKpipi->SetName(nameCuts.Data());
  cutsDstartoKpipi->SetTitle(nameCuts.Data());
  
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
  esdTrackCuts->SetMaxDCAToVertexXY(1.e6);
  cutsDstartoKpipi->AddTrackCuts(esdTrackCuts);
  
  //soft pion selections
  AliESDtrackCuts* esdTrackCutsSoftPi=new AliESDtrackCuts();
  esdTrackCutsSoftPi->SetRequireSigmaToVertex(kFALSE);
  esdTrackCutsSoftPi->SetRequireTPCRefit(kFALSE);
  esdTrackCutsSoftPi->SetRequireITSRefit(kTRUE);
  esdTrackCutsSoftPi->SetMinNClustersITS(2);
  esdTrackCutsSoftPi->SetEtaRange(-0.8,+0.8);
  esdTrackCutsSoftPi->SetPtRange(0.05,1.e10);
  cutsDstartoKpipi->AddTrackCutsSoftPi(esdTrackCutsSoftPi);
  
  if(whichCuts==0){
    const Int_t nptbinsDstar=1;
    Float_t ptlimitsDstar[nptbinsDstar+1]={0.,1000000.};
    Float_t cutsArrayDstartoKpipi[16]={0.1, 0.1, 1.1, 0.3, 0.3, 0.5, 0.5, 1.0, 0.2, 0.3, 0.025, 0.05, 100000000000.0, 0.5,-1.,0.}; // first 9 for D0 from D*, next 5 for D*, last 2 for D0 again
    //Update 16/09: Same filtering as D0 pp (some a bit looser & tighter D0 mass cut) + reduced mass window Dstar

    cutsDstartoKpipi->SetMinPtCandidate(0.);
    cutsDstartoKpipi->SetPtBins(nptbinsDstar+1,ptlimitsDstar);
    cutsDstartoKpipi->SetCuts(16,cutsArrayDstartoKpipi);
    
    //UPDATE 21/06/19, enable PID selection to reduce output size
    AliAODPidHF* pidObj=new AliAODPidHF();
    Double_t sigmasBac[5]={3.,3.,3.,3.,3.}; // 0, 1(A), 2(A) -> TPC; 3 -> TOF; 4 -> ITS
    pidObj->SetSigma(sigmasBac);
    pidObj->SetAsym(kFALSE);
    pidObj->SetMatch(1);
    pidObj->SetTPC(kTRUE);
    pidObj->SetTOF(kTRUE);
    
    cutsDstartoKpipi->SetPidHF(pidObj);
    Bool_t pidflag=kTRUE;
    cutsDstartoKpipi->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for filtering cuts"<<endl;
    else cout<<"PID is not used for filtering cuts"<<endl;
  }
  
  else if(whichCuts==1){
    
    const Int_t nvars=16;
    const Int_t nptbins=25;
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    ptbins[0]=0.5;
    ptbins[1]=1.;
    ptbins[2]=1.5;
    ptbins[3]=2.;
    ptbins[4]=2.5;
    ptbins[5]=3.;
    ptbins[6]=3.5;
    ptbins[7]=4.;
    ptbins[8]=4.5;
    ptbins[9]=5.;
    ptbins[10]=5.5;
    ptbins[11]=6.;
    ptbins[12]=6.5;
    ptbins[13]=7.;
    ptbins[14]=7.5;
    ptbins[15]=8.;
    ptbins[16]=9.;
    ptbins[17]=10.;
    ptbins[18]=11.;
    ptbins[19]=12.;
    ptbins[20]=16.;
    ptbins[21]=20.;
    ptbins[22]=24.;
    ptbins[23]=36.;
    ptbins[24]=50.;
    ptbins[25]=99999.;
    
    cutsDstartoKpipi->SetGlobalIndex(nvars,nptbins);
    cutsDstartoKpipi->SetPtBins(nptbins+1,ptbins);
    
    Float_t** fCutsRD;
    fCutsRD=new Float_t*[nvars];
    for(Int_t iv=0;iv<nvars;iv++){
      fCutsRD[iv]=new Float_t[nptbins];
    }
    
    fCutsRD[0][0] = 0.035; fCutsRD[0][1] = 0.032; fCutsRD[0][3]= 0.032; fCutsRD[0][5]= 0.032; fCutsRD[0][7]= 0.032; fCutsRD[0][9]= 0.036; fCutsRD[0][11]= 0.036; fCutsRD[0][13]= 0.04; fCutsRD[0][15]= 0.05; fCutsRD[0][17]= 0.074; fCutsRD[0][19]= 0.074; fCutsRD[0][20]= 0.074; fCutsRD[0][21]= 0.074; fCutsRD[0][22]= 0.074; fCutsRD[0][23]= 0.074; fCutsRD[0][24]= 0.074;
    fCutsRD[1][0] = 0.05; fCutsRD[1][1] = 0.038; fCutsRD[1][3]= 0.03; fCutsRD[1][5]= 0.033; fCutsRD[1][7]= 0.042; fCutsRD[1][9]= 0.05; fCutsRD[1][11]= 0.1; fCutsRD[1][13]= 0.1; fCutsRD[1][15]= 0.1; fCutsRD[1][17]= 0.1; fCutsRD[1][19]= 0.1; fCutsRD[1][20]= 10; fCutsRD[1][21]= 10; fCutsRD[1][22]= 10; fCutsRD[1][23]= 10; fCutsRD[1][24]= 10;
    fCutsRD[2][0] = 0.75; fCutsRD[2][1] = 0.9; fCutsRD[2][3]= 0.8; fCutsRD[2][5]= 0.8; fCutsRD[2][7]= 0.9; fCutsRD[2][9]= 1; fCutsRD[2][11]= 1; fCutsRD[2][13]= 1; fCutsRD[2][15]= 1; fCutsRD[2][17]= 1; fCutsRD[2][19]= 1; fCutsRD[2][20]= 10; fCutsRD[2][21]= 10; fCutsRD[2][22]= 10; fCutsRD[2][23]= 10; fCutsRD[2][24]= 10;
    fCutsRD[3][0] = 0.45; fCutsRD[3][1] = 0.6; fCutsRD[3][3]= 0.9; fCutsRD[3][5]= 0.9; fCutsRD[3][7]= 1; fCutsRD[3][9]= 1; fCutsRD[3][11]= 1; fCutsRD[3][13]= 0.6; fCutsRD[3][15]= 0.6; fCutsRD[3][17]= 0.6; fCutsRD[3][19]= 0.5; fCutsRD[3][20]= 0.2; fCutsRD[3][21]= 0.2; fCutsRD[3][22]= 0.2; fCutsRD[3][23]= 0.2; fCutsRD[3][24]= 0.2;
    fCutsRD[4][0] = 0.45; fCutsRD[4][1] = 0.6; fCutsRD[4][3]= 0.9; fCutsRD[4][5]= 0.9; fCutsRD[4][7]= 1; fCutsRD[4][9]= 1; fCutsRD[4][11]= 1; fCutsRD[4][13]= 0.6; fCutsRD[4][15]= 0.6; fCutsRD[4][17]= 0.6; fCutsRD[4][19]= 0.5; fCutsRD[4][20]= 0.2; fCutsRD[4][21]= 0.2; fCutsRD[4][22]= 0.2; fCutsRD[4][23]= 0.2; fCutsRD[4][24]= 0.2;
    fCutsRD[5][0] = 0.1; fCutsRD[5][1] = 0.1; fCutsRD[5][3]= 0.1; fCutsRD[5][5]= 0.1; fCutsRD[5][7]= 0.1; fCutsRD[5][9]= 0.1; fCutsRD[5][11]= 0.1; fCutsRD[5][13]= 0.1; fCutsRD[5][15]= 0.1; fCutsRD[5][17]= 0.1; fCutsRD[5][19]= 0.15; fCutsRD[5][20]= 0.5; fCutsRD[5][21]= 0.5; fCutsRD[5][22]= 0.5; fCutsRD[5][23]= 0.5; fCutsRD[5][24]= 0.5;
    fCutsRD[6][0] = 0.1; fCutsRD[6][1] = 0.1; fCutsRD[6][3]= 0.1; fCutsRD[6][5]= 0.1; fCutsRD[6][7]= 0.1; fCutsRD[6][9]= 0.1; fCutsRD[6][11]= 0.1; fCutsRD[6][13]= 0.1; fCutsRD[6][15]= 0.1; fCutsRD[6][17]= 0.1; fCutsRD[6][19]= 0.15; fCutsRD[6][20]= 0.5; fCutsRD[6][21]= 0.5; fCutsRD[6][22]= 0.5; fCutsRD[6][23]= 0.5; fCutsRD[6][24]= 0.5;
    fCutsRD[7][0] = 1; fCutsRD[7][1] = -0.000135; fCutsRD[7][3]= -0.00019; fCutsRD[7][5]= -0.000144; fCutsRD[7][7]= -2.8e-05; fCutsRD[7][9]= 5.5e-05; fCutsRD[7][11]= 0.0001; fCutsRD[7][13]= 0.0001; fCutsRD[7][15]= 0.001; fCutsRD[7][17]= 0.001; fCutsRD[7][19]= 0.001; fCutsRD[7][20]= 1; fCutsRD[7][21]= 1; fCutsRD[7][22]= 1; fCutsRD[7][23]= 1; fCutsRD[7][24]= 1;
    fCutsRD[8][0] = 0.7; fCutsRD[8][1] = 0.8; fCutsRD[8][3]= 0.9; fCutsRD[8][5]= 0.89; fCutsRD[8][7]= 0.81; fCutsRD[8][9]= 0.79; fCutsRD[8][11]= 0.7; fCutsRD[8][13]= 0.75; fCutsRD[8][15]= 0.7; fCutsRD[8][17]= 0.7; fCutsRD[8][19]= 0.7; fCutsRD[8][20]= 0.2; fCutsRD[8][21]= 0.2; fCutsRD[8][22]= 0.2; fCutsRD[8][23]= 0.2; fCutsRD[8][24]= 0.2;
    fCutsRD[9][0] = 0.3; fCutsRD[9][1] = 0.3; fCutsRD[9][3]= 0.3; fCutsRD[9][5]= 0.3; fCutsRD[9][7]= 0.3; fCutsRD[9][9]= 0.3; fCutsRD[9][11]= 0.3; fCutsRD[9][13]= 0.3; fCutsRD[9][15]= 0.3; fCutsRD[9][17]= 0.3; fCutsRD[9][19]= 0.3; fCutsRD[9][20]= 0.3; fCutsRD[9][21]= 0.3; fCutsRD[9][22]= 0.3; fCutsRD[9][23]= 0.3; fCutsRD[9][24]= 0.3;
    fCutsRD[10][0] = 0.3; fCutsRD[10][1] = 0.3; fCutsRD[10][3]= 0.3; fCutsRD[10][5]= 0.3; fCutsRD[10][7]= 0.3; fCutsRD[10][9]= 0.3; fCutsRD[10][11]= 0.3; fCutsRD[10][13]= 0.3; fCutsRD[10][15]= 0.3; fCutsRD[10][17]= 0.3; fCutsRD[10][19]= 0.3; fCutsRD[10][20]= 0.3; fCutsRD[10][21]= 0.3; fCutsRD[10][22]= 0.3; fCutsRD[10][23]= 0.3; fCutsRD[10][24]= 0.3;
    fCutsRD[11][0] = 0.03; fCutsRD[11][1] = 0.05; fCutsRD[11][3]= 0.05; fCutsRD[11][5]= 0.05; fCutsRD[11][7]= 0.05; fCutsRD[11][9]= 0.05; fCutsRD[11][11]= 0.05; fCutsRD[11][13]= 0.05; fCutsRD[11][15]= 0.05; fCutsRD[11][17]= 0.05; fCutsRD[11][19]= 0.05; fCutsRD[11][20]= 0.05; fCutsRD[11][21]= 0.05; fCutsRD[11][22]= 0.05; fCutsRD[11][23]= 0.05; fCutsRD[11][24]= 0.05;
    fCutsRD[12][0] = 0.2; fCutsRD[12][1] = 0.3; fCutsRD[12][3]= 0.4; fCutsRD[12][5]= 0.6; fCutsRD[12][7]= 100; fCutsRD[12][9]= 100; fCutsRD[12][11]= 100; fCutsRD[12][13]= 100; fCutsRD[12][15]= 100; fCutsRD[12][17]= 100; fCutsRD[12][19]= 100; fCutsRD[12][20]= 100; fCutsRD[12][21]= 100; fCutsRD[12][22]= 100; fCutsRD[12][23]= 100; fCutsRD[12][24]= 100;
    fCutsRD[13][0] = 1; fCutsRD[13][1] = 1; fCutsRD[13][3]= 1; fCutsRD[13][5]= 1; fCutsRD[13][7]= 1; fCutsRD[13][9]= 1; fCutsRD[13][11]= 1; fCutsRD[13][13]= 1; fCutsRD[13][15]= 1; fCutsRD[13][17]= 1; fCutsRD[13][19]= 1; fCutsRD[13][20]= 1; fCutsRD[13][21]= 1; fCutsRD[13][22]= 1; fCutsRD[13][23]= 1; fCutsRD[13][24]= 1;
    fCutsRD[14][0] = 0.85; fCutsRD[14][1] = 0.88; fCutsRD[14][3]= -1; fCutsRD[14][5]= -1; fCutsRD[14][7]= -1; fCutsRD[14][9]= -1; fCutsRD[14][11]= -1; fCutsRD[14][13]= -1; fCutsRD[14][15]= -1; fCutsRD[14][17]= -1; fCutsRD[14][19]= -1; fCutsRD[14][20]= -1; fCutsRD[14][21]= -1; fCutsRD[14][22]= -1; fCutsRD[14][23]= -1; fCutsRD[14][24]= -1;
    fCutsRD[15][0] = 0; fCutsRD[15][1] = 2.7; fCutsRD[15][3]= 3; fCutsRD[15][5]= 0; fCutsRD[15][7]= 0; fCutsRD[15][9]= 0; fCutsRD[15][11]= 0; fCutsRD[15][13]= 0; fCutsRD[15][15]= 0; fCutsRD[15][17]= 0; fCutsRD[15][19]= 0; fCutsRD[15][20]= 0; fCutsRD[15][21]= 0; fCutsRD[15][22]= 0; fCutsRD[15][23]= 0; fCutsRD[15][24]= 0;
    
    for(int ii = 0; ii < nvars; ii++){
      fCutsRD[ii][2] = fCutsRD[ii][1];
      fCutsRD[ii][4] = fCutsRD[ii][3];
      fCutsRD[ii][6] = fCutsRD[ii][5];
      fCutsRD[ii][8] = fCutsRD[ii][7];
      fCutsRD[ii][10] = fCutsRD[ii][9];
      fCutsRD[ii][12] = fCutsRD[ii][11];
      fCutsRD[ii][14] = fCutsRD[ii][13];
      fCutsRD[ii][16] = fCutsRD[ii][15];
      fCutsRD[ii][18] = fCutsRD[ii][17];
    }
    
    cutsDstartoKpipi->SetCuts(nvars,nptbins,fCutsRD);
    
    //pid settings
    AliAODPidHF* pidObj=new AliAODPidHF();
    // pidObj->SetName("pid4DSatr");
    Int_t mode=1;
    Double_t priors[5]={0.01,0.001,0.3,0.3,0.3};
    pidObj->SetPriors(priors,5);
    pidObj->SetMatch(mode);
    pidObj->SetSigma(0,3); // TPC
    pidObj->SetSigma(3,3); // TOF
    pidObj->SetTPC(kTRUE);
    pidObj->SetTOF(kTRUE);
    
    pidObj->SetOldPid(kFALSE);
    
    cutsDstartoKpipi->SetPidHF(pidObj);
    Bool_t pidflag=kTRUE;
    cutsDstartoKpipi->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for analysis cuts"<<endl;
    else cout<<"PID is not used for analysis cuts"<<endl;
    
  }
  //Do not recalculate the vertex for Dstar (we don't do this in std analyses)
  cutsDstartoKpipi->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
  
  //event selection
  cutsDstartoKpipi->SetUsePhysicsSelection(kTRUE);
  cutsDstartoKpipi->SetTriggerClass("");
  //Since X/08/19 we set the triggers in wagon code/runAnalysis script if needed
  cutsDstartoKpipi->SetTriggerMask(AliVEvent::kAny);
  cutsDstartoKpipi->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent);
  cutsDstartoKpipi->SetMinContribPileupMV(5);
  cutsDstartoKpipi->SetMaxVtxZ(10.);
  cutsDstartoKpipi->SetCutOnzVertexSPD(3);
  cutsDstartoKpipi->SetMinVtxContr(1);
  
  cout<<"This is the object I'm going to save:"<<endl;
  cutsDstartoKpipi->SetName(nameCuts.Data());
  cutsDstartoKpipi->SetTitle(nameCuts.Data());
  cutsDstartoKpipi->PrintAll();
  
  return cutsDstartoKpipi;
  
}
