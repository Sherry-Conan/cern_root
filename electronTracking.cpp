#include "TCanvas.h"
#include "TH1I.h"
#include<iostream>
#include "TRandom.h"
#include "TH1.h"
#include "TMath.h"
#include "TLorentzRotation.h"
#include "TGraph.h"
#include "TBenchmark.h"
#include "Math/RootFinder.h"

using namespace std;
//抄一下常数
const double e0 = 1;               // initial particle energy, eV
const double meanLossRatio = 2.7 * 1.e-4;    
const double density = 1.663e-4; // g/cm^3
const double aMass = 4.002602; // g/mole
const double nd = density / aMass * TMath::Na() ; // the atom number per cm^3
const double xsec = 7e-16; // cm^2, i.e., 7 A
const double meanFreePath = 1./(nd * xsec); //cm
const double me = 0.511e6/TMath::C()/TMath::C(); // eV/(m/s)^2
const double eMass = 9.10956e-31; // kg
const double eCharge = 1.602e-19; // C
											
const double ms2cmus = 1.e-4;
//const unsigned int maxStepNumber = 1e5;
const unsigned int maxStepNumber = 100;

const TVector3 eFieldDir(0., 0., 1.);

class class_root: public ROOT::Math::IGenFunction{//这玩意还得放前面
    private:
        double fVX;
        double fVY;
        double fVZ;
        double fSL;
        double fEM;
    public:
        void SetParas(double vx, double vy, double vz, double sl, double em)
        {
            fVX = vx;
            fVY = vy;
            fVZ = vz;
            fSL = sl;
            fEM = em;
        }

        double operator() ( double t ) const
        {
            return DoEval(t);
        }

        double DoEval(double t) const
        {
            double acc = fEM * eCharge / eMass * 1.e-8; // cm/us^2
    #if 1//改一为0会运行else
            double sx = fVX * t;
            double sy = fVY * t;
            double sz = fVZ * t + 0.5 * acc * t * t;
            return sqrt( sx * sx + sy * sy + sz * sz) - fSL;//为了求解dt
    #else
            double a = fVX * fVX + fVY * fVY + fVZ * fVZ;
            double b = 2 * acc * fVZ;
            double c = acc * acc;

            double t1 = asinh( (2 * c * t + b)/sqrt(4 * a * c - b * b) );
            double t2 = sqrt(c * t * t + b * t + a);

            double calcL = a * t1/(2 * sqrt(c)) - b * b * t1/(8 * sqrt(c*c*c)) + t * t2/2 + b*t2/(4*c);

            return calcL - fSL;
    #endif
        }

        class_root* Clone() const
        {
            return 0;
        }
};

TLorentzVector shoot_particle(int i, double ef_strength_i){
    gRandom->SetSeed(i);
    double PI=TMath::Pi();
    double particle_energy = e0;
    double e_field_mag = ef_strength_i; // V/cm
    double vMag = sqrt(2 * e0 / me);
    vMag = vMag * ms2cmus; // cm/us 
    double t = 0.;

    TVector3 v0_dir(0., 1., 0.);//生成电子的初始速度
    double vx = vMag * v0_dir.X();
	double vy = vMag * v0_dir.Y();
	double vz = vMag * v0_dir.Z();

    TVector3 position(0., 0., 0.);//位置

    class_root f;

    ROOT::Math::RootFinder  root;//用来求解函数的根

    root.SetFunction(f, 0.0, 0.001);
    for(i=0;i<maxStepNumber;i++){
        double theta=gRandom->Uniform( -PI , PI);
        double phi   = gRandom->Uniform(0., 2. * PI);
        //得先算一下电场
        double step_length = gRandom->Exp(meanFreePath);//撞击粒子走过的步长
        f.SetParas(vx, vy, vz, step_length, e_field_mag);
        root.Solve();
        double dt=abs(root.Root());
        double a_e=e_field_mag * eCharge / eMass * 1.e-8;//加速度
        TVector3 dl = TVector3(vx * dt, vy * dt, vz * dt + 0.5 * a_e * dt * dt);//电场作用下移动的位置
        position +=dl;
        vz = vz + a_e * dt;
        t += dt;
        //粒子碰撞
        TRotation rotate_mat;//旋转矩阵
        rotate_mat.RotateY(theta);
        rotate_mat.RotateY(phi);

        TVector3 v_dir(vx,vy,vz);//TVector3居然不能直接改值
		v_dir = rotate_mat * v_dir;
		v_dir = v_dir.Unit();
        		double energyLossRatio = meanLossRatio * (1 - cos(theta));

		double collisionEnergyLoss = energyLossRatio * particle_energy; // eV
		double eFieldWork = eFieldDir * e_field_mag * dl; // eV

		particle_energy = particle_energy - collisionEnergyLoss + eFieldWork; // eV
																			
		// check energy
		if(particle_energy <= 0) 
		{
			cout << "restart energy" << endl;
			particle_energy = 1.e-2; //restart? eV
		}

		// ----------------------- new velocity
		vMag = sqrt(2 * particle_energy / me); // m/s
		vMag = vMag * ms2cmus; // cm/us 

		vx = vMag * v_dir.X();
		vy = vMag * v_dir.Y();
		vz = vMag * v_dir.Z();
	}

	TLorentzVector v(position.X(), position.Y(), position.Z(), t);

	return v;
    
}

double one_electron_tracking(double ef_strength_i){
    TH1I* x = new TH1I("x", "x", 100, -0.2, 0.2);
	TH1I* y = new TH1I("y", "y", 100, -0.2, 0.2);
	TH1I* z = new TH1I("z", "z", 100, -0.2, 0.2);
	TH1I* t = new TH1I("v", "v", 100, 0, 2);

    gBenchmark->Start("simpleTracking");//记录计算时长

    for(int j = 0; j < 1000; j++){
		TLorentzVector a = shoot_particle(j, ef_strength_i);
	    x->Fill( a.X() );
	    y->Fill( a.Y() );
	    z->Fill( a.Z() );
	    t->Fill( a.Z()/a.T() );
		if(j % 100 == 0) cout << (int)j << " done" << '\t';//双引号单引号不同
	}
    cout<<endl;
    gBenchmark->Show("simpleTracking");

    double vd = t->GetMean();

    delete x;
    delete y;
    delete z;
    delete t;

    return vd;
}

double electron_random(int i){//根据不同种子取theta和phi的值,每一次都取种子太慢了，这个函数就不用了
    gRandom->SetSeed(i);
    double theta = acos(gRandom->Uniform(-1, 1));//acos()和数学的arcos()一样
	double phi   = gRandom->Uniform(0., 2. * TMath::Pi());//在2pi内取值
    return theta,phi;
}

void electronTracking(){
    const int ef_NUM=15;//计算的电场个数
    double ef_strength[ef_NUM];//储存电场强度
	double v_drift[ef_NUM];//储存漂移速度

    for(int i=0;i<ef_NUM;++i){
        ef_strength[i]=(i+1)*100;//一定要加一，否则求值不下去。
        cout<<ef_strength;
        v_drift[i]=one_electron_tracking(ef_strength[i]);
    }

    TGraph* g = new TGraph(ef_NUM, ef_strength, v_drift);

	g->SetTitle("Electron drift velocity in Helium");

	g->GetXaxis()->SetTitle("Electric field (V/cm)");
	g->GetYaxis()->SetTitle("Drift velocity (cm/#mus)");
	g->SetLineColor(2);
	g->Draw("AC");
}