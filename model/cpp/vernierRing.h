#ifndef VERNIERRING_H
#define VERNIERRING_H
#include <cmath>
#include <iostream>
class vernierRing
{
	private:
	double tf,ts;
	int COUNTMAX;
	public:
	vernierRing(double ts,double tf);
	void resolveTime(double tIn,int *add,int *sub,int length);
	void setTime(double ts,double tf);
};
#endif

