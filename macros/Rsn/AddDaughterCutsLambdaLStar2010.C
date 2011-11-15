#ifndef __CINT__
#include <PWG2/RESONANCES/AliRsnInputHandler.h>
#include <PWG2/RESONANCES/AliRsnCutDaughterLStar2010.h>
#include <PWG2/RESONANCES/AliRsnCutSet.h>
#endif
Int_t AddDaughterCutsLambdaLStar2010(AliPID::EParticleType type1,AliPID::EParticleType type2,Bool_t isRsnMini=kFALSE,AliRsnInputHandler *rsnIH=0,AliAnalysisTaskSE *task=0)
{

   if (!rsnIH) return 0;

   // === USER HAS TO SET CORRECT NUMBER OF CUTS SETS =====
   Int_t numberOfCuts = 2;

   // integrated kaon cut
   AliRsnCutDaughterLStar2010 *cutK = new AliRsnCutDaughterLStar2010("cutKaonForLStar", type1);
   // cut set
   AliRsnCutSet *cutSetK = new AliRsnCutSet("KaonForLStar", AliRsnTarget::kDaughter);
   cutSetK->AddCut(cutK);
   cutSetK->SetCutScheme(cutK->GetName());

   // integrated proton cut
   AliRsnCutDaughterLStar2010 *cutP = new AliRsnCutDaughterLStar2010("cutProtonForLStar", type2);
   // cut set
   AliRsnCutSet *cutSetP = new AliRsnCutSet("ProtonForLStar", AliRsnTarget::kDaughter);
   cutSetP->AddCut(cutP);
   cutSetP->SetCutScheme(cutP->GetName());



   if (isRsnMini) {
      AliRsnMiniAnalysisTask *taskRsnMini = dynamic_cast<AliRsnMiniAnalysisTask *>(task);
      if (taskRsnMini) {
         taskRsnMini->AddTrackCuts(cutSetK);
         taskRsnMini->AddTrackCuts(cutSetP);
      }
   } else {
      AliRsnDaughterSelector *sel = rsnIH->GetSelector();
      sel->Add(cutSetK, kTRUE);
      sel->Add(cutSetP, kTRUE);
   }


   return numberOfCuts;
}
