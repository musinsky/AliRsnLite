void RsnTrainInfo(TString analysisMgrFile = "analysis.root") {
   LoadLibsBase();
   TFile *f = TFile::Open(analysisMgrFile.Data(),"READ");
   //   f->ls();
   TList *l = f->GetListOfKeys();
   AliAnalysisManager *mgr = 0;
   TIter next(l);
   TKey *key;
   while ((key=(TKey*)next())) {
      if (!strcmp(key->GetClassName(), "AliAnalysisManager")) {
         mgr = (AliAnalysisManager*)f->Get(key->GetName());
         Printf("");
         PrintManager(mgr);
      }

   }


   while ((mgr = (AliAnalysisManager*) next())) {

   }
   Printf("");
}

void PrintManager(TObject *mgrObj) {
   AliAnalysisManager *mgr = (AliAnalysisManager*)mgrObj;
   if (!mgr->InitAnalysis()) return;
   mgr->RunLocalInit();
   mgr->PrintStatus();

   Printf("Analysis Manager : %s '%s'",mgr->GetName(),mgr->GetTitle() );
   TObjArray *a = mgr->GetTasks();
   if (a) Printf("Tasks total : %d",a->GetEntries());
   TIter next(mgr->GetTasks());
   AliAnalysisTask *task;
   while ((task = (AliAnalysisTask*) next())) {
      if (task->IsA() == AliRsnMiniAnalysisTask::Class()) {
         Printf("   Task [RSN-MINI] : %s '%s'",task->GetName(),task->GetTitle());
         AliRsnMiniAnalysisTask *rsnMiniTask = (AliRsnMiniAnalysisTask *)task;
         rsnMiniTask->Print();
      } else if (task->IsA() == AliRsnAnalysisTask::Class()) {
         Printf("   Task [RSN-----] : %s '%s'",task->GetName(),task->GetTitle());
         AliRsnAnalysisTask *rsnTask = (AliRsnAnalysisTask *)task;
         rsnTask->Print();
         AliVEventHandler *ih = mgr->GetInputEventHandler();
         if (ih == AliMultiInputEventHandler::Class()) {
            AliMultiInputEventHandler *ihMulti = (AliMultiInputEventHandler*) ih;
            TIter nextIH(ihMulti->InputEventHandlers());
            AliRsnInputHandler *rsnIH = 0;
            while ((ih = (AliVEventHandler*) nextIH())) {
               if (ih->IsA() == AliRsnInputHandler::Class()) {
                  rsnIH = ih;
                  AliRsnDaughterSelector *ds =  rsnIH->GetSelector();
                  ds->Print();
               }
            }

         }
      } else {
         Printf("   Task [--------] : %s '%s'",task->GetName(),task->GetTitle());
      }

   }

}

Int_t LoadLibsBase() {
   Int_t num = 0;
   if (gSystem->Load("libTree.so") < 0) {num++; return num;}
   if (gSystem->Load("libGeom.so") < 0) {num++; return num;}
   if (gSystem->Load("libVMC.so") < 0) {num++; return num;}
   if (gSystem->Load("libMinuit.so") < 0) {num++; return num;}
   if (gSystem->Load("libPhysics.so") < 0) {num++; return num;}
   if (gSystem->Load("libSTEERBase.so") < 0) {num++; return num;}
   if (gSystem->Load("libESD.so") < 0) {num++; return num;}
   if (gSystem->Load("libAOD.so") < 0) {num++; return num;}
   if (gSystem->Load("libANALYSIS.so") < 0) {num++; return num;}
   if (gSystem->Load("libOADB.so") < 0) {num++; return num;}
   if (gSystem->Load("libANALYSISalice.so") < 0) {num++; return num;}
   if (gSystem->Load("libEventMixing.so") < 0) {num++; return num;}
   if (gSystem->Load("libPWG2resonances.so") < 0) {num++; return num;}

   return 0;
}
