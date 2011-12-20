void RsnGridPlugin() {

   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) { Printf("Error[RsnGridPlugin] mgr is null !!!"); return }

   AliAnalysisAlien *plugin = (AliAnalysisAlien *) mgr->GetGridHandler();
   if (!plugin) { Printf("Error[RsnGridPlugin] : plugin is null !!!"); return; }

   plugin->SetGridWorkingDir("RsnTrain/test/0002/");
   plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output

   plugin->SetAPIVersion("V1.1x");
   plugin->SetROOTVersion("v5-30-03-1");

   TString alirootVersion="v5-02-14-AN";
//    alirootVersion = gSystem->GetFromPipe("aliroot --version | awk '{print $3}'");

   plugin->SetAliROOTVersion(alirootVersion.Data());

   plugin->SetExecutableCommand("aliroot -b -q");

   plugin->SetAnalysisMacro("RsnTrain.C");
   plugin->SetMasterResubmitThreshold(90);
   plugin->SetTTL(84600);
   plugin->SetInputFormat("xml-single");
   plugin->SetJDLName("RsnTrain.jdl");
   plugin->SetPrice(1);
   plugin->SetSplitMode("se");
   plugin->SetNtestFiles(2);
   plugin->SetMergeViaJDL();
//    plugin->SetKeepLogs(kTRUE);

   RsnSetData(plugin,"MY_DS","aod",kTRUE);
   plugin->SetSplitMaxInputFileNumber(100);
}

void RsnSetData(AliAnalysisAlien *plugin,TString dataset="",TString format="aod",Bool_t isTest=kFALSE) {
   format.ToLower();
   TString runs;

   if (!format.CompareTo("esd")) {
      plugin->SetGridDataDir("/alice/data/2010/LHC10b");
      plugin->SetDataPattern("*ESDs/pass2/*ESDs.root"); // real data check reco pass and data base directory
   } else if (!format.CompareTo("aod")) {
      plugin->SetGridDataDir("/alice/data/2010/LHC10h");
      plugin->SetDataPattern("*ESDs/pass2/AOD049/*AliAOD.root");
   } else {
      Printf("Error wrong data format!!! 'esd' or 'aod' are supported")
      plugin->SetGridDataDir("");
      plugin->SetDataPattern("");
   }

   plugin->SetRunPrefix("000");   // real data

   runs  = "137124, 137125, 137132, 137133, 137135, 137136, 137137, 137161, 137162, 137163, 137165, 137230, 137231";
   runs += ", 137232, 137235, 137236, 137243, 137365, 137366, 137370, 137430, 137431, 137432, 137434, 137439, 137440";
   runs += ", 137441, 137443, 137530, 137531, 137546, 137638, 137639, 137685, 137689, 137693, 137718, 137724, 137843";
   runs += ", 137847, 137848, 138150, 138154, 138190, 138438, 138469, 138533, 138582, 138637, 138652, 138730, 138731";
   runs += ", 138732, 138742, 138826, 138828, 138830, 138870, 138871, 138872, 138977, 138980, 139028, 139029, 139030";
   runs += ", 139031, 139034, 139036, 139105, 139308, 139309, 139310, 139311, 139316, 139360, 139438, 139439, 139440";
   runs += ", 139470, 139503, 139504, 139505, 139510, 139511, 139513, 139514";

   if (isTest) runs = "139110";

   AddRuns(plugin,runs,1000);

}

void AddRuns(AliAnalysisAlien *plugin,TString runList="", Int_t maxRunsPerMaster = 1000) {

   Int_t nRunsPerMaster = 0;
   runList.ReplaceAll(" ","");
   TObjArray *array = runList.Tokenize(",");
   TObjString *str;
   TString strr, strr2_1, strr2_2;
   for (Int_t i = 0; i < array->GetEntriesFast(); i++) {

      str = (TObjString *) array->At(i);
      strr = str->GetString();
      if (!strr.IsNull()) {
         Printf("Adding RUN : %s",strr.Data());
         plugin->AddRunNumber(strr);
         nRunsPerMaster++;
      }
   }

   if (nRunsPerMaster > maxRunsPerMaster) nRunsPerMaster = maxRunsPerMaster;
   plugin->SetNrunsPerMaster(nRunsPerMaster);
}
