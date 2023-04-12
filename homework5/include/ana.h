#include"calibration.h"
#include<TRandom.h>
#include<TGraph.h>
#include<TH1D.h>
const Double_t x[] = {8.6931, 6.6708, 6.1748, 5.5685}; //拟合用的峰值，从大到小排列，只要峰值不少于三个就可以进行计算
const Int_t nn = sizeof(x) / sizeof(x[0]);; //拟合用的峰数
const Int_t n = 48; 
class ana : private calibration{
	private:
	Int_t Ope = 0, L = 0; //n就是算几个pe，Ope是选择几个pe，L是半高宽的序号
	Double_t Dpe[48]; //Double_t 的pe
	TFile *File = NULL; //为了让主函数生成的File能成为类的一部分不需要传递
	TFile *hFile = NULL; //同上
	TTree *Data = NULL; //这个是tree，主要是装连续化的tree，防止拟合用的和输出用的不一致，其他同上
	Double_t k[48], b[48]; //就kx+b的k和b
	TGraph *FWHM[nn]; //依次装半高宽
	TRandom *gr = new TRandom(0); //提供随机数，全用这个提供
	TDirectory* myFolder1 = NULL; 
	TDirectory* myFolder2 = NULL; 
	TGraph *kbCn = new TGraph(); 
	TH1D *h[48]; 
	public:
	void forcalibration(TFile *file, TFile *hfile, TTree *tData, TDirectory* my1, TDirectory* my2); 
	TGraph* SEarch(Double_t *xpeaks,Double_t *ypeaks, Int_t n, Int_t i); 
	Double_t Adjust(TGraph *g, TH1D *h, Int_t ii); 
	void WriteDataTree(TTree *tree, TTree *FitData); 
	void Diedai(TGraph *g,std::vector<Int_t> list_i[nn], Int_t l, Int_t ysize, std::vector<Double_t> y[2],  Int_t i[nn], std::vector<Double_t> *CHn); 
}; 