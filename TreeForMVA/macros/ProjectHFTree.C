#if !defined(__CINT__) || defined(__MAKECINT__)

#include <Riostream.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TString.h>
#include <TList.h>
#include <TTree.h>
#include <TDatabasePDG.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "AliHFTreeHandler.h"

#endif

///*************************************************************************
/// main function: ProjectHFTree
/// 
// \authors:
// A. Festanti, andrea.festanti@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
///*************************************************************************

enum particle {kDzero, kDplus, kDs};

//_____________________________________________________
void ProjectHFTree(int part=kDs, bool isMC=false, TString configfilename="config_projtreeDs.txt");
bool LoadConfigFile(TString configfilename, TString inputnames[3], vector<float> &ptlims);
void DivideCanvas(TCanvas* c, int nPtBins);

//_____________________________________________________
void ProjectHFTree(int part, bool isMC, TString configfilename) {

  TString treename = "";
  double mass = 0;
  TString masstitle = "";
  switch(part) {
    case 0:
      treename="tree_D0";
      mass = TDatabasePDG::Instance()->GetParticle(421)->Mass();
      masstitle = "#it{M}(K#pi) (GeV/#it{c}^{2})";
    break;
    case 1:
      treename="tree_Dplus";
      mass = TDatabasePDG::Instance()->GetParticle(411)->Mass();
      masstitle = "#it{M}(K#pi#pi) (GeV/#it{c}^{2})";
    break;
    case 2:
      treename="fTreeDs";
      mass = TDatabasePDG::Instance()->GetParticle(431)->Mass();
      masstitle = "#it{M}(KK#pi) (GeV/#it{c}^{2})";
    break;
    default:
      std::cerr << "Error: Wrong particle set! Exit" << std::endl;
      return;
    break;
  }

  TString inputnames[3];
  vector<float> ptlims;
  bool loadconfig = LoadConfigFile(configfilename,inputnames,ptlims);
  if(!loadconfig) {
    std::cerr << "Error: Config file not loaded! Exit" << std::endl;
    return;
  }
  const unsigned int nPtBins = ptlims.size()-1;
  if(nPtBins<1) {
    std::cerr << "Error: pT bins not correctly set in config file! Exit" << std::endl;
    return;
  }

  TFile* infile = TFile::Open(inputnames[0].Data());
  if(!infile || !infile->IsOpen()) return;
  TDirectoryFile* dir = (TDirectoryFile*)infile->Get(inputnames[1].Data());
  if(!dir) {
    std::cerr << "Error: TDirectory name in config file wrong! Exit" << std::endl;
    return;
  }
  TList* list = (TList*)dir->Get(inputnames[2].Data());
  if(!list) {
    std::cerr << "Error: TList name in config file wrong! Exit" << std::endl;
    return;
  }
  TTree* tree = (TTree*)list->FindObject(treename.Data());
  if(!tree) {
    std::cerr << "Error: TTree for selected particle not found in input file! Exit" << std::endl;
    return;
  }

  TH1F* hMassPromptSignal[nPtBins];
  TH1F* hMassFDSignal[nPtBins];
  TH1F* hMassReflected[nPtBins];
  TH1F* hMassCombinatorialBkg[nPtBins];
  TH1F* hMassSelectedPromptSignal[nPtBins];
  TH1F* hMassSelectedFDSignal[nPtBins];
  TH1F* hMassSelectedReflected[nPtBins];
  TH1F* hMassSelectedCombinatorialBkg[nPtBins];

  TH1F* hMassData[nPtBins];
  TH1F* hMassSelectedData[nPtBins];

  if(isMC) {
    TCanvas* cMassPromptSignal = new TCanvas("cMassPromptSignal","",1920,1080);
    TCanvas* cMassFDSignal = new TCanvas("cMassFDSignal","",1920,1080);
    TCanvas* cMassReflected = new TCanvas("cMassPromptSignal","",1920,1080);
    TCanvas* cMassCombinatorialBkg = new TCanvas("cMassFDSignal","",1920,1080);
    DivideCanvas(cMassPromptSignal,nPtBins);
    DivideCanvas(cMassFDSignal,nPtBins);
    DivideCanvas(cMassReflected,nPtBins);
    DivideCanvas(cMassCombinatorialBkg,nPtBins);

    for(unsigned int iPt=0; iPt<nPtBins; iPt++) {
      hMassPromptSignal[iPt] = new TH1F(Form("hMassPromptSignal_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),Form(";%s;Entries",masstitle.Data()),200,mass-0.2,mass+0.2);
      hMassFDSignal[iPt] = new TH1F(Form("hMassFDSignal_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),Form(";%s;Entries",masstitle.Data()),200,mass-0.2,mass+0.2);
      hMassReflected[iPt] = new TH1F(Form("hMassReflected_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),Form(";%s;Entries",masstitle.Data()),200,mass-0.2,mass+0.2);
      hMassCombinatorialBkg[iPt] = new TH1F(Form("hMassCombinatorialBkg_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),Form(";%s;Entries",masstitle.Data()),200,mass-0.2,mass+0.2);
      hMassSelectedPromptSignal[iPt] = new TH1F(Form("hMassSelectedPromptSignal_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),Form(";%s;Entries",masstitle.Data()),200,mass-0.2,mass+0.2);
      hMassSelectedFDSignal[iPt] = new TH1F(Form("hMassSelectedFDSignal_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),Form(";%s;Entries",masstitle.Data()),200,mass-0.2,mass+0.2);
      hMassSelectedReflected[iPt] = new TH1F(Form("hMassSelectedReflected_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),Form(";%s;Entries",masstitle.Data()),200,mass-0.2,mass+0.2);
      hMassSelectedCombinatorialBkg[iPt] = new TH1F(Form("hMassSelectedCombinatorialBkg_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),Form(";%s;Entries",masstitle.Data()),200,mass-0.2,mass+0.2);
      hMassPromptSignal[iPt]->SetLineColor(kBlack);
      hMassFDSignal[iPt]->SetLineColor(kBlack); 
      hMassReflected[iPt]->SetLineColor(kBlack);
      hMassCombinatorialBkg[iPt]->SetLineColor(kBlack);
      hMassSelectedPromptSignal[iPt]->SetLineColor(kRed);
      hMassSelectedFDSignal[iPt]->SetLineColor(kRed); 
      hMassSelectedReflected[iPt]->SetLineColor(kRed);
      hMassSelectedCombinatorialBkg[iPt]->SetLineColor(kRed);

      tree->Project(Form("hMassPromptSignal_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),"inv_mass",Form("pt_cand>%0.1f && pt_cand<%0.1f && cand_type==%d",ptlims[iPt],ptlims[iPt+1],(AliHFTreeHandler::kSignal | AliHFTreeHandler::kPrompt)));
      tree->Project(Form("hMassFDSignal_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),"inv_mass",Form("pt_cand>%0.1f && pt_cand<%0.1f && cand_type==%d",ptlims[iPt],ptlims[iPt+1],(AliHFTreeHandler::kSignal | AliHFTreeHandler::kFD)));
      tree->Project(Form("hMassReflected_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),"inv_mass",Form("pt_cand>%0.1f && pt_cand<%0.1f && (cand_type==%d || candtype==%d)",ptlims[iPt],ptlims[iPt+1],(AliHFTreeHandler::kRefl | AliHFTreeHandler::kPrompt),(AliHFTreeHandler::kRefl | AliHFTreeHandler::kFD)));
      tree->Project(Form("hMassCombinatorialBkg_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),"inv_mass",Form("pt_cand>%0.1f && pt_cand<%0.1f && cand_type==%d",ptlims[iPt],ptlims[iPt+1],(AliHFTreeHandler::kBkg)));
      tree->Project(Form("hMassSelectedPromptSignal_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),"inv_mass",Form("pt_cand>%0.1f && pt_cand<%0.1f && cand_type==%d",ptlims[iPt],ptlims[iPt+1],(AliHFTreeHandler::kSignal | AliHFTreeHandler::kPrompt | AliHFTreeHandler::kSelected)));
      tree->Project(Form("hMassSelectedFDSignal_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),"inv_mass",Form("pt_cand>%0.1f && pt_cand<%0.1f && cand_type==%d",ptlims[iPt],ptlims[iPt+1],(AliHFTreeHandler::kSignal | AliHFTreeHandler::kFD | AliHFTreeHandler::kSelected)));
      tree->Project(Form("hMassSelectedReflected_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),"inv_mass",Form("pt_cand>%0.1f && pt_cand<%0.1f && (cand_type==%d || candtype==%d)",ptlims[iPt],ptlims[iPt+1],(AliHFTreeHandler::kRefl | AliHFTreeHandler::kPrompt | AliHFTreeHandler::kSelected),(AliHFTreeHandler::kRefl | AliHFTreeHandler::kFD | AliHFTreeHandler::kSelected)));
      tree->Project(Form("hMassSelectedCombinatorialBkg_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),"inv_mass",Form("pt_cand>%0.1f && pt_cand<%0.1f && cand_type==%d",ptlims[iPt],ptlims[iPt+1],(AliHFTreeHandler::kBkg | AliHFTreeHandler::kSelected)));
    
      cMassPromptSignal->cd(iPt+1);
      hMassPromptSignal[iPt]->Draw();
      hMassSelectedPromptSignal[iPt]->Draw("same");
      cMassFDSignal->cd(iPt+1);
      hMassFDSignal[iPt]->Draw();
      hMassSelectedFDSignal[iPt]->Draw("same");
      cMassReflected->cd(iPt+1);
      hMassReflected[iPt]->Draw();
      hMassSelectedReflected[iPt]->Draw("same");
      cMassCombinatorialBkg->cd(iPt+1);
      hMassCombinatorialBkg[iPt]->Draw();
      hMassSelectedCombinatorialBkg[iPt]->Draw("same");
    }
  }
  else {
    TCanvas* cMassData = new TCanvas("cMassData","",1920,1080);
    DivideCanvas(cMassData,nPtBins);

    for(unsigned int iPt=0; iPt<nPtBins; iPt++) {
      hMassData[iPt] = new TH1F(Form("hMassData_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),Form(";%s;Entries",masstitle.Data()),200,mass-0.2,mass+0.2);
      hMassSelectedData[iPt] = new TH1F(Form("hMassSelectedData_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),Form(";%s;Entries",masstitle.Data()),200,mass-0.2,mass+0.2);
      hMassData[iPt]->SetLineColor(kBlack);
      hMassSelectedData[iPt]->SetLineColor(kRed);

      tree->Project(Form("hMassData_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),"inv_mass",Form("pt_cand>%0.1f && pt_cand<%0.1f",ptlims[iPt],ptlims[iPt+1]));
      tree->Project(Form("hMassSelectedData_pt%0.f_%0.f",ptlims[iPt],ptlims[iPt+1]),"inv_mass",Form("pt_cand>%0.1f && pt_cand<%0.1f && cand_type==%d",ptlims[iPt],ptlims[iPt+1],AliHFTreeHandler::kSelected));

      cMassData->cd(iPt+1);
      hMassData[iPt]->Draw("same");
      hMassSelectedData[iPt]->Draw("same");
    }
  }

  TString outfilename = Form("MassProjection_%s.root",treename.Data());
  if(isMC) outfilename = Form("MassProjectionMC_%s.root",treename.Data());

  TFile outfile(outfilename.Data(),"recreate");
  if(isMC) {
    for(unsigned int iPt=0; iPt<nPtBins; iPt++) {
      hMassPromptSignal[iPt]->Write();
      hMassFDSignal[iPt]->Write();
      hMassReflected[iPt]->Write();
      hMassCombinatorialBkg[iPt]->Write();
      hMassSelectedPromptSignal[iPt]->Write();
      hMassSelectedFDSignal[iPt]->Write();
      hMassSelectedReflected[iPt]->Write();
      hMassSelectedCombinatorialBkg[iPt]->Write();
    }
  }
  else {
    for(unsigned int iPt=0; iPt<nPtBins; iPt++) {
      hMassData[iPt]->Write();
      hMassSelectedData[iPt]->Write();
    }
  }
  outfile.Close();
}

//_____________________________________________________
bool LoadConfigFile(TString configfilename, TString inputnames[3], vector<float> &ptlims) {

  ifstream inFile(configfilename.Data());
  if(!inFile) return false;

  string varstring;

  getline(inFile,varstring);
  inputnames[0] = varstring;
  getline(inFile,varstring);
  inputnames[1] = varstring;
  getline(inFile,varstring);
  inputnames[2] = varstring;

  float pt;
  while(inFile>>pt)
    ptlims.push_back(pt);

  return true;
}

//_____________________________________________________
void DivideCanvas(TCanvas* c, int nPtBins) {
  if(nPtBins<2)
    c->cd();
  else if(nPtBins==2 || nPtBins==3)
    c->Divide(nPtBins,1);
  else if(nPtBins==4 || nPtBins==6 || nPtBins==8)
    c->Divide(nPtBins/2,2);
  else if(nPtBins==5 || nPtBins==7)
    c->Divide((nPtBins+1)/2,2);
  else if(nPtBins==9 || nPtBins==12 || nPtBins==15)
    c->Divide(nPtBins/3,3);
  else if(nPtBins==10 || nPtBins==11)
    c->Divide(4,3);
  else if(nPtBins==13 || nPtBins==14)
    c->Divide(5,3);
  else if(nPtBins>15 && nPtBins<=20 && nPtBins%4==0)
    c->Divide(nPtBins/4,4);
  else if(nPtBins>15 && nPtBins<=20 && nPtBins%4!=0)
    c->Divide(5,4);
  else if(nPtBins==21)
    c->Divide(7,3);
  else if(nPtBins>21 && nPtBins<=25)
    c->Divide(5,5);
  else if(nPtBins>25 && nPtBins%2==0)
    c->Divide(nPtBins/2,2);
  else
    c->Divide((nPtBins+1)/2,2);
}