#include <Riostream.h>
#include <TFile.h>
#include <AliRDHFCutsD0toKpi.h>
#include <AliRDHFCutsDstoKKpi.h>
#include <AliRDHFCutsDplustoKpipi.h>
#include <TClonesArray.h>
#include <TParameter.h>

#include "makeInputCutsD0toKpi.C"
#include "makeInputCutsDstoKKpi.C"
#include "makeInputCutsDplustoKpipi.C"


void makeCutsTreeCreator()
{
    Printf("D0 filtering cuts");
    AliRDHFCutsD0toKpi  *looseCutsD0toKpi    = makeInputCutsD0toKpi(0,"D0toKpiFilteringCuts",0.,100.);
    Printf("\n\n");
    Printf("D0 analysis cuts");
    AliRDHFCutsD0toKpi  *analysisCutsD0toKpi = makeInputCutsD0toKpi(1,"D0toKpiAnalysisCuts",0.,100.);
    Printf("\n\n");
    Printf("*************************************************************");
    Printf("Ds filtering cuts");
    AliRDHFCutsDstoKKpi  *looseCutsDstoKKpi    = makeInputCutsDstoKKpi(0,"DstoKKpiFilteringCuts",0.,100.);
    Printf("\n\n");
    Printf("Ds analysis cuts");
    AliRDHFCutsDstoKKpi  *analysisCutsDstoKKpi = makeInputCutsDstoKKpi(1,"DstoKKpiAnalysisCuts",0.,100.);
    Printf("\n\n");
    Printf("*************************************************************");
    Printf("Dplus filtering cuts");
    AliRDHFCutsDplustoKpipi  *looseCutsDplustoKpipi    = makeInputCutsDplustoKpipi(0,"DplustoKpipiFilteringCuts",0.,100.);
    Printf("\n\n");
    Printf("Dplus analysis cuts");
    AliRDHFCutsDplustoKpipi  *analysisCutsDplustoKpipi = makeInputCutsDplustoKpipi(1,"DplustoKpipiAnalysisCuts",0.,100.);
    
    TFile* fout=new TFile("D0DsDplusCuts.root","recreate");
    fout->cd();
    looseCutsD0toKpi->Write();
    analysisCutsD0toKpi->Write();
    looseCutsDstoKKpi->Write();
    analysisCutsDstoKKpi->Write();
    looseCutsDplustoKpipi->Write();
    analysisCutsDplustoKpipi->Write();
    fout->Close();
    
}


