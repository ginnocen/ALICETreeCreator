AliAnalysisTaskSingleTrack4PID *AddTaskSingleTrack4PID(Bool_t readMC = kTRUE,
						       Int_t filterMask = 4)
{
  
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTaskSingleTrack4PID", "No analysis manager to connect to.");
  }
  
  //Analysis Task
  
  
  AliAnalysisTaskSingleTrack4PID *dTask = new AliAnalysisTaskSingleTrack4PID();
  dTask->SetReadMC(readMC);
  dTask->SetDebugLevel(0);  
  dTask->SetFilterMask(filterMask);
  
  mgr->AddTask(dTask);
  
  
  // Create containers for input/output

  TString baseName="SingleTracks";
  TString containerStr="";
  TString inname = Form("cinput%s%s",baseName.Data(),containerStr.Data());
  TString outname = Form("coutput%s%s",baseName.Data(),containerStr.Data());
  TString treename = Form("coutput%sTree%s",baseName.Data(),containerStr.Data());
  
  AliAnalysisDataContainer *cinput = mgr->CreateContainer(inname,TChain::Class(),
                                                          AliAnalysisManager::kInputContainer);
  TString outputfile = AliAnalysisManager::GetCommonFileName();
  outputfile += Form(":%sPID%s",baseName.Data(),containerStr.Data());
  
  
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
