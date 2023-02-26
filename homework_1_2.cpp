#include"TFile.h"
#include"TH1D.h"
#include"TCanvas.h"
#include<iostream>
#include<math.h>
#include"TTree.h"
#include<math.h>
#include"TF1.h"
#include"TGraph.h"

const Double_t L = 100.; //cm,  一半靶的长度

class read_root{
private:
    Double_t ctof, tu, td, qu, qd; 
public:
    Double_t tx_sum = 0; 
    //3. 将变量指向对应Branch的地址
    void OLD_Branch(TTree *tree){
        tree->SetBranchAddress("ctof", &ctof); 
        tree->SetBranchAddress("tu", &tu); //将ROOT文件内tree内名为"ctof"的branch的数据的指针指向ctof的变量。   
        tree->SetBranchAddress("td", &td); 
        tree->SetBranchAddress("qu", &qu); 
        tree->SetBranchAddress("qd", &qd); 
    }
    void coculate(Long64_t i, TTree *tree, TH1D *tdiff, TH1D *qdiff){
        tree->GetEntry(i); 
        if(ctof>50) {
            tdiff->Fill(td-tu); 
            qdiff->Fill(log(qu/qd)); 
        }
    }
    }; 
class Make_Gause{
private:
    Double_t tx_mean, tx_max, tx_min, qx_max, qx_min = 0; 
    Double_t NUM, tx, tx_std, qx, qx_std; 
public:
    Double_t tx_max_NUM, tx_min_NUM, qx_max_NUM, qx_min_NUM; 
    void coculate(TH1D *dtd, TH1D *dqd){
        NUM = dtd->GetEntries(); 
        for(Long64_t j = 0; j<NUM; j++){
            tx = dtd->GetBinContent(j); 
            if(tx>tx_max) {tx_max = tx; tx_max_NUM = j; }
            if(tx<tx_min) {tx_min = tx; tx_min_NUM = j; }}
        NUM = dqd->GetEntries(); 
        for(Long64_t j = 0; j < NUM; j++){
            qx = dqd->GetBinContent(j); 
            if(qx > qx_max) {qx_max = qx; qx_max_NUM = j; }
            if(qx < qx_min) {qx_min = qx; qx_min_NUM = j; }
        }
    }
    void Gause_max(TH1D *dtd, TF1 *f1, TH1D *dqd, TF1 *qf1){
        dtd->Fit("f1", "R"); 
        dqd->Fit("qf1", "R"); 
        //dtd->Fit("gaus", "", "", dtd->GetBinCenter(MG.tx_min_NUM-4), dtd->GetBinCenter(MG.tx_min_NUM+4)); //gaus不能取负值
    }
    void Gause_min(TH1D *dtd, TF1 *f1, TH1D *dqd, TF1 *qf1){
        tx = (dtd->GetBinContent(tx_min_NUM))*(dtd->GetBinCenter(tx_min_NUM)); 
        NUM = dtd->GetBinContent(tx_min_NUM); 
        for(Long64_t j = 1; j < 6; j++){
            tx += (dtd->GetBinContent(tx_min_NUM+j))*(dtd->GetBinCenter(tx_min_NUM+j)); 
            tx += (dtd->GetBinContent(tx_min_NUM-j))*(dtd->GetBinCenter(tx_min_NUM-j)); 
            NUM += dtd->GetBinContent(tx_min_NUM+j)+dtd->GetBinContent(tx_min_NUM-j); 
        }
        tx = tx/NUM; 
        tx_std = (dtd->GetBinContent(tx_min_NUM))*pow((dtd->GetBinCenter(tx_min_NUM)-tx), 2); 
        for(Long64_t j = 1; j < 6; j++){
            tx_std += (dtd->GetBinContent(tx_min_NUM+j))*pow((dtd->GetBinCenter(tx_min_NUM+j)-tx), 2); 
            tx_std += (dtd->GetBinContent(tx_min_NUM-j))*pow((dtd->GetBinCenter(tx_min_NUM-j)-tx), 2); 
        }
        tx_std = pow(tx_std/NUM, 0.5); 

        f1->SetParameter(0, dtd->GetBinCenter(tx_min_NUM)); //大致估计范围
        f1->SetParameter(1, tx); 
        f1->SetParameter(2, tx_std); 
        
        qx = (dqd->GetBinContent(qx_min_NUM))*(dqd->GetBinCenter(qx_min_NUM)); 
        NUM = dqd->GetBinContent(qx_min_NUM); 
        for(Long64_t j = 1; j<6; j++){
            qx += (dqd->GetBinContent(qx_min_NUM+j))*(dqd->GetBinCenter(qx_min_NUM+j)); 
            qx += (dqd->GetBinContent(qx_min_NUM-j))*(dqd->GetBinCenter(qx_min_NUM-j)); 
            NUM += dqd->GetBinContent(qx_min_NUM+j)+dqd->GetBinContent(qx_min_NUM-j); 
        }
        qx = qx/NUM; 
        qx_std = (dqd->GetBinContent(qx_min_NUM))*pow((dqd->GetBinCenter(qx_min_NUM)-qx), 2); 
        for(Long64_t j = 1; j<6; j++){
            qx_std += (dqd->GetBinContent(qx_min_NUM+j))*pow((dqd->GetBinCenter(qx_min_NUM+j)-qx), 2); 
            qx_std += (dqd->GetBinContent(qx_min_NUM-j))*pow((dqd->GetBinCenter(qx_min_NUM-j)-qx), 2); 
        }
        qx_std = pow(qx_std/NUM, 0.5); 

        qf1->SetParameter(0, dqd->GetBinCenter(qx_min_NUM)); //大致估计范围
        qf1->SetParameter(1, qx); 
        qf1->SetParameter(2, qx_std); 
    }
}; 
void homework_1_2(){
    system("chcp 65001"); 

    TFile *tree_data = new TFile("tree.root"); //打开ROOT文件
    if (tree_data->IsZombie()) {
    std::cout << "Error opening file" << std::endl; 
    exit(-1); 
    }
    tree_data->cd(); 
    TTree *tree = (TTree*)tree_data->Get("tree"); //得到名字为“tree”的TTree指针
    read_root rr; 
    rr.OLD_Branch(tree); 
    Long64_t entries_NUM = tree->GetEntries(); 
    TH1D *tdiff = new TH1D("tdiff", "td-tu", 140, -20, 50);  
    TH1D *qdiff = new TH1D("qdiff", "qu/qd", 70, -0.8, 0.8); 
    for(Long64_t j = 0; j < entries_NUM; j++) rr.coculate(j, tree, tdiff, qdiff); 
    Long64_t bins_NUM = tdiff->GetNbinsX(); 
    TH1D *dtd = new TH1D("dtd", "dt/dx", 141, -20, 50); 
    TH1D *dqd = new TH1D("dqd", "dq/dx", 71, -0.8, 0.8); 
    Double_t df; 
    for(Long64_t i = 0; i < bins_NUM; i++){
        df = tdiff->GetBinContent(i+1)-tdiff->GetBinContent(i); 
        dtd->Fill(tdiff->GetBinLowEdge(i+1), df); 
        df = qdiff->GetBinContent(i+1)-qdiff->GetBinContent(i); 
        dqd->Fill(qdiff->GetBinLowEdge(i+1), df); 
    }
    TCanvas *c1 = new TCanvas("c1", "c1"); 
    Make_Gause MG; 
    MG.coculate(dtd, dqd); 
    TF1 *f1  =  new TF1("f1", "gaus", dtd->GetBinCenter(MG.tx_max_NUM-5), dtd->GetBinCenter(MG.tx_max_NUM+5)); 
    TF1 *qf1  =  new TF1("qf1", "gaus", dqd->GetBinCenter(MG.qx_max_NUM-5), dqd->GetBinCenter(MG.qx_max_NUM+5)); 
    MG.Gause_max(dtd, f1, dqd, qf1); 
    TF1 *f2  =  new TF1("f2", "[0]*TMath::Exp(-0.5*((x-[1])/[2])^2)", dtd->GetBinCenter(MG.tx_min_NUM-3), dtd->GetBinCenter(MG.tx_min_NUM+3)); 
    TF1 *qf2  =  new TF1("qf2", "[0]*TMath::Exp(-0.5*((x-[1])/[2])^2)", dqd->GetBinCenter(MG.qx_min_NUM-3), dtd->GetBinCenter(MG.qx_min_NUM+2)); 
    MG.Gause_min(dtd, f2, dqd, qf2); 
    dtd->Fit("f2", "R", "same"); 
    dqd->Fit("qf2", "R", "same"); 
    double mean_min  =  f1->GetParameter(1); 
    double mean_max  =  f2->GetParameter(1); 
    double q_mean_min  =  qf1->GetParameter(1); 
    double q_mean_max  =  qf2->GetParameter(1); 
    std::cout<<"tx右边界"<<mean_max<<std::endl; 
    std::cout<<"tx左边界"<<mean_min<<std::endl; 
    std::cout<<"qx右边界"<<q_mean_max<<std::endl; 
    std::cout<<"qx左边界"<<q_mean_min<<std::endl; 

    TF1 *f3  =  new TF1("f3", "[0]*x+[1]", -2*L, 2*L); 
    f3->SetParameter(0, 2/15); //大致估计范围
    f3->SetParameter(1, 14.9); 

    TF1 *qf3  =  new TF1("qf3", "[0]*x+[1]", -2*L, 2*L); 
    qf3->SetParameter(0, 380); //大致估计范围
    qf3->SetParameter(1, 0); 

    Int_t n = 3; 
    Double_t x[3] = {-2*L, 0, 2*L}; 
    Double_t y[3] = {mean_min, mean_min/2+mean_max/2, mean_max}; 
    Double_t z[3] = {q_mean_min, q_mean_min/2+q_mean_max/2, q_mean_max}; 
    TGraph* gr1  =  new TGraph(n, x, y);  
    TGraph* qgr1  =  new TGraph(n, x, z);  
    gr1->Fit("f3", "R"); 
    qgr1->Fit("qf3", "R"); 
    std::cout<<"Vcs = "<<1/(f3->GetParameter(0))<<std::endl; 
    std::cout<<"tdoff-tuoff = "<<f3->GetParameter(1)<<std::endl; 
    std::cout<<"Lambda = "<<1/(qf3->GetParameter(0))<<std::endl; 
    std::cout<<"1/Vcs = "<<(f3->GetParameter(0))<<std::endl; 
}