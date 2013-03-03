#ifndef ADC_H
#define ADC_H
#include "cdac.h"

class adc
{
	private:
		cdac *cdacP,*cdacM;
		int add[12],sub[12];
		int ea[12],eb[12];
		int ebal[2];
		int enableThreshCal;
		double threshErr[12];
		double w[2][12];
		const static double refp=0.9;
		const static double refm=0.0;
	public:
		adc();
		void sample(double vinp,double vinm);
		void calibSample(int prcode);
		int convert();
		int calibConvert(int bitUnderCal,int config);
		double residue(int bitPosition);
		void autoCapCalib();
		void autoIndCapCalibDither(int cap,int type);
		void printCoe();
		double threshEst(int bitPos);
		void setThreshCal(int enable);
		void setCoe(int type,int pos,int val);
		~adc();
};

		
#endif
