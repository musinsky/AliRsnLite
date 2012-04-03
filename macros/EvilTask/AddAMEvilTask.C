Bool_t AddAMEvilTask(TString analysisSource = "proof", TString analysisMode = "test",TString input="aod",TString inputMC="", TString postfix = "",TString idStr="0")
{
   Int_t usePhysSel              = 0;
   Int_t useCentralityTask       = 1;
   
   Bool_t useMC = !inputMC.CompareTo("mc");

   // ALICE stuff
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) { Printf("Error[AddAMEvilTask] mgr is null !!!"); return kFALSE; }

   AliAnalysisGrid *analysisPlugin = mgr->GetGridHandler();
   if (!analysisPlugin) { Printf("Error[AddAMEvilTask] : analysisPlugin is null !!!"); return kFALSE; }

   gROOT->LoadMacro("AliAnalysisTaskEvil.cxx+g");
   analysisPlugin->SetAnalysisSource("AliAnalysisTaskEvil.cxx");
   analysisPlugin->SetAdditionalLibs("AliAnalysisTaskEvil.h AliAnalysisTaskEvil.cxx");

   AliMultiInputEventHandler *multiInputHandler = 0;
   AliInputEventHandler *inputHandler = mgr->GetInputEventHandler();

   TString className = inputHandler->ClassName();
   if (!className.CompareTo("AliMultiInputEventHandler")) {
      multiInputHandler = (AliMultiInputEventHandler*)inputHandler;
   }

   if (usePhysSel) {
      gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
      AddTaskPhysicsSelection(useMC);

      // maybe we can put it in $ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C
      if (multiInputHandler) {
         AliInputEventHandler *ih = multiInputHandler->GetFirstInputEventHandler();
         ih->SetEventSelection(multiIH->GetEventSelection());
      }
   }

   if (useCentralityTask) {
      gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskCentrality.C");
      AliCentralitySelectionTask *centralityTask = AddTaskCentrality(kFALSE);
   }
   
   // load and run AddTask macro
   gROOT->LoadMacro("AddEvilAnalysisTask.C");
   AddEvilAnalysisTask(input, useMC, postfix);

   return kTRUE;
}
