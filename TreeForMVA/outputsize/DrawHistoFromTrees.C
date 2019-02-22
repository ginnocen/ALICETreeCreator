#include <iostream>
#include <cstddef>
#include <limits>
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
#include "TH1F.h"

using namespace std;

//////////////////////////////////////////////////////////////
// \author: L. Vermunt, luuk.vermunt@cern.ch                //
// \main function: DrawHistoFromTrees                       //
// \brief: macro for to draw the distributions              //
//         related to the output of the CheckTreeSize macro //
//////////////////////////////////////////////////////////////

enum cutopt {kLowerLimit,kUpperLimit,kUpperLimitAbsValue};
enum particle {kDzero,kDplus,kDs,kDstar,kLctopKpi,kBplus};

//____________________________________________________________
void DrawHistoFromTrees(Int_t iterator, Long64_t nentriesmax = 10000000, int part=kDs, float pTmin=0, float pTmax=50, TString infilename="skimmedDs.root", TString varname="d_len_ML",float startvalue=0., float stopvalue=0.1, float step=0.01, int opt=kLowerLimit, TString outfilename="d_len_scan.root");
bool DrawTree(TString infilename, TString treename, TString varname, TString cut);

//____________________________________________________________
void DrawHistoFromTrees(Int_t iterator, Long64_t nentriesmax, int part, float pTmin, float pTmax, TString infilename, TString varname, float startvalue, float stopvalue, float step, int opt, TString outfilename) {

  gSystem->Unlink(Form("drawnttree_%d.root",iterator));
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

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
      treename="fTreeDstarFlagged";
      break;
    case 4:
      treename="fTreeLcFlagged";
      break;
    case 5:
      treename="fTreeBplusFlagged";
      break;
    case 6:
      treename="fTreeLctopK0sFlagged";
      break;
  }

  //get initial size and number of candidates in selected pt range
  TFile* infile = TFile::Open(infilename.Data());
  if(!infile || !infile->IsOpen()){ cout << "Could not open file " << infilename.Data() << ". Returning..." << endl; return; }
  else cout << "Opened file " << infilename.Data() << endl;

  TTree* treetot = (TTree*)infile->Get(treename.Data());
  if(!treetot){ cout << "Could not open TTree " << treename.Data() << " in file " << infilename.Data() << ". Returning..." << endl; return; }
  else cout << "Opened TTree " << treename.Data() << endl;

  TTree* treeev = (TTree*)infile->Get("fTreeEventChar");
  if(!treeev){ cout << "Could not open TTree fTreeEventChar in file " << infilename.Data() << ". Returning..." << endl; return; }

  Long64_t nev = treeev->GetEntriesFast();
  cout << Form("\nGetting tree for candidates with %.5f < pt_cand_ML < %.5f",pTmin,pTmax) << endl;
  TFile cuttedfile(Form("drawnttree_%d.root",iterator),"recreate");
  TTree* treecutted = treetot->CopyTree(Form("pt_cand_ML>%f && pt_cand_ML<%f",pTmin,pTmax),"",nentriesmax);
  treecutted->Write();
  Long64_t ntotentries = treecutted->GetEntriesFast();
  Long64_t ntotprompt = treecutted->GetEntries("cand_type_ML>>3&1");
  Long64_t ntotFD = treecutted->GetEntries("cand_type_ML>>4&1");
  cout << "Number of candidates: " << ntotentries << endl;
  cout << "Number of prompt: " << ntotprompt << endl;
  cout << "Number of FD: " << ntotFD << "\n" << endl;
  cuttedfile.Close();
  infile->Close();

  cout << "Getting MC true signal: cand_type_ML>>1&1" << endl;
  bool cuttree = DrawTree(Form("drawnttree_%d.root",iterator),treename,varname.Data(),"cand_type_ML>>1&1");
  if(!cuttree) {
    cerr << "Impossible to cut tree. Please check input file and var name" << endl;
    return;
  }

  TFile *file = TFile::Open(Form("drawnttree_%d.root",iterator));
  TTree* tree = (TTree*)file->Get(treename.Data());
  TCanvas* cMC = new TCanvas();
  cMC->cd();
  tree->Draw(Form("%s",varname.Data()));
  TH1F *hMC = (TH1F*)gPad->GetPrimitive("htemp");
  file->Close();
  delete file;
  file = nullptr;

  //gSystem->Unlink(Form("drawnttree_%d.root",iterator));
 
////////////////////////////////////

  cout << "Getting Data sidebands: cand_type_ML>>1&0 && cand_type_ML>>2&0 && cand_type_ML>>3&0 && cand_type_ML>>4&0  INVMASS TO BE ADDED" << endl;
  bool cuttree2 = DrawTree(Form("drawnttree_%d.root",iterator),treename,varname.Data(),"cand_type_ML>>1&0");
  if(!cuttree2) {
    cerr << "Impossible to cut tree. Please check input file and var name" << endl;
    return;
  }

  TFile* file2 = TFile::Open(Form("drawnttree_%d.root",iterator));
  TTree* tree2 = (TTree*)file2->Get(treename.Data());
  TCanvas* cData = new TCanvas();
  cData->cd();
  tree2->Draw(Form("%s",varname.Data()));
  TH1F* hData = (TH1F*)gPad->GetPrimitive("htemp");
  file2->Close();
  delete file2;
  file2 = nullptr;

/////////////////////////////////

  gSystem->Unlink(Form("drawnttree_%d.root",iterator));
  cout << "\n" << endl;
  
  TFile outfile(outfilename.Data(), "recreate");
  cMC->Write();
  cData->Write();
  hMC->Write();
  hData->Write();
  outfile.Close();
  
  //outfilename.ReplaceAll(".root",".pdf");
  //cMC->SaveAs(outfilename.Data());
}

//____________________________________________________________
bool DrawTree(TString infilename, TString treename, TString varname, TString cut) {
  
  TFile* infile = TFile::Open(infilename.Data());
  if(!infile || !infile->IsOpen()) return false;
  TTree* tree = (TTree*)infile->Get(treename.Data());
  if(!tree) return false;
  
  TFile outfile(infilename.Data(),"recreate");
  TTree* treeCopy = tree->CopyTree(Form("%s",cut.Data()));
  treeCopy->Write();
  outfile.Close();
  
  return true;
}

//____________________________________________________________

int main(int argc, char *argv[])
{
  if((argc != 13))
  {
    std::cout << "Wrong number of inputs" << std::endl;
    return 1;
  }
  
  if(argc == 13)
    DrawHistoFromTrees(atoi(argv[1]),atoi(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5]),argv[6],argv[7],atof(argv[8]),atof(argv[9]),atof(argv[10]),atoi(argv[11]),argv[12]);
  return 0;
}
