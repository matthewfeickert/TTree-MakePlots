/*
   Author: Matthew Feickert <matthew.feickert@cern.ch>
   Tested on ROOT version: 6.07
 */

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TRandom.h>

void makeTree (const char *outputFile = "MyAna.root") {
  gROOT->Reset();
  Double_t Example1 { -99999. };
  Double_t Example2 { -99999. };

  TFile MyFile(outputFile, "recreate");

  TTree *MyTree { new TTree("MyTree", "MyTree") };

  MyTree->Branch("Example1", &Example1);
  MyTree->Branch("Example2", &Example2);

  for (Int_t i = 0; i < 10000; ++i) {
    gRandom->Rannor(Example1, Example2);
    MyTree->Fill();
  }

  MyTree->Write();

  if (MyFile.IsOpen()) {
    MyFile.cd();
    MyFile.Close();
  }
} // makeTree
