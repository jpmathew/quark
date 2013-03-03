#ifndef DAC_H
#define DAC_H
class dac
{
	private:
	int bitCount;
	int* smplReg;
	int* bitReg;
	int* bitWeight;
	int  totWeight;
	double VREF;
	double vin;
	public:
	dac(double VREF,int resolution);
	void sample(double vin);
	void setReg(int pos,int val);
	double getVal();
	~dac();
};
#endif
