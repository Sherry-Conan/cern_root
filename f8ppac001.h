#ifndef f8ppac001_h
#define f8ppac001_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2D.h>

// Header file for the classes stored in the TTree if any.

class f8ppac001 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         PPACF8[5][5]; 
   Float_t         F8PPACRawData[5][5]; 
   Int_t           beamTrig; 
   Int_t           must2Trig; 
   Float_t         targetX; 
   Float_t         targetY; 

   // List of branches
   TBranch        *b_PPACF8;   //!
   TBranch        *b_F8PPACRawData;   //!
   TBranch        *b_beamTrig;   //!
   TBranch        *b_must2Trig;   //!
   TBranch        *b_targetX;   //!
   TBranch        *b_targetY;   //!

   Double_t x[5], y[5], z_x[5], z_y[5], t[5], Fitxb, Fitxk, Fityb, Fityk, check[5], ndfy, ndfx, chix, chiy, dx[5], dy[5]; 
   f8ppac001(TTree *tree=0); 

   virtual ~f8ppac001(); 
   virtual Int_t    Cut(Long64_t entry); 
   virtual Int_t    GetEntry(Long64_t entry); 
   virtual Long64_t LoadTree(Long64_t entry); 
   virtual void     Init(TTree *tree); 
   virtual void     Loop(); 
   virtual Bool_t   Notify(); 
   virtual void     Show(Long64_t entry = -1); 
   virtual Int_t    Check(Int_t i); 
   virtual void     SetBranch(TTree *Data); 
   virtual Bool_t   coculate(TH2D *hForzx, Int_t i, Int_t k); 
   virtual void     aTrace(TH2D *h, Double_t k, Double_t b, Int_t max = 0, Int_t min = -1800); 
};

#endif

#ifdef f8ppac001_cxx
f8ppac001::f8ppac001(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("f8ppac001.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("f8ppac001.root"); 
      }
      f->GetObject("tree",tree);
   }
   Init(tree);
}

f8ppac001::~f8ppac001()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t f8ppac001::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t f8ppac001::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void f8ppac001::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("PPACF8", PPACF8, &b_PPACF8);
   fChain->SetBranchAddress("F8PPACRawData", F8PPACRawData, &b_F8PPACRawData);
   fChain->SetBranchAddress("beamTrig", &beamTrig, &b_beamTrig);
   fChain->SetBranchAddress("must2Trig", &must2Trig, &b_must2Trig);
   fChain->SetBranchAddress("targetX", &targetX, &b_targetX);
   fChain->SetBranchAddress("targetY", &targetY, &b_targetY);
   Notify();
}

Bool_t f8ppac001::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void f8ppac001::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t f8ppac001::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef f8ppac001_cxx
