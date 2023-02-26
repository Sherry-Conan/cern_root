#include"TFile.h"
#include"TF1.h"
#include<iostream>
#include"TTree.h"
#include<math.h>
#include"TBranch.h"
#include"TH1D.h"

using namespace std; 

const Double_t L = 100.; //cm,  一半靶的长度
const Double_t a = 0.132814; 
const Double_t b = 14.5421; 
const Double_t gamma = L*5*3.333/100; //gamma射线运动时间

class ReadRoot{
private:
    const Double_t L = 100.; //cm,  一半靶的长度
    Double_t e, x,  ctof, tu, td, qu, qd, tdtu, qdqu, ntof, ce, to_one; 
    Int_t pid; 
    Long64_t entry_NUM; 
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
    }

    void NEW_Branch(TTree *NEW_tree){
        NEW_tree->Branch("ctof", &ctof); //将ROOT文件内tree内名为"ctof"的branch的数据的指针指向ctof的变量。
        NEW_tree->Branch("tu", &tu); 
        NEW_tree->Branch("td", &td); 
        NEW_tree->Branch("qu", &qu); 
        NEW_tree->Branch("qd", &qd); 
        NEW_tree->Branch("tx", &tdtu); 
        NEW_tree->Branch("qx", &qdqu); 
        NEW_tree->Branch("ntof", &ntof); 
        NEW_tree->Branch("ce", &ce); 
        NEW_tree->Branch("e", &e); 
        NEW_tree->Branch("x", &x); 
        NEW_tree->Branch("pid", &pid); 
    }

    void ToOne(TTree *tree, TTree *NEW_tree){
        entry_NUM = tree->GetEntries(); 
        TF1 *f1  =  new TF1("f1", "gaus", 42, 44); 
        TH1D *Branch_ctof =  new TH1D("Branch_ctof", "ctof", 140, 40, 90); 
        for(Long64_t j = 0; j<entry_NUM; j++){
            tree->GetEntry(j); 
            Branch_ctof->Fill(ctof); 
        }
        Branch_ctof->Fit("f1", "R"); 
        to_one = f1->GetParameter(1); 
        std::cout<<to_one<<std::endl; 
        for(Long64_t j = 0; j<entry_NUM; j++){
            tree->GetEntry(j); 
            if(ctof>50) {//去掉gamma
                ntof = (ctof-to_one+gamma)/5; 
                ce = pow((72.29824/ntof), 2); 
                tdtu = (td-tu); 
                qdqu = sqrt(qd*qu)*qd/qu; 
                NEW_tree->Fill(); 
                }
        }
    }
}; 

void homework_1_3(){
    system("chcp 65001"); //加入这个可以引入中文。

    TFile *tree_file = new TFile("tree.root"); //打开ROOT文件
    if (tree_file->IsZombie()) {
    std::cout << "Error opening file" << std::endl; 
    exit(-1); 
    }
    ReadRoot RR; 
    TTree *tree = (TTree*) tree_file->Get("tree"); //得到名字为“tree”的TTree指针
    RR.OLD_Branch(tree); 

    TFile *new_tree_file = new TFile("NEW_tree.root", "recreate"); //打开全新的ROOT文件
    TTree *new_tree = new TTree("NEW_tree", "tree structure"); 
    RR.NEW_Branch(new_tree); 

    RR.ToOne(tree, new_tree); 
    new_tree->Write(); 
    new_tree_file->Close(); 
    tree_file->Close(); 
}