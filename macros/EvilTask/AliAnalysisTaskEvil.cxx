#include <TChain.h>
#include <TList.h>

#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TCanvas.h>
#include <TSystem.h>

#include "AliLog.h"
#include "AliVEvent.h"
#include "AliVParticle.h"

#include "AliESDEvent.h"
#include "AliESDtrack.h"

#include "AliAODEvent.h"
#include "AliAODTrack.h"

#include "AliMCEvent.h"
#include "AliMCParticle.h"
#include "AliAnalysisManager.h"

#include "AliAnalysisTaskEvil.h"

ClassImp(AliAnalysisTaskEvil)

//________________________________________________________________________
AliAnalysisTaskEvil::AliAnalysisTaskEvil(const char *name)
   : AliAnalysisTaskSE(name),
     fHistBin(10),
     fHistMin(0),
     fHistMax(10),
     fNum1D(1),
     fNum2D(0),
     fNum3D(0),
     fNumMemoryLeak(0),
     fNumMemoryLeakOnSingleNode(0),
     fUseMC(kFALSE),
     fNoOutput(kFALSE),
     fNoOutputTotal(kFALSE),
     fMyESDEvent(0),
     fMyAODEvent(0),
     fOutputList(0),
     fHistPtMC(0),
     fBadHost(),
     fNumLoopsInOneEvent(1),
     fAODTestFilterBit(0)

{
   // Constructor
   for (Int_t i = 0; i < 10000; i++) {
      fHistPt[i] = 0;
   }
   for (Int_t i = 0; i < 1000; i++) {
      fHistPt2D[i] = 0;
   }
   for (Int_t i = 0; i < 100; i++) {
      fHistPt3D[i] = 0;
   }
   DefineOutput(1, TList::Class());
}

//________________________________________________________________________
AliAnalysisTaskEvil::~AliAnalysisTaskEvil() {
   // Destructor
   if (fOutputList && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fOutputList;
}

//________________________________________________________________________
void AliAnalysisTaskEvil::UserCreateOutputObjects() {
   // Create histograms
   // Called once
   PrintProcInfo();
   fOutputList = new TList();
   fOutputList->SetOwner();
   PrintProcInfo();
   if (!fNoOutput) {
      for (Int_t i = 0; i < fNum1D; i++) {
         fHistPt[i] = new TH1D(Form("fHistPt%d", i), "P_{T} distribution REC", fHistBin, fHistMin, fHistMax);
         fHistPt[i]->GetXaxis()->SetTitle("P_{T} (GeV/c)");
         fHistPt[i]->GetYaxis()->SetTitle("dN/dP_{T} (c/GeV)");
         fHistPt[i]->SetMarkerStyle(kFullCircle);
         fOutputList->Add(fHistPt[i]);
         PrintProcInfo();
      }
      for (Int_t i = 0; i < fNum2D; i++) {
         fHistPt2D[i] = new TH2D(Form("fHistPt2D%d", i), "P_{T} distribution REC", fHistBin, fHistMin, fHistMax, fHistBin, fHistMin, fHistMax);
         fHistPt2D[i]->GetXaxis()->SetTitle("P_{T} (GeV/c)");
         fHistPt2D[i]->GetYaxis()->SetTitle("dN/dP_{T} (c/GeV)");
         fHistPt2D[i]->SetMarkerStyle(kFullCircle);
         fOutputList->Add(fHistPt2D[i]);
         PrintProcInfo();
      }
      for (Int_t i = 0; i < fNum3D; i++) {
         fHistPt3D[i] = new TH3D(Form("fHistPt3D%d", i), "P_{T} distribution REC", fHistBin, fHistMin, fHistMax, fHistBin, fHistMin, fHistMax, fHistBin, fHistMin, fHistMax);
         fHistPt3D[i]->GetXaxis()->SetTitle("P_{T} (GeV/c)");
         fHistPt3D[i]->GetYaxis()->SetTitle("dN/dP_{T} (c/GeV)");
         fHistPt3D[i]->SetMarkerStyle(kFullCircle);
         fOutputList->Add(fHistPt3D[i]);
         PrintProcInfo();
      }
      if (fUseMC) {
         fHistPtMC = new TH1D("fHistPtMC", "P_{T} distribution MC", fHistBin, fHistMin, fHistMax);
         fHistPtMC->GetXaxis()->SetTitle("P_{T} (GeV/c)");
         fHistPtMC->GetYaxis()->SetTitle("dN/dP_{T} (c/GeV)");
         fHistPtMC->SetMarkerStyle(kFullCircle);
         fOutputList->Add(fHistPtMC);
         PrintProcInfo();
      }
   } else {
      if (!fNoOutputTotal) {
         fHistPt[0] = new TH1D(Form("fHistPt%d", 0), "P_{T} distribution REC", fHistBin, fHistMin, fHistMax);
         fHistPt[0]->GetXaxis()->SetTitle("P_{T} (GeV/c)");
         fHistPt[0]->GetYaxis()->SetTitle("dN/dP_{T} (c/GeV)");
         fHistPt[0]->SetMarkerStyle(kFullCircle);
         fOutputList->Add(fHistPt[0]);
         PrintProcInfo();
      }
   }
   PostData(1, fOutputList);
}

//________________________________________________________________________
void AliAnalysisTaskEvil::UserExec(Option_t *) {
   if (!fNumMemoryLeakOnSingleNode) {
      if (! fBadHost.CompareTo(gSystem->HostName())) {
         TList *l = new TList();
         delete l;
         delete l;
      }
   }
   // Main loop
   // Called for each event
//    AliInfo("<-");



//    PrintProcInfo();
   AliVEvent *event = InputEvent();
   if (!event) {
      Error("UserExec", "Could not retrieve event");
      return;
   }
   fMyESDEvent = dynamic_cast<AliESDEvent *>(InputEvent());

//    AliCentrality *c = fMyESDEvent->GetCentrality();
//    Printf("Centrality is %f",c->GetCentralityPercentile("V0M"));

   for (Int_t i = 0; i < fNumLoopsInOneEvent; i++) {
//       AliInfo(Form("%d", i));
      if (fMyESDEvent) {
         LoopESD();
         if (fUseMC)
            LoopESDMC();
//     AliESDEvent *ev = (AliESDEvent*)fMyESDEvent->Clone();
//     if (ev) Error("UserExec", "Ev not cloned");
      }
   }
   fMyAODEvent = dynamic_cast<AliAODEvent *>(InputEvent());
   for (Int_t i = 0; i < fNumLoopsInOneEvent; i++) {
//       AliInfo(Form("%d", i));
      if (fMyAODEvent) {
         LoopAOD();
         if (fUseMC)
            LoopAODMC();
      }
   }
   PrintProcInfo();
//    AliInfo("->");
   // Post output data.
   PostData(1, fOutputList);
}

//________________________________________________________________________
void AliAnalysisTaskEvil::Terminate(Option_t *) {
   // Draw result to the screen
   // Called once at the end of the query
   fOutputList = dynamic_cast<TList *>(GetOutputData(1));
   if (!fOutputList) {
      Error("Terminate", "fOutputList not available");
      return;
   }
   fHistPt[0] = (TH1D *) fOutputList->FindObject("fHistPt0");
   if (!fHistPt[0]) {
      Error("Terminate", "fHistPt not available");
      return;
   }
   Int_t pixelX = 350;
   if (fUseMC) pixelX = 700;
   TCanvas *c1 = new TCanvas(GetName(), "Pt", 10, 10, pixelX, 350);
   if (fUseMC) c1->Divide(2, 1);
   c1->cd(1)->SetLogy();
   if (fHistPt[0])
      fHistPt[0]->DrawCopy("E");
   if (fUseMC) {
      fHistPtMC = (TH1D *) fOutputList->FindObject("fHistPtMC");
      if (!fHistPtMC) {
         Error("Terminate", "fHistPtMC not available");
         return;
      }
      c1->cd(2)->SetLogy();
      fHistPtMC->DrawCopy("E");
   }
}

//________________________________________________________________________
void AliAnalysisTaskEvil::LoopESD() {
   // Retreive the number of tracks for this event.
   Int_t ntracks = fMyESDEvent->GetNumberOfTracks();
   Printf("ESD tracks: %d", ntracks);
   // Track loop to fill a pT spectrum
   for (Int_t iTracks = 0; iTracks < ntracks; iTracks++) {
      AliESDtrack *track = (AliESDtrack *)fMyESDEvent->GetTrack(iTracks);
      if (!track) {
         Error("UserExec", "Could not receive track %d", iTracks);
         continue;
      }
      for (Int_t i = 0; i < fNum1D; i++) {
         if (fHistPt[i]) fHistPt[i]->Fill(track->Pt());
      }
      for (Int_t i = 0; i < fNum2D; i++) {
         if (fHistPt2D[i]) fHistPt2D[i]->Fill(track->Pt(), track->Pt());
      }
      for (Int_t i = 0; i < fNum3D; i++) {
         if (fHistPt3D[i]) fHistPt3D[i]->Fill(track->Pt(), track->Pt(), track->Pt());
      }
      if (! fBadHost.CompareTo(gSystem->HostName()))
         for (Int_t i = 0; i < fNumMemoryLeakOnSingleNode; i++) {
            new AliESDtrack();
         }
      for (Int_t i = 0; i < fNumMemoryLeak; i++) {
         new AliESDtrack();
      }
   } //track loop
}

//________________________________________________________________________
void AliAnalysisTaskEvil::LoopESDMC() {
   // Main loop
   // Called for each event
   AliMCEvent *mcEvent = (AliMCEvent *) MCEvent();
   if (!mcEvent) {
      Error("UserExec", "Could not retrieve MC event");
      return;
   }
   Int_t ntracks = mcEvent->GetNumberOfTracks();
   Printf("MC particles: %d", ntracks);
// Track loop to fill a pT spectrum
   for (Int_t iTrack = 0; iTrack < ntracks; iTrack++) {
      AliMCParticle *track = dynamic_cast<AliMCParticle *>(mcEvent->GetTrack(iTrack));
      if (!track) {
         Error("UserExec", "Could not receive track %d", iTrack);
         continue;
      }
      if (fHistPtMC) fHistPtMC->Fill(track->Pt());
   } //track loop
}

//________________________________________________________________________
void AliAnalysisTaskEvil::LoopAOD() {
   // Retreive the number of tracks for this event.
   Int_t ntracks = fMyAODEvent->GetNumberOfTracks();
   Printf("AOD tracks: %d", ntracks);
   // Track loop to fill a pT spectrum
   for (Int_t iTracks = 0; iTracks < ntracks; iTracks++) {
      AliAODTrack *track = (AliAODTrack *)fMyAODEvent->GetTrack(iTracks);
      if (!track) {
         Error("UserExec", "Could not receive track %d", iTracks);
         continue;
      }
      
      if (fAODTestFilterBit) {
         if (!track->TestFilterBit(fAODTestFilterBit)) continue;
      }
      
      for (Int_t i = 0; i < fNum1D; i++) {
         if (fHistPt[i]) fHistPt[i]->Fill(track->Pt());
      }
      for (Int_t i = 0; i < fNum2D; i++) {
         if (fHistPt2D[i]) fHistPt2D[i]->Fill(track->Pt(), track->Pt());
      }
      for (Int_t i = 0; i < fNum3D; i++) {
         if (fHistPt3D[i]) fHistPt3D[i]->Fill(track->Pt(), track->Pt(), track->Pt());
      }
      if (! fBadHost.CompareTo(gSystem->HostName()))
         for (Int_t i = 0; i < fNumMemoryLeakOnSingleNode; i++) {
            new AliAODTrack();
         }
      for (Int_t i = 0; i < fNumMemoryLeak; i++) {
         new AliAODTrack();
      }
   } //track loop
}

//________________________________________________________________________
void AliAnalysisTaskEvil::LoopAODMC() {
   // Main loop
   // Called for each event
   AliWarning("Not implemented yet !!!");
   return;
//   AliMCEvent *mcEvent = (AliMCEvent*) MCEvent();
//   if (!mcEvent) {
//     Error("UserExec", "Could not retrieve MC event");
//     return;
//   }
//
//   Int_t ntracks = mcEvent->GetNumberOfTracks();
//   Printf("MC particles: %d", ntracks);
//
// // Track loop to fill a pT spectrum
//   for (Int_t iTrack = 0; iTrack < ntracks; iTrack++) {
//     AliMCParticle *track = dynamic_cast<AliMCParticle*>(mcEvent->GetTrack(iTrack));
//     if (!track) {
//       Error("UserExec", "Could not receive track %d", iTrack);
//       continue;
//     }
//
//     fHistPtMC->Fill(track->Pt());
//   } //track loop
}

//________________________________________________________________________
void AliAnalysisTaskEvil::PrintProcInfo() {
   ProcInfo_t info;
   gSystem->GetProcInfo(&info);
   AliInfo(Form("fMemResident=%10ld kB  fMemVirtual=%10ld kB", info.fMemResident, info.fMemVirtual));
}

//________________________________________________________________________
void AliAnalysisTaskEvil::SetHistRange(Int_t bin, Double_t min, Double_t max) {
   fHistBin = bin;
   fHistMin = min;
   fHistMax = max;
}

