
// use parfiles instead of libs 
Bool_t gRsnUseEventMixingPar       = 1;
Bool_t gRsnUseRSNPar               = 0;

// common options
Bool_t gRsnUsePhysSel              = 0;

// rsn options
Bool_t gRsnUseMiniPackage          = 0;

// mixing setting
Bool_t gRsnUseMixing               = 1;
Int_t  gRsnNumMix                  = 1;

// oputput settings
Bool_t gRsnOutputFull              = 0;
Bool_t gRsnUseMCMomentum           = 0;
Bool_t gRsnUseMonitor              = 0;

// expert options (don't change)
Bool_t gRsnUsePIDResponse          = 1;
Bool_t gRsnUseRsnInputHandler      = 1;
Bool_t gRsnSplitMgrByTasks         = 1;



TList * RsnManager() {

  TList *listRsn = new TList();
  listRsn->Add(new TNamed("phi","Phi2010"));
//   listRsn->Add(new TNamed("phi","Phi2010KaonFromKStar"));
//   listRsn->Add(new TNamed("kstar","KStar2010"));
//   listRsn->Add(new TNamed("lambda","LambdaLStar2010"));
  
  
  
  
  return listRsn;
}
