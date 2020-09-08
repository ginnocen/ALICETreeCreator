#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsD0toKpi.h>
#include <AliRDHFCutsDstoKKpi.h>
#include <AliRDHFCutsDplustoKpipi.h>
#include <AliRDHFCutsLctopKpi.h>
#include <AliRDHFCutsLctoV0.h>
#include <TClonesArray.h>
#include <TParameter.h>

#include "makeInputCutsD0toKpi_PbPb2018_SemiCentral.C"
#include "makeInputCutsDstoKKpi_PbPb2018_SemiCentral.C"
#include "makeInputCutsDplustoKpipi_PbPb2018_SemiCentral.C"
#include "makeInputCutsDstartoKpipi_PbPb2018_SemiCentral.C"
#include "makeInputCutsLctopKpi_PbPb2018_SemiCentral.C"
#include "makeInputCutsBplustoD0pi_PbPb2018_SemiCentral.C"
#include "makeInputCutsBstoDspi_PbPb2018_SemiCentral.C"
#include "makeInputCutsLctoV0_PbPb2018_SemiCentral.C"
#include "makeInputCutsLbtoLcpi_PbPb2018_SemiCentral.C"

Float_t minCent=30.;
Float_t maxCent=50.;


void makeCutsTreeCreator_PbPb2018_SemiCentral(Bool_t isMC = kFALSE, Int_t OptPreSelect = 1, Bool_t PIDcorrection = kTRUE, Int_t TPCClsPID = 50, Double_t minptLc = 2, Bool_t spdkAnyLc = kTRUE)
{
  if(isMC && PIDcorrection){
    cout << "\n\033[1;31m--Warning (08/06/20)--\033[0m\n";
    cout << "  Datadriven PID correction should not be applied on MC. Disabling..." << endl;
    PIDcorrection = kFALSE;
    cout << "\033[1;31m----------------------\033[0m\n\n";
  }
  
  Printf("D0 filtering cuts");
  AliRDHFCutsD0toKpi  *looseCutsD0toKpi    = makeInputCutsD0toKpi(0, "D0toKpiFilteringCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("D0 analysis cuts");
  AliRDHFCutsD0toKpi  *analysisCutsD0toKpi = makeInputCutsD0toKpi(1, "D0toKpiAnalysisCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Ds filtering cuts");
  AliRDHFCutsDstoKKpi  *looseCutsDstoKKpi    = makeInputCutsDstoKKpi(0, "DstoKKpiFilteringCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("Ds analysis cuts");
  AliRDHFCutsDstoKKpi  *analysisCutsDstoKKpi = makeInputCutsDstoKKpi(1, "DstoKKpiAnalysisCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Dplus filtering cuts");
  AliRDHFCutsDplustoKpipi  *looseCutsDplustoKpipi    = makeInputCutsDplustoKpipi(0, "DplustoKpipiFilteringCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("Dplus analysis cuts");
  AliRDHFCutsDplustoKpipi  *analysisCutsDplustoKpipi = makeInputCutsDplustoKpipi(1, "DplustoKpipiAnalysisCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Dstar filtering cuts");
  AliRDHFCutsDStartoKpipi  *looseCutsDStartoKpipi    = makeInputCutsDstartoKpipi(0, "DstartoKpipiFilteringCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("Dstar analysis cuts");
  AliRDHFCutsDStartoKpipi  *analysisCutsDStartoKpipi = makeInputCutsDstartoKpipi(1, "DstartoKpipiAnalysisCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("LctopKpi filtering cuts");
  AliRDHFCutsLctopKpi  *looseCutsLctopKpi    = makeInputCutsLctopKpi(0, "LctopKpiFilteringCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("LctopKpi analysis cuts");
  AliRDHFCutsLctopKpi  *analysisCutsLctopKpi = makeInputCutsLctopKpi(1, "LctopKpiAnalysisCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Bplus filtering cuts");
  AliRDHFCutsD0toKpi  *looseCutsBplustoD0pi    = makeInputCutsBplustoD0pi(0, "BplustoD0piFilteringCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("Bplus analysis cuts");
  AliRDHFCutsD0toKpi  *analysisCutsBplustoD0pi = makeInputCutsBplustoD0pi(1, "BplustoD0piAnalysisCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("LctoV0bachelor filtering cuts");
  AliRDHFCutsLctoV0  *looseCutsLctoV0bachelor    = makeInputCutsLctoV0(0, "Lc2V0bachelorFilteringCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection, minptLc, spdkAnyLc);
  Printf("\n\n");
  Printf("LctoV0bachelor analysis cuts");
  AliRDHFCutsLctoV0  *analysisCutsLctoV0bachelor = makeInputCutsLctoV0(1, "Lc2V0bachelorAnalysisCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection, minptLc, spdkAnyLc);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("Bs filtering cuts");
  AliRDHFCutsDstoKKpi  *looseCutsBstoDspi    = makeInputCutsBstoDspi(0, "BstoDspiFilteringCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("Bs analysis cuts");
  AliRDHFCutsDstoKKpi  *analysisCutsBstoDspi = makeInputCutsBstoDspi(1, "BstoDspiAnalysisCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("LbtoLcpi filtering cuts");
  AliRDHFCutsLctopKpi *looseCutsLbtoLcpi = makeInputCutsLbtoLcpi(0, "LbtoLcpiFilteringCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);
  Printf("\n\n");
  Printf("LbtoLcpi analysis cuts");
  AliRDHFCutsLctopKpi *analysisCutsLbtoLcpi= makeInputCutsLbtoLcpi(1, "LbtoLcpiAnalysisCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection);

  if(OptPreSelect){
    cout << "\n\033[1;31m--Info (08/06/20)--\033[0m\n";
    cout << "  PreSelect enabled, but not yet implemented for all HF hadrons." << endl;
    cout << "  Please check (and implement if needed)" << endl;
    cout << "\033[1;31m----------------------\033[0m\n\n";
  }
  
  TString fname = "cutobjects/D0DsDplusDstarLcBplusBsLbCuts_PbPb2018_";
  if(isMC) fname += "MC_";
  else     fname += "Data_";
  if(OptPreSelect) fname += Form("PreSelect%d_",OptPreSelect);
  else             fname += "noPreSelect_";
  if(PIDcorrection) fname += "DDPIDCorr_";
  else              fname += "noPIDCorr_";
  if(TPCClsPID > 0) fname += Form("TPCClsPID%d_",TPCClsPID);
  if(spdkAnyLc) fname += Form("LcpK0skAnyMinpT%d",(int)(1000*minptLc));
  else          fname += Form("LcpK0sMinpT%d",(int)(1000*minptLc));
  fname += "_SemiCentral.root";
  
  cout << "\n\nSaving cut objects in: " << fname << endl;
  
  TFile* fout = new TFile(fname.Data(),"recreate");
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
