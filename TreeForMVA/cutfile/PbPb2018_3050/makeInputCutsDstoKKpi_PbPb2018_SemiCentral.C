#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsDstoKKpi.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="DstoKKpiFilteringCuts"
 whichCuts=1, nameCuts="DstoKKpiAnalysisCuts"
 */


AliRDHFCutsDstoKKpi *makeInputCutsDstoKKpi(Int_t whichCuts=0, TString nameCuts="DstoKKpiFilteringCuts", Float_t minc=30., Float_t maxc=50., Bool_t isMC=kFALSE, Int_t OptPreSelect = 1, Int_t TPCClsPID = 50, Bool_t PIDcorrection=kTRUE)
{
  
  cout << "\n\033[1;31m--Warning (08/06/20)--\033[0m\n";
  cout << "  Don't blindly trust these cuts." << endl;
  cout << "  Relatively old and never tested." << endl;
  cout << "\033[1;31m----------------------\033[0m\n\n";
  
  AliRDHFCutsDstoKKpi* cuts=new AliRDHFCutsDstoKKpi();
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
  esdTrackCuts->SetPtRange(0.4,1.e10);
  esdTrackCuts->SetMaxDCAToVertexXY(1.);
  esdTrackCuts->SetMaxDCAToVertexZ(1.);
  esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
  
  cuts->AddTrackCuts(esdTrackCuts);
  //UPDATE 08/06/20, set to kTRUE as should be done for all other HF hadrons (pK0s was true, others false)
  cuts->SetUseTrackSelectionWithFilterBits(kTRUE);
  //UPDATE 08/06/20, Add cut on TPC clusters for PID (similar to geometrical cut)
  cuts->SetMinNumTPCClsForPID(TPCClsPID);

  if(whichCuts==0){
    
    cuts->SetStandardCutsPbPb2010();
    cuts->AddTrackCuts(esdTrackCuts);
    
    const Int_t nptbins=2;
    Float_t ptlimits[nptbins+1]={0.,5.,1000000.};
    cuts->SetPtBins(nptbins+1,ptlimits);
    
    Float_t** anacutsval=new Float_t*[20];
    for(Int_t ic=0;ic<20;ic++){anacutsval[ic]=new Float_t[nptbins];}
    
    anacutsval[0][0]=0.25;
    anacutsval[1][0]=0.4;
    anacutsval[2][0]=0.4;
    anacutsval[3][0]=0.;
    anacutsval[4][0]=0.;
    anacutsval[5][0]=0.;
    anacutsval[6][0]=0.04;
    anacutsval[7][0]=0.03;
    anacutsval[8][0]=0.;
    anacutsval[9][0]=0.96;
    anacutsval[10][0]=0.;
    anacutsval[11][0]=1000.0;
    anacutsval[12][0]=0.010;
    anacutsval[13][0]=0.001;
    anacutsval[14][0]=0.;
    anacutsval[15][0]=1.;
    anacutsval[16][0]=0.;
    anacutsval[17][0]=0.;
    anacutsval[18][0]=3.;
    anacutsval[19][0]=0.96;
    
    anacutsval[0][1]=0.3;
    anacutsval[1][1]=0.4;
    anacutsval[2][1]=0.4;
    anacutsval[3][1]=0.;
    anacutsval[4][1]=0.;
    anacutsval[5][1]=0.;
    anacutsval[6][1]=0.06;
    anacutsval[7][1]=0.03;
    anacutsval[8][1]=0.;
    anacutsval[9][1]=0.95;
    anacutsval[10][1]=0.;
    anacutsval[11][1]=100000.;
    anacutsval[12][1]=0.015;
    anacutsval[13][1]=0.0001;
    anacutsval[14][1]=-1.;
    anacutsval[15][1]=1.;
    anacutsval[16][1]=0.;
    anacutsval[17][1]=0.;
    anacutsval[18][1]=2.;
    anacutsval[19][1]=-1.;
    
    cuts->SetCuts(20,nptbins,anacutsval);
    cuts->SetMinPtCandidate(2.);
    
    cuts->SetUsePID(kTRUE);
    cuts->SetPidOption(0); //0=kConservative,1=kStrong
  }
  else if(whichCuts==1){
    
    const Int_t nptbins=9;
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    ptbins[0]=2.;
    ptbins[1]=3.;
    ptbins[2]=4.;
    ptbins[3]=5.;
    ptbins[4]=6.;
    ptbins[5]=8.;
    ptbins[6]=12.;
    ptbins[7]=16.;
    ptbins[8]=24.;
    ptbins[9]=36.;
    
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
      anacutsval[8][ipt]=0.;
      anacutsval[10][ipt]=0.;
      anacutsval[11][ipt]=1000.0;
      anacutsval[13][ipt]=0.001;
    }
    /*
     
     Cut list                                                rejection condition
     0           "inv. mass [GeV]",                          invmassDS-massDspdg>fCutsRD
     1			"pTK [GeV/c]",                              pTK<fCutsRd
     2			"pTPi [GeV/c]",                             pTPi<fCutsRd
     3			"d0K [cm]",                                 d0K<fCutsRd
     4			"d0Pi [cm]",                                d0Pi<fCutsRd
     5			"dist12 [cm]",                              dist12<fCutsRd
     6			"sigmavert [cm]",                           sigmavert>fCutsRd
     7			"decLen [cm]",                              decLen<fCutsRD
     8			"ptMax [GeV/c]",                            ptMax<fCutsRD
     9			"cosThetaPoint",                            CosThetaPoint<fCutsRD
     10			"Sum d0^2 (cm^2)",                          sumd0<fCutsRD
     11			"dca [cm]",                                 dca(i)>fCutsRD
     12			"inv. mass (Mphi-MKK) [GeV]",               invmass-pdg>fCutsRD
     13			"inv. mass (MKo*-MKpi) [GeV]"};             invmass-pdg>fCutsRD
     14    		"Abs(CosineKpiPhiRFrame)^3",
     15  		"CosPiDsLabFrame"};
     16  		"DecLengthXY
     17  		"NormDecayLength"};
     18  		"NormDecayLengthXY"};
     19  		"cosThetaPointXY"};
     */
    
    anacutsval[6][0]=0.020;   //sigmavert
    anacutsval[7][0]=0.04;   //decay length
    anacutsval[9][0]=0.985;   //cosP
    anacutsval[12][0]=0.005; //Mass Phi
    anacutsval[14][0]=0.2;  //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][0]=1.0;  //CosP labFrame
    anacutsval[16][0]=0.04;  //decayXY
    anacutsval[17][0]=0.;    //normdecay
    anacutsval[18][0]=8.0;   //normdecayXY
    anacutsval[19][0]=0.99;  //CosPXY
    
    anacutsval[6][1]=0.025;   //sigmavert
    anacutsval[7][1]=0.04;   //decay length
    anacutsval[9][1]=0.985;   //cosP
    anacutsval[12][1]=0.005; //Mass Phi
    anacutsval[14][1]=0.15;  //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][1]=1.0;  //CosP labFrame
    anacutsval[16][1]=0.04;  //decayXY
    anacutsval[17][1]=0.;    //normdecay
    anacutsval[18][1]=8.0;   //normdecayXY
    anacutsval[19][1]=0.99;  //CosPXY
    
    anacutsval[6][2]=0.030;   //sigmavert
    anacutsval[7][2]=0.04;   //decay length
    anacutsval[9][2]=0.985;   //cosP
    anacutsval[12][2]=0.005; //Mass Phi
    anacutsval[14][2]=0.15;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][2]=1.0;   //CosP labFrame
    anacutsval[16][2]=0.04;  //decayXY
    anacutsval[17][2]=0.;    //normdecay
    anacutsval[18][2]=7.0;   //normdecayXY
    anacutsval[19][2]=0.99;  //CosPXY
    
    anacutsval[6][3]=0.030;   //sigmavert
    anacutsval[7][3]=0.04;   //decay length
    anacutsval[9][3]=0.98;   //cosP
    anacutsval[12][3]=0.005; //Mass Phi
    anacutsval[14][3]=0.15;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][3]=1.0;  //CosP labFrame
    anacutsval[16][3]=0.04;   //decayXY
    anacutsval[17][3]=0.;    //normdecay
    anacutsval[18][3]=7.0;   //normdecayXY
    anacutsval[19][3]=0.985;  //CosPXY
    
    anacutsval[6][4]=0.030;   //sigmavert
    anacutsval[7][4]=0.04;   //decay length
    anacutsval[9][4]=0.98;   //cosP
    anacutsval[12][4]=0.005; //Mass Phi
    anacutsval[14][4]=0.05;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][4]=1.0;  //CosP labFrame
    anacutsval[16][4]=0.04;   //decayXY
    anacutsval[17][4]=0.;    //normdecay
    anacutsval[18][4]=7.0;   //normdecayXY
    anacutsval[19][4]=0.985;  //CosPXY
    
    anacutsval[6][5]=0.030;   //sigmavert
    anacutsval[7][5]=0.05;   //decay length
    anacutsval[9][5]=0.98;   //cosP
    anacutsval[12][5]=0.005; //Mass Phi
    anacutsval[14][5]=0.05;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][5]=1.0;  //CosP labFrame
    anacutsval[16][5]=0.05;   //decayXY
    anacutsval[17][5]=0.;    //normdecay
    anacutsval[18][5]=6.0;   //normdecayXY
    anacutsval[19][5]=0.985;  //CosPXY
    
    anacutsval[6][6]=0.035;   //sigmavert
    anacutsval[7][6]=0.05;   //decay length
    anacutsval[9][6]=0.98;   //cosP
    anacutsval[12][6]=0.005; //Mass Phi
    anacutsval[14][6]=0.;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][6]=1.0;  //CosP labFrame
    anacutsval[16][6]=0.05;   //decayXY
    anacutsval[17][6]=0.;    //normdecay
    anacutsval[18][6]=6.0;   //normdecayXY
    anacutsval[19][6]=0.985;  //CosPXY
    
    anacutsval[6][7]=0.040;   //sigmavert
    anacutsval[7][7]=0.05;   //decay length
    anacutsval[9][7]=0.975;   //cosP
    anacutsval[12][7]=0.005; //Mass Phi
    anacutsval[14][7]=0.;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][7]=1.0;  //CosP labFrame
    anacutsval[16][7]=0.05;   //decayXY
    anacutsval[17][7]=0.;    //normdecay
    anacutsval[18][7]=5.0;   //normdecayXY
    anacutsval[19][7]=0.98;  //CosPXY
    
    anacutsval[6][8]=0.045;   //sigmavert
    anacutsval[7][8]=0.05;   //decay length
    anacutsval[9][8]=0.97;   //cosP
    anacutsval[12][8]=0.005; //Mass Phi
    anacutsval[14][8]=0.;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][8]=1.0;  //CosP labFrame
    anacutsval[16][8]=0.05;   //decayXY
    anacutsval[17][8]=0.;    //normdecay
    anacutsval[18][8]=4.0;   //normdecayXY
    anacutsval[19][8]=0.975;  //CosPXY
    
    Float_t topomCuts[nptbins] = {1.5,2.,2.,2.5,2.5,2.5,3.,3.,3.}; //topomatic
    Float_t d0Cuts[nptbins] = {70.,70.,70.,70.,70.,70.,70.,70.,70.}; //impparXY
    
    cuts->SetPtBins(nptbins+1,ptbins);
    cuts->SetCuts(nvars,nptbins,anacutsval);
    cuts->Setd0MeasMinusExpCut(nptbins,topomCuts);
    cuts->Setd0Cut(nptbins,d0Cuts);
    
    cuts->SetUsePID(kTRUE);
    cuts->SetPidOption(1); //0=kConservative,1=kStrong
    cuts->SetMaxPtStrongPid(8.);
    
    cuts->SetMinPtCandidate(2.);
    cuts->SetMaxPtCandidate(36.);
    
  }
  
  //UPDATE 08/06/20: PreSelect, acting before FillRecoCasc.
  //NOTE: actual function not implemented for all HF hadrons yet (please check)
  cuts->SetUsePreSelect(OptPreSelect);

  //Do not recalculate the vertex
  cuts->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
  
  //Temporary PID fix for 2018 PbPb (only to be used on data)
  if(!isMC && PIDcorrection) cuts->EnableNsigmaDataDrivenCorrection(kTRUE, AliAODPidHF::kPbPb3050);

  //event selection
  cuts->SetUsePhysicsSelection(kTRUE);
  cuts->SetTriggerClass("");
  if(!isMC) cuts->SetTriggerMask(AliVEvent::kINT7 | AliVEvent::kSemiCentral);
  else      cuts->SetTriggerMask(AliVEvent::kAny);
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


