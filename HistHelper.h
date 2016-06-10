#ifndef HISTHELPER_H
#define HISTHELPER_H

#include <iostream>
#include <TROOT.h>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1F.h>
#include <TH2F.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>

Int_t nBins (Int_t min, Int_t max, Int_t binning) {
  return TMath::CeilNint((max - min) / binning);
}

Int_t nBins (Int_t min, Int_t max, Double_t binning) {
  return TMath::CeilNint((max - min) / binning);
}

Int_t nBins (Int_t min, Double_t max, Double_t binning) {
  return TMath::CeilNint((max - static_cast<Double_t>(min)) / binning);
}

Int_t nBins (Double_t min, Int_t max, Double_t binning) {
  return TMath::CeilNint((static_cast<Double_t>(max) - min) / binning);
}

Int_t nBins (Double_t min, Double_t max, Double_t binning) {
  return TMath::CeilNint((max - min) / binning);
}

Int_t nBins (Double_t min, Double_t max, Int_t binning) {
  return TMath::CeilNint((max - min) / static_cast<Double_t>(binning));
}

Double_t GetMaxValue (const TH1F &h) {
  return h.GetBinContent(h.GetMaximumBin());
}

Double_t GetMinValue (const TH1F &h) {
  return h.GetBinContent(h.GetMinimumBin());
}

inline Int_t SetHistOptions (TH1F &h, const char *xAxisTitle, const char *yAxisTitle,
                             Float_t TitleOffset, Float_t Minimum = 0.) {
  h.SetFillColor(kWhite);
  h.GetXaxis()->SetTitle(xAxisTitle);
  h.GetYaxis()->SetTitle(yAxisTitle);
  h.GetYaxis()->SetTitleOffset(TitleOffset);
  h.SetMinimum(Minimum);

  return 0;
} // SetHistOptions

inline Int_t SetHistOptions (TH2F &h, const char *xAxisTitle, const char *yAxisTitle,
                             Float_t TitleOffset, Float_t Minimum = 0.) {
  h.SetFillColor(kWhite);
  h.GetXaxis()->SetTitle(xAxisTitle);
  h.GetYaxis()->SetTitle(yAxisTitle);
  h.GetYaxis()->SetTitleOffset(TitleOffset);
  h.SetMinimum(Minimum);

  return 0;
} // SetHistOptions

inline Int_t SetHistOptions (THStack &h, const char *xAxisTitle, const char *yAxisTitle,
                             Float_t TitleOffset, Float_t Minimum = 0.) {
  /*Can be called only AFTER Draw() has been called.*/
  h.GetXaxis()->SetTitle(xAxisTitle);
  h.GetYaxis()->SetTitle(yAxisTitle);
  h.GetYaxis()->SetTitleOffset(TitleOffset);
  h.SetMinimum(Minimum);

  return 0;
} // SetHistOptions

Int_t Normalize (TH1F &h, Double_t area = 1.) {
  Double_t norm { area / h.Integral() };

  h.Scale(norm);

  return 0;
} // Normalize

Int_t Normalize (TH2F &h, Double_t volume = 1.) {
  Double_t norm { volume / h.Integral() };

  h.Scale(norm);

  return 0;
} // Normalize

Int_t Normalize (std::vector<TH1F*> &h, Double_t area = 1.) {
  Double_t Total { 0. };

  for (UInt_t i = 0; i < h.size(); ++i) Total += h[i]->Integral();

  for (UInt_t i = 0; i < h.size(); ++i) h[i]->Scale(area / Total);

  return 0;
} // Normalize

Int_t SaveWithExtension (const TCanvas &c, const char *name, const char *extension) {
  TString fout { name };

  fout += ".";
  fout += extension;
  c.SaveAs(fout);
  return 0;
} // SaveWithExtensions

inline Int_t DrawAsPDF (TH1F *h, const char *outputName) {
  TCanvas *c { new TCanvas("c", "", 800, 600) };

  h->Draw();
  SaveWithExtension(*c, outputName, "pdf");

  delete c;
  c = nullptr;

  return 0;
} // DrawAsPDF

inline Int_t DrawAsPDF (TH1F *h) {
  DrawAsPDF(h, h->GetName());

  return 0;
} // DrawAsPDF

inline Int_t DrawCOLZ (TH2F *h, const char *outputName) {
  TCanvas *c { new TCanvas("c", "", 600, 600) };

  h->Draw("COLZ");
  h->SetStats(kFALSE);
  c->SetLeftMargin(0.11);
  c->SetRightMargin(0.15);
  SaveWithExtension(*c, outputName, "pdf");

  delete c;
  c = nullptr;

  return 0;
} // DrawCOLZ

inline Int_t DrawCOLZ (TH2F *h) {
  DrawCOLZ(h, h->GetName());

  return 0;
} // DrawCOLZ

std::vector<TH1F*> CloneHists (const std::vector<TH1F*> v) {
  std::vector<TH1F*> h;
  h.reserve(v.size());

  for (UInt_t i = 0; i < v.size(); ++i) {
    h.push_back((TH1F*)v[i]->Clone(v[i]->GetName()));
  }
  return h;
} // CloneHists

Double_t GetHistHeight (const TH1F &h1, const TH1F &h2) {
  Double_t MaxValue { GetMaxValue(h1) > GetMaxValue(h2)
                      ? GetMaxValue(h1) : GetMaxValue(h2) };
  Double_t NextValue { GetMaxValue(h1) > GetMaxValue(h2)
                       ? GetMaxValue(h2) : GetMaxValue(h1) };

  if (NextValue < MaxValue / 2.) return MaxValue + NextValue / 3.;
  else return (3. * MaxValue - NextValue) / 2.;
} // GetHistHeight

Double_t GetHistHeight (const std::vector<TH1F*> &v) {
  std::vector<Double_t> height;
  height.reserve(v.size());

  for (UInt_t i = 0; i < v.size(); ++i) height.push_back(GetMaxValue(*v[i]));
  std::sort(height.begin(), height.end(), std::greater<Double_t>());
  Double_t MaxValue { height[0] };
  Double_t NextValue { height[1] };

  if (NextValue < MaxValue / 2.) return MaxValue + NextValue / 3.;
  else return (3. * MaxValue - NextValue) / 2.;
} // GetHistHeight

Int_t HistOverlay (TH1F &h1, TH1F &h2) {
  h1.SetMaximum(GetHistHeight(h1, h2));
  h1.SetLineColor(kBlack);
  h2.SetLineColor(kBlue);
  h1.SetStats(kFALSE);
  h2.SetStats(kFALSE);
  h1.Draw();
  gPad->Update();
  h2.Draw("SAME");
  gPad->Update();

  return 0;
} // HistOverlay

Int_t HistOverlay (std::vector<TH1F*> h, std::vector<Color_t> color) {
  h[0]->SetMaximum(GetHistHeight(h));

  for (UInt_t i = 0; i < h.size(); ++i) {
    h[i]->SetLineColor(color[i]);
    h[i]->SetStats(kFALSE);
    h[i]->Draw("SAME");
    gPad->Update();
  }
  return 0;
} // HistOverlay

Int_t DrawOverlay (const TH1F &hist1, const TH1F &hist2, const char *outputName,
                   const char *xAxisTitle, const char *yAxisTitle,
                   Float_t TitleOffset) {
  TCanvas *c { new TCanvas("c", "", 800, 600) };
  TH1F    *h1 { (TH1F*)hist1.Clone(hist1.GetName()) };
  TH1F    *h2 { (TH1F*)hist2.Clone(hist2.GetName()) };

  SetHistOptions(*h1, xAxisTitle, yAxisTitle, TitleOffset);
  HistOverlay(*h1, *h2);

  TLegend *legend { new TLegend(0.72, 0.75, 0.9, 0.9) };
  legend->AddEntry(h1, h1->GetName(), "L");
  legend->AddEntry(h2, h2->GetName(), "L");
  legend->Draw();

  SaveWithExtension(*c, outputName, "pdf");

  delete c;
  c = nullptr;
  delete legend;
  legend = nullptr;

  return 0;
} // DrawOverlay

Int_t DrawOverlay (const std::vector<TH1F*> hist, const std::vector<Color_t> color,
                   const char *outputName, const char *xAxisTitle, const char *yAxisTitle,
                   Float_t TitleOffset) {
  TCanvas *c { new TCanvas("c", "", 800, 600) };

  std::vector<TH1F*> h { CloneHists(hist) };
  SetHistOptions(*h[0], xAxisTitle, yAxisTitle, TitleOffset);
  HistOverlay(h, color);

  SaveWithExtension(*c, outputName, "pdf");

  delete c;
  c = nullptr;

  return 0;
} // DrawOverlay

Int_t DrawOverlay (const std::vector<TH1F*> hist, const std::vector<Color_t> color,
                   const std::vector<const char*> lName,
                   const std::vector<const char*> lOption, const char *outputName,
                   const char *xAxisTitle, const char *yAxisTitle, Float_t TitleOffset) {
  TCanvas *c { new TCanvas("c", "", 800, 600) };

  std::vector<TH1F*> h { CloneHists(hist) };
  SetHistOptions(*h[0], xAxisTitle, yAxisTitle, TitleOffset);
  HistOverlay(h, color);

  /*Could just use c->BuildLegend(0.72, 0.75, 0.9, 0.9) but I think the legend looks
     nicer and is worth two extra arguments.*/
  TLegend *legend { new TLegend(0.72, 0.75, 0.9, 0.9) };

  for (UInt_t i = 0; i < h.size(); ++i) legend->AddEntry(h[i], lName[i], lOption[i]);
  legend->Draw();

  SaveWithExtension(*c, outputName, "pdf");

  delete c;
  c = nullptr;
  delete legend;
  legend = nullptr;

  return 0;
} // DrawOverlay

Bool_t AreaCompare (const TH1F *a, const TH1F *b) {
  return a->Integral() > b->Integral();
}

Bool_t SameNumberBins (const std::vector<TH1F*> &v) {
  Int_t nbins { v[0]->GetSize() };

  for (UInt_t i = 0; i < v.size(); ++i) {
    if (nbins != v[i]->GetSize()) return kFALSE;
  }

  return kTRUE;
} // SameNumberBins

Int_t RebinHists (std::vector<TH1F*> &h) {
  std::vector<Int_t> numberBins;
  numberBins.clear();

  for (UInt_t i = 0; i < h.size(); ++i) numberBins.push_back(h[i]->GetSize() - 2);
  std::sort(numberBins.begin(), numberBins.end());

  for (UInt_t i = 0; i < h.size(); ++i) h[i]->Rebin(numberBins[0]);

  return 0;
} // RebinHists

THStack* HistStack (std::vector<TH1F*> &h, std::vector<Color_t> color) {
  THStack *hs { new THStack() };

  // if (!SameNumberBins(h)) RebinHists(h); //BROKEN
  std::stable_sort(h.begin(), h.end(), AreaCompare);

  for (UInt_t i = 0; i < h.size(); ++i) {
    h[i]->SetLineColor(kBlack);
    h[i]->SetFillColor(color[i]);
    h[i]->SetMarkerColor(color[i]);
    h[i]->SetStats(kFALSE);
    hs->Add(h[i]);
  }
  return hs;
} // HistStack

Int_t DrawStacked (const std::vector<TH1F*> hist, const std::vector<Color_t> color,
                   const char *outputName, const char *xAxisTitle, const char *yAxisTitle,
                   Float_t TitleOffset, Bool_t MakeNormalized = kFALSE) {
  TCanvas *c { new TCanvas("c", "", 800, 600) };

  std::vector<TH1F*> h { CloneHists(hist) };

  if (MakeNormalized) Normalize(h);
  THStack *hs { HistStack(h, color) };
  hs->Draw();
  SetHistOptions(*hs, xAxisTitle, yAxisTitle, TitleOffset);
  c->BuildLegend(0.72, 0.75, 0.9, 0.9);

  SaveWithExtension(*c, outputName, "pdf");

  delete c;
  c = nullptr;
  delete hs;
  hs = nullptr;

  return 0;
} // DrawStacked

#endif // ifndef HISTHELPER_H
