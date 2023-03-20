#define f8ppac001_cxx
#include "f8ppac001.h"
#include "TF1.h"
#include "TGraph.h"
#include <TStyle.h>
#include "TCanvas.h"

void Trace(TH2D *h, Double_t k, Double_t b, Int_t max = 0, Int_t min = -1800); 

void f8ppac001::Loop(){
   TH2D *hForzx = new TH2D("hForzx","Fit z_{x} to x by ppac1A, ppac2A and ppac3",2200, -2000, 200, 300, -150, 150); 
   TH2D *hForzy = new TH2D("hForzy","Fit z_{y} to y by ppac1A, ppac2A and ppac3",2200, -2000, 200, 300, -150, 150); 

   Long64_t nentries = fChain->GetEntriesFast(); 

   fChain->GetEntry(539575); 
   std::cout<<PPACF8[0][0]<<","<<PPACF8[1][0]<<","<<PPACF8[2][0]<<","<<PPACF8[3][0]<<","<<PPACF8[4][0]<<std::endl; 
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
      //算X
      Bool_t BoolForxFill = coculate(hForzx, 0, Check(0)); 
      //算Y
      Bool_t BoolForyFill = coculate(hForzy, 1, Check(1)); 
      if(BoolForxFill || BoolForyFill)   Data -> Fill(); 
   }
   
   Data -> Write(); 
   hForzx -> Write(); 
   hForzy -> Write(); 
   File -> Close(); 
}

void f8ppac001::aTrace(TH2D *h, Double_t k, Double_t b, Int_t max = 0, Int_t min = -1800){
   if(min > max) return;

   for(min; min < max; min++){
      h -> Fill(min , k * min + b); 
   }
}

void f8ppac001::SetBranch(TTree *Data){
   Data -> Branch("x",  &x,   "x[5]/D"); //依次x的位置
   Data -> Branch("y",  &y,   "y[5]/D"); //依次y的位置
   Data -> Branch("t",  &t,   "t[5]/D"); //依次t的位置
   Data -> Branch("dx", &dx,  "dx[5]/D"); //依次dx
   Data -> Branch("dy", &dy,  "dy[5]/D"); //依次dy
   Data -> Branch("z_x",&z_x, "z_x[5]/D"); //依次z_x的位置
   Data -> Branch("z_y",&z_y, "z_y[5]/D"); //依次z_y的位置

   Data -> Branch("Fitxk", &Fitxk, "Fitxb/D"); 
   Data -> Branch("Fitxb", &Fitxb, "Fitxk/D"); 
   Data -> Branch("Fityk", &Fityk, "Fityb/D"); 
   Data -> Branch("Fityb", &Fityb, "Fityk/D"); 

   Data -> Branch("beamTrig", &beamTrig, "beamTrig/I"); 
   Data -> Branch("must2Trig", &must2Trig, "must2Trig/I"); 
   Data -> Branch("targetX", &targetX, "targetX/F"); 
   Data -> Branch("targetY", &targetY, "targetY/F"); 

   Data -> Branch("chix", &chix, "chix/D"); 
   Data -> Branch("chiy", &chiy, "chiy/D"); 
   Data -> Branch("ndfx", &ndfx, "ndfx/D"); 
   Data -> Branch("ndfy", &ndfy, "ndfy/D"); 
}

Int_t f8ppac001::Check(Int_t i){//返回策略方式
   Int_t k = 0; 
   if(i == 0){
   for(Int_t j = 0; j < 2; j++){
      if(t[2*j] > 0){
         k += TMath::Power(4., j) * (TMath::Abs(x[2*j]) < 150); }
      if(t[2*j+1] > 0){
         k += TMath::Power(4., j) * (TMath::Abs(x[2*j+1]) < 150); }
      }
      if(t[4] > 0){
         k += 16 * (TMath::Abs(x[4]) < 100); 
      }
   }

   else{
      for(Int_t j = 0; j < 2; j++){
      if(t[2*j] > 0){
         k += TMath::Power(4., j) * (TMath::Abs(y[2*j]) < 150); }
      if(t[2*j+1] > 0){
         k += TMath::Power(4., j) * (TMath::Abs(y[2*j+1]) < 150); }
      }
      if(t[4] > 0){
         k += 16 * (TMath::Abs(y[4]) < 100); 
      }
   }
      
   return k; 
}
//为检查能否输出赋值k，k初值为0，PPAC1A可以作为输出就给其加2^0,B就加2^1
//PPAC2A就加2^2，PPAC2B就加2^3,PPAC3就加2^4，如此，
//就可以通过k值来判断是否可以读还能判断是哪个板子可读。如10001，
//就指PPAC3和PPAC1A可读。也就是说，k为0时，指没有板子可读，
//为2^n时，就指只有一个PPAC板上的值可读，为2^(2n+1)+2^(2n)时只在一个板子上可读

Bool_t f8ppac001::coculate(TH2D *hForzx, Int_t i, Int_t k){
   if(i == 0){
      if((k > 4) && (k != 8) && (k != 12) && (k != 16)){
//k的赋值是不会出现3的，0指没有板子可用，1指只有一个PPAC1A上的板子可用，3指只有PPAC1上的板子可用且全可用
//2不存在，4指只有PPAC2A上有一个板子可用，12指PPAC上的板子全可用。
         TGraph *zFitTox = new TGraph();
         Int_t n = 0; 
         TF1 *FitxLocation = new TF1("FitxLocation", "pol1", -2000, 0); 
         for(Int_t m = 0; m < 4; m++){
            if(TMath::Abs(x[m]) < 150){
               zFitTox -> SetPoint(n, z_x[m], x[m]); n++; }
         }
         if(TMath::Abs(x[4]) < 100)    zFitTox -> SetPoint(n, z_x[4], x[4]); 
         zFitTox -> Fit("FitxLocation", "SQ"); 
         Fitxb = FitxLocation -> GetParameter(0); 
         Fitxk = FitxLocation -> GetParameter(1); 
         if(TMath::Abs(Fitxb/(1+Fitxk)) > 30) return 0; 
         aTrace(hForzx, Fitxk, Fitxb); //先b后k，神奇
         chix = FitxLocation -> GetChisquare(); 
         ndfx = FitxLocation -> GetNDF(); 
         for(Int_t m = 0; m < 5; m++){
            dx[m] = x[m] - Fitxk * z_x[m] - Fitxb; //有效判断
         }
         return 1; 
      }

      return 0; 

   }


   else{
      if((k > 4) && (k != 8) && (k != 16) && (k != 12)){
         TGraph *zFitTox = new TGraph();
         Int_t n = 0; 
         TF1 *FitxLocation = new TF1("FitxLocation", "pol1", -2000, 0); 
         for(Int_t m = 0; m < 4; m++){
            if(TMath::Abs(y[m]) < 150){
               zFitTox -> SetPoint(n, z_y[m], y[m]); n++; }
         }
         if(TMath::Abs(y[4]) < 100)    zFitTox -> SetPoint(n, z_y[4], y[4]); 
         zFitTox -> Fit("FitxLocation", "SQ"); 
         Fityb = FitxLocation -> GetParameter(0); 
         Fityk = FitxLocation -> GetParameter(1); 
         if(TMath::Abs(Fitxb*Fityk/(1+Fitxk)+Fityb) > 30) return 0; 
         chiy = FitxLocation -> GetChisquare(); 
         ndfy = FitxLocation -> GetNDF(); 
         aTrace(hForzx, Fityk, Fityb); //先b后k，神奇
         for(Int_t m = 0; m < 5; m++){
            dy[m] = y[m] - Fityk * z_y[m] - Fityb; //有效判断
         }
         return 1; 
      }
   }
   
   return 0; 
}