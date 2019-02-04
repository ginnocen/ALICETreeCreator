#include <iostream>
#include <vector>
#include <algorithm>
#include <TKey.h>
#include "tree_Dzero.C"
#include "tree_Event.C"
#include "tree_Gen.C"

using namespace std;

//Commented sections is code for the additional information in the big TTree that might need to be saved to the skimmed ttree at a later point.

void skimTreeDzeroFromEvt(TString input="AnalysisResults.root",TString output="test.root",TString ttreeout="tree_Dzero", Bool_t isMC = kFALSE, Bool_t ispp = kFALSE){

  TFile *f = TFile::Open(input.Data());
  TDirectory * dir = (TDirectory*)f->Get("PWGHF_TreeCreator");
  TTree* tree = (TTree*)dir->Get(ttreeout.Data());
  TTree* tree_ev = (TTree*)dir->Get("tree_event_char");
  TTree* tree_gen = 0;
  if(isMC){
    tree_gen = (TTree*)dir->Get(Form("%s_gen",ttreeout.Data()));
    if(!tree_gen) cout << "MC generated TTree was not enabled, skipping this." << endl;
  }

  tree_Dzero t(tree);
  tree_Event t_ev(tree_ev, isMC);
  tree_Gen t_gen(tree_gen);

  int nevt = t.GetEntriesFast();
  cout << "\n\nRUNNING Dzero: " << input.Data() << endl;
  TFile *fout = new TFile(output.Data(),"recreate");

  //TH1F* hEvent = 0;
  //TH2F* hNorm = 0;
  //for(auto k : *dir->GetListOfKeys()) {
  //  TKey *key = static_cast<TKey*>(k);
  //  TClass *cl = gROOT->GetClass(key->GetClassName());
  //  if (cl->InheritsFrom("TH1F")){
  //    TH1F* hEvent=(TH1F*)key->ReadObj();
  //    hEvent->Write();
  //  } else if (cl->InheritsFrom("TH2F")){
  //    TH2F* hNorm=(TH2F*)key->ReadObj();
  //    hNorm->Write();
  //  }
  //}

  TTree* fTreeEventCharML = new TTree("fTreeEventChar","fTreeEventChar");
  TTree* fTreeDzeroML = new TTree("fTreeDzeroFlagged","fTreeDzeroFlagged");
  TTree* fTreeDzeroGenML;
  if(isMC && tree_gen) fTreeDzeroGenML = new TTree("fTreeDzeroGenFlagged","fTreeDzeroGenFlagged");

  float centrality_ML, z_vtx_reco_ML, z_vtx_gen_ML;
  int n_vtx_contributors_ML, n_tracks_ML, is_ev_rej_ML, run_number_ML;

  if(!ispp) fTreeEventCharML->Branch("centrality_ML",&centrality_ML,"centrality_ML/F");
  fTreeEventCharML->Branch("z_vtx_reco_ML",&z_vtx_reco_ML,"z_vtx_reco_ML/F");
  //fTreeEventCharML->Branch("n_vtx_contributors_ML",&n_vtx_contributors_ML,"n_vtx_contributors_ML/I");
  //fTreeEventCharML->Branch("n_tracks_ML",&n_tracks_ML,"n_tracks_ML/I");
  fTreeEventCharML->Branch("is_ev_rej_ML",&is_ev_rej_ML,"is_ev_rej_ML/I");
  //fTreeEventCharML->Branch("run_number_ML",&run_number_ML,"run_number_ML/I");
  //if(isMC) fTreeEventCharML->Branch("z_vtx_gen_ML",&z_vtx_gen_ML,"z_vtx_gen_ML/F");
  
  float inv_mass_ML, pt_cand_ML, d_len_ML, d_len_xy_ML, norm_dl_xy_ML, cos_p_ML, cos_p_xy_ML, imp_par_xy_ML, cos_t_star_ML, imp_par_prod_ML, max_norm_d0d0exp_ML;
  float y_cand_ML, eta_cand_ML, phi_cand_ML;
  int cand_type_ML;
  float imp_par_prong0_ML, imp_par_prong1_ML, p_prong0_ML, p_prong1_ML, pt_prong0_ML, pt_prong1_ML, eta_prong0_ML, eta_prong1_ML, phi_prong0_ML, phi_prong1_ML;
  float nTPCcls_prong0_ML, nTPCclspid_prong0_ML, nTPCcrossrow_prong0_ML, chi2perndf_prong0_ML, nITScls_prong0_ML, ITSclsmap_prong0_ML, nTPCcls_prong1_ML, nTPCclspid_prong1_ML, nTPCcrossrow_prong1_ML, chi2perndf_prong1_ML, nITScls_prong1_ML, ITSclsmap_prong1_ML;
  float nsigTPC_Pi_0_ML, nsigTPC_K_0_ML, nsigTOF_Pi_0_ML, nsigTOF_K_0_ML, dEdxTPC_0_ML, ToF_0_ML, pTPC_prong0_ML, pTOF_prong0_ML, trlen_prong0_ML, start_time_res_prong0_ML, nsigTPC_Pi_1_ML, nsigTPC_K_1_ML, nsigTOF_Pi_1_ML, nsigTOF_K_1_ML, dEdxTPC_1_ML, ToF_1_ML, pTPC_prong1_ML, pTOF_prong1_ML, trlen_prong1_ML, start_time_res_prong1_ML;
  
  fTreeDzeroML->Branch("inv_mass_ML",&inv_mass_ML,"inv_mass_ML/F");
  fTreeDzeroML->Branch("pt_cand_ML",&pt_cand_ML,"pt_cand_ML/F");
  fTreeDzeroML->Branch("d_len_ML",&d_len_ML,"d_len_ML/F");
  fTreeDzeroML->Branch("d_len_xy_ML",&d_len_xy_ML,"d_len_xy_ML/F");
  fTreeDzeroML->Branch("norm_dl_xy_ML",&norm_dl_xy_ML,"norm_dl_xy_ML/F");
  fTreeDzeroML->Branch("cos_p_ML",&cos_p_ML,"cos_p_ML/F");
  fTreeDzeroML->Branch("cos_p_xy_ML",&cos_p_xy_ML,"cos_p_xy_ML/F");
  fTreeDzeroML->Branch("imp_par_xy_ML",&imp_par_xy_ML,"imp_par_xy_ML/F");
  fTreeDzeroML->Branch("cos_t_star_ML",&cos_t_star_ML,"cos_t_star_ML/F");
  fTreeDzeroML->Branch("imp_par_prod_ML",&imp_par_prod_ML,"imp_par_prod_ML/F");
  fTreeDzeroML->Branch("max_norm_d0d0exp_ML",&max_norm_d0d0exp_ML,"max_norm_d0d0exp_ML/F");

  fTreeDzeroML->Branch("cand_type_ML",&cand_type_ML,"cand_type_ML/I");
  fTreeDzeroML->Branch("y_cand_ML",&y_cand_ML,"y_cand_ML/F");
  fTreeDzeroML->Branch("eta_cand_ML",&eta_cand_ML,"eta_cand_ML/F");
  fTreeDzeroML->Branch("phi_cand_ML",&phi_cand_ML,"phi_cand_ML/F");

  fTreeDzeroML->Branch("imp_par_prong0_ML",&imp_par_prong0_ML,"imp_par_prong0_ML/F");
  fTreeDzeroML->Branch("imp_par_prong1_ML",&imp_par_prong1_ML,"imp_par_prong1_ML/F");
  fTreeDzeroML->Branch("pt_prong0_ML",&pt_prong0_ML,"pt_prong0_ML/F");
  fTreeDzeroML->Branch("pt_prong1_ML",&pt_prong1_ML,"pt_prong1_ML/F");
  fTreeDzeroML->Branch("p_prong0_ML",&p_prong0_ML,"p_prong0_ML/F");
  fTreeDzeroML->Branch("p_prong1_ML",&p_prong1_ML,"p_prong1_ML/F");
  fTreeDzeroML->Branch("eta_prong0_ML",&eta_prong0_ML,"eta_prong0_ML/F");
  fTreeDzeroML->Branch("eta_prong1_ML",&eta_prong1_ML,"eta_prong1_ML/F");
  fTreeDzeroML->Branch("phi_prong0_ML",&phi_prong0_ML,"phi_prong0_ML/F");
  fTreeDzeroML->Branch("phi_prong1_ML",&phi_prong1_ML,"phi_prong1_ML/F");

  fTreeDzeroML->Branch("nTPCcls_prong0_ML",&nTPCcls_prong0_ML,"nTPCcls_prong0_ML/F");
  fTreeDzeroML->Branch("nTPCclspid_prong0_ML",&nTPCclspid_prong0_ML,"nTPCclspid_prong0_ML/F");
  fTreeDzeroML->Branch("nTPCcrossrow_prong0_ML",&nTPCcrossrow_prong0_ML,"nTPCcrossrow_prong0_ML/F");
  fTreeDzeroML->Branch("chi2perndf_prong0_ML",&chi2perndf_prong0_ML,"chi2perndf_prong0_ML/F");
  fTreeDzeroML->Branch("nITScls_prong0_ML",&nITScls_prong0_ML,"nITScls_prong0_ML/F");
  fTreeDzeroML->Branch("ITSclsmap_prong0_ML",&ITSclsmap_prong0_ML,"ITSclsmap_prong0_ML/F");
  fTreeDzeroML->Branch("nTPCcls_prong1_ML",&nTPCcls_prong1_ML,"nTPCcls_prong1_ML/F");
  fTreeDzeroML->Branch("nTPCclspid_prong1_ML",&nTPCclspid_prong1_ML,"nTPCclspid_prong1_ML/F");
  fTreeDzeroML->Branch("nTPCcrossrow_prong1_ML",&nTPCcrossrow_prong1_ML,"nTPCcrossrow_prong1_ML/F");
  fTreeDzeroML->Branch("chi2perndf_prong1_ML",&chi2perndf_prong1_ML,"chi2perndf_prong1_ML/F");
  fTreeDzeroML->Branch("nITScls_prong1_ML",&nITScls_prong1_ML,"nITScls_prong1_ML/F");
  fTreeDzeroML->Branch("ITSclsmap_prong1_ML",&ITSclsmap_prong1_ML,"ITSclsmap_prong1_ML/F");

  fTreeDzeroML->Branch("nsigTPC_Pi_0_ML",&nsigTPC_Pi_0_ML,"nsigTPC_Pi_0_ML/F");
  fTreeDzeroML->Branch("nsigTPC_K_0_ML",&nsigTPC_K_0_ML,"nsigTPC_K_0_ML/F");
  fTreeDzeroML->Branch("nsigTOF_Pi_0_ML",&nsigTOF_Pi_0_ML,"nsigTOF_Pi_0_ML/F");
  fTreeDzeroML->Branch("nsigTOF_K_0_ML",&nsigTOF_K_0_ML,"nsigTOF_K_0_ML/F");
  fTreeDzeroML->Branch("dEdxTPC_0_ML",&dEdxTPC_0_ML,"dEdxTPC_0_ML/F");
  fTreeDzeroML->Branch("ToF_0_ML",&ToF_0_ML,"ToF_0_ML/F");
  fTreeDzeroML->Branch("pTPC_prong0_ML",&pTPC_prong0_ML,"pTPC_prong0_ML/F");
  fTreeDzeroML->Branch("pTOF_prong0_ML",&pTOF_prong0_ML,"pTOF_prong0_ML/F");
  fTreeDzeroML->Branch("trlen_prong0_ML",&trlen_prong0_ML,"trlen_prong0_ML/F");
  fTreeDzeroML->Branch("start_time_res_prong0_ML",&start_time_res_prong0_ML,"start_time_res_prong0_ML/F");
  fTreeDzeroML->Branch("nsigTPC_Pi_1_ML",&nsigTPC_Pi_1_ML,"nsigTPC_Pi_1_ML/F");
  fTreeDzeroML->Branch("nsigTPC_K_1_ML",&nsigTPC_K_1_ML,"nsigTPC_K_1_ML/F");
  fTreeDzeroML->Branch("nsigTOF_Pi_1_ML",&nsigTOF_Pi_1_ML,"nsigTOF_Pi_1_ML/F");
  fTreeDzeroML->Branch("nsigTOF_K_1_ML",&nsigTOF_K_1_ML,"nsigTOF_K_1_ML/F");
  fTreeDzeroML->Branch("dEdxTPC_1_ML",&dEdxTPC_1_ML,"dEdxTPC_1_ML/F");
  fTreeDzeroML->Branch("ToF_1_ML",&ToF_1_ML,"ToF_1_ML/F");
  fTreeDzeroML->Branch("pTPC_prong1_ML",&pTPC_prong1_ML,"pTPC_prong1_ML/F");
  fTreeDzeroML->Branch("pTOF_prong1_ML",&pTOF_prong1_ML,"pTOF_prong1_ML/F");
  fTreeDzeroML->Branch("trlen_prong1_ML",&trlen_prong1_ML,"trlen_prong1_ML/F");
  fTreeDzeroML->Branch("start_time_res_prong1_ML",&start_time_res_prong1_ML,"start_time_res_prong1_ML/F");
  
  int cand_type_gen_ML;
  float pt_cand_gen_ML, y_cand_gen_ML, eta_cand_gen_ML, phi_cand_gen_ML, forCand_z_vtx_gen_ML;
  bool dau_in_acc_gen_ML;

  if(isMC && tree_gen){
    fTreeDzeroGenML->Branch("cand_type_gen_ML",&cand_type_gen_ML,"cand_type_gen_ML/I");
    fTreeDzeroGenML->Branch("pt_cand_gen_ML",&pt_cand_gen_ML,"pt_cand_gen_ML/F");
    fTreeDzeroGenML->Branch("y_cand_gen_ML",&y_cand_gen_ML,"y_cand_gen_ML/F");
    //fTreeDzeroGenML->Branch("eta_cand_gen_ML",&eta_cand_gen_ML,"eta_cand_gen_ML/F");
    //fTreeDzeroGenML->Branch("phi_cand_gen_ML",&phi_cand_gen_ML,"phi_cand_gen_ML/F");
    fTreeDzeroGenML->Branch("dau_in_acc_gen_ML",&dau_in_acc_gen_ML,"dau_in_acc_gen_ML/O");
    fTreeDzeroGenML->Branch("z_vtx_gen_ML",&forCand_z_vtx_gen_ML,"z_vtx_gen_ML/F");
  }
  
  std::cout<<"nevents (Dzero) "<<nevt<<std::endl;
  for(Long64_t jentry=0; jentry<nevt;jentry++){
    t.GetEntry(jentry);   
    t_ev.GetEntry(jentry);
    t_gen.GetEntry(jentry);
    if(jentry%25000==0) cout<<jentry<<endl;
      
    if(!ispp) centrality_ML = t_ev.centrality;
    z_vtx_reco_ML = t_ev.z_vtx_reco;
    //n_vtx_contributors_ML = t_ev.n_vtx_contributors;
    //n_tracks_ML = t_ev.n_tracks;
    is_ev_rej_ML = t_ev.is_ev_rej;
    //run_number_ML = t_ev.run_number;
    //if(isMC) z_vtx_gen_ML = t_ev.z_vtx_gen;
    fTreeEventCharML->Fill();
    
    if(is_ev_rej_ML!=0) continue;

    for(int icand = 0; icand < t.n_cand; icand++){ 
      if(t.inv_mass) inv_mass_ML=t.inv_mass -> at(icand);
      if(t.pt_cand) pt_cand_ML=t.pt_cand -> at(icand);
      if(t.d_len) d_len_ML=t.d_len -> at(icand);
      if(t.d_len_xy) d_len_xy_ML=t.d_len_xy -> at(icand);
      if(t.norm_dl_xy) norm_dl_xy_ML=t.norm_dl_xy -> at(icand);
      if(t.cos_p) cos_p_ML=t.cos_p -> at(icand);
      if(t.cos_p_xy) cos_p_xy_ML=t.cos_p_xy -> at(icand);
      if(t.imp_par_xy) imp_par_xy_ML=t.imp_par_xy -> at(icand);
      if(t.cos_t_star) cos_t_star_ML=t.cos_t_star -> at(icand);
      if(t.imp_par_prod) imp_par_prod_ML=t.imp_par_prod -> at(icand);
      if(t.max_norm_d0d0exp) max_norm_d0d0exp_ML=t.max_norm_d0d0exp -> at(icand);
        
      if(t.cand_type) cand_type_ML=t.cand_type -> at(icand);
      if(t.y_cand) y_cand_ML=t.y_cand -> at(icand);
      if(t.eta_cand) eta_cand_ML=t.eta_cand -> at(icand);
      if(t.phi_cand) phi_cand_ML=t.phi_cand -> at(icand);
    
      if(t.pt_prong0) pt_prong0_ML=t.pt_prong0 -> at(icand);
      if(t.pt_prong1) pt_prong1_ML=t.pt_prong1 -> at(icand);
      if(t.imp_par_prong0) imp_par_prong0_ML=t.imp_par_prong0 -> at(icand);
      if(t.imp_par_prong1) imp_par_prong1_ML=t.imp_par_prong1 -> at(icand);
      if(t.p_prong0) p_prong0_ML=t.p_prong0 -> at(icand);
      if(t.p_prong1) p_prong1_ML=t.p_prong1 -> at(icand);
      if(t.eta_prong0) eta_prong0_ML=t.eta_prong0 -> at(icand);
      if(t.eta_prong1) eta_prong1_ML=t.eta_prong1 -> at(icand);
      if(t.phi_prong0) phi_prong0_ML=t.phi_prong0 -> at(icand);
      if(t.phi_prong1) phi_prong1_ML=t.phi_prong1 -> at(icand);

      if(t.nTPCcls_prong0) nTPCcls_prong0_ML=t.nTPCcls_prong0 -> at(icand);
      if(t.nTPCclspid_prong0) nTPCclspid_prong0_ML=t.nTPCclspid_prong0 -> at(icand);
      if(t.nTPCcrossrow_prong0) nTPCcrossrow_prong0_ML=t.nTPCcrossrow_prong0 -> at(icand);
      if(t.chi2perndf_prong0) chi2perndf_prong0_ML=t.chi2perndf_prong0 -> at(icand);
      if(t.nITScls_prong0) nITScls_prong0_ML=t.nITScls_prong0 -> at(icand);
      if(t.ITSclsmap_prong0) ITSclsmap_prong0_ML=t.ITSclsmap_prong0 -> at(icand);
      if(t.nTPCcls_prong1) nTPCcls_prong1_ML=t.nTPCcls_prong1 -> at(icand);
      if(t.nTPCclspid_prong1) nTPCclspid_prong1_ML=t.nTPCclspid_prong1 -> at(icand);
      if(t.nTPCcrossrow_prong1) nTPCcrossrow_prong1_ML=t.nTPCcrossrow_prong1 -> at(icand);
      if(t.chi2perndf_prong1) chi2perndf_prong1_ML=t.chi2perndf_prong1 -> at(icand);
      if(t.nITScls_prong1) nITScls_prong1_ML=t.nITScls_prong1 -> at(icand);
      if(t.ITSclsmap_prong1) ITSclsmap_prong1_ML=t.ITSclsmap_prong1 -> at(icand);

      if(t.nsigTPC_Pi_0) nsigTPC_Pi_0_ML=t.nsigTPC_Pi_0 -> at(icand);
      if(t.nsigTPC_K_0) nsigTPC_K_0_ML=t.nsigTPC_K_0 -> at(icand);
      if(t.nsigTOF_Pi_0) nsigTOF_Pi_0_ML=t.nsigTOF_Pi_0 -> at(icand);
      if(t.nsigTOF_K_0) nsigTOF_K_0_ML=t.nsigTOF_K_0 -> at(icand);
      if(t.dEdxTPC_0) dEdxTPC_0_ML=t.dEdxTPC_0 -> at(icand);
      if(t.ToF_0) ToF_0_ML=t.ToF_0 -> at(icand);
      if(t.pTPC_prong0) pTPC_prong0_ML=t.pTPC_prong0 -> at(icand);
      if(t.pTOF_prong0) pTOF_prong0_ML=t.pTOF_prong0 -> at(icand);
      if(t.trlen_prong0) trlen_prong0_ML=t.trlen_prong0 -> at(icand);
      if(t.start_time_res_prong0) start_time_res_prong0_ML=t.start_time_res_prong0 -> at(icand);
      if(t.nsigTPC_Pi_1) nsigTPC_Pi_1_ML=t.nsigTPC_Pi_1 -> at(icand);
      if(t.nsigTPC_K_1) nsigTPC_K_1_ML=t.nsigTPC_K_1 -> at(icand);
      if(t.nsigTOF_Pi_1) nsigTOF_Pi_1_ML=t.nsigTOF_Pi_1 -> at(icand);
      if(t.nsigTOF_K_1) nsigTOF_K_1_ML=t.nsigTOF_K_1 -> at(icand);
      if(t.dEdxTPC_1) dEdxTPC_1_ML=t.dEdxTPC_1 -> at(icand);
      if(t.ToF_1) ToF_1_ML=t.ToF_1 -> at(icand);
      if(t.pTPC_prong1) pTPC_prong1_ML=t.pTPC_prong1 -> at(icand);
      if(t.pTOF_prong1) pTOF_prong1_ML=t.pTOF_prong1 -> at(icand);
      if(t.trlen_prong1) trlen_prong1_ML=t.trlen_prong1 -> at(icand);
      if(t.start_time_res_prong1) start_time_res_prong1_ML=t.start_time_res_prong1 -> at(icand);

      fTreeDzeroML->Fill();
    }
    
    int ncandgen = 0;
    if(isMC && tree_gen){
      ncandgen = t_gen.n_cand;
      forCand_z_vtx_gen_ML = t_ev.z_vtx_gen;
    }
    for(int icand = 0; icand < ncandgen; icand++){
      cand_type_gen_ML=t_gen.cand_type -> at(icand);
      pt_cand_gen_ML=t_gen.pt_cand -> at(icand);
      y_cand_gen_ML=t_gen.y_cand -> at(icand);
      //eta_cand_gen_ML=t_gen.eta_cand -> at(icand);
      //phi_cand_gen_ML=t_gen.phi_cand -> at(icand);
      dau_in_acc_gen_ML=t_gen.dau_in_acc -> at(icand);

      fTreeDzeroGenML->Fill();
    }
  }
  fout->Write();
  fout->Close();
}


int main(int argc, char *argv[])
{
  if((argc != 6))
  {
    std::cout << "Wrong number of inputs" << std::endl;
    return 1;
  }
  
  if(argc == 6)
    skimTreeDzeroFromEvt(argv[1],argv[2],argv[3],atoi(argv[4]),atoi(argv[5]));
  return 0;
}
