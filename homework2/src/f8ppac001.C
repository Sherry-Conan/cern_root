#define f8ppac001_cxx
#include "f8ppac001.h"
void f8ppac001::Loop(){
   TH2D *hForzx = new TH2D("hForzx","Fit z_{x} to x by ppac1A, ppac2A and ppac3",2200, -2000, 200, 300, -150, 150); 
   TH2D *hForzy = new TH2D("hForzy","Fit z_{y} to y by ppac1A, ppac2A and ppac3",2200, -2000, 200, 300, -150, 150); 
   Long64_t nentries = fChain->GetEntriesFast(); 

   TFile *File = new TFile("tracking.root", "recreate"); 
   TTree *Data = new TTree("tree", "ppac traking"); 
   SetBranch(Data); 
   for (Long64_t jentry = 0; jentry < nentries; jentry++){
      fChain->GetEntry(jentry); 
      for(Int_t l = 0; l < 5; l++){//提前进行
         x[l]    = PPACF8[l][0]; 
         y[l]    = PPACF8[l][1]; 
         z_y[l]  = PPACF8[l][3]; 
         t[l]    = PPACF8[l][4]; 
         z_x[l]  = PPACF8[l][2]; 
      }
      k_x = Check(x); k_y = Check(y); 
      Bool_t BoolForxFill = calculate(hForzx, k_x, x, z_x, dx, &Fitxb, &Fitxk, &chix, &ndfx); 
      Bool_t BoolForyFill = calculate(hForzy, k_y, y, z_y, dy, &Fityb, &Fityk, &chiy, &ndfy); 
      if(BoolForxFill || BoolForyFill)   Data -> Fill(); 
   }
   Data -> Write(); 
   hForzx -> Write(); 
   hForzy -> Write(); 
   File -> Close(); 
}
void f8ppac001::aTrace(TH2D *h, Double_t k, Double_t b, Int_t max, Int_t min){
   if(min > max) return;
   for(; min < max; min++){
      h -> Fill(min , k * min + b); 
   }
}
void f8ppac001::SetBranch(TTree *Data){
   TString List[] = {"x", "y", "t", "dx", "dy", "z_x", "z_y", "Fitxk", "Fitxb", "Fityk", "Fityb", "targetX", "targetY", "chix", "chiy", "ndfx", "ndfy", "beamTrig", "must2Trig", "k_x", "k_y"}; 
   Double_t *Ptr[] = {x, y, t, dx, dy, z_x, z_y, &Fitxk, &Fitxb, &Fityk, &Fityb, &targetX, &targetY, &chix, &chiy, &ndfx, &ndfy}; 
   Int_t *IntP[] = {&beamTrig, &must2Trig, &k_x, &k_y}; 
   for(Int_t j = 0; j < 7; j++)     Data -> Branch(List[j], Ptr[j], List[j]+"[5]/D"); 
   for(Int_t j = 0; j < 10; j++)    Data -> Branch(List[j+7], Ptr[j+7], List[j+7]+"/D"); 
   for(Int_t j = 0; j < 4 ; j++)    Data -> Branch(List[j+17], IntP[j], List[j+17]+"/I"); 
}
Int_t f8ppac001::Check(Double_t *p){//返回策略方式
   Int_t k = 0; 
   for(Int_t j = 0; j < 4; j++){
      if(t[j] > 0){
         k += TMath::Power(2., j) * (TMath::Abs(*(p+j)) < 150); 
      }
   }
   if(t[4] > 0){
      k += 16 * (TMath::Abs(*(p+4)) < 100); 
   }
   return k; 
}
Bool_t f8ppac001::calculate(TH2D *hForzx, Int_t k, Double_t *p, Double_t *pz, Double_t *pdx, Double_t *pFitxb, Double_t *pFitxk, Double_t *pchix, Double_t *pndfx){
   if((k > 4) && (k != 8) && (k != 12) && (k != 16)){
      TGraph *zFitTox = new TGraph(); 
      Int_t n = 0; 
      TF1 *FitxLocation = new TF1("FitxLocation", "pol1", -2000, 0); 
      for(Int_t m = 0; m < 4; m++){
         if((TMath::Abs(*(p+m)) < 150) && (t[m] > 0)){
            zFitTox -> SetPoint(n, *(pz+m), *(p+m)); n++; }
      }
      if((TMath::Abs(*(p+4)) < 100) && (t[4] > 0))    zFitTox -> SetPoint(n, *(pz+4), *(p+4)); 
      zFitTox -> Fit("FitxLocation", "SQ"); 
      *pFitxb = FitxLocation -> GetParameter(0); 
      *pFitxk = FitxLocation -> GetParameter(1); 
      if(TMath::Abs(*pFitxb/(1+*pFitxk)) < 30){ 
         aTrace(hForzx, *pFitxk, *pFitxb); //先b后k，神奇
         *pchix = FitxLocation -> GetChisquare(); 
         *pndfx = FitxLocation -> GetNDF(); 
         for(Int_t m = 0; m < 5; m++)     *(pdx+m) = *(p+m) - *pFitxk * *(pz+m) - *pFitxb; 
         return 1; 
      }
   } 
   for(Int_t m = 0; m < 5; m++)  *(pdx+m) = 9999; 
   *pchix = 999; 
   *pndfx = 999; 
   *pFitxb = 999; 
   *pFitxk = 999; 
   return 0; 
}