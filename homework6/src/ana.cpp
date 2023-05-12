#include "ana.h"
#include <TROOT.h>
#include <TF1.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
void ana::FitPol(TTree *aData, TDirectory* myFolder){
    Data = aData; 
    hFolder = myFolder; 
    for(Int_t ii = 0; ii < 3; ii++)
        for (Int_t i = 0; i < 32; i++) {
            XY[ii][0].push_back(i); 
            XY[ii][1].push_back(i); 
        }
    DotNum(); 
    std::cout<<"开始写入TH2I"<<std::endl; 
    for(Int_t ii = 0; ii < 3; ii++){
        for(Int_t i = 0; i < 32; i++){
            for(Int_t l = 0; l < 32; l++){
                hFolder -> WriteObject(g[ii][i][l], Form("%ih%2i%2i", ii, i, l)); 
            }
        }
    }
    std::cout<<"完成写入TH2I"<<std::endl; 
    Double_t xbin = g[0][0][0] ->GetXaxis() -> GetBinWidth(0); 
    Double_t ybin = g[0][0][0] ->GetYaxis() -> GetBinWidth(0); 
    Int_t xBinNumber = g[0][0][0] -> GetNbinsX(); //得到横坐标bin数量
    Int_t yBinNumber = g[0][0][0] -> GetNbinsY(); //得到总坐标bin数量
    Int_t p[3] = {17, 15, 17}; //选定不动条
    k[0][0][17] = 1; 
    k[1][0][15] = 1; 
    k[2][0][17] = 1; 
    for(Int_t ii = 0; ii < 3; ii++) {
        XY[ii][1].erase(std::remove(XY[ii][1].begin(), XY[ii][1].end(), p[ii]), XY[ii][1].end()); 
        k[ii][0][p[ii]] = 1; 
        b[ii][0][p[ii]] = 0; //选定不动条
        for (Int_t i:XY[ii][1]){
            for(Int_t ix = 0; ix < xBinNumber; ix++)
                for(Int_t iy = 0; iy <yBinNumber; iy++)
                    if(g[ii][p[ii]][i] -> GetBinContent(ix, iy) > 0){
                        gD[ii][0][i] -> AddPoint(g[ii][p[ii]][i] -> GetXaxis() -> GetBinLowEdge(ix) + xbin / 2, g[ii][p[ii]][i] -> GetYaxis() -> GetBinLowEdge(iy) + ybin / 2); 
                        NUM[ii][0][i] += 1; 
                    }
        }
    }
    for(Int_t jjj = 0; jjj < 63; jjj++){
        Select(); //找到没有拟合的XY条和已拟合重叠最多的条进行拟合
        FiT(); 
        for(Int_t ii = 0; ii < 3; ii++){
            XY[ii][j[ii]].erase(std::remove(XY[ii][j[ii]].begin(), XY[ii][j[ii]].end(), MAX[ii]), XY[ii][j[ii]].end()); 

            for(Int_t i:XY[ii][!j[ii]]){
                if(!j[ii]){
                    for(Int_t ix = 0; ix < xBinNumber; ix++)
                        for(Int_t iy = 0; iy < yBinNumber; iy++)
                            if(g[ii][i][MAX[ii]] -> GetBinContent(ix, iy) > 0){
                                NUM[ii][!j[ii]][i] += 1; 
                                gD[ii][!j[ii]][i] -> AddPoint(g[ii][i][MAX[ii]] -> GetXaxis() -> GetBinLowEdge(ix) + xbin / 2, k[ii][!j[ii]][MAX[ii]] * (g[ii][i][MAX[ii]] -> GetYaxis() -> GetBinLowEdge(iy) + ybin / 2) + b[ii][!j[ii]][MAX[ii]]); 
                            }
                }
                else{
                    for(Int_t ix = 0; ix < xBinNumber; ix++)
                        for(Int_t iy = 0; iy <yBinNumber; iy++)
                            if(g[ii][MAX[ii]][i] -> GetBinContent(ix, iy) > 0){
                                NUM[ii][!j[ii]][i] += 1; 
                                gD[ii][!j[ii]][i] -> AddPoint(k[ii][!j[ii]][MAX[ii]] * (g[ii][MAX[ii]][i] -> GetXaxis() -> GetBinLowEdge(ix) + xbin / 2) + b[ii][!j[ii]][MAX[ii]], g[ii][MAX[ii]][i] -> GetYaxis() -> GetBinLowEdge(iy) + ybin / 2); 
                            }
                }
            }
        }
    }
    WriTe(); 
}
void ana::DotNum(){
    for(Int_t ii = 0; ii < 3; ii++){
        for(Int_t i = 0; i < 32; i++){
            for(Int_t l = 0; l < 32; l++){
                g[ii][i][l] = new TH2I(Form("%i%2ih%2i", ii, i, l), Form("%i%2ih%2i", ii, i, l), 1000, 0, 8000, 1000, 0, 8000); 
            }
        }
    }
    for(Int_t ii = 0; ii < 3; ii++){
        for(Int_t i = 0; i < 2; i++){
            for(Int_t l = 0; l < 32; l++){
                gD[ii][i][l] = new TGraph(); 
                NUM[ii][i][l] = 0; 
            }
        }
    }
    // Load the Cut.C file
    TCutG *cut[3]; 
    for(Int_t ii = 0; ii < 3; ii++){
        gROOT -> Macro(Form("cut%1i.cpp", ii + 1)); 
        cut[ii] = (TCutG*)gROOT -> GetListOfSpecials() -> FindObject(Form("CUT%1i", ii + 1)); 
    }
    // Get the TCutG object from the current ROOT session
    for(Long64_t j = 0; j < Nentry; j++){
        fChain -> GetEntry(j); 
        DotWithg(d1xhit, d1yhit, d1xs, d1ys, d1xe, d1ye, 0, cut[0]); 
        DotWithg(d2xhit, d2yhit, d2xs, d2ys, d2xe, d2ye, 1, cut[1]); 
        DotWithg(d3xhit, d3yhit, d3xs, d3ys, d3xe, d3ye, 2, cut[2]); 
    }
}
void ana::DotWithg(Int_t xhit, Int_t yhit, Int_t xs[13], Int_t ys[13], Int_t xe[13], Int_t ye[13], Int_t jj, TCutG *cut){
    if((xhit == 1) && (yhit == 1))
    for(Int_t ixhit = 0; ixhit < xhit; ixhit++)
        for(Int_t iyhit = 0; iyhit <yhit; iyhit++)
            if (cut -> IsInside(ye[iyhit], xe[ixhit])){
                g[jj][xs[ixhit]][ys[iyhit]] -> Fill(xe[ixhit], ye[iyhit]); 
            }
}
void ana::Select(){
    for (Int_t i = 0; i < 3; i++){
        if(XY[i][0].empty()){
            MAX[i] = XY[i][1][0]; 
            j[i] = 1; 
        }
        else{
            MAX[i] = XY[i][0][0]; 
            j[i] = 0; 
        }
    }
    for(Int_t ii = 0; ii < 3; ii++){
        for(Int_t i = 0; i < 2; i++){
            for(Int_t x:XY[ii][i]){
                if(NUM[ii][i][x] > NUM[ii][j[ii]][MAX[ii]]){
                    MAX[ii] = x; 
                    j[ii] = i; 
                }
            }
        }
    }
}
void ana::FiT(){
    for(Int_t ii = 0; ii < 3; ii++){
        if(gD[ii][j[ii]][MAX[ii]] -> GetN() > 2){
            TF1 *fpol1 = new TF1("Fitpol1", "pol1", 0, 8000); 
            Double_t kl, bl; 
            gD[ii][j[ii]][MAX[ii]] -> Fit(fpol1, "SQ+", "RN"); 
            if(!j[ii]) {
                kl = 1 / (fpol1 -> GetParameter(1)); 
                bl = - (fpol1 -> GetParameter(0)) * kl; 
            }else{
                kl = fpol1 -> GetParameter(1); 
                bl = fpol1 -> GetParameter(0); 
            }
            k[ii][!j[ii]][MAX[ii]] = kl; 
            b[ii][!j[ii]][MAX[ii]] = bl; 
            ChiNdf[ii][!j[ii]][MAX[ii]] = (fpol1 -> GetChisquare()) / (fpol1 -> GetNDF()); 
            std::cout<<Form("k=%6f",k[ii][!j[ii]][MAX[ii]])<<"\t"<<Form("b=%6f",b[ii][!j[ii]][MAX[ii]])<<"\t"<<Form("chi/ndf=%6f",(fpol1 -> GetChisquare()) / (fpol1 -> GetNDF()))<<"\t"; 
            delete fpol1; 
        }
    }
    std::cout<<std::endl; 
}
void bSort(Int_t *xe, Int_t xhit, Int_t *xs){
    Int_t MIN; 
    for(Int_t i = 0; i < xhit - 1; i++)
        for(Int_t j = 0; j < xhit - 1 - i; i++)
            if(*(xe + j) < *(xe + j + 1)){
                MIN = *(xe + j); 
                *(xe + j) = *(xe + j + 1); 
                *(xe + j + 1) = MIN; 
                MIN = *(xs + j); 
                *(xs + j) = *(xs + j + 1); 
                *(xs + j + 1) = MIN; 
            }
}
void ana::WriTe(){
    std::cout<<"开始branch"<<std::endl; 
    Double_t pd1xe[13], pd1ye[13]; 
    Double_t pd2xe[13], pd2ye[13]; 
    Double_t pd3xe[13], pd3ye[13]; 
    Data -> Branch("d1xhit", &d1xhit, "d1xhit/I"); 
    Data -> Branch("d1yhit", &d1yhit, "d1yhit/I"); 
    Data -> Branch("d1xe", pd1xe, "d1xe[d1xhit]/D"); 
    Data -> Branch("d1ye", pd1ye, "d1ye[d1yhit]/D"); 
    Data -> Branch("d1xs", d1xs, "d1xs[d1xhit]/I"); 
    Data -> Branch("d1ys", d1ys, "d1ys[d1yhit]/I"); 
    Data -> Branch("d2xhit", &d2xhit, "d2xhit/I"); //
    Data -> Branch("d2yhit", &d2yhit, "d2yhit/I"); 
    Data -> Branch("d2xe", pd2xe, "d2xe[d2xhit]/D"); 
    Data -> Branch("d2ye", pd2ye, "d2ye[d2yhit]/D"); 
    Data -> Branch("d2xs", d2xs, "d2xs[d2xhit]/I"); 
    Data -> Branch("d2ys", d2ys, "d2ys[d2yhit]/I"); 
    Data -> Branch("d3xhit", &d3xhit, "d3xhit/I"); //
    Data -> Branch("d3yhit", &d3yhit, "d3yhit/I"); 
    Data -> Branch("d3xe", pd3xe, "d3xe[d3xhit]/D"); 
    Data -> Branch("d3ye", pd3ye, "d3ye[d3yhit]/D"); 
    Data -> Branch("d3xs", d3xs, "d3xs[d3xhit]/I"); 
    Data -> Branch("d3ys", d3ys, "d3ys[d3yhit]/I"); 
    std::cout<<"开始写入Data"<<std::endl; 
    for(Long64_t j = 0; j < Nentry; j++){
        fChain -> GetEntry(j); 
        bSort(d1xe, d1xhit, d1xs); 
        for(Int_t i = 0; i < d1xhit; i++)    pd1xe[i] = k[0][0][d1xs[i]] * d1xe[i] + b[0][0][d1xs[i]]; 
        bSort(d1ye, d1yhit, d1ys); 
        for(Int_t i = 0; i < d1yhit; i++)    pd1ye[i] = k[0][1][d1ys[i]] * d1ye[i] + b[0][1][d1ys[i]]; 
        bSort(d2xe, d2xhit, d2xs); 
        for(Int_t i = 0; i < d2xhit; i++)    pd2xe[i] = k[1][0][d2xs[i]] * d2xe[i] + b[1][0][d2xs[i]]; 
        bSort(d2ye, d2yhit, d2ys); 
        for(Int_t i = 0; i < d2yhit; i++)    pd2ye[i] = k[1][1][d2ys[i]] * d2ye[i] + b[1][1][d2ys[i]]; 
        bSort(d3xe, d3xhit, d3xs); 
        for(Int_t i = 0; i < d3xhit; i++)    pd3xe[i] = k[2][0][d3xs[i]] * d3xe[i] + b[2][0][d3xs[i]]; 
        bSort(d3ye, d3yhit, d3ys); 
        for(Int_t i = 0; i < d3yhit; i++)    pd3ye[i] = k[2][1][d3ys[i]] * d3ye[i] + b[2][1][d3ys[i]]; 
        Data -> Fill(); 
    }
    /*
    std::cout<<"开始写入kbChi/ndf"<<std::endl; 
    std::ofstream *file = new std::ofstream("homework6.csv", std::ios::trunc); 
    //file -> open("example.csv"); 
    for(Int_t ii = 0; ii < 3; ii++){
        *file << Form("DSSD%i", ii + 1) <<std::endl; 
        *file << "k" << "," << "b" << "," << "Chi/ndf" << std::endl; 
        for(Int_t i = 0; i < 2; i++){
            for(Int_t x = 0; x < 32 ; x++){
                *file << k[ii][i][x] << "," << b[ii][i][x] << "," << ChiNdf[ii][i][x] << std::endl; 
            }
        }
    }
    file -> close(); */
}