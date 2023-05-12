#include"dssd.h"
#include<vector>
#include<TGraph.h>
#include<TH2.h>
#include<TCutG.h>
#include<TTree.h>
class ana : private dssd{
    private:
    Int_t NUM[3][2][32]; 
    TH2I *g[3][32][32]; 
    TGraph *gD[3][2][32]; 
    Double_t k[3][2][32]; 
    Double_t b[3][2][32]; 
    Double_t ChiNdf[3][2][32]; 
    Int_t MAX[3]; 
    Bool_t j[3]; 
    std::vector<Int_t> XY[3][2]; 
    Long64_t Nentry = fChain -> GetEntries(); 
    //Int_t xy[3][2] = {{17, 12}, {15, 12}, {17, 12}}; //取同一粒子能量的概率最高的点
    TTree *Data = NULL; 
    TDirectory* hFolder = NULL; 
    public:
    void DotNum(); 
    void DotWithg(Int_t xhit, Int_t yhit, Int_t xs[13], Int_t ys[13], Int_t xe[13], Int_t ye[13], Int_t jj, TCutG *cut); 
    void FitPol(TTree *aData, TDirectory* myFolder); 
    void Select(); 
    void FiT(); 
    void WriTe(); 
}; 