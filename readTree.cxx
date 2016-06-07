// Author: Matthew Feickert <matthew.feickert@cern.ch>
#include "HistHelper.h"

void readTree (const char *inputFile = "MyAna.root",
               const char *outputFile = "histograms.root",
               Bool_t      DrawPlots = kTRUE) {
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
  Int_t nEntries { (Int_t)MyTree->GetEntries() };

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
  TH1F *h_Example3 { new TH1F("Example3", "", nBins(-4, 4, 0.2), -4, 4) };
  MyTree->Project(h_Example3->GetName(), "Example2", "");

  TH2F *h_Matrix { new TH2F("Matrix", "", nBins(-4, 4, 0.5), -4, 4,
                            nBins(-4, 4, 0.5), -4, 4) };
  MyTree->Project(h_Matrix->GetName(), "Example2:Example1", "");

  // ###------------------------
  // Histogram plotting options
  // ###------------------------
  // Set the axis and plot titles
  SetHistOptions(*h_Example1, "value [units]", "Fraction of Total Events/1 units",   1.5);
  SetHistOptions(*h_Example2, "value [units]", "Fraction of Total Events/0.5 units", 1.5);
  SetHistOptions(*h_Example3, "value [units]", "Fraction of Total Events/0.1 units", 1.5);

  SetHistOptions(*h_Matrix,
                 "value [units]",
                 "value [units]",
                 1.5,
                 -4.);
  h_Matrix->SetOption("COLZ");

  // ###------------------------
  //  Normalize histograms to unit area
  // ###------------------------
  std::vector<TH1F*> histlist = { h_Example1, h_Example2, h_Example3 };
  std::vector<TH1F*> unnorm { CloneHists(histlist) };
  Normalize(*h_Example1);
  Normalize(*h_Example2);
  Normalize(*h_Example3);

  // save the histograms to a root file for later access
  // ###------------------------
  // Write all histograms to .root file
  // ###------------------------
  TFile *histFile { new TFile(outputFile, "recreate") };

  h_Example1->Write();
  h_Example2->Write();
  h_Example3->Write();
  h_Matrix->Write();

  histFile->Close();

  if (DrawPlots) {
    DrawAsPDF(h_Example1);
    DrawAsPDF(h_Example2);
    DrawAsPDF(h_Example3);
    DrawCOLZ(h_Matrix);
    DrawOverlay(*h_Example1, *h_Example2, "overlay2Plots", "value [units]",
                "Fraction of Total Events/Bin", 1.5);

    std::vector<TH1F*>   hist        = { h_Example1, h_Example2, h_Example3 };
    std::vector<Color_t> color       = { kBlack, kBlue, kGreen };
    std::vector<const char*> legName = { hist[0]->GetName(), hist[1]->GetName(),
                                         hist[2]->GetName() };
    std::vector<const char*> legOption = { "L", "L", "L" };
    DrawOverlay(hist, color, "overlay3aPlots", "value [units]",
                "Fraction of Total Events/Bin", 1.5);
    DrawOverlay(hist, color, legName,          legOption, "overlay3bPlots",
                "value [units]", "Fraction of Total Events/Bin", 1.5);
    color.clear();
    color = { kBlue, kRed, kGreen };
    DrawStacked(unnorm, color, "stackedPlots", "value [units]", "Events/Bin", 1.5);
    DrawStacked(unnorm, color, "stackedNorm",  "value [units]",
                "Fraction of Total Events/Bin", 1.5, kTRUE);
  }

  // ###------------------------
  // Free memory
  // ###------------------------
  delete h_Example1;
  delete h_Example2;
  delete h_Example3;

  h_Example1 = nullptr;
  h_Example2 = nullptr;
  h_Example3 = nullptr;
} // readTree
