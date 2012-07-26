#ifndef __CINT__
#include <Rtypes.h>
#include <AliLog.h>
#endif

void AddAnalysisTaskMixInfo(TString opts = "")
{
   // create manager
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) return;

   mgr->AddClassDebug("AliAODInputHandler",AliLog::kError);
   mgr->AddClassDebug("AliAODEvent",AliLog::kError);
   
   // create our task
   AliAnalysisTaskMixInfo *task = new AliAnalysisTaskMixInfo("AliAnalysisTaskMixInfo");

   // create mix output container
   AliAnalysisDataContainer *outputMix = mgr->CreateContainer("cMixInfoList", TList::Class(), AliAnalysisManager::kOutputContainer, Form("MixInfo%s.root", opts.Data()));

   // add our task to the manager
   mgr->AddTask(task);

   // finaly connect input and output
   mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
   mgr->ConnectOutput(task, 1, outputMix);

}
