#ifndef __CINT__
#include <ANALYSIS/AliAnalysisTaskSE.h>
#include <PWG2/RESONANCES/AliRsnCutSet.h>
#include <PWG2/RESONANCES/AliRsnInputHandler.h>
#include <PWG2/RESONANCES/AliRsnCutKaonForPhi2010.h>
#include <PWG2/RESONANCES/AliRsnMiniAnalysisTask.h>
#include <PWG2/RESONANCES/AliRsnAnalysisTask.h>
#endif
Int_t AddDaughterCutsPhi2010(AliPID::EParticleType type1,AliPID::EParticleType type2,Bool_t isRsnMini=kFALSE,AliRsnInputHandler *rsnIH=0,AliAnalysisTaskSE *task=0)
{

   if (!rsnIH) return 0;

   // === USER HAS TO SET CORRECT NUMBER OF CUTS SETS =====
   Int_t numberOfCuts = 1;

   //---------------------------------------------
   //  Define single cuts
   //---------------------------------------------

   AliRsnCutKaonForPhi2010 *cut = new AliRsnCutKaonForPhi2010("cutKaonPhi2010");
//    cut.SetMode(AliRsnCutKaonForPhi2010::kQuality);
//    cut.SetMode(AliRsnCutKaonForPhi2010::kOnlyTPC);
//    cut.SetMode(AliRsnCutKaonForPhi2010::kOnlyTOF);

   //---------------------------------------------
   //  Combine cuts
   //---------------------------------------------

   AliRsnCutSet *cuts = new AliRsnCutSet("kaonPhi2010", AliRsnTarget::kDaughter);
   cuts->AddCut(cut);
   cuts->SetCutScheme(cut->GetName());


   if (isRsnMini) {
      AliRsnMiniAnalysisTask *taskRsnMini = dynamic_cast<AliRsnMiniAnalysisTask *>(task);
      if (taskRsnMini) {
         taskRsnMini->AddTrackCuts(cuts);
      }
   } else {
      AliRsnDaughterSelector *sel = rsnIH->GetSelector();
      sel->Add(cuts, kTRUE);
   }
   return numberOfCuts;

}
