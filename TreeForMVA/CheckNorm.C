#if !defined (__CINT__) || defined (__CLING__)

#include <Riostream.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TList.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TLegend.h>

#include "AliNormalizationCounter.h"

#endif

using namespace std;

void CheckNorm(TString input="AnalysisResults_pp.root");
double ComputeNorm(double nEvRecoVtx, double nEvNoRecoVtx, double nEvRecoVtxGtr10, double nEvForNorm);

//________________________________________________________________
void CheckNorm(TString input) {
  
  TFile *f = TFile::Open(input.Data());
  TDirectory *dir = (TDirectory*)f->Get("PWGHF_TreeCreator");
  TTree *tree = (TTree*)dir->Get("tree_event_char");
  TH2F *hNevVsCent = (TH2F*)dir->Get("coutputNormCounterHistoHFTreeCreator_all");
  TH1F *hNev = (TH1F*)hNevVsCent->ProjectionX("hNev");
  TList *listnorm = (TList*)dir->Get("coutputNormHFTreeCreator_all");
  AliNormalizationCounter *normcount = (AliNormalizationCounter*)listnorm->FindObject("norm_counter");
  
  int is_ev_rej;
  tree->SetBranchAddress("is_ev_rej",&is_ev_rej);
  
  double nEvSel = 0;
  double nEvNoRecoVtx = 0;
  double nEvRecoVtxGtr10 = 0;
  double nEvPileUp = 0;
  for(int iEntry=0; iEntry<tree->GetEntries(); iEntry++) {
    tree->GetEntry(iEntry);
    if(is_ev_rej==0) nEvSel++;
    else {
      if(is_ev_rej>>0&1 || is_ev_rej>>6&1 || is_ev_rej>>5&1 || is_ev_rej>>9&1 || is_ev_rej>>10&1 || is_ev_rej>>11&1) continue; //rejected because of trigger / physics selection / centrality
      if(is_ev_rej>>1&1 || is_ev_rej>>2&1 || is_ev_rej>>7&1 || is_ev_rej>>12&1) nEvNoRecoVtx++; //whyrejected==0 rejected because of no vtx reconstructed (different causes)
      else if(is_ev_rej>>3&1) nEvRecoVtxGtr10++;//whyrejected==6 (rejected only because of zvtx>10cm)
      else if(is_ev_rej>>4&1) nEvPileUp++; //rejected because of pileup
    }
  }
  
  double nevfornorm_ttree = ComputeNorm(nEvSel+nEvRecoVtxGtr10, nEvNoRecoVtx, nEvRecoVtxGtr10, nEvSel+nEvNoRecoVtx);
  double nevfornorm_histo = ComputeNorm(hNev->GetBinContent(1),hNev->GetBinContent(2),hNev->GetBinContent(3),hNev->GetBinContent(4));

  TH1F* hNorm = new TH1F("hNorm",";;Normalisation factor",3,0.5,3.5);
  hNorm->GetXaxis()->SetBinLabel(1,"Normalisation counter");
  hNorm->GetXaxis()->SetBinLabel(2,"TH2F");
  hNorm->GetXaxis()->SetBinLabel(3,"TTree");
  hNorm->SetBinContent(1,normcount->GetNEventsForNorm());
  hNorm->SetBinContent(2,nevfornorm_histo);
  hNorm->SetBinContent(3,nevfornorm_ttree);

  TH1F* hNevTree = new TH1F("hNevTree",";;#it{N}_{events}",5,-0.5,4.5);
  hNevTree->SetBinContent(1,nEvSel+nEvRecoVtxGtr10);
  hNevTree->SetBinContent(2,nEvNoRecoVtx);
  hNevTree->SetBinContent(3,nEvRecoVtxGtr10);
  hNevTree->SetBinContent(4,nEvSel+nEvNoRecoVtx);
  hNevTree->SetBinContent(5,nEvPileUp);
  hNevTree->SetMarkerStyle(kOpenCircle);
  hNevTree->SetMarkerColor(kRed);
  hNevTree->SetLineColor(kRed);
  
  hNev->SetMarkerStyle(kFullSquare);
  hNev->GetYaxis()->SetTitle("#it{N}_{events}");

  TLegend* leg = new TLegend(0.65,0.4,0.85,0.5);
  leg->AddEntry(hNev,"TH2F");
  leg->AddEntry(hNevTree,"TTree");

  TCanvas* cNorm = new TCanvas("cNorm","cNorm",1920,1080);
  cNorm->Divide(2,1);
  cNorm->cd(1)->SetLeftMargin(0.14);
  hNorm->Draw("text");
  cNorm->cd(2)->SetLeftMargin(0.14);
  cNorm->cd(2)->SetLogy();
  hNev->GetXaxis()->SetRangeUser(-0.5,4.5);
  hNev->Draw("E");
  hNevTree->Draw("Esame");
  leg->Draw("same");
}

//________________________________________________________________
double ComputeNorm(double nEvRecoVtx, double nEvNoRecoVtx, double nEvRecoVtxGtr10, double nEvForNorm) {
  
  return nEvForNorm-nEvNoRecoVtx*nEvRecoVtxGtr10/nEvRecoVtx;
}
