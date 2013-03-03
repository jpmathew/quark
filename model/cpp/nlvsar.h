#ifndef NLVSAR_H
#define NLVSAR_H

#include "cdac.h"
#include "nlvco.h"
#include "vernierRing.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>

class nlvsar
{
	private:
		double refp,refm;
		cdac *cdacP,*cdacM;
		int *add,*sub;
		nlvco *vcoP,*vcoM;
		vernierRing* rring;
		double *voff;
		int *numBits,*startPos,*numLoops;
		int code;
		int *bitWeight;
		double *residue;
		double est;
		int iEst;
		int code16;
		int th;
		int sgn;
		double dither;
	public:
	nlvsar(double refp,double refm);
	void sample(double vinp,double vinm);
	int convert(int dith);
	double getResidue(int stage);
	double getEst();
	void rstEst();
	int getIEst();
	int getCode();
	int getTh();
	int getSgn();
	double getDither();
	int getAdd(int pos);
	~nlvsar();
};
#endif
