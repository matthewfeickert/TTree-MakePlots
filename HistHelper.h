#ifndef HISTHELPER_H
#define HISTHELPER_H

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

Int_t NormalizeToUnity (TH1F &h) {
  Double_t norm { 1. / h.Integral() };

  h.Scale(norm);

  return 0;
} // NormalizeToUnity

Int_t NormalizeToUnity (TH2F &h) {
  Double_t norm { 1. / h.Integral() };

  h.Scale(norm);

  return 0;
} // NormalizeToUnity

inline Int_t DrawAsPDF (TH1F *h, const char *outputName) {
  TCanvas *c { new TCanvas() };
  TString  fout { outputName };

  fout += ".pdf";
  h->Draw();
  c->SaveAs(fout);

  return 0;
} // DrawAsPDF

inline Int_t DrawAsPDF (TH1F *h) {
  DrawAsPDF(h, h->GetName());

  return 0;
} // DrawAsPDF

inline Int_t DrawCOLZ (TH2F *h, const char *outputName) {
  TCanvas *c { new TCanvas() };
  TString  fout { outputName };

  fout += ".pdf";
  h->Draw("COLZ");
  h->SetStats(kFALSE);
  c->SetRightMargin(0.13);
  c->SaveAs(fout);

  return 0;
} // DrawCOLZ

inline Int_t DrawCOLZ (TH2F *h) {
  DrawCOLZ(h, h->GetName());

  return 0;
} // DrawCOLZ

Int_t DrawOverlay (const TH1F &hist1, const TH1F &hist2, const char *outputName,
                   const char *xAxisTitle, const char *yAxisTitle,
                   Float_t TitleOffset) {
  TCanvas *c { new TCanvas() };
  TH1F    *h1 { (TH1F*)hist1.Clone(hist1.GetName()) };
  TH1F    *h2 { (TH1F*)hist2.Clone(hist2.GetName()) };
  TString  fout { outputName };

  fout += ".pdf";

  SetHistOptions(*h1, xAxisTitle, yAxisTitle, TitleOffset);
  Double_t Maximum { GetMaxValue(*h1) > GetMaxValue(*h2)
                     ? GetMaxValue(*h1) : GetMaxValue(*h2) };
  Double_t Minimum { GetMaxValue(*h1) > GetMaxValue(*h2)
                     ? GetMaxValue(*h2) : GetMaxValue(*h1) };
  h1->SetMaximum((3. * Maximum - Minimum) / 2.);
  h1->SetLineColor(kBlack);
  h2->SetLineColor(kBlue);
  h1->SetStats(kFALSE);
  h2->SetStats(kFALSE);
  h1->Draw();
  gPad->Update();
  h2->Draw("SAME");
  gPad->Update();

  TLegend *legend { new TLegend(0.72, 0.75, 0.9, 0.9) };
  legend->AddEntry(h1, h1->GetName(), "L");
  legend->AddEntry(h2, h2->GetName(), "L");
  legend->Draw();

  c->SaveAs(fout);

  return 0;
} // DrawOverlay

#endif // ifndef HISTHELPER_H
