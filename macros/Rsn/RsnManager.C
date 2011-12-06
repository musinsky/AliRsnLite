
// use parfiles instead of libs
Bool_t gRsnUseEventMixingPar       = 0;
Bool_t gRsnUseRSNPar               = 0;

// common options
Bool_t gRsnUsePhysSel              = 0;

// rsn options
Bool_t gRsnUseMiniPackage          = 1;

// mixing setting
Bool_t gRsnUseMixing               = 1;
Int_t  gRsnNumMix                  = 10;

// oputput settings
Bool_t gRsnOutputFull              = 1;
Bool_t gRsnUseMCMomentum           = 0;

// expert options (don't change)
Bool_t gRsnUsePIDResponse          = 1;
Bool_t gRsnUseRsnInputHandler      = 1;
Bool_t gRsnSplitMgrByTasks         = 1;

TList *RsnManager() {

   // don't change (experts only)
   ExpertDefaultSetting();

   TList *listRsn = new TList();

   // This will use AddRsnPairs<Name>.C
   // and for cuts AddRsnDaughterCuts<CutName>.C
   // and <opt> string is passed to AddRsnDaughterCuts<CutName>.C
   // so you can control different cut settings
   // string "<Name>:mon" means that it will add monitoring histograms from cuts
   // Note : for now you have to set gRsnUseMiniPackage = 0 to have mon histograms
//    listRsn->Add(new TNamed("<Name>:mon","<CutName>:<opt>"));
   
   
//    listRsn->Add(new TNamed("Phi:mon","Phi2010"));
//    listRsn->Add(new TNamed("Phi:mon","Phi2010:qualityonly"));
//    listRsn->Add(new TNamed("Phi","Phi2010:tpconly_sigma1"));
//    listRsn->Add(new TNamed("Phi","Phi2010:tpconly_sigma2"));
//    listRsn->Add(new TNamed("Phi","Phi2010:tpconly_sigma3"));
//    listRsn->Add(new TNamed("Phi","Phi2010:tofonly_sigma1"));
//    listRsn->Add(new TNamed("Phi","Phi2010:tofonly_sigma2"));
//    listRsn->Add(new TNamed("Phi","Phi2010:tofonly_sigma3"));
//    listRsn->Add(new TNamed("Phi","BPID"));
//    
//    // in case you have MC
//    listRsn->Add(new TNamed("Phi","PDG"));
// 
   listRsn->Add(new TNamed("KStar","KStar2010"));
//    listRsn->Add(new TNamed("KStar","BPID"));

   return listRsn;
}
void ExpertDefaultSetting() {

   if (gRsnUseMiniPackage) {
      gRsnUseRsnInputHandler = 0;
   } else  {
      gRsnSplitMgrByTasks = 0;
//       gRsnUseMixing = 0;
   }
}
