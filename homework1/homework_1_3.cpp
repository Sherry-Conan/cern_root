#include"TFile.h"
#include"TF1.h"
#include<iostream>
#include"TTree.h"
#include"TBranch.h"
#include"TH1D.h"

using namespace std; 

const Double_t L = 100.; //cm,  一半靶的长度
const Double_t a = 0.132814; 
const Double_t b = 14.5421; 
const Double_t gamma = L*5*3.333/100; //gamma射线运动时间
const Double_t D = 500.; //cm,  distance between target and the scin.(Center)
const Double_t dD = 5.; //cm,  靶的厚度
const Double_t Eg0 = 1; //MeV,  gamma能量

class ReadRoot{
private:
    const Double_t L = 100.; //cm,  一半靶的长度
    Double_t e, x,  ctof, tu, td, qu, qd, tdtu, qdqu, ntof, ce, to_one, tof; 
    Int_t pid; 
    Long64_t entry_NUM; 
    Double_t d;
public:

    //3. 将变量指向对应Branch的地址
    void OLD_Branch(TTree *tree){
        tree->SetBranchAddress("ctof", &ctof); //将ROOT文件内tree内名为"ctof"的branch的数据的指针指向ctof的变量。
        tree->SetBranchAddress("tu", &tu); 
        tree->SetBranchAddress("td", &td); 
        tree->SetBranchAddress("qu", &qu); 
        tree->SetBranchAddress("qd", &qd); 
        tree->SetBranchAddress("e", &e); 
        tree->SetBranchAddress("x", &x); 
        tree->SetBranchAddress("pid", &pid); 
        tree->SetBranchAddress("tof", &tof); 
    }

    void NEW_Branch(TTree *NEW_tree){
        NEW_tree->Branch("ctof", &ctof, "ctof/D"); //将ROOT文件内tree内名为"ctof"的branch的数据的指针指向ctof的变量。
        NEW_tree->Branch("tu", &tu, "tu/D"); 
        NEW_tree->Branch("td", &td, "td/D"); 
        NEW_tree->Branch("qu", &qu, "qu/D"); 
        NEW_tree->Branch("qd", &qd, "qd/D"); 
        NEW_tree->Branch("tx", &tdtu, "tx/D"); 
        NEW_tree->Branch("qx", &qdqu, "qx/D"); 
        NEW_tree->Branch("ntof", &ntof, "ntof/D"); 
        NEW_tree->Branch("ce", &ce, "ce/D"); 
        NEW_tree->Branch("e", &e, "e/D"); 
        NEW_tree->Branch("x", &x, "x/D"); 
        NEW_tree->Branch("pid", &pid, "pid/I"); 
        NEW_tree->Branch("tof", &tof, "tof/D"); 
    }
    void ToOne(TTree *tree, TTree *NEW_tree,TH1D *Branch_ctof){
        entry_NUM = tree->GetEntries(); 
        TF1 *f1  =  new TF1("f1", "gaus", 42, 44); 
        for(Long64_t j = 0; j<entry_NUM; j++){
            tree->GetEntry(j); 
            Branch_ctof->Fill(ctof); 
        }
        Branch_ctof->Fit("f1", "RN"); 
        to_one = f1->GetParameter(1); 
        std::cout<<to_one<<std::endl; 
        for(Long64_t j = 0; j<entry_NUM; j++){
            tree->GetEntry(j); 
            if(ctof>50) {//中子
                d=TMath::Sqrt((D+dD/4)*(D+dD/4)+(a*(tu-td)+b)*(a*(tu-td)+b)); //dD随机取（-0.5，0.5）
                ntof = (ctof-to_one+gamma)/d/0.01; 
                ce = (72.29824/ntof)*(72.29824/ntof); 
            }
            else{
                ce = Eg0;  
                ntof = gamma; 
            }
            tdtu = (td-tu)/2; 
            qdqu = -TMath::Log(qd/qu)/2; 
            NEW_tree->Fill(); 
        }
    }
}; 

void homework_1_3(){
    system("chcp 65001"); //加入这个可以引入中文。

    TH1D *Branch_ctof =  new TH1D("Branch_ctof", "ctof", 140, 40, 90); 

    TFile *tree_file = new TFile("tree.root"); //打开ROOT文件
    if (tree_file->IsZombie()) {
    std::cout << "Error opening file" << std::endl; 
    exit(-1); 
    }
    ReadRoot RR; 
    TTree *tree = (TTree*) tree_file->Get("tree"); //得到名字为“tree”的TTree指针
    RR.OLD_Branch(tree); 

    TFile *new_tree_file = new TFile("NEW_tree2.root", "recreate"); //打开全新的ROOT文件
    TTree *new_tree = new TTree("NEW_tree2", "tree structure"); 
    RR.NEW_Branch(new_tree); 

    RR.ToOne(tree, new_tree,Branch_ctof); 
    new_tree->Write(); 
    new_tree_file->Close(); 
    tree_file->Close(); 
}
