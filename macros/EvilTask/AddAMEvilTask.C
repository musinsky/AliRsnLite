Bool_t AddAMEvilTask(TString analysisSource = "proof", TString analysisMode = "test",TString input="aod",TString inputMC="", TString postfix = "",TString idStr="0")
{
   Int_t usePhysSel              = 0;
   Bool_t useMC = !inputMC.CompareTo("mc");

   // ALICE stuff
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) { Printf("Error[AddAMEvilTask] mgr is null !!!"); return kFALSE; }

   AliAnalysisGrid *analysisPlugin = mgr->GetGridHandler();
   if (!analysisPlugin) { Printf("Error[AddAMEvilTask] : analysisPlugin is null !!!"); return kFALSE; }

   gROOT->LoadMacro("AliAnalysisTaskEvil.cxx+g");
   analysisPlugin->SetAnalysisSource("AliAnalysisTaskEvil.cxx");
   analysisPlugin->SetAdditionalLibs("AliAnalysisTaskEvil.h AliAnalysisTaskEvil.cxx");



   if (usePhysSel) {
      gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
      AddTaskPhysicsSelection(useMC);

      // maybe we can put it in $ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C
      AliMultiInputEventHandler *multiIH = dynamic_cast<AliMultiInputEventHandler *>(mgr->GetInputEventHandler());
      if (multiIH){
         AliESDInputHandler *esdIH = dynamic_cast<AliESDInputHandler *>(multiIH->GetFirstInputEventHandler());
         if (esdIH) esdIH->SetEventSelection(multiIH->GetEventSelection());
         AliAODInputHandler *aodIH = dynamic_cast<AliAODInputHandler *>(multiIH->GetFirstInputEventHandler());
         if (aodIH) aodIH->SetEventSelection(multiIH->GetEventSelection());
      }
   }



   // load and run AddTask macro
   gROOT->LoadMacro("AddEvilAnalysisTask.C");
   AddEvilAnalysisTask(input, useMC, postfix);

   return kTRUE;
}
