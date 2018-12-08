#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"
#include "TNtuple.h"
#include "TFile.h"

#include <iostream>
#include <vector>

using namespace std;

void DoPlots(TNtuple* ntuple, TString str, TCanvas* cDist, TCanvas* cEff, int ipad);
void PlotTest()
{
    TString stroutdir = "../output_Nevents50000_BinaryClassificationDs_dfselection_pt_cand_ML_2.0_4.0";
    vector<TString> models = {"ScikitTreeGradientBoostingClassifier","ScikitTreeRandom_Forest","ScikitTreeAdaBoost","ScikitTreeDecision_Tree","XGBoostXGBClassifier"};
    TString inputfilename(TString::Format("%s/testsampleNevents50000_BinaryClassificationDs_dfselection_pt_cand_ML_2.0_4.0MLdecision.root",stroutdir.Data()));
    TFile* inputfile = TFile::Open(inputfilename.Data());

    TString tuplename("fTreeDsFlaggedTested");
    TNtuple* ntpl = (TNtuple*) inputfile->Get(tuplename.Data());
    cout << "\n=== input TNtuple: " << ntpl << endl<<endl;

    //make plots
    TCanvas* canDist = new TCanvas("canDist","canDist",2000,800);
    TCanvas* canEff  = new TCanvas("canEff","canEff",2000,800);
    canDist->Divide(3,2);
    canEff->Divide(3,2);
    
    for(int i = 0; i < models.size(); i++)
    {
        TString model = models.at(i);
        cout << "--- " << model.Data() << endl;
        DoPlots(ntpl,model,canDist,canEff,i+1);
    }
    
    canDist->SaveAs(TString::Format("%s/modelstested_probs.pdf",stroutdir.Data()));
    canEff->SaveAs(TString::Format("%s/modelstested_eff.pdf",stroutdir.Data()));

    return;
}

void DoPlots(TNtuple* ntuple, TString str, TCanvas* cDist, TCanvas* cEff, int ipad)
{
    // 1st canvas: algorithm probability
    TString leafIsSig = "signal_ML";
    TString leafprob = "y_test_prob";
    leafprob+=str;
    
    TString histname_sig = TString::Format("hprob_sig_%s",str.Data());
    TString histname_bkg = TString::Format("hprob_bkg_%s",str.Data());
    TString strDrawSig = TString::Format("%s>>%s(100,0,1)",leafprob.Data(),histname_sig.Data());
    TString strDrawBkg = TString::Format("%s>>%s(100,0,1)",leafprob.Data(),histname_bkg.Data());
    TString strConditionSig = TString::Format("%s>0.9",leafIsSig.Data());
    TString strConditionBkg = TString::Format("%s<0.1",leafIsSig.Data());

    cDist->cd(ipad);
    gPad->SetLogy();
    ntuple->Draw(strDrawSig.Data(),strConditionSig.Data());
    ntuple->Draw(strDrawBkg.Data(),strConditionBkg.Data(),"same");
    TH1F* hprob_sig = (TH1F*) gDirectory->Get(histname_sig.Data());
    TH1F* hprob_bkg = (TH1F*) gDirectory->Get(histname_bkg.Data());

    hprob_sig->SetStats(kFALSE);
    hprob_sig->GetYaxis()->SetRangeUser(1e-01,1e4);
    hprob_sig->SetFillStyle(3005);
    hprob_bkg->SetFillStyle(3004);
    hprob_sig->SetLineColor(kRed);
    hprob_sig->SetFillColor(kRed);
    hprob_bkg->SetFillColor(kBlue);
    hprob_sig->SetTitle(str.Data());
    hprob_sig->GetXaxis()->SetTitle("ML probability");
    hprob_sig->GetYaxis()->SetTitle("counts");

    TLegend* legDist = new TLegend(0.3,0.7,0.7,0.8);
    legDist->SetBorderSize(0);
    legDist->AddEntry(hprob_sig,"signal");
    legDist->AddEntry(hprob_bkg,"background");
    legDist->Draw();

    // 2nd canvas: signal selection efficiency == sensitivity == recall & fraction of selected true signal == precision
    TH1F* hEffSig = (TH1F*) hprob_sig->Clone();
    hEffSig->SetMarkerStyle(4);
    hEffSig->SetLineColor(kOrange+10);
    hEffSig->SetLineWidth(3);
    hEffSig->SetFillStyle(0);
    hEffSig->SetFillColor(kWhite);
    hEffSig->GetYaxis()->SetRangeUser(0,1.3);
    hEffSig->GetYaxis()->SetTitle("");

    TH1F* hPrecision = (TH1F*) hEffSig->Clone();
    
    int nBins  = hEffSig->GetNbinsX();
    int intTot = hEffSig->Integral();
    
    for(int ibin = 1; ibin < nBins; ibin++)  
    {
        double TP = hprob_sig->Integral(ibin,nBins)*1.;     // true positive
        double FP = hprob_bkg->Integral(ibin,nBins)*1.;     // false positive

        if(TP<0.001)    continue;

        hEffSig->SetBinContent(ibin,TP/intTot);
        hPrecision->SetBinContent(ibin, (TP)/(TP+FP) );
        
        //cout << "= " << TP << endl;
        //cout << "== " << FP << endl;
        //cout << "=== " << (TP)/(TP+FP) << endl;
    }

    hPrecision->SetLineColor(kAzure);
    hPrecision->SetLineStyle(3);

    cEff->cd(ipad);
    hEffSig->Draw("l");
    hPrecision->Draw("lsame");

    TLegend* legEff = new TLegend(0.3,0.3,0.7,0.5);
    legEff->SetBorderSize(0);
    legEff->AddEntry(hEffSig,"signal efficiency = sensitivity","l");
    legEff->AddEntry(hPrecision,"precision","l");
    legEff->Draw();

    return;
}
