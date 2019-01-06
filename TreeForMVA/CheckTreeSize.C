#include <iostream>
#include <cstddef>
#include "TTree.h"
#include "TBranch.h"
#include "Riostream.h"
#include "TMemFile.h"
#include "TKey.h"
#include "TBranchRef.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TStyle.h"

using namespace std;

/////////////////////////////////////////////////////////////
// \author: F. Grosa, fabrizio.grosa@cern.ch               //
// \main function: checkTreeSize                           //
// \brief: macro for study of tree size on disk            //
//         needs as input a root file with skimmed ttrees  //
/////////////////////////////////////////////////////////////

enum cutopt {kLowerLimit,kUpperLimit,kUpperLimitAbsValue};
enum particle {kDzero,kDplus,kDs,kLctopKpi,kBplus};

//____________________________________________________________
void CheckTreeSize(int part=kDs, float pTmin=0, float pTmax=50, TString infilename="skimmedDs.root", TString varname="d_len_ML",float startvalue=0., float stopvalue=0.1, float step=0.01, int opt=kLowerLimit, TString outfilename="d_len_scan.root");
bool CutTree(TString infilename, TString treename, TString varname, float lowlimit, float highlimit);
Long64_t GetBasketSize(TBranch * b, bool inclusive);
Long64_t GetBasketSize(TObjArray * branches, bool inclusive);
Long64_t GetTotalSize(TTree *t);

//____________________________________________________________
void CheckTreeSize(int part, float pTmin, float pTmax, TString infilename, TString varname, float startvalue, float stopvalue, float step, int opt, TString outfilename) {

  gSystem->Unlink("cuttedttree.root");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  const int nsteps = TMath::Abs( stopvalue - startvalue ) / step + 1;

  //define graphs
  TGraph* gNbytesVsVar = new TGraph(nsteps);
  TGraph* gNbytesRatioVsVar = new TGraph(nsteps);
  TGraph* gNcandRatioVsVar = new TGraph(nsteps);
  TGraph* gPromptEffRatioVsVar = new TGraph(nsteps);
  TGraph* gFDEffRatioVsVar = new TGraph(nsteps);
  gNbytesVsVar->SetName("gNbytesVsVar");
  gNbytesVsVar->SetMarkerStyle(kFullCircle);
  gNbytesVsVar->SetMarkerColor(kBlack);
  gNbytesVsVar->SetLineColor(kBlack);
  gNbytesVsVar->GetXaxis()->SetTitle(varname.Data());
  gNbytesVsVar->GetYaxis()->SetTitle("number of bytes / event");
  gNbytesVsVar->SetTitle("");
  gNbytesRatioVsVar->SetName("gNbytesRatioVsVar");
  gNbytesRatioVsVar->SetMarkerStyle(kFullCircle);
  gNbytesRatioVsVar->SetMarkerColor(kBlack);
  gNbytesRatioVsVar->SetLineColor(kBlack);
  gNbytesRatioVsVar->GetXaxis()->SetTitle(varname.Data());
  gNbytesRatioVsVar->GetYaxis()->SetTitle("number of bytes (selected / all)");
  gNbytesRatioVsVar->SetTitle("");
  gNcandRatioVsVar->SetName("gNcandRatioVsVar");
  gNcandRatioVsVar->SetMarkerStyle(kFullCircle);
  gNcandRatioVsVar->SetMarkerColor(kBlack);
  gNcandRatioVsVar->SetLineColor(kBlack);
  gNcandRatioVsVar->GetXaxis()->SetTitle(varname.Data());
  gNcandRatioVsVar->GetYaxis()->SetTitle("number of candidates (selected / all)");
  gNcandRatioVsVar->SetTitle("");
  gPromptEffRatioVsVar->SetName("gPromptEffRatioVsVar");
  gPromptEffRatioVsVar->SetMarkerStyle(kFullCircle);
  gPromptEffRatioVsVar->SetMarkerColor(kRed);
  gPromptEffRatioVsVar->SetLineColor(kRed);
  gPromptEffRatioVsVar->GetXaxis()->SetTitle(varname.Data());
  gPromptEffRatioVsVar->GetYaxis()->SetTitle("efficiency (selected / all)");
  gPromptEffRatioVsVar->SetTitle("");
  gFDEffRatioVsVar->SetName("gFDEffRatioVsVar");
  gFDEffRatioVsVar->SetMarkerStyle(kOpenSquare);
  gFDEffRatioVsVar->SetMarkerColor(kBlue);
  gFDEffRatioVsVar->SetLineColor(kBlue);
  gFDEffRatioVsVar->GetXaxis()->SetTitle(varname.Data());
  gFDEffRatioVsVar->GetYaxis()->SetTitle("efficiency (selected / all)");
  gFDEffRatioVsVar->SetTitle("");

  TString treename = "";
  switch(part) {
    case 0:
      treename="fTreeDzeroFlagged";
      break;
    case 1:
      treename="fTreeDplusFlagged";
      break;
    case 2:
      treename="fTreeDsFlagged";
      break;
    case 3:
      treename="fTreeLcFlagged";
      break;
    case 4:
      treename="fTreeBplusFlagged";
      break;
  }

  //get initial size and number of candidates in selected pt range
  TFile* infile = TFile::Open(infilename.Data());
  if(!infile || !infile->IsOpen()) return;
  TTree* treetot = (TTree*)infile->Get(treename.Data());
  if(!treetot) return;
  TTree* treeev = (TTree*)infile->Get("fTreeEventChar");
  if(!treeev) return;
  Long64_t nev = treeev->GetEntriesFast();
  cout << Form("\nGetting tree for candidates with %.5f < pt_cand_ML < %.5f",pTmin,pTmax) << endl;
  TFile cuttedfile("cuttedttree.root","recreate");
  TTree* treecutted = treetot->CopyTree(Form("pt_cand_ML>%f && pt_cand_ML<%f",pTmin,pTmax));
  treecutted->Write();
  Long64_t ntotbyte = GetTotalSize(treecutted);
  Long64_t ntotentries = treecutted->GetEntriesFast();
  Long64_t ntotprompt = treecutted->GetEntries("cand_type_ML==10");
  Long64_t ntotFD = treecutted->GetEntries("cand_type_ML==18");
  cout << "Number of bytes: " << ntotbyte << endl;
  cout << "Number of candidates: " << ntotentries << endl;
  cout << "Number of prompt: " << ntotprompt << endl;
  cout << "Number of FD: " << ntotFD << "\n" << endl;
  cuttedfile.Close();
  infile->Close();
  
  //scan over selected variable
  float x         = -numeric_limits<float>::max();
  float lowlimit  = -numeric_limits<float>::max();
  float highlimit = numeric_limits<float>::max();
  
  for(int iCut=0; iCut<nsteps; iCut++) {
    
    if(opt==kLowerLimit) {
      lowlimit = startvalue + iCut * step;
      highlimit = numeric_limits<float>::max(); //no cut
      x = lowlimit;
    }
    else if(opt==kUpperLimit) {
      lowlimit = -numeric_limits<float>::max(); //no cut
      highlimit = startvalue - iCut * step;
      x = highlimit;
    }
    else if(opt==kUpperLimitAbsValue) {
      lowlimit = - ( startvalue - iCut * step );
      highlimit = startvalue - iCut * step;
      x = highlimit;
    }

    cout << Form("Cutting ttree in range %.5f < %s < %.5f",lowlimit,varname.Data(),highlimit) << endl;
    bool cuttree = CutTree("cuttedttree.root",treename,varname.Data(),lowlimit,highlimit);
    if(!cuttree) {
      cerr << "Impossible to cut tree. Please check input file and var name" << endl;
      return;
    }
    TFile *file = TFile::Open("cuttedttree.root");
    TTree* tree = (TTree*)file->Get(treename.Data());
    gNbytesVsVar->SetPoint(iCut,x,static_cast<double>(GetTotalSize(tree))/nev);
    gNbytesRatioVsVar->SetPoint(iCut,x,static_cast<double>(GetTotalSize(tree))/ntotbyte);
    gNcandRatioVsVar->SetPoint(iCut,x,static_cast<double>(tree->GetEntriesFast())/ntotentries);
    gPromptEffRatioVsVar->SetPoint(iCut,x,static_cast<double>(tree->GetEntries("cand_type_ML==10"))/ntotprompt);
    gFDEffRatioVsVar->SetPoint(iCut,x,static_cast<double>(tree->GetEntries("cand_type_ML==18"))/ntotFD);
    file->Close();
    delete file;
    file = nullptr;
  }
  gSystem->Unlink("cuttedttree.root");
  
  cout << "\n" << endl;
  
  //draw results
  TCanvas* c = new TCanvas("c","",800,800);
  TLegend* leg = new TLegend(0.6,0.15,0.89,0.3);
  leg->SetTextSize(0.045);
  leg->AddEntry(gPromptEffRatioVsVar,"Prompt","p");
  leg->AddEntry(gFDEffRatioVsVar,"Feed-down","p");

  float xmin = (startvalue<stopvalue) ? startvalue-step : stopvalue-step;
  float xmax = (startvalue>stopvalue) ? startvalue+step : stopvalue+step;

  c->Divide(2,2);
  c->cd(1)->SetLogy();
  c->cd(1)->SetGrid(1,1);
  gNbytesVsVar->Draw("APL");
  c->cd(2)->DrawFrame(xmin,0.1,xmax,2.,Form(";%s;number of bytes (selected / all)",varname.Data()));
  c->cd(2)->SetLogy();
  c->cd(2)->SetGrid(1,1);
  gNbytesRatioVsVar->Draw("PL");
  c->cd(3)->DrawFrame(xmin,0.1,xmax,2.,Form(";%s;number of candidates (selected / all)",varname.Data()));
  c->cd(3)->SetLogy();
  c->cd(3)->SetGrid(1,1);
  gNcandRatioVsVar->Draw("PL");
  c->cd(4)->DrawFrame(xmin,0.1,xmax,2.,Form(";%s;efficiency (selected / all)",varname.Data()));
  c->cd(4)->SetLogy();
  c->cd(4)->SetGrid(1,1);
  gFDEffRatioVsVar->Draw("PL");
  gPromptEffRatioVsVar->Draw("PL");
  leg->Draw("same");
  
  TFile outfile(outfilename.Data(), "recreate");
  c->Write();
  gNbytesVsVar->Write();
  gNbytesRatioVsVar->Write();
  gNcandRatioVsVar->Write();
  gFDEffRatioVsVar->Write();
  gPromptEffRatioVsVar->Write();
  outfile.Close();
  
  outfilename.ReplaceAll(".root",".pdf");
  c->SaveAs(outfilename.Data());
}

//____________________________________________________________
bool CutTree(TString infilename, TString treename, TString varname, float lowlimit, float highlimit) {
  
  TFile* infile = TFile::Open(infilename.Data());
  if(!infile || !infile->IsOpen()) return false;
  TTree* tree = (TTree*)infile->Get(treename.Data());
  if(!tree) return false;
  
  TFile outfile("cuttedttree.root","recreate");
  TTree* treeCopy = tree->CopyTree(Form("%s>%f && %s<%f",varname.Data(),lowlimit,varname.Data(),highlimit));
  treeCopy->Write();
  outfile.Close();
  
  return true;
}

//____________________________________________________________
Long64_t GetBasketSize(TObjArray * branches, bool inclusive) {
  Long64_t result = 0;
  size_t n = branches->GetEntries();
  for( size_t i = 0; i < n; ++ i ) {
    result += GetBasketSize( dynamic_cast<TBranch*>( branches->At( i ) ), inclusive );
  }
  return result;
}

//____________________________________________________________
Long64_t GetBasketSize(TBranch * b, bool inclusive) {
  Long64_t result = 0;
  if (b) {
    if (b->GetZipBytes() > 0) {
      result = b->GetZipBytes();
    } else {
      result = b->GetTotBytes();
    }
    if (inclusive) {
      result += GetBasketSize(b->GetListOfBranches(), true);
    }
    return result;
  }
  return result;
}

//____________________________________________________________
Long64_t GetTotalSize(TTree *t) {
  TKey *key = 0;
  if (t->GetDirectory()) {
    key = t->GetDirectory()->GetKey(t->GetName());
  }
  Long64_t ondiskSize = 0;
  Long64_t totalSize = 0;
  if (key) {
    ondiskSize = key->GetNbytes();
    totalSize = key->GetObjlen();
  } else {
    TMemFile f("buffer","CREATE");
    if (t->GetCurrentFile()) {
      f.SetCompressionSettings(t->GetCurrentFile()->GetCompressionSettings());
    }
    f.WriteTObject(t);
    key = f.GetKey(t->GetName());
    ondiskSize = key->GetNbytes();
    totalSize = key->GetObjlen();
  }
  if (t->GetBranchRef() ) {
    ondiskSize += GetBasketSize(t->GetBranchRef(), true);
  }
  return ondiskSize + GetBasketSize(t->GetListOfBranches(), true);
}
