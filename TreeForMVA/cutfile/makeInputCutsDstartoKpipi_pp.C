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
    esdTrackCutsSoftPi->SetRequireTPCRefit(kFALSE);
    esdTrackCutsSoftPi->SetMinNClustersITS(2);
    esdTrackCutsSoftPi->SetEtaRange(-0.8,+0.8);
    cutsDstartoKpipi->AddTrackCutsSoftPi(esdTrackCutsSoftPi);
    
    if(whichCuts==0){
        const Int_t nptbinsDstar=1;
        Float_t ptlimitsDstar[nptbinsDstar+1]={0.,1000000.};
        Float_t** cutsArrayDstartoKpipi;
        cutsArrayDstartoKpipi=new Float_t*[16];
        for(Int_t iv=0;iv<16;iv++){
            cutsArrayDstartoKpipi[iv]=new Float_t[nptbinsDstar];
        }
	cutsArrayDStartoKpipi[16]={0.3,999999.,1.1,0.,0.,999999.,999999.,999999.,0.,0.3, 0.1, 0.05, 100000000000.0, 0.5,-1.,0.}; // first 9 for D0 from D*, next 5 for D*, last 2 for D0 again
	
        cutsDstartoKpipi->SetMinPtCandidate(0.);
        cutsDstartoKpipi->SetUsePID(kFALSE);
	cutsD0toKpi->SetUseTrackSelectionWithFilterBits(kFALSE);
        cutsDstartoKpipi->SetPtBins(nptbinsDstar+1,ptlimitsDstar);
        cutsDstartoKpipi->SetCuts(16,cutsArrayDstartoKpipi);
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
	Float_t cuts12[16]={0.035, 0.04, 0.9, 0.5, 0.5, 0.1, 0.1, -0.0000656, 0.7, 0.3, 0.3, 0.05, 100, 1, -1, 0};
	Float_t cuts23[16]={0.035, 0.04, 0.9, 0.7, 0.7, 0.1, 0.1, -0.000172, 0.7, 0.3, 0.3, 0.05, 100, 1, -1, 0};
        Float_t cuts34[16]={0.035, 0.04, 1, 0.8, 0.8, 0.1, 0.1, -0.000057, 0.9, 0.3, 0.3, 0.05, 100, 1, -1, 0};
	Float_t cuts45[16]={0.038, 0.1, 1,  0.9, 0.9, 0.1, 0.1, -0.000016, 0.9, 0.3, 0.3, 0.05, 100, 1, -1, 0};
	Float_t cuts56[16]={0.038, 0.1, 1,  1,   1,   0.1, 0.1, 0.00004, 0.82, 0.3, 0.3, 0.05, 100, 1, -1, 0};
	Float_t cuts67[16]={0.04,  0.1, 1,  1,   1,   0.1, 0.1, 0.001, 0.74, 0.3, 0.3, 0.05, 100, 1, -1, 0};
	Float_t cuts78[16]={0.04,  0.1, 1,  1,   1,   0.1, 0.1, 0.001, 0.72, 0.3, 0.3, 0.05, 100, 1, -1, 0};
	Float_t cuts810[16]={0.045,0.1, 1,  1,   1,   0.1, 0.1, 0.006, 0.55, 0.3, 0.3, 0.05, 100, 1, -1, 0};
	Float_t cuts1012[16]={0.045,0.1,1,  1,   1,   0.1, 0.1, 0.006, 0.55, 0.3, 0.3, 0.05, 100, 1, -1, 0};
	Float_t cuts1216[16]={0.05, 0.1,1, 0.3, 0.3, 0.15, 0.15, 0.01, 0.65, 0.3, 0.3, 0.05, 100, 1, -1, 0};
	Float_t cuts1620[16]={0.07, 0.5,1, 0.3, 0.3, 0.15, 0.15, 0.01, 0.55, 0.3, 0.3, 0.05, 100, 1, -1, 0};
	Float_t cuts2024[16]={0.07, 0.5,1, 0.3, 0.3, 0.15, 0.15, 0.01, 0.55, 0.3, 0.3, 0.05, 100, 1, -1, 0};

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
    cuts->SetTriggerClass("");
    cuts->SetTriggerMask(AliVEvent::kINT7);
    cuts->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent);
    cuts->ConfigurePileupCuts(5,0.8);
    cuts->SetMaxVtxZ(10.);
    cuts->SetCutOnzVertexSPD(3);
    cuts->SetMinVtxContr(1);
    
    cout<<"This is the object I'm going to save:"<<endl;
    cutsDstartoKpipi->SetName(nameCuts.Data());
    cutsDstartoKpipi->SetTitle(nameCuts.Data());
    cutsDstartoKpipi->PrintAll();
    
    return cutsDstartoKpipi;
    
}


