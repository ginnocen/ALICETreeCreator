void debug(Long64_t nev = TTree::kMaxEntries)
{
  const Double_t mD0PDG = TDatabasePDG::Instance()->GetParticle(421)->Mass();
  printf("using D0 mass: %g\n", mD0PDG);

  auto oldDir = gDirectory;

  std::string maindir = "./";
  std::string filename = maindir + "AnalysisResults.root";

  filename = "AnalysisResults.root";

  std::unique_ptr<TFile> f {TFile::Open(filename.c_str())};
  TTree *t_ev = (TTree*) f->Get("PWGHF_TreeCreator/tree_event_char");
  TTree *t_cand = (TTree*) f->Get("PWGHF_TreeCreator/tree_D0");
  t_ev->BuildIndex("run_number", "ev_id");
  // t_ev->AddFriend(t_cand);
  t_cand->AddFriend(t_ev);

  oldDir->cd();

  t_cand->Draw("n_tracklets>>hntrk_int7(100,0,100)", "run_number == 274657 && is_ev_rej == 0 && trigger_hasbit_INT7", "", nev);
  // t_cand->Draw("n_tracklets>>hntrk_int7(100,0,100)", "run_number == 274657 && is_ev_rej == 0 && trigger_hasbit_INT7 && (trigger_bitmap & (1 << 19)) == 0", "", nev);
  // t_cand->Draw("n_tracklets_corr>>hntrk_int7_withd(100,0,100)", "run_number == 274657 && is_ev_rej == 0 && trigger_hasbit_INT7 && ((tree_D0.cand_type & 0x1800) != 0) && (TMath::Abs(tree_D0.inv_mass - 1.86484) < 0.02)", "", nev);
  t_cand->Draw("n_tracklets>>hntrk_int7_withd(100,0,100)", "run_number == 274657 && is_ev_rej == 0 && trigger_hasbit_INT7 && ((tree_D0.cand_type & 0x1800) !=0)", "", nev);
  t_cand->Draw("bc>>h1", "run_number == 274657 && is_ev_rej == 0 && trigger_hasbit_INT7 && n_tracklets < 10", "", nev);
  t_cand->SetLineColor(kRed);
  t_cand->Draw("bc>>h2", "run_number == 274657 && is_ev_rej == 0 && trigger_hasbit_INT7 && n_tracklets > 10", "", nev);

  f->Close();

  auto *hntrk_int7 = ((TH1*) gDirectory->FindObject("hntrk_int7"));
  auto *hntrk_int7_withd = ((TH1*) gDirectory->FindObject("hntrk_int7_withd"));
  auto *h1 = ((TH1*) gDirectory->FindObject("h1"));
  auto *h2 = ((TH1*) gDirectory->FindObject("h2"));

  TCanvas c("ntrkl_evsel", "ntrkl_evsel", 1600, 1024);;
  c.Divide(1, 3);
  c.cd(1);
  hntrk_int7->Draw();
  c.cd(2);
  gPad->Clear();
  // hntrk_int7_withd->Draw("");
  h1->Draw("colz");
  c.cd(3);
  h2->Draw("colz");
  c.SaveAs("debug.png");
  c.SaveAs("debug.root");
}
