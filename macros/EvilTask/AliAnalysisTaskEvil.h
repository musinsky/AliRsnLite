#ifndef ALIANALYSISTASKEVIL_H
#define ALIANALYSISTASKEVIL_H

// example of an analysis task creating a p_t spectrum
// Authors: Martin Vala

class TList;
class TH1D;
class TH2D;
class TH3D;
class TString;
class AliESDEvent;
class AliAODEvent;
#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskEvil : public AliAnalysisTaskSE {
public:
   AliAnalysisTaskEvil(const char *name = "<default name>");
   virtual ~AliAnalysisTaskEvil();

   virtual void   UserCreateOutputObjects();
   virtual void   UserExec(Option_t *option);
   virtual void   Terminate(Option_t *);

   void LoopESD();
   void LoopESDMC();
   void LoopAOD();
   void LoopAODMC();

   void NoOutput(Bool_t b = kTRUE, Bool_t btotal = kFALSE) { fNoOutput = b; fNoOutputTotal = btotal;}
   void UseMC(Bool_t useMC = kTRUE) { fUseMC = useMC; }
   void SetNum1D(Int_t num) { if (num > 10000) fNum1D = 10000; else fNum1D = num;}

   void SetNum2D(Int_t num) { if (num > 1000) fNum2D = 1000; else fNum2D = num; }
   void SetNum3D(Int_t num) { if (num > 100) fNum3D = 100; else fNum3D = num; }
   void SetNumMemoryLeak(Int_t num) { fNumMemoryLeak = num; }
   void SetHistRange(Int_t bin, Double_t min, Double_t max);
   void SetNumLoopsInOneEvent(const Int_t numLoops) { fNumLoopsInOneEvent = numLoops; }
   void PrintProcInfo();
   void SetBadHost(TString hostname = "", Int_t num = 0) { fBadHost = hostname; fNumMemoryLeakOnSingleNode = num; }
private:

   Int_t        fHistBin;
   Double_t     fHistMin;
   Double_t     fHistMax;
   Int_t        fNum1D;
   Int_t        fNum2D;
   Int_t        fNum3D;
   Int_t        fNumMemoryLeak;
   Int_t        fNumMemoryLeakOnSingleNode;
   Bool_t       fUseMC;
   Bool_t       fNoOutput;
   Bool_t       fNoOutputTotal;
   AliESDEvent *fMyESDEvent;                 //! esd event
   AliAODEvent *fMyAODEvent;                 //! aod event

   TList       *fOutputList;                 //! output list
   TH1D        *fHistPt[10000];              //!Pt spectrum ESD
   TH2D        *fHistPt2D[1000];             //!Pt spectrum ESD
   TH3D        *fHistPt3D[100];              //!Pt spectrum ESD

   TH1D        *fHistPtMC;                   //!Pt spectrum MC

   TString      fBadHost;                    // bad host string
   Int_t        fNumLoopsInOneEvent;         // number of loops in one event

   AliAnalysisTaskEvil(const AliAnalysisTaskEvil &); // not implemented
   AliAnalysisTaskEvil &operator=(const AliAnalysisTaskEvil &); // not implemented

   ClassDef(AliAnalysisTaskEvil, 1); // example of analysis
};

#endif


