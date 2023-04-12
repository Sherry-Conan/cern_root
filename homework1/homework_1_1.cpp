#include<iostream>
#include"TTree.h"
#include"TGraph.h"
#include"TRandom3.h"
#include"TFile.h"
#include"TH1D.h"
#include"TCanvas.h"

class write_root{
public:
    Double_t x; 
    Double_t e; 
    int pid; 
    Double_t tof,  ctof; 
    Double_t tu,  td; 
    Double_t qu,  qd; 
    Double_t ctof_new; 
    Double_t cx_new; 
    //3. 将变量指向对应Branch的地址
    void OLD_Branch(TTree *tree){
        tree->SetBranchAddress("ctof", &ctof); //将ROOT文件内tree内名为"ctof"的branch的数据的指针指向ctof的变量。
        tree->SetBranchAddress("tof", &tof);   
        tree->SetBranchAddress("pid", &pid); 
        tree->SetBranchAddress("tu", &tu);    
        tree->SetBranchAddress("td", &td); 
        tree->SetBranchAddress("qu", &qu);    
        tree->SetBranchAddress("qd", &qd); 
    }

    void NEW_Branch(TTree *NEW_tree){
        NEW_tree->Branch("tu",  &tu,  "tu/D"); 
        NEW_tree->Branch("td",  &td,  "td/D"); 
        NEW_tree->Branch("ctof",  &ctof_new,  "ctof/D"); 
        NEW_tree->Branch("cx",  &cx_new,  "cx/D"); 
    }

    void WRITE_Tree(Long64_t jentry, TTree *tree, TTree *NEW_tree){
        tree->GetEntry(jentry); 
        ctof_new = ( tu + td )/ 2; 
        cx_new = ( tu - td ) / 2; 
        NEW_tree->Fill(); 
    }
}; 

int homework_1_1(){
    TFile *ipf = new TFile("tree.root"); //打开ROOT文件
    if (ipf->IsZombie()) {
    std::cout << "Error opening file" << std::endl; 
    exit(-1); 
    }
    ipf->cd(); 
    write_root wr; 
    TTree *tree = (TTree*)ipf->Get("tree"); //得到名字为“tree”的TTree指针
    wr.OLD_Branch(tree); 

    TFile *opf = new TFile("new_tree.root", "recreate"); //新文件tree.root，指针 *opf
    TTree *NEW_tree = new TTree("new_tree", "tree structure"); //新tree，指针 *opt 
    wr.NEW_Branch(NEW_tree); 

    Long64_t nentries = tree->GetEntries(); //得到tree的事件总数
    for(Long64_t jentry = 0;  jentry < nentries;  jentry++){
        wr.WRITE_Tree(jentry, tree, NEW_tree); 
    }

    NEW_tree->Write(); 
    opf->Close(); 
    ipf->Close(); 
    return 0; 
}
