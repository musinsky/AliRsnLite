void RsnGridPlugin(TString dsConfig = "LHC10h_p2_AOD073.txt",Int_t id=0) {

   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) { Printf("Error[RsnGridPlugin] mgr is null !!!"); return; }

   AliAnalysisAlien *plugin = (AliAnalysisAlien *) mgr->GetGridHandler();
   if (!plugin) { Printf("Error[RsnGridPlugin] : plugin is null !!!"); return; }

   TString rsnTrainName = dsConfig;
   rsnTrainName.ReplaceAll(".txt",Form("/%03d",id));

   plugin->SetGridWorkingDir(Form("RsnTrain/%s",rsnTrainName.Data()));
   plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output

   plugin->SetAPIVersion("V1.1x");
   plugin->SetROOTVersion("v5-30-03-1");

   TString alirootVersion="v5-02-15-AN";
   alirootVersion = gSystem->GetFromPipe("aliroot --version | awk '{print $3}'");

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

   RsnSetData(plugin,dsConfig);

   plugin->SetSplitMaxInputFileNumber(50);

//   Fatal("RsnDataSet","No dataset found !!!");
}

void RsnSetData(AliAnalysisAlien *plugin,TString dsConf,Int_t maxRunsPerMaster = 1000) {

   Bool_t dsFound = kTRUE;
   Int_t nRunsPerMaster = 0;

   if (dsConf.Contains(".txt")) {
      ifstream in;
      in.open(dsConf.Data());
      TString line;
      Bool_t isRun = kFALSE;
      while (in.good())
      {
         in >> line;
         if (line.IsNull()) continue;
         if (line.Contains("BASE")) {
            GetParameterFromConfig(line);
            plugin->SetGridDataDir(line.Data());
            Printf("BASE -> %s",line.Data());
            continue;
         }
         if (line.Contains("PREFIX")) {
            GetParameterFromConfig(line);
            plugin->SetRunPrefix(line.Data());
            Printf("PREFIX -> %s",line.Data());
            continue;
         }
         if (line.Contains("DATA_PATTERN")) {
            GetParameterFromConfig(line);
            plugin->SetDataPattern(line.Data());
            Printf("DATA_PATTERN -> %s",line.Data());
            continue;
         }
         if (!line.CompareTo("RUNS")) {
            isRun = kTRUE;
            in >> line;
         }
         if (isRun) {
//            Printf("Adding RUN : %s",line.Data());
            plugin->AddRunNumber(line.Data());
            nRunsPerMaster++;
         }
      }
   } else {
      plugin->SetGridDataDir("");
      plugin->SetDataPattern("");
      Fatal("RsnDataSet","No dataset found !!!");
   }

   if (nRunsPerMaster > maxRunsPerMaster) nRunsPerMaster = maxRunsPerMaster;
   plugin->SetNrunsPerMaster(nRunsPerMaster);

}

void GetParameterFromConfig(TString &str,TString token="="){
   TObjArray *array = str.Tokenize(token.Data());
   TObjString *strObj = (TObjString *)array->At(1);
   str = strObj->GetString();
}
