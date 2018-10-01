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


AliRDHFCutsD0toKpi *makeInputCutsD0toKpi(Int_t whichCuts=0, TString nameCuts="D0toKpiFilteringCuts", Float_t minc=0.,Float_t maxc=20.)
{
    
    AliRDHFCutsD0toKpi* cutsD0toKpi=new AliRDHFCutsD0toKpi();
    cutsD0toKpi->SetName(nameCuts.Data());
    cutsD0toKpi->SetTitle(nameCuts.Data());
    
    AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
    esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
    //default
    esdTrackCuts->SetRequireTPCRefit(kTRUE);
    esdTrackCuts->SetRequireITSRefit(kTRUE);
    if(whichCuts==0)esdTrackCuts->SetMinNClustersTPC(50);
    esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);
    esdTrackCuts->SetEtaRange(-0.8,0.8);
    esdTrackCuts->SetMinDCAToVertexXY(0.);
    esdTrackCuts->SetPtRange(0.5,1.e10);
    esdTrackCuts->SetMaxDCAToVertexXY(1.);
    esdTrackCuts->SetMaxDCAToVertexZ(1.);
    if(whichCuts==1)esdTrackCuts->SetMinDCAToVertexXY(0.);
    if(whichCuts==0)esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    if(whichCuts==1)esdTrackCuts->SetMinDCAToVertexXYPtDep("0.005*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    cutsD0toKpi->AddTrackCuts(esdTrackCuts);
    
    if(whichCuts==0){
        const Int_t nptbinsD0=2;
        Float_t ptlimitsD0[nptbinsD0+1]={0.,5.,1000000.};
        Float_t** cutsArrayD0toKpi;
        cutsArrayD0toKpi=new Float_t*[11];
        for(Int_t iv=0;iv<11;iv++){
            cutsArrayD0toKpi[iv]=new Float_t[nptbinsD0];
        }
        //0-5
        cutsArrayD0toKpi[0][0]=0.25;  //D0 inv mass window
        cutsArrayD0toKpi[1][0]=999999.;
        cutsArrayD0toKpi[2][0]=1.1;
        cutsArrayD0toKpi[3][0]=0.;
        cutsArrayD0toKpi[4][0]=0.;
        cutsArrayD0toKpi[5][0]=999999.;
        cutsArrayD0toKpi[6][0]=999999.;
        cutsArrayD0toKpi[7][0]=0.;  // d0xd0
        cutsArrayD0toKpi[8][0]=0.5;
        cutsArrayD0toKpi[9][0]=-1.;
        cutsArrayD0toKpi[10][0]=0.;
        //5-inf
        cutsArrayD0toKpi[0][1]=0.4;  //D0 inv mass window
        cutsArrayD0toKpi[1][1]=999999.;
        cutsArrayD0toKpi[2][1]=1.1;
        cutsArrayD0toKpi[3][1]=0.;
        cutsArrayD0toKpi[4][1]=0.;
        cutsArrayD0toKpi[5][1]=999999.;
        cutsArrayD0toKpi[6][1]=999999.;
        cutsArrayD0toKpi[7][1]=0.0001; // d0xd0
        cutsArrayD0toKpi[8][1]=0.5;
        cutsArrayD0toKpi[9][1]=-1.;
        cutsArrayD0toKpi[10][1]=0.;
        
        cutsD0toKpi->SetStandardCutsPbPb2011();
        cutsD0toKpi->SetMinPtCandidate(0.);
        cutsD0toKpi->SetUsePID(kFALSE);
        cutsD0toKpi->SetUseTrackSelectionWithFilterBits(kFALSE);
        cutsD0toKpi->SetUseSpecialCuts(kFALSE);
        //cutsD0toKpi->SetUsePhysicsSelection(kFALSE);
        //cutsD0toKpi->SetMaxCentrality(101.);//90.);
        //cutsD0toKpi->SetMaxVtxZ(1.e6);
        cutsD0toKpi->SetPtBins(nptbinsD0+1,ptlimitsD0);
        cutsD0toKpi->SetCuts(11,nptbinsD0,cutsArrayD0toKpi);
    }
    
    else if(whichCuts==1){
        
        cutsD0toKpi->SetSelectCandTrackSPDFirst(kTRUE, 5);
        const Int_t nvars=11;
        const Int_t nptbins=32;
        Float_t* ptbins;
        ptbins=new Float_t[nptbins+1];
        ptbins[0]=0.;
        ptbins[1]=0.5;
        ptbins[2]=1.;
        ptbins[3]=1.5;
        ptbins[4]=2.;
        ptbins[5]=2.25;
        ptbins[6]=2.5;
        ptbins[7]=2.75;
        ptbins[8]=3.;
        ptbins[9]=3.25;
        ptbins[10]=3.5;
        ptbins[11]=3.75;
        ptbins[12]=4.;
        ptbins[13]=4.5;
        ptbins[14]=5.;
        ptbins[15]=5.5;
        ptbins[16]=6.;
        ptbins[17]=6.5;
        ptbins[18]=7.;
        ptbins[19]=7.5;
        ptbins[20]=8.;
        ptbins[21]=8.5;
        ptbins[22]=9.;
        ptbins[23]=10.;
        ptbins[24]=12.;
        ptbins[25]=14.;
        ptbins[26]=16.;
        ptbins[27]=20.;
        ptbins[28]=24.;
        ptbins[29]=36.;
        ptbins[30]=50.;
        ptbins[31]=100.;
        ptbins[32]=9999.;
        
        cutsD0toKpi->SetPtBins(nptbins+1,ptbins);
        
        const Int_t nvary=3;
        Float_t varyd0xd0[nptbins][nvary]={{-35000.*1E-8,-40000.*1E-8,-15000.*1E-8},/* pt<0.5*/
            {-35000.*1E-8,-40000.*1E-8,-15000.*1E-8},/* 0.5<pt<1*/
            {-30000.*1E-8,-36000.*1E-8,-30000.*1E-8},/* 1<pt<1.5 */
            {-30000.*1E-8,-36000.*1E-8,-30000.*1E-8},/* 1.5<pt<2 */
            {-30000.*1E-8,-36000.*1E-8,-36000.*1E-8},/* 2<pt<2.25 */
            {-28000.*1E-8,-31000.*1E-8,-36000.*1E-8},/* 2.25<pt<2.5 */
            {-28000.*1E-8,-31000.*1E-8,-36000.*1E-8},/* 2.5<pt<2.75 */
            {-28000.*1E-8,-31000.*1E-8,-36000.*1E-8},/* 2.75<pt<3 */
            {-28000.*1E-8,-31000.*1E-8,-36000.*1E-8},/* 3<pt<3.25 */
            {-25000.*1E-8,-28000.*1E-8,-36000.*1E-8},/* 3.25<pt<3.5 */
            {-25000.*1E-8,-28000.*1E-8,-36000.*1E-8},/* 3.5<pt<3.75 */
            {-25000.*1E-8,-28000.*1E-8,-36000.*1E-8},/* 3.75<pt<4 */
            {-12000.*1E-8,-18000.*1E-8,-36000.*1E-8},/* 4<pt<4.5 */
            {-12000.*1E-8,-18000.*1E-8,-36000.*1E-8},/* 4.5<pt<5 */
            {-5000.*1E-8,-15000.*1E-8,-23000.*1E-8},/* 5<pt<5.5 */
            {-5000.*1E-8,-15000.*1E-8,-23000.*1E-8},/* 5.5<pt<6 */
            {-5000.*1E-8,-10000.*1E-8,-23000.*1E-8},/* 6<pt<6.5 */
            {-5000.*1E-8,-10000.*1E-8,-23000.*1E-8},/* 6.5<pt<7 */
            {-5000.*1E-8,-10000.*1E-8,-23000.*1E-8},/* 7<pt<7.5 */
            {-5000.*1E-8,-10000.*1E-8,-23000.*1E-8},/* 7.5<pt<8 */
            {-0.*1E-8,-5000.*1E-8,-10000.*1E-8},/* 8<pt<8.5 */
            {-0.*1E-8,-5000.*1E-8,-10000.*1E-8},/* 8.5<pt<9*/
            {-0.*1E-8,-5000.*1E-8,-10000.*1E-8},/* 9<pt<10*/
            {-0.*1E-8,-5000.*1E-8,-10000.*1E-8},/* 10<pt<12 */
            {0.*1E-8,-5000.*1E-8,-10000.*1E-8},/* 12<pt<14 */
            {0.*1E-8,-5000.*1E-8,-10000.*1E-8},/* 14<pt<16 */
            {0.*1E-8,-5000.*1E-8,-10000.*1E-8},/* 16<pt<20 */
            {5000.*1E-8,-5000.*1E-8,-5000.*1E-8},/* 20<pt<24 */
            {5000.*1E-8,-5000.*1E-8,0.*1E-8},/* 24<pt<36 */
            {5000.*1E-8,-5000.*1E-8,0.*1E-8},/* 36<pt<50 */
            {5000.*1E-8,-5000.*1E-8,0.*1E-8},/* 50<pt<100 */
            {5000.*1E-8,-5000.*1E-8,0.*1E-8}};/* pt>100 */
        
        
        Float_t varyCosPoint[nptbins][nvary]={{0.75,0.80,0.80},    /* 0<pt<0.5 */
            {0.75,0.80,0.9},    /* 0.5<pt<1*/
            {0.75,0.80,0.9},    /* 1<pt<1.5 */
            {0.75,0.80,0.9},    /* 1.5<pt<2 */
            {0.92,0.94,0.92},    /* 2<pt<2.25 */
            {0.92,0.94,0.92},    /* 2.25<pt<2.5 */
            {0.92,0.94,0.92},    /* 2.5<pt<2.75 */
            {0.92,0.94,0.92},    /* 2.75<pt<3 */
            {0.92,0.94,0.92},    /* 3<pt<3.25 */
            {0.90,0.92,0.92},    /* 3.25<pt<3.5 */
            {0.90,0.92,0.92},    /* 3.5<pt<3.75 */
            {0.90,0.92,0.92},    /* 3.75<pt<4 */
            {0.85,0.90,0.92},    /* 4<pt<4.5 */
            {0.85,0.90,0.92},    /* 4.5<pt<5 */
            {0.88,0.88,0.90},    /* 5<pt<5.5 */
            {0.88,0.88,0.90},    /* 5.5<pt<6 */
            {0.83,0.85,0.90},    /* 6<pt<6.5 */
            {0.83,0.85,0.90},    /* 6.5<pt<7 */
            {0.83,0.85,0.90},    /* 7<pt<7.5 */
            {0.83,0.85,0.90},    /* 7.5<pt<8 */
            {0.8,0.83,0.90},/* 8<pt<8.5 */
            {0.8,0.83,0.90},/* 8.5<pt<9 */
            {0.8,0.83,0.90},/* 9<pt<10 */
            {0.8,0.83,0.90},/* 10<pt<12 */
            {0.78,0.8,0.90},/* 12<pt<14 */
            {0.78,0.8,0.90},/* 14<pt<16 */
            {0.78,0.8,0.90},/* 16<pt<20 */
            {0.76,0.78,0.90},/* 20<pt<24 */
            {0.75,0.8,0.90},/*24<pt<36 */
            {0.75,0.8,0.90},/* 36<pt<50 */
            {0.75,0.8,0.90},/* 50<pt<100 */
            {0.75,0.8,0.90}};/* pt>100 */
        
        
        Float_t d0MeasMinusExpCut[nptbins]={1.,/* 0<pt<0.5*/
            1.,/* 0.5<pt<1*/
            1.,/* 1<pt<1.5*/
            1.,/* 1.5<pt<2*/
            1.,/*2<pt<2.25*/
            1.,/*2.25<pt<2.5*/
            1.,/*2.5<pt<2.75*/
            1.,/*2.75<pt<3*/
            1.,/*3<pt<3.25*/
            1.,/*3.25<pt<3.5*/
            1.,/*3.5<pt<3.75*/
            1.,/*3.75<pt<4*/
            1.5,/*4<pt<4.5*/
            1.5,/*4.5<pt<5*/
            1.5,/*5<pt<5.5*/
            1.5,/*5.5<pt<6*/
            1.5,/*6<pt<6.5*/
            1.5,/*6.5<pt<7*/
            1.5,/*7<pt<7.5*/
            1.5,/*7.5<pt<8*/
            1.5,/*8<pt<8.5*/
            1.5,/*8.5<pt<9*/
            1.5,/*9<pt<10*/
            1.5,/*10<pt<12*/
            2.,/*12<pt<14*/
            2.,/*14<pt<16*/
            2.,/*16<pt<20*/
            2.,/*20<pt<24*/
            2.,/*24<pt<36*/
            2.,/*36<pt<50*/
            2.,/*50<pt<100*/
            2.,/*>100*/};
        
        
        Int_t varycuts = 22;
        Int_t vcd0xd0=varycuts/10;
        Int_t vccospoint=varycuts%10;
        Float_t cutsMatrixD0toKpiStand[nptbins][nvars]={{0.400,400.*1E-4,0.6,0.5,0.5,1000.*1E-4,1000.*1E-4,varyd0xd0[0][vcd0xd0],varyCosPoint[0][vccospoint],0.99,7.},/* 0<pt<0.5*/
            {0.400,400.*1E-4,0.6,0.5,0.5,1000.*1E-4,1000.*1E-4,varyd0xd0[1][vcd0xd0],varyCosPoint[1][vccospoint],0.993,7.},/* 0.5<pt<1*/
            {0.400,400.*1E-4,0.8,0.5,0.5,1000.*1E-4,1000.*1E-4,varyd0xd0[2][vcd0xd0],varyCosPoint[2][vccospoint],0.991,4.},/* 1<pt<1.5 */
            {0.400,400.*1E-4,0.8,0.5,0.5,1000.*1E-4,1000.*1E-4,varyd0xd0[3][vcd0xd0],varyCosPoint[3][vccospoint],0.991,4.},/* 1.5<pt<2 */
            {0.400,300.*1E-4,0.8,0.6,0.6,1000.*1E-4,1000.*1E-4,varyd0xd0[4][vcd0xd0],varyCosPoint[4][vccospoint],0.997,6.},/* 2<pt<2.25 */
            {0.400,300.*1E-4,0.8,0.6,0.6,1000.*1E-4,1000.*1E-4,varyd0xd0[5][vcd0xd0],varyCosPoint[5][vccospoint],0.997,6.},/* 2.25<pt<2.5 */
            {0.400,300.*1E-4,0.8,0.6,0.6,1000.*1E-4,1000.*1E-4,varyd0xd0[6][vcd0xd0],varyCosPoint[6][vccospoint],0.997,6.},/* 2.5<pt<2.75 */
            {0.400,300.*1E-4,0.8,0.6,0.6,1000.*1E-4,1000.*1E-4,varyd0xd0[7][vcd0xd0],varyCosPoint[7][vccospoint],0.997,6.},/* 2.75<pt<3 */
            {0.400,300.*1E-4,0.8,0.6,0.6,1000.*1E-4,1000.*1E-4,varyd0xd0[8][vcd0xd0],varyCosPoint[8][vccospoint],0.997,6.},/* 3<pt<3.25 */
            {0.400,300.*1E-4,0.8,0.6,0.6,1000.*1E-4,1000.*1E-4,varyd0xd0[9][vcd0xd0],varyCosPoint[9][vccospoint],0.997,6.},/* 3.25<pt<3.5 */
            {0.400,300.*1E-4,0.8,0.6,0.6,1000.*1E-4,1000.*1E-4,varyd0xd0[10][vcd0xd0],varyCosPoint[10][vccospoint],0.997,6.},/* 3.5<pt<3.75 */
            {0.400,300.*1E-4,0.8,0.6,0.6,1000.*1E-4,1000.*1E-4,varyd0xd0[11][vcd0xd0],varyCosPoint[11][vccospoint],0.997,6.},/* 3.75<pt<4 */
            {0.400,300.*1E-4,0.8,0.6,0.6,1000.*1E-4,1000.*1E-4,varyd0xd0[12][vcd0xd0],varyCosPoint[12][vccospoint],0.997,7.},/* 4<pt<4.5*/
            {0.400,300.*1E-4,0.8,0.6,0.6,1000.*1E-4,1000.*1E-4,varyd0xd0[13][vcd0xd0],varyCosPoint[13][vccospoint],0.997,7.},/* 4.5<pt<5*/
            {0.400,300.*1E-4,0.8,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[14][vcd0xd0],varyCosPoint[14][vccospoint],0.998,7.},/* 5<pt<5.5 */
            {0.400,300.*1E-4,0.8,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[15][vcd0xd0],varyCosPoint[15][vccospoint],0.998,7.},/* 5.5<pt<6 */
            {0.400,300.*1E-4,0.8,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[16][vcd0xd0],varyCosPoint[16][vccospoint],0.998,7.},/* 6<pt<6.5 */
            {0.400,300.*1E-4,0.8,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[17][vcd0xd0],varyCosPoint[17][vccospoint],0.998,7.},/* 6.5<pt<7 */
            {0.400,300.*1E-4,0.8,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[18][vcd0xd0],varyCosPoint[18][vccospoint],0.998,7.},/* 7<pt<7.5 */
            {0.400,300.*1E-4,0.8,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[19][vcd0xd0],varyCosPoint[19][vccospoint],0.998,7.},/* 7.5<pt<8 */
            {0.400,300.*1E-4,0.8,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[20][vcd0xd0],varyCosPoint[20][vccospoint],0.998,6.},/* 8<pt<8.5 */
            {0.400,300.*1E-4,0.8,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[21][vcd0xd0],varyCosPoint[21][vccospoint],0.998,6.},/* 8.5<pt<9 */
            {0.400,300.*1E-4,0.8,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[22][vcd0xd0],varyCosPoint[22][vccospoint],0.998,6.},/* 9<pt<10 */
            {0.400,300.*1E-4,0.8,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[23][vcd0xd0],varyCosPoint[23][vccospoint],0.998,5.},/* 10<pt<12 */
            {0.400,300.*1E-4,0.8,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[24][vcd0xd0],varyCosPoint[24][vccospoint],0.998,5.},/*12< pt <14*/
            {0.400,300.*1E-4,0.8,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[25][vcd0xd0],varyCosPoint[25][vccospoint],0.998,5.},/*14< pt <16*/
            {0.400,400.*1E-4,1.0,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[26][vcd0xd0],varyCosPoint[26][vccospoint],0.995,5.},/*16< pt <20*/
            {0.400,400.*1E-4,1.0,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[27][vcd0xd0],varyCosPoint[27][vccospoint],0.995,5.},/*20< pt <24*/
            {0.400,400.*1E-4,1.0,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[28][vcd0xd0],varyCosPoint[28][vccospoint],0.995,5.},/*24< pt <36*/
            {0.400,400.*1E-4,1.0,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[29][vcd0xd0],varyCosPoint[29][vccospoint],0.995,5.},/*36< pt <50*/
            {0.400,400.*1E-4,1.0,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[30][vcd0xd0],varyCosPoint[30][vccospoint],0.995,5.},/*50< pt <100*/
            {0.400,400.*1E-4,1.0,0.7,0.7,1000.*1E-4,1000.*1E-4,varyd0xd0[31][vcd0xd0],varyCosPoint[31][vccospoint],0.995,5.}};/* pt > 100*/
        
        
        Float_t **cutsMatrixTransposeStand=new Float_t*[nvars];
        for(Int_t iv=0;iv<nvars;iv++)cutsMatrixTransposeStand[iv]=new Float_t[nptbins];
        
        for (Int_t ibin=0;ibin<nptbins;ibin++){
            for (Int_t ivar = 0; ivar<nvars; ivar++){
                cutsMatrixTransposeStand[ivar][ibin]=cutsMatrixD0toKpiStand[ibin][ivar];
                //printf("cutsMatrixD0toKpi[%d][%d] = %f\n",ibin, ivar,cutsMatrixD0toKpiStand[ibin][ivar]);
            }
        }
        
        
        cutsD0toKpi->SetCuts(nvars,nptbins,cutsMatrixTransposeStand);
        cutsD0toKpi->Setd0MeasMinusExpCut(nptbins,d0MeasMinusExpCut);
        
        Bool_t pidflag=kTRUE;
        cutsD0toKpi->SetUsePID(pidflag);
        if(pidflag) cout<<"PID is used"<<endl;
        else cout<<"PID is not used"<<endl;
        
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
        
        pidObj->SetPCompatTOF(2.);
        pidObj->SetSigmaForTPCCompat(3.);
        pidObj->SetSigmaForTOFCompat(3.);
        
        pidObj->SetOldPid(kFALSE);
        
        cutsD0toKpi->SetLowPt(kFALSE);
        cutsD0toKpi->SetUseDefaultPID(kFALSE); //to use the AliAODPidHF
        
        //RDHFD0toKpi->SetMaximumPforPID(4.);
        cutsD0toKpi->SetPidHF(pidObj);
        
        //Do not recalculate the vertex
        cutsD0toKpi->SetRemoveDaughtersFromPrim(kFALSE); //activate for pp
        cutsD0toKpi->SetUseSpecialCuts(kFALSE);

        
        
    }
    
    //event selection
    cutsD0toKpi->SetTriggerClass("");
    cutsD0toKpi->SetTriggerMask(AliVEvent::kINT7);
    cutsD0toKpi->SetMinCentrality(minc);
    cutsD0toKpi->SetMaxCentrality(maxc);
    cutsD0toKpi->SetUseCentrality(AliRDHFCuts::kCentV0M); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,kCentInvalid
    cutsD0toKpi->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
    cutsD0toKpi->SetMaxVtxZ(10.);
    cutsD0toKpi->SetCutOnzVertexSPD(2);
    
    cout<<"This is the object I'm going to save:"<<endl;
    cutsD0toKpi->SetName(nameCuts.Data());
    cutsD0toKpi->SetTitle(nameCuts.Data());
    cutsD0toKpi->PrintAll();
    
    return cutsD0toKpi;
    
}


