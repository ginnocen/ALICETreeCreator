#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsD0toKpi.h>
#include <AliRDHFCutsLctoV0.h>
#include <TClonesArray.h>
#include <TParameter.h>

#include "makeInputCutsD0toKpi_PbPb2018_Cent1030.C"
#include "makeInputCutsLctoV0_PbPb2018_Cent1030.C"

Float_t minCent=10.;
Float_t maxCent=30.;


void makeCutsTreeCreator_PbPb2018_Central(Bool_t isMC = kFALSE, Int_t OptPreSelect = 1, Bool_t PIDcorrection = kTRUE, Int_t TPCClsPID = 50, Double_t minptLc = 2, Bool_t spdkAnyLc = kTRUE, Bool_t ispass1 = kFALSE)
{
  if(isMC && PIDcorrection){
    cout << "\n\033[1;31m--Warning (08/06/20)--\033[0m\n";
    cout << "  Datadriven PID correction should not be applied on MC. Disabling..." << endl;
    PIDcorrection = kFALSE;
    cout << "\033[1;31m----------------------\033[0m\n\n";
  }
  
  Printf("D0 filtering cuts");
  AliRDHFCutsD0toKpi  *looseCutsD0toKpi    = makeInputCutsD0toKpi(0, "D0toKpiFilteringCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection, ispass1);
  Printf("\n\n");
  Printf("D0 analysis cuts");
  AliRDHFCutsD0toKpi  *analysisCutsD0toKpi = makeInputCutsD0toKpi(1, "D0toKpiAnalysisCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection, ispass1);
  Printf("\n\n");
  Printf("*************************************************************");
  Printf("LctoV0bachelor filtering cuts");
  AliRDHFCutsLctoV0  *looseCutsLctoV0bachelor    = makeInputCutsLctoV0(0, "Lc2V0bachelorFilteringCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection, minptLc, spdkAnyLc, ispass1);
  Printf("\n\n");
  Printf("LctoV0bachelor analysis cuts");
  AliRDHFCutsLctoV0  *analysisCutsLctoV0bachelor = makeInputCutsLctoV0(1, "Lc2V0bachelorAnalysisCuts", minCent, maxCent, isMC, OptPreSelect, TPCClsPID, PIDcorrection, minptLc, spdkAnyLc, ispass1);

  if(ispass1){
    cout << "\n\033[1;31m--Created cutobjects for pass1--\033[0m\n";
    cout << "  The chi2/ndf cut will be disabled" << endl;
    cout << "\033[1;31m----------------------\033[0m\n\n";
  }

  if(OptPreSelect){
    cout << "\n\033[1;31m--Info (08/06/20)--\033[0m\n";
    cout << "  PreSelect enabled, but not yet implemented for all HF hadrons." << endl;
    cout << "  Please check (and implement if needed)" << endl;
    cout << "\033[1;31m----------------------\033[0m\n\n";
  }

  TString fname = "cutobjects/D0LcCuts_PbPb2018_";
  if(isMC) fname += "MC_";
  else     fname += "Data_";
  if(OptPreSelect) fname += Form("PreSelect%d_",OptPreSelect);
  else             fname += "noPreSelect_";
  if(PIDcorrection) fname += "DDPIDCorr_";
  else              fname += "noPIDCorr_";
  if(TPCClsPID > 0) fname += Form("TPCClsPID%d_",TPCClsPID);
  if(spdkAnyLc) fname += Form("LcpK0skAnyMinpT%d",(int)(1000*minptLc));
  else          fname += Form("LcpK0sMinpT%d",(int)(1000*minptLc));
  if(ispass1) fname += Form("_Cent%d%d_pass1.root",(int)(minCent),(int)(maxCent));
  else        fname += Form("_Cent%d%d.root",(int)(minCent),(int)(maxCent));
  
  cout << "\n\nSaving cut objects in: " << fname << endl;
  
  TFile* fout = new TFile(fname.Data(),"recreate");
  fout->cd();
  looseCutsD0toKpi->Write();
  analysisCutsD0toKpi->Write();
  looseCutsLctoV0bachelor->Write();
  analysisCutsLctoV0bachelor->Write();
  fout->Close();
  
}
