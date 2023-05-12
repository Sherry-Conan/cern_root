#include"ana.h"
int main(){
    TFile *File = new TFile("DSSD.root", "recreate"); 
	TDirectory* hFolder = (TDirectory*) File -> mkdir("DSSD", "DSSD"); 
    TTree *Data = new TTree("Data", "Data"); 

    ana *ana1 = new ana(); 
    ana1 -> FitPol(Data, hFolder); 
    File -> WriteObject(Data, "Data"); 
    File -> Close(); 
    return 0; 
}