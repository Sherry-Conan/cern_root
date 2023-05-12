//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Apr 18 18:54:05 2023 by ROOT version 6.28/02
// from TTree tree/tree
// found on file: /home/myusername/code/homework6/data_16C.root
//////////////////////////////////////////////////////////

#ifndef dssd_h
#define dssd_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class dssd {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        ppac1[3];
   Double_t        ppac2[3];
   Double_t        ppac3[3];
   Double_t        tarpos[3];
   Int_t           d1x[32];
   Int_t           d1y[32];
   Double_t        d1t[32];
   Int_t           d2x[32];
   Int_t           d2y[32];
   Double_t        d2t[32];
   Int_t           d3x[32];
   Int_t           d3y[32];
   Double_t        d3t[32];
   Int_t           d1xhit;
   Int_t           d1xs[13];   //[d1xhit]
   Int_t           d1xe[13];   //[d1xhit]
   Double_t        d1xt[13];   //[d1xhit]
   Int_t           d1yhit;
   Int_t           d1ys[13];   //[d1yhit]
   Int_t           d1ye[13];   //[d1yhit]
   Int_t           d2xhit;
   Int_t           d2xs[8];   //[d2xhit]
   Int_t           d2xe[8];   //[d2xhit]
   Double_t        d2xt[8];   //[d2xhit]
   Int_t           d2yhit;
   Int_t           d2ys[8];   //[d2yhit]
   Int_t           d2ye[8];   //[d2yhit]
   Int_t           d3xhit;
   Int_t           d3xs[6];   //[d3xhit]
   Int_t           d3xe[6];   //[d3xhit]
   Double_t        d3xt[6];   //[d3xhit]
   Int_t           d3yhit;
   Int_t           d3ys[8];   //[d3yhit]
   Int_t           d3ye[8];   //[d3yhit]
   Double_t        d1z;
   Double_t        d2z;
   Double_t        d3z;
   Double_t        s1e;
   Double_t        s2e;
   Double_t        s3e;
   Double_t        c1e;
   Double_t        c2e;
   Double_t        c3e;
   Double_t        c4e;
   Int_t           run;
   Long64_t        nevt;

   // List of branches
   TBranch        *b_ppac1;   //!
   TBranch        *b_ppac2;   //!
   TBranch        *b_ppac3;   //!
   TBranch        *b_tarpos;   //!
   TBranch        *b_d1x;   //!
   TBranch        *b_d1y;   //!
   TBranch        *b_d1t;   //!
   TBranch        *b_d2x;   //!
   TBranch        *b_d2y;   //!
   TBranch        *b_d2t;   //!
   TBranch        *b_d3x;   //!
   TBranch        *b_d3y;   //!
   TBranch        *b_d3t;   //!
   TBranch        *b_d1xhit;   //!
   TBranch        *b_d1xs;   //!
   TBranch        *b_d1xe;   //!
   TBranch        *b_d1xt;   //!
   TBranch        *b_d1yhit;   //!
   TBranch        *b_d1ys;   //!
   TBranch        *b_d1ye;   //!
   TBranch        *b_d2xhit;   //!
   TBranch        *b_d2xs;   //!
   TBranch        *b_d2xe;   //!
   TBranch        *b_d2xt;   //!
   TBranch        *b_d2yhit;   //!
   TBranch        *b_d2ys;   //!
   TBranch        *b_d2ye;   //!
   TBranch        *b_d3xhit;   //!
   TBranch        *b_d3xs;   //!
   TBranch        *b_d3xe;   //!
   TBranch        *b_d3xt;   //!
   TBranch        *b_d3yhit;   //!
   TBranch        *b_d3ys;   //!
   TBranch        *b_d3ye;   //!
   TBranch        *b_d1z;   //!
   TBranch        *b_d2z;   //!
   TBranch        *b_d3z;   //!
   TBranch        *b_s1e;   //!
   TBranch        *b_s2e;   //!
   TBranch        *b_s3e;   //!
   TBranch        *b_c1e;   //!
   TBranch        *b_c2e;   //!
   TBranch        *b_c3e;   //!
   TBranch        *b_c4e;   //!
   TBranch        *b_run;   //!
   TBranch        *b_nevt;   //!

   dssd(TTree *tree=0);
   virtual ~dssd();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef dssd_cxx
dssd::dssd(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/myusername/code/homeworksix/data_16C.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/home/myusername/code/homeworksix/data_16C.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

dssd::~dssd()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t dssd::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t dssd::LoadTree(Long64_t entry)
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

void dssd::Init(TTree *tree)
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

   fChain->SetBranchAddress("ppac1", ppac1, &b_ppac1);
   fChain->SetBranchAddress("ppac2", ppac2, &b_ppac2);
   fChain->SetBranchAddress("ppac3", ppac3, &b_ppac3);
   fChain->SetBranchAddress("tarpos", tarpos, &b_tarpos);
   fChain->SetBranchAddress("d1x", d1x, &b_d1x);
   fChain->SetBranchAddress("d1y", d1y, &b_d1y);
   fChain->SetBranchAddress("d1t", d1t, &b_d1t);
   fChain->SetBranchAddress("d2x", d2x, &b_d2x);
   fChain->SetBranchAddress("d2y", d2y, &b_d2y);
   fChain->SetBranchAddress("d2t", d2t, &b_d2t);
   fChain->SetBranchAddress("d3x", d3x, &b_d3x);
   fChain->SetBranchAddress("d3y", d3y, &b_d3y);
   fChain->SetBranchAddress("d3t", d3t, &b_d3t);
   fChain->SetBranchAddress("d1xhit", &d1xhit, &b_d1xhit);
   fChain->SetBranchAddress("d1xs", d1xs, &b_d1xs);
   fChain->SetBranchAddress("d1xe", d1xe, &b_d1xe);
   fChain->SetBranchAddress("d1xt", d1xt, &b_d1xt);
   fChain->SetBranchAddress("d1yhit", &d1yhit, &b_d1yhit);
   fChain->SetBranchAddress("d1ys", d1ys, &b_d1ys);
   fChain->SetBranchAddress("d1ye", d1ye, &b_d1ye);
   fChain->SetBranchAddress("d2xhit", &d2xhit, &b_d2xhit);
   fChain->SetBranchAddress("d2xs", d2xs, &b_d2xs);
   fChain->SetBranchAddress("d2xe", d2xe, &b_d2xe);
   fChain->SetBranchAddress("d2xt", d2xt, &b_d2xt);
   fChain->SetBranchAddress("d2yhit", &d2yhit, &b_d2yhit);
   fChain->SetBranchAddress("d2ys", d2ys, &b_d2ys);
   fChain->SetBranchAddress("d2ye", d2ye, &b_d2ye);
   fChain->SetBranchAddress("d3xhit", &d3xhit, &b_d3xhit);
   fChain->SetBranchAddress("d3xs", d3xs, &b_d3xs);
   fChain->SetBranchAddress("d3xe", d3xe, &b_d3xe);
   fChain->SetBranchAddress("d3xt", d3xt, &b_d3xt);
   fChain->SetBranchAddress("d3yhit", &d3yhit, &b_d3yhit);
   fChain->SetBranchAddress("d3ys", d3ys, &b_d3ys);
   fChain->SetBranchAddress("d3ye", d3ye, &b_d3ye);
   fChain->SetBranchAddress("d1z", &d1z, &b_d1z);
   fChain->SetBranchAddress("d2z", &d2z, &b_d2z);
   fChain->SetBranchAddress("d3z", &d3z, &b_d3z);
   fChain->SetBranchAddress("s1e", &s1e, &b_s1e);
   fChain->SetBranchAddress("s2e", &s2e, &b_s2e);
   fChain->SetBranchAddress("s3e", &s3e, &b_s3e);
   fChain->SetBranchAddress("c1e", &c1e, &b_c1e);
   fChain->SetBranchAddress("c2e", &c2e, &b_c2e);
   fChain->SetBranchAddress("c3e", &c3e, &b_c3e);
   fChain->SetBranchAddress("c4e", &c4e, &b_c4e);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("nevt", &nevt, &b_nevt);
   Notify();
}

Bool_t dssd::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void dssd::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t dssd::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef dssd_cxx