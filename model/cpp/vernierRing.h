#ifndef VERNIERRING_H
#define VERNIERRING_H
#include <cmath>
#include <iostream>
#include <cstdlib>
class vernierRing
{
	private:
	double tf,ts;
	int COUNTMAX;
	int delLength;
	double *delp;
	double *corrp;
	double *delm;
	double *corrm;
	public:
	vernierRing(double ts,double tf,int delLength);
	vernierRing(int delLength);
	~vernierRing();
	void resolveTime(double tIn,int *add,int *sub,int length);
	void setTime(double ts,double tf);
	void setCorr(double corr,int pm,int pos);
	double getCorr(int pm,int pos);
};
#endif

