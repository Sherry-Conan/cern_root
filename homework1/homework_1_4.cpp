#include"TFile.h"
#include"TTree.h"
#include"TCanvas.h"
#include"TH2F.h"
#include"TH1D.h"
#include<iostream>

const Double_t a = 0.132814; 
const Double_t b = 14.5421/2; 
const Double_t Lambda = 374.409; 

void homework_1_4(){
    system("chcp 65001"); //加入这个可以引入中文。
    TH2F *h3_1 = new TH2F("h3_1","x->tx",100, -10, 25, 100, -10, 25); 
    TH2F *h3_2 = new TH2F("h3_2","x->tx",100, -5, 5, 100, -10, 25); 
    TH2F *h4_1 = new TH2F("h4_1","tx_fit",100, -110, 110, 100, -110, 110); 
    TH2F *h4_2 = new TH2F("h4_2","tx_fit",100, -10, 10, 100, -110, 110); 
    TH2F *h5_1 = new TH2F("h5_1","qx_fit",100, -110, 110, 100, -110, 110); 
    TH1D *h6_1 = new TH1D("h6_1","n",14001, -0.6, 0.6); 
    TH1D *h6_2 = new TH1D("h6_2","n",1401, -0.6, 0.6); 
    TH1D *h6_3 = new TH1D("h6_3","n",141, -0.6, 0.6); 
    TH1D *h6_4 = new TH1D("h6_4","n",71, -0.6, 0.6); 


    //读取文件
    TFile *Tree_File = new TFile("NEW_tree2.root"); 
    if (Tree_File->IsZombie()) {
        std::cout << "Error opening file" << std::endl; 
        exit(-1); 
    }
    TTree *Tree_Data = (TTree*) Tree_File->Get("NEW_tree2"); 

    TH2F *h1_1 = new TH2F("h1_1","n",2000, -20, 50, 50, 30, 100); 
    TH2F *h1_2 = new TH2F("h1_2","n",2000, -120, 120, 50, 0, 100); 
    TH2F *h2_1 = new TH2F("h2_1","gamma",2000, -20, 50, 50, 39, 48); 
    TH2F *h2_2 = new TH2F("h2_2","gamma",2000, -120, 120, 50, 16, 17.3); 
    TH1D *h6_5 = new TH1D("h6_5","n",14000, -0.6, 0.6); 
    TH1D *h6_6 = new TH1D("h6_6","n",1400, -0.6, 0.6); 
    TH1D *h6_7 = new TH1D("h6_7","n",140, -0.6, 0.6); 
    TH1D *h6_8 = new TH1D("h6_8","n",70, -0.6, 0.6); 

    Tree_Data->Draw("qx>>h6_5", "pid==1", "goff"); 
    Tree_Data->Draw("qx>>h6_6", "pid==1", "goff"); 
    Tree_Data->Draw("qx>>h6_7", "pid==1", "goff"); 
    Tree_Data->Draw("qx>>h6_8", "pid==1", "goff"); 

    Long64_t Jentry6; 
    Jentry6=h6_5->GetNbinsX(); 
    for(Long64_t j=0; j<Jentry6; j++) h6_1->Fill(h6_5->GetBinLowEdge(j+1), h6_5->GetBinContent(j+1)-h6_5->GetBinContent(j)); 
    Jentry6=h6_6->GetNbinsX(); 
    for(Long64_t j=0; j<Jentry6; j++) h6_2->Fill(h6_6->GetBinLowEdge(j+1), h6_6->GetBinContent(j+1)-h6_6->GetBinContent(j)); 
    Jentry6=h6_7->GetNbinsX(); 
    for(Long64_t j=0; j<Jentry6; j++) h6_3->Fill(h6_7->GetBinLowEdge(j+1), h6_7->GetBinContent(j+1)-h6_7->GetBinContent(j)); 
    Jentry6=h6_8->GetNbinsX(); 
    for(Long64_t j=0; j<Jentry6; j++) h6_4->Fill(h6_8->GetBinLowEdge(j+1), h6_8->GetBinContent(j+1)-h6_8->GetBinContent(j)); 

    TCanvas *c6 = new TCanvas("c6", "c6", 1000, 1000); 
    c6->Divide(2,2); 
    c6->cd(1); 
    h6_1->Sumw2(0); 
    h6_1->Draw("colz"); 
    h6_1->GetXaxis()->SetTitle("qx"); 
    h6_1->GetYaxis()->SetTitle("Number"); 
    c6->cd(2); 
    h6_2->Sumw2(0); 
    h6_2->Draw("colz"); 
    h6_2->GetXaxis()->SetTitle("qx"); 
    h6_2->GetYaxis()->SetTitle("Number"); 
    c6->cd(3); 
    h6_3->Sumw2(0); 
    h6_3->Draw("colz"); 
    h6_3->GetXaxis()->SetTitle("qx"); 
    h6_3->GetYaxis()->SetTitle("Number"); 
    c6->cd(4); 
    h6_4->Sumw2(0); 
    h6_4->Draw("colz"); 
    h6_4->GetXaxis()->SetTitle("qx"); 
    h6_4->GetYaxis()->SetTitle("Number"); 
    c6->Draw(); 

    Tree_Data->Draw("ctof:td-tu>>h1_1", "pid==1", "goff"); 
    Tree_Data->Draw("tof:x>>h1_2", "pid==1", "goff"); 

    TCanvas *c1 = new TCanvas("c1", "c1", 1000, 500); 
    c1->Divide(2,1); 
    c1->cd(1); 
    h1_1->Draw("colz"); 
    h1_1->GetXaxis()->SetTitle("ctof(s)"); 
    h1_1->GetYaxis()->SetTitle("tu-td(s)"); 
    c1->cd(2); 
    h1_2->Draw("colz"); 
    h1_2->GetXaxis()->SetTitle("tof(s)"); 
    h1_2->GetYaxis()->SetTitle("x(m)"); 
    c1->Draw(); 

    Tree_Data->Draw("ctof:td-tu>>h2_1", "pid==0", "goff"); 
    Tree_Data->Draw("tof:x>>h2_2", "pid==0", "goff"); 

    TCanvas *c2 = new TCanvas("c2", "c2", 1000, 500); 
    c2->Divide(2,1); 
    c2->cd(1); 
    h2_1->Draw("colz"); 
    h2_1->GetXaxis()->SetTitle("ctof(s)"); 
    h2_1->GetYaxis()->SetTitle("tu-td(s)"); 
    c2->cd(2); 
    h2_2->Draw("colz"); 
    h2_2->GetXaxis()->SetTitle("tof(s)"); 
    h2_2->GetYaxis()->SetTitle("x(m)"); 
    c2->Draw(); 

    Double_t tx, x, qx; 
    Tree_Data->SetBranchAddress("tx", &tx); 
    Tree_Data->SetBranchAddress( "x", &x ); 
    Tree_Data->SetBranchAddress( "qx", &qx ); 
    Long64_t Jentry; 
    Jentry = Tree_Data->GetEntries(); 
    for(Long64_t j=0; j<Jentry; j++){
        Tree_Data->GetEntry(j);  
        h3_1->Fill(a*x+b, tx); 
        h3_2->Fill(a*x+b-tx, tx);  
        h4_1->Fill(tx/a-b/a, x); 
        h4_2->Fill(tx/a-b/a-x, x); 
        h5_1->Fill(qx*Lambda-x, x); 
    }

    TCanvas *c3 = new TCanvas("c3", "c3", 1000, 500); 
    c3->Divide(2,1); 
    c3->cd(1); 
    h3_1->Draw("colz");  
    h3_1->GetXaxis()->SetTitle("fit(s)"); 
    h3_1->GetYaxis()->SetTitle("tx(s)"); 
    c3->cd(2); 
    h3_2->Draw("colz"); 
    h3_2->GetXaxis()->SetTitle("fit(s)"); 
    h3_2->GetYaxis()->SetTitle("tx(s)"); 
    c3->Draw(); 

    TCanvas *c4 = new TCanvas("c4", "c4", 1000, 500); 
    c4->Divide(2,1); 
    c4->cd(1); 
    h4_1->Draw("colz"); 
    h4_1->GetXaxis()->SetTitle("fit(m)"); 
    h4_1->GetYaxis()->SetTitle("tx(m)"); 
    c4->cd(2); 
    h4_2->Draw("colz"); 
    h4_2->GetXaxis()->SetTitle("fit(m)"); 
    h4_2->GetYaxis()->SetTitle("x(m)"); 
    c4->Draw(); 

    TCanvas *c5 = new TCanvas("c5", "c5", 1000, 500); 
    c5->Divide(2,1); 
    c5->cd(1); 
    h5_1->Draw("colz"); 
    h5_1->GetXaxis()->SetTitle("fit(m)"); 
    h5_1->GetYaxis()->SetTitle("x(m)"); 
    c5->cd(2); 
    h4_2->Draw("colz"); 
    c5->Draw(); 
}
