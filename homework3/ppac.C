#define ppac_cxx
#include "ppac.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include"TRandom3.h"

void ppac::Loop()
{
//   In a ROOT session, you can do:
//      root> .L ppac.C
//      root> ppac t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;
   Double_t D = 499.; //cm, 粒子源到靶的距离，veto靶厚1cm，减去
   Double_t L = 100.; //cm, 半靶长
   Double_t dD = 1.; //cm, veto靶厚度
   Double_t TRes = 1.; //ns, 闪烁体时间分辨率(FWHM)
   Double_t Lambda = 380.; //cm, 靶的衰减长度
   Double_t QRes = 0.1; //晶片的相对能量分辨率（FWHM）。
   Double_t Vsc = 7.5; //ns/cm, 靶内光速
//gamma
   Double_t Rg = 0.05; //伽马比率
   Double_t Eg0 = 10; //MeV, γ能量

//neutron
   Double_t Rn = 0.75; //中子比
   Double_t En0 = 50; //MeV, 中子平均能
   Double_t EnRes = 50.; //MeV, 中子平均速度(FWHM)

//中子探测器击中的带电粒子,
   Double_t Rc = 0.1; //质子占比
   Double_t Ec0 = 50; //MeV, 质子平均能
   Double_t EcRes = 50; //MeV 质子平均速度(FWHM)

//ADC
   Double_t ADCgain = 40; //1MeV=40ch.
   Double_t ADCuPed = 140; //上侧ADC基线
   Double_t ADCdPed = 130; //下侧
   Double_t ADCnoise = 10; //噪声标准差
   Int_t    ADCoverflow = 4095; 

//TDC
   Double_t TriggerDelay = 15; //ns, trigger延迟,将感兴趣的时间信号放在TDC量程以内。
   Double_t TDCch2ns = 25.; //1ns=25ch.
   Int_t TDCoverflow = 4095; 

   Double_t tu_off = 15.5; //重设时间偏移
   Double_t td_off = 10.4; //重设时间偏移

   Long64_t nentries = fChain->GetEntriesFast(); 
   Int_t veto_iqu, veto_iqd, veto_itu, veto_itd; 
   Double_t veto_e, veto_q0, q0, veto_qd, veto_qu, veto_td, veto_tu, veto_tof, x, Dr, d, ctof; 
   TRandom3 *gr=new TRandom3(0); 


   TFile *File = new TFile("VETO.root", "recreate"); 
   TTree *veto_tree = new TTree("treeVETO", "tree structure"); 

   veto_tree->Branch("veto_itu", &veto_itu, "veto_itu/I"); 
   veto_tree->Branch("veto_itd", &veto_itd, "veto_itd/I"); 
   veto_tree->Branch("veto_iqu", &veto_iqu, "veto_iqu/I"); 
   veto_tree->Branch("veto_iqd", &veto_iqd, "veto_iqd/I"); 
   veto_tree->Branch("veto_e", &veto_e, "veto_e/D"); 
   veto_tree->Branch("veto_qu", &veto_qu, "veto_qu/D"); 
   veto_tree->Branch("veto_qd", &veto_qd, "veto_qd/D"); 
   veto_tree->Branch("veto_tu", &veto_tu, "veto_tu/D"); 
   veto_tree->Branch("veto_td", &veto_td, "veto_td/D"); 
   veto_tree->Branch("veto_tof", &veto_tof, "veto_tof/D"); 
   veto_tree->Branch("veto_pid", &pid, "veto_pid/I"); 

   veto_tree->Branch("e", &e, "e/D"); 
   veto_tree->Branch("pid", &pid, "pid/I"); 
   veto_tree->Branch("qu", &qu, "qu/D"); 
   veto_tree->Branch("qd", &qd, "qd/D"); 
   veto_tree->Branch("itu", &itu, "itu/I"); 
   veto_tree->Branch("itd", &itd, "itd/I"); 
   veto_tree->Branch("iqu", &iqu, "iqu/I"); 
   veto_tree->Branch("iqd", &iqd, "iqd/I"); 
   veto_tree->Branch("tof", &tof, "tof/D"); //veto对时间影响不大，认为不变
   veto_tree->Branch("tu", &tu, "tu/D"); 
   veto_tree->Branch("td", &td, "td/D"); 
   veto_tree->Branch("ctof", &ctof, "ctof/D"); 
   veto_tree->Branch("diff",&diff,"diff/D"); 

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      fChain->GetEntry(jentry); 
      if(pid==2){
         x = gr->Uniform(-L, L); 
         Dr = D + gr->Uniform(-0.5,0.5) * dD; 
         d = TMath::Sqrt(Dr*Dr+x*x); //cm

         veto_e = e/10; 
         veto_tof = 72.29824/TMath::Sqrt(e) * (d * 0.01); 

         veto_q0 = e * ADCgain / 10; 
         q0 = e * ADCgain * 9 / 10; 

         qu = q0 * TMath::Exp(-(L-x)/Lambda); 
         qd = q0 * TMath::Exp(-(L+x)/Lambda); 
         qu += gr->Gaus(ADCuPed,ADCnoise); 
         qd += gr->Gaus(ADCdPed,ADCnoise); //有在125-126一起更新
         veto_qu = veto_q0 * TMath::Exp(-(L-x)/Lambda); 
         veto_qd = veto_q0 * TMath::Exp(-(L+x)/Lambda); 
         veto_qu += gr->Gaus(ADCuPed,ADCnoise); 
         veto_qd += gr->Gaus(ADCdPed,ADCnoise); 
         veto_iqu = Int_t(veto_qu); 
         veto_iqd = Int_t(veto_qd); 

         veto_tu = veto_tof + (L - x)/Vsc+gr->Gaus(0,TRes/2.35) + tu_off - TriggerDelay; 
         veto_td = veto_tof + (L + x)/Vsc+gr->Gaus(0,TRes/2.35) + td_off - TriggerDelay; 
         veto_tu *= TDCch2ns; 
         veto_td *= TDCch2ns; 
         if(veto_tu > TDCoverflow) veto_tu = TDCoverflow; 
         if(veto_td > TDCoverflow) veto_td = TDCoverflow; 
         veto_itu = Int_t(veto_tu); 
         veto_itd = Int_t(veto_td); 

         diff = tu - itu; 
         ctof = itd - itu; 
      }
      else{
         veto_e = -1; 
         veto_tof = -1; 

         veto_qu = ADCuPed+gr->Gaus(0,ADCnoise); 
         veto_qd = ADCdPed+gr->Gaus(0,ADCnoise); 
         veto_iqu = Int_t(veto_qu); 
         veto_iqd = Int_t(veto_qd); 

         veto_td = TDCoverflow; 
         veto_tu = TDCoverflow; 
         veto_itu = TDCoverflow; 
         veto_itd = TDCoverflow; 

      }
      veto_tree->Fill(); 
   }
   veto_tree->Write(); 
   File->Close(); 
}