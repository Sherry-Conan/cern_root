#include"ana.h"

int main(){
	TFile *File = new TFile("track.root", "recreate"); 
	TTree *Data = new TTree("tree", "ppac traking"); 
	TH2D *hForzx = new TH2D("hForzx","Fit z_{x} to x by ppac1A, ppac2A and ppac3",2200, -2000, 200, 300, -150, 150); 
	TH2D *hForzy = new TH2D("hForzy","Fit z_{y} to y by ppac1A, ppac2A and ppac3",2200, -2000, 200, 300, -150, 150); 
	ana ana; 
	ana.Loop(Data, hForzx, hForzy); 
	File -> WriteObject(Data, "Data", "ppac tracking"); 
	File -> WriteObject(hForzx, "hForzx"); 
	File -> WriteObject(hForzy, "hForzy"); 
	File -> Close(); 
	return 0; 
}