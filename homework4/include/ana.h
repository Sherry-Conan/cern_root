#ifndef ana_h
#define ana_h

#include"tracking.h"
#include<TCanvas.h>
#include<TH2D.h>
#include<TF1.h>
#include<TGraph.h>

class ana{
	private:  
	tracking track; 
	Int_t    k_x, k_y; 
	Double_t x[5], y[5], z_x[5], z_y[5], t[5], Fitxb, Fitxk, Fityb, Fityk, check[5], ndfy, ndfx, chix, chiy, dx[5], dy[5]; 
	public: 
	void     Loop(TTree *Data, TH2D *hForzx, TH2D *hForzy); 
	Int_t    Check(Double_t *p); 
   	void     SetBranch(TTree *Data); 
   	void     aTrace(TH2D *h, Double_t k, Double_t b, Int_t max = 0, Int_t min = -1800); 
   	Bool_t   calculate(TH2D *hForzx, Int_t k, Double_t *p, Double_t *pz, Double_t *pdx, Double_t *pFitxb, Double_t *pFitxk, Double_t *pchix, Double_t *pndfx); 
}; 

#endif