#ifndef CDAC_H
#define CDAC_H
#include <cmath>
#include <iostream>
using namespace std;

class cdac
{
	private:
		int capCount;
		int samplingCapCount;
		double totSamplingCap;
		double* capWeight;
		double* capVoltage;
		double topPlateVoltage;
		double sampledVoltage;
	public:
		cdac();
		cdac(int errcount,double* err);
		cdac(int capCount,int samplingCapCOunt,double* capWeight);
		void sample(double vin,double refp,double refm);
		void calibSample(double refp,double refm,int prcode);
		double changeBotPlateVoltage(double botVoltage,int botPosition);
		double getTopPlateVoltage();
		void printBotstat();
		~cdac();
};
#endif
