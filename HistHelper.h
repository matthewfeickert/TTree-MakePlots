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

Int_t SaveWithExtension (const TCanvas &c, const char *name, const char *extension) {
  TString fout { name };

  fout += ".";
  fout += extension;
  c.SaveAs(fout);
  return 0;
} // SaveWithExtensions

inline Int_t DrawAsPDF (TH1F *h, const char *outputName) {
  TCanvas *c { new TCanvas() };

  h->Draw();
  SaveWithExtension(*c, outputName, "pdf");

  return 0;
} // DrawAsPDF

inline Int_t DrawAsPDF (TH1F *h) {
  DrawAsPDF(h, h->GetName());

  return 0;
} // DrawAsPDF

inline Int_t DrawCOLZ (TH2F *h, const char *outputName) {
  TCanvas *c { new TCanvas() };

  h->Draw("COLZ");
  h->SetStats(kFALSE);
  c->SetRightMargin(0.13);
  SaveWithExtension(*c, outputName, "pdf");

  return 0;
} // DrawCOLZ

inline Int_t DrawCOLZ (TH2F *h) {
  DrawCOLZ(h, h->GetName());

  return 0;
} // DrawCOLZ

std::vector<TH1F*> CloneHists (const std::vector<TH1F*> v) {
  std::vector<TH1F*> h;
  h.reserve(v.size());

  for (Int_t i = 0; i < v.size(); ++i) {
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

  for (Int_t i = 0; i < v.size(); ++i) height.push_back(GetMaxValue(*v[i]));
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

  for (Int_t i = 0; i < h.size(); ++i) {
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
  TCanvas *c { new TCanvas() };
  TH1F    *h1 { (TH1F*)hist1.Clone(hist1.GetName()) };
  TH1F    *h2 { (TH1F*)hist2.Clone(hist2.GetName()) };

  SetHistOptions(*h1, xAxisTitle, yAxisTitle, TitleOffset);
  HistOverlay(*h1, *h2);

  TLegend *legend { new TLegend(0.72, 0.75, 0.9, 0.9) };
  legend->AddEntry(h1, h1->GetName(), "L");
  legend->AddEntry(h2, h2->GetName(), "L");
  legend->Draw();

  SaveWithExtension(*c, outputName, "pdf");

  return 0;
} // DrawOverlay

Int_t DrawOverlay (const std::vector<TH1F*> hist, const std::vector<Color_t> color,
                   const char *outputName, const char *xAxisTitle, const char *yAxisTitle,
                   Float_t TitleOffset) {
  TCanvas *c { new TCanvas() };

  std::vector<TH1F*> h { CloneHists(hist) };
  SetHistOptions(*h[0], xAxisTitle, yAxisTitle, TitleOffset);
  HistOverlay(h, color);

  SaveWithExtension(*c, outputName, "pdf");

  return 0;
} // DrawOverlay

Int_t DrawOverlay (const std::vector<TH1F*> hist, const std::vector<Color_t> color,
                   const std::vector<const char*> lName,
                   const std::vector<const char*> lOption, const char *outputName,
                   const char *xAxisTitle, const char *yAxisTitle, Float_t TitleOffset) {
  TCanvas *c { new TCanvas() };

  std::vector<TH1F*> h { CloneHists(hist) };
  SetHistOptions(*h[0], xAxisTitle, yAxisTitle, TitleOffset);
  HistOverlay(h, color);

  TLegend *legend { new TLegend(0.72, 0.75, 0.9, 0.9) };

  for (Int_t i = 0; i < h.size(); ++i) legend->AddEntry(h[i], lName[i], lOption[i]);
  legend->Draw();

  SaveWithExtension(*c, outputName, "pdf");

  return 0;
} // DrawOverlay

#endif // ifndef HISTHELPER_H
