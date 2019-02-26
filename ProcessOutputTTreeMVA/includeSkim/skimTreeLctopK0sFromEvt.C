#include <iostream>
#include <vector>
#include <algorithm>
#include <TKey.h>
#include <TMath.h>
#include "TGrid.h"
#include "tree_LctopK0s.C"
#include "tree_Event.C"
#include "tree_Gen.C"

using namespace std;

//Commented sections is code for the additional information in the big TTree that might need to be saved to the skimmed ttree at a later point.

void skimTreeLcFromEvt(TString input="AnalysisResults.root",TString output="test.root",TString ttreeout="tree_LctopK0s", Bool_t isMC = kFALSE, Bool_t ispp = kFALSE, Bool_t doDirectGRID = kFALSE, Bool_t wasLcSecVertexReconstructed = kFALSE){

/*-------------Connect to GRID, when downloading stage is skipped-------------*/
  if(doDirectGRID){
    if(!TGrid::Connect("alien://")) {
      printf("no grid connection available... Exiting!");
       return;
    }
  }
  
/*-------------Reading input-------------*/
  TFile *f = TFile::Open(input.Data());
  TDirectory * dir = (TDirectory*)f->Get("PWGHF_TreeCreator");
  TTree* tree = (TTree*)dir->Get(ttreeout.Data());
  TTree* tree_ev = (TTree*)dir->Get("tree_event_char");
  TTree* tree_gen = 0;
  if(isMC){
    tree_gen = (TTree*)dir->Get(Form("%s_gen",ttreeout.Data()));
    if(!tree_gen) cout << "MC generated TTree was not enabled, skipping this." << endl;
  }

/*-------------Input TTrees and initialisation-------------*/
  tree_LctopK0s t(tree);
  tree_Event t_ev(tree_ev, isMC);
  tree_Gen t_gen(tree_gen);

  int nevt = t.GetEntriesFast();
  cout << "\n\nRUNNING LctopK0s: " << input.Data() << endl;
  TFile *fout = new TFile(output.Data(),"recreate");

/*-------------Output TTrees-------------*/
  TTree* fTreeEventCharML = new TTree("fTreeEventChar","fTreeEventChar");
  TTree* fTreeLcML = new TTree("fTreeLctopK0sFlagged","fTreeLctopK0sFlagged");
  TTree* fTreeLcGenML;
  if(isMC && tree_gen) fTreeLcGenML = new TTree("fTreeLctopK0sGenFlagged","fTreeLctopK0sGenFlagged");

  /*-------EventChar-------*/
  float centrality_ML, z_vtx_reco_ML, z_vtx_gen_ML;
  int n_vtx_contributors_ML, n_tracks_ML, is_ev_rej_ML, run_number_ML, fileID_ML, evtID_ML;
  int ev_has_reco_cand_ML, ev_has_gen_cand_ML, n_reco_cand_ML, n_gen_cand_ML;
  bool flagVtxPos_ML, flagVtxIsReco_ML;

  if(!ispp) fTreeEventCharML->Branch("centrality_ML",&centrality_ML,"centrality_ML/F");
  fTreeEventCharML->Branch("z_vtx_reco_ML",&z_vtx_reco_ML,"z_vtx_reco_ML/F");
  //fTreeEventCharML->Branch("n_vtx_contributors_ML",&n_vtx_contributors_ML,"n_vtx_contributors_ML/I");
  //fTreeEventCharML->Branch("n_tracks_ML",&n_tracks_ML,"n_tracks_ML/I");
  fTreeEventCharML->Branch("is_ev_rej_ML",&is_ev_rej_ML,"is_ev_rej_ML/I");
  fTreeEventCharML->Branch("flagVtxPos_ML",&flagVtxPos_ML,"flagVtxPos_ML/O");
  fTreeEventCharML->Branch("flagVtxIsReco_ML",&flagVtxIsReco_ML,"flagVtxIsReco_ML/O");
  fTreeEventCharML->Branch("fileID_ML",&fileID_ML,"fileID_ML/I");
  fTreeEventCharML->Branch("evtID_ML",&evtID_ML,"evtID_ML/I");
  //fTreeEventCharML->Branch("run_number_ML",&run_number_ML,"run_number_ML/I");
  //if(isMC) fTreeEventCharML->Branch("z_vtx_gen_ML",&z_vtx_gen_ML,"z_vtx_gen_ML/F");

  /*-------LctopK0sFlagged-------*/
  float inv_mass_ML, pt_cand_ML, d_len_ML, d_len_xy_ML, norm_dl_xy_ML, cos_p_ML, cos_p_xy_ML, imp_par_xy_ML, dca_ML, cos_t_star_ML, signd0_ML, inv_mass_K0s_ML, dca_K0s_ML, imp_par_K0s_ML, d_len_K0s_ML, armenteros_K0s_ML, ctau_K0s_ML, cos_p_K0s_ML, pt_K0s_ML, eta_K0s_ML, phi_K0s_ML;
  float y_cand_ML, eta_cand_ML, phi_cand_ML;
  int cand_type_ML, cand_fileID_ML, cand_evtID_ML;
  float imp_par_prong0_ML, imp_par_prong1_ML, imp_par_prong2_ML, p_prong0_ML, p_prong1_ML, p_prong2_ML, pt_prong0_ML, pt_prong1_ML, pt_prong2_ML, eta_prong0_ML, eta_prong1_ML, eta_prong2_ML, phi_prong0_ML, phi_prong1_ML, phi_prong2_ML;
  float nTPCcls_prong0_ML, nTPCclspid_prong0_ML, nTPCcrossrow_prong0_ML, chi2perndf_prong0_ML, nITScls_prong0_ML, ITSclsmap_prong0_ML, nTPCcls_prong1_ML, nTPCclspid_prong1_ML, nTPCcrossrow_prong1_ML, chi2perndf_prong1_ML, nITScls_prong1_ML, ITSclsmap_prong1_ML, nTPCcls_prong2_ML, nTPCclspid_prong2_ML, nTPCcrossrow_prong2_ML, chi2perndf_prong2_ML, nITScls_prong2_ML, ITSclsmap_prong2_ML;
  float nsigTPC_Pi_0_ML, nsigTPC_Pr_0_ML, nsigTOF_Pi_0_ML, nsigTOF_Pr_0_ML, dEdxTPC_0_ML, ToF_0_ML, pTPC_prong0_ML, pTOF_prong0_ML, trlen_prong0_ML, start_time_res_prong0_ML, nsigTPC_Pi_1_ML, nsigTPC_Pr_1_ML, nsigTOF_Pi_1_ML, nsigTOF_Pr_1_ML, dEdxTPC_1_ML, ToF_1_ML, pTPC_prong1_ML, pTOF_prong1_ML, trlen_prong1_ML, start_time_res_prong1_ML, nsigTPC_Pi_2_ML, nsigTPC_Pr_2_ML, nsigTOF_Pi_2_ML, nsigTOF_Pr_2_ML, dEdxTPC_2_ML, ToF_2_ML, pTPC_prong2_ML, pTOF_prong2_ML, trlen_prong2_ML, start_time_res_prong2_ML;

  if(t.EnabledSkim_inv_mass) fTreeLcML->Branch("inv_mass_ML",&inv_mass_ML,"inv_mass_ML/F");
  if(t.EnabledSkim_pt_cand) fTreeLcML->Branch("pt_cand_ML",&pt_cand_ML,"pt_cand_ML/F");
  if(t.EnabledSkim_d_len) fTreeLcML->Branch("d_len_ML",&d_len_ML,"d_len_ML/F");
  if(t.EnabledSkim_d_len_xy) fTreeLcML->Branch("d_len_xy_ML",&d_len_xy_ML,"d_len_xy_ML/F");
  if(t.EnabledSkim_norm_dl_xy) fTreeLcML->Branch("norm_dl_xy_ML",&norm_dl_xy_ML,"norm_dl_xy_ML/F");
  if(t.EnabledSkim_cos_p) fTreeLcML->Branch("cos_p_ML",&cos_p_ML,"cos_p_ML/F");
  if(t.EnabledSkim_cos_p_xy) fTreeLcML->Branch("cos_p_xy_ML",&cos_p_xy_ML,"cos_p_xy_ML/F");
  if(t.EnabledSkim_imp_par_xy) fTreeLcML->Branch("imp_par_xy_ML",&imp_par_xy_ML,"imp_par_xy_ML/F");
  if(t.EnabledSkim_dca) fTreeLcML->Branch("dca_ML",&dca_ML,"dca_ML/F");
  if(t.EnabledSkim_cos_t_star) fTreeLcML->Branch("cos_t_star_ML",&cos_t_star_ML,"cos_t_star_ML/F");
  if(t.EnabledSkim_signd0) fTreeLcML->Branch("signd0_ML",&signd0_ML,"signd0_ML/F");
  if(t.EnabledSkim_inv_mass_K0s) fTreeLcML->Branch("inv_mass_K0s_ML",&inv_mass_K0s_ML,"inv_mass_K0s_ML/F");
  if(t.EnabledSkim_dca_K0s) fTreeLcML->Branch("dca_K0s_ML",&dca_K0s_ML,"dca_K0s_ML/F");
  if(t.EnabledSkim_imp_par_K0s) fTreeLcML->Branch("imp_par_K0s_ML",&imp_par_K0s_ML,"imp_par_K0s_ML/F");
  if(t.EnabledSkim_d_len_K0s) fTreeLcML->Branch("d_len_K0s_ML",&d_len_K0s_ML,"d_len_K0s_ML/F");
  if(t.EnabledSkim_armenteros_K0s) fTreeLcML->Branch("armenteros_K0s_ML",&armenteros_K0s_ML,"armenteros_K0s_ML/F");
  if(t.EnabledSkim_ctau_K0s) fTreeLcML->Branch("ctau_K0s_ML",&ctau_K0s_ML,"ctau_K0s_ML/F");
  if(t.EnabledSkim_cos_p_K0s) fTreeLcML->Branch("cos_p_K0s_ML",&cos_p_K0s_ML,"cos_p_K0s_ML/F");
  if(t.EnabledSkim_pt_K0s) fTreeLcML->Branch("pt_K0s_ML",&pt_K0s_ML,"pt_K0s_ML/F");
  if(t.EnabledSkim_eta_K0s) fTreeLcML->Branch("eta_K0s_ML",&eta_K0s_ML,"eta_K0s_ML/F");
  if(t.EnabledSkim_phi_K0s) fTreeLcML->Branch("phi_K0s_ML",&phi_K0s_ML,"phi_K0s_ML/F");

  if(t.EnabledSkim_cand_type) fTreeLcML->Branch("cand_type_ML",&cand_type_ML,"cand_type_ML/I");
  if(t.EnabledSkim_y_cand) fTreeLcML->Branch("y_cand_ML",&y_cand_ML,"y_cand_ML/F");
  if(t.EnabledSkim_eta_cand) fTreeLcML->Branch("eta_cand_ML",&eta_cand_ML,"eta_cand_ML/F");
  if(t.EnabledSkim_phi_cand) fTreeLcML->Branch("phi_cand_ML",&phi_cand_ML,"phi_cand_ML/F");
  fTreeLcML->Branch("fileID_ML",&cand_fileID_ML,"fileID_ML/I");
  fTreeLcML->Branch("evtID_ML",&cand_evtID_ML,"evtID_ML/I");
  
  if(t.EnabledSkim_imp_par_prong0) fTreeLcML->Branch("imp_par_prong0_ML",&imp_par_prong0_ML,"imp_par_prong0_ML/F");
  if(t.EnabledSkim_imp_par_prong1) fTreeLcML->Branch("imp_par_prong1_ML",&imp_par_prong1_ML,"imp_par_prong1_ML/F");
  if(t.EnabledSkim_imp_par_prong2) fTreeLcML->Branch("imp_par_prong2_ML",&imp_par_prong2_ML,"imp_par_prong2_ML/F");
  if(t.EnabledSkim_pt_prong0) fTreeLcML->Branch("pt_prong0_ML",&pt_prong0_ML,"pt_prong0_ML/F");
  if(t.EnabledSkim_pt_prong1) fTreeLcML->Branch("pt_prong1_ML",&pt_prong1_ML,"pt_prong1_ML/F");
  if(t.EnabledSkim_pt_prong2) fTreeLcML->Branch("pt_prong2_ML",&pt_prong2_ML,"pt_prong2_ML/F");
  if(t.EnabledSkim_p_prong0) fTreeLcML->Branch("p_prong0_ML",&p_prong0_ML,"p_prong0_ML/F");
  if(t.EnabledSkim_p_prong1) fTreeLcML->Branch("p_prong1_ML",&p_prong1_ML,"p_prong1_ML/F");
  if(t.EnabledSkim_p_prong2) fTreeLcML->Branch("p_prong2_ML",&p_prong2_ML,"p_prong2_ML/F");
  if(t.EnabledSkim_eta_prong0) fTreeLcML->Branch("eta_prong0_ML",&eta_prong0_ML,"eta_prong0_ML/F");
  if(t.EnabledSkim_eta_prong1) fTreeLcML->Branch("eta_prong1_ML",&eta_prong1_ML,"eta_prong1_ML/F");
  if(t.EnabledSkim_eta_prong2) fTreeLcML->Branch("eta_prong2_ML",&eta_prong2_ML,"eta_prong2_ML/F");
  if(t.EnabledSkim_phi_prong0) fTreeLcML->Branch("phi_prong0_ML",&phi_prong0_ML,"phi_prong0_ML/F");
  if(t.EnabledSkim_phi_prong1) fTreeLcML->Branch("phi_prong1_ML",&phi_prong1_ML,"phi_prong1_ML/F");
  if(t.EnabledSkim_phi_prong2) fTreeLcML->Branch("phi_prong2_ML",&phi_prong2_ML,"phi_prong2_ML/F");
    
  if(t.EnabledSkim_nTPCcls_prong0) fTreeLcML->Branch("nTPCcls_prong0_ML",&nTPCcls_prong0_ML,"nTPCcls_prong0_ML/F");
  if(t.EnabledSkim_nTPCclspid_prong0) fTreeLcML->Branch("nTPCclspid_prong0_ML",&nTPCclspid_prong0_ML,"nTPCclspid_prong0_ML/F");
  if(t.EnabledSkim_nTPCcrossrow_prong0) fTreeLcML->Branch("nTPCcrossrow_prong0_ML",&nTPCcrossrow_prong0_ML,"nTPCcrossrow_prong0_ML/F");
  if(t.EnabledSkim_chi2perndf_prong0) fTreeLcML->Branch("chi2perndf_prong0_ML",&chi2perndf_prong0_ML,"chi2perndf_prong0_ML/F");
  if(t.EnabledSkim_nITScls_prong0) fTreeLcML->Branch("nITScls_prong0_ML",&nITScls_prong0_ML,"nITScls_prong0_ML/F");
  if(t.EnabledSkim_ITSclsmap_prong0) fTreeLcML->Branch("ITSclsmap_prong0_ML",&ITSclsmap_prong0_ML,"ITSclsmap_prong0_ML/F");
  if(t.EnabledSkim_nTPCcls_prong1) fTreeLcML->Branch("nTPCcls_prong1_ML",&nTPCcls_prong1_ML,"nTPCcls_prong1_ML/F");
  if(t.EnabledSkim_nTPCclspid_prong1) fTreeLcML->Branch("nTPCclspid_prong1_ML",&nTPCclspid_prong1_ML,"nTPCclspid_prong1_ML/F");
  if(t.EnabledSkim_nTPCcrossrow_prong1) fTreeLcML->Branch("nTPCcrossrow_prong1_ML",&nTPCcrossrow_prong1_ML,"nTPCcrossrow_prong1_ML/F");
  if(t.EnabledSkim_chi2perndf_prong1) fTreeLcML->Branch("chi2perndf_prong1_ML",&chi2perndf_prong1_ML,"chi2perndf_prong1_ML/F");
  if(t.EnabledSkim_nITScls_prong1) fTreeLcML->Branch("nITScls_prong1_ML",&nITScls_prong1_ML,"nITScls_prong1_ML/F");
  if(t.EnabledSkim_ITSclsmap_prong1) fTreeLcML->Branch("ITSclsmap_prong1_ML",&ITSclsmap_prong1_ML,"ITSclsmap_prong1_ML/F");
  if(t.EnabledSkim_nTPCcls_prong2) fTreeLcML->Branch("nTPCcls_prong2_ML",&nTPCcls_prong2_ML,"nTPCcls_prong2_ML/F");
  if(t.EnabledSkim_nTPCclspid_prong2) fTreeLcML->Branch("nTPCclspid_prong2_ML",&nTPCclspid_prong2_ML,"nTPCclspid_prong2_ML/F");
  if(t.EnabledSkim_nTPCcrossrow_prong2) fTreeLcML->Branch("nTPCcrossrow_prong2_ML",&nTPCcrossrow_prong2_ML,"nTPCcrossrow_prong2_ML/F");
  if(t.EnabledSkim_chi2perndf_prong2) fTreeLcML->Branch("chi2perndf_prong2_ML",&chi2perndf_prong2_ML,"chi2perndf_prong2_ML/F");
  if(t.EnabledSkim_nITScls_prong2) fTreeLcML->Branch("nITScls_prong2_ML",&nITScls_prong2_ML,"nITScls_prong2_ML/F");
  if(t.EnabledSkim_ITSclsmap_prong2) fTreeLcML->Branch("ITSclsmap_prong2_ML",&ITSclsmap_prong2_ML,"ITSclsmap_prong2_ML/F");

  if(t.EnabledSkim_nsigTPC_Pi_0) fTreeLcML->Branch("nsigTPC_Pi_0_ML",&nsigTPC_Pi_0_ML,"nsigTPC_Pi_0_ML/F");
  if(t.EnabledSkim_nsigTPC_Pr_0) fTreeLcML->Branch("nsigTPC_Pr_0_ML",&nsigTPC_Pr_0_ML,"nsigTPC_Pr_0_ML/F");
  if(t.EnabledSkim_nsigTOF_Pi_0) fTreeLcML->Branch("nsigTOF_Pi_0_ML",&nsigTOF_Pi_0_ML,"nsigTOF_Pi_0_ML/F");
  if(t.EnabledSkim_nsigTOF_Pr_0) fTreeLcML->Branch("nsigTOF_Pr_0_ML",&nsigTOF_Pr_0_ML,"nsigTOF_Pr_0_ML/F");
  if(t.EnabledSkim_dEdxTPC_0) fTreeLcML->Branch("dEdxTPC_0_ML",&dEdxTPC_0_ML,"dEdxTPC_0_ML/F");
  if(t.EnabledSkim_ToF_0) fTreeLcML->Branch("ToF_0_ML",&ToF_0_ML,"ToF_0_ML/F");
  if(t.EnabledSkim_pTPC_prong0) fTreeLcML->Branch("pTPC_prong0_ML",&pTPC_prong0_ML,"pTPC_prong0_ML/F");
  if(t.EnabledSkim_pTOF_prong0) fTreeLcML->Branch("pTOF_prong0_ML",&pTOF_prong0_ML,"pTOF_prong0_ML/F");
  if(t.EnabledSkim_trlen_prong0) fTreeLcML->Branch("trlen_prong0_ML",&trlen_prong0_ML,"trlen_prong0_ML/F");
  if(t.EnabledSkim_start_time_res_prong0) fTreeLcML->Branch("start_time_res_prong0_ML",&start_time_res_prong0_ML,"start_time_res_prong0_ML/F");
  if(t.EnabledSkim_nsigTPC_Pi_1) fTreeLcML->Branch("nsigTPC_Pi_1_ML",&nsigTPC_Pi_1_ML,"nsigTPC_Pi_1_ML/F");
  if(t.EnabledSkim_nsigTPC_Pr_1) fTreeLcML->Branch("nsigTPC_Pr_1_ML",&nsigTPC_Pr_1_ML,"nsigTPC_Pr_1_ML/F");
  if(t.EnabledSkim_nsigTOF_Pi_1) fTreeLcML->Branch("nsigTOF_Pi_1_ML",&nsigTOF_Pi_1_ML,"nsigTOF_Pi_1_ML/F");
  if(t.EnabledSkim_nsigTOF_Pr_1) fTreeLcML->Branch("nsigTOF_Pr_1_ML",&nsigTOF_Pr_1_ML,"nsigTOF_Pr_1_ML/F");
  if(t.EnabledSkim_dEdxTPC_1) fTreeLcML->Branch("dEdxTPC_1_ML",&dEdxTPC_1_ML,"dEdxTPC_1_ML/F");
  if(t.EnabledSkim_ToF_1) fTreeLcML->Branch("ToF_1_ML",&ToF_1_ML,"ToF_1_ML/F");
  if(t.EnabledSkim_pTPC_prong1) fTreeLcML->Branch("pTPC_prong1_ML",&pTPC_prong1_ML,"pTPC_prong1_ML/F");
  if(t.EnabledSkim_pTOF_prong1) fTreeLcML->Branch("pTOF_prong1_ML",&pTOF_prong1_ML,"pTOF_prong1_ML/F");
  if(t.EnabledSkim_trlen_prong1) fTreeLcML->Branch("trlen_prong1_ML",&trlen_prong1_ML,"trlen_prong1_ML/F");
  if(t.EnabledSkim_start_time_res_prong1) fTreeLcML->Branch("start_time_res_prong1_ML",&start_time_res_prong1_ML,"start_time_res_prong1_ML/F");
  if(t.EnabledSkim_nsigTPC_Pi_2) fTreeLcML->Branch("nsigTPC_Pi_2_ML",&nsigTPC_Pi_2_ML,"nsigTPC_Pi_2_ML/F");
  if(t.EnabledSkim_nsigTPC_Pr_2) fTreeLcML->Branch("nsigTPC_Pr_2_ML",&nsigTPC_Pr_2_ML,"nsigTPC_Pr_2_ML/F");
  if(t.EnabledSkim_nsigTOF_Pi_2) fTreeLcML->Branch("nsigTOF_Pi_2_ML",&nsigTOF_Pi_2_ML,"nsigTOF_Pi_2_ML/F");
  if(t.EnabledSkim_nsigTOF_Pr_2) fTreeLcML->Branch("nsigTOF_Pr_2_ML",&nsigTOF_Pr_2_ML,"nsigTOF_Pr_2_ML/F");
  if(t.EnabledSkim_dEdxTPC_2) fTreeLcML->Branch("dEdxTPC_2_ML",&dEdxTPC_2_ML,"dEdxTPC_2_ML/F");
  if(t.EnabledSkim_ToF_2) fTreeLcML->Branch("ToF_2_ML",&ToF_2_ML,"ToF_2_ML/F");
  if(t.EnabledSkim_pTPC_prong2) fTreeLcML->Branch("pTPC_prong2_ML",&pTPC_prong2_ML,"pTPC_prong2_ML/F");
  if(t.EnabledSkim_pTOF_prong2) fTreeLcML->Branch("pTOF_prong2_ML",&pTOF_prong2_ML,"pTOF_prong2_ML/F");
  if(t.EnabledSkim_trlen_prong2) fTreeLcML->Branch("trlen_prong2_ML",&trlen_prong2_ML,"trlen_prong2_ML/F");
  if(t.EnabledSkim_start_time_res_prong2) fTreeLcML->Branch("start_time_res_prong2_ML",&start_time_res_prong2_ML,"start_time_res_prong2_ML/F");
  
  /*-------LctopK0sGenFlagged-------*/
  int cand_type_gen_ML, cand_fileID_gen_ML, cand_evtID_gen_ML;
  float pt_cand_gen_ML, y_cand_gen_ML, eta_cand_gen_ML, phi_cand_gen_ML, forCand_z_vtx_gen_ML;
  bool dau_in_acc_gen_ML;

  if(isMC && tree_gen){
    fTreeLcGenML->Branch("cand_type_gen_ML",&cand_type_gen_ML,"cand_type_gen_ML/I");
    fTreeLcGenML->Branch("pt_cand_gen_ML",&pt_cand_gen_ML,"pt_cand_gen_ML/F");
    fTreeLcGenML->Branch("y_cand_gen_ML",&y_cand_gen_ML,"y_cand_gen_ML/F");
    //fTreeLcGenML->Branch("eta_cand_gen_ML",&eta_cand_gen_ML,"eta_cand_gen_ML/F");
    //fTreeLcGenML->Branch("phi_cand_gen_ML",&phi_cand_gen_ML,"phi_cand_gen_ML/F");
    fTreeLcGenML->Branch("dau_in_acc_gen_ML",&dau_in_acc_gen_ML,"dau_in_acc_gen_ML/O");
    fTreeLcGenML->Branch("z_vtx_gen_ML",&forCand_z_vtx_gen_ML,"z_vtx_gen_ML/F");
    fTreeLcGenML->Branch("fileID_ML",&cand_fileID_gen_ML,"fileID_ML/I");
    fTreeLcGenML->Branch("evtID_ML",&cand_evtID_gen_ML,"evtID_ML/I");
  }

  
  /*-------------Extracting file identifier-------------*/
  int place = input.Index("child_");
  int childid_int;
  if(place == -1){
    std::cout<<"Warning, child_X not in the path name. Setting child ID to zero...";
    childid_int = 0;
  } else{
    //Note that this needs to be improved when number of childs > 9!!
    char childid_char = input(place+6);
    childid_int = childid_char - '0';
  }
  int placelast = input.Last('/');
  int placeonetolast = ((TString)input(0,placelast)).Last('/');
  TString jobid = (TString)input(placeonetolast+1,(placelast-placeonetolast-1));
  int jobid_int = jobid.Atoi();
  if(jobid_int == 0){
    std::cout<<"Warning, no job ID was found in path name. Setting to zero...";
  }
  int fileid_int = TMath::Power(10,(jobid.Length())) * childid_int + jobid_int;
  fileID_ML = fileid_int;
  cand_fileID_ML = fileid_int;
  cand_fileID_gen_ML = fileid_int;
  
  
  /*-------------Filling skimmed TTrees-------------*/
  std::cout<<"nevents (Lc) "<<nevt<<std::endl;
  for(Long64_t jentry=0; jentry<nevt;jentry++){
    t.GetEntry(jentry);
    t_ev.GetEntry(jentry);
    t_gen.GetEntry(jentry);
    if(jentry%25000==0) cout<<jentry<<endl;

    evtID_ML = jentry;
    cand_evtID_ML = jentry;
    cand_evtID_gen_ML = jentry;

    if(!ispp) centrality_ML = t_ev.centrality;
    z_vtx_reco_ML = t_ev.z_vtx_reco;
    //n_vtx_contributors_ML = t_ev.n_vtx_contributors;
    //n_tracks_ML = t_ev.n_tracks;
    is_ev_rej_ML = t_ev.is_ev_rej;
    //run_number_ML = t_ev.run_number;
    //if(isMC) z_vtx_gen_ML = t_ev.z_vtx_gen;

    bool rejTrigPhSelCentr = false;
    flagVtxPos_ML = true;
    flagVtxIsReco_ML = true;
    if(is_ev_rej_ML>>0&1 || is_ev_rej_ML>>6&1 || is_ev_rej_ML>>5&1 || is_ev_rej_ML>>9&1 || is_ev_rej_ML>>10&1 || is_ev_rej_ML>>11&1){
      rejTrigPhSelCentr = true; //  ->rejected because of trigger / physics selection / centrality (We do nothing with this flag, but should be checked first)
    } else if(is_ev_rej_ML>>1&1 || is_ev_rej_ML>>2&1 || is_ev_rej_ML>>7&1 || is_ev_rej_ML>>12&1){
      flagVtxIsReco_ML = false; //  ->whyrejected==0 rejected because of no vtx reconstructed (different causes)
    } else if(is_ev_rej_ML>>3&1){
      flagVtxPos_ML = false; //  ->whyrejected==6 (rejected only because of zvtx>10cm)
    }

    fTreeEventCharML->Fill();
    
    if(is_ev_rej_ML!=0) continue;
      
    for(int icand = 0; icand < t.n_cand; icand++){
      if(t.inv_mass) inv_mass_ML=t.inv_mass -> at(icand);
      if(t.pt_cand) pt_cand_ML=t.pt_cand -> at(icand);
      if(wasLcSecVertexReconstructed){
        if(t.d_len) d_len_ML=t.d_len -> at(icand);
        if(t.d_len_xy) d_len_xy_ML=t.d_len_xy -> at(icand);
        if(t.norm_dl_xy) norm_dl_xy_ML=t.norm_dl_xy -> at(icand);
        if(t.cos_p) cos_p_ML=t.cos_p -> at(icand);
        if(t.cos_p_xy) cos_p_xy_ML=t.cos_p_xy -> at(icand);
        if(t.imp_par_xy) imp_par_xy_ML=t.imp_par_xy -> at(icand);
        if(t.dca) dca_ML=t.dca -> at(icand);
      } else {
        d_len_ML=0;
        d_len_xy_ML=0;
        norm_dl_xy_ML=0;
        cos_p_ML=0;
        cos_p_xy_ML=0;
        imp_par_xy_ML=0;
        dca_ML=0;
      }
      if(t.cos_t_star) cos_t_star_ML=t.cos_t_star -> at(icand);
      if(t.signd0) signd0_ML=t.signd0 -> at(icand);
      if(t.inv_mass_K0s) inv_mass_K0s_ML=t.inv_mass_K0s -> at(icand);
      if(t.dca_K0s) dca_K0s_ML=t.dca_K0s -> at(icand);
      if(t.imp_par_K0s) imp_par_K0s_ML=t.imp_par_K0s -> at(icand);
      if(t.d_len_K0s) d_len_K0s_ML=t.d_len_K0s -> at(icand);
      if(t.armenteros_K0s) armenteros_K0s_ML=t.armenteros_K0s -> at(icand);
      if(t.ctau_K0s) ctau_K0s_ML=t.ctau_K0s -> at(icand);
      if(t.cos_p_K0s) cos_p_K0s_ML=t.cos_p_K0s -> at(icand);
      if(t.pt_K0s) pt_K0s_ML=t.pt_K0s -> at(icand);
      if(t.eta_K0s) eta_K0s_ML=t.eta_K0s -> at(icand);
      if(t.phi_K0s) phi_K0s_ML=t.phi_K0s -> at(icand);
      
      if(t.cand_type) cand_type_ML=t.cand_type -> at(icand);
      if(t.y_cand) y_cand_ML=t.y_cand -> at(icand);
      if(t.eta_cand) eta_cand_ML=t.eta_cand -> at(icand);
      if(t.phi_cand) phi_cand_ML=t.phi_cand -> at(icand);

      if(t.pt_prong0) pt_prong0_ML=t.pt_prong0 -> at(icand);
      if(t.pt_prong1) pt_prong1_ML=t.pt_prong1 -> at(icand);
      if(t.pt_prong2) pt_prong2_ML=t.pt_prong2 -> at(icand);
      if(t.imp_par_prong0) imp_par_prong0_ML=t.imp_par_prong0 -> at(icand);
      if(t.imp_par_prong1) imp_par_prong1_ML=t.imp_par_prong1 -> at(icand);
      if(t.imp_par_prong2) imp_par_prong2_ML=t.imp_par_prong2 -> at(icand);
      if(t.p_prong0) p_prong0_ML=t.p_prong0 -> at(icand);
      if(t.p_prong1) p_prong1_ML=t.p_prong1 -> at(icand);
      if(t.p_prong2) p_prong2_ML=t.p_prong2 -> at(icand);
      if(t.eta_prong0) eta_prong0_ML=t.eta_prong0 -> at(icand);
      if(t.eta_prong1) eta_prong1_ML=t.eta_prong1 -> at(icand);
      if(t.eta_prong2) eta_prong2_ML=t.eta_prong2 -> at(icand);
      if(t.phi_prong0) phi_prong0_ML=t.phi_prong0 -> at(icand);
      if(t.phi_prong1) phi_prong1_ML=t.phi_prong1 -> at(icand);
      if(t.phi_prong2) phi_prong2_ML=t.phi_prong2 -> at(icand);

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
      if(t.nTPCcls_prong2) nTPCcls_prong2_ML=t.nTPCcls_prong2 -> at(icand);
      if(t.nTPCclspid_prong2) nTPCclspid_prong2_ML=t.nTPCclspid_prong2 -> at(icand);
      if(t.nTPCcrossrow_prong2) nTPCcrossrow_prong2_ML=t.nTPCcrossrow_prong2 -> at(icand);
      if(t.chi2perndf_prong2) chi2perndf_prong2_ML=t.chi2perndf_prong2 -> at(icand);
      if(t.nITScls_prong2) nITScls_prong2_ML=t.nITScls_prong2 -> at(icand);
      if(t.ITSclsmap_prong2) ITSclsmap_prong2_ML=t.ITSclsmap_prong2 -> at(icand);

      if(t.nsigTPC_Pi_0) nsigTPC_Pi_0_ML=t.nsigTPC_Pi_0 -> at(icand);
      if(t.nsigTPC_Pr_0) nsigTPC_Pr_0_ML=t.nsigTPC_Pr_0 -> at(icand);
      if(t.nsigTOF_Pi_0) nsigTOF_Pi_0_ML=t.nsigTOF_Pi_0 -> at(icand);
      if(t.nsigTOF_Pr_0) nsigTOF_Pr_0_ML=t.nsigTOF_Pr_0 -> at(icand);
      if(t.dEdxTPC_0) dEdxTPC_0_ML=t.dEdxTPC_0 -> at(icand);
      if(t.ToF_0) ToF_0_ML=t.ToF_0 -> at(icand);
      if(t.pTPC_prong0) pTPC_prong0_ML=t.pTPC_prong0 -> at(icand);
      if(t.pTOF_prong0) pTOF_prong0_ML=t.pTOF_prong0 -> at(icand);
      if(t.trlen_prong0) trlen_prong0_ML=t.trlen_prong0 -> at(icand);
      if(t.start_time_res_prong0) start_time_res_prong0_ML=t.start_time_res_prong0 -> at(icand);
      if(t.nsigTPC_Pi_1) nsigTPC_Pi_1_ML=t.nsigTPC_Pi_1 -> at(icand);
      if(t.nsigTPC_Pr_1) nsigTPC_Pr_1_ML=t.nsigTPC_Pr_1 -> at(icand);
      if(t.nsigTOF_Pi_1) nsigTOF_Pi_1_ML=t.nsigTOF_Pi_1 -> at(icand);
      if(t.nsigTOF_Pr_1) nsigTOF_Pr_1_ML=t.nsigTOF_Pr_1 -> at(icand);
      if(t.dEdxTPC_1) dEdxTPC_1_ML=t.dEdxTPC_1 -> at(icand);
      if(t.ToF_1) ToF_1_ML=t.ToF_1 -> at(icand);
      if(t.pTPC_prong1) pTPC_prong1_ML=t.pTPC_prong1 -> at(icand);
      if(t.pTOF_prong1) pTOF_prong1_ML=t.pTOF_prong1 -> at(icand);
      if(t.trlen_prong1) trlen_prong1_ML=t.trlen_prong1 -> at(icand);
      if(t.start_time_res_prong1) start_time_res_prong1_ML=t.start_time_res_prong1 -> at(icand);
      if(t.nsigTPC_Pi_2) nsigTPC_Pi_2_ML=t.nsigTPC_Pi_2 -> at(icand);
      if(t.nsigTPC_Pr_2) nsigTPC_Pr_2_ML=t.nsigTPC_Pr_2 -> at(icand);
      if(t.nsigTOF_Pi_2) nsigTOF_Pi_2_ML=t.nsigTOF_Pi_2 -> at(icand);
      if(t.nsigTOF_Pr_2) nsigTOF_Pr_2_ML=t.nsigTOF_Pr_2 -> at(icand);
      if(t.dEdxTPC_2) dEdxTPC_2_ML=t.dEdxTPC_2 -> at(icand);
      if(t.ToF_2) ToF_2_ML=t.ToF_2 -> at(icand);
      if(t.pTPC_prong2) pTPC_prong2_ML=t.pTPC_prong2 -> at(icand);
      if(t.pTOF_prong2) pTOF_prong2_ML=t.pTOF_prong2 -> at(icand);
      if(t.trlen_prong2) trlen_prong2_ML=t.trlen_prong2 -> at(icand);
      if(t.start_time_res_prong2) start_time_res_prong2_ML=t.start_time_res_prong2 -> at(icand);

      fTreeLcML->Fill();
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

      fTreeLcGenML->Fill();
    }
      
  }
  fout->Write();
  fout->Close();
}


int main(int argc, char *argv[])
{
  if((argc != 8))
  {
    std::cout << "Wrong number of inputs" << std::endl;
    return 1;
  }
  
  if(argc == 8)
    skimTreeLcFromEvt(argv[1],argv[2],argv[3],atoi(argv[4]),atoi(argv[5]),atoi(argv[6]),atoi(argv[7]));
  return 0;
}
