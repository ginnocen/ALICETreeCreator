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


AliRDHFCutsDStartoKpipi *makeInputCutsDstartoKpipi(Int_t whichCuts=0, TString nameCuts="DstartoKpipiFilteringCuts", Float_t minc=0.,Float_t maxc=20.)
{
    
    AliRDHFCutsDStartoKpipi* cutsDstartoKpipi=new AliRDHFCutsDStartoKpipi();
    cutsDstartoKpipi->SetName(nameCuts.Data());
    cutsDstartoKpipi->SetTitle(nameCuts.Data());
    
    AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
    esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
    //default
    esdTrackCuts->SetRequireTPCRefit(kTRUE);
    esdTrackCuts->SetRequireITSRefit(kTRUE);
    if(whichCuts==0)esdTrackCuts->SetMinNClustersTPC(50);
    if(whichCuts==1)esdTrackCuts->SetMinNClustersTPC(80);
    esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);
    esdTrackCuts->SetEtaRange(-0.8,0.8);
    esdTrackCuts->SetMinDCAToVertexXY(0.);
    esdTrackCuts->SetPtRange(0.5,1.e10); 
    esdTrackCuts->SetMaxDCAToVertexXY(1.);
    esdTrackCuts->SetMaxDCAToVertexZ(1.);
    if(whichCuts==0)esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    cutsDstartoKpipi->AddTrackCuts(esdTrackCuts);
    
    //soft pion selections
    AliESDtrackCuts* esdTrackCutsSoftPi=new AliESDtrackCuts();
    esdTrackCutsSoftPi->SetRequireTPCRefit(kFALSE);
    esdTrackCutsSoftPi->SetMinNClustersITS(2);
    esdTrackCutsSoftPi->SetMaxDCAToVertexXY(1.);  
    esdTrackCutsSoftPi->SetMaxDCAToVertexZ(1.);
    esdTrackCutsSoftPi->SetPtRange(0.1,1.e10);
    esdTrackCutsSoftPi->SetEtaRange(-0.8,+0.8);
    cutsDstartoKpipi->AddTrackCutsSoftPi(esdTrackCutsSoftPi);
    
    if(whichCuts==0){
        const Int_t nptbinsDstar=2;
        Float_t ptlimitsDstar[nptbinsDstar+1]={0.,5.,1000000.};
        Float_t** cutsArrayDstartoKpipi;
        cutsArrayDstartoKpipi=new Float_t*[16];
        for(Int_t iv=0;iv<16;iv++){
            cutsArrayDstartoKpipi[iv]=new Float_t[nptbinsDstar];
        }
	//0-5
	cutsArrayDstartoKpipi[0][0]=0.095;  //D0 inv mass window
	cutsArrayDstartoKpipi[1][0]=0.05;   // dca
	cutsArrayDstartoKpipi[2][0]=0.9;    // thetastar
	cutsArrayDstartoKpipi[3][0]=0.5;    // pt Pion
	cutsArrayDstartoKpipi[4][0]=0.5;    // Pt Kaon
	cutsArrayDstartoKpipi[5][0]=0.1;    // d0K
	cutsArrayDstartoKpipi[6][0]=0.1;    // d0Pi
	cutsArrayDstartoKpipi[7][0]=-0.00001; // d0xd0
	cutsArrayDstartoKpipi[8][0]=0.9;    // costhetapoint
	cutsArrayDstartoKpipi[9][0]=0.3;   // Dstar inv mass window
	cutsArrayDstartoKpipi[10][0]=0.03;  // half width of (M_Kpipi-M_D0)
	cutsArrayDstartoKpipi[11][0]=0.1;   // Pt min of Pi soft
	cutsArrayDstartoKpipi[12][0]=100.;  // Pt max of pi soft
	cutsArrayDstartoKpipi[13][0]=9999.; // theta
	cutsArrayDstartoKpipi[14][0]=0.96;   // |cosThetaPointXY|
	cutsArrayDstartoKpipi[15][0]=2.5;    // NormDecayLenghtXY
	//5-999
	cutsArrayDstartoKpipi[0][1]=0.12;   //D0 inv mass window
	cutsArrayDstartoKpipi[1][1]=0.06;   // dca
	cutsArrayDstartoKpipi[2][1]=0.9;    // thetastar
	cutsArrayDstartoKpipi[3][1]=0.5;    // pt Pion
	cutsArrayDstartoKpipi[4][1]=0.5;    // Pt Kaon
	cutsArrayDstartoKpipi[5][1]=0.1;    // d0K
	cutsArrayDstartoKpipi[6][1]=0.1;    // d0Pi
	cutsArrayDstartoKpipi[7][1]=0.0001; // d0xd0
	cutsArrayDstartoKpipi[8][1]=0.7;    // costhetapoint
	cutsArrayDstartoKpipi[9][1]=0.3;   // Dstar inv mass window
	cutsArrayDstartoKpipi[10][1]=0.03;  // half width of (M_Kpipi-M_D0)
	cutsArrayDstartoKpipi[11][1]=0.1;   // Pt min of Pi soft
	cutsArrayDstartoKpipi[12][1]=100.;  // Pt max of pi soft
	cutsArrayDstartoKpipi[13][1]=9999.; // theta
	cutsArrayDstartoKpipi[14][1]=0.8;   // |cosThetaPointXY|
	cutsArrayDstartoKpipi[15][1]=0.;    // NormDecayLenghtXY

        cutsDstartoKpipi->SetMinPtCandidate(1.);
	cutsDStartoKpipi->SetUseTrackSelectionWithFilterBits(kFALSE);
        cutsDstartoKpipi->SetUsePID(kFALSE);
        cutsDstartoKpipi->SetPtBins(nptbinsDstar+1,ptlimitsDstar);
        cutsDstartoKpipi->SetCuts(16,nptbinsDstar,cutsArrayDstartoKpipi);
    }
    
    else if(whichCuts==1){
        
      //        cutsDstartoKpipi->SetSelectCandTrackSPDFirst(kTRUE, 5);
        const Int_t nvars=16;
        const Int_t nptbins=13;
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
        
        cutsDstartoKpipi->SetPtBins(nptbins+1,ptbins);
	Float_t cuts12[16]={0.032, 0.0225, 0.8, 1., 1., 0.1, 0.1, -0.0005, 0.994, 0.3, 0.15, 0.05, 1., 0.5, 0.999, 9};
	Float_t cuts23[16]={0.032, 0.022, 0.8, 1., 1., 0.1, 0.1, -0.00046, 0.994, 0.3, 0.15, 0.1, 1., 0.5, 0.999, 9}; 
        Float_t cuts34[16]={0.032, 0.023, 0.8, 1., 1., 0.1, 0.1, -0.00039, 0.99, 0.3, 0.15, 0.05, 1., 0.5, 0.998, 10};
	Float_t cuts45[16]={0.032, 0.022, 0.8, 1., 1., 0.1, 0.1, -0.00035, 0.99, 0.2, 0.15, 0.05, 10., 0.5, 0.999, 6.5};
	Float_t cuts56[16]={0.04, 0.02,   1,   0.9,  0.9, 0.08, 0.08, -0.00023, 0.99, 0.3, 0.3, 0.25,  10., 0.5, 0.999, 6.5};
	Float_t cuts67[16]={0.043, 0.021, 1, 1., 1., 0.1, 0.1, -0.00015, 0.99, 0.3, 0.15, 0.3, 100., 0.5, 0.999, 6.4};
	Float_t cuts78[16]={0.043, 0.02,   1, 1., 1., 0.1, 0.1, -0.00015, 0.99, 0.3, 0.15, 0.3, 100., 0.5, 0.998, 6.4}; 
	Float_t cuts810[16]={0.055, 0.02,  1, 0.9, 0.9, 0.12, 0.12, -0.000085, 0.97,0.3, 0.15, 0.3, 100., 1., 0.998, 6}; 
	Float_t cuts1012[16]={0.055, 0.02,  1, 1., 1., 0.12, 0.12, -0.000075, 0.97,0.3, 0.15, 0.3, 100., 1., 0.998, 6};
	Float_t cuts1216[16]={0.074, 0.02, 1, 0.7, 0.7, 0.15, 0.15, -0.000075, 0.97, 0.3, 0.15, 0.3, 100., 1., 0.998, 4.};
	Float_t cuts1620[16]={0.074, 0.021, 1., 0.5, 0.5, 0.15, 0.15, -0.00005, 0.95, 0.3, 0.15, 0.3, 100., 1., 0.998, 4.}; 
	Float_t cuts2024[16]={0.074, 0.02, 1., 0.5, 0.5, 0.15, 0.15, -0.00005, 0.93, 0.15, 0.15, 0.3, 100., 1., 0.995, 1.};

	Float_t** rdcutsvalmine;
	rdcutsvalmine=new Float_t*[nvars];
	for(Int_t iv=0;iv<nvars;iv++){
	  rdcutsvalmine[iv]=new Float_t[nptbins];
	  //pt 1-2
	  rdcutsvalmine[iv][0]=cuts12[iv];
	  //pt 2-3
	  rdcutsvalmine[iv][1]=cuts23[iv];
	  //pt 3-4
	  rdcutsvalmine[iv][2]=cuts34[iv];
	  //pt 4-5
	  rdcutsvalmine[iv][3]=cuts45[iv];
	  //pt 5-6
	  rdcutsvalmine[iv][4]=cuts56[iv];
	  //pt 6-7
	  rdcutsvalmine[iv][5]=cuts67[iv];
	  //pt 7-8
	  rdcutsvalmine[iv][6]=cuts78[iv];
	  //pt 8-10
	  rdcutsvalmine[iv][7]=cuts810[iv];
	  //pt 10-12
	  rdcutsvalmine[iv][8]=cuts1012[iv];
	  //pt 12-16
	  rdcutsvalmine[iv][9]=cuts1216[iv];
	  //pt 16-20
	  rdcutsvalmine[iv][10]=cuts1620[iv];
	  //pt 20-24
	  rdcutsvalmine[iv][11]=cuts2024[iv];
	  //pt 24-36
	  rdcutsvalmine[iv][12]=cuts2024[iv]; 
	}


        
        
        cutsDstartoKpipi->SetCuts(nvars,nptbins,rdcutsvalmine);
        
        Bool_t pidflag=kTRUE;
        cutsDstartoKpipi->SetUsePID(pidflag);
        if(pidflag) cout<<"PID is used"<<endl;
        else cout<<"PID is not used"<<endl;
        
        //pid settings
        AliAODPidHF* pidObj=new AliAODPidHF();
        //pidObj->SetName("pid4D0");
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
        
        //Do not recalculate the vertex
        cutsDstartoKpipi->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
        
        
        
    }
    
    //event selection
    cutsDstartoKpipi->SetTriggerClass("");
    cutsDstartoKpipi->SetTriggerMask(AliVEvent::kINT7);
    cutsDstartoKpipi->SetMinCentrality(minc);
    cutsDstartoKpipi->SetMaxCentrality(maxc);
    cutsDstartoKpipi->SetUseCentrality(AliRDHFCuts::kCentV0M); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,kCentInvalid
    cutsDstartoKpipi->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
    cutsDstartoKpipi->SetMaxVtxZ(10.);
    cutsDstartoKpipi->SetCutOnzVertexSPD(2);
    
    cout<<"This is the object I'm going to save:"<<endl;
    cutsDstartoKpipi->SetName(nameCuts.Data());
    cutsDstartoKpipi->SetTitle(nameCuts.Data());
    cutsDstartoKpipi->PrintAll();
    
    return cutsDstartoKpipi;
    
}


