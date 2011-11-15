#ifndef __CINT__
#include <PWG2/RESONANCES/AliRsnCutPrimaryVertex.h>
#include <PWG2/RESONANCES/AliRsnValuePair.h>
#include <PWG2/RESONANCES/AliRsnListOutput.h>
#include <PWG2/RESONANCES/AliRsnPairDef.h>
#include <PWG2/RESONANCES/AliRsnLoopPair.h>
#include <PWG2/RESONANCES/AliRsnAnalysisTask.h>
#include <PWG2/RESONANCES/AliRsnLoopDaughter.h>
#include <PWG2/RESONANCES/AliRsnValueDaughter.h>
#include <PWG2/RESONANCES/AliRsnMiniAnalysisTask.h>
#include <PWG2/RESONANCES/AliRsnCutMiniPair.h>
#include <PWG2/RESONANCES/AliRsnInputHandler.h>
#endif

Bool_t RsnConfig(AliAnalysisTaskSE *task,Bool_t isMC,Bool_t isMixing,AliRsnInputHandler *rsnIH=0,TList *listRsn=0) {

   if (!task) return kFALSE;

   if (!listRsn) {
      return kFALSE;
   }

   Int_t cutIndex = 0;
   Int_t numOfCuts = 0;

   // set commol eventCuts
   AliRsnCutSet *commonEventCuts = EventCuts(task);

   AliRsnCutSet *commonPairCuts = PairCuts();


   // TODO this is tmp hack
   if (!rsnIH) rsnIH = new AliRsnInputHandler();

   TIter next(listRsn);
   TNamed *rsnObj=0;
   TString rsnName,rsnCutName;
   while ((rsnObj = (TNamed *)next())) {
      rsnName = rsnObj->GetName();
      rsnName.ToLower();
      rsnCutName = rsnObj->GetTitle();
      gROOT->LoadMacro(Form("AddDaughterCuts%s.C",rsnCutName.Data()));
      // add cuts
      if (!rsnName.CompareTo("phi")) {
         numOfCuts = gROOT->ProcessLine(Form("AddDaughterCuts%s(AliPID::kKaon,AliPID::kKaon,%d,(AliRsnInputHandler*)%p,(AliAnalysisTaskSE*)%p)",rsnCutName.Data(), gRsnUseMiniPackage,rsnIH, task));
         Printf("pointer is %p %p",rsnIH,task);

         if (numOfCuts) {
            AddParticleMonitor(task,isMC,cutIndex,commonEventCuts,commonPairCuts,gRsnUseMCMomentum,Form("%s_%s_K",rsnName.Data(),rsnCutName.Data()));
            AddPair(task,isMC,isMixing,AliPID::kKaon,cutIndex,AliPID::kKaon,cutIndex,333, 1.019455,commonEventCuts,commonPairCuts,gRsnOutputFull,gRsnUseMCMomentum,Form("%s_%s",rsnName.Data(),rsnCutName.Data()));
            cutIndex+=numOfCuts;
         }
      } else if (!rsnName.CompareTo("kstar")) {
         numOfCuts = gROOT->ProcessLine(Form("AddDaughterCuts%s(AliPID::kKaon,AliPID::kPion,%d,(AliRsnInputHandler*)%p,(AliAnalysisTaskSE*)%p)",rsnCutName.Data(),gRsnUseMiniPackage,rsnIH,task));
         if (numOfCuts) {
            AddParticleMonitor(task,isMC,cutIndex,commonEventCuts,commonPairCuts,gRsnUseMCMomentum,Form("%s_%s_K",rsnName.Data(),rsnCutName.Data()));
            AddParticleMonitor(task,isMC,cutIndex+1,commonEventCuts,commonPairCuts,gRsnUseMCMomentum,Form("%s_%s_pi",rsnName.Data(),rsnCutName.Data()));
            AddPair(task,isMC,isMixing,AliPID::kKaon,cutIndex,AliPID::kPion,cutIndex+1,313, 0.896,commonEventCuts,commonPairCuts,gRsnOutputFull,gRsnUseMCMomentum,Form("%s_%s",rsnName.Data(),rsnCutName.Data()));
            cutIndex+=numOfCuts;
         }
      } else if (!rsnName.CompareTo("lambda")) {
         numOfCuts = gROOT->ProcessLine(Form("AddDaughterCuts%s(AliPID::kKaon,AliPID::kProton,%d,(AliRsnInputHandler*)%p,(AliAnalysisTaskSE*)%p)",rsnCutName.Data(),gRsnUseMiniPackage,rsnIH,task));
         if (numOfCuts) {
            AddParticleMonitor(task,isMC,cutIndex,commonEventCuts,commonPairCuts,gRsnUseMCMomentum,Form("%s_%s_K",rsnName.Data(),rsnCutName.Data()));
            AddParticleMonitor(task,isMC,cutIndex+1,commonEventCuts,commonPairCuts,gRsnUseMCMomentum,Form("%s_%s_p",rsnName.Data(),rsnCutName.Data()));
            AddPair(task,isMC,isMixing,AliPID::kKaon,cutIndex,AliPID::kProton,cutIndex+1,3124, 1.520,commonEventCuts,commonPairCuts,gRsnOutputFull,gRsnUseMCMomentum,Form("%s_%s",rsnName.Data(),rsnCutName.Data()));
            cutIndex+=numOfCuts;
         }
      } else {
         numOfCuts = gROOT->ProcessLine(Form("%s(%d,(AliRsnInputHandler*)%p,(AliAnalysisTaskSE*)%p)",rsnCutName.Data(),rsnIH,task));
      }

   }

   return kTRUE;
}

AliRsnCutSet *EventCuts(AliAnalysisTaskSE *task=0,Bool_t isPP=kTRUE)
{

   return 0;
   // primary vertex:
   // - 2nd argument --> |Vz| range
   // - 3rd argument --> minimum required number of contributors
   // - 4th argument --> tells if TPC stand-alone vertexes must be accepted
   // we switch on the check for pileup
   AliRsnCutPrimaryVertex *cutVertex = new AliRsnCutPrimaryVertex("cutVertex", 10.0, 0, kFALSE);
   if (isPP) cutVertex->SetCheckPileUp(kTRUE);

   // primary vertex is always used
   AliRsnCutSet *commonEventCuts = new AliRsnCutSet("commonEventCuts", AliRsnTarget::kEvent);
   commonEventCuts->AddCut(cutVertex);
   commonEventCuts->SetCutScheme(cutVertex->GetName());

   // if task is mini
   AliRsnMiniAnalysisTask *taskRsn = dynamic_cast<AliRsnMiniAnalysisTask *>(task);
   if (taskRsn) taskRsn->SetEventCuts(commonEventCuts);

   return commonEventCuts;
}

AliRsnCutSet *PairCuts() {
//   TODO
//    AliRsnCutMiniPair *cutY = new AliRsnCutMiniPair("cutRapidity", AliRsnCutMiniPair::kRapidityRange);
//    cutY->SetRangeD(-0.5, 0.5);
//
//    AliRsnCutSet *cutsPair = new AliRsnCutSet("pairCuts", AliRsnTarget::kMother);
//    cutsPair->AddCut(cutY);
//    cutsPair->SetCutScheme(cutY->GetName());
//
   AliRsnCutSet *cutsPair = 0;
   return cutsPair;
}

void AddPairOutputPhi(AliRsnLoopPair *pair,Bool_t gRsnOutputFull=kTRUE)
{
   // axes
   AliRsnValuePair *axisIM = new AliRsnValuePair("IM", AliRsnValuePair::kInvMass);
   AliRsnValuePair *axisPt = new AliRsnValuePair("PT", AliRsnValuePair::kPt);
   axisIM     ->SetBins(400, 0.9, 1.4);
   axisPt     ->SetBins(120, 0.0, 12.0);

   // output: 2D histogram of inv. mass vs. pt
   AliRsnListOutput *outPair = 0;
   if (!gRsnOutputFull) {
      outPair = new AliRsnListOutput("pair", AliRsnListOutput::kHistoDefault);
      outPair->AddValue(axisIM);
   } else {
      outPair = new AliRsnListOutput("pair", AliRsnListOutput::kHistoSparse);
      outPair->AddValue(axisIM);
      outPair->AddValue(axisPt);
   }
   // add outputs to loop
   pair->AddOutput(outPair);
}

void AddPairOutputKStar(AliRsnLoopPair *pair,Bool_t gRsnOutputFull=kTRUE)
{
   // axes
   AliRsnValuePair *axisIM = new AliRsnValuePair("IM", AliRsnValuePair::kInvMass);
   AliRsnValuePair *axisPt = new AliRsnValuePair("PT", AliRsnValuePair::kPt);
   axisIM     ->SetBins(900, 0.6, 1.5);
   axisPt     ->SetBins(120, 0.0, 12.0);

   // output: 2D histogram of inv. mass vs. pt
   AliRsnListOutput *outPair = 0;
   if (!gRsnOutputFull) {
      outPair = new AliRsnListOutput("pair", AliRsnListOutput::kHistoDefault);
      outPair->AddValue(axisIM);
   } else {
      outPair = new AliRsnListOutput("pair", AliRsnListOutput::kHistoSparse);
      outPair->AddValue(axisIM);
      outPair->AddValue(axisPt);
   }
   // add outputs to loop
   pair->AddOutput(outPair);
}

void AddPairOutputLambda(AliRsnLoopPair *pair,Bool_t gRsnOutputFull=kTRUE)
{
   // axes
   AliRsnValuePair *axisIM = new AliRsnValuePair("IM", AliRsnValuePair::kInvMass);
   AliRsnValuePair *axisPt = new AliRsnValuePair("PT", AliRsnValuePair::kPt);
   axisIM     ->SetBins(400, 1.4, 1.8);
   axisPt     ->SetBins(50, 0.0, 5.0);

   // output: 2D histogram of inv. mass vs. pt
   AliRsnListOutput *outPair = 0;
   if (!gRsnOutputFull) {
      outPair = new AliRsnListOutput("pair", AliRsnListOutput::kHistoDefault);
      outPair->AddValue(axisIM);
   } else {
      outPair = new AliRsnListOutput("pair", AliRsnListOutput::kHistoSparse);
      outPair->AddValue(axisIM);
      outPair->AddValue(axisPt);
   }
   // add outputs to loop
   pair->AddOutput(outPair);
}

Bool_t AddPair(AliAnalysisTaskSE *task, Bool_t isMC,Bool_t isMixing, AliPID::EParticleType pType1,Int_t listID1, AliPID::EParticleType pType2,Int_t listID2, Int_t pdgMother,Double_t massMother, AliRsnCutSet *commonEventCuts=0,AliRsnCutSet *cutPair=0,Bool_t gRsnOutputFull=kTRUE,Bool_t gRsnUseMCMomentum=kFALSE, TString name = "") {

   // For mini there is special config
   if (gRsnUseMiniPackage)
      return AddPairMini(task,isMC,isMixing,pType1,listID1,pType2,listID2,pdgMother,massMother,commonEventCuts,cutPair,gRsnOutputFull,gRsnUseMCMomentum,name);

   Bool_t typeSame = (pType1 == pType2);

   TList *listLoops = new TList;

   // pair definition
   AliRsnPairDef  *pairDefPM         = new AliRsnPairDef((AliRsnDaughter::ESpecies)pType1, '+', (AliRsnDaughter::ESpecies)pType2, '-', pdgMother, massMother);
   AliRsnPairDef  *pairDefMP         = new AliRsnPairDef((AliRsnDaughter::ESpecies)pType1, '-', (AliRsnDaughter::ESpecies)pType2, '+', pdgMother, massMother);
   AliRsnPairDef  *pairDefPP         = new AliRsnPairDef((AliRsnDaughter::ESpecies)pType1, '+', (AliRsnDaughter::ESpecies)pType2, '+', pdgMother, massMother);
   AliRsnPairDef  *pairDefMM         = new AliRsnPairDef((AliRsnDaughter::ESpecies)pType1, '-', (AliRsnDaughter::ESpecies)pType2, '-', pdgMother, massMother);

   // loop object creation
   AliRsnLoopPair *lp = 0;

   // sets +-
   lp = new AliRsnLoopPair(Form("%s_PM", name.Data()), pairDefPM, kFALSE);
   listLoops->Add(lp);

   if (!typeSame) {
      lp = new AliRsnLoopPair(Form("%s_MP", name.Data()), pairDefMP, kFALSE);
      listLoops->Add(lp);
   }

   // sets +- TRUE pairs
   if (isMC) {
      lp = new AliRsnLoopPair(Form("%s_PM_TRUE", name.Data()), pairDefPM, kFALSE);
      lp->SetOnlyTrue(kTRUE);
      lp->SetCheckDecay(kTRUE);
      listLoops->Add(lp);
      if (!typeSame) {
         lp = new AliRsnLoopPair(Form("%s_MP_TRUE", name.Data()), pairDefMP, kFALSE);
         lp->SetOnlyTrue(kTRUE);
         lp->SetCheckDecay(kTRUE);
         listLoops->Add(lp);
      }
      // sets +- TRUE paris (MC is used for momentum)
      lp = new AliRsnLoopPair(Form("%s_PM_TRUE_MC", name.Data()), pairDefPM, kFALSE);
      lp->SetTrueMC(kTRUE);
      listLoops->Add(lp);
      if (!typeSame) {
         // sets +- TRUE paris (MC is used for momentum)
         lp = new AliRsnLoopPair(Form("%s_MP_TRUE_MC", name.Data()), pairDefMP, kFALSE);
         lp->SetTrueMC(kTRUE);
         listLoops->Add(lp);
      }
   }

   // sets ++
   lp = new AliRsnLoopPair(Form("%s_PP", name.Data()), pairDefPP, kFALSE);
   listLoops->Add(lp);

   // sets --
   lp = new AliRsnLoopPair(Form("%s_MM", name.Data()), pairDefMM, kFALSE);
   listLoops->Add(lp);

   if (isMixing) {
      // sets +- Mixing (NOT mini)
      lp = new AliRsnLoopPair(Form("%s_PM", name.Data()), pairDefPM, kTRUE);
      listLoops->Add(lp);

      // sets -+ Mixing (NOT mini)
      lp = new AliRsnLoopPair(Form("%s_MP", name.Data()), pairDefMP, kTRUE);
      listLoops->Add(lp);

      // sets ++ Mixing (NOT mini)
      lp = new AliRsnLoopPair(Form("%s_PP", name.Data()), pairDefPP, kTRUE);
      listLoops->Add(lp);

      // sets -- Mixing (NOT mini)
      lp = new AliRsnLoopPair(Form("%s_MM", name.Data()), pairDefMM, kTRUE);
      listLoops->Add(lp);
   }


   // loops over all AliRsnLoops and sets everything (don't touch it if you don't know what you are doing)
   TIter next(listLoops);
   while ((lp = (AliRsnLoopPair *)next.Next()))) {
      lp->SetListID(0, listID1);
      lp->SetListID(1, listID2);
      lp->SetMCRefInfo(gRsnUseMCMomentum);
//       if (cutPair) lp->SetPairCuts(cutPair);
      if (commonEventCuts) lp->SetEventCuts(commonEventCuts);
      if (name.Contains("phi")) AddPairOutputPhi(lp,gRsnOutputFull);
      else if (name.Contains("kstar")) AddPairOutputKStar(lp,gRsnOutputFull);
      else if (name.Contains("lambda"))AddPairOutputLambda(lp,gRsnOutputFull);
      else continue;
      ((AliRsnAnalysisTask *)task)->AddLoop(lp);
   }
   return kTRUE;
}

Bool_t AddPairMini(AliAnalysisTaskSE *task, Bool_t isMC,Bool_t isMixing, AliPID::EParticleType pType1,Int_t listID1, AliPID::EParticleType pType2,Int_t listID2, Int_t pdgMother,Double_t massMother, AliRsnCutSet *commonEventCuts=0,AliRsnCutSet *cutPair=0,Bool_t gRsnOutputFull=kTRUE,Bool_t gRsnUseMCMomentum=kFALSE, TString name = "") {

   // TODO handle pp from outside
   Bool_t isPP = kTRUE;

   AliRsnMiniAnalysisTask *taskRsnMini =  dynamic_cast<AliRsnMiniAnalysisTask *>(task);
//   Int_t multID = taskRsnMini->CreateValue(AliRsnMiniValue::kMult, kFALSE);
//
//    // create event-related output
//    AliRsnMiniOutput *outMult = taskRsnMini->CreateOutput("eventMult", "HIST", "EVENT");
//    // set axes, by passing value ID and defining the binning
//    if (isPP)
//       outMult->AddAxis(multID, 300, 0.0, 300.0);
//    else
//       outMult->AddAxis(multID, 100, 0.0, 100.0);


//    AliRsnCutMiniPair *cutY = new AliRsnCutMiniPair("cutRapidity", AliRsnCutMiniPair::kRapidityRange);
//    cutY->SetRangeD(-0.5, 0.5);
//
//    AliRsnCutSet *cutsPair = new AliRsnCutSet("pairCuts", AliRsnTarget::kMother);
//    cutsPair->AddCut(cutY);
//    cutsPair->SetCutScheme(cutY->GetName());


   if (name.Contains("phi")) AddPairOutputMiniPhi(taskRsnMini,isMC,isMixing,pType1,listID1,pType2,listID2,pdgMother,massMother,PairCuts(),gRsnOutputFull,gRsnUseMCMomentum,name);
   else if (name.Contains("kstar")) AddPairOutputMiniKStar(taskRsnMini,isMC,isMixing,pType1,listID1,pType2,listID2,pdgMother,massMother,PairCuts(),gRsnOutputFull,gRsnUseMCMomentum,name);
   else if (name.Contains("lambda"))AddPairOutputMiniLambda(taskRsnMini,isMC,isMixing,pType1,listID1,pType2,listID2,pdgMother,massMother,PairCuts(),gRsnOutputFull,gRsnUseMCMomentum,name);
   else continue;





   return kTRUE;

}
void AddPairOutputMiniPhi(AliRsnMiniAnalysisTask *taskRsnMini, Bool_t isMC,Bool_t isMixing, AliPID::EParticleType pType1,Int_t listID1, AliPID::EParticleType pType2,Int_t listID2, Int_t pdgMother,Double_t massMother, AliRsnCutSet *cutsPair=0,Bool_t gRsnOutputFull=kTRUE,Bool_t gRsnUseMCMomentum=kFALSE, TString suffix = "") {


   /* invariant mass   */ Int_t imID   = taskRsnMini->CreateValue(AliRsnMiniValue::kInvMass, kFALSE);
   /* IM resolution    */ Int_t resID  = taskRsnMini->CreateValue(AliRsnMiniValue::kInvMassDiff, kTRUE);
   /* transv. momentum */ Int_t ptID   = taskRsnMini->CreateValue(AliRsnMiniValue::kPt, kFALSE);
   /* centrality       */ Int_t centID = taskRsnMini->CreateValue(AliRsnMiniValue::kMult, kFALSE);


   // use an array for more compact writing, which are different on mixing and charges
   // [0] = unlike
   // [1] = mixing
   // [2] = like ++
   // [3] = like --
   Bool_t  use     [5] = { 1      ,  1      ,  1      ,  1      ,  isMC  };
   TString name    [5] = {"Unlike", "Mixing", "LikePP", "LikeMM", "Trues"};
   TString comp    [5] = {"PAIR"  , "MIX"   , "PAIR"  , "PAIR"  , "TRUE" };
   Char_t  charge1 [5] = {'+'     , '+'     , '+'     , '-'     , '+'    };
   Char_t  charge2 [5] = {'-'     , '-'     , '+'     , '-'     , '-'    };

   // common definitions
   TString outputType = "HIST";
   if (gRsnOutputFull) outputType = "SPARSE";

   Int_t nIM   = 300; Double_t minIM   = 0.9, maxIM =  1.2;
   Int_t nPt   = 100; Double_t minPt   = 0.0, maxPt = 10.0;
   Int_t nCent = 100; Double_t minCent = 0.0, maxCent = 100.0;
   Int_t nRes  = 200; Double_t maxRes  = 0.01;

   // retrieve mass from PDG database
   Int_t         pdg  = 333;
   TDatabasePDG *db   = TDatabasePDG::Instance();
   TParticlePDG *part = db->GetParticle(pdg);

   // create standard outputs
   for (Int_t i = 0; i < 5; i++) {
      if (!use[i]) continue;
      // create output
      AliRsnMiniOutput *out = taskRsnMini->CreateOutput(Form("phi_%s%s", name[i].Data(), suffix.Data()), outputType.Data(), comp[i].Data());
      // selection settings
      out->SetCutID(0, listID1);
      out->SetCutID(1, listID1);
      out->SetDaughter(0, AliRsnDaughter::kKaon);
      out->SetDaughter(1, AliRsnDaughter::kKaon);
      out->SetCharge(0, charge1[i]);
      out->SetCharge(1, charge2[i]);
      out->SetMotherPDG(pdg);
      out->SetMotherMass(part->Mass());
      // pair cuts
      if (cutsPair) out->SetPairCuts(cutsPair);
      // axis X: invmass
      out->AddAxis(imID, nIM, minIM, maxIM);

      if (gRsnOutputFull) {
         // axis Y: transverse momentum
         out->AddAxis(ptID, nPt, minPt, maxPt);
         // axis Z: centrality
         out->AddAxis(centID, nCent, minCent, maxCent);
      }
   }

   // add output for resolution
   if (isMC) {
      AliRsnMiniOutput *outRes = taskRsnMini->CreateOutput(Form("phi_Res%s", suffix.Data()), outputType.Data(), "TRUE");
      // selection settings
      outRes->SetCutID(0, listID1);
      outRes->SetCutID(1, listID1);
      outRes->SetDaughter(0, AliRsnDaughter::kKaon);
      outRes->SetDaughter(1, AliRsnDaughter::kKaon);
      outRes->SetCharge(0, '+');
      outRes->SetCharge(1, '-');
      outRes->SetMotherPDG(pdg);
      outRes->SetMotherMass(part->Mass());
      // pair cuts
      if (cutsPair) outRes->SetPairCuts(cutsPair);
      // axis X: resolution
      outRes->AddAxis(resID, nRes, -maxRes, maxRes);

      if (gRsnOutputFull) {
         // axis Y: transverse momentum
         outRes->AddAxis(ptID, nPt, minPt, maxPt);
         // axis Z: centrality
         outRes->AddAxis(centID, nCent, minCent, maxCent);
      }
   }

   //
   // -- Create output for MC generated ------------------------------------------------------------
   //

   if (isMC) {
      // create ouput
      AliRsnMiniOutput *outMC = taskRsnMini->CreateOutput(Form("phi_MCGen%s", suffix.Data()), outputType.Data(), "MOTHER");
      // selection settings
      outMC->SetDaughter(0, AliRsnDaughter::kKaon);
      outMC->SetDaughter(1, AliRsnDaughter::kKaon);
      outMC->SetMotherPDG(pdg);
      outMC->SetMotherMass(part->Mass());
      // pair cuts
      if (cutsPair) outMC->SetPairCuts(cutsPair);
      // axis X: invmass
      outMC->AddAxis(imID, nIM, minIM, maxIM);
      if (gRsnOutputFull) {
         // axis Y: transverse momentum
         outMC->AddAxis(ptID, nPt, minPt, maxPt);
         // axis Z: centrality
         outMC->AddAxis(centID, nCent, minCent, maxCent);
      }
   }


}

void AddPairOutputMiniKStar(AliRsnMiniAnalysisTask *taskRsnMini, Bool_t isMC,Bool_t isMixing, AliPID::EParticleType pType1,Int_t listID1, AliPID::EParticleType pType2,Int_t listID2, Int_t pdgMother,Double_t massMother, AliRsnCutSet *cutsPair=0,Bool_t gRsnOutputFull=kTRUE,Bool_t gRsnUseMCMomentum=kFALSE, TString suffix = "") {

   /* invariant mass   */ Int_t imID   = taskRsnMini->CreateValue(AliRsnMiniValue::kInvMass, kFALSE);
   /* IM resolution    */ Int_t resID  = taskRsnMini->CreateValue(AliRsnMiniValue::kInvMassRes, kTRUE);
   /* transv. momentum */ Int_t ptID   = taskRsnMini->CreateValue(AliRsnMiniValue::kPt, kFALSE);
   /* centrality       */ Int_t centID = taskRsnMini->CreateValue(AliRsnMiniValue::kMult, kFALSE);

   //
   // -- Create all needed outputs -----------------------------------------------------------------
   //

   Int_t iCutK = listID1;
   Int_t iCutPi = listID2;

   // use an array for more compact writing, which are different on mixing and charges
   // [0] = unlike
   // [1] = mixing
   // [2] = like ++
   // [3] = like --
   Bool_t  use     [10] = { 1       ,  1       ,  1       ,  1       ,  1      ,  1      ,  isMC   ,   isMC   ,  isMC   ,   isMC   };
   Bool_t  useIM   [10] = { 1       ,  1       ,  1       ,  1       ,  1      ,  1      ,  1      ,   1      ,  0      ,   0      };
   TString name    [10] = {"Unlike1", "Unlike2", "Mixing1", "Mixing2", "LikePP", "LikeMM", "Trues1",  "Trues2", "Res1"  ,  "Res2"  };
   TString comp    [10] = {"PAIR"   , "PAIR"   , "MIX"    , "MIX"    , "PAIR"  , "PAIR"  , "TRUE"  ,  "TRUE"  , "TRUE"  ,  "TRUE"  };
   TString output  [10] = {"HIST"   , "HIST"   , "HIST"   , "HIST"   , "HIST"  , "HIST"  , "HIST"  ,  "HIST"  , "HIST"  ,  "HIST"  };
   Char_t  charge1 [10] = {'+'      , '-'      , '+'      , '-'      , '+'     , '-'     , '+'     ,  '-'     , '+'     ,  '-'     };
   Char_t  charge2 [10] = {'-'      , '+'      , '-'      , '+'      , '+'     , '-'     , '-'     ,  '+'     , '-'     ,  '+'     };
   Int_t   cutID1  [10] = { iCutK   ,  iCutK   ,  iCutK   ,  iCutK   ,  iCutK  ,  iCutK  ,  iCutK  ,   iCutK  ,  iCutK  ,   iCutK  };
   Int_t   cutID2  [10] = { iCutPi  ,  iCutPi  ,  iCutPi  ,  iCutPi  ,  iCutPi ,  iCutPi ,  iCutPi ,   iCutPi ,  iCutPi ,   iCutPi };

   for (Int_t i = 0; i < 10; i++) {
      if (!use[i]) continue;
      // create output
      AliRsnMiniOutput *out = taskRsnMini->CreateOutput(Form("kstar_%s%s", name[i].Data(), suffix.Data()), output[i].Data(), comp[i].Data());
      // selection settings
      out->SetCutID(0, cutID1[i]);
      out->SetCutID(1, cutID2[i]);
      out->SetDaughter(0, AliRsnDaughter::kKaon);
      out->SetDaughter(1, AliRsnDaughter::kPion);
      out->SetCharge(0, charge1[i]);
      out->SetCharge(1, charge2[i]);
      out->SetMotherPDG(313);
      out->SetMotherMass(0.896);
      // pair cuts
      if (cutsPair) out->SetPairCuts(cutsPair);
      // axis X: invmass (or resolution)
      if (useIM[i])
         out->AddAxis(imID, 90, 0.6, 1.5);
      else
         out->AddAxis(resID, 200, -0.02, 0.02);

      if (gRsnOutputFull) {
         // axis Y: transverse momentum
         out->AddAxis(ptID, 100, 0.0, 10.0);
         // axis Z: centrality
         out->AddAxis(centID, 100, 0.0, 100.0);
      }
   }
}

void AddPairOutputMiniLambda(AliRsnMiniAnalysisTask *taskRsnMini, Bool_t isMC,Bool_t isMixing, AliPID::EParticleType pType1,Int_t listID1, AliPID::EParticleType pType2,Int_t listID2, Int_t pdgMother,Double_t massMother, AliRsnCutSet *cutPair=0,Bool_t gRsnOutputFull=kTRUE,Bool_t gRsnUseMCMomentum=kFALSE, TString suffix = "") {

   /* invariant mass   */ Int_t imID   = taskRsnMini->CreateValue(AliRsnMiniValue::kInvMass, kFALSE);
   /* IM resolution    */ Int_t resID  = taskRsnMini->CreateValue(AliRsnMiniValue::kInvMassRes, kTRUE);
   /* transv. momentum */ Int_t ptID   = taskRsnMini->CreateValue(AliRsnMiniValue::kPt, kFALSE);
   /* centrality       */ Int_t centID = taskRsnMini->CreateValue(AliRsnMiniValue::kMult, kFALSE);

   Bool_t  use     [6] = { 1      ,  1      ,  1      ,  1      ,  isMC   ,  isMC     };
   Bool_t  useIM   [6] = { 1      ,  1      ,  1      ,  1      ,  1      ,  0        };
   TString name    [6] = {"Unlike", "Mixing", "LikePP", "LikeMM", "Trues" , "Res"     };
   TString comp    [6] = {"PAIR"  , "MIX"   , "PAIR"  , "PAIR"  , "TRUE"  , "TRUE"    };
   TString output  [6] = {"HIST"  , "HIST"  , "HIST"  , "HIST"  , "HIST"  , "HIST"    };
   Char_t  charge1 [6] = {'+'     , '+'     , '+'     , '-'     , '+'     , '+'       };
   Char_t  charge2 [6] = {'-'     , '-'     , '+'     , '-'     , '-'     , '-'       };
   Int_t   cutID   [6] = { listID1   ,  listID1   ,  listID1   ,  listID1   ,  listID1   ,  listID1     };

   for (Int_t i = 0; i < 6; i++) {
      if (!use[i]) continue;
      // create output
      AliRsnMiniOutput *out = taskRsnMini->CreateOutput(Form("phi_%s%s", name[i].Data(), suffix.Data()), output[i].Data(), comp[i].Data());
      // selection settings
      out->SetCutID(0, cutID[i]);
      out->SetCutID(1, cutID[i]);
      out->SetDaughter(0, AliRsnDaughter::kKaon);
      out->SetDaughter(1, AliRsnDaughter::kKaon);
      out->SetCharge(0, charge1[i]);
      out->SetCharge(1, charge2[i]);
      out->SetMotherPDG(pdgMother);
      out->SetMotherMass(massMother);
      // pair cuts
      out->SetPairCuts(cutPair);
      // axis X: invmass (or resolution)
      if (useIM[i])
         out->AddAxis(imID, 500, 0.9,  1.4);
      else
         out->AddAxis(resID, 200, -0.02, 0.02);
      // axis Y: transverse momentum
//       out->AddAxis(ptID, 100, 0.0, 10.0);
   }
}


void AddMonitorOutput(AliRsnLoopDaughter *mon, Bool_t gRsnUseMCMomentum=kFALSE)
{
// mcinfo is not supported yet
   if (gRsnUseMCMomentum) return ;

   // dEdx tpc
   AliRsnValueDaughter *axisMomTPC = new AliRsnValueDaughter("pTPC", AliRsnValueDaughter::kPtpc);
   AliRsnValueDaughter *axisSigTPC = new AliRsnValueDaughter("sTPC", AliRsnValueDaughter::kTPCsignal);
   axisMomTPC->SetBins(0.0,5.0,0.01);
   axisSigTPC->SetBins(0.0,500.0,2.0);

   // output: 2D histogram of TPC signal vs. TPC momentum
   AliRsnListOutput *outMonitordEdxTPC = new AliRsnListOutput("dEdx", AliRsnListOutput::kHistoDefault);
   outMonitordEdxTPC->AddValue(axisMomTPC);
   outMonitordEdxTPC->AddValue(axisSigTPC);

   // add outputs to loop
   mon->AddOutput(outMonitordEdxTPC);

   // dEdx tpc
   AliRsnValueDaughter *axisMomTPCForTOF = new AliRsnValueDaughter("pTPC", AliRsnValueDaughter::kPtpc);
   AliRsnValueDaughter *axisSigTOF = new AliRsnValueDaughter("sTOF", AliRsnValueDaughter::kTOFsignal);
   axisMomTPCForTOF->SetBins(0.0,5.0,0.01);
   axisSigTOF->SetBins(0.0,500.0,2.0);

   // output: 2D histogram of TPC signal vs. TPC momentum
   AliRsnListOutput *outMonitordEdxTOF = new AliRsnListOutput("Edx", AliRsnListOutput::kHistoDefault);
   outMonitordEdxTOF->AddValue(axisMomTPCForTOF);
   outMonitordEdxTOF->AddValue(axisSigTOF);

   // add outputs to loop
//    mon->AddOutput(outMonitordEdxTOF);


   // Momentum
   AliRsnValueDaughter *axisMomP = new AliRsnValueDaughter("p", AliRsnValueDaughter::kP);
   axisMomP->SetBins(0.0,5.0,0.01);

   // output: 2D histogram of TPC signal vs. TPC momentum
   AliRsnListOutput *outMonitorP = new AliRsnListOutput("P", AliRsnListOutput::kHistoDefault);
   outMonitorP->AddValue(axisMomP);

   // add outputs to loop
   mon->AddOutput(outMonitorP);


   // Momentum Pt
   AliRsnValueDaughter *axisMomPt = new AliRsnValueDaughter("pt", AliRsnValueDaughter::kPt);
   axisMomPt->SetBins(0.0,5.0,0.01);

   // output: 2D histogram of TPC signal vs. TPC momentum
   AliRsnListOutput *outMonitorPt = new AliRsnListOutput("Pt", AliRsnListOutput::kHistoDefault);
   outMonitorPt->AddValue(axisMomPt);

   // add outputs to loop
   mon->AddOutput(outMonitorPt);

   // Eta
   AliRsnValueDaughter *axisMomEta = new AliRsnValueDaughter("eta", AliRsnValueDaughter::kEta);
   axisMomEta->SetBins(-1.0,1.0,0.01);

   // output: 2D histogram of TPC signal vs. TPC momentum
   AliRsnListOutput *outMonitorEta = new AliRsnListOutput("Eta", AliRsnListOutput::kHistoDefault);
   outMonitorEta->AddValue(axisMomEta);

   // add outputs to loop
   mon->AddOutput(outMonitorEta);

//    mon->SetMCRefInfo(gRsnUseMCMomentum);

}


void AddParticleMonitor(AliAnalysisTaskSE *task, Bool_t isMC, Int_t listID1,AliRsnCutSet *commonEventCuts=0,AliRsnCutSet *cutPair=0,Bool_t gRsnUseMCMomentum=kFALSE,TString name = "")
{

   // tmp supported for non-mini package
   if (gRsnUseMiniPackage) return;

   if (!gRsnUseMonitor) return;

   TList *listLoops = new TList;
   // monitor definition
   AliRsnDaughterDef *tracksAll = new AliRsnDaughterDef(AliRsnDaughter::kTrack /*'+' or '-'*/);
   AliRsnDaughterDef *tracksPos = new AliRsnDaughterDef(AliRsnDaughter::kTrack,'+');
   AliRsnDaughterDef *tracksNeg = new AliRsnDaughterDef(AliRsnDaughter::kTrack,'-');

   AliRsnLoopDaughter *lm =0;
   // loop object
   listLoops->Add(new AliRsnLoopDaughter(Form("%s_monAll", name.Data()), listID1, tracksAll));
   listLoops->Add(new AliRsnLoopDaughter(Form("%s_monPos", name.Data()), listID1, tracksPos));
   listLoops->Add(new AliRsnLoopDaughter(Form("%s_monNeg", name.Data()), listID1, tracksNeg));

   TIter next(listLoops);
   while ((lm = (AliRsnLoopDaughter *)next.Next()))) {
      if (commonEventCuts) lm->SetEventCuts(commonEventCuts);
      AddMonitorOutput(lm,gRsnUseMCMomentum);
      ((AliRsnAnalysisTask *)task)->AddLoop(lm);
   }

}


