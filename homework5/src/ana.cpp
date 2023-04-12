#include"ana.h"
#include<TH1D.h>
#include<TSpectrum.h>
#include<TF1.h>
#include<iostream>
#include<map>
#include<TMath.h>
#include<vector>
#include<TLatex.h>
#include<sstream>
void ana::forcalibration(TFile *file, TFile *hfile, TTree *tData, TDirectory* my1, TDirectory* my2){
	File = file; 
	hFile = hfile; 
	Data = tData; //把传进来的变量，类里面不需要传递就能使用
	myFolder1 = my1; 
	myFolder2 = my2; 
	Data -> Branch("pe", Dpe, "pe[48]/D"); 
	for(Int_t i = 0; i < n; i++){
		h[i] = new TH1D(Form("%ih", i), Form("%ih", i), 1000, 0, 2000); 
	}
	for(Int_t j = 0; j < fChain -> GetEntries(); j++){
		fChain -> GetEntry(j); 
		for(Int_t i = 0; i < n; i++){
			Dpe[i] = pe[i] + gr -> Uniform(0.0, 1.0); //装Data
			h[i] -> Fill(Dpe[i]); //装用于拟合的TH1D
		}
	}
	for(Int_t i = 0; i < 4; i++) FWHM[i] = new TGraph(); 
	for(Int_t i = 0; i < n; i++){
		Double_t *xpeaks,*ypeaks; 
		TSpectrum *Sp = new TSpectrum(500); 
		Int_t nfound = Sp -> Search(h[i], 2, ""); 
		xpeaks = Sp -> GetPositionX(); 
		ypeaks = Sp -> GetPositionY(); //寻峰
		TGraph* g = SEarch(xpeaks, ypeaks, nfound, i); //搜索我想要的五个点，就是最右面的五个峰位应该要返回一个TGraph吧
		kbCn -> SetPoint(i, i, Adjust(g, h[i], i)); 
		std::cout<<kbCn -> GetY()[i]<<","<<k[i]<<std::endl; //用gaus拟合一下，来找到更好的峰位
		myFolder2 -> WriteObject(h[i], Form("h%i", i)); //装函数
		delete Sp; 
	}
}
/*TGraph *ana::SEarch(Double_t *xpeaks, Double_t *ypeaks, Int_t nf){//搜索我想要的五个点，就是最右面的五个峰位会返回一个TGraph吧
	TGraph *g  = new TGraph(); 
	std::map<Double_t, Long64_t> m; 
	for(Int_t i = 0; i < nf; i++)	m.insert(std::pair<Double_t, Long64_t>(xpeaks[i], ypeaks[i])); 

	Int_t j = 0;  
	for(auto im = m.rbegin(); im != m.rend(); im++){
		if(im ->second > 400){
			if((j != 3) || (im -> first < 995)) g -> AddPoint(im ->first, im ->second); 
			if((j == 3) && (im -> first < 995)) j++; 
			j++; 
		}
		if(j == 5) break; 
	}
	return g; 
}*/
TGraph *ana::SEarch(Double_t *xpeaks, Double_t *ypeaks, Int_t nf, Int_t ii){//第二种search方式，比较普世化，上一个是根据这次的图形进行特别处理的
	Int_t i[nn] ={0, 1, 2, 3}; 
	std::vector<Double_t> y[2]; 
	std::map<Double_t, Long64_t> m; 
	for(Int_t i = 0; i < nf; i++)	m.insert(std::pair<Double_t, Long64_t>(xpeaks[i], ypeaks[i])); 
	for(auto im = m.rbegin(); im != m.rend(); im++){
		if(im ->second > 50) {//给的峰不会太小
			std::stringstream ss; 
			y[0].push_back(im -> first); 
			y[1].push_back(im -> second);  
	        ss<<im -> first; 
	        TString s1 = ss.str(); 
	        TLatex *tex=new TLatex(im -> first,im -> second,s1); 
			tex->SetTextFont(13);
            tex->SetTextSize(14);
            tex->SetTextAlign(12);
            tex->SetTextAngle(90);
            tex->SetTextColor(kRed);
        	h[ii]->GetListOfFunctions()->Add(tex); 
            tex->Draw(); 
		}//先将xpeaks重排，这样能大幅度减少穷举的数量。
	}
	std::vector<Double_t> *CHn = new std::vector<Double_t>; 
	std::vector<Int_t> list_i[nn]; 
	TGraph *g  = new TGraph(nn, x, x); 
	Int_t ysize = y[0].size(); 
	/*for(i[0] = 0; i[0] < ysize - nn + 1; i[0]++){//穷举找最小的chi/ndf
		g -> SetPoint(0, y[0][i[0]], x[0]); 
		for(i[1] = 1 + i[0]; i[1] < ysize - nn + 2; i[1]++){
			g -> SetPoint(1, y[0][i[1]], x[1]); 
			for(i[2] = 1 + i[1]; i[2] < ysize - nn + 3; i[2]++){
				g -> SetPoint(2, y[0][i[2]], x[2]); 
				for(i[3] = 1 + i[2]; i[3] < ysize - nn + 4; i[3]++){
					g -> SetPoint(3, y[0][i[3]], x[3]); 
					TF1 *fpol1 = new TF1("Fitpol1", "pol1", 0, 2000); 
					g -> Fit(fpol1, "SQ+", "RN"); 
					CHn.push_back(fpol1 -> GetChisquare()/fpol1 -> GetNDF()); 
					delete fpol1; 
					for(Int_t j = 0; j < nn; j++)	list_i[j].push_back(i[j]); 
				}
			}
		}
	}*/
	Diedai(g, list_i, nn, ysize, y, i, CHn); 
	//改成一个迭代，这样就能处理任意个点了。
	Double_t minCHn = 100000.; 
	ii = 0; 
	Int_t listi = list_i[0].size(); 
	for(Int_t j = 0; j < listi; j++){//冒泡法找最小chi/ndf的数据
		if((*CHn)[j] < minCHn){
			minCHn = (*CHn)[j]; 
			ii = j; 
		}
	}
	for(Int_t j = 0; j < nn; j++){
		g->SetPoint(j, y[0][list_i[j][ii]], y[1][list_i[j][ii]]); 
	}
	return g; 
}
Double_t ana::Adjust(TGraph *g, TH1D *h, Int_t ii){//调整道址成能量
	TGraph *gForAdjust = new TGraph(); 
	Double_t mean[3], sigma[3], chindf[3]; 
	for(Int_t i = 0; i < nn; i++){
		for(Int_t j = 0; j < 3; j++){
    		TF1 *f = new TF1(Form("fg%d",i),"gaus"); 
			f -> SetParameter(0, Double_t(g -> GetPointY(i))); //constant
			f -> SetParameter(1, Double_t(g -> GetPointX(i))); //mean
			h -> Fit(f, "SQ+", "RN", g -> GetPointX(i) - 5 - j, g -> GetPointX(i) + 8 - j); 
			mean[j] = f -> GetParameter(1); 
			sigma[j] = f -> GetParameter(2); 
			chindf[j] = f -> GetChisquare()/f -> GetNDF(); 
			delete f; 
		}
		Double_t minCHn = 100000.; 
		Int_t jj = 0; 
		for(Int_t j = 0; j < 3; j++){//冒泡法找chi/ndf最小值对应的数据
			if(chindf[j] < minCHn){
				minCHn = chindf[j];  
				jj = j; 
			}
		}
		gForAdjust -> AddPoint(mean[jj], x[i]); 
		FWHM[i] -> AddPoint(L, 2 * TMath::Sqrt(2 * TMath::Log(2)) * (sigma[jj])); 
	}
	L++; 
	TF1 *fpol1 = new TF1("FitxLocation", "pol1", 0, 2000); 
	gForAdjust -> Fit(fpol1, "SQ+", "RN"); 
	k[ii] = fpol1 -> GetParameter(1); 
	b[ii] = fpol1 -> GetParameter(0); 
	return fpol1 -> GetChisquare()/fpol1 -> GetNDF(); 
}
void ana::WriteDataTree(TTree *tree, TTree *FitData){//把量写入传递进来的File
	Int_t PID[n]; 
	tree -> Branch("Ope", &Ope, "Ope/I"); 
	tree -> Branch("pe", Dpe, "pe[Ope]/D"); 
	tree -> Branch("PID", PID, "PID[Ope]/I"); 
	Double_t Fitpe[48]; 
	FitData -> Branch("Ope", &Ope, "Ope/I"); 
	FitData -> Branch("Fitpe", Fitpe, "Fitpe[Ope]/D"); 
	FitData -> Branch("PID", PID, "PID[Ope]/I"); 
	TH1D *hforFillI[48]; 
	for(Int_t i = 0; i < n; i++)	hforFillI[i] = new TH1D(Form("%ihforFill", i), Form("%ihforFill", i), 1000, 0, 12); 
	for(Int_t j = 0; j < fChain -> GetEntries(); j ++){
		Ope = 0; 
		fChain -> GetEntry(j); 
		for(Int_t i = 0; i < n; i++) {
			if(pe[i] != -10){
				Dpe[Ope] = pe[i] + gr -> Uniform(); 
				Fitpe[Ope] = k[i] * Dpe[Ope] + b[i]; 
				PID[Ope] = i; 
				hforFillI[i] -> Fill(Fitpe[Ope]); 
				Ope++; 
			}
		}
		if(Ope != 0){
		tree -> Fill(); 
		FitData -> Fill(); 
		}
	}
	TH1D *hFull = new TH1D("hFull", "hFull", 1000, 0, 12); 
	for(Int_t i = 0; i < n; i++){
		hFull -> Add(hFull, hforFillI[i], 1, 1); 
		myFolder1 -> WriteObject(hforFillI[i], Form("hforFill%i", i)); 
	}
	File -> WriteObject(hFull, "AllAdjusted"); 
	for(Int_t j = 0; j < nn; j++) {
		for(Int_t jj = 0; jj < n; jj++){
			FWHM[j] -> SetPoint(jj, jj, k[jj] * (FWHM[j] -> GetY()[jj])); 
		}
		File -> WriteObject(FWHM[j], Form("FWHM%i", j)); 
	}
	File -> WriteObject(kbCn, "kbFitChiNdf"); 
}
void ana::Diedai(TGraph *g,std::vector<Int_t> list_i[nn], Int_t l, Int_t ysize, std::vector<Double_t> y[2], Int_t i[nn], std::vector<Double_t> *CHn){
	Int_t ll; 
	if(nn != l)	ll = 1 + i[nn - l - 1]; 
	else ll = 0; 
	if(l != 1){
		for(i[nn - l] = ll; i[nn - l] < ysize - l + 1; i[nn - l]++){
			g -> SetPoint(nn - l, y[0][i[nn - l]], x[nn - l]); 
			Diedai(g, list_i, l - 1, ysize, y, i, CHn); 
		}
	}
	else{
		for(i[nn - l] = ll; i[nn - l] < ysize - l + 1; i[nn - l]++){
			g -> SetPoint(nn - l, y[0][i[nn - l]], x[nn - l]); 
			TF1 *fpol1 = new TF1("Fitpol1", "pol1", 0, 2000); 
			g -> Fit(fpol1, "SQ+", "RN"); 
			CHn -> push_back(fpol1 -> GetChisquare()/fpol1 -> GetNDF()); 
			delete fpol1; 
			for(Int_t j = 0; j < nn; j++)	list_i[j].push_back(i[j]); 
		}
	}
}