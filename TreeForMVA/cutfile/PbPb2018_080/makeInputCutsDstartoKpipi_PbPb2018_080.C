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


AliRDHFCutsDStartoKpipi *makeInputCutsDstartoKpipi(Int_t whichCuts=0, TString nameCuts="DstartoKpipiFilteringCuts", Float_t minc=0., Float_t maxc=80., Bool_t isMC=kFALSE, Int_t OptPreSelect = 1, Int_t TPCClsPID = 50, Bool_t PIDcorrection=kTRUE)
{
  
  cout << "\n\033[1;31m--Warning (08/06/20)--\033[0m\n";
  cout << "  Don't blindly trust these cuts." << endl;
  cout << "  Relatively old and never tested." << endl;
  cout << "\033[1;31m----------------------\033[0m\n\n";

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
  esdTrackCuts->SetPtRange(0.4,1.e10);
  esdTrackCuts->SetMaxDCAToVertexXY(1.);
  esdTrackCuts->SetMaxDCAToVertexZ(1.);
  cutsDstartoKpipi->AddTrackCuts(esdTrackCuts);
  
  //soft pion selections
  AliESDtrackCuts* esdTrackCutsSoftPi=new AliESDtrackCuts();
  esdTrackCutsSoftPi->SetRequireSigmaToVertex(kFALSE);
  esdTrackCutsSoftPi->SetRequireTPCRefit(kTRUE);
  esdTrackCutsSoftPi->SetRequireITSRefit(kTRUE);
  esdTrackCutsSoftPi->SetMinNClustersITS(2);
  esdTrackCutsSoftPi->SetEtaRange(-0.8,+0.8);
  esdTrackCutsSoftPi->SetPtRange(0.0,1.e10);
  cutsDstartoKpipi->AddTrackCutsSoftPi(esdTrackCutsSoftPi);
  
  //UPDATE 08/06/20, set to kTRUE as should be done for all other HF hadrons (pK0s was true, others false)
  cutsDstartoKpipi->SetUseTrackSelectionWithFilterBits(kTRUE);
  
  //UPDATE 08/06/20, Add cut on TPC clusters for PID (similar to geometrical cut)
  cutsDstartoKpipi->SetMinNumTPCClsForPID(TPCClsPID);

  if(whichCuts==0){
    const Int_t nvars=16;
    const Int_t nptbinsDstar=2;
    Float_t* ptbins;
    ptbins=new Float_t[nptbinsDstar+1];
    ptbins[0]=0.;
    ptbins[1]=6.;
    ptbins[2]=999.;
    
    cutsDstartoKpipi->SetPtBins(nptbinsDstar+1,ptbins);
    
    Float_t** rdcutsvalmine;
    rdcutsvalmine=new Float_t*[nvars];
    for(Int_t iv=0;iv<nvars;iv++){
      rdcutsvalmine[iv]=new Float_t[nptbinsDstar];
    }
    //0-5
    rdcutsvalmine[0][0]=0.05;     //D0 inv mass window
    rdcutsvalmine[1][0]=0.05;     // dca
    rdcutsvalmine[2][0]=0.9;      // thetastar
    rdcutsvalmine[3][0]=0.6;      // pt Pion
    rdcutsvalmine[4][0]=0.6;      // Pt Kaon
    rdcutsvalmine[5][0]=0.1;      // d0K
    rdcutsvalmine[6][0]=0.1;      // d0Pi
    rdcutsvalmine[7][0]=-0.00001; // d0xd0
    rdcutsvalmine[8][0]=0.95;     // costhetapoint
    rdcutsvalmine[9][0]=0.07;     // Dstar inv mass window
    rdcutsvalmine[10][0]=0.02;    // half width of (M_Kpipi-M_D0)
    rdcutsvalmine[11][0]=0.1;     // Pt min of Pi soft
    rdcutsvalmine[12][0]=100.;    // Pt max of pi soft
    rdcutsvalmine[13][0]=9999.;   // theta
    rdcutsvalmine[14][0]=0.97;    // |cosThetaPointXY|
    rdcutsvalmine[15][0]=3.;     // NormDecayLenghtXY
    //6-999
    rdcutsvalmine[0][1]=0.12;   //D0 inv mass window
    rdcutsvalmine[1][1]=0.06;   // dca
    rdcutsvalmine[2][1]=0.9;    // thetastar
    rdcutsvalmine[3][1]=0.5;    // pt Pion
    rdcutsvalmine[4][1]=0.5;    // Pt Kaon
    rdcutsvalmine[5][1]=0.1;    // d0K
    rdcutsvalmine[6][1]=0.1;    // d0Pi
    rdcutsvalmine[7][1]=0.0001; // d0xd0
    rdcutsvalmine[8][1]=0.7;    // costhetapoint
    rdcutsvalmine[9][1]=0.2;   // Dstar inv mass window
    rdcutsvalmine[10][1]=0.02;  // half width of (M_Kpipi-M_D0)
    rdcutsvalmine[11][1]=0.1;   // Pt min of Pi soft
    rdcutsvalmine[12][1]=100.;  // Pt max of pi soft
    rdcutsvalmine[13][1]=9999.; // theta
    rdcutsvalmine[14][1]=0.8;   // |cosThetaPointXY|
    rdcutsvalmine[15][1]=0.;    // NormDecayLenghtXY
    
    cutsDstartoKpipi->SetMinPtCandidate(2.);
    cutsDstartoKpipi->SetCuts(nvars,nptbinsDstar,rdcutsvalmine);
    
    Bool_t pidflag=kTRUE;
    cutsDstartoKpipi->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used"<<endl;
    else cout<<"PID is not used"<<endl;
    
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
  }
  else if(whichCuts==1){
    
    const Int_t nvars=16;
    const Int_t nptbins=14;//13;
    
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    ptbins[0]=1.;
    ptbins[1]=2.;
    ptbins[2]=3.;
    ptbins[3]=4.;
    ptbins[4]=5.;
    ptbins[5]=6.;
    ptbins[6]=7.;
    ptbins[7]=8.;
    ptbins[8]=10.;
    ptbins[9]=12.;
    ptbins[10]=16.;
    ptbins[11]=20.;
    ptbins[12]=24.;
    ptbins[13]=36.;
    ptbins[14]=50.;
    
    cutsDstartoKpipi->SetPtBins(nptbins+1,ptbins);
    
    Float_t** rdcutsvalmine;
    rdcutsvalmine=new Float_t*[nvars];
    for(Int_t iv=0;iv<nvars;iv++){
      rdcutsvalmine[iv]=new Float_t[nptbins];
    }
    
    //1-2
    rdcutsvalmine[0][0]=0.03; //0.024;
    rdcutsvalmine[1][0]=0.025; //0.021; // vary this
    rdcutsvalmine[2][0]=0.8;
    rdcutsvalmine[3][0]=0.9; //0.8;
    rdcutsvalmine[4][0]=0.9; //0.8;
    rdcutsvalmine[5][0]=0.1;
    rdcutsvalmine[6][0]=0.1;
    rdcutsvalmine[7][0]=-0.00045;
    rdcutsvalmine[8][0]=0.99; //0.95; // vary this
    rdcutsvalmine[9][0]=0.3;
    rdcutsvalmine[10][0]=0.15;
    rdcutsvalmine[11][0]=0.1; //0.05;
    rdcutsvalmine[12][0]=0.5;
    rdcutsvalmine[13][0]=1.;
    rdcutsvalmine[14][0]=0.998;
    rdcutsvalmine[15][0]=8.;
    //2-3
    rdcutsvalmine[0][1]=0.03; //0.024;
    rdcutsvalmine[1][1]=0.025;  // vary this
    rdcutsvalmine[2][1]=0.8;
    rdcutsvalmine[3][1]=0.9;
    rdcutsvalmine[4][1]=0.9; //1.0;
    rdcutsvalmine[5][1]=0.1;
    rdcutsvalmine[6][1]=0.1;
    rdcutsvalmine[7][1]=-0.00045;
    rdcutsvalmine[8][1]=0.99; //0.97;  // vary this
    rdcutsvalmine[9][1]=0.3;
    rdcutsvalmine[10][1]=0.15;
    rdcutsvalmine[11][1]=0.1; //0.05;
    rdcutsvalmine[12][1]=0.5;
    rdcutsvalmine[13][1]=1.;
    rdcutsvalmine[14][1]=0.998;
    rdcutsvalmine[15][1]=8.;
    //3-4
    rdcutsvalmine[0][2]=0.03; //0.024;
    rdcutsvalmine[1][2]=0.022;  // vary this
    rdcutsvalmine[2][2]=0.8;
    rdcutsvalmine[3][2]=0.9;
    rdcutsvalmine[4][2]=0.9; //1.0;
    rdcutsvalmine[5][2]=0.1; //0.09;
    rdcutsvalmine[6][2]=0.1; //0.09;
    rdcutsvalmine[7][2]=-0.00045;
    rdcutsvalmine[8][2]=0.98; //0.99;//0.98; // vary this
    rdcutsvalmine[9][2]=0.3;
    rdcutsvalmine[10][2]=0.15;
    rdcutsvalmine[11][2]=0.1; //0.05;
    rdcutsvalmine[12][2]=0.5;
    rdcutsvalmine[13][2]=1.;
    rdcutsvalmine[14][2]=0.998;
    rdcutsvalmine[15][2]=8.;
    //4-5
    rdcutsvalmine[0][3]=0.03;
    rdcutsvalmine[1][3]=0.021; // vary this
    rdcutsvalmine[2][3]=0.8;
    rdcutsvalmine[3][3]=0.9;
    rdcutsvalmine[4][3]=0.9; //1.0;
    rdcutsvalmine[5][3]=0.1;
    rdcutsvalmine[6][3]=0.1;
    rdcutsvalmine[7][3]=-0.00035;
    rdcutsvalmine[8][3]=0.98;
    rdcutsvalmine[9][3]=0.3; //0.2;
    rdcutsvalmine[10][3]=0.15;
    rdcutsvalmine[11][3]=0.1; //0.05;
    rdcutsvalmine[12][3]=10.;
    rdcutsvalmine[13][3]=1.;
    rdcutsvalmine[14][3]=0.998;
    rdcutsvalmine[15][3]=6.5;
    //5-6
    rdcutsvalmine[0][4]=0.03;
    rdcutsvalmine[1][4]=0.021;
    rdcutsvalmine[2][4]=1.0;
    rdcutsvalmine[3][4]=0.9;
    rdcutsvalmine[4][4]=0.9; //1.0;
    rdcutsvalmine[5][4]=0.1; //0.08;
    rdcutsvalmine[6][4]=0.1; //0.08;
    rdcutsvalmine[7][4]=-0.00023;
    rdcutsvalmine[8][4]=0.93;
    rdcutsvalmine[9][4]=0.3;
    rdcutsvalmine[10][4]=0.3;
    rdcutsvalmine[11][4]=0.2; //0.25;
    rdcutsvalmine[12][4]=10.;
    rdcutsvalmine[13][4]=1.;
    rdcutsvalmine[14][4]=0.998;
    rdcutsvalmine[15][4]=6.5; //6.;
    //6-7
    rdcutsvalmine[0][5]=0.034;
    rdcutsvalmine[1][5]=0.021;
    rdcutsvalmine[2][5]=1.0;
    rdcutsvalmine[3][5]=0.9; //1.0;
    rdcutsvalmine[4][5]=0.9; //1.0;
    rdcutsvalmine[5][5]=0.1; //0.12;
    rdcutsvalmine[6][5]=0.1; //0.12;
    rdcutsvalmine[7][5]=-0.0002; //-0.0001;
    rdcutsvalmine[8][5]=0.93;
    rdcutsvalmine[9][5]=0.3;
    rdcutsvalmine[10][5]=0.15;
    rdcutsvalmine[11][5]=0.3;
    rdcutsvalmine[12][5]=100;
    rdcutsvalmine[13][5]=1.0;
    rdcutsvalmine[14][5]=0.998;
    rdcutsvalmine[15][5]=6.5;
    //7-8
    rdcutsvalmine[0][6]=0.036;
    rdcutsvalmine[1][6]=0.021;
    rdcutsvalmine[2][6]=1.0;
    rdcutsvalmine[3][6]=0.9; //1.0;
    rdcutsvalmine[4][6]=0.9; //1.0;
    rdcutsvalmine[5][6]=0.1;
    rdcutsvalmine[6][6]=0.1;
    rdcutsvalmine[7][6]=-0.000127;
    rdcutsvalmine[8][6]=0.93;
    rdcutsvalmine[9][6]=0.3;
    rdcutsvalmine[10][6]=0.15;
    rdcutsvalmine[11][6]=0.3;
    rdcutsvalmine[12][6]=100;
    rdcutsvalmine[13][6]=1.;
    rdcutsvalmine[14][6]=0.998;
    rdcutsvalmine[15][6]=6.;
    //8-10
    rdcutsvalmine[0][7]=0.055;
    rdcutsvalmine[1][7]=0.021;
    rdcutsvalmine[2][7]=1.0;
    rdcutsvalmine[3][7]=0.9;
    rdcutsvalmine[4][7]=0.9;
    rdcutsvalmine[5][7]=0.15;
    rdcutsvalmine[6][7]=0.15;
    rdcutsvalmine[7][7]=-7.5e-05;
    rdcutsvalmine[8][7]=0.93;
    rdcutsvalmine[9][7]=0.3;
    rdcutsvalmine[10][7]=0.15;
    rdcutsvalmine[11][7]=0.3;
    rdcutsvalmine[12][7]=100;
    rdcutsvalmine[13][7]=1.;
    rdcutsvalmine[14][7]=0.998;
    rdcutsvalmine[15][7]=5.;
    //10-12
    rdcutsvalmine[0][8]=0.055;
    rdcutsvalmine[1][8]=0.021;
    rdcutsvalmine[2][8]=1.0;
    rdcutsvalmine[3][8]=0.9;
    rdcutsvalmine[4][8]=0.9;
    rdcutsvalmine[5][8]=0.15;
    rdcutsvalmine[6][8]=0.15;
    rdcutsvalmine[7][8]=-7.5e-05;
    rdcutsvalmine[8][8]=0.93;
    rdcutsvalmine[9][8]=0.3;
    rdcutsvalmine[10][8]=0.15;
    rdcutsvalmine[11][8]=0.3;
    rdcutsvalmine[12][8]=100;
    rdcutsvalmine[13][8]=1.;
    rdcutsvalmine[14][8]=0.998;
    rdcutsvalmine[15][8]=5.;
    //12-16
    rdcutsvalmine[0][9]=0.074;
    rdcutsvalmine[1][9]=0.021;
    rdcutsvalmine[2][9]=1.0;
    rdcutsvalmine[3][9]=0.7;
    rdcutsvalmine[4][9]=0.7;
    rdcutsvalmine[5][9]=0.15;
    rdcutsvalmine[6][9]=0.15;
    rdcutsvalmine[7][9]=-7.5e-05;
    rdcutsvalmine[8][9]=0.93;
    rdcutsvalmine[9][9]=0.3;
    rdcutsvalmine[10][9]=0.15;
    rdcutsvalmine[11][9]=0.3;
    rdcutsvalmine[12][9]=100;
    rdcutsvalmine[13][9]=1.;
    rdcutsvalmine[14][9]=0.99;
    rdcutsvalmine[15][9]=3.7;
    //16-20
    rdcutsvalmine[0][10]=0.074;
    rdcutsvalmine[1][10]=0.021;
    rdcutsvalmine[2][10]=1.0;
    rdcutsvalmine[3][10]=0.5;
    rdcutsvalmine[4][10]=0.5;
    rdcutsvalmine[5][10]=0.15;
    rdcutsvalmine[6][10]=0.15;
    rdcutsvalmine[7][10]=-5e-05;
    rdcutsvalmine[8][10]=0.92;
    rdcutsvalmine[9][10]=0.3;
    rdcutsvalmine[10][10]=0.15;
    rdcutsvalmine[11][10]=0.3;
    rdcutsvalmine[12][10]=100;
    rdcutsvalmine[13][10]=1.;
    rdcutsvalmine[14][10]=0.99;
    rdcutsvalmine[15][10]=2.;
    //20-24
    rdcutsvalmine[0][11]=0.074;
    rdcutsvalmine[1][11]=0.021;
    rdcutsvalmine[2][11]=1.0;
    rdcutsvalmine[3][11]=0.5;
    rdcutsvalmine[4][11]=0.5;
    rdcutsvalmine[5][11]=0.15;
    rdcutsvalmine[6][11]=0.15;
    rdcutsvalmine[7][11]=-5e-05;
    rdcutsvalmine[8][11]=0.92;
    rdcutsvalmine[9][11]=0.15;
    rdcutsvalmine[10][11]=0.15;
    rdcutsvalmine[11][11]=0.3;
    rdcutsvalmine[12][11]=100;
    rdcutsvalmine[13][11]=1.;
    rdcutsvalmine[14][11]=0.99;
    rdcutsvalmine[15][11]=1.;
    //24-36
    rdcutsvalmine[0][12]=0.094;
    rdcutsvalmine[1][12]=0.02;
    rdcutsvalmine[2][12]=1.0;
    rdcutsvalmine[3][12]=0.5;
    rdcutsvalmine[4][12]=0.5;
    rdcutsvalmine[5][12]=0.2;
    rdcutsvalmine[6][12]=0.2;
    rdcutsvalmine[7][12]=0.0004;
    rdcutsvalmine[8][12]=0.85;
    rdcutsvalmine[9][12]=0.15;
    rdcutsvalmine[10][12]=0.15;
    rdcutsvalmine[11][12]=0.3;
    rdcutsvalmine[12][12]=100;
    rdcutsvalmine[13][12]=1.;
    rdcutsvalmine[14][12]=0.9;
    rdcutsvalmine[15][12]=0;
    //36-50
    rdcutsvalmine[0][13]=0.094;
    rdcutsvalmine[1][13]=0.02;
    rdcutsvalmine[2][13]=1.0;
    rdcutsvalmine[3][13]=0.5;
    rdcutsvalmine[4][13]=0.5;
    rdcutsvalmine[5][13]=0.2;
    rdcutsvalmine[6][13]=0.2;
    rdcutsvalmine[7][13]=0.0004;
    rdcutsvalmine[8][13]=0.8;
    rdcutsvalmine[9][13]=0.15;
    rdcutsvalmine[10][13]=0.15;
    rdcutsvalmine[11][13]=0.3;
    rdcutsvalmine[12][13]=100;
    rdcutsvalmine[13][13]=1.;
    rdcutsvalmine[14][13]=0.9;
    rdcutsvalmine[15][13]=0;
    
    cutsDstartoKpipi->SetCuts(nvars,nptbins,rdcutsvalmine);
    
    Bool_t pidflag=kTRUE;
    cutsDstartoKpipi->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used"<<endl;
    else cout<<"PID is not used"<<endl;
    
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
  }
  
  //UPDATE 08/06/20: PreSelect, acting before FillRecoCasc.
  //NOTE: actual function not implemented for all HF hadrons yet (please check)
  cutsDstartoKpipi->SetUsePreSelect(OptPreSelect);

  //Do not recalculate the vertex
  cutsDstartoKpipi->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
  
  //Temporary PID fix for 2018 PbPb (only to be used on data)
  //NOT READY FOR 0-80!
  if(!isMC && PIDcorrection) cutsDstartoKpipi->EnableNsigmaDataDrivenCorrection(kTRUE, AliAODPidHF::kPbPb010);
  
  //event selection
  cutsDstartoKpipi->SetUsePhysicsSelection(kTRUE);
  cutsDstartoKpipi->SetTriggerClass("");
  if(!isMC) cutsDstartoKpipi->SetTriggerMask(AliVEvent::kINT7 | AliVEvent::kSemiCentral | AliVEvent::kCentral);
  else      cutsDstartoKpipi->SetTriggerMask(AliVEvent::kAny);
  cutsDstartoKpipi->SetMinCentrality(minc);
  cutsDstartoKpipi->SetMaxCentrality(maxc);
  cutsDstartoKpipi->SetUseCentrality(AliRDHFCuts::kCentV0M); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,kCentInvalid
  cutsDstartoKpipi->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
  cutsDstartoKpipi->SetMaxVtxZ(10.);
  cutsDstartoKpipi->SetCutOnzVertexSPD(3);
  
  cout<<"This is the object I'm going to save:"<<endl;
  cutsDstartoKpipi->SetName(nameCuts.Data());
  cutsDstartoKpipi->SetTitle(nameCuts.Data());
  cutsDstartoKpipi->PrintAll();
  
  return cutsDstartoKpipi;
}
