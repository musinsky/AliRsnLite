#ifndef __CINT__
#include <TString.h>
#include <TROOT.h>
#include <ANALYSIS/AliAnalysisManager.h>
#include <ANALYSIS/AliAnalysisGrid.h>
#endif

Bool_t AddAMEvilTask(TString analysisSource = "proof", TString analysisMode = "test",TString input="aod",TString inputMC="", TString postfix = "",TString idStr="0")
{

   Bool_t useMC = !inputMC.CompareTo("mc");

   // ALICE stuff
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) { Printf("Error[AddAMEvilTask] mgr is null !!!"); return kFALSE; }

   AliAnalysisGrid *analysisPlugin = mgr->GetGridHandler();
   if (!analysisPlugin) { Printf("Error[AddAMEvilTask] : analysisPlugin is null !!!"); return kFALSE; }

   gROOT->LoadMacro("AliAnalysisTaskEvil.cxx+g");
   analysisPlugin->SetAnalysisSource("AliAnalysisTaskEvil.cxx");
   analysisPlugin->SetAdditionalLibs("AliAnalysisTaskEvil.h AliAnalysisTaskEvil.cxx");

   // load and run AddTask macro
   gROOT->LoadMacro("AddEvilAnalysisTask.C");
   AddEvilAnalysisTask(input, useMC, postfix);

   return kTRUE;
}
