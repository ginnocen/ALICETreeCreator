#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsD0toKpi.h>
#include <AliRDHFCutsDstoKKpi.h>
#include <AliRDHFCutsDplustoKpipi.h>
#include <AliRDHFCutsLctopKpi.h>
#include <AliRDHFCutsLctoV0.h>
#include <TClonesArray.h>
#include <TParameter.h>

#include "makeInputCutsD0toKpi_pp.C"
#include "makeInputCutsDstoKKpi_pp.C"
#include "makeInputCutsDplustoKpipi_pp.C"
#include "makeInputCutsDstartoKpipi_pp.C"
#include "makeInputCutsLctopKpi_pp.C"
#include "makeInputCutsBplustoD0pi_pp.C"
#include "makeInputCutsBstoDspi_pp.C"
#include "makeInputCutsLctoV0_pp.C"
#include "makeInputCutsLbtoLcpi_pp.C"

Float_t minCent=0.;
Float_t maxCent=100.;


void makeCutsTreeCreator_pp()
{
  Printf("D0 filtering cuts");
  AliRDHFCutsD0toKpi  *looseCutsD0toKpi    = makeInputCutsD0toKpi_pp(0,"D0toKpiFilteringCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("D0 analysis cuts");
  AliRDHFCutsD0toKpi  *analysisCutsD0toKpi = makeInputCutsD0toKpi_pp(1,"D0toKpiAnalysisCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Ds filtering cuts");
  AliRDHFCutsDstoKKpi  *looseCutsDstoKKpi    = makeInputCutsDstoKKpi_pp(0,"DstoKKpiFilteringCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("Ds analysis cuts");
  AliRDHFCutsDstoKKpi  *analysisCutsDstoKKpi = makeInputCutsDstoKKpi_pp(1,"DstoKKpiAnalysisCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Dplus filtering cuts");
  AliRDHFCutsDplustoKpipi  *looseCutsDplustoKpipi    = makeInputCutsDplustoKpipi_pp(0,"DplustoKpipiFilteringCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("Dplus analysis cuts");
  AliRDHFCutsDplustoKpipi  *analysisCutsDplustoKpipi = makeInputCutsDplustoKpipi_pp(1,"DplustoKpipiAnalysisCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Dstar filtering cuts");
  AliRDHFCutsDStartoKpipi  *looseCutsDstartoKpipi    = makeInputCutsDstartoKpipi_pp(0,"DstartoKpipiFilteringCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("Dstar analysis cuts");
  AliRDHFCutsDStartoKpipi  *analysisCutsDstartoKpipi = makeInputCutsDstartoKpipi_pp(1,"DstartoKpipiAnalysisCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("LctopKpi filtering cuts");
  AliRDHFCutsLctopKpi *looseCutsLctopKpi = makeInputCutsLctopKpi_pp(0,"LctopKpiFilteringCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("LctopKpi analysis cuts");
  AliRDHFCutsLctopKpi *analysisCutsLctopKpi= makeInputCutsLctopKpi_pp(1,"LctopKpiAnalysisCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Bplus filtering cuts");
  AliRDHFCutsD0toKpi *looseCutsBplustoD0pi    = makeInputCutsBplustoD0pi_pp(0,"BplustoD0piFilteringCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("Bplus analysis cuts");
  AliRDHFCutsD0toKpi *analysisCutsBplustoD0pi = makeInputCutsBplustoD0pi_pp(1,"BplustoD0piAnalysisCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("LctoV0bachelor filtering cuts");
  AliRDHFCutsLctoV0  *looseCutsLctoV0bachelor    = makeInputCutsLctoV0(0,"Lc2V0bachelorFilteringCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("LctoV0bachelor analysis cuts");
  AliRDHFCutsLctoV0  *analysisCutsLctoV0bachelor = makeInputCutsLctoV0(1,"Lc2V0bachelorAnalysisCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Bs filtering cuts");
  AliRDHFCutsDstoKKpi  *looseCutsBstoDspi    = makeInputCutsBstoDspi_pp(0,"BstoDspiFilteringCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("Bs analysis cuts");
  AliRDHFCutsDstoKKpi  *analysisCutsBstoDspi = makeInputCutsBstoDspi_pp(1,"BstoDspiAnalysisCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("LbtoLcpi filtering cuts");
  AliRDHFCutsLctopKpi *looseCutsLbtoLcpi = makeInputCutsLbtoLcpi_pp(0,"LbtoLcpiFilteringCuts",minCent,maxCent);
  Printf("\n\n");
  Printf("LbtoLcpi analysis cuts");
  AliRDHFCutsLctopKpi *analysisCutsLbtoLcpi= makeInputCutsLbtoLcpi_pp(1,"LbtoLcpiAnalysisCuts",minCent,maxCent);
  
  TFile* fout=new TFile("D0DsDplusDstarLcBplusBsLbCuts_pp.root","recreate");
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
  looseCutsDstartoKpipi->Write();
  analysisCutsDstartoKpipi->Write();
  looseCutsLctoV0bachelor->Write();
  analysisCutsLctoV0bachelor->Write();
  looseCutsBstoDspi->Write();
  analysisCutsBstoDspi->Write();
  looseCutsLbtoLcpi->Write();
  analysisCutsLbtoLcpi->Write();
  fout->Close();
  
}
