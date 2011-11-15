#ifndef __CINT__
#include <AliAnalysisManager.h>
#include <AliMultiInputEventHandler.h>
#include <ANALYSIS/EventMixing/AliMixEventPool.h>
#include <ANALYSIS/EventMixing/AliMixEventCutObj.h>
#include <PWG2/RESONANCES/AliRsnAnalysisTask.h>
#include <PWG2/RESONANCES/AliRsnMiniAnalysisTask.h>
#endif

void AddMixingHandler ( AliMultiInputEventHandler* multiInputHandler,AliAnalysisTaskSE *task, TString format = "esd", Bool_t useMC = kFALSE,Bool_t isRsnMini=kFALSE,const Int_t mixNum = 10, TString opts = "" ) {

  if (!isRsnMini) {

    if ( !multiInputHandler ) return;

    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    const Int_t bufferSize = 1;
    AliMixInputEventHandler *mixHandler = new AliMixInputEventHandler ( bufferSize, mixNum );
    mixHandler->SetInputHandlerForMixing ( dynamic_cast<AliMultiInputEventHandler*> ( mgr->GetInputEventHandler() ) );
    AliMixEventPool *evPool = new AliMixEventPool();

//     AliMixEventCutObj *multi = new AliMixEventCutObj ( AliMixEventCutObj::kMultiplicity, 2, 102, 10 );
//     AliMixEventCutObj *zvertex = new AliMixEventCutObj ( AliMixEventCutObj::kZVertex, -10, 10, 1 );

//    AliMixEventCutObj *centrality = new AliMixEventCutObj(AliMixEventCutObj::kCentrality, 0, 20, 10, "V0M");
//    AliMixEventCutObj *multi = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 2, 102, 10);
//    AliMixEventCutObj *zvertex = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -5, 5, 1);
//
//    AliMixEventCutObj *multi = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 2, 5002, 100);
//    AliMixEventCutObj *zvertex = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -10, 10, 1);
// 
    AliMixEventCutObj *centrality = new AliMixEventCutObj ( AliMixEventCutObj::kCentrality, 0, 100, 10, "V0M" );
    AliMixEventCutObj *zvertex = new AliMixEventCutObj ( AliMixEventCutObj::kZVertex, -10, 10, 1 );

   evPool->AddCut(centrality);
//    evPool->AddCut(multi);
    evPool->AddCut ( zvertex );

    // adds event pool (comment it and u will have default mixing)
    mixHandler->SetEventPool ( evPool );

//    mixHandler->SelectCollisionCandidates(AliVEvent::kAny);

//    mixHandler->DoMixIfNotEnoughEvents(kFALSE);

    multiInputHandler->AddInputEventHandler ( mixHandler );

    // adds mixing info task
    gROOT->LoadMacro ( "AddAnalysisTaskMixInfo.C" );
    AddAnalysisTaskMixInfo (opts );
    
  } else if (dynamic_cast<AliRsnMiniAnalysisTask*>(task)) {
   
   AliRsnMiniAnalysisTask *taskRsn = (AliRsnMiniAnalysisTask*) task;
   Bool_t isPP = kTRUE; 
   
   // settings
   if (isPP) 
      taskRsn->UseMultiplicity("QUALITY");
   else
      taskRsn->UseCentrality("V0M");
   
   // set mixing
   taskRsn->UseContinuousMix();
   //task->UseBinnedMix();
   taskRsn->SetNMix(mixNum);
   taskRsn->SetMaxDiffVz(1.0);
   taskRsn->SetMaxDiffMult(10.0);
   taskRsn->SetMaxDiffAngle(1E20);
    
  } else {
    Printf("Error: No Mixing was added (task is null or not AliRsnAnalysisTask or AliRsnMiniAnalysisTask) !!!");
  }
}
