#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsD0toKpi.h>
#include <AliRDHFCutsDstoKKpi.h>
#include <AliRDHFCutsDplustoKpipi.h>
#include <AliRDHFCutsLctopKpi.h>
#include <AliRDHFCutsLctoV0.h>
#include <TClonesArray.h>
#include <TParameter.h>

#include "makeInputCutsD0toKpi_PbPb2018_noCentrality.C"
#include "makeInputCutsDstoKKpi_PbPb2018_noCentrality.C"
#include "makeInputCutsDplustoKpipi_PbPb2018_noCentrality.C"
#include "makeInputCutsBplustoD0pi_PbPb2018_noCentrality.C"
#include "makeInputCutsLctopKpi_PbPb2018_noCentrality.C"
#include "makeInputCutsDstartoKpipi_PbPb2018_noCentrality.C"
#include "makeInputCutsLctoV0_PbPb2018_noCentrality.C"
#include "makeInputCutsBstoDspi_PbPb2018_noCentrality.C"
#include "makeInputCutsLbtoLcpi_PbPb2018_noCentrality.C"

Float_t minCent=0.;
Float_t maxCent=100.;


void makeCutsTreeCreator_PbPb2018_noCentrality(Bool_t usePID = kTRUE, Bool_t usePreSelectBs = kTRUE)
{
  Printf("D0 filtering cuts");
  AliRDHFCutsD0toKpi  *looseCutsD0toKpi    = makeInputCutsD0toKpi(0,"D0toKpiFilteringCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("D0 analysis cuts");
  AliRDHFCutsD0toKpi  *analysisCutsD0toKpi = makeInputCutsD0toKpi(1,"D0toKpiAnalysisCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Ds filtering cuts");
  AliRDHFCutsDstoKKpi  *looseCutsDstoKKpi    = makeInputCutsDstoKKpi(0,"DstoKKpiFilteringCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("Ds analysis cuts");
  AliRDHFCutsDstoKKpi  *analysisCutsDstoKKpi = makeInputCutsDstoKKpi(1,"DstoKKpiAnalysisCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Dplus filtering cuts");
  AliRDHFCutsDplustoKpipi  *looseCutsDplustoKpipi    = makeInputCutsDplustoKpipi(0,"DplustoKpipiFilteringCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("Dplus analysis cuts");
  AliRDHFCutsDplustoKpipi  *analysisCutsDplustoKpipi = makeInputCutsDplustoKpipi(1,"DplustoKpipiAnalysisCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Dstar filtering cuts");
  AliRDHFCutsDStartoKpipi  *looseCutsDStartoKpipi    = makeInputCutsDstartoKpipi(0,"DstartoKpipiFilteringCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("Dstar analysis cuts");
  AliRDHFCutsDStartoKpipi  *analysisCutsDStartoKpipi = makeInputCutsDstartoKpipi(1,"DstartoKpipiAnalysisCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("LctopKpi filtering cuts");
  AliRDHFCutsLctopKpi  *looseCutsLctopKpi    = makeInputCutsLctopKpi(0,"LctopKpiFilteringCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("LctopKpi analysis cuts");
  AliRDHFCutsLctopKpi  *analysisCutsLctopKpi = makeInputCutsLctopKpi(1,"LctopKpiAnalysisCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Bplus filtering cuts");
  AliRDHFCutsD0toKpi  *looseCutsBplustoD0pi    = makeInputCutsBplustoD0pi(0,"BplustoD0piFilteringCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("Bplus analysis cuts");
  AliRDHFCutsD0toKpi  *analysisCutsBplustoD0pi = makeInputCutsBplustoD0pi(1,"BplustoD0piAnalysisCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("LctoV0bachelor filtering cuts");
  AliRDHFCutsLctoV0  *looseCutsLctoV0bachelor    = makeInputCutsLctoV0(0,"Lc2V0bachelorFilteringCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("LctoV0bachelor analysis cuts");
  AliRDHFCutsLctoV0  *analysisCutsLctoV0bachelor = makeInputCutsLctoV0(1,"Lc2V0bachelorAnalysisCuts",minCent,maxCent,usePID);
  Printf("*************************************************************");
  Printf("\n\n");
  Printf("Bs filtering cuts");
  AliRDHFCutsDstoKKpi  *looseCutsBstoDspi    = makeInputCutsBstoDspi(0,"BstoDspiFilteringCuts",minCent,maxCent,usePID,usePreSelectBs);
  Printf("\n\n");
  Printf("Bs analysis cuts");
  AliRDHFCutsDstoKKpi  *analysisCutsBstoDspi = makeInputCutsBstoDspi(1,"BstoDspiAnalysisCuts",minCent,maxCent,usePID,usePreSelectBs);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("LbtoLcpi filtering cuts");
  AliRDHFCutsLctopKpi *looseCutsLbtoLcpi = makeInputCutsLbtoLcpi(0,"LbtoLcpiFilteringCuts",minCent,maxCent,usePID);
  Printf("\n\n");
  Printf("LbtoLcpi analysis cuts");
  AliRDHFCutsLctopKpi *analysisCutsLbtoLcpi= makeInputCutsLbtoLcpi(1,"LbtoLcpiAnalysisCuts",minCent,maxCent,usePID);
  
  TFile* fout;
  if(usePID && usePreSelectBs) fout=new TFile("D0DsDplusDstarLcBplusBsLbCuts_PbPb2018_kAny_noCentnoPS_ConsvPID_usePreSelect.root","recreate");
  else if(usePID && !usePreSelectBs) fout=new TFile("D0DsDplusDstarLcBplusBsLbCuts_PbPb2018_kAny_noCentnoPS_ConsvPID.root","recreate");
  else if(!usePID && !usePreSelectBs) fout=new TFile("D0DsDplusDstarLcBplusBsLbCuts_PbPb2018_kAny_noCentnoPS.root","recreate");
  else       fout=new TFile("D0DsDplusDstarLcBplusBsLbCuts_PbPb2018_kAny_noCentnoPSnoPID_usePreSelect.root","recreate");
  fout->cd();
  looseCutsD0toKpi->Write();
  analysisCutsD0toKpi->Write();
  looseCutsDstoKKpi->Write();
  analysisCutsDstoKKpi->Write();
  looseCutsDplustoKpipi->Write();
  analysisCutsDplustoKpipi->Write();
  looseCutsLctopKpi->Write();
  analysisCutsLctopKpi->Write();
  looseCutsBplustoD0pi->Write();
  analysisCutsBplustoD0pi->Write();
  looseCutsDStartoKpipi->Write();
  analysisCutsDStartoKpipi->Write();
  looseCutsLctoV0bachelor->Write();
  analysisCutsLctoV0bachelor->Write();
  looseCutsBstoDspi->Write();
  analysisCutsBstoDspi->Write();
  looseCutsLbtoLcpi->Write();
  analysisCutsLbtoLcpi->Write();
  fout->Close();
  
}
