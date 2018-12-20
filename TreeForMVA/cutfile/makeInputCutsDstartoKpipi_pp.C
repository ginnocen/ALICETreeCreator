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


AliRDHFCutsDStartoKpipi *makeInputCutsDstartoKpipi_pp(Int_t whichCuts=0, TString nameCuts="DstartoKpipiFilteringCuts", Float_t minc=0.,Float_t maxc=20.)
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
    if(whichCuts==1)esdTrackCuts->SetMinNClustersTPC(70);
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
    esdTrackCutsSoftPi->SetPtRange(0.0,1.e10);
    cutsDstartoKpipi->AddTrackCutsSoftPi(esdTrackCutsSoftPi);
    
    if(whichCuts==0){
        const Int_t nptbinsDstar=1;
        Float_t ptlimitsDstar[nptbinsDstar+1]={0.,1000000.};
        Float_t cutsArrayDstartoKpipi[16]={0.3,999999.,1.1,0.,0.,999999.,999999.,999999.,0.,0.3, 0.1, 0.05, 100000000000.0, 0.5,-1.,0.}; // first 9 for D0 from D*, next 5 for D*, last 2 for D0 again
	
        cutsDstartoKpipi->SetMinPtCandidate(0.);
        cutsDstartoKpipi->SetUsePID(kFALSE);
        cutsDstartoKpipi->SetUseTrackSelectionWithFilterBits(kFALSE);
        cutsDstartoKpipi->SetPtBins(nptbinsDstar+1,ptlimitsDstar);
        cutsDstartoKpipi->SetCuts(16,cutsArrayDstartoKpipi);
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

        fCutsRD[0][0] = 0.035; fCutsRD[0][1] = 0.032; fCutsRD[0][2] = 0.032; fCutsRD[0][3] = 0.032; fCutsRD[0][4] = 0.032; fCutsRD[0][5] = 0.036; fCutsRD[0][6] = 0.036; fCutsRD[0][7] = 0.04; fCutsRD[0][8] = 0.05; fCutsRD[0][9] = 0.074; fCutsRD[0][10] = 0.074; fCutsRD[0][11] = 0.074; fCutsRD[0][12] = 0.074; fCutsRD[0][13] = 0.074; fCutsRD[0][14] = 0.074;
        fCutsRD[1][0] = 0.05; fCutsRD[1][1] = 0.038; fCutsRD[1][2] = 0.03; fCutsRD[1][3] = 0.033; fCutsRD[1][4] = 0.042; fCutsRD[1][5] = 0.05; fCutsRD[1][6] = 0.1; fCutsRD[1][7] = 0.1; fCutsRD[1][8] = 0.1; fCutsRD[1][9] = 0.1; fCutsRD[1][10] = 0.1; fCutsRD[1][11] = 10; fCutsRD[1][12] = 10; fCutsRD[1][13] = 10; fCutsRD[1][14] = 10;
        fCutsRD[2][0] = 0.75; fCutsRD[2][1] = 0.9; fCutsRD[2][2] = 0.8; fCutsRD[2][3] = 0.8; fCutsRD[2][4] = 0.9; fCutsRD[2][5] = 1; fCutsRD[2][6] = 1; fCutsRD[2][7] = 1; fCutsRD[2][8] = 1; fCutsRD[2][9] = 1; fCutsRD[2][10] = 1; fCutsRD[2][11] = 10; fCutsRD[2][12] = 10; fCutsRD[2][13] = 10; fCutsRD[2][14] = 10;
        fCutsRD[3][0] = 0.45; fCutsRD[3][1] = 0.6; fCutsRD[3][2] = 0.9; fCutsRD[3][3] = 0.9; fCutsRD[3][4] = 1; fCutsRD[3][5] = 1; fCutsRD[3][6] = 1; fCutsRD[3][7] = 0.6; fCutsRD[3][8] = 0.6; fCutsRD[3][9] = 0.6; fCutsRD[3][10] = 0.5; fCutsRD[3][11] = 0.2; fCutsRD[3][12] = 0.2; fCutsRD[3][13] = 0.2; fCutsRD[3][14] = 0.2;
        fCutsRD[4][0] = 0.45; fCutsRD[4][1] = 0.6; fCutsRD[4][2] = 0.9; fCutsRD[4][3] = 0.9; fCutsRD[4][4] = 1; fCutsRD[4][5] = 1; fCutsRD[4][6] = 1; fCutsRD[4][7] = 0.6; fCutsRD[4][8] = 0.6; fCutsRD[4][9] = 0.6; fCutsRD[4][10] = 0.5; fCutsRD[4][11] = 0.2; fCutsRD[4][12] = 0.2; fCutsRD[4][13] = 0.2; fCutsRD[4][14] = 0.2;
        fCutsRD[5][0] = 0.1; fCutsRD[5][1] = 0.1; fCutsRD[5][2] = 0.1; fCutsRD[5][3] = 0.1; fCutsRD[5][4] = 0.1; fCutsRD[5][5] = 0.1; fCutsRD[5][6] = 0.1; fCutsRD[5][7] = 0.1; fCutsRD[5][8] = 0.1; fCutsRD[5][9] = 0.1; fCutsRD[5][10] = 0.15; fCutsRD[5][11] = 0.5; fCutsRD[5][12] = 0.5; fCutsRD[5][13] = 0.5; fCutsRD[5][14] = 0.5;
        fCutsRD[6][0] = 0.1; fCutsRD[6][1] = 0.1; fCutsRD[6][2] = 0.1; fCutsRD[6][3] = 0.1; fCutsRD[6][4] = 0.1; fCutsRD[6][5] = 0.1; fCutsRD[6][6] = 0.1; fCutsRD[6][7] = 0.1; fCutsRD[6][8] = 0.1; fCutsRD[6][9] = 0.1; fCutsRD[6][10] = 0.15; fCutsRD[6][11] = 0.5; fCutsRD[6][12] = 0.5; fCutsRD[6][13] = 0.5; fCutsRD[6][14] = 0.5;
        fCutsRD[7][0] = 1; fCutsRD[7][1] = -0.000135; fCutsRD[7][2] = -0.00019; fCutsRD[7][3] = -0.000144; fCutsRD[7][4] = -2.8e-05; fCutsRD[7][5] = 5.5e-05; fCutsRD[7][6] = 0.0001; fCutsRD[7][7] = 0.0001; fCutsRD[7][8] = 0.001; fCutsRD[7][9] = 0.001; fCutsRD[7][10] = 0.001; fCutsRD[7][11] = 1; fCutsRD[7][12] = 1; fCutsRD[7][13] = 1; fCutsRD[7][14] = 1;
        fCutsRD[8][0] = 0.7; fCutsRD[8][1] = 0.8; fCutsRD[8][2] = 0.9; fCutsRD[8][3] = 0.89; fCutsRD[8][4] = 0.81; fCutsRD[8][5] = 0.79; fCutsRD[8][6] = 0.7; fCutsRD[8][7] = 0.75; fCutsRD[8][8] = 0.7; fCutsRD[8][9] = 0.7; fCutsRD[8][10] = 0.7; fCutsRD[8][11] = 0.2; fCutsRD[8][12] = 0.2; fCutsRD[8][13] = 0.2; fCutsRD[8][14] = 0.2;
        fCutsRD[9][0] = 0.3; fCutsRD[9][1] = 0.3; fCutsRD[9][2] = 0.3; fCutsRD[9][3] = 0.3; fCutsRD[9][4] = 0.3; fCutsRD[9][5] = 0.3; fCutsRD[9][6] = 0.3; fCutsRD[9][7] = 0.3; fCutsRD[9][8] = 0.3; fCutsRD[9][9] = 0.3; fCutsRD[9][10] = 0.3; fCutsRD[9][11] = 0.3; fCutsRD[9][12] = 0.3; fCutsRD[9][13] = 0.3; fCutsRD[9][14] = 0.3;
        fCutsRD[10][0] = 0.3; fCutsRD[10][1] = 0.3; fCutsRD[10][2] = 0.3; fCutsRD[10][3] = 0.3; fCutsRD[10][4] = 0.3; fCutsRD[10][5] = 0.3; fCutsRD[10][6] = 0.3; fCutsRD[10][7] = 0.3; fCutsRD[10][8] = 0.3; fCutsRD[10][9] = 0.3; fCutsRD[10][10] = 0.3; fCutsRD[10][11] = 0.3; fCutsRD[10][12] = 0.3; fCutsRD[10][13] = 0.3; fCutsRD[10][14] = 0.3;
        fCutsRD[11][0] = 0.03; fCutsRD[11][1] = 0.05; fCutsRD[11][2] = 0.05; fCutsRD[11][3] = 0.05; fCutsRD[11][4] = 0.05; fCutsRD[11][5] = 0.05; fCutsRD[11][6] = 0.05; fCutsRD[11][7] = 0.05; fCutsRD[11][8] = 0.05; fCutsRD[11][9] = 0.05; fCutsRD[11][10] = 0.05; fCutsRD[11][11] = 0.05; fCutsRD[11][12] = 0.05; fCutsRD[11][13] = 0.05; fCutsRD[11][14] = 0.05;
        fCutsRD[12][0] = 0.2; fCutsRD[12][1] = 0.3; fCutsRD[12][2] = 0.4; fCutsRD[12][3] = 0.6; fCutsRD[12][4] = 100; fCutsRD[12][5] = 100; fCutsRD[12][6] = 100; fCutsRD[12][7] = 100; fCutsRD[12][8] = 100; fCutsRD[12][9] = 100; fCutsRD[12][10] = 100; fCutsRD[12][11] = 100; fCutsRD[12][12] = 100; fCutsRD[12][13] = 100; fCutsRD[12][14] = 100;
        fCutsRD[13][0] = 1; fCutsRD[13][1] = 1; fCutsRD[13][2] = 1; fCutsRD[13][3] = 1; fCutsRD[13][4] = 1; fCutsRD[13][5] = 1; fCutsRD[13][6] = 1; fCutsRD[13][7] = 1; fCutsRD[13][8] = 1; fCutsRD[13][9] = 1; fCutsRD[13][10] = 1; fCutsRD[13][11] = 1; fCutsRD[13][12] = 1; fCutsRD[13][13] = 1; fCutsRD[13][14] = 1;
        fCutsRD[14][0] = 0.85; fCutsRD[14][1] = 0.88; fCutsRD[14][2] = -1; fCutsRD[14][3] = -1; fCutsRD[14][4] = -1; fCutsRD[14][5] = -1; fCutsRD[14][6] = -1; fCutsRD[14][7] = -1; fCutsRD[14][8] = -1; fCutsRD[14][9] = -1; fCutsRD[14][10] = -1; fCutsRD[14][11] = -1; fCutsRD[14][12] = -1; fCutsRD[14][13] = -1; fCutsRD[14][14] = -1;
        fCutsRD[15][0] = 0; fCutsRD[15][1] = 2.7; fCutsRD[15][2] = 3; fCutsRD[15][3] = 0; fCutsRD[15][4] = 0; fCutsRD[15][5] = 0; fCutsRD[15][6] = 0; fCutsRD[15][7] = 0; fCutsRD[15][8] = 0; fCutsRD[15][9] = 0; fCutsRD[15][10] = 0; fCutsRD[15][11] = 0; fCutsRD[15][12] = 0; fCutsRD[15][13] = 0; fCutsRD[15][14] = 0;

        cutsDstartoKpipi->SetCuts(nvars,nptbins,fCutsRD);
        
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
        
        //Do not recalculate the vertex
        cutsDstartoKpipi->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
    }
    
    //event selection
    cutsDstartoKpipi->SetTriggerClass("");
    cutsDstartoKpipi->SetTriggerMask(AliVEvent::kINT7);
    cutsDstartoKpipi->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent);
    cutsDstartoKpipi->ConfigurePileupCuts(5,0.8);
    cutsDstartoKpipi->SetMaxVtxZ(10.);
    cutsDstartoKpipi->SetCutOnzVertexSPD(3);
    cutsDstartoKpipi->SetMinVtxContr(1);
    
    cout<<"This is the object I'm going to save:"<<endl;
    cutsDstartoKpipi->SetName(nameCuts.Data());
    cutsDstartoKpipi->SetTitle(nameCuts.Data());
    cutsDstartoKpipi->PrintAll();
    
    return cutsDstartoKpipi;
    
}
