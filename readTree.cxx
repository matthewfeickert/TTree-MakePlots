// Author: Matthew Feickert <matthew.feickert@cern.ch>

Int_t nBins (Int_t min, Int_t max, Int_t binning) {
  return TMath::CeilNint((max - min) / binning);
}

Int_t nBins (Int_t min, Int_t max, Double_t binning) {
  return TMath::CeilNint((max - min) / binning);
}

Int_t nBins (Double_t min, Double_t max, Double_t binning) {
  return TMath::CeilNint((max - min) / binning);
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
  Double_t norm = 1 / h.Integral();

  h.Scale(norm);

  return 0;
} // NormalizeToUnity

Int_t NormalizeToUnity (TH2F &h) {
  Double_t norm = 1 / h.Integral();

  h.Scale(norm);

  return 0;
} // NormalizeToUnity

inline Int_t DrawAsPDF (TH1F *h, const char *outputName) {
  TCanvas *c { new TCanvas() };
  TString  fout = outputName;

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
  TString  fout = outputName;

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

void readTree (const char *inputFile = "MyAna.root",
               const char *outputFile = "histograms.root",
               Bool_t      DrawPlots = kFALSE) {
  // Load the TTree creaed from the xAOD
  gROOT->Reset();

  TFile *file { new TFile(inputFile) };
  TTree *MyTree { (TTree*)file->Get("MyTree") };

  // ### Long method

  // access the branches of the TTree
  static double Example1;

  TBranch *b_Example1 { MyTree->GetBranch("Example1") };
  b_Example1->SetAddress(&Example1);

  // Make and fill histograms
  // ###------------------------
  // Define all histograms
  // ###------------------------
  TH1F *h_Example1 { new TH1F("Example1", "", nBins(-4, 4, 1), -4, 4) };

  // ###------------------------
  // Fill all histograms
  // ###------------------------
  Int_t nEntries = (Int_t)MyTree->GetEntries();

  for (Int_t i = 0; i < nEntries; i++) {
    // branches
    b_Example1->GetEntry(i);

    // histograms
    h_Example1->Fill(Example1);
  }

  // ### Long method

  // ### Short method

  TH1F *h_Example2 { new TH1F("Example2", "", nBins(0, 4, 0.5), 0, 4) };
  MyTree->Project(h_Example2->GetName(), "Example2", "Example2 >= 0.");

  // ### Short method

  TH2F *h_Matrix { new TH2F("Matrix", "", nBins(-4, 4, 0.5), -4, 4,
                            nBins(-4, 4, 0.5), -4, 4) };
  MyTree->Project(h_Matrix->GetName(), "Example2:Example1", "");

  // ###------------------------
  // Histogram plotting options
  // ###------------------------
  // Set the axis and plot titles
  SetHistOptions(*h_Example1, "value [units]", "Fraction of Total Events/1 units",   1.5);
  SetHistOptions(*h_Example2, "value [units]", "Fraction of Total Events/0.5 units", 1.5);

  SetHistOptions(*h_Matrix,
                 "value [units]",
                 "value [units]",
                 1.5,
                 -4.);
  h_Matrix->SetOption("COLZ");

  // ###------------------------
  //  Normalize histograms to unit area
  // ###------------------------
  NormalizeToUnity(*h_Example1);
  NormalizeToUnity(*h_Example2);

  // save the histograms to a root file for later access
  // ###------------------------
  // Write all histograms to .root file
  // ###------------------------
  TFile *histFile { new TFile(outputFile, "recreate") };

  h_Example1->Write();
  h_Example2->Write();
  h_Matrix->Write();

  histFile->Close();

  if (DrawPlots) {
    DrawAsPDF(h_Example1);
    DrawAsPDF(h_Example2);
    DrawCOLZ(h_Matrix);
  }
} // readTree
