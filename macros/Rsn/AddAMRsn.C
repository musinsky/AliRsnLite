#ifndef __CINT__
#include <TString.h>
#include <TROOT.h>
#include <TSystem.h>
#include <ANALYSIS/AliAnalysisManager.h>
#include <PWG2/RESONANCES/AliRsnInputHandler.h>
#include <ANALYSIS/AliAnalysisAlien.h>
#include <ANALYSIS/AliAnalysisTaskSE.h>
#endif

Bool_t AddAMRsn(TString analysisSource = "proof", TString analysisMode = "test",TString input="aod",TString inputMC="", TString postfix = "",TString idStr="0")
{

   if (!RsnLoadMacro("RsnManager.C")) return kFALSE;
   TList *listRsn = RsnManager();

   Bool_t useMC = !inputMC.CompareTo("mc");
   Bool_t valid;

   Int_t eventMixinPar = AliAnalysisManager::GetGlobalInt("rsnUseEventMixingPar",valid);
   if (eventMixinPar) AliAnalysisManager::SetGlobalInt(rsnUseRSNPar,1);
   Int_t rsnPar = AliAnalysisManager::GetGlobalInt("rsnUseRSNPar",valid);

   Int_t pidResponse = AliAnalysisManager::GetGlobalInt("rsnUsePIDResponse",valid);
   Int_t useRsnIH = AliAnalysisManager::GetGlobalInt("rsnUseRsnInputHandler",valid);
   Int_t physSel = AliAnalysisManager::GetGlobalInt("rsnUsePhysSel",valid);
   Int_t splitMgrByTask = AliAnalysisManager::GetGlobalInt("rsnSplitMgrByTasks",valid);

   Int_t useMixing = AliAnalysisManager::GetGlobalInt("rsnUseMixing",valid);

   Int_t isRsnMini = AliAnalysisManager::GetGlobalInt("rsnUseMiniPackage",valid);
   Int_t mixNum = AliAnalysisManager::GetGlobalInt("rsnNumMix",valid);

   // ALICE stuff
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) { Printf("Error[AddAMRsn] mgr is null !!!"); return kFALSE; }

   AliAnalysisGrid *analysisPlugin = mgr->GetGridHandler();
   if (!analysisPlugin) { Printf("Error[AddAMRsn] : analysisPlugin is null !!!"); return kFALSE; }

   TString myAdditionalLibs;
   if (eventMixinPar) { AliAnalysisAlien::SetupPar("EventMixing"); myAdditionalLibs += " EventMixing.par"; }
   else { gSystem->Load("libEventMixing.so"); myAdditionalLibs += " libEventMixing.so"; }

   if (rsnPar) { AliAnalysisAlien::SetupPar("PWG2resonances"); myAdditionalLibs += " PWG2resonances.par"; }
   else { gSystem->Load("libPWG2resonances.so"); myAdditionalLibs += " libPWG2resonances.so"; }

   analysisPlugin->SetAdditionalLibs(myAdditionalLibs.Data());


   AliMultiInputEventHandler *multiInputHandler = mgr->GetInputEventHandler();
   AliRsnInputHandler *rsnIH=0;

   if (pidResponse) {
      // add PID Response Handler
      if (!RsnLoadMacro("AddPIDResponseInputHandler.C")) return kFALSE;
      AddPIDResponseInputHandler(multiInputHandler);
   }

   if (useRsnIH) {
      // add Rsn input handler (it has to be after ESD,MC,Tender input handler, but before Mixing)
      AliRsnInputHandler *rsnIH = new AliRsnInputHandler();
      multiInputHandler->AddInputEventHandler(rsnIH);
   }

   if (physSel) {
      gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
      AddTaskPhysicsSelection(useMC);

      // maybe we can put it in $ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C
      AliMultiInputEventHandler *multiIH = dynamic_cast<AliMultiInputEventHandler *>(mgr->GetInputEventHandler());
      if (multiIH) {
         AliESDInputHandler *esdIH = dynamic_cast<AliESDInputHandler *>(multiIH->GetFirstInputEventHandler());
         if (esdIH) esdIH->SetEventSelection(multiIH->GetEventSelection());
         AliAODInputHandler *aodIH = dynamic_cast<AliAODInputHandler *>(multiIH->GetFirstInputEventHandler());
         if (aodIH) aodIH->SetEventSelection(multiIH->GetEventSelection());
      }
   }


   // load and run AddTask macro
   if (!RsnLoadMacro("AddRsnAnalysisTask.C")) return kFALSE;
   if (!RsnLoadMacro("RsnConfig.C")) return kFALSE;
   if (!RsnLoadMacro("AddMixingHandler.C")) return kFALSE;
   if (!RsnLoadMacro("RsnGirdPlugin.C")) return kFALSE;
   RsnGridPlugin();

   if (splitMgrByTask) {
      Int_t iTask=0;
      TList *l=0;
      TNamed *rsnObj = 0;
      AliAnalysisTaskSE *task=0;
      TString rsnName,rsnCutName;
      TIter next(listRsn);
      while ((rsnObj = (TNamed *)next())) {
         l = new TList();
         Printf("Adding task for RSN:%s CUT:%s ",rsnObj->GetName(),rsnObj->GetTitle());
         l->Add(new TNamed(*rsnObj));
         task = AddRsnAnalysisTask(input, useMC, useMixing,rsnIH,l,Form("%s_%d",postfix.Data(),iTask++));
         if (useMixing) {
            // add mixing handler (uncomment to turn on Mixnig)
            AddMixingHandler(multiInputHandler,task, input, useMC,isRsnMini, mixNum,postfix);
         }
      }
   } else {
      task = AddRsnAnalysisTask(input, useMC, useMixing,rsnIH,listRsn,postfix);
      if (useMixing) {
         // add mixing handler (uncomment to turn on Mixnig)
         AddMixingHandler(multiInputHandler,task, input, useMC,isRsnMini, mixNum,postfix);
      }
   }

   return kTRUE;
}

Bool_t RsnLoadMacro(TString macro,TString path="") {

   Bool_t valid;
   TString lego_path = AliAnalysisManager::GetGlobalStr("rsnLegoTrainPath",valid);
   if (!valid) lego_path = "$ALICE_ROOT/PWG2/RESONANCES/macros/lego_train";

   if (!gSystem->AccessPathName(macro.Data())) {
      gROOT->LoadMacro(macro.Data());
      Printf("Macro loaded from %s/%s ...",gSystem->pwd(),macro.Data());
      return kTRUE;
   }

   if (!gSystem->AccessPathName(gSystem->ExpandPathName(Form("%s/%s",lego_path.Data(),macro.Data())))) {
      gROOT->LoadMacro(gSystem->ExpandPathName(Form("%s/%s",lego_path.Data(),macro.Data())));
      Printf("Macro loaded from %s ...",gSystem->ExpandPathName(Form("%s/%s",lego_path.Data(),macro.Data())));
      return kTRUE;
   }

   Printf("Error loading %s",macro.Data());

   return kFALSE;
}