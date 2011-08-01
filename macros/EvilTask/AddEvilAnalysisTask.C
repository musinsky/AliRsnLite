#ifndef __CINT__
#include <TString.h>
#include <ANALYSIS/AliAnalysisManager.h>
#include "AliAnalysisTaskEvil.h"
#endif
AliAnalysisTask *AddEvilAnalysisTask(TString format = "esd", Bool_t useMC = kFALSE,TString postfix="")
{
  // create manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) { Printf("Error [AddEvilAnalysisTask%s] : mgr is null !!!",postfix.Data()); return 0;}

  // create our task
  AliAnalysisTaskEvil *task = new AliAnalysisTaskEvil(Form("AliAnalysisTaskEvil%s",postfix.Data()));

  // uncomment this to use MC
  task->UseMC(useMC);

  task->SetHistRange(100, 0, 10);

  // add some more 1D histograms
//   task->SetNum1D(100);

  // add some more 2D histograms
//    task->SetNum2D(1000);

  // add some more 3D hostograms
//     task->SetNum3D(100);

  // Let's create memory leak (0 - no memory leak)
//   task->SetNumMemoryLeak(1);

  // Set bad host (it will crash on that machine)
//   task->SetBadHost("prf005-iep-grid.saske.sk",1);

//   task->SetNumLoopsInOneEvent(10);

  // no putput is produced (only TList is there)
//    task->NoOutput(kTRUE/*,kTRUE*/);

  // create output container
  AliAnalysisDataContainer *output1 = mgr->CreateContainer(Form("cCustomList%s",postfix.Data()), TList::Class(), AliAnalysisManager::kOutputContainer, "MyEvil.root");

  // add our task to the manager
  mgr->AddTask(task);

  // finaly connect input and output
  mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, output1);

  return task;
}

