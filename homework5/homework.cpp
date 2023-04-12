#include"ana.h"

int main(){
	TFile *File = new TFile("calibration.root", "recreate"); 
	TTree *tree = new TTree("Adjust", "Adjust"); 
	TTree *FitData = new TTree("FitData", "FitData"); 
	TDirectory* myFolder1 = (TDirectory*) File -> mkdir("hForFill", "hForFill"); 

	TFile *hFile = new TFile("SaveGause.root", "recreate"); 
	TDirectory* myFolder2 = (TDirectory*) hFile -> mkdir("hForGaus", "hForGaus"); 
	TTree *Data = new TTree("D_Pe48", "calibration"); 

	ana *ana1 = new ana(); 
	ana1 -> forcalibration(File, hFile, Data, myFolder1, myFolder2); 
	ana1 -> WriteDataTree(tree, FitData); 

	hFile -> WriteObject(Data, "D_Pe48"); 
	File -> WriteObject(tree, "Adjust"); 
	File -> WriteObject(FitData, "FitData"); 
	hFile -> Close(); 
	File -> Close(); 
}
