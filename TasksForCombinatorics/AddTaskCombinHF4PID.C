AliAnalysisTaskCombinHF4PID *AddTaskCombinHF4PID(Int_t meson = 0,
						 Bool_t readMC = kTRUE,
						 Int_t filterMask = 4)
{
  
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTaskCombinHF4PID", "No analysis manager to connect to.");
  }
  
  //Analysis Task
  
  
  AliRDHFCuts* analysiscuts=0x0;
  Double_t minMass=1.72;
  Double_t maxMass=2.15;
  
  if(meson==0) analysiscuts=new AliRDHFCutsD0toKpi();
  else if(meson==1) analysiscuts=new AliRDHFCutsDplustoKpipi();
  else if(meson==3) analysiscuts=new AliRDHFCutsDstoKKpi();
  else if(meson==4){
    analysiscuts=new AliRDHFCutsLctopKpi();
    minMass=1.98;
    maxMass=2.48;
  }
  analysiscuts->SetStandardCutsPP2010();
  
  AliAnalysisTaskCombinHF4PID *dTask = new AliAnalysisTaskCombinHF4PID(meson,analysiscuts);
  dTask->SetReadMC(readMC);
  dTask->SetDebugLevel(0);
  
  dTask->SetFilterMask(filterMask);
  
  // mass and pt range for histograms
  dTask->SetMassWindow(minMass, maxMass);
  
  mgr->AddTask(dTask);
  
  
  // Create containers for input/output
  
  TString mesname="Dzero";
  if(meson==1) mesname="Dplus";
  else if(meson==3) mesname="Ds";
  else if(meson==4) mesname="Lc";
  TString containerStr="";
  TString inname = Form("cinput%s%s",mesname.Data(),containerStr.Data());
  TString outname = Form("coutput%s%s",mesname.Data(),containerStr.Data());
  TString treename = Form("coutput%sTree%s",mesname.Data(),containerStr.Data());
  
  AliAnalysisDataContainer *cinput = mgr->CreateContainer(inname,TChain::Class(),
                                                          AliAnalysisManager::kInputContainer);
  TString outputfile = AliAnalysisManager::GetCommonFileName();
  outputfile += Form(":%sPID%s",mesname.Data(),containerStr.Data());
  
  
  AliAnalysisDataContainer *coutput = mgr->CreateContainer(outname,TList::Class(),
                                                           AliAnalysisManager::kOutputContainer,
                                                           outputfile.Data());
  AliAnalysisDataContainer *coutputTree = mgr->CreateContainer(treename,TTree::Class(),
                                                               AliAnalysisManager::kOutputContainer,
                                                               outputfile.Data());
  
  coutputTree->SetSpecialOutput();

  mgr->ConnectInput(dTask,0,mgr->GetCommonInputContainer());
  mgr->ConnectOutput(dTask,1,coutput);
  mgr->ConnectOutput(dTask,2,coutputTree);
  
  return dTask;
}
