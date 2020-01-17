#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsDstoKKpi.h>
#include <TClonesArray.h>
#include <TParameter.h>

/*
 whichCuts=0, nameCuts="BstoDspiFilteringCuts"
 whichCuts=1, nameCuts="BstoDspiAnalysisCuts"
 */


AliRDHFCutsDstoKKpi *makeInputCutsBstoDspi(Int_t whichCuts=0, TString nameCuts="BstoDspiFilteringCuts", Float_t minc=0.,Float_t maxc=100.,Bool_t usePID=kTRUE,Bool_t usePreSelect=kTRUE)
{
  
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
  //  esdTrackCuts->SetMinNClustersTPC(50);
  esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
  esdTrackCuts->SetMinNCrossedRowsTPC(70);
  esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);
  esdTrackCuts->SetEtaRange(-0.8,0.8);
  esdTrackCuts->SetPtRange(0.5,1.e10);
  esdTrackCuts->SetMaxDCAToVertexXY(1.);
  esdTrackCuts->SetMaxDCAToVertexZ(1.);
  esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0015*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");

  cuts->AddTrackCuts(esdTrackCuts);
  cuts->SetUseTrackSelectionWithFilterBits(kFALSE);
  
  if(whichCuts==0){
    cuts->SetStandardCutsPbPb2010();
    cuts->AddTrackCuts(esdTrackCuts);

    const Int_t nptbins=3;
    Float_t ptlimits[nptbins+1]={0.,10.,1000000.};
    cuts->SetPtBins(nptbins+1,ptlimits);
    
    Float_t** anacutsval=new Float_t*[20];
    for(Int_t ic=0;ic<20;ic++){anacutsval[ic]=new Float_t[nptbins];}
    //Used values for Ds analysis taken at 20/08/19 but tightened InvMass cut (0.25/0.3->0.12)
    //04-12-20: Used filtering cuts for Ds, tightening Ds and phi mass window, set min pT to 2
    
    /*
     Cut list                                           rejection condition
     0      "inv. mass [GeV]",                          invmassDS-massDspdg>fCutsRD
     1      "pTK [GeV/c]",                              pTK<fCutsRd
     2      "pTPi [GeV/c]",                             pTPi<fCutsRd
     3      "d0K [cm]",                                 d0K<fCutsRd
     4      "d0Pi [cm]",                                d0Pi<fCutsRd
     5      "dist12 [cm]",                              dist12<fCutsRd
     6      "sigmavert [cm]",                           sigmavert>fCutsRd
     7      "decLen [cm]",                              decLen<fCutsRD
     8      "ptMax [GeV/c]",                            ptMax<fCutsRD
     9      "cosThetaPoint",                            CosThetaPoint<fCutsRD
     10     "Sum d0^2 (cm^2)",                          sumd0<fCutsRD
     11     "dca [cm]",                                 dca(i)>fCutsRD
     12     "inv. mass (Mphi-MKK) [GeV]",               invmass-pdg>fCutsRD
     13     "inv. mass (MKo*-MKpi) [GeV]"};             invmass-pdg>fCutsRD
     14     "Abs(CosineKpiPhiRFrame)^3",
     15     "CosPiDsLabFrame"};
     16     "DecLengthXY
     17     "NormDecayLength"};
     18     "NormDecayLengthXY"};
     19     "cosThetaPointXY"};
     */
    
    //Filtering ITS3 MC
    //Float_t cutsArrayDstoKKpi[20]={0.3,0.3,0.3,0.,0.,0.,0.06,0.,0.,0.9,0.,100000.,0.02,0.0001,-1.,1.,0.,0.,0.,-1.};

    anacutsval[0][0]=0.07; //tighter than filtering
    anacutsval[1][0]=0.5; //tighter than filtering
    anacutsval[2][0]=0.5; //tighter than filtering
    anacutsval[3][0]=0.;
    anacutsval[4][0]=0.;
    anacutsval[5][0]=0.;
    anacutsval[6][0]=0.035; //tighter than filtering
    anacutsval[7][0]=0.01; //=tighter than filtering (0.0, bkg level otherwise too high)
    anacutsval[8][0]=0.;
    anacutsval[9][0]=0.9; //filtering
    anacutsval[10][0]=0.;
    anacutsval[11][0]=0.05; //tighter than filtering
    anacutsval[12][0]=0.01; //tighter than filtering
    anacutsval[13][0]=0.0001; //filtering
    anacutsval[14][0]=-1; //Open for ML optimisation
    anacutsval[15][0]=1.; //Open for ML optimisation
    anacutsval[16][0]=0.; //Open for ML optimisation
    anacutsval[17][0]=0.; //Open for ML optimisation
    anacutsval[18][0]=0.; //Open for ML optimisation
    anacutsval[19][0]=-1; //Open for ML optimisation

    anacutsval[0][1]=0.07; //tighter than filtering
    anacutsval[1][1]=0.5; //tighter than filtering
    anacutsval[2][1]=0.5; //tighter than filtering
    anacutsval[3][1]=0.;
    anacutsval[4][1]=0.;
    anacutsval[5][1]=0.;
    anacutsval[6][1]=0.035; //tighter than filtering
    anacutsval[7][1]=0.01; //=tighter than filtering (0.0, bkg level otherwise too high)
    anacutsval[8][1]=0.;
    anacutsval[9][1]=0.9; //filtering
    anacutsval[10][1]=0.;
    anacutsval[11][1]=0.1; //tighter than filtering
    anacutsval[12][1]=0.01; //tighter than filtering
    anacutsval[13][1]=0.0001; //filtering
    anacutsval[14][1]=-1; //Open for ML optimisation
    anacutsval[15][1]=1.; //Open for ML optimisation
    anacutsval[16][1]=0.; //Open for ML optimisation
    anacutsval[17][1]=0.; //Open for ML optimisation
    anacutsval[18][1]=0.; //Open for ML optimisation
    anacutsval[19][1]=-1; //Open for ML optimisation
    
    cuts->SetCuts(20,nptbins,anacutsval);
    cuts->SetMinPtCandidate(1.);
    
    //AliAODPidHF* pidObj=new AliAODPidHF();
    //Double_t sigmasBac[5]={3.,3.,3.,3.,3.}; // 0, 1(A), 2(A) -> TPC; 3 -> TOF; 4 -> ITS
    //pidObj->SetSigma(sigmasBac);
    //pidObj->SetAsym(kFALSE);
    //pidObj->SetMatch(1);
    //pidObj->SetTPC(kTRUE);
    //pidObj->SetTOF(kTRUE);
    //cuts->SetPidHF(pidObj);
    
    cuts->SetPidOption(0); //0=kConservative,1=kStrong
    Bool_t pidflag=usePID;
    cuts->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for filtering cuts"<<endl;
    else cout<<"PID is not used for filtering cuts"<<endl;
  } else if(whichCuts==1){
    
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
    
    anacutsval[6][0]=0.020;   //sigmavert
    anacutsval[7][0]=0.05;   //decay length
    anacutsval[9][0]=0.99;   //cosP
    anacutsval[12][0]=0.005; //Mass Phi
    anacutsval[14][0]=0.15;  //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][0]=1.0;  //CosP labFrame
    anacutsval[16][0]=0.05;  //decayXY
    anacutsval[17][0]=0.;    //normdecay
    anacutsval[18][0]=9.0;   //normdecayXY
    anacutsval[19][0]=0.995;  //CosPXY
    
    anacutsval[6][1]=0.020;   //sigmavert
    anacutsval[7][1]=0.05;   //decay length
    anacutsval[9][1]=0.99;   //cosP
    anacutsval[12][1]=0.005; //Mass Phi
    anacutsval[14][1]=0.15;  //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][1]=1.0;  //CosP labFrame
    anacutsval[16][1]=0.05;  //decayXY
    anacutsval[17][1]=0.;    //normdecay
    anacutsval[18][1]=9.0;   //normdecayXY
    anacutsval[19][1]=0.995;  //CosPXY
    
    anacutsval[6][2]=0.025;   //sigmavert
    anacutsval[7][2]=0.05;   //decay length
    anacutsval[9][2]=0.99;   //cosP
    anacutsval[12][2]=0.005; //Mass Phi
    anacutsval[14][2]=0.1;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][2]=1.0;   //CosP labFrame
    anacutsval[16][2]=0.05;  //decayXY
    anacutsval[17][2]=0.;    //normdecay
    anacutsval[18][2]=9.0;   //normdecayXY
    anacutsval[19][2]=0.995;  //CosPXY
    
    anacutsval[6][3]=0.025;   //sigmavert
    anacutsval[7][3]=0.05;   //decay length
    anacutsval[9][3]=0.98;   //cosP
    anacutsval[12][3]=0.005; //Mass Phi
    anacutsval[14][3]=0.1;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][3]=1.0;  //CosP labFrame
    anacutsval[16][3]=0.05;   //decayXY
    anacutsval[17][3]=0.;    //normdecay
    anacutsval[18][3]=9.0;   //normdecayXY
    anacutsval[19][3]=0.99;  //CosPXY
    
    anacutsval[6][4]=0.025;   //sigmavert
    anacutsval[7][4]=0.05;   //decay length
    anacutsval[9][4]=0.98;   //cosP
    anacutsval[12][4]=0.005; //Mass Phi
    anacutsval[14][4]=0.05;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][4]=1.0;  //CosP labFrame
    anacutsval[16][4]=0.05;   //decayXY
    anacutsval[17][4]=0.;    //normdecay
    anacutsval[18][4]=8.0;   //normdecayXY
    anacutsval[19][4]=0.99;  //CosPXY
    
    anacutsval[6][5]=0.025;   //sigmavert
    anacutsval[7][5]=0.05;   //decay length
    anacutsval[9][5]=0.98;   //cosP
    anacutsval[12][5]=0.005; //Mass Phi
    anacutsval[14][5]=0.05;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][5]=1.0;  //CosP labFrame
    anacutsval[16][5]=0.05;   //decayXY
    anacutsval[17][5]=0.;    //normdecay
    anacutsval[18][5]=8.0;   //normdecayXY
    anacutsval[19][5]=0.99;  //CosPXY
    
    anacutsval[6][6]=0.025;   //sigmavert
    anacutsval[7][6]=0.05;   //decay length
    anacutsval[9][6]=0.98;   //cosP
    anacutsval[12][6]=0.005; //Mass Phi
    anacutsval[14][6]=0.;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][6]=1.0;  //CosP labFrame
    anacutsval[16][6]=0.05;   //decayXY
    anacutsval[17][6]=0.;    //normdecay
    anacutsval[18][6]=6.0;   //normdecayXY
    anacutsval[19][6]=0.98;  //CosPXY
    
    anacutsval[6][7]=0.035;   //sigmavert
    anacutsval[7][7]=0.05;   //decay length
    anacutsval[9][7]=0.98;   //cosP
    anacutsval[12][7]=0.005; //Mass Phi
    anacutsval[14][7]=0.;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][7]=1.0;  //CosP labFrame
    anacutsval[16][7]=0.05;   //decayXY
    anacutsval[17][7]=0.;    //normdecay
    anacutsval[18][7]=6.0;   //normdecayXY
    anacutsval[19][7]=0.98;  //CosPXY
    
    anacutsval[6][8]=0.035;   //sigmavert
    anacutsval[7][8]=0.05;   //decay length
    anacutsval[9][8]=0.98;   //cosP
    anacutsval[12][8]=0.005; //Mass Phi
    anacutsval[14][8]=0.;   //Abs(CosineKpiPhiRFrame)^3
    anacutsval[15][8]=1.0;  //CosP labFrame
    anacutsval[16][8]=0.05;   //decayXY
    anacutsval[17][8]=0.;    //normdecay
    anacutsval[18][8]=6.0;   //normdecayXY
    anacutsval[19][8]=0.98;  //CosPXY
    
    Float_t topomCuts[nptbins] = {1.5,1.5,2.,2.,2.,2.5,2.5,2.5,2.5}; //topomatic
    Float_t d0Cuts[nptbins] = {70.,70.,70.,70.,70.,70.,70.,70.,70.}; //topomatic
    
    cuts->SetPtBins(nptbins+1,ptbins);
    cuts->SetCuts(nvars,nptbins,anacutsval);
    cuts->Setd0MeasMinusExpCut(nptbins,topomCuts);
    cuts->Setd0Cut(nptbins,d0Cuts);

    cuts->SetMinPtCandidate(2.);
    cuts->SetMaxPtCandidate(36.);

    cuts->SetPidOption(0); //0=kConservative,1=kStrong
    //cuts->SetMaxPtStrongPid(8.);
    //cuts->SetMaxPtStrongPid(9999.);
    
    Bool_t pidflag=usePID;
    cuts->SetUsePID(pidflag);
    if(pidflag) cout<<"PID is used for analysis cuts"<<endl;
    else cout<<"PID is not used for analysis cuts"<<endl;
  }

  //Use Ds PreSelect
  if(usePreSelect) cuts->SetUsePreSelect(2);
  
  //Do not recalculate the vertex
  cuts->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
  
  //event selection
  cuts->SetUsePhysicsSelection(kFALSE);
  cuts->SetTriggerClass("");
  cuts->SetTriggerMask(AliVEvent::kAny);
  cuts->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
  cuts->SetMaxVtxZ(10.);
  cuts->SetCutOnzVertexSPD(3);
  
  cuts->SetName(nameCuts.Data());
  cuts->SetTitle(nameCuts.Data());
  cout<<"This is the object I'm going to save:"<<endl;
  cuts->PrintAll();
  
  return cuts;
  
}
